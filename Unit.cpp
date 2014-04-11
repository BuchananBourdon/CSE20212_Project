#include <SDL/SDL_gfxPrimitives.h>

#include "Unit.h"

int Unit::unitcount = 0;

const int Unit::RIGHT = 0;
const int Unit::LEFT = 1;
const int Unit::UP = 2;
const int Unit::DOWN = 3;

bool Unit::inView(View &view) {
	return x + w > view.x && x < view.x + view.w
		&& y + h > view.y && y < view.y + view.h;
}

// Handle generic stuff like health bars
void Unit::draw(View &view) {
	this->drawUnit(view);
}

// Draw a selection indicator
void Unit::drawSelected(View &view) {
	roundedRectangleRGBA(SDL_GetVideoSurface(),(x - view.x)*view.zoom,
		(y - view.y)*view.zoom,(x + w - view.x)*view.zoom,
		(y + h - view.y)*view.zoom,3,0xFF,0x00,0x00,0xFF);
}

// Set the unit's goal to be to move to (newx, newy)
void Unit::move(Uint16 newx, Uint16 newy) {
	goal = GOAL_MOVE;

	if(path) delete path;
	path = new Path(x,y,newx,newy,w,h);
}

// Deterministically update the unit according to whatever its current goal is
void Unit::update(Map &map) {
	switch(goal) {
	case GOAL_NONE:
		frame = 0;
		break;

	case GOAL_MOVE:
		int startx = x;
		int starty = y;
		
		setOccupancy(map,false);

		Path::Location loc = path->step(map);

		if(loc.x > x) status = RIGHT;
		if(loc.x < x) status = LEFT;
		if(loc.y > y) status = DOWN;
		if(loc.y < y) status = UP;

		// Goal acheived?
		if(loc.x == x && loc.y == y)
			goal = GOAL_NONE;

		x = loc.x;
		y = loc.y;

		setOccupancy(map,true);
		//Only animate if the unit has moved to a new tile
		if(x!=startx || y!=starty) frame++;
		break;
	}
	
	this->updateUnit(map);
}

void Unit::setOccupancy(Map &map, bool occupy) {
	for(Uint16 r = y; r < y + h; r++) {
		for(Uint16 c = x; c < x + w; c++) {
			if(occupy) map.occupy(c,r);
			else map.clear(c,r);
		}
	}
}

