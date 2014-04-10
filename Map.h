#ifndef MAP_H
#define MAP_H

#include "Random.h"
#include "View.h"

class Map {
public:
	Map(unsigned int, Random *);
	~Map();

	unsigned int getWidth()  { return width;  }
	unsigned int getHeight() { return height; }

	void draw(const View &);

private:
	static const Uint8 roughness;

	static const Uint8 waterlevel;
	static const Uint8 mountainlevel;

	static const int numrivers;
	static const int riverlength;

	//For tiles
	static SDL_Surface * mountainSurface;
	static SDL_Surface * grassSurface;
	static SDL_Surface * waterSurface;	
	static SDL_Rect mountainClips[17][5];
	static SDL_Rect grassClips[17];
	static SDL_Rect waterClips[17];

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
	
	void setClips();
};

#endif

