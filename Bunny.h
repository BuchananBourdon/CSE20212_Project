#ifndef BUNNY_H
#define BUNNY_H

#include "Unit.h"


class Bunny : public Unit {
public:
	Bunny(int, int);
	
	int getType();
	
private:
	// Has static Surface and Rect for blitting, will modify clip when MoveUnitOrder is finished
	static SDL_Surface * bunnySurface;
	static SDL_Rect  clip;
	
	// Pertain to image pixel dimensions
	int BUNNYWIDTH;
	int BUNNYHEIGHT;

	void drawUnit(View &);
};

#endif
