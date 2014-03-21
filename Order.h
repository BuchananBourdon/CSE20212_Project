class Order;

#ifndef ORDER_H
#define ORDER_H

#include <vector>

#include <SDL/SDL_net.h>

#include "Game.h"

class Order {
public:
	Order(int _type) : type(_type) {};
	virtual ~Order() {};

	static Order *deserialize(Uint8 *&);

	virtual void execute(Game &, Uint8) = 0;
	virtual void serialize(std::vector<Uint8> &) = 0;

protected:
	enum order_type {
		OT_MOVE_UNIT
	};

	const int type;
};

#endif

