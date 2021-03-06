class Game;

#ifndef GAME_H
#define GAME_H

#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>

#include <SDL/SDL_net.h>

#include "ActionBar.h"
#include "Map.h"
#include "Message.h"
#include "MessageQueue.h"
#include "Order.h"
#include "PlayerTurn.h"
#include "Random.h"
#include "Turn.h"
#include "Unit.h"
#include "View.h"

class Game {
	friend class Message;

public:
	Game(bool, IPaddress);
	~Game();

	bool   isHosting()   { return hosting; }

	Uint8  getPlayerId() { return playerid; }
	Uint32 getSeed()     { return random->getSeed(); }

	PlayerTurn *getTurn() { return turn.get(); }

	Map *getMap() { return map.get(); }

	UDPsocket getSocket();

	void setPlayerId(Uint8);
	void setWon(bool);
	void setSeed(Uint32);

	void play(); // Game loop
	void exit(); // Stop loop

	Uint8 addPlayer();                // New player
	void addPlayerTurn(PlayerTurn *); // Add to queue
	void sendMessage(Message *);      // Add to queue

	void addUnit(Uint8, Unit *);
	void moveUnit(Uint8, Uint16, Uint16, Uint16, Uint16);
	void attackUnit(Uint8, Uint16, Uint16);

	bool useResources(int); // If we can

	void groupMove(Uint16);  // Set
	bool groupMoved(Uint16); // Query

private:
	static const int ticksperturn; // 1 SDL tick = 1 ms
	static const int turndelay;    // Execution delay

	static const int mapsize; // Width and height

	bool hosting; // Acting as host?
	bool playing; // Still in the game?
	bool won;     // Yeah?

	Uint8 playerid; // Who am I?
	int numplayers; // Players in the game

	std::auto_ptr<Random> random; // Pseudorandom source

	int xdown;	// Stores x location on screen on left click
	int ydown; 	// Stores y location on screen on left click
	int xup;   	// Stores x location on screen on left click release
	int yup;   	// Stores y location on screen on left click release
	int moved;  	// Toggles depending on whether the cursor is moving
	int mousex; 	// Stores x location of cursor on screen
	int mousey;	// Stores y location of cursor on screen

	Uint32 start;     // Start of the game
	Uint32 lastframe; // For FPS-limiting

	SDL_Surface *endframe;

	std::auto_ptr<MessageQueue> messagequeue; // Communication manager

	std::auto_ptr<PlayerTurn> turn; // For this turn
	std::list<Turn *> turnqueue;    // Play these soon

	std::auto_ptr<Map> map;   // World to play in
	std::auto_ptr<View> view; // What we can see

	ActionBar bar;

	int resources;
	bool showResources;	//for showing resource-count on mouse hover-over

	std::vector<std::vector<Unit *> > units; // For each player
	std::set<int> selected;                  // Currently active

	std::map<Uint16,std::pair<bool,bool> > movegroups; // Units moving

	int viewVelocity_x;	// x velocity for how fast the view is panning
	int viewVelocity_y;	// y velocity for how fast the view is panning

	 enum action_state {
		AS_SELECT,
		AS_SPAWN,
		AS_TURRET,
		AS_BARRIER,
		AS_STRUCTURE
	};

	enum action_state state;

	void handleEvents();     // Process all events
	void broadcastTurn();    // Send our current turn
	void updateSimulation(); // Deterministic simulation
	void updateView();       // Move view
	void draw();             // Update screen

	void checkEndState(); // All units dead?
	void drawEndFrame();  // Win or lose

	// Helper functions for handleEvents()
	void handleKeyDown(SDL_Event &);
	void handleKeyUp(SDL_Event &);
	void handleMouseDown(SDL_Event &);
	void handleMouseUp(SDL_Event &);
	void handleMouseMove(SDL_Event &);
	void handleQuit(SDL_Event &);

	void defaultAction(unsigned int, unsigned int); // Contextual
	void selectUnits(bool, int, int, int, int);     // Within bounds
	void attackUnit(Unit *);                        // Selected attack
	void moveUnits(Uint16, Uint16);                 // Move selected

	void gatherResources(Unit *, Map &);
};

#endif

