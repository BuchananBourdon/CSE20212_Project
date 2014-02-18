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
	GameSimulation(); // Default constructor
	~GameSimulation();

	void broadcastPlayerTurn(PlayerTurn &); // Process and distribute
	void update();                          // Update the simulation

private:
	Player *players[2]; // Who's giving the orders around here?

	unsigned int turnid; // Which turn now?
	list<Turn> turns;    // Future turns

	// Game state
	SDL_Surface *image; // Just for testing
};

#endif

