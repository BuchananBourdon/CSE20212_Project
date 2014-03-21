//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frames per second
const int FRAMES_PER_SECOND = 50;

//The dimenstions of the robot
const int ROBOT_WIDTH = 68;
const int ROBOT_HEIGHT = 68;

//The direction status of the robot
const int ROBOT_RIGHT = 0;
const int ROBOT_LEFT = 1;

//The surfaces
SDL_Surface *robot = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The areas of the sprite sheet
SDL_Rect clipsRight[ 4 ];
SDL_Rect clipsLeft[ 4 ];

//The robot
class Robot
{
    private:
    //The offset
    int offSet;

    //Its rate of movement
    int velocity;

    //Its current frame
    int frame;

    //Its animation status
    int status;

    public:
    //Initializes the variables
    Robot();

    //Handles input
    void handle_events();

    //Moves the robot
    void move();

    //Shows the robot
    void show();
};

//The timer
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
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
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

void set_clips()
{
    //Clip the sprites
    clipsRight[ 0 ].x = 0;
    clipsRight[ 0 ].y = 0;
    clipsRight[ 0 ].w = ROBOT_WIDTH;
    clipsRight[ 0 ].h = ROBOT_HEIGHT;

    clipsRight[ 1 ].x = ROBOT_WIDTH;
    clipsRight[ 1 ].y = 0;
    clipsRight[ 1 ].w = ROBOT_WIDTH;
    clipsRight[ 1 ].h = ROBOT_HEIGHT;

    clipsRight[ 2 ].x = ROBOT_WIDTH * 2;
    clipsRight[ 2 ].y = 0;
    clipsRight[ 2 ].w = ROBOT_WIDTH;
    clipsRight[ 2 ].h = ROBOT_HEIGHT;

    clipsRight[ 3 ].x = ROBOT_WIDTH * 3;
    clipsRight[ 3 ].y = 0;
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
    robot = load_image( "robot.png" );

    //If there was a problem in loading the sprite
    if( robot == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( robot );

    //Quit SDL
    SDL_Quit();
}

Robot::Robot()
{
    //Initialize movement variables
    offSet = 0;
    velocity = 0;

    //Initialize animation variables
    frame = 0;
    status = ROBOT_RIGHT;
}

void Robot::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: velocity += ROBOT_WIDTH / 4; break;
            case SDLK_LEFT: velocity -= ROBOT_WIDTH / 4; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: velocity -= ROBOT_WIDTH / 4; break;
            case SDLK_LEFT: velocity += ROBOT_WIDTH / 4; break;
        }
    }
}

void Robot::move()
{
    //Move
    offSet += velocity;

    //Keep the stick figure in bounds
    if( ( offSet < 0 ) || ( offSet + ROBOT_WIDTH > SCREEN_WIDTH ) )
    {
        offSet -= velocity;
    }
}

void Robot::show()
{
    //If Robot is moving left
    if( velocity < 0 )
    {
        //Set the animation to left
        status = ROBOT_LEFT;

        //Move to the next frame in the animation
        frame++;
    }
    //If Robot is moving right
    else if( velocity > 0 )
    {
        //Set the animation to right
        status = ROBOT_RIGHT;

        //Move to the next frame in the animation
        frame++;
    }
    //If Robot standing
    else
    {
        //Restart the animation
        frame = 0;
    }

    //Loop the animation
    if( frame >= 4 )
    {
        frame = 0;
    }

    //Show the robot
    if( status == ROBOT_RIGHT )
    {
        apply_surface( offSet, SCREEN_HEIGHT - ROBOT_HEIGHT, robot, screen, &clipsRight[ frame ] );
    }
    else if( status == ROBOT_LEFT )
    {
        apply_surface( offSet, SCREEN_HEIGHT - ROBOT_HEIGHT, robot, screen, &clipsLeft[ frame ] );
    }
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

    //The frame rate regulator
    Timer fps;

    //The robot
    Robot walk;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the stick figure
            walk.handle_events();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Move the robot
        walk.move();

        //Fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        //Show the robot on the screen
        walk.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}
