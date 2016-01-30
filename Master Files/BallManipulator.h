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
	BallManipulator();

	// Classes to manually make the lifter go up or down
	void GoUp();
	void GoDown();

private:
	Jaguar motor;
};



#endif /* SRC_BALLMANIPULATOR_H_ */
