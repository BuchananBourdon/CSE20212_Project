#include <SDL/SDL.h>

#include "Game.h"

// Execute the main game loop
void Game::play() {
	bool running = true;

	while(running) {
		// Input
		SDL_PumpEvents();

		// Just in case...
		if(SDL_GetKeyState(NULL)[SDLK_DELETE])
			running = false;

		// Update
		simulation.update();

		// Draw
	}
}

