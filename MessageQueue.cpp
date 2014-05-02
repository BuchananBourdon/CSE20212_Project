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

	for(map<Uint32,Message *>::iterator it = receivequeue.begin();
		it != receivequeue.end(); it++)
		delete it->second;
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
	if(SDL_GetTicks() - lastsend > 300) {
		lastsend = SDL_GetTicks();
		for(list<Message *>::iterator it = sendqueue.begin();
			it != sendqueue.end(); it++) {
			(*it)->setAck(numreceived);

			(*it)->getPacket(packet);

			SDLNet_UDP_Send(socket,0,&packet);
		}
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
		Uint32 sequence = message->getSequence();

		// Only save new ones
		if(sequence >= numreceived && !receivequeue.count(sequence))
			receivequeue.insert(make_pair(sequence,message));
		else delete message;
	}

	// Process as many as we can, in order
	while(receivequeue.count(numreceived)) {
		Message *message = receivequeue[numreceived];

//		cerr << "handling message " << message->getSequence() << endl;
		message->handle(game);

		numacked = max(numacked,message->getAck());

		receivequeue.erase(numreceived);
		delete message;

		numreceived++;
	}
}

