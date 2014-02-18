#include <iostream>

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

