#include "Robot.cpp"
#include "Definitions.h"
#include "WPILib.h"

void Robot::AutoPlaceBinOnly()
{
	static int autonomousStep = INIT;
	//elevator.UpTick(); //Picks up first tote
	switch(autonomousStep)
	{
		case INIT:
		encoder1.Reset();
		encoder2.Reset();
		encoder3.Reset();
		autonomousStep++;
		break;

		case FORWARD:
		if(encoder1.GetRaw() < 4522 && encoder2.GetRaw() < 4699) //While there is distance still to travel
		{
			DriveControl(0, 7, -0.4, 0); //Move forward at third speed
			elevator.Stop(0);
			Wait(0.2);
			SmartDashboard::PutNumber("Encoder L get raw (auto)", encoder1.GetRaw());
			SmartDashboard::PutNumber("Encoder R get raw (auto)", encoder2.GetRaw());
			SmartDashboard::PutNumber("Elevator encoder raw (auto)", encoder3.GetRaw());
		}
		else
		{
			autonomousStep++;
			DriveControl(0, 7, 0, 0); //Stop the robot
		encoder1.Reset();
		encoder2.Reset();
		encoder3.Reset();

		SmartDashboard::PutNumber("Encoder L get raw (auto)", encoder1.GetRaw());
		SmartDashboard::PutNumber("Encoder R get raw (auto)", encoder2.GetRaw());
		SmartDashboard::PutNumber("Elevator encoder raw (auto)", encoder3.GetRaw());
		SmartDashboard::PutNumber("auto step", autonomousStep);
		}
		break;

		case UP:
		if(!elevator.UpTick())
		{

		}
		else
		{
			encoder1.Reset();
			encoder2.Reset();
			encoder3.Reset();
			autonomousStep++;

		}
		break;

		case REVERSE:
		if(encoder1.GetRaw() > (-8 * ONE_FOOT_LEFT_ENOCDER) && encoder2.GetRaw() < (-8 * ONE_FOOT_RIGHT_ENCODER))
		{
			DriveControl(0, 7, 0.6, 0); //Move Backwards at third speed
			elevator.Stop(-ELEVATOR_STALL_SPEED);
			//gyro.Reset();
		}
		else
		{
			SmartDashboard::PutNumber("Encoder L get raw (auto)", encoder1.GetRaw());
			SmartDashboard::PutNumber("Encoder R get raw (auto)", encoder2.GetRaw());
			SmartDashboard::PutNumber("Elevator encoder raw (auto)", encoder3.GetRaw());
			SmartDashboard::PutNumber( "Stop", 0);
			autonomousStep++;
			DriveControl(0, 7, 0, 0); //Stop the robot
			elevator.Stop(-ELEVATOR_STALL_SPEED);
			encoder1.Reset();
			encoder2.Reset();
			//gyro.Reset();
			}
		break;

		default:
		break;

	}
}


void Robot::DriveControl(Joystick *inputStick , int override, float autoStraight, float autoTurn )
{
	if( ! inputStick)
	{
		inputStick = &driveStationInput;
	}

	driveOption = static_cast<int> (inputStick->GetRawAxis(DIAL)) + static_cast<int> (inputStick->GetRawAxis(SWITCH)*(DIAL_MAX_VALUE + 1));

	if(override != 0)
	{
		driveOption = override;
	}

	if(IsTurning(autoTurn))
	{
		//gyro.Reset();
		switch(driveOption)
			{
				//1 joystick
		case ARCADE_1:
			myRobot.ArcadeDrive(driveSpeed*rStick.GetRawAxis(Y_AXIS),-driveSpeed*rStick.GetRawAxis(X_AXIS));
			break;

				//2 joysticks
		case ARCADE_2:
			myRobot.ArcadeDrive(-driveSpeed*lStick.GetRawAxis(Y_AXIS),-driveSpeed*rStick.GetRawAxis(X_AXIS));
			break;

				//1 gamepad joystick arcade controls
		case ARCADE_GAMEPAD_1:
			myRobot.ArcadeDrive(gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y),-gPad.GetRawAxis(GAMEPAD_LEFT_STICK_X));
			break;

				//2 gamepad joystick arcade split controls
		case ARCADE_GAMEPAD_2:
			myRobot.ArcadeDrive(-gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y),-gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_X));
			break;

				//2 joystick gamepad tank drive
		case TANK_GAMEPAD:
			myRobot.TankDrive(-gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y));
			break;

				//2 joystick tank drive
		case TANK_2:
			myRobot.TankDrive(-lStick.GetRawAxis(Y_AXIS), -rStick.GetRawAxis(Y_AXIS));
			break;

		//case AUTO:
			//myRobot.ArcadeDrive(autoStraight, autoTurn);
			//break;
			//autonomous mode

	}
		//gyro.Reset();
	}
	else
	{
		//GyroDrive(autoStraight);
	}
}

