#ifndef PLAYER_TURN_MESSAGE_H
#define PLAYER_TURN_MESSAGE_H

#include "Message.h"
#include "PlayerTurn.h"

class PlayerTurnMessage : public Message {
public:
	PlayerTurnMessage(PlayerTurn *);

	static void handle(Game &, Message &);
};

#endif

