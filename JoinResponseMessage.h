#ifndef JOIN_RESPONSE_MESSAGE_H
#define JOIN_RESPONSE_MESSAGE_H

#include "Message.h"

class JoinResponseMessage : public Message {
public:
	JoinResponseMessage(Uint8); // (playerid)

	static void handle(Game &, Message &); // Deserialize and execute
};

#endif

