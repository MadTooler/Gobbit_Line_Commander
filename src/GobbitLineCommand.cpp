/*
*	GobbitLineCommand.h
*	Library for line following, intersection detection, and basic motor control of Gobbit robot.
*	Created by Jason Talley 
*	Last edit 05/04/2019
*	Released under GNU agreement
*/

/*
*	PD Line Following for the Gobbit and Zagros Robotics Starter Kit with Pololu Line Sensor
*
*	For assembly, wiring, programming, and other examples, see: http://www.primalengineering.com/robots 
*
*
*	Parts list:
*
*		1) Gobbit Robot with line following configuration
*			Components:
*				Gobbit Chassis with motors, switch, and battery holder
*				Sparkfun Redboard (arduino uno clone)
*				Motor shield
*					Sparkfun Ardumoto Shield
*					or
*					Adafruit v 2.3 motor shield
*				Jumper wires
*				Breadboard optional
*				Pololu QTR-8RC RC Reflectance Sensor Array
*
*			  see: Zagros Robot Starter Kit - Gobbit
*			       http://www.zagrosrobotics.com/
*
*		2) Black electrical tape line course on white background
*
*		4) Batteries
*/


#include <Arduino.h>

//QTRSensors library must be loaded to your arduino libraries folder.
// Easiest to use the library manager to install
// later releases may be able to auto load if this library is made accessible in arduino library manager.
//#include <QTRSensors.h> // Pololu QTR Library 
#include "QTRSensors/QTRSensors.h" //**** use a local copy of Pololu QTR Library for temporary easier install

#include "GobbitLineCommand.h"
#include "config.h"
#include "AdafruitMSDefaults.h"
#include "ArdumotoDefaults.h"

#if SERVO_ENABLE
	#include <Servo.h>

	// object for the gripper servo
	Servo gripper;
#endif

// load Adafruit Motor Shield library and initialize objects
//     NOTE cannot access USE_AFMS or status of ADAFRUIT_MS from main sketch for conditional loading of AFMS library.
//     This is a limitation of the arduino IDE compiler, and it does not appear to be adopted in the future.
//#include <Adafruit_MotorShield.h>
#include "Adafruit_Motor_Shield_V2_Library/Adafruit_MotorShield.h" //**** use a local copy of Adafruit Library for temporary easier install

// #define ADAFRUIT_MS in main program if the Adafruit motor shield v2.3 is to be used.
// M1 and M2 will be used.  Right motor on M1, Left on M2.
// No define will default to Sparkfun Ardumoto or other makes of Dir
// and PWM pin driven style drivers similar to the Ardumoto and those
// based upon the L298 driver.  The default pin settings will be used
// except where new values have been declared.

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor* rightMotor = AFMS.getMotor(1);
// and another motor on port M2
Adafruit_DCMotor* leftMotor = AFMS.getMotor(2);

// line sensor object
QTRSensorsRC qtrrc;


// Initializes the sensor and motors.  
// Call in setup loop.  
// Call setQTRpins, setLeftMotorPinsDirPWM, setRightMotorPinsDirPWM, and setBatteryVolts
// functions before calling this function, if other than default are to be used.
void GobbitLineCommand::beginGobbit(void)
{

	// initialize sensor
	qtrrc.init(sensorPins, NUM_SENSORS, TIME_OUT, EMITTER_PIN);
	delay(500); // give sensors time to set
	
	// set reference time for PID loop
	// **** not sure if this is necessary or needed here
	mLastTime = millis()-mSampleTime;

	if (volts == NOT_SET) {
		volts = 9;
	}

	if (useAFMS) {
		
		// Adafruit Motorshield v2.3
		
		if (brakeStrength == NOT_SET)
				brakeStrength = BRAKE_STRENGTH_AF;

		if (volts <= 8.3) // use 7.2 and 7.4 settings
		//if(volts > 5 && volts < 7.8) // use 7.2 and 7.4 settings
		{
			if (_kp == NOT_SET)
				_kp = SEVENV_AF_KP;
			if (_ki == NOT_SET)
				_ki = SEVENV_AF_KI;			
			if (_kd == NOT_SET)
				_kd = SEVENV_AF_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = SEVENV_AF_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = SEVENV_AF_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = SEVENV_AF_KD_COARSE;
			if (PDfineRange == NOT_SET)
				PDfineRange = SEVENV_AF_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = SEVENV_AF_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = SEVENV_AF_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = SEVENV_AF_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = SEVENV_AF_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = SEVENV_AF_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = SEVENV_AF_TURN_SPD_LOW;
		}

		else if (volts > 8.3 && volts <= 9.9) // use 9v settings
		{
			if (_kp == NOT_SET)
				_kp = NINEV_AF_KP;
			if (_ki == NOT_SET)
				_ki = NINEV_AF_KI;	
			if (_kd == NOT_SET)
				_kd = NINEV_AF_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = NINEV_AF_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = NINEV_AF_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = NINEV_AF_KD_COARSE;
			if (PDfineRange == NOT_SET)
				PDfineRange = NINEV_AF_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = NINEV_AF_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = NINEV_AF_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = NINEV_AF_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = NINEV_AF_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = NINEV_AF_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = NINEV_AF_TURN_SPD_LOW;
		}

		else if (volts > 9.9) // use 11.1v settings
		{
			if (_kp == NOT_SET)
				_kp = ELEVENV_AF_KP;
			if (_ki == NOT_SET)
				_ki = ELEVENV_AF_KI;	
			if (_kd == NOT_SET)
				_kd = ELEVENV_AF_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = ELEVENV_AF_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = ELEVENV_AF_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = ELEVENV_AF_KD_COARSE;			
			if (PDfineRange == NOT_SET)
				PDfineRange = ELEVENV_AF_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = ELEVENV_AF_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = ELEVENV_AF_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = ELEVENV_AF_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = ELEVENV_AF_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = ELEVENV_AF_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = ELEVENV_AF_TURN_SPD_LOW;
		}

		// create with the default frequency 1.6KHz
		AFMS.begin();

		//set both motors to stop
		setMotors(0, 0);
	}

	else { 		
	
		// ardumoto or similar driver Dir and PWM based driver
		
		if (brakeStrength == NOT_SET)
				brakeStrength = BRAKE_STRENGTH;

		if (volts < 8.3) // use 7.2 and 7.4 settings
		//if(volts > 5 && volts < 7.8) // use 7.2 and 7.4 settings
		{
			if (_kp == NOT_SET)
				_kp = SEVENV_KP;
			if (_ki == NOT_SET)
				_ki = SEVENV_KI;
			if (_kd == NOT_SET)
				_kd = SEVENV_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = SEVENV_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = SEVENV_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = SEVENV_KD_COARSE;
			if (PDfineRange == NOT_SET)
				PDfineRange = SEVENV_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = SEVENV_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = SEVENV_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = SEVENV_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = SEVENV_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = SEVENV_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = SEVENV_TURN_SPD_LOW;
		}

		else if (volts > 8.3 && volts < 9.9) // use 9v settings
		{
			if (_kp == NOT_SET)
				_kp = NINEV_KP;
			if (_ki == NOT_SET)
				_ki = NINEV_KI;
			if (_kd == NOT_SET)
				_kd = NINEV_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = NINEV_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = NINEV_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = NINEV_KD_COARSE;
			if (PDfineRange == NOT_SET)
				PDfineRange = NINEV_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = NINEV_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = NINEV_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = NINEV_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = NINEV_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = NINEV_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = NINEV_TURN_SPD_LOW;
		}

		else if (volts > 9.9) // use 11.1v settings
		{
			if (_kp == NOT_SET)
				_kp = ELEVENV_KP;
			if (_ki == NOT_SET)
				_ki = ELEVENV_KI;
			if (_kd == NOT_SET)
				_kd = ELEVENV_KD;
			if (_kpCoarse == NOT_SET)
				_kpCoarse = ELEVENV_KP_COARSE;
			if (_kiCoarse == NOT_SET)
				_kiCoarse = ELEVENV_KI_COARSE;
			if (_kdCoarse == NOT_SET)
				_kdCoarse = ELEVENV_KD_COARSE;
			if (PDfineRange == NOT_SET)
				PDfineRange = ELEVENV_PD_FINE_RANGE;
			if (_calSpeed == NOT_SET)
				_calSpeed = ELEVENV_CAL_SPD;
			if (minSpeed == NOT_SET)
				minSpeed = ELEVENV_MIN_SPD;
			if (maxSpeed == NOT_SET)
				maxSpeed = ELEVENV_MAX_SPD;
			if (brakeSpeed == NOT_SET)
				brakeSpeed = ELEVENV_BRAKE_SPD;
			if (turnSpeedHigh == NOT_SET)
				turnSpeedHigh = ELEVENV_TURN_SPD_HIGH;
			if (turnSpeedLow == NOT_SET)
				turnSpeedLow = ELEVENV_TURN_SPD_LOW;
		}

		//Set motor driver control pins to be outputs
		pinMode(pwm_a, OUTPUT);
		pinMode(pwm_b, OUTPUT);
		pinMode(dir_a, OUTPUT);
		pinMode(dir_b, OUTPUT);
		
		// stop both motors
		setMotors(0, 0);
	}

}



