#include <cstring>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "Game.h"
#include "LocalPlayer.h"
#include "NetworkPlayer.h"

using namespace std;

// Default constructor
Game::Game() {
	ready = false;

	players[0] = NULL;
	players[1] = NULL;

	simulation = NULL;
}

// Destructor
Game::~Game() {
	if(players[0]) delete players[0];
	if(players[1]) delete players[1];

	if(simulation) delete simulation;
}

// Enable outputing IPaddress structs to streams
ostream &operator<<(ostream &stream, IPaddress &address) {
	// Format: x.x.x.x:port
	stream << (address.host & 0xFF) << '.' << (address.host >> 8 & 0xFF)
		<< '.' << (address.host >> 16 & 0xFF) << '.'
		<< (address.host >> 24) << ':'
		<< (address.port << 8 & 0xFF00 | address.port >> 8);

	return stream;
}

// Act as the host in a network game and listens on port for someone to join
void Game::host(Uint16 port) {
	cerr << "hosting a game; listening on port " << port
		<< " for second player" << endl;

	// We (the host) are player #0
	players[0] = new LocalPlayer(0);

	UDPsocket socket = SDLNet_UDP_Open(port);
	if(socket == NULL) {
		cerr << "error: cannot open port " << port << endl;
		return;
	}

	UDPpacket *packet = SDLNet_AllocPacket(0xFFFF);

	// Poll for a 'join game' packet
	while(SDLNet_UDP_Recv(socket,packet) != 1
	|| !isPacketHead(packet,"join"))
		SDL_Delay(10);

	cerr << "received join request from " << packet->address << endl;

	SDLNet_UDP_Bind(socket,0,&packet->address);

	// Send a response
	UDPpacket *response = SDLNet_AllocPacket(0xFFFF);
	memcpy(response->data,"rkapjoinok\001",11);
	response->len = 11;
	SDLNet_UDP_Send(socket,0,response);

	// Set up the remote player's handler
	players[1] = new NetworkPlayer(socket,1);

	// Set up the local simulation
	simulation = new GameSimulation(players);

	// Cleanup
	SDLNet_FreePacket(packet);
	SDLNet_FreePacket(response);

	ready = true;
}

// Join a game hosted remotely at address
void Game::join(IPaddress *address) {
	cerr << "attempting to join a game hosted at " << *address << endl;

	UDPsocket socket = SDLNet_UDP_Open(0);
	if(socket == NULL) {
		cerr << "error: cannot open UDP socket" << endl;
		return;
	}
	SDLNet_UDP_Bind(socket,0,address);

	// Send join packets until we hear back
	UDPpacket *joinpacket = SDLNet_AllocPacket(0xFFFF);
	memcpy(joinpacket->data,"rkapjoin",8);
	joinpacket->len = 8;

	UDPpacket *response = SDLNet_AllocPacket(0xFFFF);

	for(bool joined = false; !joined;
	joined = isPacketHead(response,"joinok")) {
		SDLNet_UDP_Send(socket,0,joinpacket);

		// Wait up to a second before retrying
		Uint32 start = SDL_GetTicks();
		while(SDLNet_UDP_Recv(socket,response) != 1
		&& SDL_GetTicks() - start < 1000)
			SDL_Delay(10);
	}

	cerr << "received OK to join" << endl;

	// Add the players
	players[0] = new LocalPlayer(*(Uint8 *) (response->data + 10));
	players[1] = new NetworkPlayer(socket,0);

	// Set up the local simulation
	simulation = new GameSimulation(players);

	// Cleanup
	SDLNet_FreePacket(joinpacket);
	SDLNet_FreePacket(response);

	ready = true;
}

// Execute the main game loop
void Game::play() {
	// Only play if we've joined or had a player join a game
	if(!ready || !simulation) {
		cerr << "error: game not ready to play" << endl;
		return;
	}

	bool running = true;

	while(running) {
		// Input
		SDL_PumpEvents();

		// Just in case...
		if(SDL_GetKeyState(NULL)[SDLK_DELETE])
			running = false;

		// Update
		simulation->update();

		// Draw
		SDL_Surface *screen = SDL_GetVideoSurface();
		SDL_BlitSurface(simulation->getImage(),NULL,screen,NULL);
		SDL_Flip(screen);
	}
}

// Format of a packet: 'r' 'k' 'a' 'p' other stuff...
bool Game::isPacketHead(UDPpacket *packet, string head) {
	return strncmp((char *) packet->data,"rkap",4) == 0
	&& strncmp((char *) packet->data + 4,head.c_str(),head.size()) == 0;
}

