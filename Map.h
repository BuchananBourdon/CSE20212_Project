#ifndef MAP_H
#define MAP_H

#include "Random.h"

class Map {
public:
	Map(unsigned int, unsigned int, Random *);

	void draw();

private:
	const unsigned int width;
	const unsigned int height;

	enum tile_type {
		TILE_LAND,
		TILE_WATER
	};

	struct map_tile {
		enum tile_type type;
	} **map;
};

#endif

