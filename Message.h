class Message;

#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>

#include <SDL/SDL_net.h>

#include "Game.h"

class Message {
public:
	Message(bool, Uint8); // For composing
	Message(UDPpacket &); // For receiving

	std::vector<Uint8> &getData()    { return data; }
	IPaddress          &getAddress() { return address; }

	void getPacket(UDPpacket &); // For sending

	void handle(Game &); // Deserialize and execute

protected:
	static const int headersize; // Fields below
	static const Uint32 magic;   // Start of message
	static const Uint8 version;  // Compatibility check

	enum message_type {
		MT_HANGUP,
		MT_JOIN,
		MT_JOIN_RESPONSE,
		MT_PLAYER_TURN,

		MT_NUM_TYPES
	};

	bool valid;              // Header OK?
	Uint8 type;              // Message type
	std::vector<Uint8> data; // Message body

	IPaddress address; // From where?
};

#endif

