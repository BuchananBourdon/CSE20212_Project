#ifndef RANDOM_H
#define RANDOM_H

#include <SDL/SDL.h>

class Random {
public:
	Random(Uint32);

	Uint32 getSeed() { return seed; }

	Uint32 next();

private:
	static const Uint32 a, c, m;

	const Uint32 seed;

	Uint32 state;
};

#endif

