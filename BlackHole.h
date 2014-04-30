#ifndef BLACK_HOLE_H
#define BLACK_HOLE_H

#include "Unit.h"

class BlackHole : public Unit {
public:
	BlackHole(Game &_game, int _ownerid, int _x, int _y)
		: Unit(_game,_ownerid,_x,_y,1,1,100,10,true) {};

	int getType() { return UT_BLACK_HOLE; }

private:
	void drawUnit(View &);
	void updateUnit();
};

#endif

