#ifndef MOUSEMOVEORDER_H
#define MOUSEMOVEORDER_H

#include "Order.h"

class MouseMoveOrder : public Order {
public:
	MouseMoveOrder(Uint32, Uint8, Uint16, Uint16, Uint16, Uint16, Uint16);
	virtual ~MouseMoveOrder() {};

	// For Order::addDeserializer()
	static Order *deserialize(Uint8 **, Uint32, Uint8, Uint16);

	// From Order
	virtual void execute(GameSimulation &);
	virtual vector<Uint8> serialize();

private:
	static int deserializerid;

	Uint16 startx, starty;
	Uint16 endx, endy;
};

#endif

