#ifndef HANGUP_MESSAGE_H
#define HANGUP_MESSAGE_H

#include "Message.h"

class HangupMessage : public Message {
public:
	HangupMessage();

	static void handle(Game &, Message &); // Deserialize and execute
};

#endif

