#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <SDL/SDL.h>

class ActionBar {
public:
	ActionBar(int, int);	//constructor	
	int getHeight();	//returns height of bar
	int getWidth();		//returns width of bar
	void draw(int,bool,int,int);	//draws the bar and resource bar

private:
	int h,w;		//height and width data only used for other methods (surface doesnt resize)

	static SDL_Surface* bar; //action bar surface
	static SDL_Surface* button;

	static SDL_Rect clipBar; //clip for action bar surface
	static SDL_Rect clipButton[3][2];

	void setClip();		//method that sets the action bar clip
};

#endif
