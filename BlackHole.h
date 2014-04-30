#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include "Unit.h"

class BlackHole : public Unit {
public:
	BlackHole(Map &_map, int _x, int _y, bool _local)
		: Unit(_map,_x,_y,1,1,100,10,true,_local) {};

	int getType() { return UT_BLACK_HOLE; }

private:
	void drawUnit(View &);
	void updateUnit();
};

#endif

