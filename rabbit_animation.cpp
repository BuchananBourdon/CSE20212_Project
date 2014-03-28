  
#include "SDL/SDL.h"
#include <iostream>
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <string>
#include <vector>

using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Dimensions of the bunny
const int BUNNY_WIDTH = 32;
const int BUNNY_HEIGHT = 32;

int xdown = 0;
int ydown = 0;
int xup = 0;
int yup = 0;
int moved = 0;
int mousex = 0;
int mousey = 0;

const int FRAMES_PER_SECOND = 10;


//The direction status of the figure
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_UP = 2;
const int FOO_DOWN = 3;
using namespace std;

//The surfaces
SDL_Surface *bunny = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *selected_circle = NULL;
SDL_Surface *health = NULL;
//The event structure
SDL_Event event;

//The rectangles on the sprite sheet
SDL_Rect clipsRight[ 4 ];
SDL_Rect clipsLeft[ 4 ];
SDL_Rect clipsUp[ 4 ];
SDL_Rect clipsDown[ 4 ];
SDL_Rect circle;
SDL_Rect health_full;

//The Bunny
class Bunny
{
	friend void trackBunny(Bunny *);
	friend int compareBunnyOffsets(int);
        public:
               	//Initializes the variables
                Bunny(int = 0,int = SCREEN_HEIGHT - BUNNY_HEIGHT);

                //Handles input
                void handle_events();

                //Moves the bunny
                void move();

                //Shows the bunny
                void show();

        private:
                //The offset
                int offSet_x;
                int offSet_y;

                //Its rate of movement
                int velocity_x;
                int velocity_y;

                //Its current frame
                int frame;

                //Its animation status
                int status;

                int xgoal;
                int ygoal;
		int selected;
		int showhealth;
};


class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};



void set_clips()
{
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

	circle.x = 0;
	circle.y = 0;
	circle.w = 80;
	circle.h = 80;

	health_full.x = 0;
	health_full.y = 0;
	health_full.w = 45;
	health_full.h = 20;
}



SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
     	//Create an optimized surface
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}



Bunny::Bunny(int x, int y)
{
    //Initialize movement variables
    offSet_x = x;
        //offSet_y = SCREEN_HEIGHT - BUNNY_HEIGHT;
    	offSet_y = y;
	velocity_x = 0;
        velocity_y = 0;
	
    //Initialize animation variables
    frame = 0;
    status = FOO_RIGHT;
        xgoal = offSet_x;
        ygoal = offSet_y;
	trackBunny(this);
	selected = 0;
	showhealth = 0;
}

void Bunny::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
	if(event.button.button == SDL_BUTTON_RIGHT)	selected = 0;
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    {
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		xup = event.button.x;
		yup = event.button.y;
	
		int xleft,xright,ytop,ybottom;
		if(xup < xdown)
		{	
			xleft = xup;
			xright = xdown;
		}
		else if(xup > xdown)
		{
			xleft = xdown;
			xright = xup;
		}
		if(yup < ydown)	
		{
			ytop = yup;
			ybottom = ydown;
		}
		else if(yup > ydown)
		{
			ytop = ydown;
			ybottom = yup;
		}


		if(xdown == xup && ydown == yup)
		{
			if(selected==1)
			{
				xgoal = xup;
				ygoal = yup;
				if(offSet_x < xgoal)    velocity_x = BUNNY_WIDTH / 2;
                		if(xgoal < offSet_x)     velocity_x = -(BUNNY_WIDTH / 2);
                		if(ygoal < offSet_y+BUNNY_HEIGHT)   velocity_y = -(BUNNY_WIDTH /2);
                		if(ygoal > offSet_y+BUNNY_HEIGHT)   velocity_y = BUNNY_WIDTH /2;
			}
			selected = 0;
		}
		else
		{
			if(offSet_x+BUNNY_WIDTH/2 < xright && offSet_x+BUNNY_WIDTH/2 > xleft)
			{
				if(offSet_y+BUNNY_HEIGHT/2 > ytop && offSet_y+BUNNY_HEIGHT/2 < ybottom)
				{
					selected = 1;
				}
			}
		}
	}
    }
	if(event.type == SDL_KEYDOWN)
	{
		if(event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT)	
		{
			showhealth = 1;
		}
	}
	if(event.type == SDL_KEYUP)
	{
		if(event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT)
		{
			showhealth = 0;
		}
	}

}

void Bunny::move()
{
        if(velocity_x>0 && offSet_x >= xgoal-BUNNY_WIDTH)	
	{
		velocity_x = 0;
	}
        if(velocity_x<0 && offSet_x <= xgoal)
	{
	   	velocity_x = 0;
	}
        if(velocity_y>0 && offSet_y >= ygoal-BUNNY_HEIGHT)	
	{
		velocity_y = 0;
	}
        if(velocity_y<0 && offSet_y <= ygoal)
	{	
		velocity_y = 0;
	}
    

    		offSet_x += velocity_x;
        	offSet_y += velocity_y;

	if(velocity_x > 0 && velocity_y > 0)
        {
                status = FOO_DOWN;
                frame++;
        }
	else if( velocity_x >0 && velocity_y <0)
        {
                status = FOO_UP;
                frame++;
        }
        else if( velocity_x < 0 && velocity_y >0)
        {
                status = FOO_DOWN;
                frame++;
        }
        else if(velocity_x < 0 && velocity_y <0)
        {
                status = FOO_UP;
                frame++;
        }
        else if(velocity_x == 0 && velocity_y > 0)
        {
                status = FOO_DOWN;
                frame++;
        }
        else if(velocity_x == 0 && velocity_y < 0)
        {
               	status = FOO_UP;
                frame++;
        }
	else if(velocity_x >0 && velocity_y == 0)
        {
                status = FOO_RIGHT;
                frame++;
        }
	else if(velocity_x < 0 && velocity_y == 0)
        {
                status = FOO_LEFT;
                frame++;
        }
	else
	{
                frame = 0;
        }

    //Loop the animation
    if( frame >= 4 )
    {
     	frame = 0;
    }

	if(abs(offSet_x - xgoal) < 40 && abs(offSet_y - ygoal) < 40)	frame = 0;

}


