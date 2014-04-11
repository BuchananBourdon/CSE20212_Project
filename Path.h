#ifndef PATH_H
#define PATH_H

#include <deque>
#include <utility>

#include "Map.h"

class Path {
public:
	Path(Uint16 _fromx, Uint16 _fromy, Uint16 _tox, Uint16 _toy,
		Uint16 _w, Uint16 _h)
		: from(_fromx,_fromy), to(_tox,_toy), w(_w), h(_h) {}


	struct Location {
		Location(Uint16 _x, Uint16 _y) : x(_x), y(_y) {}

		Uint16 x, y;
	};

	Location step(Map &); // Ge the next step

private:
	// For use in calcPath()
	struct Step {
		Step(Location _loc, Location *_to, unsigned int _cost,
			Step *_prev)
			: loc(_loc), to(_to), cost(_cost), prev(_prev) {}

		std::pair<Uint16,Uint16> getDistance() const;

		Location loc;
		Location *to;

		unsigned int cost;

		Step *prev;
	};

	struct CompareStep {
		bool operator() (Step *&, Step *&);
	};

	bool canStep(Map &, Location &); // Valid step?
	void calcPath(Map &);            // Generate path

	Location from;
	Location to;

	Uint16 w;
	Uint16 h;

	std::deque<Location> path;
};

#endif

