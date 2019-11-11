#include "door.h"

stIO io;

door::door(param* inParam) {
	short test = inParam->eveningTime;
	resetOutputs();
}

door::~door()
{
	resetOutputs();
}



void door::moveUp(void)
{
	io.outMotUp = true;
}

void resetOutputs(void) {
	io.outMotDown = false;
	io.outMotUp = false;
}