// Create unit order format: [type:1][x:2][y:2]

#include <iostream>

#include "BlackHole.h"
#include "CreateUnitOrder.h"
#include "Bunny.h"

using namespace std;

Order *CreateUnitOrder::deserialize(Uint8 *&data) {
	Uint8 unittype = *data++;

	Uint16 x = SDLNet_Read16(data);
	data += 2;

	Uint16 y = SDLNet_Read16(data);
	data += 2;

	return new CreateUnitOrder(unittype,x,y);
}

void CreateUnitOrder::execute(Game &game, Uint8 playerid) {
	cerr << "creating a unit of type " << (int) unittype << " at (" << x
		<< ", " << y << ")" << endl;

	switch(unittype) {
	case Unit::UT_BLACK_HOLE:
		game.addUnit(playerid,new BlackHole(x,y));
		break;
	
	case Unit::UT_BUNNY:
		game.addUnit(playerid,new Bunny(x,y));
		break;
	
	default:
		cerr << "warning: order to create unknown unit type "
			<< unittype << endl;
		break;
	}
}

void CreateUnitOrder::serialize(std::vector<Uint8> &data) {
	data.push_back(type);

	data.push_back(unittype);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(x,&data[data.size() - 2]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(y,&data[data.size() - 2]);
}

