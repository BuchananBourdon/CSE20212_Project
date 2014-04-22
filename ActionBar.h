#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <SDL/SDL.h>

class ActionBar {
public:
	ActionBar(int, int);	//constructor	
	int getHeight();	//returns height of bar
	int getWidth();		//returns width of bar
	void draw(int,bool);	//draws the bar and resource bar

private:
	int h,w;		//height and width data only used for other methods (surface doesnt resize)
	
	/* ***To be added***
	ActionButton spawn;
	ActionButton turret;
	ActionButton select;	
	ActionButton structure;
	ActionButton barrier;
	*/

	static SDL_Surface* bar; //action bar surface
	static SDL_Rect clipBar; //clip for action bar surface
	
	void setClip();		//method that sets the action bar clip
};

#endif
