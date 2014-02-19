#ifndef GAME_H
#define GAME_H

#include <string>

#include <SDL/SDL_net.h>

#include "GameSimulation.h"
#include "Player.h"

using namespace std;

class Game {
public:
	Game();  // Default constructor
	~Game(); // Destructor

	void host(Uint16); // Host a new game
	void join(IPaddress *); // Join a new game
	void play(); // Main loop

private:
	bool isPacketHead(UDPpacket *, string);

	bool ready;                 // Ready to play?
	Player *players[2];         // Who's playing?
	GameSimulation *simulation; // All of the real game state and logic
};

#endif

