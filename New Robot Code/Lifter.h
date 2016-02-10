/*
 * Lifter.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Alex
 */

#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include "Definitions.h"
#include "WPILib.h"



//
class Lifter {
public:
	// Our constructor classes
	Lifter();	// DO NOT CALL
	Lifter(unsigned int jaguarLift, unsigned int jaguarPinch, Encoder &eEncoder);

	// Classes to manually make the lifter go up or down
	void LiftUp();
	void LiftDown();
	void TiltUp();
	void TiltDown();
	void StopAll();

private:
	Jaguar liftMotor, tiltMotor;
	Encoder *lifterEncoder;
};



#endif /* SRC_LIFTER_H_ */
