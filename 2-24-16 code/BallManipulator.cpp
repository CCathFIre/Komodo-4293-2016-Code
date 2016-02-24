#include "BallManipulator.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
BallManipulator::BallManipulator()
	: liftMotor(0),
	  leftPincher(0),
	  rightPincher(0)
{

}

BallManipulator::BallManipulator(unsigned int lift, unsigned int pinchL, unsigned int pinchR, Encoder &eEncoder)
	: liftMotor(lift),
	  leftPincher(pinchL),
	  rightPincher(pinchR),
	  ballManipulatorEncoder(&eEncoder)
{
	ballManipulatorEncoder->Reset();

}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void BallManipulator::Tilt(float speed) {
	liftMotor.SetSpeed(speed);
}


void BallManipulator::Open() {
	leftPincher.Set(false);
	rightPincher.Set(false); //Open the pinchers by retracting both solenoids
}

void BallManipulator::Close() {
	leftPincher.Set(true);
	rightPincher.Set(true); //Close the pinchers by extending both solenoids
}


void BallManipulator::StopAll() {
	liftMotor.SetSpeed(0);
}




