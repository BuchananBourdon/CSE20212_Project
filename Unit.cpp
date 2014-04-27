#include <iostream>

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include "Unit.h"

using namespace std;

int Unit::unitcount = 0;
vector<Unit *> Unit::units;

const int Unit::RIGHT = 0;
const int Unit::LEFT = 1;
const int Unit::UP = 2;
const int Unit::DOWN = 3;

SDL_Surface* Unit::selectSurface = NULL;
SDL_Rect Unit::clipsSelect[17];

Unit::Unit(Map &map, int _x, int _y, int _w, int _h, int _maxhp, int _power,
		bool _isMovable)
		: id(unitcount++), x(_x), y(_y), w(_w), h(_h), goal(GOAL_NONE),
			path(NULL), maxhp(_maxhp), hp(_maxhp), power(_power),
			status(DOWN), frame(0), isMovable(_isMovable)  {
	units.push_back(this);

	//The first time a unit is constructed, load the image and set the clips
	if(selectSurface==NULL) {
		SDL_Surface * loadedImage = IMG_Load("Select.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
		selectSurface = optimizedImage;
		setSelectionClips();
	}

	setOccupancy(map,true);
}

// Sets the clips for the selection indicator
void Unit::setSelectionClips() {

	for(int i=0;i<17;i++) {
		clipsSelect[i].x = 0;
		clipsSelect[i].y = 880-(16-i)*(55-3*i);
		clipsSelect[i].w = 100-i*6;
		clipsSelect[i].h = 100-i*6;
	}
}

bool Unit::inView(View &view) {
	return x + w > view.x && x < view.x + view.w
		&& y + h > view.y && y < view.y + view.h;
}

// Handle generic stuff, then delegate to the sub-class
void Unit::draw(View &view) {
//	if(path) path->draw(view);

	if(!isDead()) // TODO: add deadness frames
		this->drawUnit(view);
}

// Draw a selection indicator
void Unit::drawSelected(View &view) {
	SDL_Surface *screen = SDL_GetVideoSurface();

	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	rect.w = rect.h = view.zoom;

	// Draw the normal SDL_Surface indicator if the zoom is greater than 10
	if(view.zoom > 10)
		SDL_BlitSurface(selectSurface,
			&clipsSelect[16-(((view.zoom+2)/6)-1)],screen,&rect);
	// If the zoom is 10 or less, than just draw the gfxPrimitive red box,
	// because it's easier to see when the resolution is so low
	else roundedRectangleRGBA(screen,rect.x,rect.y,rect.x + rect.w,
		rect.y + rect.h,3,0xFF,0,0,0xFF);

	// Draw the health bar
	Uint8 r, g, b;
	if(hp < 0.3*maxhp) r = 0xFF, g = 0, b = 0;
	else if(hp < 0.7*maxhp) r = 0xFF, g = 0xFF, b = 0;
	else r = 0, g = 0xFF, b = 0;

	boxRGBA(screen,rect.x,rect.y,rect.x + rect.w*hp/maxhp,
		rect.y + 0.1*rect.h,r,g,b,0xFF);
}

// Set the unit's goal to be to move to (newx, newy)
void Unit::move(Uint16 newx, Uint16 newy) {
	if(isMovable) {
		goal = GOAL_MOVE;

		if(path) delete path;
		path = new Path(x,y,newx,newy,w,h);
	}
}

// Set the unit's goal to be to attack _target
void Unit::attack(Unit *_target) {
	if(isMovable) {
		goal = GOAL_ATTACK;

		target = _target;
	}
}

// Deterministically update the unit according to whatever its current goal is
void Unit::update(Map &map) {
	int dx, dy;

	// Dead men tell no tales...about goals
	if(isDead()) {
		goal = GOAL_DEAD;
	}

	switch(goal) {
	case GOAL_NONE:
		frame = 0;
		break;

	case GOAL_DEAD:
		setOccupancy(map,false);
		break;

	case GOAL_MOVE:
		followPath(map);

		// Goal achieved?
		if(path->isFinished())
			goal = GOAL_NONE;
		break;

	case GOAL_ATTACK:
		// Where is our target, relative to us?
		dx = (signed) target->getX() - x;
		dy = (signed) target->getY() - y;

		// Do we need to get closer?
		if(abs(dx) > 1 || abs(dy) > 1) {
			if(path) delete path;
			path = new Path(x,y,target->getX(),target->getY(),w,h);
			followPath(map);
		} else { // It's right where we want it
			if(dx > 0) status = RIGHT;
			if(dx < 0) status = LEFT;
			if(dy > 0) status = DOWN;
			if(dy < 0) status = UP;

			target->hit(this,power);

			// Did we win?
			if(target->isDead())
				goal = GOAL_NONE;
		}
		break;

	default:
		cerr << "unknown goal (" << goal << ")" << endl;
		break;
	}

	this->updateUnit(map);
}

void Unit::setOccupancy(Map &map, bool occupy) {
	for(Uint16 r = y; r < y + h; r++) {
		for(Uint16 c = x; c < x + w; c++) {
			if(occupy) map.occupy(c,r,id);
			else map.clear(c,r);
		}
	}
}

// Go one step along path
void Unit::followPath(Map &map) {
	setOccupancy(map,false);

	Path::Location loc = path->step(map);

	if(loc.x > x) status = RIGHT;
	if(loc.x < x) status = LEFT;
	if(loc.y > y) status = DOWN;
	if(loc.y < y) status = UP;

	if(map.isOccupied(loc.x,loc.y))
		cerr << "warning: ordered to move to occupied location ("
			<< loc.x << ", " << loc.y << ")" << endl;

	if(x != loc.x || y != loc.y)
		frame++;

	x = loc.x;
	y = loc.y;

	setOccupancy(map,true);
}

// Take damage proportional to power
void Unit::hit(Unit *attacker, unsigned int power) {
	if(hp > power) hp -= power;
	else hp = 0;

	// Retaliate, if we're doing nothing else
	if(goal == GOAL_NONE)
		attack(attacker);
}

