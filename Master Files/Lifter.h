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
	Lifter(unsigned int jaguar, Encoder &eEncoder);

	// Classes to manually make the lifter go up or down
	void GoUp();
	void GoDown();

private:
	Jaguar motor;
	Encoder *lifterEncoder;
};



#endif /* SRC_LIFTER_H_ */
