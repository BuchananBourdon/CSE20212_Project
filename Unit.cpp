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

