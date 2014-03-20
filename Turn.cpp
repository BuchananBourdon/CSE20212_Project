#include "Turn.h"

void Turn::execute() {
	for(unsigned int i = 0; i < playerturns.size(); i++)
		playerturns[i]->execute();
}

