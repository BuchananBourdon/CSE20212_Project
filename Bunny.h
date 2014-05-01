#ifndef BUNNY_H
#define BUNNY_H

#include "Unit.h"

class Bunny : public Unit {
public:
	Bunny(Game &, int, int, int);
	
	static int getCost() { return 5; }

	int getType() { return UT_BUNNY; }
	
private:
	// Has static Surface and Rect for blitting
	static SDL_Surface * bunnySurface;
	static SDL_Rect clipsRight[ 17][ 4 ];
	static SDL_Rect clipsLeft[ 17 ][ 4 ];
	static SDL_Rect clipsUp[ 17 ][ 4 ];
	static SDL_Rect clipsDown[ 17 ][ 4 ];

	void drawUnit(View &);
	void updateUnit();
	void setClips();
};

#endif