//-----------------
// Run a calibration of the line sensor by sweeping back and forth 
// over a line at the called speed between 0-100.
//   Speed must be a 0 or positive value
//   0 will load the default values based upon voltage if declared.
void GobbitLineCommand::calibrateLineSensor(void)
{
	calibrateLineSensor(0);
}



//-----------------
// Run a calibration of the line sensor by sweeping back and forth 
// over a line at the called speed between 0-100.
//   Speed must be a 0 or positive value
//   0 will load the default values based upon voltage if declared.
void GobbitLineCommand::calibrateLineSensor(int calSpeed)
{
	calSpeed = constrain(calSpeed, 0,100);
	
	// use default calspeed if not set here
	if (calSpeed==0)
		calSpeed = _calSpeed;
	
	// only used for updating value that is printed with serial print.
	else
		_calSpeed = calSpeed;

	// calibrate line sensor; determines min/max range of sensed values for the current course
	for (int i = 0; i <= 100; i++) // begin calibration cycle for 100 reads of sensors
	{
		if (i == 0 || i == 60) // slow sweeping turn right to pass sensors over line
		{
			setMotors(calSpeed, -calSpeed);
		}

		else if (i == 20 || i == 100) // slow sweeping turn left to pass sensors over line
		{
			setMotors(-calSpeed, calSpeed);
		}

		qtrrc.calibrate(); // reads all sensors with the define timeout set at 2500 microseconds (2.5 milliseconds) for sensor outputs to go low.

	} // end calibration cycle

	// read calibrated sensor values and obtain a measure of the line position from 0 to 7000
	linePosition = qtrrc.readLine(sensorValues);


 	// read the value of only a single sensor to see the line.
	// when the value is greater than 200 the sensor sees the line.
	while (sensorValues[7] < 200) // wait for outer line sensor to see line
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	/* **** was using this prior, but forced setting both calSpeed and turnSpeedLow if needed to adjust process.
	// slow down speed
	setMotors(-turnSpeedLow, turnSpeedLow);   */
	

	// make sure outer sensor is no longer over line
	while (sensorValues[7] > 190) // wait for outer most sensor to exit the line
	{
		linePosition = qtrrc.readLine(sensorValues);
	}

	// find near center
	while (linePosition > (3500)) // wait for line position to find near center
	{
		linePosition = qtrrc.readLine(sensorValues);
	} 

	//stop both motors with braking to the right (opposite the current left turning)
	brakeMotors(100,'R'); 
		
	// delay as indicator setup and calibration is complete
	delay(1000);

} // end calibration

//-----------------
// Drive will start driving/following the line and continue following until it is able to complete the requested 
//   direction at the next found intersection or end.  If it cannot make the requested direction, it will spin 
//   around fast and stop performing all other commands.
// Turn direction value of ('L')eft, ('R')ight, ('F')orward, ('S')top, or ('U')turn.
//    The U-turn will be evaluated by the found intersection turns available to achieve a 180 degree turn.
void GobbitLineCommand::drive(char turnDir) 
{
	// local flag
	byte exit = 0;

	while (1) // re-run until exits at turn instruction completion
	{

		beepCycle();

		if (turnDir == 'L') {

			if (foundLeft) {
				turn('L');
				return;
			}
			else if (foundEnd)
				noWayFreakOut();
		}

		else if (turnDir == 'R') {

			if (foundRight) {
				turn('R');
				return;
			}
			else if (foundEnd)
				noWayFreakOut();
		}

		else if (turnDir == 'F') {
			if (foundForward) {
				// reset flags except foundMark
				resetIntFlags(0);
				exit=1;
			}
			else if (foundEnd)
				noWayFreakOut();
		}

		else if (turnDir == 'S'){
			
			if(foundLeft || foundRight || foundForward || foundEnd) {

				brakeMotors(brakeStrength,'B');
			
				// make sure brake flag is now off in case the previous drive command was forward
				brakeNext = 0;

				return;
			}
		}

		else if (turnDir == 'U') {

			if (foundLeft && foundRight) {
				turn('U');
				return;
			}
			else if (foundLeft) {
				turn('R');
				return;
			}
			else if (foundRight) {
				turn('L');
				return;
			}

			else if (foundEnd) {
				turn('R');
				return;
			}
		}
		
		// followLine in follow until intersection is found mode
		followLine(3);
		
		// exit if this was a forward drive and the first intersection has already been passed
		if(exit){
			
			// set brake flag if the motors are running faster than brakeSpeed value
			// if they are not, the next turn call will see motorSpeeds as less than brakeSpeed
			// and not brake.
			if (RmotorSpeed > brakeSpeed || LmotorSpeed > brakeSpeed) 
				brakeNext = 1;
			
			// stop motors but not brake.
			setMotors(0,0);
			return;
		}

	} // end while

} // end drive function

