#include "BallManipulator.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
BallManipulator::BallManipulator()
	: liftMotor(0),
	  pinchMotor(0),
	  testMotor(0)
{

}

BallManipulator::BallManipulator(unsigned int jaguarLift, unsigned int jaguarPinch, Encoder &eEncoder)
	: liftMotor(jaguarLift),
	  pinchMotor(jaguarPinch),
	  testMotor(9),
	  ballManipulatorEncoder(&eEncoder)
{
	ballManipulatorEncoder->Reset();

	testMotorMaxAngle = testMotor.GetMaxAngle();
	testMotorMinAngle = testMotor.GetMinAngle();
	testMotorRange = testMotorMaxAngle - testMotorMinAngle;
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

void BallManipulator::testMotorPanToMax() {
	// Go go through the range of angles, starting from the min to max
	//for (float angle = testMotorMinAngle; angle<testMotorMaxAngle; angle+=testMotorRange/10.0) {
	//	// Set to the angle
	//	testMotor.SetAngle(angle);
		// Wait 1 second
	//	Wait(1.0);
	//}

	testMotor.SetAngle(testMotorMaxAngle);
}

void BallManipulator::testMotorPanToMin() {
	// Go go through the range of angles, starting from the max to min
	//for (float angle = testMotorMaxAngle; angle>testMotorMinAngle; angle-=testMotorRange/10.0) {
		// Set to the angle
	//	testMotor.SetAngle(angle);
		// Wait 1 second
	//	Wait(1.0);
	//}

	testMotor.SetAngle(testMotorMinAngle);
}
