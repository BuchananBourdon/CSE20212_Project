#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include "Unit.h"

class BlackHole : public Unit {
public:
	BlackHole(Map &_map, int _x, int _y)
		: Unit(_map,_x,_y,1,3,100,DOWN,0,true) {};

	int getType() { return UT_BLACK_HOLE; }

private:
	void drawUnit(View &);
	void updateUnit(Map &);
};

#endif

