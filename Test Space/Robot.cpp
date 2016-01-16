#include "WPILib.h"
#include "Definitions.h"
#include "Elevator.h"


class Robot: public IterativeRobot
{

	int driveOption;  //Switch for different drive modes
	bool showGyro;
	bool showUltrasonic;
	bool showEncoderRaw;
	bool showEncoderRate;
	bool showEncoderIndex;
	float speedInput;
	float turnInput;
	float driveSpeed;
	bool buttonDone[8]; //Button pulsing array
	float gyroScaleFactor;
	bool showGyroScaleFactor;
	bool showElevatorSwitch;
	bool fullDownRunning;
	bool downTickRunning;
	bool upTickRunning;
	float stallSpeed;
	int temp;
	bool lJoystickDone[3];
	bool rJoystickDone[4];
	bool elevatorRunning[3];


	RobotDrive myRobot; // robot drive system
	Joystick rStick; //Right joystick
	Joystick lStick; //Left joystick
	Joystick gPad; //Gamepad
	Joystick driveStationInput; //Driver station switches and dials
	LiveWindow *lw;
	int autoLoopCounter;
	//Gyro gyro;
	Encoder encoder1;
	Encoder encoder2;
	Encoder encoder3;
	Elevator elevator;
	AnalogInput ultrasonic;
	AnalogInput elevatorSwitch;


public:
	Robot() :
		myRobot(LEFT_DRIVE_CONTROL, RIGHT_DRIVE_CONTROL),	// these must be initialized in the same order
		rStick(RIGHT_JOYSTICK_INPUT_CHANNEL),		// as they are declared above.
		lStick(LEFT_JOYSTICK_INPUT_CHANNEL),
		gPad(GAMEPAD_INPUT_CHANNEL),
		driveStationInput(DRIVE_STATION_INPUT_CHANNEL),
		lw(NULL),
		autoLoopCounter(0),
		//gyro(GYRO_INPUT_CHANNEL),
		encoder1(ENCODER_CHANNEL_1A,ENCODER_CHANNEL_1B),
		encoder2(ENCODER_CHANNEL_2A,ENCODER_CHANNEL_2B),
		encoder3(ELEVATOR_ENCODER_CHANNEL_1,ELEVATOR_ENCODER_CHANNEL_2),
		elevator(ELEVATOR_CHANNEL, encoder3, ELEVATOR_SWITCH_CHANNEL),
		ultrasonic(ULTRASONIC_CHANNEL),
		elevatorSwitch(ELEVATOR_SWITCH_CHANNEL)
	{
		myRobot.SetExpiration(0.1);
		speedInput = 0;
		turnInput = 0;
		driveOption = 0;
		gyroScaleFactor = 0;

		showGyro = false;
		showUltrasonic = false;
		showEncoderRaw = true;
		showEncoderRate = false;
		showEncoderIndex = false;
		showGyroScaleFactor = false;
		showElevatorSwitch = false;

		fullDownRunning = false;
		downTickRunning = false;
		upTickRunning = false;

		for(int c = 0; c < 8; c++)
				{
				buttonDone[c] = false;
				}
		for(int c = 0; c < 3; c++)
				{
				lJoystickDone[c] = false;
				}
		for(int c = 0; c < 4; c++)
				{
				rJoystickDone[c] = false;
				}

		//gyro.InitGyro();
		encoder1.Reset();
		encoder2.Reset();
		encoder3.Reset();
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
		encoder1.Reset();
		encoder2.Reset();
		encoder3.Reset();
	}

	void AutoBinAndTote();

  	void AutoPlaceBinOnly();

	void AutonomousPeriodic()
	{

		if(AUTO_OPTION == 1)
		{
			AutoPlaceBinOnly();
		}
		else if(AUTO_OPTION == 2)
		{
			AutoBinAndTote();
		}

	}

	void TeleopInit()
	{
		//gyro.Reset();
		gyroScaleFactor = GYRO_SCALE_FACTOR;
		stallSpeed = ELEVATOR_STALL_SPEED;
		driveSpeed = ROBOT_START_SPEED;
	}

