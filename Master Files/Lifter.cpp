#include "Lifter.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
Lifter::Lifter()
	: motor(0)
{
	// Doesn't do anything yet
}

Lifter::Lifter(unsigned int jaguar, Encoder &eEncoder)
	: motor(jaguar),
	  lifterEncoder(&eEncoder)
{
	lifterEncoder->Reset();
}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void Lifter::GoUp() {
	motor.SetSpeed(LIFTER_UP_SPEED);
}

// Sets the lifter to go down
void Lifter::GoDown() {
	motor.SetSpeed(LIFTER_DOWN_SPEED);
}
