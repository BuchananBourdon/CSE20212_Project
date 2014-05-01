#ifndef CANNON_H
#define CANNON_H

#include "Unit.h"

class Cannon : public Unit {
public:
	Cannon(Game &, int, int, int);
	
	static int getCost() { return 10; }

	int getType() { return UT_TURRET; }

private:

	static SDL_Surface * cannonSurface;
	static SDL_Rect clipsCarrotCannon[17];
	static SDL_Rect clipsLaserCannon[17];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif
