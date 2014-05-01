#include <iostream>

#include "HangupMessage.h"

using namespace std;

HangupMessage::HangupMessage() : Message(true,MT_HANGUP) {
	cout << "sending hangup message" << endl;
}

void HangupMessage::handle(Game &game, Message &message) {
	cout << "received hangup message" << endl;

	game.setWon(true);
	game.exit();
}