//-----------------
// Follow the line in called Mode:
//   Mode 0, along the line and never exit so it will not do anything else outside of the function.
//   Mode 1, do 1 thing... run through the followLine function one time and make only one 
//           motor adjustment then exit.
//   Mode 2, do 2 things... run through the followLine function one time and make only one motor 
//           adjustment while checking if an intersection is present then exit.
//   Mode 3, follow along the line until an intersection is found, update intersection turn flags, then exit.
//
//      If setSonar has been called and a valid pin was set for the sensor, followLine also reads the sonar,
//      updates the distanceInch variable, and controls/slows down the motor speed per the range of safe distance
//      every time it is called
void GobbitLineCommand::followLine(byte followMode)
{

	// local flags
	byte findIntersection = 0;
	byte runOnce = 0;
	byte offLine = 0;
	
	// set mode flags
	switch (followMode) {

	// follow forever and do not leave loop
	case 0:
		findIntersection = 0;
		runOnce = 0;
		break;

	// follow for one motor adjustment then return
	case 1:
		findIntersection = 0;
		runOnce = 1;
		break;
		
	// follow for one motor adjustment and check if there was an intersection found then return
	case 2:
		findIntersection = 1;
		runOnce = 1;
		break;	
	
	// follow until an intersection is found then return
	case 3:
		findIntersection = 1;
		runOnce = 0;
		break;	
		
	// default to same as mode 0 values that will follow forever and not leave this loop
	default:
		findIntersection = 0;
		runOnce = 0;
		break;
	}

	float kpCurrent;
	float kiCurrent;
	float kdCurrent;

	// enable Operation Flux Capacitor if in mode run until find intersections
	// Part 1 of 3
	if (findIntersection && !runOnce) {
		// read calibrated sensor values and obtain a measure of the current line position from 0 to 7000.
		// This will become the previous read slightly delayed for line following while the
		// current will be used for reacting to the current intersection before following is
		// adjusted that had been leading to an exaggerated "twirk" when crossing the intersections.
		// "Twirk" is a result of fast reaction of bot to early or uneven sensor reads from seeing
		// part of an intersection before other parts since it would only not "twirk" if the intersection
		// was perfecting entered with all sensors reading simultaneously, which could almost never 
		// be the case since they are read in series.

		// tried this array before with a larger buffer, but it did not seem to work well.
		// It read sensors and stored the values numPastRead times to use as past reference.
		// the approximate time of 2.5ms/call creates a buffered time delay
		// for (int i = 0; i < numPastReads; i++)
		//	pastLinePositions[i] = qtrrc.readLine(sensorValues);

		pastLinePosition = qtrrc.readLine(sensorValues);
		// the read takes a very short time, but is a slight delay which we are utilizing.  Do not add additional delay.
	}

	while (1) {

		offLine = 0;

		// read calibrated sensor values and obtain a measure of the current line position from 0 to 7000.
		linePosition = qtrrc.readLine(sensorValues);

		// simple line following portion for way off line adjustment
		// 0 is far Right sensor while 7 (7000 return) is far Left sensor
		if (!findIntersection) {
			switch (linePosition) {

			// Line has moved off the left edge of sensor
			// This will make it turn fast to the left
			case 7000:
				setMotors(-turnSpeedHigh, turnSpeedHigh);
				offLine = 1;
				break;

			// Line had moved off the right edge of sensor
			// This will make it turn fast to the right
			case 0:
				setMotors(turnSpeedHigh, -turnSpeedHigh);
				offLine = 1;
				break;

			default:
				break;
			}
		}

		if (!offLine) {
			// The line is still within the sensors.
			// This will calculate adjusting speed to keep the line in center.

			// If followLine was called in a mode which is to check for intersections, then check
			if (findIntersection) {	
				// if an intersection is found then the found flags will be updated and will exit followLine
				if(detectIntersection())
					return;
			}

			// Operation Flux Capacitor if in mode run until find intersections
			// Part 2 of 3
			if (findIntersection && !runOnce) {
				error = (float)pastLinePosition - 3500; // 3500 is center measure of 7000 far left and 0 on far right
			}
			else
				error = (float)linePosition - 3500; // 3500 is center measure of 7000 far left and 0 on far right

			// determine how great error is and set kp and kd aggressive if needed
			if (abs(error) > 3500 * PDfineRange) {
				kpCurrent = _kpCoarse;
				kiCurrent = _kiCoarse;
				kdCurrent = _kdCoarse;
			}
			else {
				kpCurrent = _kp;
				kiCurrent = _ki;
				kdCurrent = _kd;
			}
			
			
#if PROCESS_TIME
			// added time check here to run only if PROCESS_TIME IS greater than 0
			unsigned long mCurrentTime = millis();
			unsigned long mCurrentDuration = (mCurrentTime - mLastTime);
			if(mCurrentDuration>=mSampleTime){
#endif
			
				// update iAccumError accumulated error
				iAccumError = iAccumError+error;

				// calculate the new Process Variable
				// this is the value that will be used to alter the speeds
				PV = kpCurrent * error + kiCurrent*iAccumError + kdCurrent * (error - lastError);
				
				lastError = error;

				//this code limits the PV (motor speed pwm value)
				// limit PV to maxSpeed - minSpeed
				if (PV > (maxSpeed - minSpeed)) {
					PV = (maxSpeed - minSpeed);
				}

				if (PV < -(maxSpeed - minSpeed)) {
					PV = -(maxSpeed - minSpeed);
				}

				// run beeper cycle
				beepCycle();

				if (useRangeSensor) 
					obstacleSpeedFactor = speedAdjust(readSonarInches());
				else
					obstacleSpeedFactor = 1;

				if (PV > 0) {
					RmotorSpeed = maxSpeed * obstacleSpeedFactor;
					LmotorSpeed = (maxSpeed - abs(PV)) * obstacleSpeedFactor;
				}

				if (PV < 0) {
					RmotorSpeed = (maxSpeed - abs(PV)) * obstacleSpeedFactor;
					LmotorSpeed = maxSpeed * obstacleSpeedFactor;
				}

				//set motor speeds
				setMotors(LmotorSpeed, RmotorSpeed);

#if PROCESS_TIME				
				mLastTime = mCurrentTime;
				
			}
			else beepCycle(); // run beeper cycle
#endif
			
		}
		

		// Exit followLine if it is in a single adjustment mode
		if(runOnce)
			return;
		
		// Operation Flux Capacitor if in mode run until find intersections
		// Part 3 of 3
		if (findIntersection && !runOnce) {
			// make old same as current
			pastLinePosition = linePosition;
		}

	} // end while

} // end followLine

//-----------------
// Turns the robot using the line sensor on a lined course.
// Uses the 'turnSpeedHigh' and 'turnSpeedLow' motor values while turning.
// Turn direction values of ('L')eft, ('R')ight, or ('U')turn
//   Left turns left until the next line is found, regardless of 90 or 180 degree.
//   Right turns right until the next line is found, regardless of 90 or 180 degree.
//   U-turn turns left and assumes a 180 degree turn is requested where one line is 
//      to be passed then stop at next.  This is actually the same as running two
//      turn('L') commands, therefore, if there was no line at 90 degree to the left,
//      the turn will pass the 180 mark and stop at the next line, wherever it may be.
//      If that was the case, a single Left or Right turn request would have been better.
void GobbitLineCommand::turn(char dir)
{
	beepCycle();

	// brake the motors if running faster than brakeSpeed value
	if (RmotorSpeed > brakeSpeed || LmotorSpeed > brakeSpeed || brakeNext) 
		// stop motors with full braking
		brakeMotors(brakeStrength,'A');
		
	else // stop both motors
		setMotors(0, 0);
	
	// reset flags except mark
	resetIntFlags(0);
	
	byte i = 1;
	
	// Turn left 90deg
	if(dir =='L' || dir=='U'){
		setMotors(-turnSpeedHigh, turnSpeedHigh);
			
		linePosition = qtrrc.readLine(sensorValues);
		
		
		if (dir == 'U') i=2;
		while(i){
			while (sensorValues[7] < 200) // wait for outer most sensor to find the line
			{
				beepCycle();

				linePosition = qtrrc.readLine(sensorValues);
			}

			if(i<2)
			// slow down speed to soften turn
			setMotors(-turnSpeedLow, turnSpeedLow);


			// make sure outer sensor is no longer over line
			while (sensorValues[7] > 190) // wait for outer most sensor to exit the line
			{
				beepCycle();

				linePosition = qtrrc.readLine(sensorValues);
			}
			i--;
		}
			
		turnPID();
	}
	

	// Turn right 90deg
	else if(dir=='R'){
	

		setMotors(turnSpeedHigh, -turnSpeedHigh);
						
		linePosition = qtrrc.readLine(sensorValues);

		while (sensorValues[0] < 200) // wait for outer most sensor to find the line
		{
			beepCycle();
			linePosition = qtrrc.readLine(sensorValues);
		}

		// slow down speed to soften turn
		setMotors(turnSpeedLow, -turnSpeedLow);

		// make sure outer sensor is no longer over line
		while (sensorValues[0] > 190) // wait for outer most sensor to exit the line
		{
			beepCycle();
			linePosition = qtrrc.readLine(sensorValues);
		}

			
		turnPID();	
			
		
			
		// previous method
		/* 		linePosition = qtrrc.readLine(sensorValues);

		while (sensorValues[0] < 200) // wait for outer most sensor to find the line
		{
			beepCycle();
			linePosition = qtrrc.readLine(sensorValues);
		}

		// slow down speed
		setMotors(turnSpeedLow, -turnSpeedLow);

		// make sure outer sensor is no longer over line
		while (sensorValues[0] > 190) // wait for outer most sensor to exit the line
		{
			beepCycle();
			linePosition = qtrrc.readLine(sensorValues);
		}

		// find center
		while (linePosition < (3500))// wait for line position to find near center
		{
			beepCycle();
			linePosition = qtrrc.readLine(sensorValues);
		}
		
		// stop both motors
		//setMotors(0, 0);
		
		// stop motors with full braking
		if(turnSpeedHigh>brakeSpeed)
			brakeMotors();
		else setMotors(0, 0);

		break; */
	}

} // end turn


