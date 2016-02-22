/*
 * BallManipulator.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Alex
 */

#ifndef SRC_BALLMANIPULATOR_H_
#define SRC_BALLMANIPULATOR_H_

#include "Definitions.h"
#include "WPILib.h"



//
class BallManipulator {
public:
	// Our constructor class
	BallManipulator();	// DO NOT CALL
	BallManipulator(unsigned int lift, unsigned int pinch, Encoder &eEncoder);

	// Classes to manually make the lifter go up or down
	void GoUp(float speed);
	void GoDown();
	void TestServoF();
	void TestServoR();
	void Open();
	void Close();
	void StopAll();
	float GetAngle();

	float currentAngle, incrementAngle;
	float pincherRange, pincherMinAngle, pincherMaxAngle;

	// A function to test the servo motor

	void testMotorPanToMax();
	void testMotorPanToMin();


private:
	VictorSP liftMotor;
	Servo pinchServo;
	Encoder *ballManipulatorEncoder;
};



#endif /* SRC_BALLMANIPULATOR_H_ */
