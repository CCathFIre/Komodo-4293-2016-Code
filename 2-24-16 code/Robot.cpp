#include "WPILib.h"
#include "Definitions.h"
#include "Lifter.h"
#include "BallManipulator.h"



// Main robot class
class Robot: public IterativeRobot
{


	Relay solenoid;

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
	bool povDone[9]; //Separate array for the d-pad on the gamepad. Starts with null value, then clockwise starting North.

	// To know what data will be put on the dashboard
	bool showDriveEncoders;
	bool showEncoderIndex;
	bool showGyro;
	bool showDriveControl;
	bool showBallManipulatorData;

	// Switch for gyro drive
	bool isGyroDrive;

	//Switch for different auto defenses
	bool autoOption;


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
		solenoid(1),
		ballManipulator(B_MANIPULATOR_CHANNEL_LIFT, LEFT_PINCHER, RIGHT_PINCHER, encoderM)
	{
		myRobot.SetExpiration(0.1);

		driveOption = TANK_2;
		autoOption = CHEVAL;

		gyro.InitGyro();

		showDriveEncoders = false;
		showEncoderIndex = false;
		showGyro = true;
		showDriveControl = true;
		showBallManipulatorData= true;

		autoTurn = 0;

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
	void PrintStatements(); //I got tired of the giant print statement cloud, so I made it its own function!

	//Functions to automatically handle certain defenses, used in auto and teleop as needed
	void AutoCheval();
	void AutoPortculis();
	void AutoDrawbridge();
	void AutoGate();

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

	// Edits the gyro angle to account for drift
	if (fabs(gyro.GetRate()) > GYRO_DRIFT_VALUE)
		editedGyroAngle = gyro.GetAngle();
	else {
		gyro.Reset();
		editedGyroAngle = 0;
	}

	switch(autoOption){ //switch statement for different defense crossings in auto.

	//THESE NEED TO BE SEVERELY EDITED!!!!!!!!!!!!!
	case ROUGH_TERRAIN:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	break;


	case RAMPARTS:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	break;


	case CHEVAL:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	AutoCheval();

	break;


	case PORTCULIS:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	AutoPortculis();

	break;


	case DRAWBRIDGE:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	AutoDrawbridge();

	break;


	case GATE:

	autoTurn = 0; //Don't turn
	// Go foward 3 feet at 1/3 speed, stop (adjusting for drift)
	if (encoder1.GetRaw() < ONE_FOOT_LEFT_ENCODER*3 && encoder2.GetRaw() < ONE_FOOT_RIGHT_ENCODER*3)
		DriverControl(AUTOBOT, 0.3, autoTurn);
	else
		DriverControl(AUTOBOT, 0, 0);

	AutoGate();

	break;

	}


	// Print the raw encoder data
	SmartDashboard::PutNumber("Encoder L get raw", encoder1.GetRaw());
	SmartDashboard::PutNumber("Encoder R get raw", encoder2.GetRaw());

}


////////////////////////


// Starts at the beginning of the teleop (user controlled) period
void Robot::TeleopInit() {

	isGyroDrive = true;
	driveOption = TANK_2;
	gyro.Reset();
	encoder1.Reset();
	encoder2.Reset();
	encoderM.Reset();
	encoderU.Reset();
	gyroScaleFactor = GYRO_SCALE_FACTOR;

	for(int c=0; c<7; c++) // Reset the button pulse array
		buttonDone[c] = false;

	for(int c=0; c<9; c++) // Reset the POV pulse array
		povDone[c] = false;

}

// During every loop interval of the teleop period
void Robot::TeleopPeriodic() {

	// Edits the gyro angle to account for drift
	if (fabs(gyro.GetRate()) > GYRO_DRIFT_VALUE && fabs(gyro.GetRate()) < GYRO_RUSH_SPEED && isGyroDrive == true)
		editedGyroAngle = gyro.GetAngle();
	else {
		gyro.Reset();
		editedGyroAngle = 0;
	}

	//IncrementVariable(driveOption, 1, 1, 6); //Enable toggling between drive modes with a min of 1 (ARCADE_1) and a max of 6 (TANK_2)

	DriverControl(driveOption, 0, 0); //Currently, all driving is being done in the TANK_2 mode.
	ButtonControl();
	ballManipulator.Tilt(B_MANIPULATOR_SCALE_FACTOR*gamePad.GetRawAxis(GAMEPAD_RIGHT_STICK_Y));

	PrintStatements(); //This is at the BOTTOM OF THE CODE (so I don't have to scroll through it constantly)


}

/////////////////

void Robot::AutoCheval() { //Automatic functions using encoder data to handle defenses

}

void Robot::AutoPortculis() {

}

void Robot::AutoDrawbridge() {

}

void Robot::AutoGate() {

}

/////////////////

void Robot::ButtonControl() { //Allows for single-pulse functions on every gamepad button, as well as the D-Pad

//N, S, E, and W on the D-pad give autonomous cheval de frise, portculis, drawbridge, and gate functions.
	if(gamePad.GetRawButton(GAMEPAD_N_POV)== true && povDone[GAMEPAD_N_POV]== false){
		povDone[GAMEPAD_N_POV] = true;
		AutoCheval();

	} else if(gamePad.GetRawButton(GAMEPAD_N_POV) == false && povDone[GAMEPAD_N_POV] == true){
		povDone[GAMEPAD_N_POV] = false;
	}


	if(gamePad.GetRawButton(GAMEPAD_S_POV)== true && povDone[GAMEPAD_S_POV]== false){
		povDone[GAMEPAD_S_POV] = true;
		AutoPortculis();

	} else if(gamePad.GetRawButton(GAMEPAD_S_POV) == false && povDone[GAMEPAD_S_POV] == true){
		povDone[GAMEPAD_S_POV] = false;
	}


	if(gamePad.GetRawButton(GAMEPAD_E_POV)== true && povDone[GAMEPAD_E_POV]== false){
		povDone[GAMEPAD_E_POV] = true;
		AutoDrawbridge();

	} else if(gamePad.GetRawButton(GAMEPAD_E_POV) == false && povDone[GAMEPAD_E_POV] == true){
		povDone[GAMEPAD_E_POV] = false;
	}


	if(gamePad.GetRawButton(GAMEPAD_W_POV)== true && povDone[GAMEPAD_W_POV]== false){
		povDone[GAMEPAD_W_POV] = true;
		AutoGate();

	} else if(gamePad.GetRawButton(GAMEPAD_W_POV) == false && povDone[GAMEPAD_W_POV] == true){
		povDone[GAMEPAD_W_POV] = false;
	}


//Buttons on the gamepad control various other things.
	if(gamePad.GetRawButton(GAMEPAD_BUTTON_A)== true && buttonDone[GAMEPAD_BUTTON_A]== false){
		buttonDone[GAMEPAD_BUTTON_A] = true;
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
		ballManipulator.Open();
		solenoid.Set(Relay::kForward);

	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_X) == false && buttonDone[GAMEPAD_BUTTON_X] == true){
		buttonDone[GAMEPAD_BUTTON_X] = false;
		solenoid.Set(Relay::kOff);
	}



