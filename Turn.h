#ifndef TURN_H
#define TURN_H

#include <vector>

#include "Order.h"
#include "PlayerTurn.h"

class Turn {
public:
	Turn(int _turnid) : turnid(_turnid) {};

	int getPlayerCount() { return playerturns.size(); }
	int getTurnId()      { return turnid; }

	void execute(); // Do everything

private:
	const int turnid; // Which turn?

	std::vector<PlayerTurn *> playerturns;
};

#endif

