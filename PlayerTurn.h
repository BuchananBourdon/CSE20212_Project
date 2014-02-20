class PlayerTurn;

#ifndef PLAYERTURN_H
#define PLAYERTURN_H

#include <vector>

#include <SDL/SDL.h>

#include "Order.h"

using namespace std;

struct PlayerTurn {
	PlayerTurn(int _turnid, int _playerid, vector<Order *> _orders)
		: turnid(_turnid), playerid(_playerid), orders(_orders) {};
	~PlayerTurn();

	static PlayerTurn deserialize(Uint8 **); // Import from byte array

	vector<Uint8> serialize(); // Export from byte array

	Uint32 turnid;          // Which turn?
	Uint8 playerid;         // Which player?
	vector<Order *> orders; // What to do?
};

#endif

