#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include <string>

#include <SDL/SDL_net.h>

#include "Player.h"
#include "PlayerTurn.h"

using namespace std;

class NetworkPlayer : public Player {
public:
	NetworkPlayer(UDPsocket, Uint8);

	// From Player
	virtual void receivePlayerTurn(PlayerTurn *);
	virtual void update();

private:
	static const int maxpacketsize = 65507; // 0xFFFF - 20 (IP) - 8 (UDP)

	UDPsocket socket; // To whom am I connected?
};

#endif

