#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_net.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "AttackUnitOrder.h"
#include "Cannon.h"
#include "CreateUnitOrder.h"
#include "Game.h"
#include "HangupMessage.h"
#include "JoinMessage.h"
#include "MoveUnitOrder.h"
#include "PlayerTurnMessage.h"
#include "SpawnBunny.h"
#include "SpawnRobot.h"

using namespace std;

const int Game::ticksperturn = 400; // For the deterministic simulation

const int Game::turndelay = 2; // Delay between turn creation and execution

const int Game::mapsize = 65;

Game::Game(bool _hosting, IPaddress address)
	: hosting(_hosting), won(false), numplayers(1), xdown(0), ydown(0),
		xup(0), yup(0), moved(0), mousex(0), mousey(0), start(0),
		bar(80,400), resources(100), showResources(false) {
	// Prepare the connection
	UDPsocket socket;
	Uint16 port = SDLNet_Read16(&address.port);
	if(hosting) { // Hosting a game
		setPlayerId(0); // The host is always player 0

		setSeed(time(NULL)); // Always a unique game

		if(socket = SDLNet_UDP_Open(port), !socket) {
			cerr << "error: cannot open UDP socket on port "
				<< port << endl;
			abort();
		}

		messagequeue.reset(new MessageQueue(socket));

		cout << "listening for join request on port " << port << endl;
	} else { // Joining a game
		if(socket = SDLNet_UDP_Open(0), !socket) {
			cerr << "error: cannot open anonymous UDP socket"
				<< endl;
			abort();
		}

		SDLNet_UDP_Bind(socket,0,&address);

		messagequeue.reset(new MessageQueue(socket));

		sendMessage(new JoinMessage());
	}
	viewVelocity_x = 0;
	viewVelocity_y = 0;

	state = AS_SPAWN;

	lastframe = 0;
}

Game::~Game() {
	for(unsigned int i = 0; i < units.size(); i++)
		for(unsigned int j = 0; j < units[i].size(); j++)
			delete units[i][j];

	SDL_FreeSurface(SDL_GetVideoSurface());
}

UDPsocket Game::getSocket() {
	return messagequeue->getSocket();
}

void Game::setPlayerId(Uint8 _playerid) {
	if(numplayers <= _playerid) {
		numplayers = _playerid + 1;
		units.resize(numplayers);
	}

	playerid = _playerid;
}

void Game::setSeed(Uint32 seed) {
	// Only do this once
	if(random.get()) {
		cerr << "warning: attempt to overwrite the random number"
			" generator" << endl;
		return;
	}

	random.reset(new Random(seed));
}

// Main game loop
void Game::play() {
	playing = true;

	// Play the game
	while(playing) {
		handleEvents();

		messagequeue->receive(*this);

		broadcastTurn();

		messagequeue->send();

		updateSimulation();

		draw();

		// Max out at 60 fps
		if(SDL_GetTicks() - lastframe < 16)
			SDL_Delay(16-(SDL_GetTicks() - lastframe));
		lastframe = SDL_GetTicks();
	}

	drawEndFrame();

	// Did we win or lose?
	playing = true;
	while(playing) {
		handleEvents();

		// Just in case the others don't know, yet
		messagequeue->send();

		// Max out at 60 fps
		if(SDL_GetTicks() - lastframe < 16)
			SDL_Delay(16-(SDL_GetTicks() - lastframe));
		lastframe = SDL_GetTicks();
	}
}

// Stop the main game loop
void Game::exit() {
	cout << "exiting main game loop" << endl;

	playing = false;
}

// Add another player to the game
Uint8 Game::addPlayer() {
	Uint8 id = numplayers++;

	units.resize(numplayers);

	return id;
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
	messagequeue->add(message);
}

// Assign a unit to a player
void Game::addUnit(Uint8 playerid, Unit *unit) {
	units[playerid].push_back(unit);
}

// Tell a unit to move to a location
void Game::moveUnit(Uint8 playerid, Uint16 unitid, Uint16 group, Uint16 x,
	Uint16 y) {
	movegroups.insert(pair<Uint16,pair<bool,bool> >(group,
		pair<bool,bool>(false,true)));
	units[playerid][unitid]->move(group,x,y);
}

