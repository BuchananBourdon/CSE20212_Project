#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"

class Robot : public Unit {
public:
	Robot(Game &, int, int, int);

	int getType();

private:
  	// Has static Surface and Rect for blitting, will modify clip when MoveUnitOrder is finished
  	static SDL_Surface * robotSurface;
  	static SDL_Rect clipsRight[17][ 4 ];
  	static SDL_Rect clipsLeft[17][ 4 ];
  	static SDL_Rect clipsUp[17][ 4 ];
  	static SDL_Rect clipsDown[17][ 4 ];

   	void drawUnit(View &);
	void updateUnit();
  	void setClips();
};

#endif