bool Robot::IsTurning(float autoTurn)
{
	bool turning;
	switch(driveOption)
	{
		//1 joystick
		case ARCADE_1:
		if(fabs(rStick.GetRawAxis(X_AXIS)) > RSTICK_DEAD_ZONE)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;

		//2 joysticks
		case ARCADE_2:
		if(fabs(rStick.GetRawAxis(X_AXIS)) > RSTICK_DEAD_ZONE)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;

		//1 joystick
		case ARCADE_GAMEPAD_1:
		if(fabs(gPad.GetRawAxis(GAMEPAD_LEFT_STICK_X)) > GPAD_DEAD_ZONE)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;
		//2 joysticks
		case ARCADE_GAMEPAD_2:
		if(fabs(gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_X)) > GPAD_DEAD_ZONE)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;

	case TANK_GAMEPAD:
		if(fabs(gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y) - gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y)) > GPAD_TURN_THRESHOLD)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;

	case TANK_2:
		if(fabs(lStick.GetRawAxis(Y_AXIS) - rStick.GetRawAxis(Y_AXIS)) > JOYSTICK_TURN_THRESHOLD)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;
/*
	case AUTO:
		if(fabs(autoTurn) > 0.01)
		{
			turning = true;
		}
		else
		{
			turning = false;
		}
		break;

*/
}
	return turning;
}

/*
void Robot::GyroDrive(float autoStraight)
{
	switch(driveOption)
	{
		case ARCADE_1:
			myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), gyro.GetAngle()*gyroScaleFactor);
			break;

				//2 joysticks
		case ARCADE_2:
			myRobot.ArcadeDrive(-driveSpeed*lStick.GetRawAxis(Y_AXIS), gyro.GetAngle()*gyroScaleFactor);
			break;

				//1 gamepad joystick arcade controls
		case ARCADE_GAMEPAD_1:
			myRobot.ArcadeDrive(gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), gyro.GetAngle()*gyroScaleFactor);
			break;

				//2 gamepad joystick arcade split controls
		case ARCADE_GAMEPAD_2:
			myRobot.ArcadeDrive(-gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), gyro.GetAngle()*gyroScaleFactor);
			break;

				//2 joystick gamepad tank drive
		case TANK_GAMEPAD:
			myRobot.ArcadeDrive((gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y) +gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y))/2, gyro.GetAngle()*gyroScaleFactor);
			break;

				//2 joystick tank drive
		case TANK_2:
			myRobot.ArcadeDrive((lStick.GetRawAxis(Y_AXIS) +rStick.GetRawAxis(Y_AXIS))/2, gyro.GetAngle()*gyroScaleFactor);
			break;

		case AUTO:
			myRobot.ArcadeDrive(autoStraight, gyro.GetAngle()*gyroScaleFactor);
			break;

	}
}

*/

void Robot::ButtonControl()


