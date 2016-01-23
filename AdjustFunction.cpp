/*
 * AdjustFunction.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: Kyle
 */
/*
#include "define.h"

void Adjust(float &target, int increaseSpeed = GP_Y_BUTTON, int decreaseSpeed = GP_A_BUTTON, int reset = GP_B_BUTTON)
{
	static bool aButton = false, yButton = false;

	//If Y is pressed
	if(gPad.GetRawButton(increaseSpeed) == true && yButton == false)
	{
		yButton = true;
		if(target + ROBOT_INCREASE_SPEED <= 1)
		{
			target = target + ROBOT_INCREASE_SPEED;
		}
	}
	//exit loop
	else if(gPad.GetRawButton(increaseSpeed) == false && yButton == true)
		{
			yButton = false;
		}


	//Otherwise if A is pressed
	else if(gPad.GetRawButton(decreaseSpeed) == true && aButton == false)
	{
		aButton = true;
		if(target - ROBOT_DECREASE_SPEED >= -1)
		{

			target = target - ROBOT_DECREASE_SPEED;
		}
	}
	//exit loop
		else if(gPad.GetRawButton(decreaseSpeed) == false && aButton == true)
		{
			aButton = false;
		}


	//reset to 0.0
	if(gPad.GetRawButton(reset)== true)
	{
		target = 0.0;
	}

	SmartDashboard::PutNumber("drive speed", target); //display to Smart Dashboard

}

//*/
