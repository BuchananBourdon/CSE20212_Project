#include <SDL/SDL.h>

#include "Game.h"

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640,480,0,0);

	// TODO: configuration/options/etc. prior to starting a game
	Game game;
	game.play();
}

