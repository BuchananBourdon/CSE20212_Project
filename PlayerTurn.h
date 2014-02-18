#ifndef PLAYERTURN_H
#define PLAYERTURN_H

#include <vector>

#include "Order.h"

using namespace std;

struct PlayerTurn {
	PlayerTurn(int _turnid, int _playerid, vector<Order *> _orders)
		: turnid(_turnid), playerid(_playerid), orders(_orders) {};

	const unsigned int turnid;    // Which turn?
	const unsigned int playerid;  // Which player?
	const vector<Order *> orders; // What to do?
};

#endif