//-----------------
// Turning/Pivot with full PID to find center
void GobbitLineCommand::turnPID(void)
{
	
	float kpCurrent;
	float kiCurrent;
	float kdCurrent;
	iAccumError=0;
	
	// counter to stay in turn pd loop
	// this had to be moved outside of case.
	byte keepTurning = 0; 
	
	int turnSpeedPID = turnSpeedLow*1.5;
	turnSpeedPID = constrain(turnSpeedPID, 25, 60);
		
	lastError = 0;
	
	while(1){
		error = (float)linePosition - 3500; // 3500 is center measure of 7000 far left and 0 on far right

		if (abs(error) < 100){ // value of 3500 to allow as OK off center.  too fine and it will hunt.
			
			keepTurning++;
			
			// stop both motors and exit if within stop zone for enough passes
			if(keepTurning>1){ 
				setMotors(0, 0);
				// reset iAccumError so the lineFollow calculation is not exaggerated
				iAccumError = 0;
				return;
			}
		}
		else keepTurning = 0;

				
		// adjust the PID values per motor shield.
		// these values were created by tuning and testing.
		if(useAFMS){
			kpCurrent = _kp / 5;
			kiCurrent = _ki * 3;
			kdCurrent = _kd;
		}
		else{
			kpCurrent = _kp / 10;
			kiCurrent = _ki * 1.5;
			kdCurrent = _kd * 1.25;
		}

		// make sure the ki value is not 0
		// This needs the ki value.  If PID is manually changed to 0 value, a minimal value will
		// be added here to make sure the turn is completed.
		if(kiCurrent == 0) kiCurrent = 0.0005;
		
		// update iAccumError accumulated error
		iAccumError = iAccumError+error;

		// calculate the new Process Variable
		// this is the value that will be used to alter the speeds
		// ****opposite turning motors is more aggressive than followline where motors are usually turning
		// 	in the same direction, so only use 70%  ****maybe this formula needs adjusting in a way other than this.
		PV = (kpCurrent * error + kiCurrent*iAccumError + kdCurrent * (error - lastError)); // **** *0.7;  
		lastError = error;
		
		
		//this code limits the PV (motor speed pwm value)
		// limit PV between -turnSpeedPID and turnSpeedPID	
		if (PV > turnSpeedPID) {
			PV = constrain(PV, 0,turnSpeedPID);
		}

		if (PV < -turnSpeedPID) {
			PV = constrain(PV, -turnSpeedPID, -.0001); // -.0001 b/c still need to keep it less than 0 for direction selection below
		}
		

		// run beeper cycle
		beepCycle();

		
		if (PV > 0) {
			RmotorSpeed = PV;
			LmotorSpeed = -PV;
			
			//RmotorSpeed = PV+turnSpeedLow;//turnSpeedHigh;// * obstacleSpeedFactor;
			//LmotorSpeed = -(PV+turnSpeedLow);//(turnSpeedHigh - abs(PV));// * obstacleSpeedFactor;
		}

		if (PV < 0) {
			RmotorSpeed = PV;
			LmotorSpeed = abs(PV);
			
			//RmotorSpeed = PV-turnSpeedLow;//(turnSpeedHigh - abs(PV));// * obstacleSpeedFactor;
			//LmotorSpeed = (abs(PV)+turnSpeedLow);//turnSpeedHigh;// * obstacleSpeedFactor;
		}

		//set motor speeds
		setMotors(LmotorSpeed, RmotorSpeed);
		
		linePosition = qtrrc.readLine(sensorValues);
	}
	
}


//-----------------
// spin around and stop forever.  Used to indicate could not do requested task.
void GobbitLineCommand::noWayFreakOut(void)
{
	// turn off beeper if it is on
	beep(0, 0, 0);

	// spin around
	setMotors(turnSpeedHigh, -turnSpeedHigh);

	delay(2000);

	// stop both motors
	setMotors(0, 0);

	// do nothing forever
	while (1) {
		// turn off beeper if it is on
		beep(0, 0, 0);
	}
}

//-----------------
//generate adjusting factor for speed based upon distance of object closer than safeDistanceInch
float GobbitLineCommand::speedAdjust(float currentDistance)
{

	float speedTemp_var = 0;

	if (currentDistance < safeDistanceInch) {

		speedTemp_var = ((currentDistance / safeDistanceInch) * gain);

		// compare to value for shutoff of motors
		if (speedTemp_var > 1) {
			speedTemp_var = 1;
		}

		else if (speedTemp_var < .35) {
			speedTemp_var = 0;
		}

		return speedTemp_var;
	}

	else {
		return 1;
	}
}

//-----------------
// Move function receives forward/reverse Speed and left/right Turn float values.
// Used for basic motion without any line following or any sensor feedback.
//   First float value is the Speed/Velocity, value of -100 to 100.
//     -100 moveSpeed = reverse full speed
//     100 = forward full speed
//     0 = stopped;
//   Second float value is the Turn, value of -100 to 100. 
//     -100 moveTurn = full left
//     100 = full right
//     0 = straight
void GobbitLineCommand::move(float moveSpeed, float moveTurn) 
{
	// reset all flags
	resetIntFlags(1);

	float thisTurnSpeed = 0;

	if (moveSpeed != 0) {

		// moveTurn is full right
		if (moveTurn == 100) // spin right
		{

			// spin to right ignoring -/+ of speed
		
			setMotors(abs(moveSpeed), -abs(moveSpeed));

		}

		// moveTurn is full left
		else if (moveTurn == -100) // spin left
		{
			// spin to left ignoring -/+ of speed
		
			setMotors(-abs(moveSpeed), abs(moveSpeed));
		}

		// move turn is not full left or right
		else {
			// calculate slower motor speed
			thisTurnSpeed = abs(moveSpeed) * ((100 - abs(moveTurn)) / 100);

			// steer while moving forward
			if (moveSpeed > 0) {
				// forward motion while pivoting/turning to the right
				if (moveTurn > 0) {
					setMotors(moveSpeed, thisTurnSpeed);
				}

				// forward motion while pivoting/turning to the left
				else if (moveTurn < 0) {
					setMotors(thisTurnSpeed, moveSpeed);
				}

				// turn is 0 so go straight forward
				else {
					setMotors(moveSpeed, moveSpeed);
				}
			}

			// steer while moving in reverse
			else if (moveSpeed < 0) {
				// reverse motion while pivoting/turning to the right
				if (moveTurn > 0) {
					setMotors(-thisTurnSpeed, moveSpeed);
				}

				// reverse motion while pivoting/turning to the left
				else if (moveTurn < 0) {
					setMotors(moveSpeed, -thisTurnSpeed);
				}

				// turn is 0 so go straight reverse
				else {
					setMotors(moveSpeed, moveSpeed);
				}
			}
		}
	}

	// must be 0 moveSpeed so stop both motors
	else {
		setMotors(0, 0);
	}
	
	beepCycle();
	
} // end move function


