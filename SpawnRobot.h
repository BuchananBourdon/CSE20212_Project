#ifndef SPAWNROBOT_H
#define SPAWNROBOT_H

#include "Unit.h"

class SpawnRobot : public Unit {
public:
	SpawnRobot(Game &, int, int, int);
	
	int getType();

	void act();

private:
	static SDL_Surface * spawnrobotSurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif
