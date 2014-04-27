class Unit;

#ifndef UNIT_H
#define UNIT_H

#include <vector>

#include <SDL/SDL.h>

#include "Map.h"
#include "Path.h"
#include "View.h"

class Unit {
public:
	Unit(Map &, int, int, int, int, int, int, bool);
	virtual ~Unit() {};

	static Unit *getById(int id) { return units[id]; }

	int    getId() { return id; }
	Uint16 getX()  { return x; }
	Uint16 getY()  { return y; }

	bool isDead() { return hp == 0; }

	virtual int getType() = 0;

	bool inView(View &);

	void draw(View &);
	void drawSelected(View &);

	void move(Uint16, Uint16);
	void attack(Unit *);

	void update(Map &);

	void hit(Unit *, unsigned int);

	enum unit_type {
		UT_BLACK_HOLE,
		UT_BUNNY,
		UT_ROBOT,
		UT_SPAWN_BUNNY,
		UT_SPAWN_ROBOT
	};

protected:
	virtual void drawUnit(View &) = 0;
	virtual void updateUnit(Map &) = 0;

	const int id; // Which am I?

	Uint16 x, y;       // Where am I?
	const Uint16 w, h; // How big am I?

	enum {
		GOAL_NONE,
		GOAL_DEAD,
		GOAL_MOVE,
		GOAL_ATTACK
	} goal;

	Path *path;

	Unit *target;

	const unsigned int maxhp;
	unsigned int hp;

	const unsigned int power;

	int status;
	int frame;

	static const int LEFT;
        static const int RIGHT;
        static const int UP;
        static const int DOWN;

private:
	static int unitcount;             // For unique IDs
	static std::vector<Unit *> units; // Indexed by id

	void setOccupancy(Map &, bool); // Stake our claim

	void setSelectionClips(); //sets the Rect dimensions for selection

	void followPath(Map &); // One step

	//For the selection icon
	static SDL_Surface * selectSurface;
	static SDL_Rect clipsSelect[17];

	// Flags whether or not the unit can be moved on map
	bool isMovable;
};

#endif
