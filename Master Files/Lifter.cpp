#include "Lifter.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
Lifter::Lifter()
	: liftMotor(0),
	  tiltMotor(0)
{
	// Doesn't do anything yet
}

Lifter::Lifter(unsigned int jaguarLift, unsigned int jaguarTilt, Encoder &eEncoder)
	: liftMotor(jaguarLift),
	  tiltMotor(jaguarTilt),
	  lifterEncoder(&eEncoder)
{
	lifterEncoder->Reset();
}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void Lifter::LiftUp() {
	liftMotor.SetSpeed(LIFTER_UP_SPEED);
}

// Sets the lifter to go down
void Lifter::LiftDown() {
	liftMotor.SetSpeed(LIFTER_DOWN_SPEED);

}


// Set the tilter to go up
void Lifter::TiltUp() {
	tiltMotor.SetSpeed(LIFTER_UP_SPEED);
}

// Sets the tilter to go down
void Lifter::TiltDown() {
	tiltMotor.SetSpeed(LIFTER_DOWN_SPEED);

}


void Lifter::StopAll() {
	liftMotor.SetSpeed(0);
	tiltMotor.SetSpeed(0);
}
