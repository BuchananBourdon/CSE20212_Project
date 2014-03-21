// Order serialization format: [type:1][data...]

#include <iostream>

#include "MoveUnitOrder.h"
#include "Order.h"

using namespace std;

Order *Order::deserialize(Uint8 *&data) {
	enum order_type type = (enum order_type) *data++;
	switch(type) {
	case OT_MOVE_UNIT: return MoveUnitOrder::deserialize(data);

	default:
		cerr << "error: received order of unknown type" << endl;
		return NULL;
	}
}

