#ifndef SPAWNBUNNY_H
#define SPAWNBUNNY_H

#include "Unit.h"

class SpawnBunny : public Unit {
public:
	SpawnBunny(Game &, int, int, int);

	static int getCost() { return 80; }

	int getType() { return UT_SPAWN_BUNNY; }

	void act();
	
private:
	// Has static Surface and Rect for blitting
	static SDL_Surface * spawnbunnySurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif

