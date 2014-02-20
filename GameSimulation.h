class GameSimulation;

#ifndef GAMESIMULATION_H
#define GAMESIMULATION_H

#include <list>

#include <SDL/SDL.h>

#include "Order.h"
#include "Player.h"
#include "PlayerTurn.h"
#include "Turn.h"

using namespace std;

class GameSimulation {
	friend class Order;

public:
	GameSimulation(Player *(&)[2]); // Default constructor
	~GameSimulation();

	void broadcastPlayerTurn(PlayerTurn &); // Process and distribute
	void update();                          // Update the simulation

	Uint32 getTicks();       // Since the dawn of time
	SDL_Surface *getImage(); // Just for testing

private:
	Player *(&players)[2]; // Who's giving the orders around here?

	Uint32 simstart;     // The dawn of time
	unsigned int turnid; // Which turn now?
	list<Turn> turns;    // Future turns

	// Game state
	SDL_Surface *image; // Just for testing
};

#endif

