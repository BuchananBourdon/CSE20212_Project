#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Bunny.h"
#include <cmath>

SDL_Surface* Bunny::bunnySurface = NULL;
SDL_Rect Bunny::clipsRight[17][ 4 ];
SDL_Rect Bunny::clipsLeft[17][ 4 ];
SDL_Rect Bunny::clipsUp[17][ 4 ];
SDL_Rect Bunny::clipsDown[17][ 4 ];
	
Bunny::Bunny(Map &_map, int _x, int _y) : Unit(_map,_x,_y,1,1,100,DOWN,0)
{
	//If it's the first Bunny, then handle the image loading and clip dimensions
	if(bunnySurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("Bunny.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		bunnySurface = optimizedImage;
		
		setClips();
	}
}


int Bunny::getType() { return UT_BUNNY; }


void Bunny::drawUnit(View & view) {
	//blit the bunny to the screen if it's in view
	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	if(status==RIGHT) 	SDL_BlitSurface(bunnySurface,&clipsRight[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == LEFT) SDL_BlitSurface(bunnySurface,&clipsLeft[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == UP) 	SDL_BlitSurface(bunnySurface,&clipsUp[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == DOWN)	SDL_BlitSurface(bunnySurface,&clipsDown[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
}

//Virtual update method handles Bunny frame and status 
void Bunny::updateUnit(Map &map) {
	//loop frame (Bunny animation has 4 frames)
	if(frame>=4)	frame=0;	
}

void Bunny::setClips() {
	//Algorithm clips the Bunny sprite sheet with 17 different sizes
	for(int i = 0;i<17;i++) {
		for(int j = 0;j<4;j++) {
			clipsRight[i][j].x = (100-i*6) * j;
			clipsRight[i][j].y = 4*(880-(16-i)*(55-3*i));
			clipsRight[i][j].h = 100-i*6;
			clipsRight[i][j].w = j==1 ? 100-i*6-(16-i) : 100-i*6; 	

			clipsLeft[i][j].x = (100-i*6) * j;
                        clipsLeft[i][j].y = 4*(880-(16-i)*(55-3*i))+(100-6*i);
                        clipsLeft[i][j].h = 100-i*6;
                        clipsLeft[i][j].w = j==1 ? 100-i*6-(16-i) : 100-i*6; 	// Makes these clips just a few pixels less wide 
										// because the images slightly overlap

			clipsUp[i][j].x = j<3 ? (100-i*6)*j : (100-i*6)*1; //4th frame is really just second frame again
			clipsUp[i][j].y = 4*(880-(16-i)*(55-3*i)) + 2*(100-6*i);
			clipsUp[i][j].w = 100-i*6;
			clipsUp[i][j].h = 100-i*6;

			clipsDown[i][j].x = j<3 ? (100-i*6)*j : (100-i*6)*1; //4th frame is really just second frame again
			clipsDown[i][j].y = 4*(880-(16-i)*(55-3*i)) + 3*(100-6*i);
			clipsDown[i][j].w = 100-i*6;
			clipsDown[i][j].h = 100-i*6;
		}
	}
}
