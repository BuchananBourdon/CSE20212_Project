#include <iostream>
#include "SDL/SDL_image.h"
#include "Map.h"

using namespace std;

const Uint8 Map::roughness = 0xFF;

const Uint8 Map::waterlevel = 0x38;
const Uint8 Map::mountainlevel = 0x80;

const int Map::numrivers = 10;
const int Map::riverlength = 50;

SDL_Surface* Map::mountainSurface = NULL;
SDL_Surface* Map::grassSurface = NULL;
SDL_Surface* Map::waterSurface = NULL;
SDL_Rect Map::mountainClips[17][5];
SDL_Rect Map::grassClips[17];
SDL_Rect Map::waterClips[17];

Map::Map(unsigned int size, Random *r) : width(size), height(size) {
	if(!size || size - 1 & size - 2)
		cerr << "warning: map generation may not work properly for map"
			" size != 2^n + 1" << endl;

	// Allocate the map
	map = new struct map_tile *[height];
	for(unsigned int y = 0; y < height; y++)
		map[y] = new struct map_tile[width]();

	// Set up the corners consistently
	map[         0][        0].height = r->next();
	map[height - 1][        0].height = r->next();
	map[         0][width - 1].height = r->next();
	map[height - 1][width - 1].height = r->next();

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
	
	if(mountainSurface == NULL) {
                SDL_Surface * loadedImage = IMG_Load("Mountains.png");
		SDL_Surface * loadedImage2 = IMG_Load("Grass.png");
		SDL_Surface * loadedImage3 = IMG_Load("Water.png");
                SDL_Surface * optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
		SDL_Surface * optimizedImage2 = SDL_DisplayFormatAlpha( loadedImage2 );
		SDL_Surface * optimizedImage3 = SDL_DisplayFormatAlpha( loadedImage3 );
                SDL_FreeSurface(loadedImage);
		SDL_FreeSurface(loadedImage2);
		SDL_FreeSurface(loadedImage3);
                mountainSurface = optimizedImage;
		grassSurface = optimizedImage2;
		waterSurface = optimizedImage3;
                setClips();
        }
	
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
			
			//these variables are used in the surface blitting of the moutains. Confines snow-capped mountains to specific regions
			int mtnIndex = (x+y)/2 > (width+height)/8 ? 3:2;
			int add = mtnIndex==3 ? 0:3;

			switch(map[y][x].type) {
			case TILE_NONE: break;

			case TILE_WATER:
				SDL_BlitSurface(waterSurface,&waterClips[16-(((view.zoom+2)/6)-1)],surface,&rect);
				break;
			case TILE_LAND:
				SDL_BlitSurface(grassSurface,&grassClips[16-(((view.zoom+2)/6)-1)],surface,&rect);
				break;
			case TILE_MOUNTAIN:
				SDL_BlitSurface(mountainSurface,&mountainClips[16-(((view.zoom+2)/6)-1)][(x+y)%mtnIndex+add],surface,&rect);
				break;
			}


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

		unsigned int minheight = 0x100;
		unsigned int newx = x, newy = y;

		// Flow in the direction of least elevation
		for(int dir = 0; dir < 4; dir++) {
			int dx = dir&1 ? dir&2 ? -1 : 1 : 0;
			int dy = dir&1 ? 0 : dir&2 ? 1 : -1;

			if(x + dx >= 0 && x + dx < width && y + dy >= 0
				&& y + dy < height
				&& map[y + dy][x + dx].type != TILE_WATER
				&& map[y + dy][x + dx].height < minheight) {
				newx = x + dx;
				newy = y + dy;
				minheight = map[newy][newx].height;
			}
		}

		if(newx == x && newy == y)
			break;

		x = newx;
		y = newy;
	}
}

// set the clips for the 3 different surfaces
void Map::setClips() {
	
	for(int i = 0;i<17;i++) {
		for(int j = 0;j<5;j++) {	//double for-loop eliminates ~300 lines of manual code here
			mountainClips[i][j].x = (100-i*6) * j;
			mountainClips[i][j].y = 880-(16-i)*(55-3*i);	//each successive y-direction clip gets smaller
			mountainClips[i][j].w = 100-i*6;
			mountainClips[i][j].h = 100-i*6; 
		}

		grassClips[i].x = 0;
		grassClips[i].y = 880-(16-i)*(55-3*i);
		grassClips[i].w = 100-i*6;
		grassClips[i].h = 100-i*6;

		waterClips[i].x = 0;
		waterClips[i].y = 880-(16-i)*(55-3*i);
		waterClips[i].w = 100-i*6;
		waterClips[i].h = 100-i*6;
	}
	
}

void Map::occupy(Uint16 x, Uint16 y) {
	// Sanity check
	if(isOccupied(x,y)) {
		cerr << "warning: attempt to occupy occupied tile" << endl;
		return;
	}

	map[y][x].occupied = true;
}

bool Map::isOccupied(Uint16 x, Uint16 y) {
	// Sanity check
	if(x >= width || y >= height) {
		cerr << "warning: attempt to access out-of-bounds tile (" << x
			<< ", " << y << ")" << endl;
		return true;
	}

	return map[y][x].occupied;
}

void Map::clear(Uint16 x, Uint16 y) {
	if(x < width && y < height)
		map[y][x].occupied = false;
}

enum Map::tile_type Map::tileType(Uint16 x, Uint16 y) {
	// Sanity check
	if(x >= width || y >= height) {
		cerr << "warning: attempt to get type of out-of-bounds tile"
			<< endl;
		return TILE_NONE;
	}

	return map[y][x].type;
}
