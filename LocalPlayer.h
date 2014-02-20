#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include <vector>

#include <SDL/SDL.h>

#include "Player.h"
#include "PlayerTurn.h"

using namespace std;

class LocalPlayer : public Player {
public:
	LocalPlayer(Uint8);

	// From Player
	virtual void setOwnerSimulation(GameSimulation *);
	virtual void update();

private:
	static int filterEvents(const SDL_Event *);

	static bool instantiated;

	static Uint8 id;
	static GameSimulation *simulation;

	static Uint32 turnid;
	static int turnevents;

	static SDL_mutex *turnsmutex;
	static vector<PlayerTurn> turns;
};

#endif

