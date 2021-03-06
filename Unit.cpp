#include <iostream>

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include "Unit.h"

using namespace std;

const int Unit::attackradius = 4;

int Unit::unitcount = 0;
vector<Unit *> Unit::units;

const int Unit::RIGHT = 0;
const int Unit::LEFT = 1;
const int Unit::UP = 2;
const int Unit::DOWN = 3;

SDL_Surface* Unit::selectSurface = NULL;
SDL_Surface* Unit::deathSurface = NULL;
SDL_Rect Unit::clipsSelect[17];
SDL_Rect Unit::clipsDeath[17][2];

Unit::Unit(Game &_game, int _ownerid, int _x, int _y, int _w, int _h,
		int _maxhp, int _power, bool _isMovable)
		: id(unitcount++), ownerid(_ownerid), game(_game), x(_x),
			y(_y), w(_w), h(_h), goal(GOAL_NONE), path(NULL),
			maxhp(_maxhp), hp(_maxhp), power(_power), status(DOWN),
			frame(0), isMovable(_isMovable) {
	units.push_back(this);

	//The first time a unit is constructed, load the image and set the clips
	if(selectSurface==NULL) {
		SDL_Surface * loadedImage = IMG_Load("Select.png");
		SDL_Surface * loadedImage2 = IMG_Load("Dead.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_Surface * optimizedImage2 = SDL_DisplayFormatAlpha(loadedImage2);
		SDL_FreeSurface(loadedImage);
		SDL_FreeSurface(loadedImage2);
		selectSurface = optimizedImage;
		deathSurface = optimizedImage2;
		setSelectionClips();
		setDeathClips();
	}

	// Make sure we only occupy an empty location
	Uint16 newx, newy;
	Map *map = game.getMap();
	for(unsigned int radius = 0;
		radius < max(map->getWidth(),map->getHeight()); radius++) {
		for(unsigned int pos = 0; pos < 2*radius + 1; pos++) {
			if(!map->isOccupied(newx = x + radius,
				newy = y + radius - pos,w,h)
				&& map->tileType(newx,newy) == Map::TILE_LAND)
				goto empty_spot;

			if(!map->isOccupied(newx = x - radius,
				newy = y + radius - pos,w,h)
				&& map->tileType(newx,newy) == Map::TILE_LAND)
				goto empty_spot;

			if(!map->isOccupied(newx = x + radius - pos,
				newy = y + radius,w,h)
				&& map->tileType(newx,newy) == Map::TILE_LAND)
				goto empty_spot;

			if(!map->isOccupied(newx = x + radius - pos,
				newy = y - radius,w,h)
				&& map->tileType(newx,newy) == Map::TILE_LAND)
				goto empty_spot;
		}
	}

	// Somewhat unlikely in normal gameplay
	cerr << "cannot find an empty location to create a unit" << endl;

empty_spot:
	x = newx;
	y = newy;

	setOccupancy(true);
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

//Set the clips for the dead unit graphics
void Unit::setDeathClips() {
	for(int i=0;i<17;i++) {
		for(int j=0;j<2;j++) {
			clipsDeath[i][j].x = (100-i*6) * j;
			clipsDeath[i][j].y = (880-(16-i)*(55-3*i));
			clipsDeath[i][j].h = clipsDeath[i][j].w = 100-i*6;
		}
	}
}

bool Unit::inView(View &view) {
	return x + w > view.x && x < view.x + view.w
		&& y + h > view.y && y < view.y + view.h;
}

// Handle generic stuff, then delegate to the sub-class
void Unit::draw(View &view) {
	// Don't draw if we're in the fog
	if(game.getMap()->isFoggy(x,y,w,h)) return;

//	if(path) path->draw(view);

	if(!isDead())
		this->drawUnit(view);
	else {
		SDL_Rect rect;
		rect.x = (x - view.x)*view.zoom;
		rect.y = (y - view.y)*view.zoom;
		rect.w = rect.h = view.zoom;
		int type = getType();
		if(type==1 || type==2) {
			SDL_BlitSurface(deathSurface,
				&clipsDeath[16-(((view.zoom+2)/6)-1)][type-1],
				SDL_GetVideoSurface(),&rect);
		}
	}
}

// Draw a selection indicator
void Unit::drawSelected(View &view) {
	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	rect.w = w*view.zoom;
	rect.h = h*view.zoom;

	//Only draw the selection box for movable units
	if(isMovable) {
		// Draw the normal SDL_Surface indicator if the zoom is greater than 10
		if(view.zoom > 10)
			SDL_BlitSurface(selectSurface,
				&clipsSelect[16-(((view.zoom+2)/6)-1)],screen,&rect);
		// If the zoom is 10 or less, than just draw the gfxPrimitive red box,
		// because it's easier to see when the resolution is so low
		else roundedRectangleRGBA(screen,rect.x,rect.y,rect.x + rect.w,
			rect.y + rect.h,3,0xFF,0,0,0xFF);
	}

	rect.w = w*view.zoom;
	rect.h = h*view.zoom;

	// Draw the health bar
	Uint8 r, g, b;
	if(hp < 0.3*maxhp) r = 0xFF, g = 0, b = 0;
	else if(hp < 0.7*maxhp) r = 0xFF, g = 0xFF, b = 0;
	else r = 0, g = 0xFF, b = 0;

	if(rect.x>0 && rect.y>0)
		boxRGBA(screen,rect.x,rect.y,rect.x + rect.w*hp/maxhp,
			rect.y + .1*rect.h,r,g,b,0xFF);
}

// Set the unit's goal to be to move to (newx, newy)
void Unit::move(Uint16 newgroup, Uint16 newx, Uint16 newy) {
	if(isMovable) {
		goal = GOAL_MOVE;

		group = newgroup;

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
void Unit::update() {
	int dx, dy;
	Unit *enemy;

	// Dead men tell no tales...about goals
	if(isDead()) {
		goal = GOAL_DEAD;
	}

	switch(goal) {
	case GOAL_NONE:
		frame = 0;

		// Attack if an enemy is nearby
		enemy = NULL;
		for(int r = 1; !enemy && r <= attackradius; r++) {
			for(int p = 0; !enemy && p < r; p++) {
				   (enemy = getEnemy(x + r,y + r - p))
				|| (enemy = getEnemy(x - r,y + r - p))
				|| (enemy = getEnemy(x + r - p,y + r))
				|| (enemy = getEnemy(x + r - p,y - r));
			}
		}

		if(enemy) attack(enemy);
		break;

	case GOAL_DEAD:
		break;

	case GOAL_MOVE:
		if(followPath()) game.groupMove(group);

		// Goal achieved/giving up?
		if(path->isFinished() || !game.groupMoved(group))
			goal = GOAL_NONE;
		break;

	case GOAL_ATTACK:
		// Where is our target, relative to us?
		dx = (signed) target->getX() - x;
		dy = (signed) target->getY() - y;

		// Is it where we want it?
		if(isAdjacent(target)) {
			if(dx > 0) status = RIGHT;
			if(dx < 0) status = LEFT;
			if(dy > 0) status = DOWN;
			if(dy < 0) status = UP;

			target->hit(this,power);
		} else { // We need to get closer
			if(path) delete path;
			path = new Path(x,y,target->getX(),target->getY(),w,h);
			followPath();
		}

		// Did we win?
		if(target->isDead())
			goal = GOAL_NONE;
		break;

	default:
		cerr << "unknown goal (" << goal << ")" << endl;
		break;
	}

	this->updateUnit();
}

Unit *Unit::getEnemy(Uint16 x, Uint16 y) {
	Unit *unit;

	Map *map = game.getMap();
	if(map->isOccupied(x,y)) {
		unit = Unit::getById(map->getOccupier(x,y));

		if(!unit->isDead() && ownerid != unit->getOwnerId())
			return unit;
	}

	return NULL;
}

void Unit::setOccupancy(bool occupy) {
	Map *map = game.getMap();

	for(Uint16 r = y; r < y + h; r++) {
		for(Uint16 c = x; c < x + w; c++) {
			if(occupy) map->occupy(c,r,id);
			else map->clear(c,r);
		}
	}

	if(occupy && ownerid == game.getPlayerId())
		map->defog(x,y,max(w,h) + 2);
}

// Returns whether we actually moved along the path
bool Unit::followPath() {
	bool moved = false;

	Map *map = game.getMap();

	setOccupancy(false);

	Path::Location loc = path->step(*map);

	if(loc.x > x) status = RIGHT;
	if(loc.x < x) status = LEFT;
	if(loc.y > y) status = DOWN;
	if(loc.y < y) status = UP;

	if(map->isOccupied(loc.x,loc.y))
		cerr << "warning: ordered to move to occupied location ("
			<< loc.x << ", " << loc.y << ")" << endl;

	if(x != loc.x || y != loc.y) {
		moved = true;
		frame++;
	}

	x = loc.x;
	y = loc.y;

	setOccupancy(true);

	return moved;
}

// Returns whether there is no space between this and that
bool Unit::isAdjacent(Unit *that) {
	struct { int x, y, w, h; } thisbb = {
		x - 1, y - 1,
		w + 2, h + 2
	}, thatbb = {
		that->getX(), that->getY(),
		that->getWidth(), that->getHeight()
	};

	// thisbb and thatbb intersect iff
	//  thisbb.{x,y} is in thatbb
	// or
	//  thatbb.{x,y} is in thisbb

	return (thisbb.x >= thatbb.x && thisbb.x < thatbb.x + thatbb.w
		&& thisbb.y >= thatbb.y && thisbb.y < thatbb.y + thatbb.h)
	|| (thatbb.x >= thisbb.x && thatbb.x < thisbb.x + thisbb.w
		&& thatbb.y >= thisbb.y && thatbb.y < thisbb.y + thisbb.h);
}

// Take damage proportional to power
void Unit::hit(Unit *attacker, unsigned int power) {
	if(hp > power) hp -= power;
	else hp = 0;

	// Did we die?
	if(isDead()) {
		goal = GOAL_DEAD;

		setOccupancy(false);
	}

	// Retaliate, if we're doing nothing else
	if(goal == GOAL_NONE)
		attack(attacker);
}