#if SERVO_ENABLE

	//-----------------
	// Setup for the gripper servo.
	// Pin#
	// Open position in degrees, 0-180
	// Closed position in degrees, 0-180
	void GobbitLineCommand::setGripPinOpenClosed(int pin, int open, int closed)
	{
		// set pin mode for gripper servo
		pinMode(pin, OUTPUT);

		gripPin = pin; // only used as flag and feedback for serialPrintCurrentSettings
		gripOpenAngle = open;
		gripClosedAngle = closed;

		// attaches the servo on pin declared
		gripper.attach(pin);
	}


	//-----------------
	// Closes the gripper to the closed angle that was declared with setGripPinOpenClosed
	void GobbitLineCommand::gripClose(void)
	{
		// close gripper
		gripper.write(gripClosedAngle);
		delay(700);
	}

	//-----------------
	// Opens the gripper to the open angle that was declared with setGripPinOpenClosed
	void GobbitLineCommand::gripOpen(void)
	{
		
		// open gripper
		gripper.write(gripOpenAngle);
		delay(700);
	}


	//-----------------
	// Opens the gripper to the declared open percent
	//   0 is same as closed
	//   100 is same as fully open
	void GobbitLineCommand::gripPercentOpen(int openPercent)
	{
		openPercent = constrain(openPercent, 0, 100);

		// set gripper angle to position percent open
		int angle = (((abs(gripOpenAngle - gripClosedAngle)) * openPercent) / 100) + gripClosedAngle;
		gripper.write(angle);
		delay(700);
	}

#endif


//-----------------
// Backup with declared speed (100 max) and for a period of milliseconds.
// This function is intended to be used with the drive() function and expected
// with the gripper as a method to clear the gripper from objects.
// The flags for intersection detection are reset, 
// so the next drive() will begin moving forward again to find an intersection.
// Backup far enough to clear the intersection and use the turn() function
// if you do not want the robot to drive forward before turning around.
void GobbitLineCommand::backup(int speed, int delayTime)
{
	if(foundLeft || foundRight || foundEnd || foundForward){
		// reset all flags except leave End so the U drive command will still work without
		// forcing it to move the robot forward and find the next intersection.
		// this will work most of the time, but if the sensor was not backed up enough
		// to clear the intersection, if it is there, it may fail a U turn with the sensor
		// picking up on the intersection.
		
		// reset all flags
		resetIntFlags(1);
		// set end back to found
		foundEnd = 1;			
	}
	
	else{
		// reset all flags
		resetIntFlags(1);
	}	
	
	// go straight reverse
	setMotors(-abs(speed), -abs(speed));

	delay(delayTime);

	// stop the motors
	setMotors(0, 0);
}

//-----------------
// Sets the battery voltage which is used for choosing proper pd and motor tunings.
// This function does not read from any pins.
// 0 will force default of 9volt settings
void GobbitLineCommand::setBatteryVolts(float unreadVolts)
{

	volts = unreadVolts;
}

//-----------------
// Reads the battery voltage on declared analog pin and returns the voltage as a float value.
// Requires a voltage divider to avoid over voltage to the analog input.
// Analog Pin#
// Small Resistor size, resistor values in K ohms.
// Large Resistor size, resistor values in K ohms.
//    33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
//
//    CAUTION:  If using Lipo batteries, also use the checkBattery function to keep from running 
//              the battery too low and damaging your cells or causing a fire risk.  
//
//    ALWAYS   be careful if you are using Lithium batteries and do not rely on this or any
//             other software solution for absolute battery safety.
//
float GobbitLineCommand::readBatteryVolts(int analogPin, float smallResK, float largeResK)
{

	// set pin for battery reading
	pinMode(analogPin, INPUT);

	volts = analogRead(analogPin); // read voltage on analog pin
	volts = (volts * 5 / 1024) * ((smallResK + largeResK) / smallResK); // convert read steps x volts/steps of DAC (5/1024) by volts with (total divider resistance of large + small resister) to small resistor multiplier ratio
	//Serial.print(volts);
	//Serial.println("Battery Voltage");
	//Serial.println("");

	return volts;
}

//-----------------
// Checks the battery voltage by calling readBatteryVolts, then stops all if the voltage is 
//   less than minVoltage cutoff voltage.
// Requires a voltage divider to avoid over voltage to the analog input.
// Analog Pin#
// Cutoff minimum voltage.  Value range depends on your divider.
//    If using Lipo batteries, a value of 3.25 x (Number of cells) is a safe value for minVoltage to avoid 
//      low voltage battery damage.
//    Alkaline, NiCAD, and NmHd batteries could be set lower, or even 0 if there is to be no cutoff.
// Small Resistor size, resistor values in K ohms.
// Large Resistor size, resistor values in K ohms.
//    33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
//
//    CAUTION:  If using Lipo batteries, also use the checkBattery function to keep from running 
//              the battery too low and damaging your cells or causing a fire risk.  
//
//    ALWAYS   be careful if you are using Lithium batteries and do not rely on this or any
//             other software solution for absolute battery safety.
//
void GobbitLineCommand::checkBattery(int analogPin, float minVoltage, float smallResK, float largeResK)
{

	// if voltage is below cutoff voltage, flash the onboard LED on pin 13 fast and do not run anything else
	if (readBatteryVolts(analogPin, smallResK, largeResK) < minVoltage){
		while (1) // if using Lipo batteries, 3.25 per cell would be conservatively safe.
		{
			setMotors(0, 0);

			digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
			delay(200); // wait for a second
			digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
			delay(120); // wait for a second
			//Serial.println("LOW VOLTAGE HOLD!!!!");
			//Serial.print(volts);
			//Serial.println("Battery Voltage");
			//Serial.println("");
		}
	}
}

//-----------------
// Setup for the Sonar/obstacle avoidance sensor.
// Pin# (-1 disables)
// Safe Distance/range to maintain between sensor and obstacle.
//   Setting range to 8 is a good start with no gripper to allow speed adjustments
//   and obstacle avoidance in the followLine function, while 1000 would be so large it 
//   essentially disables any speed adjustments in follow mode, although that could be
//   useful in keeping the sonar enabled for other control logic.
void GobbitLineCommand::setSonar(int analogPin, float range)
{

	if (analogPin > -1) {
		// set pin mode for sonar pin
		pinMode(analogPin, INPUT);

		sonarPin = analogPin;

		safeDistanceInch = range;

		// enable sonar range finder in line following
		useRangeSensor = 1;
	}

	else
		useRangeSensor = 0;
}


//-----------------
// Read the sonar sensor from the set pin and returns the float current distance in inches.
// This will calculate the range/distance by multiplying the analog reading
// by the SONAR_CONVERSION_FACTOR defined in the config.h file.
//   NOTE: to use readSonarInches, the setSonar must have been called first.
float GobbitLineCommand::readSonarInches(void)
{
	if(sonarPin>-1){
	float sonarReading;
	sonarReading = analogRead(sonarPin);
	distanceInch = (sonarReading * SONAR_CONVERSION_FACTOR);
	
	beepCycle();
	
	return distanceInch;
	}
	else return 0;
}


//-----------------
// Sets the fine/basic/small kp, ki, and kd values
// used by PID motor control calculations.
void GobbitLineCommand::setPID(float kp, float ki, float kd)
{
	_kp = kp;
	_ki = ki;
	_kd = kd;
}

//-----------------
// Sets the coarse/fast/aggressive kp, ki, and kd values
// used by PID motor control calculations.
void GobbitLineCommand::setPIDcoarse(float kpC, float kiC, float kdC)
{
	_kpCoarse = kpC;
	_kiCoarse = kiC;
	_kdCoarse = kdC;
}

