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
	Player(Uint8 _id) : id(_id), ownersim(NULL) {};
	virtual ~Player() {};

	virtual void setOwnerSimulation(GameSimulation *sim) { ownersim = sim; }

	virtual void receivePlayerTurn(PlayerTurn &) {}; // Only if you care

	virtual void update() = 0; // Handle events and such

protected:
	Uint8 id;
	GameSimulation *ownersim;
};

#endif

