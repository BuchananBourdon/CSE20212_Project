#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "Game.h"

static void print_help() {
	cout << "main (host|join) address:port" << endl;
}

int main(int argc, char **argv) {
	bool hosting;

	// Check command-line arguments
	if(argc < 3) {
		print_help();
		return 1;
	}

	// Hosting a game?
	if(strcmp(argv[1],"host") == 0) hosting = true;
	else if(strcmp(argv[1],"join") == 0) hosting = false;
	else {
		print_help();
		return 1;
	}

	// Where are we listening/to whom are we talking?
	char name[256];
	unsigned char ip[4];
	unsigned short port;

	IPaddress address;

	if(sscanf(argv[2],"%hhu.%hhu,%hhu.%hhu:%hu",ip + 0,ip + 1,ip + 2,
	ip + 3,&port) == 5) {
		// IPv4 address, with port
		address.host = (Uint32) ip[0] << 24 | (Uint32) ip[1] << 16
			| (Uint32) ip[2] << 8 | ip[3];
		address.port = port;
	} else if(sscanf(argv[2]," %255[^:]:%hu",name,&port) == 2) {
		// Domain name, with port
		if(SDLNet_ResolveHost(&address,name,port) != 0) {
			cerr << "cannot resolve host '" << name << "'" << endl;
			return 1;
		}
	} else { // ???
		print_help();
		return 1;
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	SDL_SetVideoMode(640,480,30,SDL_HWSURFACE | SDL_DOUBLEBUF);

	Game game;
	if(hosting) game.host(port);
	else game.join(&address);
	game.play();
}

