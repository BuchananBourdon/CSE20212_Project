#ifndef MOVE_UNIT_ORDER_H
#define MOVE_UNIT_ORDER_H

#include "Order.h"

class MoveUnitOrder : public Order {
public:
	MoveUnitOrder(Uint16 _unitid, Uint16 _group, Uint16 _x, Uint16 _y)
		: Order(OT_MOVE_UNIT), unitid(_unitid), group(_group), x(_x),
			y(_y) {};

	// From Order
	static Order *deserialize(Uint8 *&);

	void execute(Game &, Uint8);
	void serialize(std::vector<Uint8> &);

private:
	const Uint16 unitid; // What?
	const Uint16 group;  // With whom?
	const Uint16 x, y;   // Where to?
};

#endif

