#include "WPILib.h"
#include "define.h"

class Robot: public IterativeRobot
{
	float driveSpeed;
	bool yButton;
	bool aButton;





	RobotDrive myRobot; // robot drive system
	Joystick gPad; // only gPad
	LiveWindow *lw;
	int autoLoopCounter;

public:
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		gPad(GAMEPAD_CHANNEL),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void AutonomousInit()
	{
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		if(autoLoopCounter < 100) //Check if we've completed 100 loops (approximately 2 seconds)
		{
			myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
			autoLoopCounter++;
			} else {
			myRobot.Drive(0.0, 0.0); 	// stop robot
		}
	}

	void TeleopInit()
	{
		driveSpeed = ROBOT_START_SPEED; //DECLARING DRIVESPEED WITH THE ROBOT START SPEED
		yButton = false;
		aButton = false;

	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(-gPad.GetRawAxis(GP_RSTICK_YAXIS)*ROBOT_START_SPEED, -gPad.GetRawAxis(GP_RSTICK_XAXIS)*ROBOT_START_SPEED ); // drive with arcade style (use right stick)
		if(gPad.GetRawButton(GP_Y_BUTTON) == true && yButton == false)
		{
			if(driveSpeed + 0.25 < 1 && driveSpeed - 0.25 > 0)
			{
				yButton = true;
				driveSpeed = driveSpeed + 0.25;
			}
			else
			{
				yButton = false;
			}
		}

		if(gPad.GetRawButton(GP_A_BUTTON) == true && aButton == false)
		{
			if(driveSpeed + 0.25 < 1 && driveSpeed - 0.25 > 0)
			{
				aButton = true;
				driveSpeed = driveSpeed + 0.25;
			}
			else
			{
				aButton = false;
			}
		}

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
