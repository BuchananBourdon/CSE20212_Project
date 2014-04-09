#include <cmath>

#include "SDL/SDL_gfxPrimitives.h"

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

// Set the unit's goal to be to move to (x, y)
void Unit::move(Uint16 x, Uint16 y) {
	target.x = x;
	target.y = y;
}

// Deterministically update the unit according to whatever its current goal is
void Unit::update(Map &map) {
	int dx = target.x - x;
	int dy = target.y - y;

	if(!dx || !dy) { // Trivial cases - move along axis, or not at all
		if(dx) x += dx > 0 ? 1 : -1;
		if(dy) y += dy > 0 ? 1 : -1;
	} else { // Non-right angle
		if(pow(1 + fabs(dx)/fabs(dy),2) > 2)
			x += dx > 0 ? 1 : -1;
		if(pow(1 + fabs(dy)/fabs(dx),2) > 2)
			y += dy > 0 ? 1 : -1;
	}

	// Don't go off the edge of the map
	Uint16 maxx = map.getWidth() - w;
	Uint16 maxy = map.getHeight() - h;
	if(x > maxx) x = maxx;
	if(y > maxy) y = maxy;
}

