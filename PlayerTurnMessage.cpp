// Player turn message format: [turnid:4][playerid:1][orders...]

#include <iostream>

#include "PlayerTurnMessage.h"

using namespace std;

PlayerTurnMessage::PlayerTurnMessage(PlayerTurn *turn)
	: Message(true,MT_PLAYER_TURN) {
//	cerr << "broadcasting a player turn for turn " << turn->getTurnId()
//		<< endl;

	// Send the turn ID
	data.push_back(0); data.push_back(0);
	data.push_back(0); data.push_back(0);
	SDLNet_Write32(turn->getTurnId(),&data[0]);

	// Send the player ID
	data.push_back(turn->getPlayerId());

	// Send each order
	const vector<Order *> orders = turn->getOrders();
	for(unsigned int i = 0; i < orders.size(); i++)
		orders[i]->serialize(data);
}

void PlayerTurnMessage::handle(Game &game, Message &message) {
	Uint8 *datap = &message.getData()[0];

	// Load the turn ID
	int turnid = SDLNet_Read32(datap);
	datap += 4;

	// Load the player ID
	Uint8 playerid = *datap++;

	PlayerTurn *turn = new PlayerTurn(turnid,playerid);

//	cerr << "received a player turn from player " << (int) playerid
//		<< " for turn " << turnid << endl;

	// Load the orders
	while(datap && datap < &*message.getData().end())
		turn->addOrder(Order::deserialize(datap));

	game.addPlayerTurn(turn);
}

