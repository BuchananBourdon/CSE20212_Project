#ifndef UNIT_H
#define UNIT_H

#include "View.h"

class Unit {
public:
	Unit(int _x, int _y, int _w, int _h, int _maxhp)
		: id(unitcount++), x(_x), y(_y), w(_w), h(_h), maxhp(_maxhp),
			hp(_maxhp) {}

	virtual int getType() = 0;

	bool inView(View &);

	void draw(View &);
	void drawSelected(View &);

	enum unit_type {
		UT_BLACK_HOLE,
		UT_BUNNY
	};

protected:
	virtual void drawUnit(View &) = 0; // Per-subclass

	const int id; // Which am I?

	unsigned int x, y;       // Where am I?
	const unsigned int w, h; // How big am I?

	const unsigned int maxhp;
	unsigned int hp;

private:
	static int unitcount; // For unique IDs
};

#endif

