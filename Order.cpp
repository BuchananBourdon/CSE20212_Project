#include <iostream>

#include <SDL/SDL_net.h>

#include "Order.h"

using namespace std;

// Compare this and that--first on time, then on player number
bool Order::operator<(const Order &that) {
	// Sanity check
	if(time == that.time && player == that.player && id == that.id) {
		cerr << "error: found two duplicate Orders (" << time << ", "
			<< player << ", " << id << ")" << endl;
		return false;
	}

	// Actual comparison
	if((time < that.time)
	|| (time == that.time && player < that.player)
	|| (time == that.time && player == that.player && id < that.id))
		return true;
	else return false;
}

// Read the header, then delegate the rest to a derived class
Order *Order::deserialize(Uint8 *data) {
	// Header: [Uint8 type][Uint32 time][Uint8 player][Uint16 id]
	const int headersize = 1 + 4 + 1 + 2;

	Uint8 type = *(Uint8 *) (data + 0);

	// Do we know this type?
	if(type >= deserializers.size()) {
		cerr << "error: unknown Order type (" << type << ")" << endl;
		return NULL;
	}

	Uint32 time = *(Uint32 *) (data + 1);

	Uint8 player = *(Uint8 *) (data + 5);

	Uint16 id = *(Uint16 *) (data + 6);

	return deserializers[type](data + headersize,time,player,id);
}