//-----------------
// Sets the range of the sensor from the middle to edge that the fine/small PID values should be used.
// Sensor readings that are in the range from this value to the edge of the sensor will use the 
// coarse/fast/aggressive PID values.
// PDfineRange/fraction/portion value from 0-1.0.  
//    0 will force the coarse/fast/aggressive PID active always
//    1 will leave the fine/basic/small PID always active.
void GobbitLineCommand::setPIDfineRange(float fineRange)
{
	PDfineRange = constrain(fineRange, 0, 1);
}

//-----------------
// Sets pins for the QTR-8RC sensor, if default are not to be used.
// All 8 sensors need to be used for this library
// MUST BE RUN BEFORE QTR IS INITIALIZED, OR BEFORE THE beginGobbit() is run in setup
void GobbitLineCommand::setQTRpins(unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4, unsigned char pin5, unsigned char pin6, unsigned char pin7, unsigned char pin8)  //(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8)
{

	unsigned char qtrPins[] = { pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8 };

	unsigned char i;
	for (i = 0; i < NUM_SENSORS; i++) {
		sensorPins[i] = qtrPins[i];
	}
}


//-----------------
// Sets the High turning speed when the turn or Drive command make the major part of the turn.
// High is used to make the larger portion of the turn quickly, 
// Setting the speeds too low will not allow the robot to make the turns at all.
// High speed value 0-100.  
void GobbitLineCommand::setTurnHighSpeed(int highTurn)
{
	turnSpeedHigh = constrain(highTurn,0,100);
}


//-----------------
// Sets the Low turning speed used by the turn, Drive, and end of the calibration commands.
// The Low speed is used to reduce turn speed and creep up on the final position more accurately.
// The low speed is also used as a reference in the turn function for controlling motors with PID, 
// although it is further constrained there as well.
// Setting the speeds too low may keep the robot from making full or timely turns.
// Low speed value 0-100.  
void GobbitLineCommand::setTurnLowSpeed(int lowTurn)
{
	turnSpeedLow = constrain(lowTurn,0,100);
}


//-----------------
// Sets the Minimum and Maximum drive() and followLine() speeds.  This does not currently affect the move() speeds.
// Min and Max speeds -100 to 100.
//   0 for both will lead to no movement.
//   Equal values for both will not allow any corrections by PID.
//   values > 0 for Minimum will make turns while following less sharp.
//   values 0 or less for Maximum may result in unpredictable actions
//     NOTE: When using less than 100 for Max, the robot may run smoother with Min
//           values closer to, or at 0, if PID values are not adjusted.
void GobbitLineCommand::setMinMaxSpeeds(int minimum, int maximum)
{
	minimum = constrain(minimum, -100, 100);

	maximum = constrain(maximum, -100, 100);

	minSpeed = minimum;
	maxSpeed = maximum;
}

//-----------------
// Sets the Brake speed.  This does not currently affect move().
// Brake speed can be from 1 to 100, but probably should not be lower than 60 for any voltage.
//   100 will disable braking.
void GobbitLineCommand::setBrakeSpeed(int bSpeed)
{
	brakeSpeed = constrain(bSpeed, 1, 100);
}

//-----------------
// Sets the Braking strength for the turn and drive functions.  This does not currently affect move().
// Brake strength is a percentage of the BRAKING_TIME milliseconds (see config.h).
// 0% to 200%.
// 	 0 will disable braking
//   100 is 100% of BRAKING_TIME milliseconds
//   200 is 200% of BRAKING_TIME milliseconds

void GobbitLineCommand::setBrakeStrength(int bStrength)
{
	brakeStrength = constrain(bStrength, 0, 200);
}

//-----------------
// Sets the custom Left Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// Ardumoto pins are set by default.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed.
void GobbitLineCommand::setLeftMotorPinsDirPWM(int dirPin, int pwmPin)
{

	// set pin variables
	dir_a = dirPin; //direction control for Left motor
	pwm_a = pwmPin; //PWM speed control for Left motor
}

//-----------------
// Sets the custom Right Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// Ardumoto pins are set by default.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed.
void GobbitLineCommand::setRightMotorPinsDirPWM(int dirPin, int pwmPin)
{

	// set pin variables
	dir_b = dirPin; //direction control for Right motor
	pwm_b = pwmPin; //PWM speed control for Right motor
}

//-----------------
// Serial prints all of the current settings and values.
// Intended as a diagnostic tool.
//   Due to the way the serial print is configured, this will use A LOT of
//   memory and may lead a larger program to run out of resources and crash the arduino.
void GobbitLineCommand::serialPrintCurrentSettings(void)
{
	Serial.begin(9600);

	Serial.println("THESE ARE YOUR CURRENT SETTINGS");
	Serial.println();

	if (useAFMS)
		Serial.println("Adafruit MotorShield");
	else {
		Serial.println("Ardumoto MotorShield or similar with Output Pins:");
		Serial.print(dir_a);
		Serial.println("   Left Motor Direction Pin");
		Serial.print(pwm_a);
		Serial.println("   Left Motor PWM/Speed Pin");
		Serial.print(dir_b);
		Serial.println("   Right Motor Direction Pin");
		Serial.print(pwm_b);
		Serial.println("   Right Motor PWM/Speed Pin");
	}
	Serial.println();

	Serial.println("QTR-8RC Input Pins");
	Serial.print(sensorPins[0]);
	Serial.print(", ");
	Serial.print(sensorPins[1]);
	Serial.print(", ");
	Serial.print(sensorPins[2]);
	Serial.print(", ");
	Serial.print(sensorPins[3]);
	Serial.print(", ");
	Serial.print(sensorPins[4]);
	Serial.print(", ");
	Serial.print(sensorPins[5]);
	Serial.print(", ");
	Serial.print(sensorPins[6]);
	Serial.print(", ");
	Serial.println(sensorPins[7]);
	Serial.println();

	if (useRangeSensor) {
		Serial.println("Sonar Range Finder Enabled.");
		Serial.print(sonarPin);
		Serial.println("   Input Pin");
		Serial.print(safeDistanceInch);
		Serial.println("   Inches");
	}
	else
		Serial.println("Sonar Range Finder is Disabled.");
	Serial.println();

	#if SERVO_ENABLE
		if (gripPin != NOT_SET) {
			Serial.println("Gripper is Enabled.");
			Serial.print(gripPin);
			Serial.println("   Input Pin");
			Serial.print(gripOpenAngle);
			Serial.println("   Open Angle");
			Serial.print(gripClosedAngle);
			Serial.println("   Closed Angle");
		}
		else
			Serial.println("Gripper is Disabled.");
		Serial.println();
	#endif
	
	
	if (beepPin != NOT_SET) {
		Serial.println("Beeper is enabled.");
		Serial.print(beepPin);
		Serial.println("   Output Pin");
	}
	else
		Serial.println("Beeper is Disabled.");
	Serial.println();

	Serial.print(volts);
	Serial.println("   Volts");
	Serial.println();

	Serial.print(_kp);
	Serial.println("   P fine variable");
	Serial.println();
	
	Serial.print(_ki);
	Serial.println("   I fine variable");
	Serial.println();

	Serial.print(_kd);
	Serial.println("   D fine variable");
	Serial.println();
	
	Serial.print(_kpCoarse);
	Serial.println("   P coarse variable");
	Serial.println();
	
	Serial.print(_kiCoarse);
	Serial.println("   I coarse variable");
	Serial.println();

	Serial.print(_kdCoarse);
	Serial.println("   D coarse variable");
	Serial.println();
	

	Serial.print(PDfineRange);
	Serial.println("   PD Fine Range of Sensor");
	Serial.println();

	Serial.print(_calSpeed);
	Serial.println("   DEFAULT Calibration Motor Speed, not value entered at calibrateLineSensor('value')");
	Serial.println();

	Serial.print(minSpeed);
	Serial.println("   Minimum Following and Drive Motor Speed");
	Serial.println();

	Serial.print(maxSpeed);
	Serial.println("   Maximum Following and Drive Motor Speed");
	Serial.println();

	Serial.print(brakeSpeed);
	Serial.println("   Brake Speed, if current max speed is less than this, will use motor braking at all Drive turns.");
	Serial.println();

	Serial.print(turnSpeedHigh);
	Serial.println("   High Motor Turn Speed for Drive Command");
	Serial.println();

	Serial.print(turnSpeedLow);
	Serial.println("   Low Motor Turn Speed for Drive Command");
	Serial.println();

}

