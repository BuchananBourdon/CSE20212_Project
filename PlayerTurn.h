#ifndef PLAYER_TURN_H
#define PLAYER_TURN_H

#include <vector>

#include "Order.h"

class PlayerTurn {
public:
	PlayerTurn(int _turnid, int _playerid)
		: turnid(_turnid), playerid(_playerid) {};

	int getTurnId() { return turnid; }

	void addOrder(Order *);
	void execute();

private:
	const int turnid;
	const int playerid;

	std::vector<Order *> orders;
};

#endif

