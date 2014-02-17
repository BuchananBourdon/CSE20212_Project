#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

using namespace std;

static bool running = true;

static void input() {
	SDL_PumpEvents();

	// Just in case...
	if(SDL_GetKeyState(NULL)[SDLK_DELETE])
		running = 0;
}

static void update() {
}

static void draw() {
}

static void fps() {
	static Uint32 prev = 0;

	Uint32 now = SDL_GetTicks();

	if(prev) {
		cerr << " " << (now - prev) << " ms\r";
	}

	prev = now;
}

void game_loop() {
	while(running) {
		input();
		update();
		draw();

		fps();
	}
}

