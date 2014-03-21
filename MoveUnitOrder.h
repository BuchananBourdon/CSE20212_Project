#ifndef MOVE_UNIT_ORDER_H
#define MOVE_UNIT_ORDER_H

#include "Order.h"

class MoveUnitOrder : public Order {
public:
	MoveUnitOrder(int _unitid, int _x, int _y)
		: Order(OT_MOVE_UNIT), unitid(_unitid), x(_x), y(_y) {};

	// From Order
	static Order *deserialize(Uint8 *&);

	void execute(Game &, Uint8);
	void serialize(std::vector<Uint8> &);

private:
	const int unitid; // What
	const int x, y; // Where to?
};

#endif

