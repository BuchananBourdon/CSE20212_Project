#include <iostream>

#include "Map.h"

using namespace std;

const Uint8 Map::roughness = 0xFF;

const Uint8 Map::waterlevel = 0x38;
const Uint8 Map::mountainlevel = 0x80;

const int Map::numrivers = 20;
const int Map::riverlength = 20;

Map::Map(unsigned int size, Random *r) : width(size), height(size) {
	if(!size || size - 1 & size - 2)
		cerr << "warning: map generation may not work properly for map"
			" size != 2^n + 1" << endl;

	// Allocate the map
	map = new struct map_tile *[height];
	for(unsigned int y = 0; y < height; y++)
		map[y] = new struct map_tile[width];

	// Set up the corners consistently
	map[         0][        0].height = 0x00;
	map[height - 1][        0].height = 0x80;
	map[         0][width - 1].height = 0x80;
	map[height - 1][width - 1].height = 0x00;

	// Use the diamond-square algorithm to perturb the land
	perturb(0,r);

	// Tile type depends on height
	for(unsigned int y = 0; y < height; y++) {
		for(unsigned int x = 0; x < width; x++) {
			if(map[y][x].height < mountainlevel)
				map[y][x].type = TILE_LAND;
			else map[y][x].type = TILE_MOUNTAIN;
		}
	}

	// Add some river obstacles
	for(int i = 0; i < numrivers; i++)
		trace_river(r);
cerr << "done with map generation at time " << time(NULL) << endl;
}

Map::~Map() {
	for(unsigned int y = 0; y < height; y++)
		delete[] map[y];
	delete[] map;
}

void Map::draw(const View &view) {
	SDL_Surface *surface = SDL_GetVideoSurface();

	for(unsigned int y = view.y; y < height && y < view.y + view.h; y++) {
		for(unsigned int x = view.x;
			x < width && x < view.x + view.w; x++) {
			SDL_Rect rect;
			rect.x = (x - view.x)*view.zoom;
			rect.y = (y - view.y)*view.zoom;
			rect.w = rect.h = view.zoom;

			Uint32 color;
			switch(map[y][x].type) {
			case TILE_WATER:
				color = SDL_MapRGB(surface->format,
					0x00,0x00,0xFF);
				break;

			case TILE_LAND:
				color = SDL_MapRGB(surface->format,
					0x00,map[y][x].height,0x00);
				break;

			case TILE_MOUNTAIN:
				color = SDL_MapRGB(surface->format,
					0x80,0x80,0x80);
				break;
			}

			SDL_FillRect(surface,&rect,color);
		}
	}
}

// Recursive method for the diamond-square generation method
void Map::perturb(int level, Random *r) {
	unsigned int divisions = 1 << level;

	if(divisions > width || divisions > height) return;

	for(unsigned int yi = 0; yi < divisions; yi++) {
		for(unsigned int xi = 0; xi < divisions; xi++) {
			int x = xi*(width - 1)/divisions;
			int y = yi*(height - 1)/divisions;
			int w = (xi + 1)*(width - 1)/divisions - x;
			int h = (yi + 1)*(height - 1)/divisions - y;

			// We need at least a 3x3 area to do anything
			if(w < 2 || h < 2) continue;

			// Center
			map[y + h/2][x + h/2].height = square_avg(x,y,w,h,r);

			// Left and top only if we're on the corresponding edge
			if(x == 0) map[y + h/2][x].height
				= diamond_avg(x,y,w,h,r);
			if(y == 0) map[y][x + w/2].height
				= diamond_avg(x + w/2,y - h/2,w,h,r);

			// Right and bottom
			map[y + h/2][x + w].height
				= diamond_avg(x + w,y,w,h,r);
			map[y + h][x + w/2].height
				= diamond_avg(x + w/2,y + h/2,w,h,r);
		}
	}

	// Perturb the next level
	perturb(level + 1,r);
}

// Compute the average of the corners of a "square" region
Uint8 Map::square_avg(int x, int y, int w, int h, Random *r) {
	return ((Uint32) map[y][x].height + map[y + h][x].height
		+ map[y][x + w].height + map[y + h][x + w].height
		+ (r->next()&0xFF)*roughness/0xFF)*0xFF/(4*0xFF + roughness);
}

// Compute the acerage of the corners of a "diamond" region,
// with the top of the diamond at (x, y)
Uint8 Map::diamond_avg(int x, int y, int w, int h, Random *r) {
	Uint32 sum = 0;
	int points = 0;

	if(y >= 0) {
		sum += map[y][x].height;
		points++;
	}

	if(x + w/2 < (int) width && y + h/2 < (int) height) {
		sum += map[y + h/2][x + w/2].height;
		points++;
	}

	if(y + h < (int) height) {
		sum += map[y + h][x].height;
		points++;
	}

	if(x - w/2 >= 0 && y + h/2 < (int) height) {
		sum += map[y + h/2][x - w/2].height;
		points++;
	}

	sum += (r->next()&0xFF)*roughness/0xFF;

	return sum*0xFF/(points*0xFF + roughness);
}

// Trace a out a river on the map
void Map::trace_river(Random *r) {
	// Trace out a river
	unsigned int x = r->next()%width;
	unsigned int y = r->next()%height;
	for(int i = 0; i < riverlength; i++) {
		map[y][x].type = TILE_WATER;

		// Flow in a random direction,
		// but not uphill if higher than waterlevel
		unsigned int newx, newy;

		int dirx = r->next()&0x7;
		newx = x + (dirx < 3 ? -1 : dirx < 5 ? 0 : 1);

		int diry = r->next()&0x7;
		newy = y + (diry < 3 ? -1 : diry < 5 ? 0 : 1);

		if(newx >= 0 && newx < width && newy >= 0 && newy < height
			&& (map[newy][newx].height < waterlevel
			|| map[newy][newx].height < map[y][x].height)) {
			x = newx;
			y = newy;
		}
	}
}

