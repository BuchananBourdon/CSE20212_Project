#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "Player.h"

using namespace std;

class LocalPlayer : public Player {
public:
	LocalPlayer(GameSimulation &_ownersim, int _id)
		: Player(_ownersim, _id) {}

	// From Player
	virtual void update();
};

#endif

