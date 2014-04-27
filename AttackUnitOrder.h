#ifndef ATTACK_UNIT_ORDER_H
#define ATTACK_UNIT_ORDER_H

#include "Order.h"

class AttackUnitOrder : public Order {
public:
	AttackUnitOrder(int _unitid, int _targetid)
		: Order(OT_ATTACK_UNIT), unitid(_unitid), targetid(_targetid)
			{};

	// From Order
	static Order *deserialize(Uint8 *&);

	void execute(Game &, Uint8);
	void serialize(std::vector<Uint8> &);

private:
	const int unitid;   // Attacker
	const int targetid; // Attackee
};

#endif

