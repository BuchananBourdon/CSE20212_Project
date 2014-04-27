// Message format: [magic:4][sequence:4][ack:4][type:1][data:...]

#include <algorithm>
#include <iostream>

#include "HangupMessage.h"
#include "JoinMessage.h"
#include "JoinResponseMessage.h"
#include "Message.h"
#include "PlayerTurnMessage.h"

using namespace std;

const int Message::headersize = 13;       // Total length of the header fields
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

	// Get the sequence number
	sequence = SDLNet_Read32(datap);
	datap += 4;

	// Get the ack number
	ack = SDLNet_Read32(datap);
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

// Compare this and that via sequence numbers
bool Message::operator<(const Message &that) {
	return sequence < that.getSequence();
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

	// Copy over the sequence number
	SDLNet_Write32(sequence,datap);
	datap += 4;

	// Copy over the ack number
	SDLNet_Write32(ack,datap);
	datap += 4;

	// Copy over the type
	*datap++ = type;

	// Copy over the data
	copy(data.begin(),data.end(),datap);
}

// Indicate the queueing order of this message
void Message::setSequence(Uint32 _sequence) {
	sequence = _sequence;
}

// Indicate how many message we've received
void Message::setAck(Uint32 _ack) {
	ack = _ack;
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
	case MT_HANGUP:        HangupMessage::handle(game,*this);       break;
	case MT_JOIN:          JoinMessage::handle(game,*this);         break;
	case MT_JOIN_RESPONSE: JoinResponseMessage::handle(game,*this); break;
	case MT_PLAYER_TURN:   PlayerTurnMessage::handle(game,*this);   break;

	default:
		cerr << "warning: unhandled message type (" << type << ")"
			<< endl;
		break;
	}
}

