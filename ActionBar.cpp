#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <string>
#include <sstream>

#include "ActionBar.h"

using namespace std;

SDL_Surface* ActionBar::bar = NULL;
SDL_Surface* ActionBar::button = NULL;

SDL_Rect ActionBar::clipBar;
SDL_Rect ActionBar::clipButton[5][2];

ActionBar::ActionBar(int _height, int _width) 
		: h(_height), w(_width) {
	if(bar==NULL) {
		SDL_Surface* loadedImage = IMG_Load("ActionBar.png");
		SDL_Surface* loadedImage2 = IMG_Load("Buttons.png");
		SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_Surface* optimizedImage2 = SDL_DisplayFormatAlpha(loadedImage2);
		SDL_FreeSurface(loadedImage);
		SDL_FreeSurface(loadedImage2);
		bar = optimizedImage;
		button = optimizedImage2;
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

	for(int i=0; i<5; i++) {
		for(int j=0; j<2; j++) {
			clipButton[i][j].x = 50*j;
			clipButton[i][j].y = 50*i;
			clipButton[i][j].w = clipButton[i][j].h = 50;
		}
	}

}

//draw method takes in number of resources, and whether or not to display the string resource count
void ActionBar::draw(int resources, bool showResources, int _state, int id) {
	SDL_Surface* surface = SDL_GetVideoSurface();
	SDL_Rect offSet;
	offSet.x = (surface->w - w)/2;
	offSet.y = (surface->h - h);

	SDL_Rect buttonOffsets[3];
	for(int i=0; i<3; i++) {
		buttonOffsets[i].x = 25*(i+1)+50*i + offSet.x;
		buttonOffsets[i].y = 6 + offSet.y;
	}

	SDL_BlitSurface(bar,&clipBar,surface,&offSet); 	//blit the bar
	// blit the buttons to the screen, else transparency won't show
	SDL_BlitSurface(button,&clipButton[0][0],surface,&buttonOffsets[0]); //select button
	SDL_BlitSurface(button,&clipButton[id+1][resources >= 80 ? 0:1],surface,&buttonOffsets[1]); //spawn button
	SDL_BlitSurface(button,&clipButton[id+3][resources >= 10 ? 0:1],surface,&buttonOffsets[2]); //turret button
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

	roundedRectangleRGBA(surface,buttonOffsets[_state].x,
		buttonOffsets[_state].y,buttonOffsets[_state].x+50,
		buttonOffsets[_state].y+50,3,150,150,15,255);
}
