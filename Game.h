#ifndef GAME_H
#define GAME_H

#include "GameSimulation.h"

class Game {
public:
	void play(); // Main loop

private:
	GameSimulation simulation; // All of the real game state and logic
};

#endif

