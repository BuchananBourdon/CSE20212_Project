#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <string>
#include <sstream>

#include "ActionBar.h"

using namespace std;

SDL_Surface* ActionBar::bar = NULL;
SDL_Rect ActionBar::clipBar;

ActionBar::ActionBar(int _height, int _width) 
		: h(_height), w(_width) {
	if(bar==NULL) {
		SDL_Surface* loadedImage = IMG_Load("ActionBar.png");
		SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
		bar = optimizedImage;
		setClip();
	}
}

int ActionBar::getHeight() { return h; }

int ActionBar::getWidth() { return w; }

//set the clip for the blitting function
void ActionBar::setClip() {
	clipBar.x = 0;
	clipBar.y = 0;
	clipBar.w = 400;
	clipBar.h = 80;
}

//draw method takes in number of resources, and whether or not to display the string resource count
void ActionBar::draw(int resources, bool showResources) {
	SDL_Surface* surface = SDL_GetVideoSurface();
	SDL_Rect offSet;
	offSet.x = (surface->w - w)/2;
	offSet.y = (surface->h - h);
	SDL_BlitSurface(bar,&clipBar,surface,&offSet); 	//blit the bar
	boxRGBA(surface,(640-w)/2+15,(480-18),		//draw the gray underlying resource pane
                (640-w)/2+385,480-2,100,100,100,150);
	boxRGBA(surface,(640-w)/2+20,(480-15),		//draw the red resource bar
                (640-w)/2+20+3.6*resources,480-5,250,0,0,180);	
	if(showResources==true) {			//show resource count
		string str = "RESOURCES: ";
        	stringstream ss2;
        	ss2 << resources;
        	string str2 = ss2.str();
        	string str3 = " / 100";
        	str.append(str2);
        	str.append(str3);
		const char * cstr = str.c_str();	//need a c-style string for stringRGBA function
		stringRGBA(surface,surface->w/2-80,480-10,cstr,255,255,255,255);
	}
}
