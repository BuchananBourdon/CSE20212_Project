// A Galois linear feedback shift register, courtesy of Wikipedia

#include <iostream>

#include "Random.h"

using namespace std;

Random::Random(Uint32 _seed) : seed(_seed), state(_seed) {
	cerr << "creating pseudorandom source with seed " << seed << endl;
}

Uint32 Random::next() {
	return state = (state >> 1) ^ (-(state & 1) & 0xD0000001);
}

