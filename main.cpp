#include <cstdlib>

#include <SDL/SDL.h>

#include "Game.h"

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	SDL_SetVideoMode(640,480,30,SDL_HWSURFACE | SDL_DOUBLEBUF);

	Game game;
	game.play();
}

