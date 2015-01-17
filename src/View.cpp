#include <algorithm>
#include <cmath>

#include <SDL/SDL.h>

#include "View.h"

using namespace std;

const Uint16 View::minzoom = 10;
const Uint16 View::maxzoom = 100;
const Uint16 View::zoomstep = 6;

View::View(Uint16 _x, Uint16 _y, Uint16 _zoom) : x(_x), y(_y), zoom(_zoom) {
	zoom = max(zoom,minzoom);
	zoom = min(zoom,maxzoom);

	SDL_Surface *screen = SDL_GetVideoSurface();
	w = ceil((float) screen->w/zoom);
	h = ceil((float) screen->h/zoom);
}

void View::adjustZoom(Map &map, int dzoom, int mx, int my) {
	struct { float x, y; } to;
	to.x = x + (float) mx/zoom;
	to.y = y + (float) my/zoom;

	SDL_Surface *screen = SDL_GetVideoSurface();

	// Bounds-check the new zoom level
	Uint16 newzoom = zoom + dzoom*zoomstep;
	if(newzoom >= minzoom && newzoom <= maxzoom)
		zoom = newzoom;

	// Update the view specification
	w = ceil((float) screen->w/zoom);
	h = ceil((float) screen->h/zoom);

	x = to.x - (float) mx/zoom;
	y = to.y - (float) my/zoom;

	// More bounds checking
	x = x < 0 ? 0 : x < map.getWidth() - w ? x : map.getWidth() - w;
	y = y < 0 ? 0 : y < map.getHeight() - h ? y : map.getHeight() - h;
}

