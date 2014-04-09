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
	static SDL_Rect clipsRight[ 4 ];
	static SDL_Rect clipsLeft[ 4 ];
	static SDL_Rect clipsUp[ 4 ];
	static SDL_Rect clipsDown[ 4 ];

	// Pertain to image pixel dimensions
	static int BUNNY_WIDTH;
	static int BUNNY_HEIGHT;

	int status;
	int frame;

	void drawUnit(View &);
	void update(Map &);
	void setClips();
};

#endif
