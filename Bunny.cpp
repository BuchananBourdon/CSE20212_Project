#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Bunny.h"
#include <cmath>

SDL_Surface* Bunny::bunnySurface = NULL;
SDL_Rect Bunny::clipsRight[ 4 ];
SDL_Rect Bunny::clipsLeft[ 4 ];
SDL_Rect Bunny::clipsUp[ 4 ];
SDL_Rect Bunny::clipsDown[ 4 ];
	
//Init these to 32, until we get image resize figured out
int Bunny::BUNNY_WIDTH = 32;
int Bunny::BUNNY_HEIGHT = 32;

Bunny::Bunny(int _x, int _y) : Unit(_x,_y,1,1,100)
{
	//Begin each bunny facing down and in the "rested" first frame
	status = DOWN;
	frame = 0;

	//If it's the first Bunny, then handle the image loading and clip dimensions
	if(bunnySurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("rabbit_animations.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		bunnySurface = optimizedImage;
		
		setClips();
	}
}


int Bunny::getType() { return UT_BUNNY; }


void Bunny::drawUnit(View & view) {

	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	if(status==RIGHT) 	SDL_BlitSurface(bunnySurface,&clipsRight[frame],screen,&rect);
	else if(status == LEFT) SDL_BlitSurface(bunnySurface,&clipsLeft[frame],screen,&rect);
	else if(status == UP) 	SDL_BlitSurface(bunnySurface,&clipsUp[frame],screen,&rect);
	else if(status == DOWN)	SDL_BlitSurface(bunnySurface,&clipsDown[frame],screen,&rect);
}

//Virtual update method handles Bunny frame and status 
void Bunny::update(Map &map) {
        int dx = target.x - x;
        int dy = target.y - y;

        if(!dx || !dy) { // Trivial cases - move along axis, or not at all
                if(dx) {
			x += dx > 0 ? 1 : -1;
			if(dx>0)	status = RIGHT;
			else if(dx<0)	status = LEFT;
		}
                if(dy) {
			y += dy > 0 ? 1 : -1;
			if(dy>0)	status = DOWN;
			else if(dy<0)	status = UP;
		}
		frame++;

        } else { // Non-right angle
                if(pow(1 + fabs(dx)/fabs(dy),2) > 2 && pow(1 + fabs(dy)/fabs(dx),2) <= 2) {
                        x += dx > 0 ? 1 : -1;
			if(dx>0)	status = RIGHT;
			else if(dx<0)	status = LEFT;
		}
                if(pow(1 + fabs(dy)/fabs(dx),2) > 2 && pow(1 + fabs(dx)/fabs(dy),2) <= 2) {
                        y += dy > 0 ? 1 : -1;
			if(dy>0)	status = DOWN;
			else if(dy<0)	status = UP;
		}
		if(pow(1 + fabs(dy)/fabs(dx),2) > 2 && pow(1 + fabs(dx)/fabs(dy),2) > 2) {
			x += dx > 0 ? 1 : -1;
			y += dy > 0 ? 1 : -1;
			if(dy>0)	status = DOWN;
			else if(dy<0)	status = UP;
		}
		
		frame++;
        }

	// Don't go off the edge of the map
        Uint16 maxx = map.getWidth() - w;
        Uint16 maxy = map.getHeight() - h;
        if(x > maxx) x = maxx;
        if(y > maxy) y = maxy;

	//Don't let Bunny finish in "mid-run" frame and be sure to loop frame 
	if(!dx && !dy)	frame=0;	
	if(frame>=4)	frame=0;	
}

void Bunny::setClips() {
	
	//Clip the sprites
    	clipsRight[ 0 ].x = 0;
    	clipsRight[ 0 ].y = 0;
    	clipsRight[ 0 ].w = BUNNY_WIDTH;
    	clipsRight[ 0 ].h = BUNNY_HEIGHT;

    	clipsRight[ 1 ].x = BUNNY_WIDTH;
    	clipsRight[ 1 ].y = 0;
    	clipsRight[ 1 ].w = BUNNY_WIDTH;
    	clipsRight[ 1 ].h = BUNNY_HEIGHT;

    	clipsRight[ 2 ].x = BUNNY_WIDTH * 2;
    	clipsRight[ 2 ].y = 0;
    	clipsRight[ 2 ].w = BUNNY_WIDTH;
    	clipsRight[ 2 ].h = BUNNY_HEIGHT;

    	clipsRight[ 3 ].x = BUNNY_WIDTH * 3;
    	clipsRight[ 3 ].y = 0;
    	clipsRight[ 3 ].w = BUNNY_WIDTH;
    	clipsRight[ 3 ].h = BUNNY_HEIGHT;

    	clipsLeft[ 0 ].x = 0;
    	clipsLeft[ 0 ].y = BUNNY_HEIGHT;
    	clipsLeft[ 0 ].w = BUNNY_WIDTH;
    	clipsLeft[ 0 ].h = BUNNY_HEIGHT;

    	clipsLeft[ 1 ].x = BUNNY_WIDTH;
    	clipsLeft[ 1 ].y = BUNNY_HEIGHT;
    	clipsLeft[ 1 ].w = BUNNY_WIDTH;
    	clipsLeft[ 1 ].h = BUNNY_HEIGHT;

    	clipsLeft[ 2 ].x = BUNNY_WIDTH * 2;
 	clipsLeft[ 2 ].y = BUNNY_HEIGHT;
    	clipsLeft[ 2 ].w = BUNNY_WIDTH;
    	clipsLeft[ 2 ].h = BUNNY_HEIGHT;

    	clipsLeft[ 3 ].x = BUNNY_WIDTH * 3;
    	clipsLeft[ 3 ].y = BUNNY_HEIGHT;
    	clipsLeft[ 3 ].w = BUNNY_WIDTH;
    	clipsLeft[ 3 ].h = BUNNY_HEIGHT;

	clipsUp[0].x = BUNNY_WIDTH * 0;
        clipsUp[0].y = BUNNY_HEIGHT * 2;
        clipsUp[0].w = BUNNY_WIDTH;
        clipsUp[0].h = BUNNY_HEIGHT;

        clipsUp[1].x = BUNNY_WIDTH * 1;
        clipsUp[1].y = BUNNY_HEIGHT * 2;
        clipsUp[1].w = BUNNY_WIDTH;
        clipsUp[1].h = BUNNY_HEIGHT;

        clipsUp[2].x = BUNNY_WIDTH * 2;
        clipsUp[2].y = BUNNY_HEIGHT * 2;
        clipsUp[2].w = BUNNY_WIDTH;
        clipsUp[2].h = BUNNY_HEIGHT;

        clipsUp[3].x = BUNNY_WIDTH * 1;
        clipsUp[3].y = BUNNY_HEIGHT * 2;
        clipsUp[3].w = BUNNY_WIDTH;
      	clipsUp[3].h = BUNNY_HEIGHT;


        clipsDown[0].x = 0;
        clipsDown[0].y = BUNNY_HEIGHT * 3;
        clipsDown[0].w = BUNNY_WIDTH;
        clipsDown[0].h = BUNNY_HEIGHT;

        clipsDown[1].x = BUNNY_WIDTH;
        clipsDown[1].y = BUNNY_HEIGHT * 3;
        clipsDown[1].w = BUNNY_WIDTH;
        clipsDown[1].h = BUNNY_HEIGHT;

        clipsDown[2].x = BUNNY_WIDTH * 2;
        clipsDown[2].y = BUNNY_HEIGHT * 3;
        clipsDown[2].w = BUNNY_WIDTH;
        clipsDown[2].h = BUNNY_HEIGHT;

        clipsDown[3].x = BUNNY_WIDTH;
        clipsDown[3].y = BUNNY_HEIGHT * 3;
        clipsDown[3].w = BUNNY_WIDTH;
        clipsDown[3].h = BUNNY_HEIGHT;			
}
