#ifndef UNIT_H
#define UNIT_H

class Unit {
public:
	Unit(int, int);

	virtual int getType() = 0;

private:
	static int unitcount; // For unique IDs

	int x, y; // Where am I?
};

#endif

