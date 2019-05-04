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




#ifndef GobbitLineCommand_h
#define GobbitLineCommand_h

#include <Arduino.h>

// get other library default values
#include "config.h"

#if SERVO_ENABLE
	// will use servos
	#include <Servo.h>
#endif

// QTRSensors folder must be placed in your arduino libraries folder
//#include <QTRSensors.h>  // Pololu QTR Library 
#include "QTRSensors/QTRSensors.h" //**** use a local copy of Pololu QTR Library for temporary easier install

// #include <Adafruit_MotorShield.h> if it was defined in the main program
// NOTE the order which IDE compiles is not linear with the way it is written so conditional
// statements do not work when referencing defines from main sketch.  Therefore, the
// default values have to be called after.
#ifdef ADAFRUIT_MS
	// if using adafruit motor shield v2.3
	//#include <Adafruit_MotorShield.h>
	#include "Adafruit_Motor_Shield_V2_Library/Adafruit_MotorShield.h" //**** use a local copy of Adafruit Library for temporary easier install
	#define USE_AFMS 1
	//#include "AdafruitMSDefaults.h"	
#else 
	// if using ardumoto or L298 type motor driver
	#define USE_AFMS 0	
	//#include "ArdumotoDefaults.h"
#endif

// get the motor shield and voltage relative defaults.
#include "ArdumotoDefaults.h"
#include "AdafruitMSDefaults.h"



// indicator initial value used to indicate variable has not been set by main program
#define NOT_SET	9999 
	
class GobbitLineCommand
{
	public: 

		// functions
		
		void setQTRpins(unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4, unsigned char pin5, unsigned char pin6, unsigned char pin7, unsigned char pin8); // use to set qtr sensor pins if default Gobbit wiring will not be used
		void setRightMotorPinsDirPWM(int dirPin, int pwmPin);  // sets the Right Motor driver pins for simple direction and PWM style drivers, such as L298 type.
		void setLeftMotorPinsDirPWM(int dirPin, int pwmPin);  // sets the Left Motor driver pins for simple direction and PWM style drivers, such as L298 type.
		void setSonar(int analogPin, float range); // sets the Sonar/obstacle avoidance pin# (-1 disables), safe distance/range to maintain for obstacle avoidance (8 is a good start with no gripper while 1000 is so large it essentially disables any speed adjustments in follow mode)
		//void setGripPinOpenClosed(int pin, int open, int closed); // sets the gripper servo pin#, degree of open position, degree of closed position.
		void setPID(float kp, float ki, float kd); // sets the fine/basic/small kp, ki, and kd values
		void setPIDcoarse(float kpC, float kiC, float kdC); // sets the coarse/fast/aggressive kp, ki, and kd values
		void setPIDfineRange(float fineRange);  // Sets the range of the sensor from then middle to edge that the fine/small PID values should be used. PDfineRange/fraction/portion value from 0-1.0.
		void setMinMaxSpeeds(int minimum, int maximum);  // sets the Minimum and Maximum drive() and followLine() speeds.  This does not currently affect the move() speeds.
		void setBrakeSpeed(int bSpeed); // sets the Brake speed.  This does not currently affect the move().  Brake speed can be from 1 to 100, but probably should not be lower than 60 for any voltage.  100 will disable braking.
		void setBrakeStrength(int bStrength); // sets the Braking strength for the turn and drive functions.  This does not currently affect the move(). Brake strength is a percentage of the BRAKING_TIME milliseconds. 0% to 200%.
		void setTurnLowSpeed(int lowTurn); // sets the Low turning speeds when using turn or Drive commands, and the end of the calibrate.
		void setTurnHighSpeed(int highTurn); // sets the High turning speeds when using turn or Drive commands.
		void setBatteryVolts(float unreadVolts);  // sets the battery voltage which is used for choosing proper pd and motor tunings, this function does not read anything from any pins.
		void setBeeperPin(int pin);  // sets the beeper pin#.  The beeper functionality is assumed as a simple HIGH/LOW output, therefore, other devices may be used as alternatives.
		void beginGobbit(void);  // initializes the sensor.  Run in setup loop.  Run all "set" functions and declare variables before running this function.
		void calibrateLineSensor(void);  // basic auto mode that will call the advanced using the default calibration speed.
		void calibrateLineSensor(int calSpeed);
		void serialPrintCurrentSettings(void);  // serial prints all of the current settings and values. Do not run this in the main program loop because it will loop the restart or the serial connection.
		void followLine(byte followMode); // line follow with modes 0) follow with no return, 1) follow one motor adjust and return, 3) follow one motor adjust and check for intersection then return, 3) follow until an intersection is found then return
		void drive(char turn); // Drive receives ('L')eft, ('R')ight, ('F')orward, ('S')top, or ('U')turn request and runs followLine until it is able to complete the turn.
		void turn(char turnDir); // Turns to the sent variable of 'L' (left) or 'R' (right)
		byte detectLine(char sensorLRCA);  // Detect if a line is found and return 1 if found, 0 if not.
		void catchLine(void);  // Catch the Line and followLine enough to align to line. This assumes there was some move or other motor command prior.
		void move(float moveSpeed, float moveTurn); // simple moves without any line following. Typically used with delay statements as sensorless control.
		void setMotors(float leftVelocity, float rightVelocity); // direct motor control.  Must have run beginGobbit first.
		void brakeMotors(void);  // Brake motors without any arguments, Auto choice of strength and direction by a quick reversal of motors to stop motion. 
		void brakeMotors(int bStrength,char direction);  // Brake motors expanded function by a quick reversal of motors to stop motion in the declared direction.  Strength is a percentage of the BRAKING_TIME milliseconds. 0% to 200%, direction is 'F'orward, 'B'ackward, 'R'ight, 'L'eft, or 'A'uto and intended as the opposite of the current direction of motion.
		void backup(int speed, int delayTime); // backup with declared speed (100 max) and for a period of milliseconds	
		//void gripClose(void); // closes the gripper to the declared closed angle
		//void gripOpen(void); // opens the gripper to the declared open angle
		//void gripPercentOpen(int openPercent);  // opens the gripper to the declared open percent. 0 is same as closed, 100 is same as fully open	
		float readBatteryVolts(int analogPin, float smallResK, float largeResK); // reads the battery voltage on declared analog pin, with voltage divider resistor values in K ohms
		void checkBattery(int analogPin, float minVoltage, float smallResK, float largeResK); // checks the battery voltage on declared analog pin, cutoff voltage, and resistor values in K for voltage divider
		void beep(unsigned int count,unsigned int length, byte wait);  // set values for the beeper to be engaged within the main library loops.
		float readSonarInches(void); // Read the sonar sensor from the set pin and return the current distance in inches.
		void  QTRtest(void); // QTRtest serial program.  This has been moved within the library to simplify accessing a local QTRsensors library
		
