#include "WPILib.h"
#include "Definitions.h"
#include "Lifter.h"
#include "BallManipulator.h"



// My main robot class
class Robot: public IterativeRobot
{

	Solenoid leftPincher;
	Solenoid rightPincher;

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
	bool showGyro;
	bool showDriveControl;
	bool showBallManipulatorData;

	// Switch for gyro drive
	bool isGyroDrive;

	// Gyro stuff
	AnalogGyro gyro;
	double editedGyroRate;
	double editedGyroAngle;
	float gyroScaleFactor;

	// Encoders
	Encoder encoder1; //Right drive motor
	Encoder encoder2; //Left drive motor

	Encoder encoderM; //Ball manipulator encoder
	Encoder encoderU; //Lifter encoder

	// A turn value for when the robot is in auto
	double autoTurn;

	// Manipulator classes
	Lifter lifter;
	BallManipulator ballManipulator;

	Servo testServo;

	// For when we press 'A', it goes up. When we press it again, it goes down. These variables help
	// do that
	int servoActionType;
	float servoAngle;
	float servoRange;
	float servoIncrement;

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
		encoder1(ENCODER_CHANNEL_RA, ENCODER_CHANNEL_RB),
		encoder2(ENCODER_CHANNEL_LA, ENCODER_CHANNEL_LB),
		encoderM(ENCODER_CHANNEL_MA, ENCODER_CHANNEL_MB),
		encoderU(ENCODER_CHANNEL_UA, ENCODER_CHANNEL_UB),
		lifter(LIFTER_CHANNEL_LIFT, LIFTER_CHANNEL_TILT, encoderU),
		ballManipulator(B_MANIPULATOR_CHANNEL_LIFT, B_MANIPULATOR_CHANNEL_PINCH, encoderM),
		testServo(6),
		leftPincher(LEFT_PINCHER),
		rightPincher(RIGHT_PINCHER)
	{
		myRobot.SetExpiration(0.1);

		driveOption = TANK_GAMEPAD;

		gyro.InitGyro();

		showEncoderRaw = false;
		showEncoderRate = false;
		showEncoderIndex = false;
		showGyro = true;
		showDriveControl = true;
		showBallManipulatorData= false;

		autoTurn = 0;

		servoActionType = 0;
	}

private:
	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestPeriodic();


	void DriverControl(int driveControl, float autoSpeed, float autoTurn);
	void IncrementVariable(int &var, float delta, float min, float max);
	void ButtonControl();
};



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Starts at the beginning of the autonomous period
void Robot::AutonomousInit() {
	autoLoopCounter = 0;

	gyroScaleFactor = GYRO_SCALE_FACTOR;
	gyro.Reset();
	encoder1.Reset();
	encoder2.Reset();
	encoderU.Reset();
	encoderM.Reset();
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

	autoTurn = 0; //Don't turn


	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);


	// Print the raw encoder data
	SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
	SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());

}


////////////////////////


// Starts at the beginning of the teleop (user controlled) period
void Robot::TeleopInit() {

	isGyroDrive = true;
	gyro.Reset();
	encoder1.Reset();
	encoder2.Reset();
	encoderM.Reset();
	encoderU.Reset();
	gyroScaleFactor = GYRO_SCALE_FACTOR;

	servoAngle = testServo.GetAngle();	// Reset the buttons
	servoRange = fabs(testServo.GetMaxAngle() - testServo.GetMinAngle());
	servoIncrement = servoRange / 30;
	for(int c=0; c<7; c++)
		buttonDone[c] = false;

}

