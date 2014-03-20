#include <iostream>

#include "PlayerTurnMessage.h"

using namespace std;

PlayerTurnMessage::PlayerTurnMessage(PlayerTurn *turn)
	: Message(true,MT_PLAYER_TURN) {
	cerr << "broadcasting a player turn for turn " << turn->getTurnId()
		<< endl;
}

void PlayerTurnMessage::handle(Game &game, Message &message) {
	
}

