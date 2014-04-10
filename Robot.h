#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"

class Robot : public Unit {
public:
  Robot(int, int);

  int getType();

private:
  // Has static Surface and Rect for blitting, will modify clip when MoveUnitOrder is finished
  static SDL_Surface * robotSurface;
  static SDL_Rect clipsRight[ 4 ];
  static SDL_Rect clipsLeft[ 4 ];
  static SDL_Rect clipsUp[ 4 ];
  static SDL_Rect clipsDown[ 4 ];

  // Pertain to image pixel dimensions
  static int ROBOT_WIDTH;
  static int ROBOT_HEIGHT;

  int status;
  int frame;

  void drawUnit(View &);
  void setClips();
};

#endif
