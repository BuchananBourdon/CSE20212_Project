class Order;

#ifndef ORDER_H
#define ORDER_H

#include "GameSimulation.h"

class Order {
public:
	// Default constructor
	Order(long _time, int _player, int _id)
		: time(_time), player(_player), id(_id) {};

	bool operator<(const Order &); // For sorting

	virtual void execute(GameSimulation &) = 0; // Update the simulation

protected:
	long time;  // When the order was given
	int player; // Who gave the order
	int id;     // Unique, player-assigned id
};

#endif

