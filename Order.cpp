#include <iostream>

#include <SDL/SDL_net.h>

#include "Order.h"

using namespace std;

vector<deserializer_func_t> Order::deserializers;

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
Order *Order::deserialize(Uint8 **data) {
	// Header: [Uint8 type][Uint32 time][Uint8 player][Uint16 id]

	Uint8 type = **data;

	// Do we know this type?
	if(type >= deserializers.size()) {
		cerr << "error: unknown Order type (" << type << ")" << endl;
		return NULL;
	}

	*data += 1;

	Uint32 time = SDLNet_Read32(*data);
	*data += 4;

	Uint8 player = **data;
	*data += 1;

	Uint16 id = SDLNet_Read16(data);
	*data += 2;

	return deserializers[type](data,time,player,id);
}

// Register a deserializer and return its id
Uint8 Order::addDeserializer(deserializer_func_t deserializer) {
	deserializers.push_back(deserializer);
	return deserializers.size() - 1;
}

