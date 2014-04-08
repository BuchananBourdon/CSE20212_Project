#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Bunny.h"

SDL_Surface* Bunny::bunnySurface = NULL;
SDL_Rect Bunny::clip;

// setting height and width to 5 map tiles until we get image resize figured out
Bunny::Bunny(int _x, int _y) : Unit(_x,_y,5,5,100)
{
	//Init these to 32, until we get image resize figured out
	BUNNYWIDTH = 32;
        BUNNYHEIGHT = 32;

	//If it's the first Bunny, then handle the image loading and clip dimensions
	if(bunnySurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("rabbit_animations.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		bunnySurface = optimizedImage;

		clip.x = 0;
		clip.y = 0;
		clip.w = BUNNYWIDTH;
		clip.h = BUNNYHEIGHT;
	}
}


int Bunny::getType() { return UT_BUNNY; }


void Bunny::drawUnit(View & view) {

	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	SDL_BlitSurface(bunnySurface,&clip,screen,&rect);
}
