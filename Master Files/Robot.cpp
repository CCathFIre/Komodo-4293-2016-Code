#include "WPILib.h"
#include "Definitions.h"

#include "Lifter.h"
#include "BallManipulator.h"

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

	// For switching drive speed
	bool buttonDone[6]; //Array for gamepad buttons. Null-A-B-X-Y-BumperR-BumperL

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

	Encoder lifterEncoder;
	Encoder ballManipulatorEncoder;

	// A turn value for when the robot is in auto
	double autoTurn;

	// Manipulator classes
	Lifter lifter;
	BallManipulator ballManipulator;

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
		encoder2(ENCODER_CHANNEL_2A, ENCODER_CHANNEL_2B),
		lifterEncoder(LIFTER_ENCODER_CHANNEL_1, LIFTER_ENCODER_CHANNEL_2),
		ballManipulatorEncoder(B_MANIPULATOR_ENCODER_CHANNEL_1, B_MANIPULATOR_ENCODER_CHANNEL_2),
		lifter(LIFTER_CHANNEL_LIFT, LIFTER_CHANNEL_TILT, lifterEncoder),
		ballManipulator(B_MANIPULATOR_CHANNEL_LIFT, B_MANIPULATOR_CHANNEL_PINCH, ballManipulatorEncoder)
	{
		myRobot.SetExpiration(0.1);

		driveOption = TANK_GAMEPAD;

		gyro.InitGyro();
		showGyro = true;

		showEncoderRaw = true;
		showEncoderRate = false;
		showEncoderIndex = false;

		autoTurn = 0;
	}

private:
	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestPeriodic();


	void DriverControl(int driveControl);
	void IncrementVariable(int &var, float delta);
};



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Starts at the beginning of the autonomous period
void Robot::AutonomousInit() {
	autoLoopCounter = 0;

	encoder1.Reset();
	encoder2.Reset();
	lifterEncoder.Reset();
	ballManipulatorEncoder.Reset();
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



	// Edits the gyro angle to account for drift
	if (fabs(gyro.GetRate()) > GYRO_DRIFT_VALUE)
		editedGyroAngle = gyro.GetAngle();
	else {
		gyro.Reset();
		editedGyroAngle = 0;
	}



	// Adjust course based on gyro data
	if (editedGyroAngle > 0)
		autoTurn = 0.3;
	else if (editedGyroAngle < 0)
		autoTurn = -0.3;
	else
		autoTurn = 0;

	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 || encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		myRobot.Drive(0.3, autoTurn);
	else
		myRobot.Drive(0.0, 0.0);


	// Print the raw encoder data
	SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
	SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());
}


////////////////////////


// Starts at the beginning of the teleop (user controlled) period
void Robot::TeleopInit() {
	gyro.Reset();
}

// During every loop intervel of the teleop period
void Robot::TeleopPeriodic() {
	// Choose the teleop drive option
	DriverControl(driveOption);


	// A simple thing to test a motor
	if (gamePad.GetRawButton(GAMEPAD_BUTTON_A) == true)
		ballManipulator.GoUp();
	else
		ballManipulator.StopAll();



	// Edits the gyro rate to account for drift
	/*
	editedGyroRate = gyro.GetRate();
	if (gyro.GetRate() > GYRO_DRIFT_VALUE_MIN && gyro.GetRate() < GYRO_DRIFT_VALUE_MAX) {
		editedGyroRate = 0;
	} else {
		editedGyroRate += GYRO_DRIFT_VALUE_AVERAGE;
	}
	*/

	// Edits the gyro angle to account for drift
	if (fabs(gyro.GetRate()) > GYRO_DRIFT_VALUE)
		editedGyroAngle = gyro.GetAngle();
	else {
		gyro.Reset();
		editedGyroAngle = 0;
	}


	// Reset the buttons
	for(int c=0; c<7; c++)
		buttonDone[c] = false;



	// To reset encoder data for the wheels
	if (gamePad.GetRawButton(GAMEPAD_BUTTON_Y) == true) {
			encoder1.Reset();
			encoder2.Reset();
	}

	// Print gyro data
	if (showGyro == true) {
		SmartDashboard::PutNumber("Gyro Angle (Raw)", gyro.GetAngle()*GYRO_SCALE_FACTOR);
		SmartDashboard::PutNumber("Gyro Angle (Edited)", editedGyroAngle*GYRO_SCALE_FACTOR);
		SmartDashboard::PutNumber("Gyro Rate (Raw)", gyro.GetRate()*GYRO_SCALE_FACTOR);
		SmartDashboard::PutNumber("Gyro Rate (Edited)", editedGyroRate*GYRO_SCALE_FACTOR);
	}


	// Print out the encoder data
	// Raw encoder data
//	if (showEncoderRaw == true) {
		SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
		SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());

