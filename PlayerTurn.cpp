#include <SDL/SDL_net.h>

#include "PlayerTurn.h"

// Destructor: free all the orders
PlayerTurn::~PlayerTurn() {
	for(unsigned int i = 0; i < orders.size(); i++)
		if(orders[i]) delete orders[i];
}

PlayerTurn PlayerTurn::deserialize(Uint8 **data) {
	int turnid = SDLNet_Read32(*data);
	*data += 4;

	int playerid = **data;
	*data += 1;

	int numorders = SDLNet_Read16(*data);
	*data += 2;

	vector<Order *> orders(numorders);
	for(int i = 0; i < numorders; i++)
		orders.push_back(Order::deserialize(data));

	return PlayerTurn(turnid,playerid,orders);
}

vector<Uint8> PlayerTurn::serialize() {
	vector<Uint8> data;

	data.push_back(0); data.push_back(0);
	data.push_back(0); data.push_back(0);
	SDLNet_Write32(turnid,&data[data.size() - 4]);

	data.push_back(playerid);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(orders.size(),&data[data.size() - 2]);

	for(unsigned int i = 0; i < orders.size(); i++) {
		vector<Uint8> orderdata = orders[i]->serialize();
		data.insert(data.end(),orderdata.begin(),orderdata.end());
	}

	return data;
}

