#include <iostream>

#include "NetworkPlayer.h"

using namespace std;

// Constructor: listen for/contact a remote host
NetworkPlayer::NetworkPlayer(GameSimulation &_ownersim, int _id,
	string address, bool listen) : Player(_ownersim,_id) {
	if(listen) {
		cerr << "listening on " << address << endl;
		// TODO: listen for a connection
	} else {
		cerr << "attempting to connect to " << address << endl;
		// TODO: connect to a game host
	}
}

// Send this to the remote host
void NetworkPlayer::receivePlayerTurn(PlayerTurn &playerturn) {
	// TODO: implement NetworkPlayer::receivePlayerTurn()
}

// Check for anything new from the remote host
void NetworkPlayer::update() {
	// TODO: implement NetworkPlayer::update()
}