// During every loop interval of the teleop period
void Robot::TeleopPeriodic() {

	servoAngle = testServo.GetAngle();
	// Choose the teleop drive option
	//IncrementVariable(driveOption, 1, 1, 6); //Enable toggling between drive modes with a min of 1 (ARCADE_1) and a max of 6 (TANK_2)
	driveOption = TANK_2;
	DriverControl(driveOption, 0, 0);
	ButtonControl();


	// Edits the gyro angle to account for drift
	if (fabs(gyro.GetRate()) > GYRO_DRIFT_VALUE && fabs(gyro.GetRate()) < GYRO_RUSH_SPEED && isGyroDrive == true)
		editedGyroAngle = gyro.GetAngle();
	else {
		gyro.Reset();
		editedGyroAngle = 0;
	}


	//Temp? functions to test the manipulator tilter motor.
/*
	if(gamePad.GetPOV(GAMEPAD_N_POV)){
		ballManipulator.GoUp();
	}
	if(gamePad.GetPOV(GAMEPAD_S_POV)){
		ballManipulator.GoDown();
	}
	if(!gamePad.GetPOV(GAMEPAD_N_POV) && !gamePad.GetPOV(GAMEPAD_S_POV)){
		ballManipulator.StopAll();
	}
	*/
	//End temp? functions
	ballManipulator.GoUp(-0.5*gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y));


	SmartDashboard::PutNumber("Gamepad POV", gamePad.GetPOV());
	SmartDashboard::PutNumber("Servo angle", ballManipulator.GetAngle());
	//print servo data
	/*
	if(showBallManipulatorData == true) {
		SmartDashboard::PutNumber("Servo angle", ballManipulator.currentAngle);
		SmartDashboard::PutNumber("Servo increment", ballManipulator.incrementAngle);
		SmartDashboard::PutNumber("Lift moter encoder raw", encoderM.GetRaw());
		SmartDashboard::PutNumber("Lift moter encoder rate", encoderM.GetRate());
		SmartDashboard::PutNumber("Pincher Range", ballManipulator.pincherRange);
		SmartDashboard::PutNumber("Pincher max angle", ballManipulator.pincherMaxAngle);
		SmartDashboard::PutNumber("Pincher min angle", ballManipulator.pincherMinAngle);

	}
*/

	//Print Drive Mode
	if(showDriveControl == true) {
	    SmartDashboard::PutNumber("Drive Mode = ", driveOption);
	    SmartDashboard::PutNumber("Joystick Arcade = ", ARCADE_1);
	    SmartDashboard::PutNumber("Split Joystick Arcade = ", ARCADE_2);
	    SmartDashboard::PutNumber("Gamepad Arcade = ", ARCADE_GAMEPAD_1);
	    SmartDashboard::PutNumber("Split Gamepad Arcade = ", ARCADE_GAMEPAD_2);
	    SmartDashboard::PutNumber("Gamepad Tank = ", TANK_GAMEPAD);
	    SmartDashboard::PutNumber("Joystick Tank = ", TANK_2);
	    SmartDashboard::PutNumber("isGyroDrive",isGyroDrive);
	}

	//Print Gyro scale factor
	SmartDashboard::PutNumber("Gyro Scale Factor", gyroScaleFactor);

	// Print gyro data
	if (showGyro == true) {
		SmartDashboard::PutNumber("Gyro Angle (Raw)", gyro.GetAngle()*gyroScaleFactor);
		SmartDashboard::PutNumber("Gyro Angle (Edited)", editedGyroAngle*gyroScaleFactor);
		SmartDashboard::PutNumber("Gyro Rate (Raw)", gyro.GetRate()*gyroScaleFactor);
		SmartDashboard::PutNumber("Gyro Rate (Edited)", editedGyroRate*gyroScaleFactor);
	}


	// Print out the encoder data
	// Raw encoder data
	//if (showEncoderRaw == true) {
		SmartDashboard::PutNumber("Encoder R get raw", encoder1.GetRaw());
		SmartDashboard::PutNumber("Encoder L get raw", encoder2.GetRaw());
		SmartDashboard::PutNumber("Manipulator Encoder Raw", encoderM.GetRaw());

	//}


	// The delta encoder change
	if (showEncoderRate == true) {
		SmartDashboard::PutNumber("Encoder R (reversed) get rate", encoder1.GetRate());
		SmartDashboard::PutNumber("Encoder L get rate", encoder2.GetRate());
		SmartDashboard::PutNumber("Manipulator Encoder Rate", encoderM.GetRate());
	}


	// Print the encoder index
	if (showEncoderIndex == true) {
		SmartDashboard::PutNumber("Encoder R index", encoder1.GetFPGAIndex());
		SmartDashboard::PutNumber("Encoder L index", encoder2.GetFPGAIndex());
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
void Robot::DriverControl(int driveControl, float autoSpeed, float autoTurning) {
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
			myRobot.ArcadeDrive(rStick.GetRawAxis(Y_AXIS), editedGyroAngle*gyroScaleFactor);
		}

		break;

	// Arcade drive w/ 2 joysticks
	case ARCADE_2:

		if (fabs(rStick.GetRawAxis(X_AXIS)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), -rStick.GetRawAxis(X_AXIS));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(-lStick.GetRawAxis(Y_AXIS), editedGyroAngle*gyroScaleFactor);
		}

		break;

	// Arcade drive w/ left stick on gamepad (the knockoff xbox controller)
	case ARCADE_GAMEPAD_1:

		if (fabs(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_X));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroAngle*gyroScaleFactor);
		}

		break;

	// Arcade drive w/ BOTH gamepad
	case ARCADE_GAMEPAD_2:

		if (fabs(gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X)) > JOYSTICK_STANDARD_DRIFT) {
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), -gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_X));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(-gamePad.GetRawAxis(GAMEPAD_LEFT_STICK_Y), editedGyroAngle*gyroScaleFactor);
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
			myRobot.ArcadeDrive(joystickAverage, editedGyroAngle*gyroScaleFactor);
		}

		break;

	// Tank drive w/ joysticks
	case TANK_2:
		joystickDifference = lStick.GetRawAxis(LEFT_STICK_Y) - rStick.GetRawAxis(RIGHT_STICK_Y);
		joystickAverage = (lStick.GetRawAxis(LEFT_STICK_Y) + rStick.GetRawAxis(RIGHT_STICK_Y))/2;

		if (fabs(joystickDifference) > TANK_TURN_THRESHOLD) {
			myRobot.TankDrive(rStick.GetRawAxis(RIGHT_STICK_Y), lStick.GetRawAxis(LEFT_STICK_Y));
			gyro.Reset();
		} else {
			myRobot.ArcadeDrive(joystickAverage, editedGyroAngle*gyroScaleFactor);
		}

		break;

	case AUTOBOT:
		if(autoTurning == 0){
			myRobot.ArcadeDrive(autoSpeed, editedGyroAngle*gyroScaleFactor);
		} else {
			myRobot.ArcadeDrive(autoSpeed, autoTurning);
			gyro.Reset();
		}


		break;
	}
}


