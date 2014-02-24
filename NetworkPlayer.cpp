// Turn packet format: "rkapturn"...

#include <algorithm>
#include <cmath>
#include <iostream>

#include "NetworkPlayer.h"

using namespace std;

// Constructor: start communicating with the remote host
NetworkPlayer::NetworkPlayer(UDPsocket _socket, Uint8 _id)
	: Player(_id), socket(_socket) {}

// Send this to the remote player
void NetworkPlayer::receivePlayerTurn(PlayerTurn *playerturn) {
	// Don't send our own turn back!
	if(playerturn->playerid == id) return;

	Uint8 buffer[maxpacketsize];

	UDPpacket packet;
	packet.data = buffer;

	vector<Uint8> data = playerturn->serialize();

	packet.len = data.size() + 8;
	if(packet.len > maxpacketsize) {
		cerr << "error: serialized PlayerTurn is too big ("
			<< data.size() << " bytes)" << endl;
		return;
	}

	memcpy(packet.data,"rkapturn",8);
	memcpy(packet.data + 8,&data[0],data.size());

	SDLNet_UDP_Send(socket,0,&packet);

	// TODO: implement retry in NetworkPlayer::receivePlayerTurn()
}

// Check for anything new from the remote player
void NetworkPlayer::update() {
	Uint8 buffer[maxpacketsize];

	UDPpacket packet;
	packet.data = buffer;
	packet.maxlen = maxpacketsize;

	while(SDLNet_UDP_Recv(socket,&packet) == 1) {
		// Is this packet a player turn?
		if(strncmp((char *) packet.data,"rkapturn",8) != 0) return;

		Uint8 *dataptr = packet.data + 8;
		PlayerTurn *playerturn
			= PlayerTurn::deserialize(&dataptr);
		ownersim->broadcastPlayerTurn(playerturn);
		if(playerturn) delete playerturn;
	}
}

