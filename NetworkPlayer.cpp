#include <iostream>

#include "NetworkPlayer.h"

using namespace std;

// Constructor: listen for/contact a remote host
NetworkPlayer::NetworkPlayer(UDPsocket _socket, int _id)
	: Player(_id), socket(_socket) {
	// TODO: implement NetworkPlayer::NetworkPlayer()
}

// Send this to the remote player
void NetworkPlayer::receivePlayerTurn(PlayerTurn &playerturn) {
	// TODO: implement NetworkPlayer::receivePlayerTurn()
}

// Check for anything new from the remote player
void NetworkPlayer::update() {
	// TODO: implement NetworkPlayer::update()
}

