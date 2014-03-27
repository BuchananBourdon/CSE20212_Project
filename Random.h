#ifndef RANDOM_H
#define RANDOM_H

#include <SDL/SDL.h>

class Random {
public:
	Random(Uint32);

	Uint32 getSeed() { return seed; }

	Uint32 next();

private:
	static const Uint32 a;
	static const Uint32 c;

	Uint32 seed;
	Uint32 state;
};

#endif

