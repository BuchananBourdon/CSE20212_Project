#ifndef VIEW_H
#define VIEW_H

struct View {
	View(Uint16 _x, Uint16 _y, Uint16 _w, Uint16 _h, Uint16 _zoom)
		: x(_x), y(_y), w(_w), h(_h), zoom(_zoom) {};

	Uint16 x, y;
	Uint16 w, h;

	Uint16 zoom;
};

#endif

