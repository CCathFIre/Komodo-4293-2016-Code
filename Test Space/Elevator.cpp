#include "Elevator.h"
#include "WPILib.h"
#include "Definitions.h"

Elevator::Elevator()
	:motor(0),
	 upEncoder(NULL),
	stopButton(3),
	eStop(8)
{
	loopCount = 0;
	target = 0;
//DO NOT USE
}


Elevator::Elevator(unsigned int jaguar, Encoder &eEncoder, unsigned int button)
	:motor(jaguar),
	 stopButton(button),
	 upEncoder(&eEncoder),
	eStop(9)
{
	loopCount = 0;
	//motor.SetExpiration(0.1);

	motor.SetSafetyEnabled(true);

	upEncoder->Reset();
	target = upEncoder->GetRaw();

}

bool Elevator::UpTick()
{
	bool finished = false;
	if(loopCount == 0)
	{
	int temp = upEncoder->GetRaw();
	target = temp + ELEVATOR_TICK_DISTANCE;
	}
	loopCount ++;
	motor.Set(ELEVATOR_UP_SPEED);
	if(upEncoder->GetRaw() > target)
	{
		finished = true;
		motor.Set(0);
		loopCount = 0;
	}
		SmartDashboard::PutNumber("Elevator Up Target", target);
		SmartDashboard::PutNumber("Elevator Encoder", upEncoder->GetRaw());
	return finished;
}

bool Elevator::DownTick()
{
	bool finished = false;
	if(loopCount == 0)
	{
int temp = upEncoder->GetRaw();
		target = temp - ELEVATOR_TICK_DISTANCE;
	}
	loopCount ++;
		motor.Set(ELEVATOR_DOWN_SPEED);
		if(upEncoder->GetRaw() < target)
		{
			finished = true;
			motor.Set(0);
			loopCount = 0;

		}
			SmartDashboard::PutNumber("Elevator Down Target", target);
			SmartDashboard::PutNumber("Elevator Encoder", upEncoder->GetRaw());
	return finished;
}

//Temp functions for basic testing

void Elevator::GoUp()
{
	motor.Set(ELEVATOR_UP_SPEED);
}

void Elevator::GoDown()
{
	motor.Set(ELEVATOR_DOWN_SPEED);
}

float Elevator::Stop(float speed)
{

	motor.Set(speed);

	/*
	if(abs(upEncoder->GetRate()) > ELEVATOR_ENCODER_THRESHOLD)
	{

		if(upEncoder->GetRate() > 0)
		{
			speed = speed - 0.005;
		}
		else
		{
			speed = speed + 0.005;
		}

	}
*/
	return 0;
}

bool Elevator::EStopButton()
{
	if(!eStop.Get())
	{
		motor.Set(0);
		Wait(.25);
	}
	return !eStop.Get();
}
