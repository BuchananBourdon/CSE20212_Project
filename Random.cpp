// A Galois linear feedback shift register (courtesy of Wikipedia),
// mashed together with a linear congruential generator

#include <iostream>

#include "Random.h"

using namespace std;

const Uint32 Random::a = 1103515245;
const Uint32 Random::c = 12345;
const Uint32 Random::m = 0x80000000;

Random::Random(Uint32 _seed) : seed(_seed), state(_seed) {
	cerr << "creating pseudorandom source with seed " << seed << endl;
}

Uint32 Random::next() {
	state = (state >> 1) ^ (-(state & 1) & 0xD0000001);
	return state = (a*state + c)%m;
}