//-----------------
// Set the speed and direction of the motors for both motor shield/driver types.
// Receives Left motor velocity/speed, Right motor velocity/speed.
//   Both are float values:
//    -100 full reverse
//    100 full forward
//    0 stopped
void GobbitLineCommand::setMotors(float leftVelocity, float rightVelocity)
{
	
	// DO NOT place beepCycle in this function. setMotors is called by beepCycle, therefore it would cause a cyclical crash.
	
	// Update current motor speed values for reference by other functions
	// Primarily used for braking decisions at this time
	RmotorSpeed = rightVelocity;
	LmotorSpeed = leftVelocity;
	
	// set speed with ratio of max possible for controller of 255 / max speed value
	leftVelocity = leftVelocity * 2.55;
	rightVelocity = rightVelocity * 2.55;

	//	limit velocities to 255 range of PWM
	leftVelocity = constrain(leftVelocity, -255, 255);
	rightVelocity = constrain(rightVelocity, -255, 255);

	// if Adafruit Motorshield v2.3 was defined
	// Set the speed to start, from 0 (off) to 255 (max speed)
	if (useAFMS) {
		// Left stop motor
		if (leftVelocity == 0) {
			leftMotor->setSpeed(0);
			leftMotor->run(FORWARD);
			//leftMotor->run(RELEASE);
		}

		// Left forward
		else if (leftVelocity > 0) {
			leftMotor->run(FORWARD);
			leftMotor->setSpeed(leftVelocity);
		}

		// Left Backward
		else if (leftVelocity < 0) {
			leftMotor->run(BACKWARD);
			leftMotor->setSpeed(abs(leftVelocity));
		}

		// Right stop motor
		if (rightVelocity == 0) {
			rightMotor->setSpeed(0);
			rightMotor->run(FORWARD);
		}

		// Right forward
		else if (rightVelocity > 0) {
			rightMotor->run(FORWARD);
			rightMotor->setSpeed(rightVelocity);
		}

		// Right Backward
		else if (rightVelocity < 0) {
			rightMotor->run(BACKWARD);
			rightMotor->setSpeed(abs(rightVelocity));
		}
	}

	// ardumoto or similar driver Dir and PWM based driver
	else {

		// Left stop motor
		if (leftVelocity == 0) {
			analogWrite(pwm_a, 0);
		}

		// Left forward
		else if (leftVelocity > 0) {
			digitalWrite(dir_a, LOW);
			analogWrite(pwm_a, leftVelocity);
		}

		// Left Backward
		else if (leftVelocity < 0) {
			digitalWrite(dir_a, HIGH);
			analogWrite(pwm_a, abs(leftVelocity));
		}

		// Right stop motor
		if (rightVelocity == 0) {
			analogWrite(pwm_b, 0);
		}

		// Right forward
		else if (rightVelocity > 0) {
			digitalWrite(dir_b, LOW);
			analogWrite(pwm_b, rightVelocity);
		}

		// Right Backward
		else if (rightVelocity < 0) {
			digitalWrite(dir_b, HIGH);
			analogWrite(pwm_b, abs(rightVelocity));
		}
	}

}



//-----------------
// Brake motors with Auto choice of strength and direction by a quick reversal of motors to stop motion.  
void GobbitLineCommand::brakeMotors(void)
{
	brakeMotors(100,'A');
}

	

//-----------------
// Brake motors expanded function by a quick reversal of motors to stop motion in the declared direction. 
// Receives an int value as a percentage of the BRAKING_TIME milliseconds, and direction of reversal as character 
//   0% to 200% percentage
//   'F'orward, 'B'ackward, 'R'ight, 'L'eft, 'A'uto ... direction is intended as the opposite of the current direction of motion
void GobbitLineCommand::brakeMotors(int bStrength,char direction)
{
	
	bStrength = constrain(bStrength, 0, 200);
	
	// adjust the amount of time the motors will be reversed by the bStrength (brake strength)
	int brakeTime = (BRAKING_TIME * bStrength)/100;

	// brake both motors

	if(direction == 'A'){

		// if turning/spinning left then set direction of braking to right
		if(LmotorSpeed<0  && RmotorSpeed>0)
			direction ='R';
		
		// if turning/spinning right then set direction of braking to the left
		else if(LmotorSpeed>0  && RmotorSpeed<0)
			direction ='L';

		
		// if moving in reverse then set direction of braking to forward
		else if(LmotorSpeed<0)
			direction ='F';
		
		// then must be moving forward then set direction of braking to backward
		else 
			direction ='B';
	}
	
	switch (direction){

		case 'R': // brake by reversing to the right
			setMotors(100, -100);
			
			// using half the time that is used for straight travels
			delay(brakeTime/2);	  
			break;
     
		case 'L': // brake by reversing to the left
			setMotors(-100, 100);
			
			// using half the time that is used for straight travels
			delay(brakeTime/2);	  
			break;
			
		case 'F': // brake by reversing forward
  			setMotors(100, 100);

			delay(brakeTime);
				
			// brake right motor a little longer
			//setMotors(0, 100);
			//delay(5);
			break;	

		case 'B': // brake by reversing backwards/reverse
  			setMotors(-100, -100);

			delay(brakeTime);
			
			// brake left motor a little longer
			setMotors(-100, 0);
			delay(5);
			break;

	}
	
/*  Prior version with only the auto mode and percent of brake time
	
	// if turning/spinning left
	if(LmotorSpeed<0  && RmotorSpeed>0){
		setMotors(100, -100);
		
		// using half the time that is used for straight travels
		delay(brakeTime/2);
	}
	
	// if turning/spinning right
	else if(LmotorSpeed>0  && RmotorSpeed<0){
		setMotors(-100, 100);
		
		// using half the time that is used for straight travels
		delay(brakeTime/2);
	}
	
	// if moving in reverse
	else if(LmotorSpeed<0){
		setMotors(100, 100);

		delay(brakeTime);
			
		// brake right motor a little longer
		setMotors(0, 100);
		delay(5);
	} 
	
	// then must be moving forward
  	else {
		setMotors(-100, -100);

		delay(brakeTime);
		
		// brake left motor a little longer
		setMotors(-100, 0);
		delay(5);
	}   */
	

	// stop both motors
	setMotors(0, 0);
}

//-----------------
// Sets the beeper pin#.
//   The beeper functionality is a simple HIGH/LOW output, intended for
//   activating a simple piezo style beeper/buzzer.  However, 
//   devices other than beepers could be controlled by this function.
void GobbitLineCommand::setBeeperPin(int pin)
{
	// set pin mode for piezo style beeper
	pinMode(pin, OUTPUT);

	// only used as flag and feedback for serialPrintCurrentSettings
	beepPin = pin; 
}

