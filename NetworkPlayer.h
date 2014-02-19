#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include <string>

#include <SDL/SDL_net.h>

#include "Player.h"
#include "PlayerTurn.h"

using namespace std;

class NetworkPlayer : public Player {
public:
	NetworkPlayer(UDPsocket, int);

	// From Player
	virtual void receivePlayerTurn(PlayerTurn &);
	virtual void update();

private:

	UDPsocket socket; // To whom am I connected?
};

#endif

