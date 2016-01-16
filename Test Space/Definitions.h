#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

//Control device input channels
#define RIGHT_JOYSTICK_INPUT_CHANNEL		0
#define LEFT_JOYSTICK_INPUT_CHANNEL			1
#define GAMEPAD_INPUT_CHANNEL				2
#define DRIVE_STATION_INPUT_CHANNEL			3

//Joystick Axes
#define X_AXIS				0 //All experimentally confirmed
#define Y_AXIS				1

//Drive Station Inputs
#define DIAL				1
#define DIAL_MAX_VALUE		3
#define SWITCH				2 //All temp placeholder values

//Drive Modes
#define ARCADE_1			1
#define ARCADE_2			2
#define ARCADE_GAMEPAD_1	3
#define ARCADE_GAMEPAD_2	ARCADE_GAMEPAD_1 + 1
#define TANK_GAMEPAD		ARCADE_GAMEPAD_2 + 1
#define TANK_2				TANK_GAMEPAD + 1
//#define AUTO				0

//Comment out GAMEPAD_DOLPHIN to define GAMEPAD_FOX
//There can be only one!
#define GAMEPAD_DOLPHIN
#ifndef GAMEPAD_DOLPHIN
#define GAMEPAD_FOX
#endif

//Gamepad input channels
#ifdef GAMEPAD_DOLPHIN
#define GAMEPAD_LEFT_STICK_Y			1 //All experimentally confirmed
#define GAMEPAD_LEFT_STICK_X			0
#define GAMEPAD_RIGHT_STICK_Y			5
#define GAMEPAD_RIGHT_STICK_X			4
#define GAMEPAD_LEFT_BUMPER				5
#define GAMEPAD_RIGHT_BUMPER			6
#define GAMEPAD_BUTTON_X				3
#define GAMEPAD_BUTTON_Y				4
#define GAMEPAD_BUTTON_A				1
#define GAMEPAD_BUTTON_B				2

#define RESET_ENCODER					GAMEPAD_LEFT_BUMPER
#define ELEVATOR_GO_UP					GAMEPAD_BUTTON_B
#define ELEVATOR_GO_DOWN				GAMEPAD_BUTTON_A
#define UPTICK_BUTTON					GAMEPAD_BUTTON_Y
#define DOWNTICK_BUTTON					GAMEPAD_BUTTON_X

#endif

#ifdef GAMEPAD_FOX
#define GAMEPAD_LEFT_STICK_Y			1
#define GAMEPAD_LEFT_STICK_X			0
#define GAMEPAD_RIGHT_STICK_Y			3
#define GAMEPAD_RIGHT_STICK_X			2
#define GAMEPAD_LEFT_BUMPER				7 //5 for top bumper
#define GAMEPAD_RIGHT_BUMPER			8 //6 for top bumper
#define GAMEPAD_BUTTON_X				1
#define GAMEPAD_BUTTON_Y				4
#define GAMEPAD_BUTTON_A				2
#define GAMEPAD_BUTTON_B				3
#endif

//Left Joystick
#define LJOYSTICK_SPEED_UP				10
#define LJOYSTICK_SPEED_DOWN			12
#define LJOYSTICK_ENCODER_RESET			11

//Right Joystick
#define RJOYSTICK_DOWNTOTE				12
#define RJOYSTICK_UPTOTE				10
#define RJOYSTICK_ELEVATOR_UP			9
#define RJOYSTICK_ELEVATOR_DOWN			11

//Drive Control
#define LEFT_DRIVE_CONTROL				0
#define RIGHT_DRIVE_CONTROL				1
#define ROBOT_START_SPEED				.85
//Gyroscope
#define GYRO_INPUT_CHANNEL				0
#define GYRO_SCALE_FACTOR				0.04 //NEEDS VERIFICATION!!!! WAS .085
#define GYRO_DEAD_ZONE					0.2  //NEEDS VERIFICATION!!!!
#define RSTICK_DEAD_ZONE				0.1
#define GPAD_DEAD_ZONE					0.05
#define JOYSTICK_TURN_THRESHOLD			0.09
#define GPAD_TURN_THRESHOLD				0.09

//encoder
#define ENCODER_CHANNEL_1A				1
#define ENCODER_CHANNEL_1B				2
#define ENCODER_CHANNEL_INDEX1			1
#define ENCODER_CHANNEL_2A				4	// CHANGE THESE!!
#define ENCODER_CHANNEL_2B				5	// CHANGE THESE!!
#define ENCODER_CHANNEL_INDEX2			4   // CHANGE THESE!!



//elevator
#define ELEVATOR_CHANNEL				2
#define ELEVATOR_SWITCH_CHANNEL			3	// CHANGE THESE!!
#define BUTTON_OPEN_VALUE				500
#define BUTTON_CLOSED_VALUE				2000
#define ELEVATOR_ENCODER_CHANNEL_1		7 //Should be 7 and 8
#define ELEVATOR_ENCODER_CHANNEL_2		8
#define ELEVATOR_ENCODER_INDEX			0  //CHANGE THESE!!
#define ELEVATOR_ENCODER_THRESHOLD		50 //CHANGE THESE!!
#define ELEVATOR_UP_SPEED				-1 // CHANGE THESE!!
#define ELEVATOR_DOWN_SPEED				1// CHANGE THESE!!
#define ELEVATOR_STALL_SPEED			0.09
#define ELEVATOR_TICK_DISTANCE			17390

//auto code
#define AUTO_OPTION						1

#define INIT							0
#define FORWARD							(INIT + 1)
#define UP								(FORWARD + 1)
#define REVERSE							(UP + 1)
#define REVERSE2						(REVERSE + 1)

#define ONE_FOOT_LEFT_ENOCDER			4522
#define ONE_FOOT_RIGHT_ENCODER			-4699


//ultrasonic sensor
#define ULTRASONIC_CHANNEL				1


#endif