// Tell a unit to attack another unit
void Game::attackUnit(Uint8 playerid, Uint16 unitid, Uint16 targetid) {
	Unit *target = Unit::getById(targetid);
	units[playerid][unitid]->attack(target);
}

// Process whatever SDL throws at us
void Game::handleEvents() {
	SDL_PumpEvents();

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:         handleKeyDown(event);   break;
		case SDL_KEYUP:           handleKeyUp(event);     break;
		case SDL_MOUSEBUTTONDOWN: handleMouseDown(event); break;
		case SDL_MOUSEBUTTONUP:   handleMouseUp(event);   break;
		case SDL_MOUSEMOTION:     handleMouseMove(event); break;
		case SDL_QUIT:            handleQuit(event);      break;

		default: break;
		}
	}

}

void Game::handleKeyDown(SDL_Event &e) {
	switch(e.key.keysym.sym) {
	case SDLK_DELETE: // Quick-exit key
		sendMessage(new HangupMessage());
		exit();
		break;

	case SDLK_UP: // Scroll up the map
		viewVelocity_y += -1;
		break;

	case SDLK_DOWN: // Scroll down the map
		viewVelocity_y += 1;
		break;

	case SDLK_LEFT: // Scroll left across the map
		viewVelocity_x += -1;
		break;

	case SDLK_RIGHT: // Scroll right across the map
		viewVelocity_x += 1;
		break;

	default: break;
	}
}

void Game::handleKeyUp(SDL_Event &e) {
	switch(e.key.keysym.sym) {
	case SDLK_UP: // Stop vertical scrolling
	case SDLK_DOWN:
		viewVelocity_y = 0;
		break;

	case SDLK_LEFT: // Stop horizontal scrolling
	case SDLK_RIGHT:
		viewVelocity_x = 0;
		break;

	default: break;
	}
}

void Game::handleMouseDown(SDL_Event &e) {
	int mx, my;
	SDL_GetMouseState(&mx,&my);

	switch(e.button.button) {
	case SDL_BUTTON_WHEELUP: // Zoom in
		view->adjustZoom(*map,1,mx,my);
		break;

	case SDL_BUTTON_WHEELDOWN: // Zoom out
		view->adjustZoom(*map,-1,mx,my);
		break;

	case SDL_BUTTON_LEFT: // Selection box
		xdown = e.button.x;
		ydown = e.button.y;
		moved = 1;
		break;

	default: break;
	}
}

void Game::handleMouseUp(SDL_Event &e) {
	SDL_Surface* surface = SDL_GetVideoSurface();

	switch(e.button.button) {
	case SDL_BUTTON_LEFT:
		// Stop drawing the selection box
		moved = 0;


		// Select units?
		if(abs(e.button.x - xdown) > 2 && abs(e.button.y - ydown) > 2) {
			if(state==AS_SELECT) {
				selectUnits(SDL_GetModState()&KMOD_SHIFT,xdown,ydown,
					e.button.x,e.button.y);
			}
		// Action bar action?
		} else if(e.button.x > (surface->w - bar.getWidth())/2
			&& e.button.x < (surface->w - (surface->w - bar.getWidth())/2)
			&& e.button.y > (surface->h - bar.getHeight())) {

			if(e.button.x>145 && e.button.x<195 && e.button.y >406 && e.button.y < 456) state=AS_SELECT;
			else if(e.button.x > 220 && e.button.x < 270 && e.button.y >406 && e.button.y < 456) state = AS_SPAWN;
			else if(e.button.x > 295 && e.button.x < 345 && e.button.y >406 && e.button.y < 456) state = AS_TURRET;
		// Move units and place spawns
		} else {
			if(state==AS_SELECT) {
				defaultAction(e.button.x,e.button.y);
			} else if(state==AS_SPAWN || state==AS_TURRET) {
				Uint16 mapx = min(view->x + (float) e.button.x/view->zoom,view->x + view->w - 1u);
				Uint16 mapy = min(view->y + (float) e.button.y/view->zoom,view->y + view->h - 1u);
				int badTileCount = 0;
				int xlength,ylength;
				if(state==AS_SPAWN) xlength=3,ylength=2;
				if(state==AS_TURRET) xlength=1,ylength=1-playerid;
				for(int i=0; i<ylength+playerid; i++) {
					for(int j=0; j<xlength; j++) {
						if(map->isOccupied(mapx+j,mapy+i)
							|| map->isFoggy(mapx + j,mapy + i)
							|| map->tileType(mapx+j,mapy+i) != 2
							|| map->resourceType(mapx+j,mapy+i) != 0)
							badTileCount++;
					}
				}

				int cost;
				enum Unit::unit_type type;
				if(state == AS_SPAWN) {
					switch(playerid) {
					default:
					case 0:
						type = Unit::UT_SPAWN_BUNNY;
						cost = SpawnBunny::getCost();
						break;

					case 1:
						type = Unit::UT_SPAWN_ROBOT;
						cost = SpawnRobot::getCost();
						break;
					}
				} else if(state == AS_TURRET) {
					type = Unit::UT_TURRET;
					cost = Cannon::getCost();
				}

				if(badTileCount == 0 && useResources(cost))
					turn->addOrder(new CreateUnitOrder(type,mapx,mapy));
			}
		}
		break;

	default: break;
	}
}

