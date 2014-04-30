#include <SDL/SDL.h>
#include "SDL/SDL_image.h"

#include "Robot.h"

SDL_Surface* Robot::robotSurface = NULL;
SDL_Rect Robot::clipsRight[17][ 4 ];
SDL_Rect Robot::clipsLeft[17][ 4 ];
SDL_Rect Robot::clipsUp[17][ 4 ];
SDL_Rect Robot::clipsDown[17][ 4 ];
	
Robot::Robot(Map &_map, int _x, int _y, bool _local)
	: Unit(_map,_x,_y,1,1,100,10,true,_local)
{
	//If it's the first robot, then handle the image loading and clip dimensions
	if(robotSurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("Robot.png");
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
	if(status==RIGHT) 	SDL_BlitSurface(robotSurface,&clipsRight[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == LEFT) SDL_BlitSurface(robotSurface,&clipsLeft[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == UP) 	SDL_BlitSurface(robotSurface,&clipsUp[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
	else if(status == DOWN)	SDL_BlitSurface(robotSurface,&clipsDown[16-(((view.zoom+2)/6)-1)][frame],screen,&rect);
}

void Robot::updateUnit() {
	//loop frame (robot animation has 4 frames)
	if(frame>=4)	frame=0;
}

void Robot::setClips() {
    
	for(int i = 0;i<17;i++) {
		for(int j=0;j<4;j++) {
			//Robot frame animations begin in middle of PNG img
			clipsRight[i][j].x = j<2 ? (100-i*6)*(j+1) : (100-i*6)*(3-j); 
			clipsRight[i][j].y = 4*(880-(16-i)*(55-3*i)) + 2*(100-6*i); 
			clipsRight[i][j].w = 100-i*6;
			clipsRight[i][j].h = 100-i*6;

			clipsLeft[i][j].x = j<2 ? (100-i*6)*(j+1) : (100-i*6)*(3-j);
                        clipsLeft[i][j].y = 4*(880-(16-i)*(55-3*i)) + 1*(100-6*i);
                        clipsLeft[i][j].w = 100-i*6;
                        clipsLeft[i][j].h = 100-i*6;

			clipsUp[i][j].x = j<2 ? (100-i*6)*(j+1) : (100-i*6)*(3-j);
                        clipsUp[i][j].y = 4*(880-(16-i)*(55-3*i)) + 3*(100-6*i);
                        clipsUp[i][j].w = 100-i*6;
                        clipsUp[i][j].h = 100-i*6;

			clipsDown[i][j].x = j<2 ? (100-i*6)*(j+1) : (100-i*6)*(3-j);
                        clipsDown[i][j].y = 4*(880-(16-i)*(55-3*i));
                        clipsDown[i][j].w = 100-i*6;
                        clipsDown[i][j].h = 100-i*6;
		}
	}

}


