#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Robot.h"

SDL_Surface* Robot::robotSurface = NULL;
SDL_Rect Robot::clipsRight[ 4 ];
SDL_Rect Robot::clipsLeft[ 4 ];
SDL_Rect Robot::clipsUp[ 4 ];
SDL_Rect Robot::clipsDown[ 4 ];
	
//Init these to 68, until we get image resize figured out
int Robot::ROBOT_WIDTH = 68;
int Robot::ROBOT_HEIGHT = 68;

// setting height and width to 5 map tiles until we get image resize figured out
Robot::Robot(int _x, int _y) : Unit(_x,_y,5,5,100)
{
	//Begin each robot facing down and in the "rested" first frame
	status = DOWN;
	frame = 0;

	//If it's the first robot, then handle the image loading and clip dimensions
	if(robotSurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("robot_animation.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		robotSurface = optimizedImage;
		
		setClips();
	}
}


int Robot::getType() { return UT_ROBOT; }


void Robot::drawUnit(View & view) {

	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	if(status==RIGHT) 	SDL_BlitSurface(robotSurface,&clipsRight[frame],screen,&rect);
	else if(status == LEFT) SDL_BlitSurface(robotSurface,&clipsLeft[frame],screen,&rect);
	else if(status == UP) 	SDL_BlitSurface(robotSurface,&clipsUp[frame],screen,&rect);
	else if(status == DOWN)	SDL_BlitSurface(robotSurface,&clipsDown[frame],screen,&rect);
}

void Robot::setClips() {
    
    // Clip the sprites
    clipsRight[ 0 ].x = 0;
    clipsRight[ 0 ].y = ROBOT_HEIGHT * 2;
    clipsRight[ 0 ].w = ROBOT_WIDTH;
    clipsRight[ 0 ].h = ROBOT_HEIGHT;

    clipsRight[ 1 ].x = ROBOT_WIDTH;
    clipsRight[ 1 ].y = ROBOT_HEIGHT * 2;
    clipsRight[ 1 ].w = ROBOT_WIDTH;
    clipsRight[ 1 ].h = ROBOT_HEIGHT;

    clipsRight[ 2 ].x = ROBOT_WIDTH * 2;
    clipsRight[ 2 ].y = ROBOT_HEIGHT * 2;
    clipsRight[ 2 ].w = ROBOT_WIDTH;
    clipsRight[ 2 ].h = ROBOT_HEIGHT;

    clipsRight[ 3 ].x = ROBOT_WIDTH * 3;
    clipsRight[ 3 ].y = ROBOT_HEIGHT * 2;
    clipsRight[ 3 ].w = ROBOT_WIDTH;
    clipsRight[ 3 ].h = ROBOT_HEIGHT;

    clipsLeft[ 0 ].x = 0;
    clipsLeft[ 0 ].y = ROBOT_HEIGHT;
    clipsLeft[ 0 ].w = ROBOT_WIDTH;
    clipsLeft[ 0 ].h = ROBOT_HEIGHT;

    clipsLeft[ 1 ].x = ROBOT_WIDTH;
    clipsLeft[ 1 ].y = ROBOT_HEIGHT;
    clipsLeft[ 1 ].w = ROBOT_WIDTH;
    clipsLeft[ 1 ].h = ROBOT_HEIGHT;

    clipsLeft[ 2 ].x = ROBOT_WIDTH * 2;
    clipsLeft[ 2 ].y = ROBOT_HEIGHT;
    clipsLeft[ 2 ].w = ROBOT_WIDTH;
    clipsLeft[ 2 ].h = ROBOT_HEIGHT;

    clipsLeft[ 3 ].x = ROBOT_WIDTH * 3;
    clipsLeft[ 3 ].y = ROBOT_HEIGHT;
    clipsLeft[ 3 ].w = ROBOT_WIDTH;
    clipsLeft[ 3 ].h = ROBOT_HEIGHT;

    clipsUp[ 0 ].x = 0;
    clipsUp[ 0 ].y = ROBOT_HEIGHT * 3;
    clipsUp[ 0 ].w = ROBOT_WIDTH;
    clipsUp[ 0 ].h = ROBOT_HEIGHT;

    clipsUp[ 1 ].x = ROBOT_WIDTH;
    clipsUp[ 1 ].y = ROBOT_HEIGHT * 3;
    clipsUp[ 1 ].w = ROBOT_WIDTH;
    clipsUp[ 1 ].h = ROBOT_HEIGHT;

    clipsUp[ 2 ].x = ROBOT_WIDTH * 2;
    clipsUp[ 2 ].y = ROBOT_HEIGHT * 3;
    clipsUp[ 2 ].w = ROBOT_WIDTH;
    clipsUp[ 2 ].h = ROBOT_HEIGHT;

    clipsUp[ 3 ].x = ROBOT_WIDTH * 3;
    clipsUp[ 3 ].y = ROBOT_HEIGHT * 3;
    clipsUp[ 3 ].w = ROBOT_WIDTH;
    clipsUp[ 3 ].h = ROBOT_HEIGHT;

    clipsDown[ 0 ].x = 0;
    clipsDown[ 0 ].y = 0;
    clipsDown[ 0 ].w = ROBOT_WIDTH;
    clipsDown[ 0 ].h = ROBOT_HEIGHT;

    clipsDown[ 1 ].x = ROBOT_WIDTH;
    clipsDown[ 1 ].y = 0;
    clipsDown[ 1 ].w = ROBOT_WIDTH;
    clipsDown[ 1 ].h = ROBOT_HEIGHT;

    clipsDown[ 2 ].x = ROBOT_WIDTH * 2;
    clipsDown[ 2 ].y = 0;
    clipsDown[ 2 ].w = ROBOT_WIDTH;
    clipsDown[ 2 ].h = ROBOT_HEIGHT;

    clipsDown[ 3 ].x = ROBOT_WIDTH*3;
    clipsDown[ 3 ].y = 0;
    clipsDown[ 3 ].w = ROBOT_WIDTH;
    clipsDown[ 3 ].h = ROBOT_HEIGHT;			
}
