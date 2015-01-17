#ifndef SPAWNROBOT_H
#define SPAWNROBOT_H

#include "Unit.h"

class SpawnRobot : public Unit {
public:
	SpawnRobot(Game &, int, int, int);

	static int getCost() { return 80; }
	
	int getType() { return UT_SPAWN_ROBOT; }

	void act();

private:
	static SDL_Surface * spawnrobotSurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif

