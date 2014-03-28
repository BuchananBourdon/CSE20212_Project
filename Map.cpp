#include "Map.h"

Map::Map(unsigned int _width, unsigned int _height, Random *random)
	: width(_width), height(_height) {

	// Allocate the row pointers
	map = new struct map_tile *[height];

	for(unsigned int y = 0; y < height; y++) {
		// Allocate this row
		map[y] = new struct map_tile[width];

		for(unsigned int x = 0; x <  width; x++) {
			// Assign the tile type randomly for now
			map[y][x].type
				= random->next()&1 ? TILE_LAND : TILE_WATER;
		}
	}
}

// For now, just indicate the tile type with a single pixel
void Map::draw() {
	// Get the display surface and lock it for access
	SDL_Surface *surface = SDL_GetVideoSurface();
	SDL_LockSurface(surface);

	Uint32 landcolor = SDL_MapRGB(surface->format,0x00,0xFF,0x00);
	Uint32 watercolor = SDL_MapRGB(surface->format,0x00,0x00,0xFF);

	for(unsigned int y = 0; y < height; y++) {
		for(unsigned int x = 0; x < width; x++) {
			*((Uint32 *) ((char *) surface->pixels + y*surface->pitch) + x)
				= map[y][x].type == TILE_LAND
				? landcolor : watercolor;
		}
	}

	// Done with the surface
	SDL_UnlockSurface(surface);
}

