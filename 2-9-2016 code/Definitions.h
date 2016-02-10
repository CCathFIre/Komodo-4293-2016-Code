/*
 * Definitions.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Alex
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_


//////////////////////////////////////////////


// To know what int values are assigned to the left and right joysticks channels
#define RIGHT_JOYSTICK_INPUT_CHANNEL	0
#define LEFT_JOYSTICK_INPUT_CHANNEL		1


// The int values for the joystick axis
#define X_AXIS		0
#define Y_AXIS		1


// Int values for motors
#define RIGHT_MOTOR		0
#define LEFT_MOTOR		1


// Int values for left/right stick axis
#define LEFT_STICK_X	0
#define LEFT_STICK_Y	1

#define RIGHT_STICK_X   0
#define RIGHT_STICK_Y   1

// For the gamepad
#define GAMEPAD_INPUT_CHANNEL	2


// Gamepad button stuff
#define GAMEPAD_LEFT_STICK_X		0
#define GAMEPAD_LEFT_STICK_Y		1

#define GAMEPAD_RIGHT_STICK_X		4
#define GAMEPAD_RIGHT_STICK_Y		5

#define GAMEPAD_LEFT_BUMPER			5
#define GAMEPAD_RIGHT_BUMPER		6

#define GAMEPAD_BUTTON_Y			4
#define GAMEPAD_BUTTON_B			2
#define GAMEPAD_BUTTON_A			1
#define GAMEPAD_BUTTON_X			3


// To account for joystick/gamepad drift
#define JOYSTICK_STANDARD_DRIFT		0.1
#define GAMEPAD_STANDARD_DRIFT		0.05

#define TANK_TURN_THRESHOLD			0.09



// Drive modes
#define ARCADE_1			1
#define ARCADE_2			2

#define ARCADE_GAMEPAD_1	3
#define ARCADE_GAMEPAD_2	ARCADE_GAMEPAD_1 + 1

#define TANK_GAMEPAD		ARCADE_GAMEPAD_2 + 1
#define TANK_2				TANK_GAMEPAD + 1



// Values for the encoders
#define ENCODER_CHANNEL_1A		1
#define ENCODER_CHANNEL_1B		2

#define ENCODER_CHANNEL_2A		4
#define ENCODER_CHANNEL_2B		5

#define ONE_FOOT_LEFT_ENCODER	2505.25
#define ONE_FOOT_RIGHT_ENCODER	2484.71

#define LIFTER_ENCODER_CHANNEL_1			0
#define LIFTER_ENCODER_CHANNEL_2			1
#define B_MANIPULATOR_ENCODER_CHANNEL_1		2
#define B_MANIPULATOR_ENCODER_CHANNEL_2		3



// Gyro stuff
#define GYRO_INPUT_CHANNEL			0
#define GYRO_DRIFT_VALUE_MIN	   -0.5
#define GYRO_DRIFT_VALUE_MAX		0.5
#define GYRO_DRIFT_VALUE_AVERAGE	0
#define GYRO_SCALE_FACTOR 			0.02

#define GYRO_DRIFT_VALUE			0.1



// Stuff for the manipulators below
#define LIFTER_CHANNEL_LIFT			0
#define LIFTER_CHANNEL_TILT			0

// Lifter speeds
#define LIFTER_UP_SPEED				1
#define LIFTER_DOWN_SPEED		   -1



#define B_MANIPULATOR_CHANNEL_LIFT		0
#define B_MANIPULATOR_CHANNEL_PINCH		4

// Ball Manipulator speeds
#define B_MANIPULATOR_UP_SPEED			1
#define B_MANIPULATOR_DOWN_SPEED	   -1



//////////////////////////////////////////////


#endif /* SRC_DEFINITIONS_H_ */
