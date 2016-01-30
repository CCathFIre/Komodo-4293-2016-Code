#include "BallManipulator.h"
#include "WPILib.h"
#include "Definitions.h"



// DO NOT CALL
BallManipulator::BallManipulator()
	: motor(0)
{

}

BallManipulator::BallManipulator(unsigned int jaguar, Encoder &eEncoder)
	: motor(jaguar),
	  ballManipulatorEncoder(&eEncoder)
{
	ballManipulatorEncoder->Reset();
}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Sets the lifter to go up
void BallManipulator::GoUp() {
	motor.SetSpeed(B_MANIPULATOR_UP_SPEED);
}

// Sets the lifter to go down
void BallManipulator::GoDown() {
	motor.SetSpeed(B_MANIPULATOR_DOWN_SPEED);
}
