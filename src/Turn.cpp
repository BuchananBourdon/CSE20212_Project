#include <algorithm>
#include <iostream>

#include "Turn.h"

using namespace std;

Turn::~Turn() {
	for(unsigned int i = 0; i < playerturns.size(); i++)
		if(playerturns[i]) delete playerturns[i];
}

void Turn::addPlayerTurn(PlayerTurn *turn) {
	playerturns.push_back(turn);
}

// Execute each PlayerTurn, in order
void Turn::execute(Game &game) {
//	cerr << "executing turn " << turnid << endl;

	sort(playerturns.begin(),playerturns.end());
	for(unsigned int i = 0; i < playerturns.size(); i++)
		playerturns[i]->execute(game);
}

