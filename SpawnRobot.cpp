#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "CreateUnitOrder.h"
#include "SpawnRobot.h"

SDL_Surface* SpawnRobot::spawnrobotSurface = NULL;
SDL_Rect SpawnRobot::clips[17];


SpawnRobot::SpawnRobot(Game &_game, int _playerid, int _x, int _y)
	: Unit(_game,_playerid,_x,_y,3,3,10000,0,false) {
	// Load the spawn image if it hasn't been loaded yet
	if(spawnrobotSurface == NULL) {
		SDL_Surface * loadedImage = IMG_Load("RobotSpawn.png");
		SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_FreeSurface(loadedImage);
		spawnrobotSurface = optimizedImage;
		setClips();
	}
}

int SpawnRobot::getType() { return UT_SPAWN_ROBOT; }

void SpawnRobot::act() {
	game.getTurn()->addOrder(new CreateUnitOrder(Unit::UT_ROBOT,x + w/2,
		y + h));
}

// Blit the robot spawn structure to the screen
void SpawnRobot::drawUnit(View & view) {
	SDL_Surface * screen = SDL_GetVideoSurface();
	SDL_Rect rect;
	rect.x = (x - view.x)*view.zoom;
	rect.y = (y - view.y)*view.zoom;
	SDL_BlitSurface(spawnrobotSurface,&clips[16-(((view.zoom+2)/6)-1)],screen,&rect);
}

// Virtual method. No animation to be done for this unit
void SpawnRobot::updateUnit() { frame=0; }

//Set the clips for the robot spawn structure
void SpawnRobot::setClips() {
	for(int i = 0;i<17;i++) {
		clips[i].x = 0;
		clips[i].y = 3*(880-(16-i)*(55-3*i));
		clips[i].h = 3*(100-i*6);
		clips[i].w = 3*(100-i*6);
	}
}
