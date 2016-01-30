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
	BallManipulator(unsigned int jaguarLift, unsigned int jaguarPinch, Encoder &eEncoder);

	// Classes to manually make the lifter go up or down
	void GoUp();
	void GoDown();
	void Open();
	void Close();
	void StopAll();

private:
	Jaguar liftMotor, pinchMotor;
	Encoder *ballManipulatorEncoder;
};



#endif /* SRC_BALLMANIPULATOR_H_ */
