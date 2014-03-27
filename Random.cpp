// A dead-simple linear congruential pseudorandom number generator

#include <iostream>

#include "Random.h"

using namespace std;

const Uint32 Random::a = 1103515245;
const Uint32 Random::c = 12345;

Random::Random(Uint32 _seed) : seed(_seed), state(_seed) {
	cerr << "creating pseudorandom source with seed " << seed << endl;
}

Uint32 Random::next() {
	// Modulus is fixed at 2^32 by use of 32-bit math
	return state = a*state + c;
}

