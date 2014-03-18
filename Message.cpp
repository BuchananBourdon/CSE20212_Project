// Message format: [magic:4][type:1][data:...]

#include <algorithm>
#include <iostream>

#include "JoinMessage.h"
#include "JoinResponseMessage.h"
#include "Message.h"

using namespace std;

const int Message::headersize = 5;        // Total length of the fields below
const Uint32 Message::magic = 0x726b6170; // First four bytes of every message
const Uint8 Message::version = 0;         // To indicate incompatible changes

// Meant for creating a new message, probably using a child class
Message::Message(bool _valid, Uint8 _type) : valid(_valid), type(_type) {}

// Meant for deserializing a received message
Message::Message(UDPpacket &packet) {
	valid = false;

	// Sanity check
	if(packet.len < headersize) {
		cerr << "warning: packet of size " << packet.len
			<< " too short" << endl;
		return;
	}

	Uint8 *datap = packet.data; // Track where we are

	// Check the magic number
	if(SDLNet_Read32(datap) != magic) {
		cerr << "warning: foreign or corrupted packet" << endl;
		return;
	}
	datap += 4;

	// Get the message type
	if(type = *datap++, type >= MT_NUM_TYPES) {
		cerr << "warning: unknown message type (" << type << ")"
			<< endl;
		return;
	}

	// Copy the message body
	data.insert(data.end(),datap,packet.data + packet.len);

	// Copy the sender's address
	address = packet.address;

	valid = true;
}

// Export this message in a (preallocated) packet
void Message::getPacket(UDPpacket &packet) {
	// Sanity checks
	if(!packet.data) {
		cerr << "error: packet data pointer is NULL" << endl;
		return;
	}

	if(packet.len = headersize + data.size(), packet.len > packet.maxlen) {
		cerr << "error: packet data buffer is too small" << endl;
		return;
	}

	Uint8 *datap = packet.data; // Track where we are

	// Copy over the header
	SDLNet_Write32(magic,datap);
	datap += 4;

	// Copy over the type
	*datap++ = type;

	// Copy over the data
	copy(data.begin(),data.end(),datap);
}

// Handle the contents of the message, probably by delegating to a child class
void Message::handle(Game &game) {
	// Sanity check
	if(!valid) {
		cerr << "warning: cannot handle invalid message" << endl;
		return;
	}

	// Delegate the body based on its type
	switch(type) {
	case MT_JOIN:          JoinMessage::handle(game,*this);         break;
	case MT_JOIN_RESPONSE: JoinResponseMessage::handle(game,*this); break;

	default:
		cerr << "warning: unhandled message type (" << type << ")"
			<< endl;
		break;
	}
}

