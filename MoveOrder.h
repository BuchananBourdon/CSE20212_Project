#ifndef MOVE_ORDER_H
#define MOVE_ORDER_H

#include "Order.h"

class MoveOrder : public Order {
public:
	MoveOrder(int, int, int); // (unitid, x, y)

	// From Order
	void execute();
};

#endif

