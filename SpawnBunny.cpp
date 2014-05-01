#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Bunny.h"
#include "CreateUnitOrder.h"
#include "SpawnBunny.h"

SDL_Surface* SpawnBunny::spawnbunnySurface = NULL;
SDL_Rect SpawnBunny::clips[17];

//constructor initializes isMovable to false so we can't move the spawn structures	
SpawnBunny::SpawnBunny(Game &_game, int _ownerid, int _x, int _y)
	: Unit(_game,_ownerid,_x,_y,3,2,4000,0,false) {
	//If it's the first Bunny, then handle the image loading and clip dimensions
	if(spawnbunnySurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("Rainbow.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		spawnbunnySurface = optimizedImage;
		setClips();
	}
}

void SpawnBunny::act() {
	if(game.useResources(Bunny::getCost()))
		game.getTurn()->addOrder(new CreateUnitOrder(Unit::UT_BUNNY,
			x + w/2,y + h));
}

//Draw those pretty rainbows
void SpawnBunny::drawUnit(View & view) {
	//blit the bunny to the screen if it's in view
	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	SDL_BlitSurface(spawnbunnySurface,&clips[16-(((view.zoom+2)/6)-1)],screen,&rect);
}

//Virtual update method. No animation to be done for this unit 
void SpawnBunny::updateUnit() {frame=0;}

//clip the rainbow sprite sheet
void SpawnBunny::setClips() {
	//Algorithm clips the Bunny sprite sheet with 17 different sizes
	for(int i = 0;i<17;i++) {
			clips[i].x = 0;
			clips[i].y = 2*(880-(16-i)*(55-3*i));
			clips[i].h = 2*(100-i*6);
			clips[i].w = 3*(100-i*6); 	
	}
}
