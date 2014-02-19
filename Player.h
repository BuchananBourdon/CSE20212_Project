class Player;

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "GameSimulation.h"
#include "Order.h"
#include "PlayerTurn.h"

using namespace std;

class Player {
public:
	Player(int _id) : id(_id) {};
	virtual ~Player() {};

	virtual void receivePlayerTurn(PlayerTurn &) {}; // Only if you care

	virtual void update() = 0; // Handle events and such

protected:
	int id;
};

#endif