void Bunny::show()
{
    //Show the bunny
    if( status == FOO_RIGHT )
    {
        apply_surface( offSet_x, offSet_y, bunny, screen, &clipsRight[ frame ] );
    }
    else if( status == FOO_LEFT )
    {
        apply_surface( offSet_x, offSet_y, bunny, screen, &clipsLeft[ frame ] );
    }
    else if( status == FOO_DOWN )
    {
        apply_surface(offSet_x,offSet_y,bunny,screen,&clipsDown[ frame ] );
    }
    else if(status == FOO_UP )
    {
        apply_surface(offSet_x,offSet_y,bunny,screen,&clipsUp[ frame ] );
    }

	if(selected==1)
	{
		apply_surface(offSet_x+BUNNY_WIDTH/2-40,offSet_y+BUNNY_HEIGHT/2-40,selected_circle,screen,&circle);
		
		if(showhealth==1)
		{
			apply_surface(offSet_x - 7,offSet_y+BUNNY_HEIGHT-3,health,screen,&health_full);
		}
		
	}
}


vector<Bunny*> bunnyVec;

void trackBunny(Bunny * rabbit)
{
        bunnyVec.push_back(rabbit);
}


int compareBunnyOffsets(int number)
{
	int i = bunnyVec.size();
	for(int j = 0;j<1;j++)
	{
		if(abs((*bunnyVec[j]).offSet_x - (*bunnyVec[number]).offSet_x) < 40 && abs((*bunnyVec[j]).offSet_x - (*bunnyVec[j]).xgoal) < 60)
		{
			if(abs((*bunnyVec[j]).offSet_y - (*bunnyVec[number]).offSet_y) < 40 && abs((*bunnyVec[j]).offSet_y - (*bunnyVec[j]).ygoal) < 60)               
                	{
				return 0;
			}
		}
	}

	return 1;
}


Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
     	//Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}


void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
     	//Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
     	//If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
	else
	{
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}


bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
     	return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
     	return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Animation Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the sprite sheet
    bunny = load_image( "rabbit_animations.png" );

    //If there was a problem in loading the sprite
    if( bunny == NULL )
    {
        return false;
    }

	selected_circle = load_image("selected_circle.png");
	if(selected_circle == NULL)
	{
		return false;
	}
	
	health = load_image("health_full.png");
	if(health == NULL)
	{
		return false;
	}
	
    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( bunny );

	SDL_FreeSurface(selected_circle);
	SDL_FreeSurface(health);

    //Quit SDL
    SDL_Quit();
}


void setSelection()	//function that sets the global mouse input variables
{
	if( event.type == SDL_MOUSEBUTTONDOWN )
    	{
        	if(event.button.button == SDL_BUTTON_LEFT )
        	{
                	xdown = event.button.x;		//xdown and ydown are set if user left clicks
                	ydown = event.button.y;
			moved = 1;
        	}
	}

	if(event.type == SDL_MOUSEMOTION)
	{
		mousex = event.motion.x;
		mousey = event.motion.y;
	}

	if(event.type == SDL_MOUSEBUTTONUP)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			moved = 0;
		}
	}
}

void highlightSelection()	//will draw a transparent highlight box
{
	if(moved==1)
        {
                boxRGBA(screen,xdown,ydown,mousex,mousey,60,60,255,170);
        }
}




int main( int argc, char* args[] )
{
    	bool quit = false; //quit flag

    	if( init() == false ) //Initialize
    	{
        	return 1;
    	}

	Timer fps;	//instantiate the timer
    	
    	if( load_files() == false ) 	//Load the files
    	{
        	return 1;
    	}

 	set_clips();     	//Clip the sprite sheet


    	//Instantiate the minions
    	Bunny rabbit(200,400);
	Bunny rabbit2(300,100);

    	while( quit == false )	//main while loop, runs until user X's out of screen
    	{
		fps.start();	//begin the timer
        	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) ); //clear the screen white

        	//While there's events to handle
       		while( SDL_PollEvent( &event ) )
        	{
            		rabbit.handle_events();
			rabbit2.handle_events();
			setSelection();			//set user mouse input
            		if( event.type == SDL_QUIT )        //If the user has Xed out the window

            		{
                		quit = true;
            		}
        	}
		
		highlightSelection();	//draw the highlight square, will only work under appropriate conditions

        	rabbit.move();
		rabbit2.move();
	

        	//Show the stick figures on the screen
        	rabbit.show();
		rabbit2.show();

        	if( SDL_Flip( screen ) == -1 )         	//Update the screen

        	{
            		return 1;
        	}



		//Cap the frame rate
        	if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        	{
            		SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        	}

	
    }

    clean_up();
    return 0;
}
