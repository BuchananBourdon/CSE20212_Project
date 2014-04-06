#include <algorithm>

#include "Turn.h"

Turn::~Turn() {
	for(unsigned int i = 0; i < playerturns.size(); i++)
		if(playerturns[i]) delete playerturns[i];
}

void Turn::addPlayerTurn(PlayerTurn *turn) {
	playerturns.push_back(turn);
}

// Execute each PlayerTurn, in order
void Turn::execute(Game &game) {
	sort(playerturns.begin(),playerturns.end());
	for(unsigned int i = 0; i < playerturns.size(); i++)
		playerturns[i]->execute(game);
}

