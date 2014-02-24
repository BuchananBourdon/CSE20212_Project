#include <algorithm>
#include <iostream>
#include <vector>

#include "GameSimulation.h"
#include "LocalPlayer.h"
#include "NetworkPlayer.h"
#include "Order.h"
#include "PlayerTurn.h"
#include "Turn.h"

using namespace std;

// Default constructor: one local player and one network player
GameSimulation::GameSimulation(Player *(&_players)[2])
	: players(_players), turnid(2) {
	players[0]->setOwnerSimulation(this);
	players[1]->setOwnerSimulation(this);

	simstart = SDL_GetTicks();

	image = SDL_CreateRGBSurface(SDL_HWSURFACE,640,480,32,0,0,0,0);
}

// Destructor: get rid of the image
GameSimulation::~GameSimulation() {
	while(turns.size()) {
		if(turns.front())
			delete turns.front();
		turns.pop_front();
	}

	if(image) SDL_FreeSurface(image);
}

// Disseminate a player's turn and store it for later simulation
void GameSimulation::broadcastPlayerTurn(PlayerTurn *playerturn) {
	// Sanity checks
	if(playerturn->turnid < turnid) {
		cerr << "error: old turn broadcast from player "
			<< playerturn->playerid << endl;
		return;
	}

	if(playerturn->playerid < 0 || playerturn->playerid > 1) {
		cerr << "error: turn broadcast from unknown player "
			<< playerturn->playerid << endl;
		return;
	}

	// Tell all of the players (useed for network play)
	players[0]->receivePlayerTurn(playerturn);
	players[1]->receivePlayerTurn(playerturn);

	// Store it for later simulation
	unsigned int turnindex = playerturn->turnid - turnid;
	if(turnindex < turns.size()) { // Add to an existing turn
		list<Turn *>::iterator turniter = turns.begin();
		advance(turniter,turnindex);

		(*turniter)->players[playerturn->playerid] = true;
		(*turniter)->orders.insert((*turniter)->orders.end(),
			playerturn->orders.begin(),playerturn->orders.end());
	} else { // Add a new turn
		// Add in any skipped turns
		while(turnindex > turns.size()) {
			cerr << "warning: possible dropped packet for turn "
				<< (turnid + turns.size()) << endl;
			turns.push_back(new Turn());
		}

		// Add this turn
		Turn *turn = new Turn();

		turn->players[playerturn->playerid] = true;
		turn->orders.insert(turn->orders.end(),
			playerturn->orders.begin(),playerturn->orders.end());

		turns.push_back(turn);
	}
}

// Simulate one tick of the simulation, if it is time and if the orders are in
void GameSimulation::update() {
	// Have the players do their updating
	players[0]->update();
	players[1]->update();

	// Have we already done this turn?
	if(SDL_GetTicks() - simstart < turnid*100) return;

	Turn *turn = turns.front();

	// Can we actually do this turn?
	if(!turn->players[0] || !turn->players[1])
		return; // We don't have all the players' orders yet

	// Put all the orders in a well-defined order (he, he, :-)
	sort(turn->orders.begin(),turn->orders.end());

	// Follow the players' orders
	for(unsigned int i = 0; i < turn->orders.size(); i++)
		if(turn->orders[i])
			turn->orders[i]->execute(*this);

	// TODO: simulate

	turns.pop_front();
	delete turn;

	// Next turn!
	turnid++;
}

// Return the number of milliseconds that this simulation has been active
Uint32 GameSimulation::getTicks() {
	return SDL_GetTicks() - simstart;
}

// Return the testing image
SDL_Surface *GameSimulation::getImage() {
	return image;
}