	if(gamePad.GetRawButton(GAMEPAD_BUTTON_Y)== true && buttonDone[GAMEPAD_BUTTON_Y]== false){
		buttonDone[GAMEPAD_BUTTON_Y] = true;
		ballManipulator.Close();
		solenoid.Set(Relay::kReverse);

	} else if(gamePad.GetRawButton(GAMEPAD_BUTTON_Y) == false && buttonDone[GAMEPAD_BUTTON_Y] == true){
		buttonDone[GAMEPAD_BUTTON_Y] = false;
		solenoid.Set(Relay::kOff);
	}


}

////////////////////////

// Increments values of anything based on buttons on the gamepad. Right bumper increases the value, Left bumper decreases
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

///////////////////////////

// During every loop interval of the test period
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

//////////////////



void Robot::PrintStatements() { //Function to print stuff. Makes the code much easier to read through.

		if(showBallManipulatorData == true) {
			SmartDashboard::PutNumber("Lift moter encoder raw", encoderM.GetRaw());
			SmartDashboard::PutNumber("Lift moter encoder rate", encoderM.GetRate());

		}


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

		// Print gyro data
		if (showGyro == true) {
			SmartDashboard::PutNumber("Gyro Angle (Raw)", gyro.GetAngle()*gyroScaleFactor);
			SmartDashboard::PutNumber("Gyro Angle (Edited)", editedGyroAngle*gyroScaleFactor);
			SmartDashboard::PutNumber("Gyro Rate (Raw)", gyro.GetRate()*gyroScaleFactor);
			SmartDashboard::PutNumber("Gyro Rate (Edited)", editedGyroRate*gyroScaleFactor);
			SmartDashboard::PutNumber("Gyro Scale Factor", gyroScaleFactor);
		}


		// Print out the encoder data
		// Raw encoder data
		if (showDriveEncoders == true) {
			SmartDashboard::PutNumber("Encoder R get raw", encoder1.GetRaw());
			SmartDashboard::PutNumber("Encoder L get raw", encoder2.GetRaw());
			SmartDashboard::PutNumber("Manipulator Encoder Raw", encoderM.GetRaw());
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



////////////////////////////////////////
/////////////   Divider!   /////////////
////////////////////////////////////////



// Implements the 'Robot' class into the main robot loop
START_ROBOT_CLASS(Robot)
