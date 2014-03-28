#ifndef VIEW_H
#define VIEW_H

struct View {
	View(unsigned int _x, unsigned int _y, unsigned int _w,
		unsigned int _h, unsigned int _zoom)
		: x(_x), y(_y), w(_w), h(_h), zoom(_zoom) {};

	unsigned int x, y;
	unsigned int w, h;

	unsigned int zoom;
};

#endif

