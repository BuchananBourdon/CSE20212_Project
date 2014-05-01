#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Cannon.h"
#include <cmath>

SDL_Surface* Cannon::cannonSurface = NULL;
SDL_Rect Cannon::clipsCarrotCannon[17];
SDL_Rect Cannon::clipsLaserCannon[17];

Cannon::Cannon(Game &_game, int _ownerid, int _x, int _y)
	: Unit(_game,_ownerid,_x,_y,1,1,1000,0,false) {
	//If it's the first cannon, handle the image loading and clipping
	if(cannonSurface==NULL) {
		SDL_Surface * loadedImage = IMG_Load("Cannons.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
		cannonSurface = optimizedImage;
		setClips();
	}
}

void Cannon::drawUnit(View & view) {
	SDL_Surface * surface = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x-view.x)*view.zoom;
	rect.y = (y-view.y)*view.zoom;
	if(ownerid==0)
		SDL_BlitSurface(cannonSurface,&clipsCarrotCannon[16-(((view.zoom+2)/6)-1)],surface,&rect);
	else if(ownerid==1)
		SDL_BlitSurface(cannonSurface,&clipsLaserCannon[16-(((view.zoom+2)/6)-1)],surface,&rect);
}

void Cannon::updateUnit() { frame=0; }

void Cannon::setClips() {
	for(int i = 0;i<17;i++) {
		clipsCarrotCannon[i].x = 0;
		clipsCarrotCannon[i].y = (880-(16-i)*(55-3*i));
		clipsCarrotCannon[i].w = 100-i*6;
		clipsCarrotCannon[i].h = 100-i*6;

		clipsLaserCannon[i].x = (100-i*6);
		clipsLaserCannon[i].y = (880-(16-i)*(55-3*i));
		clipsLaserCannon[i].w = 100-i*6;
		clipsLaserCannon[i].h = 100-i*6;
	}
}
