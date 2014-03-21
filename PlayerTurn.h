class PlayerTurn;

#ifndef PLAYER_TURN_H
#define PLAYER_TURN_H

#include <vector>

#include <SDL/SDL_net.h>

#include "Order.h"

class PlayerTurn {
public:
	PlayerTurn(int _turnid, Uint8 _playerid)
		: turnid(_turnid), playerid(_playerid) {};
	~PlayerTurn();

	int                        getTurnId()   { return turnid; }
	Uint8                      getPlayerId() { return playerid; }
	const std::vector<Order *> getOrders()   { return orders; }

	void addOrder(Order *);
	void execute(Game &);

private:
	const int turnid;
	const Uint8 playerid;

	std::vector<Order *> orders;
};

#endif

