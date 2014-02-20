#include <iostream>

#include <SDL/SDL.h>

#include "LocalPlayer.h"
#include "MouseMoveOrder.h"

using namespace std;

bool LocalPlayer::instantiated = false;

Uint8 LocalPlayer::id;
GameSimulation *LocalPlayer::simulation;

Uint32 LocalPlayer::turnid = 0;
int LocalPlayer::turnevents = 0;

SDL_mutex *LocalPlayer::turnsmutex;
vector<PlayerTurn> LocalPlayer::turns;

LocalPlayer::LocalPlayer(Uint8 _id) : Player(_id) {
	if(instantiated) {
		cerr << "error: there can be only one LocalPlayer" << endl;
		return;
	}
	instantiated = true;

	id = _id;

	turnsmutex = SDL_CreateMutex();

	SDL_SetEventFilter(LocalPlayer::filterEvents);
}

void LocalPlayer::setOwnerSimulation(GameSimulation *_simulation) {
	simulation = _simulation;
}

void LocalPlayer::update() {
	// Broadcast this turn only if it's finished
	if(simulation->getTicks()/100 <= turnid) return;

	if(turns.size() < 1) {
		cerr << "warning: empty turns vector" << endl;
		return;
	}

	SDL_mutexP(turnsmutex);

	// Set this for two turns ahead of when it happened, to allow for lag
	turns[0].turnid = turnid + 2;

	// Tell the world about it
	simulation->broadcastPlayerTurn(turns[0]);

	// Next!
	turns.erase(turns.begin());
	turnid++;
	turnevents = 0;

	SDL_mutexV(turnsmutex);
}

// Just for testing
int LocalPlayer::filterEvents(const SDL_Event *event) {
	if(event->type != SDL_MOUSEMOTION) return 1;

	int actualturn = simulation->getTicks()/100;

	SDL_mutexP(turnsmutex);

	Uint32 turnindex = actualturn - turnid;

	// Make sure we've allocated what we need
	if(turns.size() <= turnindex)
		turns.resize(turnindex + 1,PlayerTurn(0,id,vector<Order *>()));

	turns[turnindex].orders.push_back(
		new MouseMoveOrder(simulation->getTicks(),id,turnevents++,
		event->motion.x - event->motion.xrel,
		event->motion.y - event->motion.yrel,
		event->motion.x,event->motion.y));

	SDL_mutexV(turnsmutex);

	return 0;
}

