  
#include "SDL/SDL.h"
#include <iostream>
#include "SDL/SDL_image.h"
#include <string>


//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Dimensions of the bunny
const int BUNNY_WIDTH = 32;
const int BUNNY_HEIGHT = 32;

//The direction status of the figure
const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_UP = 2;
const int FOO_DOWN = 3;
using namespace std;

//The surfaces
SDL_Surface *bunny = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The rectangles on the sprite sheet
SDL_Rect clipsRight[ 4 ];
SDL_Rect clipsLeft[ 4 ];
SDL_Rect clipsUp[ 4 ];
SDL_Rect clipsDown[ 4 ];




//The stick figure
class Bunny
{
        public:
               	//Initializes the variables
                Bunny();

                //Handles input
                void handle_events();

                //Moves the stick figure
                void move();

                //Shows the stick figure
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

	clipsUp[0].x = 0;
        clipsUp[0].y = BUNNY_HEIGHT * 2;
        clipsUp[0].w = BUNNY_WIDTH;
        clipsUp[0].h = BUNNY_HEIGHT;

        clipsUp[1].x = BUNNY_WIDTH;
        clipsUp[1].y = BUNNY_HEIGHT * 2;
        clipsUp[1].w = BUNNY_WIDTH;
        clipsUp[1].h = BUNNY_HEIGHT;

        clipsUp[2].x = BUNNY_WIDTH * 2;
        clipsUp[2].y = BUNNY_HEIGHT * 2;
        clipsUp[2].w = BUNNY_WIDTH;
        clipsUp[2].h = BUNNY_HEIGHT;

        clipsUp[2].x = BUNNY_WIDTH;
        clipsUp[2].y = BUNNY_HEIGHT * 2;
        clipsUp[2].w = BUNNY_WIDTH;
     	clipsUp[2].h = BUNNY_HEIGHT;


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



Bunny::Bunny()
{
    //Initialize movement variables
    offSet_x = 0;
        offSet_y = SCREEN_HEIGHT - BUNNY_HEIGHT;
    velocity_x = 0;
        velocity_y = 0;

    //Initialize animation variables
    frame = 0;
    status = FOO_RIGHT;
        xgoal = offSet_x;
        ygoal = offSet_y;
}

void Bunny::handle_events()
{

    //If a key was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //Set the velocity
        if( event.button.button == SDL_BUTTON_LEFT )
        {
                int x = 0;
                int y = 0;
                x = event.button.x;
                xgoal = x;
                y = event.button.y;
                ygoal = y;
            if(x > offSet_x)    velocity_x = BUNNY_WIDTH / 4;
            if(x< offSet_x)     velocity_x = -(BUNNY_WIDTH / 4);
                if(y < offSet_y+BUNNY_HEIGHT)   velocity_y = -(BUNNY_WIDTH /4);
                if(y > offSet_y+BUNNY_HEIGHT)   velocity_y = BUNNY_WIDTH /4;
        }
    }
}

void Bunny::move()
{

        if(velocity_x>0 && offSet_x >= xgoal-BUNNY_WIDTH)	velocity_x = 0;
        if(velocity_x<0 && offSet_x <= xgoal)   velocity_x = 0;
        if(velocity_y>0 && offSet_y >= ygoal-BUNNY_HEIGHT)	velocity_y = 0;
        if(velocity_y<0 && offSet_y <= ygoal-BUNNY_HEIGHT)	velocity_y = 0;
    //Move
    offSet_x += velocity_x;
        offSet_y += velocity_y;

}


void Bunny::show()
{

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

    //Show the stick figure
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

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( bunny );

    //Quit SDL
    SDL_Quit();
}






int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Clip the sprite sheet
    set_clips();

    //The stick figure
    Bunny rabbit;

    //While the user hasn't quit
    while( quit == false )
    {

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the stick figure
            rabbit.handle_events();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Move the stick figure
        rabbit.move();

        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        //Show the stick figure on the screen
        rabbit.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
	
    }

    //Clean up
    clean_up();

    return 0;
}
