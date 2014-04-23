#include <SDL/SDL.h>

#include "View.h"

const int View::minzoom = 10;
const int View::maxzoom = 100;
const int View::zoomstep = 6;

void View::adjustZoom(Map &map, int dzoom, int mx, int my) {
	struct { float x, y; } to;
	to.x = x + (float) mx/zoom;
	to.y = y + (float) my/zoom;

	SDL_Surface *surface = SDL_GetVideoSurface();

	// Bounds-check the new zoom level
	Uint16 newzoom = zoom + dzoom*zoomstep;
	if(newzoom >= minzoom && newzoom <= maxzoom)
		zoom = newzoom;

	// Update the view specification
	w = (surface->w + zoom - 1)/zoom;
	h = (surface->h + zoom - 1)/zoom;

	x = to.x - (float) mx/zoom;
	y = to.y - (float) my/zoom;

	// More bounds checking
	x = x < 0 ? 0 : x < map.getWidth() - w ? x : map.getWidth() - w;
	y = y < 0 ? 0 : y < map.getHeight() - h ? y : map.getHeight() - h;
}

