#include <SDL/SDL.h>
#include <SDL/SDL_net.h>
#include "SDL/SDL_gfxPrimitives.h"

#include <ctime>
#include <iostream>

#include "Game.h"
#include "JoinMessage.h"
#include "MoveUnitOrder.h"
#include "PlayerTurnMessage.h"

using namespace std;

const int Game::ticksperturn = 400; // For the deterministic simulation

const int Game::turndelay = 2; // Delay between turn creation and execution

Game::Game(bool _hosting, IPaddress address)
	: hosting(_hosting), numplayers(1), random(NULL), xdown(0), ydown(0), xup(0),
		yup(0), moved(0), mousex(0), mousey(0), start(0), turn(NULL),
		view(0,0,0,0,0) {
	// Prepare the connection
	Uint16 port = SDLNet_Read16(&address.port);
	if(hosting) { // Hosting a game
		setPlayerId(0); // The host is always player 0

		setSeed(time(NULL)); // Always a unique game

		if(socket = SDLNet_UDP_Open(port), !socket) {
			cerr << "error: cannot open UDP socket on port "
				<< port << endl;
		}

		cout << "listening for join request on port " << port << endl;
	} else { // Joining a game
		if(socket = SDLNet_UDP_Open(0), !socket) {
			cerr << "error: cannot open anonymous UDP socket"
				<< endl;
			return;
		}

		SDLNet_UDP_Bind(socket,0,&address);

		sendMessage(new JoinMessage());
	}
	viewVelocity_x = 0;
	viewVelocity_y = 0;
}

Game::~Game() {
	if(socket) SDLNet_UDP_Close(socket);
	if(random) delete random;
}

void Game::setPlayerId(Uint8 _playerid) {
	if(numplayers <= _playerid) numplayers = _playerid + 1;

	playerid = _playerid;
}

void Game::setSeed(Uint32 seed) {
	// Only do this once
	if(random) {
		cerr << "warning: attempt to overwrite the random number"
			" generator" << endl;
		return;
	}

	random = new Random(seed);
}

// Main game loop
void Game::play() {
	playing = true;

	while(playing) {
		handleEvents();
		handleMessages();

		broadcastTurn();

		sendMessages();

		executeTurns();

		draw();
		
		// Max out at 60 fps
		if(SDL_GetTicks() - lastframe < 16)
			SDL_Delay(16-(SDL_GetTicks() - lastframe));
		lastframe = SDL_GetTicks();

		SDL_Delay(100); // Don't hog the CPU
	}
}

// Add another player to the game
Uint8 Game::addPlayer() {
	return numplayers++;
}

// Add a player's turn to the queue
void Game::addPlayerTurn(PlayerTurn *playerturn) {
	// Make sure the turn is in the queue
	if(turnqueue.size()) { // Empty?
		int turnid = turnqueue.front()->getTurnId()
			+ turnqueue.size() - 1;

		while(turnid < playerturn->getTurnId())
			turnqueue.push_back(new Turn(++turnid));
	} else turnqueue.push_back(new Turn(playerturn->getTurnId()));

	// Search for the appropriate spot in the queue
	list<Turn *>::iterator turniter = turnqueue.begin();
	while(turniter != turnqueue.end()) {
		if(playerturn->getTurnId() == (*turniter)->getTurnId()) {
			(*turniter)->addPlayerTurn(playerturn);
			return;
		}

		turniter++;
	}
}

// Add message to messagequeue
void Game::sendMessage(Message *message) {
	messagequeue.push(message);
}

// Process whatever SDL throws at us
void Game::handleEvents() {
	SDL_PumpEvents();

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			// Quick-exit key
			if(event.key.keysym.sym == SDLK_DELETE)
				playing = false;

			// Just for testing the MoveUnitOrder class
			if(event.key.keysym.sym == SDLK_m)
				turn->addOrder(new MoveUnitOrder(0,
					rand() & 0xFFFF,rand() & 0xFFFF));
			// Scrolling around the map
			if(event.key.keysym.sym == SDLK_UP)
				viewVelocity_y += -1;
			if(event.key.keysym.sym == SDLK_DOWN)
				viewVelocity_y += 1;
			if (event.key.keysym.sym == SDLK_LEFT)
				viewVelocity_x += -1;
			if(event.key.keysym.sym == SDLK_RIGHT)
				viewVelocity_x += 1;
			break;

		case SDL_KEYUP:
			//For continuous scrolling around the map
			//View wont change when you release keys
			if(event.key.keysym.sym == SDLK_UP)
                                viewVelocity_y = 0;
                        if(event.key.keysym.sym == SDLK_DOWN)
                                viewVelocity_y = 0;
                        if (event.key.keysym.sym == SDLK_LEFT)
                                viewVelocity_x = 0;
                        if(event.key.keysym.sym == SDLK_RIGHT)
                                viewVelocity_x = 0;
			break;

		case SDL_MOUSEBUTTONDOWN:
			// Zoom in and out
			if(event.button.button == SDL_BUTTON_WHEELUP) {
				SDL_Surface *surface = SDL_GetVideoSurface();

				if(view.zoom < 100) view.zoom++;
				view.w = (surface->w + view.zoom - 1)
					/view.zoom;
				view.h = (surface->h + view.zoom - 1)
					/view.zoom;
			}

			if(event.button.button == SDL_BUTTON_WHEELDOWN) {
				SDL_Surface *surface = SDL_GetVideoSurface();

				if(view.zoom > 1) view.zoom--;
				view.w = (surface->w + view.zoom - 1)
					/view.zoom;
				view.h = (surface->h + view.zoom - 1)
					/view.zoom;
			}
			//store the click locations and toggle moved to 1, drawing the selection square
			if(event.button.button == SDL_BUTTON_LEFT ) {
                        	xdown = event.button.x;         
                        	ydown = event.button.y;
                        	moved = 1;
                	}
			break;

		case SDL_MOUSEBUTTONUP:
			// Toggle moved to 0, to stop drawing the selection box
			if(event.button.button == SDL_BUTTON_LEFT) {
                        	moved = 0;
                	}
			break;
	
		case SDL_MOUSEMOTION:
			// Keep track of mouse location
			mousex = event.motion.x;
                	mousey = event.motion.y;
			break;
		}
	
	}

}