{

	if((upTickRunning || gPad.GetRawButton(UPTICK_BUTTON)) && !buttonDone[UPTICK_BUTTON] && !downTickRunning && !elevator.EStopButton())
				{
			upTickRunning = true;
			if(elevator.UpTick())
			{
		   buttonDone[UPTICK_BUTTON] = true;
		   buttonDone[RJOYSTICK_UPTOTE] = true;
		  upTickRunning = false;
			}
				}

		if((downTickRunning || gPad.GetRawButton(DOWNTICK_BUTTON)) && !buttonDone[DOWNTICK_BUTTON] && !upTickRunning && !elevator.EStopButton())
				{
			downTickRunning = true;
			if(elevator.DownTick())
			{
			buttonDone[DOWNTICK_BUTTON] = true;
			buttonDone[RJOYSTICK_DOWNTOTE] = true;
			downTickRunning = false;
			}
				}


	if(gPad.GetRawButton(ELEVATOR_GO_DOWN) && !elevator.EStopButton())// && !buttonDone[ELEVATOR_GO_DOWN])
	{
		elevator.GoDown();

		buttonDone[ELEVATOR_GO_DOWN] = true;
	}

	if(gPad.GetRawButton(ELEVATOR_GO_UP)  && !elevator.EStopButton())// && !buttonDone[ELEVATOR_GO_UP])
			{
		elevator.GoUp();

		buttonDone[ELEVATOR_GO_UP] = true;
			}
	if(!gPad.GetRawButton(ELEVATOR_GO_DOWN) && !gPad.GetRawButton(ELEVATOR_GO_UP) && !downTickRunning && !upTickRunning)
	{
		float temp = elevator.Stop(0);
	}
	//If we aren't telling it to go, stop (temp function)
	/*
	if(((!upTickRunning && !downTickRunning && !fullDownRunning && elevator.EStopButton()) && //if we are not doing anything and
			((!gPad.GetRawButton(ELEVATOR_GO_UP) && !gPad.GetRawButton(ELEVATOR_GO_DOWN)) || //we are not doing anything on the gamepad and
			(!rStick.GetRawButton(RJOYSTICK_UPTOTE) && !rStick.GetRawButton(RJOYSTICK_DOWNTOTE))))) //we are not doing pressing the up or down button on the joystick
*/
/*
	if(!upTickRunning && !downTickRunning && !fullDownRunning && elevator.EStopButton() && !gPad.GetRawButton(ELEVATOR_GO_UP) && !gPad.GetRawButton(ELEVATOR_GO_DOWN))

	{
		stallSpeed = -1*elevator.Stop(-stallSpeed);
	}
*/
	if(gPad.GetRawButton(GAMEPAD_LEFT_BUMPER) && !buttonDone[GAMEPAD_LEFT_BUMPER])
			{
		encoder1.Reset();
		encoder2.Reset();
		encoder3.Reset();

		buttonDone[GAMEPAD_LEFT_BUMPER] = true;
			}

	if(lStick.GetRawButton(LJOYSTICK_SPEED_UP) && !lJoystickDone[0])
	{
		if(driveSpeed < 1)
		{
			driveSpeed = driveSpeed + 0.01;

			lJoystickDone[0] = true;
		}

	}
	if(lStick.GetRawButton(LJOYSTICK_SPEED_DOWN) && !lJoystickDone[0])
	{
		if(driveSpeed > 0)
		{
			driveSpeed = driveSpeed - 0.01;

			lJoystickDone[1] = true;
		}
	}
	if(lStick.GetRawButton(LJOYSTICK_ENCODER_RESET) && !lJoystickDone[0])
	{
			encoder1.Reset();
			encoder2.Reset();

			lJoystickDone[2] = true;
	}
	if(rStick.GetRawButton(RJOYSTICK_DOWNTOTE) && !rJoystickDone[0])
	{
			elevator.DownTick();

			rJoystickDone[0] = true;
	}
	if(rStick.GetRawButton(RJOYSTICK_UPTOTE) && !rJoystickDone[0])
	{
			elevator.UpTick();

			rJoystickDone[1] = true;
	}
	if(rStick.GetRawButton(RJOYSTICK_ELEVATOR_UP) && !rJoystickDone[0])
	{
			elevator.GoUp();

			rJoystickDone[2] = true;
	}
	if(rStick.GetRawButton(RJOYSTICK_ELEVATOR_DOWN) && !rJoystickDone[0])
	{
			elevator.GoDown();

			rJoystickDone[3] = true;
	}



	/*

	if((fullDownRunning || gPad.GetRawButton(UPTICK_BUTTON)) && !buttonDone[UPTICK_BUTTON] && !upTickRunning && !downTickRunning)
			{
		fullDownRunning = true;
		if(elevator.FullDown())
		{
		buttonDone[UPTICK_BUTTON] = true;
		fullDownRunning = false;
		}
			}
	*/


	for(int c = 0; c < 8; c++)
	{
		if(!gPad.GetRawButton(c))
		{
			buttonDone[c] = false;
		}
	}

	for(int c = 0; c < 3; c++)
	{
		if(!lStick.GetRawButton(c))
		{
			lJoystickDone[c] = false;
		}
	}

	for(int c = 0; c < 4; c++)
		{
			if(!rStick.GetRawButton(c))
			{
				rJoystickDone[c] = false;
			}
		}
}


START_ROBOT_CLASS(Robot);