////////////////////////


// Increments values of anything based on buttons on the gamepad
// (Right bumper increases the value, Left bumper decreases)
void Robot::IncrementVariable(int &var, float delta, float min, float max) {
	if(gamePad.GetRawButton(GAMEPAD_RIGHT_BUMPER) == true && buttonDone[GAMEPAD_RIGHT_BUMPER] == false) {
		buttonDone[GAMEPAD_RIGHT_BUMPER] = true;
		var = var + delta;
		if(var > max + 0.0001)
			var = min;
	} else if(gamePad.GetRawButton(GAMEPAD_RIGHT_BUMPER) == false && buttonDone[GAMEPAD_RIGHT_BUMPER] == true){
		buttonDone[GAMEPAD_RIGHT_BUMPER] = false;
	}

	if(gamePad.GetRawButton(GAMEPAD_LEFT_BUMPER) == true && buttonDone[GAMEPAD_LEFT_BUMPER] == false) {
		buttonDone[GAMEPAD_LEFT_BUMPER] = true;
		var = var - delta;
		if(var < min - 0.0001)
			var = max;
	} else if(gamePad.GetRawButton(GAMEPAD_LEFT_BUMPER) == false && buttonDone[GAMEPAD_LEFT_BUMPER] == true){
		buttonDone[GAMEPAD_LEFT_BUMPER] = false;
	}

}







void Robot::ButtonControl() {

	if(gamePad.GetRawButton(GAMEPAD_BUTTON_A)== true && buttonDone[GAMEPAD_BUTTON_A]== false){
		buttonDone[GAMEPAD_BUTTON_A] = true;
/*
		if (servoActionType == 0) {
						ballManipulator.Open();
						servoActionType = 1;
					} else if (servoActionType == 1) {
						ballManipulator.Close();
						servoActionType = 0;
					}

*/
//Put this back when Open and Close are a toggle.
	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_A) == false && buttonDone[GAMEPAD_BUTTON_A] == true){
		buttonDone[GAMEPAD_BUTTON_A] = false;
	}



	if(gamePad.GetRawButton(GAMEPAD_BUTTON_B)== true && buttonDone[GAMEPAD_BUTTON_B]== false){
		buttonDone[GAMEPAD_BUTTON_B] = true;
		isGyroDrive = !isGyroDrive;

	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_B) == false && buttonDone[GAMEPAD_BUTTON_B] == true){
		buttonDone[GAMEPAD_BUTTON_B] = false;
	}


	// To reset encoder data for the wheels

    if(gamePad.GetRawButton(GAMEPAD_BUTTON_X)== true && buttonDone[GAMEPAD_BUTTON_X]== false){
		buttonDone[GAMEPAD_BUTTON_X] = true;
		/*
		servoAngle = servoAngle + servoIncrement;
		if(servoAngle > testServo.GetMaxAngle())
			servoAngle = testServo.GetMaxAngle() - servoIncrement;
		testServo.Set(servoAngle);
		*/
		ballManipulator.TestServoF();
	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_X) == false && buttonDone[GAMEPAD_BUTTON_X] == true){
		buttonDone[GAMEPAD_BUTTON_X] = false;
	}



	if(gamePad.GetRawButton(GAMEPAD_BUTTON_Y)== true && buttonDone[GAMEPAD_BUTTON_Y]== false){
		buttonDone[GAMEPAD_BUTTON_Y] = true;
		/*
		servoAngle = servoAngle - servoIncrement;
				if(servoAngle < testServo.GetMinAngle())
					servoAngle = testServo.GetMinAngle() + servoIncrement;
				testServo.Set(servoAngle);
		*/
		ballManipulator.TestServoR();
	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_Y) == false && buttonDone[GAMEPAD_BUTTON_Y] == true){
		buttonDone[GAMEPAD_BUTTON_Y] = false;
	}


} //



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Implements the 'Robot' class into the main robot loop
START_ROBOT_CLASS(Robot)
