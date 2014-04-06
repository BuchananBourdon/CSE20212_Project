#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include "Unit.h"

class BlackHole : public Unit {
public:
	BlackHole(int _x, int _y) : Unit(_x,_y,1,1,100) {};

	int getType() { return UT_BLACK_HOLE; }

	void draw(View &);
};

#endif

