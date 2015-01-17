class Map;

#ifndef MAP_H
#define MAP_H

#include "Random.h"
#include "Unit.h"
#include "View.h"

class Map {
public:
	Map(unsigned int, Random *);
	~Map();

	enum tile_type {
		TILE_NONE,
		TILE_WATER,
		TILE_LAND,
		TILE_MOUNTAIN
	};

	enum resource {
		RES_NONE,
		RES_CANDY,
		RES_COAL
	};

	unsigned int getWidth()  { return width;  }
	unsigned int getHeight() { return height; }

	void draw(const View &);

	void occupy(Uint16, Uint16, int);
	bool isOccupied(Uint16, Uint16);
	bool isOccupied(Uint16, Uint16, Uint16, Uint16);
	int getOccupier(Uint16, Uint16);
	void clear(Uint16, Uint16);

	void defog(Uint16, Uint16, Uint16);
	bool isFoggy(Uint16, Uint16);
	bool isFoggy(Uint16, Uint16, Uint16, Uint16);

	enum tile_type tileType(Uint16, Uint16);
	enum resource resourceType(Uint16,Uint16);

	void elim_resource(Uint16, Uint16); 	//remove resource from map
	int getResourceAmount(Uint16,Uint16);	//get the resourceAmount at Map tile
	void decrementResourceAmount(Uint16,Uint16);	//decrement the resourceAmount at tile

private:
	static const Uint8 roughness;

	static const Uint8 waterlevel;
	static const Uint8 mountainlevel;

	static const int numrivers;
	static const int riverlength;

	static const int numfields;
	static const int fieldsize;

	//For tiles
	static SDL_Surface * mountainSurface;
	static SDL_Surface * grassSurface;
	static SDL_Surface * waterSurface;	
	static SDL_Rect mountainClips[17][5];
	static SDL_Rect grassClips[17];
	static SDL_Rect waterClips[17];

	//For resources
	static SDL_Surface * candySurface;
	static SDL_Surface * coalSurface;
	static SDL_Rect candyClips[17];
	static SDL_Rect coalClips[17];

	const unsigned int width;
	const unsigned int height;

	struct map_tile {
		map_tile() : height(0), type(TILE_NONE), resource(RES_NONE),
			resourceAmount(0), fog(true), unitid(-1) {}

		Uint8 height;

		enum tile_type type;

		enum resource resource;
		int resourceAmount;

		bool fog;

		int unitid;
	} **map;

	void perturb(int, Random *); // For generation
	Uint8 square_avg(int, int, int, int, Random *);
	Uint8 diamond_avg(int, int, int, int, Random *);

	void trace_river(Random *); // Add river

	void grow_resource(enum resource, Random *); // Add resource field
	int density(int, int, enum resource);        // Resourceful neighbors?

	void setClips();
};

#endif