//	}

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
			//myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), editedGyroRate*GYRO_SCALE_FACTOR);
			myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), editedGyroAngle*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ 2 joysticks
	case ARCADE_2:
		if (fabs(rStick.GetRawAxis(X_AXIS)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), -rStick.GetRawAxis(X_AXIS));
			gyro.Reset();
		} else {
			//myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), editedGyroRate*GYRO_SCALE_FACTOR);
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), editedGyroAngle*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ left stick on gamepad (the knockoff xbox controller)
	case ARCADE_GAMEPAD_1:
		if (fabs(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X));
			gyro.Reset();
		} else {
			//myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroRate*GYRO_SCALE_FACTOR);
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroAngle*GYRO_SCALE_FACTOR);
		}

		break;

	// Arcade drive w/ BOTH gamepad
	case ARCADE_GAMEPAD_2:
		if (fabs(gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X));
			gyro.Reset();
		} else {
			//myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroRate*GYRO_SCALE_FACTOR);
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroAngle*GYRO_SCALE_FACTOR);
		}

		break;

	// Tank drive (requires 2 sticks; each stick controls its respective 'tread' or side;
	//			   Ex: moving the right stick moves only the wheels on the right) w/ gamepad
	case TANK_GAMEPAD:
		joystickDifference = gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y) - gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y);
		joystickAverage = (gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y) + gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y))/2;
		if (fabs(joystickDifference) > TANK_TURN_THRESHOLD) {
			myRobot.TankDrive(gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y), gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y));
			gyro.Reset();
		} else {
			//myRobot.ArcadeDrive(joystickAverage, editedGyroRate*GYRO_SCALE_FACTOR);
			myRobot.ArcadeDrive(joystickAverage, editedGyroAngle*GYRO_SCALE_FACTOR);
		}

		break;

	// Tank drive w/ joysticks
	case TANK_2:
		joystickDifference = lStick.GetRawAxis(LEFT_STICK_Y) - rStick.GetRawAxis(RIGHT_STICK_Y);
		joystickAverage = (lStick.GetRawAxis(LEFT_STICK_Y) + rStick.GetRawAxis(RIGHT_STICK_Y))/2;
		if (fabs(joystickDifference) > TANK_TURN_THRESHOLD) {
			myRobot.TankDrive(lStick.GetRawAxis(LEFT_STICK_Y), rStick.GetRawAxis(RIGHT_STICK_Y));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(joystickAverage, -rStick.GetRawAxis(RIGHT_STICK_Y));
		}

		break;
	}
}


////////////////////////


// Increments values of anything based on buttons on the gamepad
// (Right bumper increases the value, Left bumper decreases)
void Robot::IncrementVariable(int &var, float delta) {
	if(gamePad.GetRawButton(GAMEPAD_RIGHT_BUMPER) == true && buttonDone[GAMEPAD_RIGHT_BUMPER] == false) {
		buttonDone[GAMEPAD_RIGHT_BUMPER] = true;
		var = var + delta;
	} else if(gamePad.GetRawButton(GAMEPAD_RIGHT_BUMPER) == false && buttonDone[GAMEPAD_RIGHT_BUMPER] == true){
		buttonDone[GAMEPAD_RIGHT_BUMPER] = false;
	}


	if(gamePad.GetRawButton(GAMEPAD_LEFT_BUMPER) == true && buttonDone[GAMEPAD_LEFT_BUMPER] == false) {
		buttonDone[GAMEPAD_LEFT_BUMPER] = true;
		var = var - delta;
	} else if(gamePad.GetRawButton(GAMEPAD_LEFT_BUMPER) == false && buttonDone[GAMEPAD_LEFT_BUMPER] == true){
		buttonDone[GAMEPAD_LEFT_BUMPER] = false;
	}

}



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Implements the 'Robot' class into the main robot loop
START_ROBOT_CLASS(Robot)
