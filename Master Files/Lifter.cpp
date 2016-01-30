#include "Lifter.h"
#include "WPILib.h"
#include "Definitions.h"



Lifter::Lifter() {
	// Doesn't do anything yet
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
