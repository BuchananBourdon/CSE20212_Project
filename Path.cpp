#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

#include "Path.h"

using namespace std;

// Return the next step in the path
Path::Location Path::step(Map &map) {
	// Make sure we have a path to step along
	if(path.empty()) calcPath(map);
	if(path.empty()) return from;

	Location loc = path.front();
	path.pop_front();

	// If we're blocked, calculate a new path
	if(!canStep(map,loc)) {
		calcPath(map);
		if(path.empty()) return from;

		loc = path.front();
		path.pop_front();
	}

	return from = loc;
}

// Returns whether a is worse than b
bool Path::CompareStep::operator()(Path::Step *&a, Path::Step *&b) {
	// First, sort by total cost
	pair<Uint16,Uint16> adist = a->getDistance();
	pair<Uint16,Uint16> bdist = b->getDistance();
	int diff = (a->cost + adist.first) - (b->cost + bdist.first);

	if(diff > 0) return true;
	else if(diff < 0) return false;

	// Next, sort by estimated distance remaining
	diff = adist.first - bdist.first;

	if(diff > 0) return true;
	else if(diff < 0) return false;

	// Finally, sort by heading (for visual intuitiveness)
	return adist.second > bdist.second;
}

// Returns the long and short distances, in order
pair<Uint16,Uint16> Path::Step::getDistance() const {
	Uint16 distx = loc.x > to->x ? loc.x - to->x : to->x - loc.x;
	Uint16 disty = loc.y > to->y ? loc.y - to->y : to->y - loc.y;

	if(distx > disty) return pair<Uint16,Uint16>(distx,disty);
	else return pair<Uint16,Uint16>(disty,distx);
}

// Is it okay to step to loc?
bool Path::canStep(Map &map, Location &loc) {
	for(Uint16 y = loc.y; y < loc.y + h; y++)
		for(Uint16 x = loc.x; x < loc.x + w; x++)
			if(map.isOccupied(x,y)
				|| map.tileType(x,y) != Map::TILE_LAND)
				return false;
	return true;
}

// Generate a path through map, using the A* algorithm
void Path::calcPath(Map &map) {
	list<Step *> steps;
	priority_queue<Step *,vector<Step *>,CompareStep> open;

	vector<bool> *searched;
	searched = new vector<bool>[map.getHeight()]();
	for(unsigned int i = 0; i < map.getHeight(); i++)
		searched[i].resize(map.getWidth());

	// Start at from
	steps.push_front(new Step(from,&to,0,NULL));
	open.push(steps.front());

	// Add the neighbors of the best candidate until we find it or give up
	while(!open.empty()
		&& (open.top()->loc.x != to.x || open.top()->loc.y != to.y)) {
		Step *s = open.top();
		open.pop();

		for(int dir = 0; dir < 8; dir++) {
			int dx = (dir&3) == 1 ? 0 : !(dir&4) ^ !(dir&2)
				? 1 : -1;
			int dy = dir < 3 ? -1 : (dir&3) == 3 ? 0 : 1;

			Location newloc(s->loc.x + dx,s->loc.y + dy);

			if(newloc.x >= 0
				&& newloc.x < (int) map.getWidth()
				&& newloc.y >= 0
				&& newloc.y < (int) map.getHeight()
				&& !searched[newloc.y][newloc.x]
				&& canStep(map,newloc)) {
				steps.push_front(new Step(newloc,s->to,
					s->cost + 1,s));
				open.push(steps.front());
				searched[newloc.y][newloc.x] = true;
			}
		}
	}

	// Save the path we found
	if(!open.empty()) {
		for(Step *step = open.top(); step; step = step->prev)
			path.push_front(step->loc);
		path.pop_front();
	}

	// Free all of the temporary data
	for(list<Step *>::iterator iter = steps.begin();
		iter != steps.end(); iter++)
		delete *iter;

	delete[] searched;
}

