class CreateUnitOrder;

#ifndef CREATE_UNIT_ORDER_H
#define CREATE_UNIT_ORDER_H

#include "Order.h"

class CreateUnitOrder : public Order {
public:
	CreateUnitOrder(Uint8 _unittype, Uint16 _x, Uint16 _y)
		: Order(OT_CREATE_UNIT), unittype(_unittype), x(_x), y(_y) {}

	static Order *deserialize(Uint8 *&);

	void execute(Game &, Uint8);
	void serialize(std::vector<Uint8> &);

private:
	const Uint8 unittype; // What?
	const Uint16 x, y;    // Where?
};

#endif

