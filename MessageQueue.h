class MessageQueue;

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <list>
#include <set>

#include <SDL/SDL_net.h>

#include "Game.h"
#include "Message.h"

class MessageQueue {
public:
	MessageQueue(UDPsocket _socket)
		: socket(_socket), numsent(0), numacked(0), numreceived(0) {}
	~MessageQueue();

	UDPsocket getSocket() { return socket; }

	void add(Message *);

	void send();
	void receive(Game &);

private:
	UDPsocket socket;

	Uint32 numsent;     // For send sequencing
	Uint32 numacked;    // For re-sending
	Uint32 numreceived; // For receipt sequencing

	std::list<Message *> sendqueue;
	std::set<Message *> receivequeue;
};

#endif

