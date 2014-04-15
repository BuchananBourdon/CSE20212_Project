#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "Game.h"

using namespace std;

static void print_help_and_die() {
	cout << "usage: main (host|join) address:port" << endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	srand(time(NULL));

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640,480,0,0);

	// Get connection settings from the command line
	if(argc != 3) print_help_and_die();

	// Hosting or joining?
	bool host;
	if(strcmp(argv[1],"host") == 0) host = true;
	else if(strcmp(argv[1],"join") == 0) host = false;
	else print_help_and_die();

	// Where are we listening/to whom are we talking
	char name[256];
	unsigned char ip[4];
	unsigned short port;

	IPaddress address;

	if(sscanf(argv[2],"%hhu.%hhu.%hhu.%hhu:%hu",ip + 0,ip + 1,ip + 2,
		ip + 3,&port) == 5) {
		// IPv4 address and port
		address.host = (Uint32) ip[0] << 24 | (Uint32) ip[1] << 16
			| (Uint32) ip[2] << 8 | ip[3];
		address.port = port;
	} else if(sscanf(argv[2]," %255[^:]:%hu",name,&port) == 2) {
		// Domain name and port
		if(SDLNet_ResolveHost(&address,name,port) != 0) {
			cerr << "error: cannot resolve host '" << name << "'"
				<< endl;
			return 1;
		}
	} else print_help_and_die();

	// TODO: better configuration/options/etc. prior to starting a game
	Game game(host,address);
	game.play();
}

