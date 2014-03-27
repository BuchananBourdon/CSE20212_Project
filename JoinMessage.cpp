// Join message format: [version:1]

#include <iostream>

#include "IPaddress.h"
#include "JoinMessage.h"
#include "JoinResponseMessage.h"

using namespace std;

const unsigned int JoinMessage::datasize = 1;

JoinMessage::JoinMessage() : Message(true,MT_JOIN) {
	cout << "sending a join request" << endl;

	data.push_back(version);
}

// Parse, check, and act on a request to join the game we're hosting
void JoinMessage::handle(Game &game, Message &message) {
	// Sanity check
	if(!game.isHosting()) {
		cerr << "warning: received join request, but not hosting"
			<< endl;
		return;
	}

	vector<Uint8> &data = message.getData();
	if(data.size() != datasize) {
		cerr << "warning: join request incorrect length (is "
			<< data.size() << ", should be " << datasize << ")"
			<< endl;
		return;
	}

	Uint8 *datap = &data[0]; // Keep track of where we are

	// Make sure we're speaking the same language
	Uint8 remoteversion = *datap++;
	if(remoteversion != version) {
		cerr << "warning: received join request from incompatible"
			" version " << remoteversion << " (current version is "
			<< version << ")" << endl;
		return;
	}

	// Passed all the tests--they can join
	cout << "received join request from " << message.getAddress() << endl;

	SDLNet_UDP_Bind(game.getSocket(),0,&message.getAddress());

	game.sendMessage(new JoinResponseMessage(game.addPlayer(),
		game.getSeed()));
}