void Game::handleMouseMove(SDL_Event &e) {
	// Keep track of the mouse
	mousex = e.motion.x;
	mousey = e.motion.y;

	// Show the resource count when hovering over the resource bar
	showResources = mousex > 135 && mousex < 505 && mousey < 480 - 2
		&& mousey > 480 - 18;
}

void Game::handleQuit(SDL_Event &e) {
	sendMessage(new HangupMessage());
	exit();
}

// Send our turn out to the other players, if ready
void Game::broadcastTurn() {
	// Sanity check
	if(!start) return;

	// Is this our first turn?
	if(!turn.get())
		turn.reset(new PlayerTurn(0,playerid));

	int turnid = (SDL_GetTicks() - start)/ticksperturn;
	if(turnid > turn->getTurnId()) {
		// Tell every one about this
		sendMessage(new PlayerTurnMessage(turn.get()));
		addPlayerTurn(turn.get());

		// Time for a new turn
		turn.reset(new PlayerTurn(turn.release()->getTurnId() + 1,
			playerid));
	}
}

// Gather Resources, and eliminate the resource on the map
void Game::gatherResources(Unit * unit, Map &map) {
	if(resources<100){
		int xlocation = unit->getX();
		int ylocation = unit->getY();
		int type = unit->getType();
		//only gather resources if it's the correct unit on
		//the correct resource, for the correct playerid
		if((map.resourceType(xlocation,ylocation)==2 && type==2) || (map.resourceType(xlocation,ylocation)==1 && type==1)) {
			map.decrementResourceAmount(xlocation,ylocation);
			if(map.getResourceAmount(xlocation,ylocation)<=0)
				map.elim_resource(xlocation,ylocation);
			if(playerid==1&&type==2 || playerid==0&&type==1)
				resources++;
		}
	}
}

