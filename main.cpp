#include <cstdlib>

#include <SDL/SDL.h>

#include "game_loop.h"

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	SDL_SetVideoMode(640,480,30,SDL_HWSURFACE | SDL_DOUBLEBUF);

	game_loop();
}

