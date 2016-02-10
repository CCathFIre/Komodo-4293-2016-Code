#include "BallManipulator.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
BallManipulator::BallManipulator()
	: liftMotor(0),
	  pinchMotor(0)
{

}

BallManipulator::BallManipulator(unsigned int jaguarLift, unsigned int jaguarPinch, Encoder &eEncoder)
	: liftMotor(jaguarLift),
	  pinchMotor(jaguarPinch),
	  ballManipulatorEncoder(&eEncoder)
{
	ballManipulatorEncoder->Reset();
}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void BallManipulator::GoUp() {
	pinchMotor.SetSpeed(B_MANIPULATOR_UP_SPEED);
}

// Sets the lifter to go down
void BallManipulator::GoDown() {
	liftMotor.SetSpeed(B_MANIPULATOR_DOWN_SPEED);
}


void BallManipulator::Open() {
	liftMotor.SetSpeed(B_MANIPULATOR_UP_SPEED);
}

void BallManipulator::Close() {
	pinchMotor.SetSpeed(B_MANIPULATOR_DOWN_SPEED);
}


void BallManipulator::StopAll() {
	liftMotor.SetSpeed(0);
	pinchMotor.SetSpeed(0);
}
