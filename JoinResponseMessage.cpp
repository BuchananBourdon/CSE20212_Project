// Join response message format: [playerid:1]

#include <iostream>

#include "IPaddress.h"
#include "JoinResponseMessage.h"

using namespace std;

JoinResponseMessage::JoinResponseMessage(Uint8 _playerid)
	: Message(true,MT_JOIN_RESPONSE) {
	data.push_back(_playerid);
}

void JoinResponseMessage::handle(Game &game, Message &message) {
	// Sanity check
	if(game.isHosting()) {
		cerr << "warning: received join-request response, but hosting"
			<< endl;
		return;
	}

	// 0 if rejected, otherwise the player's ID
	Uint8 playerid = message.getData()[0];
	if(playerid) {
		cout << "joined game at " << message.getAddress()
			<< " as player " << (int) playerid << endl;

		game.setPlayerId(playerid);
	} else cout << "refused by host at " << message.getAddress() << endl;
}

