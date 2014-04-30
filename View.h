struct View;

#ifndef VIEW_H
#define VIEW_H

#include "Map.h"

struct View {
	View(Uint16, Uint16, Uint16);
	View(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h, Uint16 _zoom)
		: x(_x), y(_y), w(_w), h(_h), zoom(_zoom) {}

	void adjustZoom(Map &, int, int, int); // Convenience function

	static const Uint16 minzoom;  // No further than
	static const Uint16 maxzoom;  // No closer than
	static const Uint16 zoomstep; // Granularity

	float x, y;
	Uint16 w, h;

	Uint16 zoom;
};

#endif