// Advance, if appropriate, the fully-determined part of the simulation
void Game::updateSimulation() {
	// Have we started actual gameplay, yet?
	if(!start) {
		// Should we?
		if(numplayers > 1) {
			cout << "game has at least two players; starting play"
				<< endl;

			struct { Uint16 x, y; } defog, viewstart;
			switch(playerid) {
			case 0:
				defog.x = defog.y = 0;
				viewstart.x = viewstart.y = 0;
				break;

			case 1:
				defog.x = mapsize - 1;
				defog.y = mapsize - 1;

				viewstart.x = mapsize - 1;
				viewstart.y = mapsize - 1;
				break;

			default:
				cerr << "warning: unexpected player ID ("
					<< (int) playerid << ")" << endl;
				break;
			}

			// Generate the map
			map.reset(new Map(mapsize,random.get()));
			map->defog(defog.x,defog.y,20);

			// Prepare the view
			view.reset(new View(viewstart.x,viewstart.y,
				View::maxzoom));
			updateView();

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
		turnqueue.size() && turnid >= turn->getTurnId() + turndelay) {
		// Go no further if we're missing data from someone
		if(turn->getPlayerCount() < numplayers) {
			cerr << "warning: possible dropped packet for turn "
				<< turn->getTurnId() << endl;
			break;
		}

		if(turnid > turn->getTurnId() + turndelay)
			cerr << "warning: executing turn " << turn->getTurnId()
				<< " " << (SDL_GetTicks() - (start
				+ (turn->getTurnId() + turndelay + 1)
				*ticksperturn)) << " ms late" << endl;

		// All systems are go
		turn->execute(*this);

		turnqueue.pop_front();

		// Update each unit and gather resources
		for(unsigned int i = 0; i < units.size(); i++) {
			for(unsigned int j = 0; j < units[i].size(); j++) {
				//start coordinates of the unit
				int startX = units[i][j]->getX();
				int startY = units[i][j]->getY();
				units[i][j]->update();
				//end coordinates after updating
				int endX = units[i][j]->getX();
				int endY = units[i][j]->getY();
				//only gather resources if the unit is stationary
				if(startX==endX && startY==endY)
					gatherResources(units[i][j],*map);
			}
		}

		checkEndState();

		// Cycle the move groups
		for(std::map<Uint16,pair<bool,bool> >::iterator it
			= movegroups.begin(); it != movegroups.end(); it++) {
			it->second.second = it->second.first;
			it->second.first = false;
		}
	}
}

//Change the view if not at the extents
void Game::updateView() {
	SDL_Surface *screen = SDL_GetVideoSurface();

	view->x += viewVelocity_x;
	view->x = max(view->x,0.f);
	view->x = min(view->x,map->getWidth() - (float) screen->w/view->zoom);

	view->y += viewVelocity_y;
	view->y = max(view->y,0.f);
	view->y = min(view->y,map->getHeight() - (float) screen->h/view->zoom);
}

// Put everything on the screen
void Game::draw() {
	// Only draw if we've started the game
	if(!start) return;

	// Clear the screen for consistency
	SDL_Surface *surface = SDL_GetVideoSurface();
	SDL_Rect rect = {0, 0, (Uint16) surface->w, (Uint16) surface->h};
	SDL_FillRect(surface,&rect,SDL_MapRGB(surface->format,0,0,0));

	// Update the view before the map is drawn
	updateView();

	// Draw the background
	map->draw(*view);

	// Draw all the units
	for(unsigned int i = 0; i < units.size(); i++)
		for(unsigned int j = 0; j < units[i].size(); j++)
			if(units[i][j]->inView(*view))
				units[i][j]->draw(*view);

	for(set<int>::iterator iter = selected.begin(); iter != selected.end();
		iter++)
		units[playerid][*iter]->drawSelected(*view);

	// draw selection box, transparent blue/gray
	if(state==AS_SELECT) {
		if(moved) {
			boxRGBA(surface,xdown,ydown,mousex,mousey,90,90,255,80);
			rectangleRGBA(surface,xdown,ydown,mousex,mousey,150,150,255,170);
		}
	}
	// Draw the spawn availability box
	else if(state==AS_SPAWN || state==AS_TURRET) {
		Uint16 mapx = min(view->x + (float) mousex/view->zoom,view->x + view->w - 1u);
		Uint16 mapy = min(view->y + (float) mousey/view->zoom,view->y + view->h - 1u);
		int drawx, drawy, drawh;
		int xlength,ylength;
		if(state==AS_SPAWN) xlength=3,ylength=2;
		if(state==AS_TURRET) xlength=1,ylength=1-playerid;
		for(int i=0; i<ylength+playerid; i++) {
			for(int j=0; j<xlength; j++) {
				drawx = (mapx - view->x + j)*view->zoom;
				drawy = (mapy - view->y + i)*view->zoom;
				drawh = view->zoom;
				if(map->isOccupied(mapx+j,mapy+i)
					|| map->isFoggy(mapx + j,mapy + i)
					|| map->tileType(mapx+j,mapy+i) != 2
					|| map->resourceType(mapx+j,mapy+i) != 0)
					boxRGBA(surface,drawx,drawy,drawx+drawh,drawy+drawh,255,0,0,125);
				else boxRGBA(surface,drawx,drawy,drawx+drawh,drawy+drawh,0,255,0,125);
			}
		}
	}

	// Draw the ActionBar last, so it's drawn atop the map/unit layer
	bar.draw(resources,showResources,state,playerid);

	// Make sure this shows up on the screen
	SDL_Flip(SDL_GetVideoSurface());
}

// Stop playing if one player has won
void Game::checkEndState() {
	for(unsigned int i = 0; i < units.size(); i++) {
		// Are all of these player's units dead?
		bool alive = false;
		for(unsigned int j = 0; j < units[i].size(); j++)
			if(!units[i][j]->isDead())
				alive = true;

		if(units[i].size() && !alive) {
			if(i == playerid) won = false;
			else won = true;

			playing = false;
		}
	}
}

// Tell the user what happened
void Game::drawEndFrame() {
	cout << "game over; you have " << (won ? "won" : "lost") << endl;

	if(endframe = IMG_Load(won ? "win.png" : "lose.png"), !endframe) {
		cerr << "cannot load end-game image" << endl;
		return;
	}

	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_BlitSurface(endframe,NULL,screen,NULL);
	SDL_Flip(screen);
}

// Decide what to do based on what was clicked
void Game::defaultAction(unsigned int x, unsigned int y) {
	Uint16 mapx = min(view->x + (float) x/view->zoom,view->x + view->w - 1u);
	Uint16 mapy = min(view->y + (float) y/view->zoom,view->y + view->h - 1u);

	if(map->isOccupied(mapx,mapy)) {
		Unit *unit = Unit::getById(map->getOccupier(mapx,mapy));

		// Enemy unit?
		if(unit->getOwnerId() != playerid) attackUnit(unit);
		// Friendly unit: already selected?
		else if(selected.size() == 1
			&& units[playerid][*selected.begin()] == unit)
			unit->act();
		// Friendly unit: select it
		else selectUnits(false,x,y,x,y);
	} else moveUnits(mapx,mapy);
}

// Order all selected units to move
// Select all owned units contained in the onscreen rectangle
void Game::selectUnits(bool add, int x1, int y1, int x2, int y2) {
	// Ensure (x1, y1) is the upper left
	if(x1 > x2) swap(x1,x2);
	if(y1 > y2) swap(y1,y2);

	View selection(view->x + x1/view->zoom,view->y + y1/view->zoom,
		x2/view->zoom - x1/view->zoom + 1,
		y2/view->zoom - y1/view->zoom + 1,view->zoom);

	// Deselect previous selection?
	if(!add) selected.clear();

	// Add new selection
	for(unsigned int i = 0; i < units[playerid].size(); i++)
		if(units[playerid][i]->inView(selection)
			&& !units[playerid][i]->isDead())
			selected.insert(i);
}

void Game::attackUnit(Unit *unit) {
	for(set<int>::iterator iter = selected.begin(); iter != selected.end();
		iter++)
		turn->addOrder(new AttackUnitOrder(*iter,unit->getId()));
}

void Game::moveUnits(Uint16 mapx, Uint16 mapy) {
	Uint16 group = movegroups.size() ? (*movegroups.end()--).first + 1 : 0;

	for(set<int>::iterator iter = selected.begin(); iter != selected.end();
		iter++)
		turn->addOrder(new MoveUnitOrder(*iter,group,mapx,mapy));
}

// Returns whether we had enough
bool Game::useResources(int cost) {
	if(resources < cost) return false;

	resources -= cost;

	return true;
}

// Indicate a member of group has moved
void Game::groupMove(Uint16 group) {
	movegroups[group].first = true;
}

// Returns whether a member of group moved last turn
bool Game::groupMoved(Uint16 group) {
	return movegroups[group].second;
}

