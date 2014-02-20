#include <SDL/SDL_net.h>

#include "MouseMoveOrder.h"

int MouseMoveOrder::deserializerid = -1;

// Constructor
MouseMoveOrder::MouseMoveOrder(Uint32 _time, Uint8 _player, Uint16 _id,
	Uint16 _startx, Uint16 _starty, Uint16 _endx, Uint16 _endy)
	: Order(_time,_player,_id), startx(_startx), starty(_starty),
	endx(_endx), endy(_endy) {
	// Register this class with Order, if we haven't already
	if(deserializerid < 0)
		deserializerid = Order::addDeserializer(deserialize);
}

// Deserialize from byte stream
Order *MouseMoveOrder::deserialize(Uint8 **data, Uint32 time, Uint8 player,
	Uint16 id) {
	Uint16 startx = SDLNet_Read16(*data);
	*data += 2;

	Uint16 starty = SDLNet_Read16(*data);
	*data += 2;

	Uint16 endx = SDLNet_Read16(*data);
	*data += 2;

	Uint16 endy = SDLNet_Read16(*data);
	*data += 2;

	return new MouseMoveOrder(time,player,id,startx,starty,endx,endy);
}

// Just for testing: draw a line from (startx, starty) to (endx, endy)
void MouseMoveOrder::execute(GameSimulation &simulation) {
	SDL_Surface *image = simulation.getImage();

	Uint32 color;

	if(player == 0) color = SDL_MapRGB(image->format,0xFF,0,0);
	else color = SDL_MapRGB(image->format,0,0xFF,0);

	int x = startx, y = starty;

	while(x != endx || y != endy) {
		// Color in this pixel
		*(((Uint32 *) image->pixels) + (y*image->pitch + x)) = color;

		// Move to the next pixel
		if((x - startx)*(endy - starty) < (y - starty)*(endx - startx))
			x++;
		else y++;
	}
}

// Export to byte stream
vector<Uint8> MouseMoveOrder::serialize() {
	vector<Uint8> data;

	data.push_back(0); data.push_back(0);
	data.push_back(0); data.push_back(0);
	SDLNet_Write32(time,&data[0]);

	data.push_back(player);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(id,&data[5]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(startx,&data[7]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(starty,&data[9]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(endx,&data[11]);

	data.push_back(0); data.push_back(0);
	SDLNet_Write16(endy,&data[13]);

	return data;
}