//-----------------
// Starts the beeper with values: 
//   Count of how many beeps,
//   Time in milliseconds for duration of beep with equal time between beeps,
//   Wait 0/1 if motors and all functions should be paused and wait till beeping stops.
// The beepCycle function completes the beep cycle and is called from other library 
//   functions to complete the beeps without delay if the Wait was called as 0.
void GobbitLineCommand::beep(unsigned int count, unsigned int length, byte wait)
{
	beepCount = count;
	beepLength = length;
	beepWait = wait;

	if (count && length) {
		mLastBeepRefTime = millis();
		digitalWrite(beepPin, HIGH);
		beepCycle();
	}
	// else may not be necessary
	else
		digitalWrite(beepPin, LOW);
	
}

//-----------------
// run beep cycle if the beeper is currently called.
void GobbitLineCommand::beepCycle(void)
{
	if (beepCount) {
		// if all motion is to stop and beep cycle is to complete before any further processing
		if (beepWait) {
			// stop motors
			setMotors(0, 0);

			// beep for count and length
			unsigned int i = 0;
			while (i < beepCount) {
				digitalWrite(beepPin, HIGH);
				delay(beepLength);
				digitalWrite(beepPin, LOW);
				i++;
				if (i < beepCount)
					delay(beepLength);
			}

			beep(0, 0, 0);
		}

		else {

			if (millis() - mLastBeepRefTime >= beepLength) {

				// if it is on, turn off, then it is done with that count of beeping so decrement count
				if (digitalRead(beepPin)) {
					digitalWrite(beepPin, LOW);
					beepCount--;
				}

				else
					digitalWrite(beepPin, HIGH);

				if (!beepCount) {
					beep(0, 0, 0);
				}

				else
					mLastBeepRefTime = millis();
			}
		}
	}

	//else just returns
}

//-----------------
// Detect Intersections and update flags
byte GobbitLineCommand::detectIntersection(void)
{
	// reset all flags
	resetIntFlags(1);
	
	// We use the inner six sensors (1 thru 6) to
	// determine if there is a line forward (straight ahead), and the
	// sensors 0 and 7 if the path turns.
	if (sensorValues[1] < 100 && sensorValues[2] < 100 && sensorValues[3] < 100 && sensorValues[4] < 100 && sensorValues[5] < 100 && sensorValues[6] < 100) {
		// There is no line visible ahead, and we didn't see any
		// intersection.  Must be a dead end.
		foundEnd = 1;
		
		// Set motors both to maxSpeed to allow braking to work properly in the event some followLine motor adjust
		// had last made on motor a negative correction value.
		setMotors(maxSpeed, maxSpeed);

		return 1;
	}

	else if (sensorValues[0] > 200 || sensorValues[7] > 200) {
		// Found an intersection.
		
		byte left = 0;
		byte right = 0;

		// Continue Driving forward while detecting what type of intersection.
		setMotors(maxSpeed, maxSpeed);

		// time value for checking if the detection has gone too long
		unsigned long mLastTime = millis();

		byte lookingRightLeft = 1;
		while (lookingRightLeft) {

			beepCycle();

			// Now read the sensors and check the intersection type.
			linePosition = qtrrc.readLine(sensorValues);

			// Check for left and right exits found.
			if (sensorValues[0] > 200 && !foundRight) {
				foundRight = 1;
				right = 1;
			}
			if (sensorValues[7] > 200 && !foundLeft) {
				foundLeft = 1;
				left = 1;
			}

			// check if sensor has passed the found lines
			if (sensorValues[0] < 200 && foundRight)
				right = 0;
			if (sensorValues[7] < 200 && foundLeft)
				left = 0;

			// if passed, exit loop
			if ((foundRight || foundLeft) && !right && !left){
				lookingRightLeft = 0;
				
				// if the time driving through intersection was greater than FIND_MARK_TIME, then a marker must have been found
				if (millis() - mLastTime > FIND_MARK_TIME) 
					foundMark = 1;
			}	

			// check if timed out and may be over running the course
			else if (millis() - mLastTime > FIND_INTERSECTION_MAXTIME) {
				setMotors(0, 0);
				while (1)
					;
			}
		}

		// check if line continues forward
		linePosition = qtrrc.readLine(sensorValues);
		if (sensorValues[1] > 200 || sensorValues[2] > 200 || sensorValues[3] > 200 || sensorValues[4] > 200 || sensorValues[5] > 200 || sensorValues[6] > 200)
			foundForward = 1;

		// check if at intersection with no forward option
		else if ((foundLeft || foundRight) && !foundForward) {
			// found tee
			foundEnd = 1;
		}
		
		return 1;
	}
	
	else{
		// No intersection found
		return 0;
	}
}

//-----------------
// Detect if a line is found in the called part of the QTR sensor, then return a byte of 1 if found, 0 if not.
// Sensor location value of far ('L')eft, far ('R')ight, the two in ('C')enter, or ('A')ny sensor.
byte GobbitLineCommand::detectLine(char sensorLRCA)
{
	// check if a line is present
	linePosition = qtrrc.readLine(sensorValues);
	
	beepCycle();

	switch (sensorLRCA) {

	// If the line is found on the far Left sensor
	case 'L':
		if (sensorValues[7] > 200) 
		return 1;
		break;
		
	// If the line is found on the far Right sensor
	case 'R':
		if (sensorValues[0] > 200) 
		return 1;
		break;
		
	// If the line is found in the middle 2 sensors
	// Will not use the outer sensors to try and find when more centered.
	case 'C':
		if (sensorValues[3] > 200 || sensorValues[4] > 200) 
		return 1;
		break;	
		
	// Look for the line if found on Any of the sensors
	case 'A':
		if (sensorValues[0] > 200 || sensorValues[1] > 200 || sensorValues[2] > 200 || sensorValues[3] > 200 || sensorValues[4] > 200 || sensorValues[5] > 200 || sensorValues[6] > 200 || sensorValues[7] > 200)
		return 1;
		break;	

	default:
		break;
	}

	// if not found 
	return 0;	

}


//-----------------
// Catch the Line by calling detectLine('A') until any sensor sees the line,
// then calling followLine(1) enough times to align the robot with the line.
// This assumes there was already some move or other motor command prior that 
// has the robot moving towards a line. 
// The function does not exit until it has caught the line.
void GobbitLineCommand::catchLine(void)
{
	// do nothing until the line is seen on any one of the sensors
	while(!detectLine('A'));

	for(int i=0; i<150; i++) followLine(1);
	
}


//-----------------
// Reset Intersection flags
void GobbitLineCommand::resetIntFlags(byte resetMark)
{
	if(resetMark) 
		foundMark = 0;

	foundLeft = 0;
	foundForward = 0;
	foundRight = 0;
	foundEnd = 0;
	brakeNext = 0;
	
}


//-----------------
// QTRtest serial program
// this has been moved within the library to simplify accessing a local QTRsensors library
void GobbitLineCommand::QTRtest(void)
{
  Serial.begin(115200);

  Serial.println("Gobbit Line Command");
  Serial.println("QTR line sensor Test");
  //Serial.print("Version ");
  //Serial.println(SKETCH_VERSION);
  Serial.println();

  delay(2500);

	while(1){
	// This will print the sensor numbers. If needed uncomment.
	//Serial.println("8 7 6 5 4 3 2 1 ");

	// read raw sensor values.
	qtrrc.read(sensorValues);

	// print indicators if the sensor sees the line
	for (int i = NUM_SENSORS - 1; i >= 0; i--)
	{
		if (sensorValues[i] > 600)
		{
			if (sensorValues[i] > 1000)
		{
		Serial.print("XX");
		}
		else Serial.print("--");
	}
	else Serial.print("__");
	}
	Serial.println();

	// This will print the raw sensor values. If needed, uncomment.
	//   for (int i = NUM_SENSORS-1; i >= 0; i--)
	//  {
	//    Serial.print(sensorValues[i]);
	//    Serial.print("  ");
	//  }
	//  Serial.println();

	delay(20);
	}
	
}
