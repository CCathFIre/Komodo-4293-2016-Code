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

	// Gamepad
	Joystick gamePad;

	// For switching driving controls
	int driveOption;
	double joystickDifference;
	double joystickAverage;

	// To know what data will be put on the dashboard
	bool showEncoderRaw;
	bool showEncoderRate;
	bool showEncoderIndex;

	// Gyro stuff
	AnalogGyro gyro;
	bool showGyro;
	double editedGyroRate;
	double editedGyroAngle;

	// Encoders
	Encoder encoder1;
	Encoder encoder2;

public:
	// The initializer of dooooooom!!!!!
	Robot() :
		myRobot(0, 1),	// these must be initialized in the same order
		rStick(RIGHT_JOYSTICK_INPUT_CHANNEL),		// as they are declared above.
		lStick(LEFT_JOYSTICK_INPUT_CHANNEL),
		gamePad(GAMEPAD_INPUT_CHANNEL),
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		gyro(GYRO_INPUT_CHANNEL),
		encoder1(ENCODER_CHANNEL_1A, ENCODER_CHANNEL_1B),
		encoder2(ENCODER_CHANNEL_2A, ENCODER_CHANNEL_2B)
	{
		myRobot.SetExpiration(0.1);

		driveOption = TANK_GAMEPAD;

		gyro.InitGyro();
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


	void DriverControl(int driveControl);
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


////////////////////////


// Starts at the beginning of the teleop (user controlled) period
void Robot::TeleopInit() {

}

// During every loop intervel of the teleop period
void Robot::TeleopPeriodic() {
	// Tank drive, both left and right joystick control their respective motor along the
	// joystick's 'y' axis
	//myRobot.TankDrive(-rStick.GetRawAxis(RIGHT_STICK_Y), -lStick.GetRawAxis(LEFT_STICK_Y));

	// Choose the teleop drive option
	DriverControl(driveOption);


	// Edits the gyro data to account for drift
	/*
	editedGyroRate = gyro.GetRate();
	if (gyro.GetRate() > GYRO_DRIFT_VALUE_MIN && gyro.GetRate() < GYRO_DRIFT_VALUE_MAX) {
		editedGyroRate = 0;
	} else {
		editedGyroRate += GYRO_DRIFT_VALUE_AVERAGE;
	}
	*/

	editedGyroRate = gyro.GetAngle();



	SmartDashboard::PutString("Operation Type:", "TeleOp");

	// Print gyro data
	if (showGyro == true) {
		SmartDashboard::PutNumber("Gyro Angle", gyro.GetAngle()*GYRO_SCALE_FACTOR);
		SmartDashboard::PutNumber("Gyro Rate (Raw)", gyro.GetRate()*GYRO_SCALE_FACTOR);
		SmartDashboard::PutNumber("Gyro Rate (Edited)", editedGyroRate*GYRO_SCALE_FACTOR);
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


////////////////////////


// During every loop intervel of the test period
void Robot::TestPeriodic() {
	lw->Run();
}


////////////////////////


// To determine which drive control type the robot to use
// In case user operator wants different controls
void Robot::DriverControl(int driveControl) {
	switch (driveControl) {
	// Arcade drive (think racing games) w/ 1 joysticks
	case ARCADE_1:
		// If the turn is outside the joystick's standard drift, robot will assume the robot is turning
		// If not, robot will assume it is pushed, or has naturally drifted
		if (fabs(rStick.GetRawAxis(X_AXIS)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), -rStick.GetRawAxis(X_AXIS));
			gyro.Reset();
		} else {
			// The turn will be the opposite of what the gyro says the angle of unintentional drift is,
			// which will have the robot go straight
			myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), editedGyroRate*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ 2 joysticks
	case ARCADE_2:
		if (fabs(rStick.GetRawAxis(X_AXIS)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), -rStick.GetRawAxis(X_AXIS));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), editedGyroRate*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ left stick on gamepad (the knockoff xbox controller)
	case ARCADE_GAMEPAD_1:
		if (fabs(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroRate*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ BOTH gamepad
	case ARCADE_GAMEPAD_2:
		if (fabs(gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroRate*GYRO_SCALE_FACTOR);
		}

		break;

	// Tank drive (requires 2 sticks; each stick controls its respective 'tread' or side;
	//			   Ex: moving the right stick moves only the wheels on the right) w/ gamepad
	case TANK_GAMEPAD:
		joystickDifference = gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y) - gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y);
		joystickAverage = (gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y) + gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y))/2;
		if (fabs(joystickDifference) > TANK_TURN_THRESHOLD) {
			myRobot.TankDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(joystickAverage, editedGyroRate*GYRO_SCALE_FACTOR);
		}

		break;

	// Tank drive w/ joysticks
	case TANK_2:
		joystickDifference = lStick.GetRawAxis(LEFT_STICK_Y) - rStick.GetRawAxis(RIGHT_STICK_Y);
		joystickAverage = (lStick.GetRawAxis(LEFT_STICK_Y) + rStick.GetRawAxis(RIGHT_STICK_Y))/2;
		if (fabs(joystickDifference) > TANK_TURN_THRESHOLD) {
			myRobot.TankDrive(-lStick.GetRawAxis(LEFT_STICK_Y), -rStick.GetRawAxis(RIGHT_STICK_Y));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(joystickAverage, -rStick.GetRawAxis(RIGHT_STICK_Y));
		}

		break;
	}
}


////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////


// Implements the 'Robot' class into the main robot loop
START_ROBOT_CLASS(Robot)
