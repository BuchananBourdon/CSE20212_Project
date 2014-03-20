#include "Turn.h"

void Turn::execute() {
	for(unsigned int i = 0; i < orders.size(); i++)
		for(unsigned int j = 0; j < orders[i].size(); j++)
			orders[i][j]->execute();
}

