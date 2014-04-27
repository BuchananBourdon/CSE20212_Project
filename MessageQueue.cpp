#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "MessageQueue.h"

using namespace std;

MessageQueue::~MessageQueue() {
	if(socket) SDLNet_UDP_Close(socket);

	while(sendqueue.size()) {
		delete sendqueue.front();
		sendqueue.pop_front();
	}

	for(set<Message *>::iterator it = receivequeue.begin();
		it != receivequeue.end(); it++)
		delete *it;
}

void MessageQueue::add(Message *message) {
	message->setSequence(numsent++);
	sendqueue.push_back(message);
}

void MessageQueue::send() {
	Uint8 data[0xFFFF];
	UDPpacket packet;

	packet.data = data;
	packet.maxlen = 0xFFFF;

	// Clear away everything that has been ACKed
	while(sendqueue.size() && sendqueue.front()->getSequence() < numacked)
		sendqueue.pop_front();

	// (Re)send everything else
	for(list<Message *>::iterator it = sendqueue.begin();
		it != sendqueue.end(); it++) {
		(*it)->setAck(numreceived);

		(*it)->getPacket(packet);

		SDLNet_UDP_Send(socket,0,&packet);
	}
}

void MessageQueue::receive(Game &game) {
	// Have storage for a packet ready
	Uint8 data[0xFFFF];
	UDPpacket packet;

	packet.data = data;
	packet.maxlen = 0xFFFF;

	// Save the packets as we get them
	while(SDLNet_UDP_Recv(socket,&packet) == 1) {
		Message *message = new Message(packet);

		// Only save new ones
		if(message->getSequence() >= numreceived)
			receivequeue.insert(new Message(packet));
	}

	// Process as many as we can, in order
	while(receivequeue.size()) {
		Message *message = *receivequeue.begin();
		if(message->getSequence() != numreceived) {
			if(message->getSequence() < numreceived) {
				receivequeue.erase(receivequeue.begin());
				delete message;
			}

			break;
		}

		message->handle(game);

		numacked = max(numacked,message->getAck());

		receivequeue.erase(receivequeue.begin());
		delete message;

		numreceived++;
	}
}

