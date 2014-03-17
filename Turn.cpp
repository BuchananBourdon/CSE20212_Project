#include "Turn.h"

Turn::~Turn() {
	for(unsigned int i = 0; i < orders.size(); i++)
		if(orders[i]) delete orders[i];
}

