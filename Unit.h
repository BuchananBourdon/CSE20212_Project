#ifndef UNIT_H
#define UNIT_H

#include <SDL/SDL.h>

#include "Map.h"
#include "View.h"

class Unit {
public:
	Unit(int _x, int _y, int _w, int _h, int _maxhp)
		: id(unitcount++), x(_x), y(_y), w(_w), h(_h), target(_x,_y),
			maxhp(_maxhp), hp(_maxhp) {}
	virtual ~Unit() {};

	virtual int getType() = 0;

	bool inView(View &);

	void draw(View &);
	void drawSelected(View &);

	void move(Uint16, Uint16);

	virtual void update(Map &);

	enum unit_type {
		UT_BLACK_HOLE,
		UT_BUNNY
	};

protected:
	virtual void drawUnit(View &) = 0; // Per-subclass

	const int id; // Which am I?

	Uint16 x, y;             // Where am I?
	const unsigned int w, h; // How big am I?

	struct target {
		target(Uint16 _x, Uint16 _y) : x(_x), y(_y) {}

		Uint16 x, y;
	} target;

	const unsigned int maxhp;
	unsigned int hp;

	static const int LEFT;
        static const int RIGHT;
        static const int UP;
        static const int DOWN;

private:
	static int unitcount; // For unique IDs
};

#endif