// Process anything we receive on the network
void Game::handleMessages() {
	// Have storage for a packet ready
	Uint8 data[0xFFFF];
	UDPpacket packet;

	packet.data = data;
	packet.maxlen = 0xFFFF;

	// Process packets, one by one
	while(SDLNet_UDP_Recv(socket,&packet) == 1) {
		Message message(packet);
		message.handle(*this);
	}
}

// Send our turn out to the other players, if ready
void Game::broadcastTurn() {
	// Sanity check
	if(!start) return;

	// Is this our first turn?
	if(!turn) turn = new PlayerTurn(0,playerid);

	int turnid = (SDL_GetTicks() - start)/ticksperturn;
	if(turnid > turn->getTurnId()) {
		// Tell every one about this
		addPlayerTurn(turn);
		sendMessage(new PlayerTurnMessage(turn));

		// Time for a new turn
		turn = new PlayerTurn(turn->getTurnId() + 1,playerid);
	}
}

// Send out the messages in messagequeue over the network
void Game::sendMessages() {
	Uint8 data[0xFFFF];
	UDPpacket packet;

	packet.data = data;
	packet.maxlen = 0xFFFF;

	while(messagequeue.size()) {
		Message *message = messagequeue.front();
		messagequeue.pop();

		message->getPacket(packet);
		delete message;

		SDLNet_UDP_Send(socket,0,&packet);
	}
}

// Advance, if appropriate, the fully-determined part of the simulation
void Game::executeTurns() {
	// Have we started actual gameplay, yet?
	if(!start) {
		// Should we?
		if(numplayers > 1) {
			cout << "game has at least two players; starting play"
				<< endl;

			// Generate the map
			map = new Map(65,random);

			// Prepare the view
			SDL_Surface *surface = SDL_GetVideoSurface();
			view = View(0,0,(surface->w + 15)/16,
				(surface->h + 15)/16,16);

			start = SDL_GetTicks();
		}
		return;
	}

	int turnid = (SDL_GetTicks() - start)/ticksperturn;

	// Sanity check
	if(turnid > turndelay && !turnqueue.size()) {
		cerr << "warning: empty turn queue" << endl;
	}

	// Execute as many turns as we can
	Turn *turn;
	while(turn = turnqueue.front(),
		turn && turnid >= turn->getTurnId() + turndelay) {
		// Go no further if we're missing data from someone
		if(turn->getPlayerCount() < numplayers) {
			cerr << "warning: possible dropped packet(s) for turn "
				<< turn->getTurnId() << endl;
			break;
		}
		
		if(turnid > turn->getTurnId() + turndelay)
			cerr << "warning: executing turn " << turn->getTurnId()
				<< " " << (SDL_GetTicks() - (start
				+ (turn->getTurnId() + turndelay)
 				*ticksperturn)) << " ms late" << endl;

		// All systems are go
		turn->execute(*this);

		turnqueue.pop_front();
	}
}

//Change the view if not at the extents 
void Game::updateView() {
	//x velocity can only be applied if view is within reasonable range
	if(viewVelocity_x < 0) {
		if(view.x > 0) view.x += viewVelocity_x;
	}
	else if(viewVelocity_x > 0){
		if(view.x < map->getWidth() - 2) view.x += viewVelocity_x;
	}
	//y velocity can only be applied if view is within reasonable range
	if(viewVelocity_y < 0){
		if(view.y > 0) view.y += viewVelocity_y;
	}
	else if(viewVelocity_y > 0){
		if(view.y < map->getHeight() - 2) view.y += viewVelocity_y;
	}

}

// Put everything on the screen
void Game::draw() {
	// Only draw if we've started the game
	if(!start) return;

	// Clear the screen for consistency
	SDL_Surface *surface = SDL_GetVideoSurface();
	SDL_Rect rect = {0, 0, (Uint16) surface->w, (Uint16) surface->h};
	SDL_FillRect(surface,&rect,SDL_MapRGB(surface->format,0,0,0));

	//Update the view before the map is drawn
	updateView();
	
	map->draw(view);
	
	//Draw the selection box if the left mouse button is down
	if(moved==1) boxRGBA(surface,xdown,ydown,mousex,mousey,60,60,255,170);
     
	// Make sure this shows up on the screen
	SDL_Flip(SDL_GetVideoSurface());
}

