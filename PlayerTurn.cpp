#include "PlayerTurn.h"

void PlayerTurn::addOrder(Order *order) {
	orders.push_back(order);
}

void PlayerTurn::execute() {
	for(unsigned int i = 0; i < orders.size(); i++)
		orders[i]->execute();
}

