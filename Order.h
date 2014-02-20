class Order;

#ifndef ORDER_H
#define ORDER_H

#include <vector>

#include <SDL/SDL.h>

#include "GameSimulation.h"

using namespace std;

typedef Order *(*deserializer_func_t)(Uint8 **, Uint32, Uint8, Uint16);

class Order {
public:
	// Default constructor
	Order(long _time, int _player, int _id)
		: time(_time), player(_player), id(_id) {};
	virtual ~Order() {};

	bool operator<(const Order &); // For sorting

	static Order *deserialize(Uint8 **); // Import from byte stream

	virtual void execute(GameSimulation &) = 0; // Update the simulation
	virtual vector<Uint8> serialize() = 0;      // Export to byte stream

protected:
	static Uint8 addDeserializer(deserializer_func_t); // Returns unique id

	Uint32 time;  // When the order was given
	Uint8 player; // Who gave the order
	Uint16 id;    // Unique, player-assigned id

private:
	static vector<deserializer_func_t> deserializers; // Each derived class
};

#endif