		#if SERVO_ENABLE
			void setGripPinOpenClosed(int pin, int open, int closed); // sets the gripper servo pin#, degree of open position, degree of closed position.
			void gripClose(void); // closes the gripper to the declared closed angle
			void gripOpen(void); // opens the gripper to the declared open angle
			void gripPercentOpen(int openPercent);  // opens the gripper to the declared open percent. 0 is same as closed, 100 is same as fully open
		#endif
				
		// variables
			
		// These variables are byte "flags" to indicate if the robot has seen a line to the
		// left, forward (straight ahead), or right.  Also "flags" to indicate if an 
		// end or if a marker was found.
		//    1 if found
		//    0 if not found
		// Calling the drive (not when drive('S')), move, turn, and backup will reset the flags.
		byte foundLeft = 0;
		byte foundForward = 0;
		byte foundRight = 0;
		byte foundEnd = 0;
		byte foundMark = 0;
		
		// Sonar reading variable in inches
		// The latest range in inches read by sonar sensor.  The drive(), followline(), and readSonarInches() 
		// all update this value, but setSonar must have been called first.
		float distanceInch = 0.00;
		
		
	private:
		
		// functions
		
		void turnPID(void);
		void noWayFreakOut(void);
		float speedAdjust(float currentDistance);
		void beepCycle(void);
		byte detectIntersection(void); // Detect Intersections and update flags
		void resetIntFlags(byte resetMark); // Reset Intersection flags
		
		
		// variables
		
		// QTR sensor variables
		// default is for sensors 0 through 7 to be connected to digital pins 2 through 10, respectively (pin 3 is skipped and used by the Ardumoto controller)
		// 0 is far Right sensor while 7 is far Left sensor
		unsigned int sensorValues[NUM_SENSORS]; // array with individual sensor reading values
		unsigned int linePosition = 0; // value from 0-7000 to indicate position of line between sensor 0 - 7
		unsigned int pastLinePosition = 0;  // value from 0-7000 to indicate position of line between sensor 0 - 7, used for Operation Flux Capacitor
		// **** moved pin defines to #ifdef's for motor driver options
		//unsigned char sensorPins[8]={2, 4, 5, 6, 7, 8, 9, 10}; // default values for Gobbit wiring
		
