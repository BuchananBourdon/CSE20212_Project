#ifndef JOIN_MESSAGE_H
#define JOIN_MESSAGE_H

#include "Message.h"

class JoinMessage : public Message {
public:
	JoinMessage();

	static void handle(Game &, Message &); // Deserialize and execute

private:
	static const int datasize; // Expected payload
};

#endif

