#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "Player.h"

using namespace std;

class LocalPlayer : public Player {
public:
	LocalPlayer(int);

	// From Player
	virtual void update();
};

#endif

