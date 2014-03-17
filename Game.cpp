#include <SDL/SDL.h>

#include "Game.h"

Game::Game() {
}

Game::~Game() {
}

// Main game loop
void Game::play() {
	playing = true;

	while(playing) {
		handleEvents();
	}
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

