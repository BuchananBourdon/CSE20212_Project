#ifndef SPAWNROBOT_H
#define SPAWNROBOT_H

#include "Unit.h"

class SpawnRobot : public Unit {
public:
	SpawnRobot(Map &, int, int);
	
	int getType();
private:
	static SDL_Surface * spawnrobotSurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit(Map &);
	void setClips();
};

#endif
