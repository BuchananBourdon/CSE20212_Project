#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <iostream>

#include "Game.h"
#include "JoinMessage.h"

using namespace std;

const int Game::ticksperturn = 200; // For the deterministic simulation

Game::Game(bool _hosting, IPaddress address)
	: hosting(_hosting), numplayers(1), start(0) {
	// Prepare the connection
	Uint16 port = SDLNet_Read16(&address.port);
	if(hosting) { // Hosting a game
		setPlayerId(0); // The host is always player 0

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
}

Game::~Game() {
	if(socket) SDLNet_UDP_Close(socket);
}

void Game::setPlayerId(Uint8 _playerid) {
	if(numplayers <= _playerid) numplayers = _playerid + 1;

	playerid = _playerid;
}

// Main game loop
void Game::play() {
	playing = true;

	while(playing) {
		handleEvents();
		handleMessages();

		sendMessages();

		executeTurns();

		SDL_Delay(10); // Don't hog the CPU
	}
}

// Add another player to the game
Uint8 Game::addPlayer() {
	return numplayers++;
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
		if(numplayers > 1) {
			cout << "game has at least two players; starting play"
				<< endl;
			start = SDL_GetTicks();
		}
		return;
	}

	// Execute as many turns as we can
	Turn *turn;
	int turnid = (SDL_GetTicks() - start)/ticksperturn;
	while(turn = turnqueue.front(), turn && turnid >= turn->getTurnId()) {
		// Go no further if we're missing data from someone
		if(turn->getPlayerCount() < numplayers) {
			if(turnid < 2)
				cerr << "warning: possible dropped packet(s)"
					" for turn " << turn->getTurnId()
					<< endl;
			break;
		}

		turn->execute();

		turnqueue.pop_front();
	}
}

