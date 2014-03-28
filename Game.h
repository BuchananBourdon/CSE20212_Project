class Game;

#ifndef GAME_H
#define GAME_H

#include <list>
#include <queue>

#include <SDL/SDL_net.h>

#include "Map.h"
#include "Message.h"
#include "Order.h"
#include "PlayerTurn.h"
#include "Random.h"
#include "Turn.h"
#include "Unit.h"

class Game {
	friend class Message;

public:
	Game(bool, IPaddress);
	~Game();

	bool      isHosting() { return hosting; }
	UDPsocket getSocket() { return socket; }
	Uint32    getSeed()   { return random->getSeed(); }

	void setPlayerId(Uint8);
	void setSeed(Uint32);

	void play();                      // Game loop
	Uint8 addPlayer();                // New player
	void addPlayerTurn(PlayerTurn *); // Add to queue
	void sendMessage(Message *);      // Add to queue

private:
	static const int ticksperturn; // 1 SDL tick = 1 ms
	static const int turndelay;    // Execution delay

	bool hosting;     // Acting as host?
	UDPsocket socket; // Network connection
	bool playing;     // Stay in the game?

	Uint8 playerid; // Who am I?
	int numplayers; // Players in the game
	Random *random; // Pseudorandom source

	Uint32 start; // Start of the game

	std::queue<Message *> messagequeue; // Send these soon

	PlayerTurn *turn;            // For this turn
	std::list<Turn *> turnqueue; // Play these soon

	Map *map; // World to play in

	std::vector<std::vector<Unit *> > units; // For each player

	void handleEvents();   // Process all events
	void handleMessages(); // Process all messages
	void broadcastTurn();  // Send our current turn
	void sendMessages();   // Empty out messagequeue
	void executeTurns();   // Deterministic simulation
	void draw();           // Update screen

	void handleJoinMessage(Uint8 *, IPaddress *); // Request to join game
};

#endif
