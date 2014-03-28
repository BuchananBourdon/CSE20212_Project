#ifndef MAP_H
#define MAP_H

#include "Random.h"

class Map {
public:
	Map(unsigned int, Random *);

	void draw();

private:
	static const Uint8 roughness;

	static const Uint8 waterlevel;
	static const Uint8 mountainlevel;

	static const int numrivers;
	static const int riverlength;

	const unsigned int width;
	const unsigned int height;

	enum tile_type {
		TILE_WATER,
		TILE_LAND,
		TILE_MOUNTAIN
	};

	struct map_tile {
		Uint8 height;

		enum tile_type type;

		bool resource;
	} **map;

	void perturb(int, Random *); // For generation
	Uint8 square_avg(int, int, int, int, Random *);
	Uint8 diamond_avg(int, int, int, int, Random *);

	void trace_river(Random *); // Add river
};

#endif

