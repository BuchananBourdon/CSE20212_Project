#ifndef TURN_H
#define TURN_H

#include <vector>

#include "Order.h"

struct Turn {
	bool players[2];        // Got orders from each player?
	vector<Order *> orders; // All player's orders
};

#endif

