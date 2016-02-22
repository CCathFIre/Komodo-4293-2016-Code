#include "BallManipulator.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
BallManipulator::BallManipulator()
	: liftMotor(0),
	  pinchServo(0)
{

}

BallManipulator::BallManipulator(unsigned int lift, unsigned int pinch, Encoder &eEncoder)
	: liftMotor(lift),
	  pinchServo(pinch),
	  ballManipulatorEncoder(&eEncoder)
{
	ballManipulatorEncoder->Reset();

	pincherMaxAngle = pinchServo.GetMaxAngle();
	pincherMinAngle = pinchServo.GetMinAngle();
	pincherRange = pincherMaxAngle - pincherMinAngle;
	incrementAngle = pincherRange/PINCHER_INTERVAL;
	currentAngle = pinchServo.GetAngle();
}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void BallManipulator::GoUp(float speed) {
	liftMotor.SetSpeed(speed);
}

// Sets the lifter to go down
void BallManipulator::GoDown() {
	liftMotor.SetSpeed(B_MANIPULATOR_DOWN_SPEED);
}

void BallManipulator::TestServoF() {
	pinchServo.Set(1);
			Wait(1);
	pinchServo.Set(0);
}

void BallManipulator::TestServoR() {
	pinchServo.Set(-1);
			Wait(1);
	pinchServo.Set(0);
}


void BallManipulator::Open() {
	/*
	currentAngle = currentAngle + incrementAngle;
	pinchServo.SetAngle(currentAngle);
	*/
	pinchServo.Set(1);
			Wait(1);
	pinchServo.Set(0);
}

void BallManipulator::Close() {
	currentAngle = currentAngle - incrementAngle;
	pinchServo.SetAngle(currentAngle);
}


void BallManipulator::StopAll() {
	liftMotor.SetSpeed(0);
	//pinchServo.SetSpeed(0);
}

float BallManipulator::GetAngle() {
	return currentAngle;
}


//Testing functions for ball manipulator motors

void BallManipulator::testMotorPanToMax() {
	// Go go through the range of angles, starting from the min to max
	//for (float angle = testMotorMinAngle; angle<testMotorMaxAngle; angle+=testMotorRange/10.0) {
	//	// Set to the angle
	//	testMotor.SetAngle(angle);
		// Wait 1 second
	//	Wait(1.0);
	//}

	pinchServo.SetAngle(pincherMaxAngle);
}

void BallManipulator::testMotorPanToMin() {
	// Go go through the range of angles, starting from the max to min
	//for (float angle = testMotorMaxAngle; angle>testMotorMinAngle; angle-=testMotorRange/10.0) {
		// Set to the angle
	//	testMotor.SetAngle(angle);
		// Wait 1 second
	//	Wait(1.0);
	//}

	pinchServo.SetAngle(pincherMinAngle);
}