	void TeleopPeriodic()
	{
		//DriveControl(0, ARCADE_2);
		//compressor.Start(); //De-comment if a compressor is used
		myRobot.ArcadeDrive(driveSpeed*lStick.GetRawAxis(Y_AXIS),-driveSpeed*rStick.GetRawAxis(X_AXIS));

		// Smart dashboard features
		if(driveOption == ARCADE_1)
		{
			SmartDashboard::PutNumber("Y axis", -rStick.GetRawAxis(Y_AXIS));
			SmartDashboard::PutNumber("X axis", -rStick.GetRawAxis(X_AXIS));
		}
		if(driveOption == ARCADE_2)
		{
			SmartDashboard::PutNumber("Left stick Y", -lStick.GetRawAxis(Y_AXIS)/2);
			SmartDashboard::PutNumber("Right stick X", -rStick.GetRawAxis(X_AXIS)/2);
		}
		if(driveOption == TANK_2)
		{
			SmartDashboard::PutNumber("Left stick Y", -lStick.GetRawAxis(Y_AXIS));
			SmartDashboard::PutNumber("Right stick Y", -rStick.GetRawAxis(Y_AXIS));
		}
		if(driveOption == ARCADE_GAMEPAD_1)
		{
			SmartDashboard::PutNumber("Gamepad Left Stick Y", -gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y));
			SmartDashboard::PutNumber("Gamepad Left Stick X", -gPad.GetRawAxis(GAMEPAD_LEFT_STICK_X));
		}
		if(driveOption == ARCADE_GAMEPAD_2)
		{
			SmartDashboard::PutNumber("Gamepad Left Stick Y", -gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y));
			SmartDashboard::PutNumber("Gamepad Right Stick X", -gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_X));
		}
		if(driveOption == TANK_GAMEPAD)
		{
			SmartDashboard::PutNumber("Gamepad Left Stick Y", -gPad.GetRawAxis(GAMEPAD_LEFT_STICK_Y));
			SmartDashboard::PutNumber("Gamepad Right Stick Y", -gPad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y));
		}
		if(showGyro == true)
		{
			//SmartDashboard::PutNumber("Gyro Angle", gyro.GetAngle());
			//SmartDashboard::PutNumber("Gyro Rate", gyro.GetRate());
		}

		if(showUltrasonic == true)
		{
			SmartDashboard::PutNumber("Ultrasonic", ultrasonic.GetValue());
		}
		if(showEncoderRaw == true)
		{
			SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
			SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());
			SmartDashboard::PutNumber("Elevator encoder raw", encoder3.GetRaw());

		}
		if(showEncoderRate == true)
		{
			SmartDashboard::PutNumber("Encoder L get rate", encoder1.GetRate());
			SmartDashboard::PutNumber("Encoder R (reversed) get rate", encoder2.GetRate());
			SmartDashboard::PutNumber("Elevator encoder rate", encoder3.GetRate());
		}
		if(showEncoderIndex == true)
		{
			SmartDashboard::PutNumber("Encoder L index", encoder1.GetFPGAIndex());
			SmartDashboard::PutNumber("Encoder R index", encoder2.GetFPGAIndex());
		}
		if(showGyroScaleFactor == true)
		{
			SmartDashboard::PutNumber("Gyro scale factor", gyroScaleFactor);
			//SmartDashboard::PutNumber("RStick Value", rStick.GetRawAxis(X_AXIS));
		}
		if(showElevatorSwitch == true)
		{
			SmartDashboard::PutNumber("Elevator Switch ", elevatorSwitch.GetAverageValue());
		}

		SmartDashboard::PutNumber("DriveSpeed", driveSpeed);

		ButtonControl();

		if (gPad.GetRawButton(1))
		{
			//gyro.Reset();
		}

	}

	void DriveControl(Joystick *inputStick = 0, int override = 0, float autoStraight = 0.0, float autoTurn = 0.0);

	bool IsTurning(float autoTurn = 0);

	void GyroDrive(float autoStraight = 0);

	void TestPeriodic()
	{
		lw->Run();
	}

	void ButtonControl();
};

