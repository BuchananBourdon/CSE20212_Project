/*
 * Helper function for the IPaddress struct.
 */

#include <ostream>

#include <SDL/SDL_net.h>

#include "IPaddress.h"

using namespace std;

// Outputs address to stream
ostream &operator<<(ostream &stream, IPaddress &address) {
	Uint32 host = SDLNet_Read32(&address.host);
	Uint16 port = SDLNet_Read16(&address.port);

	stream << (host >> 24 & 0xFF) << "." << (host >> 16 & 0xFF) << "."
		<< (host >> 8 & 0xFF) << "." << (host & 0xFF) << ":" << port;

	return stream;
}

