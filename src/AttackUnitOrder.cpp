// Attack unit order format: [unitid:2][targetid:2]

#include <iostream>

#include "AttackUnitOrder.h"

using namespace std;

Order *AttackUnitOrder::deserialize(Uint8 *&data) {
	int unitid = SDLNet_Read16(data);
	data += 2;

	int targetid = SDLNet_Read16(data);
	data += 2;

	return new AttackUnitOrder(unitid,targetid);
}

void AttackUnitOrder::execute(Game &game, Uint8 playerid) {
//	cerr << "ordering player " << (int) playerid << "'s unit " << unitid
//		<< " to attack unit " << targetid << endl;

	game.attackUnit(playerid,unitid,targetid);
}

void AttackUnitOrder::serialize(vector<Uint8> &data) {
	data.push_back(type);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(unitid,&data[data.size() - 2]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(targetid,&data[data.size() - 2]);
}

