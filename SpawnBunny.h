#ifndef SPAWNBUNNY_H
#define SPAWNBUNNY_H

#include "Unit.h"

class SpawnBunny : public Unit {
public:
	SpawnBunny(Map &, int, int);
	
	int getType();
	
private:
	// Has static Surface and Rect for blitting
	static SDL_Surface * spawnbunnySurface;
	static SDL_Rect clips[ 17 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif
