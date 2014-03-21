class Turn;

#ifndef TURN_H
#define TURN_H

#include <vector>

#include "Order.h"
#include "PlayerTurn.h"

class Turn {
public:
	Turn(int _turnid) : turnid(_turnid) {};
	~Turn();

	int getPlayerCount() { return playerturns.size(); }
	int getTurnId()      { return turnid; }

	void addPlayerTurn(PlayerTurn *); // Add a turn
	void execute(Game &);             // Do everything

private:
	const int turnid; // Which turn?

	std::vector<PlayerTurn *> playerturns;
};

#endif

