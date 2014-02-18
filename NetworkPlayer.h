#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include <string>

#include "Player.h"
#include "PlayerTurn.h"

using namespace std;

class NetworkPlayer : public Player {
public:
	NetworkPlayer(GameSimulation &, int, string, bool);

	// From Player
	virtual void receivePlayerTurn(PlayerTurn &);
	virtual void update();
};

#endif

