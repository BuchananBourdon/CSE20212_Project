// Move unit order format: [unitid:2][x:2][y:2]

#include <iostream>

#include "MoveUnitOrder.h"

using namespace std;

Order *MoveUnitOrder::deserialize(Uint8 *&data) {
	int unitid = SDLNet_Read16(data);
	data += 2;

	int x = SDLNet_Read16(data);
	data += 2;

	int y = SDLNet_Read16(data);
	data += 2;

	return new MoveUnitOrder(unitid,x,y);
}

void MoveUnitOrder::execute(Game &game, Uint8 playerid) {
//	cerr << "ordering player " << (int) playerid << "'s unit " << unitid
//		<< " to move to (" << x << ", " << y << ")" << endl;

	game.moveUnit(playerid,unitid,x,y);
}

void MoveUnitOrder::serialize(vector<Uint8> &data) {
	data.push_back(type);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(unitid,&data[data.size() - 2]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(x,&data[data.size() - 2]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(y,&data[data.size() - 2]);
}

