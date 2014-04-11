#ifndef UNIT_H
#define UNIT_H

#include <SDL/SDL.h>

#include "Map.h"
#include "Path.h"
#include "View.h"

class Unit {
public:
	Unit(int _x, int _y, int _w, int _h, int _maxhp, int _status,
		int _frame)
		: id(unitcount++), x(_x), y(_y), w(_w), h(_h), goal(GOAL_NONE),
			path(NULL), maxhp(_maxhp), hp(_maxhp), status(_status),
			frame(_frame) {}
	virtual ~Unit() {};

	virtual int getType() = 0;

	bool inView(View &);

	void draw(View &);
	void drawSelected(View &);

	void move(Uint16, Uint16);

	void update(Map &);

	enum unit_type {
		UT_BLACK_HOLE,
		UT_BUNNY
	};

protected:
	virtual void drawUnit(View &) = 0; // Per-subclass
	virtual void updateUnit(Map &) = 0;

	const int id; // Which am I?

	Uint16 x, y;       // Where am I?
	const Uint16 w, h; // How big am I?

	enum {
		GOAL_NONE,
		GOAL_MOVE
	} goal;

	Path *path;

	const unsigned int maxhp;
	unsigned int hp;

	int status;
	int frame;

	static const int LEFT;
        static const int RIGHT;
        static const int UP;
        static const int DOWN;

private:
	void setOccupancy(Map &, bool); // Stake our claim

	static int unitcount; // For unique IDs
};

#endif