		#if SERVO_ENABLE
			// gripper angle limit default values
			int gripPin = NOT_SET;
			int gripClosedAngle = NOT_SET;
			int gripOpenAngle = NOT_SET;
		#endif
		


		// ****
		// set pin values if a standard motor driver was defined
		#ifdef ADAFRUIT_MS
			// if using adafruit motor shield v2.3 set the pins
			unsigned char sensorPins[8]={AFV23_QTR0, AFV23_QTR1, AFV23_QTR2, AFV23_QTR3, AFV23_QTR4, AFV23_QTR5, AFV23_QTR6, AFV23_QTR7}; // default values for Gobbit wiring
			
			// ***** these will not be used when adafruit driver is declared, only keeping in for now until confirm missing does not break anything
			int dir_a = 12;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
			int pwm_a = 3;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
			int dir_b = 13;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)
			int pwm_b = 11;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor)
			
 		#elif defined ARDUMOTO_14
			unsigned char sensorPins[8]={AMV14_QTR0, AMV14_QTR1, AMV14_QTR2, AMV14_QTR3, AMV14_QTR4, AMV14_QTR5, AMV14_QTR6, AMV14_QTR7}; // default values for Gobbit wiring
			int dir_a = AMV14_DIRA;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
			int pwm_a = AMV14_PWMA;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
			int dir_b = AMV14_DIRB;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)
			int pwm_b = AMV14_PWMB;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor) 

		#elif defined ARDUMOTO_20
			unsigned char sensorPins[8]={AMV20_QTR0, AMV20_QTR1, AMV20_QTR2, AMV20_QTR3, AMV20_QTR4, AMV20_QTR5, AMV20_QTR6, AMV20_QTR7}; // default values for Gobbit wiring
			int dir_a = AMV20_DIRA;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
			int pwm_a = AMV20_PWMA;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
			int dir_b = AMV20_DIRB;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)
			int pwm_b = AMV20_PWMB;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor)
		
		#else
			unsigned char sensorPins[8]={2, 4, 5, 6, 7, 8, 9, 10}; // default values for Gobbit wiring
			// for default if nothing was declared, use original ArduMoto motor driver vars
			// dir_a/b sets direction.  LOW is Forward, HIGH is Reverse
			// pwm_a/b sets speed.  Value range is 0-255.  For example, if you set the speed at 100 then 100/255 = 39% duty cycle = slow
			int dir_a = 12;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
			int pwm_a = 3;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
			int dir_b = 13;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)
			int pwm_b = 11;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor) 
		#endif

		
		// Use the Adafruit Motor Shield flag
		// This is a work around
		byte useAFMS = USE_AFMS;	
		

		// pid loop vars
		unsigned long mLastTime;
		unsigned long mSampleTime = PROCESS_TIME;
		float error = 0;
		float lastError = 0;
		float iAccumError = 0;	
		float PV = 0 ;
		float _kp = NOT_SET;
		float _ki = NOT_SET;
		float _kd = NOT_SET; 
		float _kpCoarse = NOT_SET;
		float _kiCoarse = NOT_SET;
		float _kdCoarse = NOT_SET;	
		float PDfineRange = NOT_SET;
		float RmotorSpeed = 0;
		float LmotorSpeed = 0;

		// motor speed variables for line and intersection driving
		int _calSpeed = NOT_SET;
		int minSpeed = NOT_SET;
		int maxSpeed = NOT_SET;
		int turnSpeedHigh = NOT_SET;
		int turnSpeedLow = NOT_SET;
		int brakeSpeed = NOT_SET;
		int brakeStrength = NOT_SET;
		byte brakeNext = 0;

		// obstacle avoidance variables
		byte useRangeSensor = 0; // flag to apply sensor readings to drive and follow routines
		int sonarPin = -1;
		float obstacleSpeedFactor = 1.0;
		float safeDistanceInch; // 8 works well if no gripper is on front of robot
		float gain = .5;
		
		// look for intersection flag
		bool findIntersection = false;
		
		// battery variables	
		float volts = NOT_SET; // battery voltage.  If the checkBattery() is in use with a voltage sensor/divider, this variable will be the read value.  Without, it can be set to engage default values for various voltage batteries.  If not declared, 9v is the assumed default.
		
		// beeper variables
		unsigned int beepPin = NOT_SET;
		unsigned long mLastBeepRefTime = 0;
		unsigned int beepCount = 0; // number of beeps for piezo style beeper
		unsigned int beepLength = 0; // length in milliseconds of beep and the delay between beeps if beepCount > 1
		byte beepWait = 0; // set to 1 if the motors should be stopped and all other actions should be on hold until beeping is completed
	
	};

#endif


