#include "PlayerTurn.h"

PlayerTurn::~PlayerTurn() {
	for(unsigned int i = 0; i < orders.size(); i++)
		if(orders[i]) delete orders[i];
}

void PlayerTurn::addOrder(Order *order) {
	orders.push_back(order);
}

void PlayerTurn::execute(Game &game) {
	for(unsigned int i = 0; i < orders.size(); i++)
		orders[i]->execute(game,playerid);
}

