#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include "Unit.h"

int Unit::unitcount = 0;

const int Unit::RIGHT = 0;
const int Unit::LEFT = 1;
const int Unit::UP = 2;
const int Unit::DOWN = 3;

SDL_Surface* Unit::selectSurface = NULL;
SDL_Rect Unit::clipsSelect[17];

Unit::Unit(Map &map, int _x, int _y, int _w, int _h, int _maxhp, int _status,
		int _frame)
                : id(unitcount++), x(_x), y(_y), w(_w), h(_h), goal(GOAL_NONE),
                        path(NULL), maxhp(_maxhp), hp(_maxhp), status(_status),
                        frame(_frame) {
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

// Handle generic stuff like health bars
void Unit::draw(View &view) {
	this->drawUnit(view);
}

// Draw a selection indicator
void Unit::drawSelected(View &view) {
	if(view.zoom>10) //Draw the normal SDL_Surface indicator if the zoom is greater than 10
	{
        	SDL_Rect rect;
        	rect.x = (x - view.x)*view.zoom;
        	rect.y = (y - view.y)*view.zoom;
        	SDL_BlitSurface(selectSurface,&clipsSelect[16-(((view.zoom+2)/6)-1)],
			SDL_GetVideoSurface(),&rect);
	}
	else {	//If the zoom is 10 or less, than just draw the gfxPrimitive red box because
		//it's easier to see when the resolution is so low
		roundedRectangleRGBA(SDL_GetVideoSurface(),(x - view.x)*view.zoom,
			(y - view.y)*view.zoom,(x + w - view.x)*view.zoom,
			(y + h - view.y)*view.zoom,3,0xFF,0x00,0x00,0xFF);
	}
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

		// Goal acheived/failed?
		if((loc.x == x && loc.y == y) || map.isOccupied(loc.x,loc.y))
			goal = GOAL_NONE;

		if(!map.isOccupied(loc.x,loc.y)) {
			x = loc.x;
			y = loc.y;
		}

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

