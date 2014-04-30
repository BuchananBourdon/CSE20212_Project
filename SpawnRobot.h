#ifndef SPAWNROBOT_H
#define SPAWNROBOT_H

#include "Unit.h"

class SpawnRobot : public Unit {
public:
	SpawnRobot(Map &, int, int, bool);
	
	int getType();
private:
	static SDL_Surface * spawnrobotSurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif
