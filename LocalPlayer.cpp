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
vector<PlayerTurn *> LocalPlayer::turns;

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

LocalPlayer::~LocalPlayer() {
	if(turnsmutex)
		SDL_DestroyMutex(turnsmutex);
	turnsmutex = NULL;
}

void LocalPlayer::setOwnerSimulation(GameSimulation *_simulation) {
	simulation = _simulation;
}

void LocalPlayer::update() {
	// Sanity check
	if(!simulation) {
		cerr << "error: updating player before owner simulation is set"
			<< endl;
		return;
	}

	// Broadcast this turn only if it's finished
	if(simulation->getTicks()/100 <= turnid) return;

	SDL_mutexP(turnsmutex);

	// We still need to talk, even if there's nothing to talk about
	if(turns.size() == 0) {
		turns.push_back(new PlayerTurn(turnid + turns.size() + 2,id));
	}

	// Tell the world about it
	simulation->broadcastPlayerTurn(turns[0]);

	// Out with the old and in with the new!
	delete turns[0];
	turns.erase(turns.begin());

	turnid++;
	turnevents = 0;

	SDL_mutexV(turnsmutex);
}

// Just for testing
int LocalPlayer::filterEvents(const SDL_Event *event) {
	if(event->type != SDL_MOUSEMOTION) return 1;

	// Sanity check
	if(!simulation) {
		cerr << "error: filtering event before owner simulation is set"
			<< endl;
		return 1;
	}

	int actualturn = simulation->getTicks()/100;

	SDL_mutexP(turnsmutex);

	Uint32 turnindex = actualturn - turnid;

	// Make sure we've allocated what we need
	while(turns.size() <= turnindex)
		turns.push_back(new PlayerTurn(turnid + turns.size() + 2,id));

	turns[turnindex]->orders.push_back(
		new MouseMoveOrder(simulation->getTicks(),id,turnevents++,
		event->motion.x - event->motion.xrel,
		event->motion.y - event->motion.yrel,
		event->motion.x,event->motion.y));

	SDL_mutexV(turnsmutex);

	return 0;
}

