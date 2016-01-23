#include "WPILib.h"
#include "Definitions.h"

// My main robot class
class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;
	int autoLoopCounter;

	// The joysticks
	Joystick rStick;
	Joystick lStick;

	// To know what data will be put on the dashboard
	bool showEncoderRaw;
	bool showEncoderRate;
	bool showEncoderIndex;

	// Gyro stuff
	AnalogGyro gyro;
	bool showGyro;

	// Encoders
	Encoder encoder1;
	Encoder encoder2;

public:
	// The initializer of dooooooom!!!!!
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		rStick(RIGHT_JOYSTICK_INPUT_CHANNEL),		// as they are declared above.
		lStick(LEFT_JOYSTICK_INPUT_CHANNEL),
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		gyro(GYRO_INPUT_CHANNEL),
		encoder1(ENCODER_CHANNEL_1A, ENCODER_CHANNEL_1B),
		encoder2(ENCODER_CHANNEL_2A, ENCODER_CHANNEL_2B)
	{
		myRobot.SetExpiration(0.1);

		showGyro = true;

		showEncoderRaw = true;
		showEncoderRate = true;
		showEncoderIndex = false;
	}

private:
	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestPeriodic();
};

////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////

// Starts at the beginning of the autonomous period
void Robot::AutonomousInit() {
	autoLoopCounter = 0;

	encoder1.Reset();
	encoder2.Reset();
}

// When the robot is on autonomous period, it will drive forwards at half speed for about two
// seconds, then stops
void Robot::AutonomousPeriodic() {
	/*
	if(autoLoopCounter < 100) { //Check if we've completed 100 loops (approximately 2 seconds)
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		autoLoopCounter++;
	} else {
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}
	*/

	// Correct course (adjust for physical motor imperfections) based on encoders




	SmartDashboard::PutString("Operation Type:", "Auto");

	// Print the raw encoder data
	SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
	SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());
}

//////

// Starts at the beginning of the teleop (user controlled) period
void Robot::TeleopInit() {

}

// During every loop intervel of the teleop period
void Robot::TeleopPeriodic() {
	// Tank drive, both left and right joystick control their respective motor along the
	// joystick's 'y' axis
	//myRobot.TankDrive(-lStick.GetRawAxis(LEFT_STICK_Y), -rStick.GetRawAxis(RIGHT_STICK_Y));
	myRobot.TankDrive(-rStick.GetRawAxis(RIGHT_STICK_Y), -lStick.GetRawAxis(LEFT_STICK_Y));



	SmartDashboard::PutString("Operation Type:", "TeleOp");

	// Print gyro data
	if (showGyro == true) {
		SmartDashboard::PutNumber("Gyro Angle", gyro.GetAngle());
		SmartDashboard::PutNumber("Gyro Rate", gyro.GetRate());
	}


	// Print out the encoder data
	// Raw encoder data
	if (showEncoderRaw == true) {
		SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
		SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());

	}

	// The delta encoder change
	if (showEncoderRate == true) {
		SmartDashboard::PutNumber("Encoder L get rate", encoder1.GetRate());
		SmartDashboard::PutNumber("Encoder R (reversed) get rate", encoder2.GetRate());
	}

	// Print the encoder index
	if (showEncoderIndex == true) {
		SmartDashboard::PutNumber("Encoder L index", encoder1.GetFPGAIndex());
		SmartDashboard::PutNumber("Encoder R index", encoder2.GetFPGAIndex());
	}
}

//////

// During every loop intervel of the test period
void Robot::TestPeriodic() {
	lw->Run();
}

////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////

// Implements the 'Robot' class into the main robot loop
START_ROBOT_CLASS(Robot)
