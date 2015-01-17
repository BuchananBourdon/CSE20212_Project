#include <SDL/SDL.h>

#include "BlackHole.h"

void BlackHole::drawUnit(View &view) {
	SDL_Surface *surface = SDL_GetVideoSurface();

	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	rect.w = w*view.zoom;
	rect.h = h*view.zoom;

	Uint32 color = SDL_MapRGB(surface->format,0,0,0);

	SDL_FillRect(surface,&rect,color);
}

void BlackHole::updateUnit() {
}

