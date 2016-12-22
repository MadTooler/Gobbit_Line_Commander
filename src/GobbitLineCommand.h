/*
 *  GobbitLineCommand.h - Library for line following, intersection detection, and basic motor control of Gobbit robot.
 *  Created by Jason Talley 
 *	Last edit 12/22/2016
 *  Released under GNU agreement
 */

/*
 *  PD Line Following for the Gobbit and Zagros Robotics Starter Kit with Pololu Line Sensor
 *
 *  For assembly, wiring, programming, and other examples, see: http://www.primalengineering.com/robots 
 *
 *
 *  Parts list:
 *
 *    1) Zagros Robot Starter Kit - Gobbit
 *       (http://www.zagrosrobotics.com/)
 *       Included components needed:
 *          Gobbit Chassis with motors and battery holder
 *          Sparkfun Redboard
 *          Sparkfun Ardumoto Shield
 *          Jumper wires
 *          Breadboard optional
 *
 *    2) Pololu QTR-8RC RC Reflectance Sensor Array
 *       (http://www.zagrosrobotics.com/shop/item.aspx?itemid=896)
 *
 *    3) Black electrical tape Line course on white background
 *
 *	  4) Batteries
 */


 
#ifndef GobbitLineCommand_h
#define GobbitLineCommand_h

#include <Arduino.h>

#include <Servo.h>

// QTRSensors folder must be placed in your arduino libraries folder
#include <QTRSensors.h>  // Pololu QTR Library 

	
// line sensor defines/constants
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // emitter control pin not used.  If added, replace QTR_NO_EMITTER_PIN with pin#


// #include <Adafruit_MotorShield.h> if it was defined in the main program
#if defined(ADAFRUIT_MS)
	//#include <Wire.h>
	#include <Adafruit_MotorShield.h>
	//#include "utility/Adafruit_MS_PWMServoDriver.h"
	#define USE_AFMS 1
#else 
	#define USE_AFMS 0
#endif


#define NOT_SET	9999 // indicator value that variable has not been set by main program

// default values speeds are based on +/-100 max
// 7.2 and 7.4v
#define SEVENV_KP	0.05
#define SEVENV_KD	0.5
#define SEVENV_MULTIPLIER_PD	5 
#define SEVENV_SMALL_RANGE_PD	0.04
#define SEVENV_CAL_SPD	50
#define SEVENV_MIN_SPD	-100
#define SEVENV_MAX_SPD	100
#define SEVENV_TURN_SPD_HIGH	85 
#define SEVENV_TURN_SPD_LOW	50
#define SEVENV_TURN_LOW_SET_POINT	0.25
//#define SEVENV_DRIVE_PAST_DELAY	130

// 9 and 9.6v not fully tested
#define NINEV_KP	0.04
#define NINEV_KD	1
#define NINEV_MULTIPLIER_PD	4 
#define NINEV_SMALL_RANGE_PD	0.08
#define NINEV_CAL_SPD	40
#define NINEV_MIN_SPD	-100
#define NINEV_MAX_SPD	100
#define NINEV_TURN_SPD_HIGH	70
#define NINEV_TURN_SPD_LOW	45
#define NINEV_TURN_LOW_SET_POINT	0.25
//#define NINEV_DRIVE_PAST_DELAY	100

// 11.1v needs re-testing
#define ELEVENV_KP	0.02
#define ELEVENV_KD	2
#define ELEVENV_MULTIPLIER_PD	4 
#define ELEVENV_SMALL_RANGE_PD	0.12
#define ELEVENV_CAL_SPD	30
#define ELEVENV_MIN_SPD	-100
#define ELEVENV_MAX_SPD	100
#define ELEVENV_TURN_SPD_HIGH	45 
#define ELEVENV_TURN_SPD_LOW	25
#define ELEVENV_TURN_LOW_SET_POINT	0.4
//#define ELEVENV_DRIVE_PAST_DELAY	30

// 12v not tested
#define TWELVEV_KP	0.02
#define TWELVEV_KD	2
#define TWELVEV_MULTIPLIER_PD	4 
#define TWELVEV_SMALL_RANGE_PD	0.12
#define TWELVEV_CAL_SPD	30
#define TWELVEV_MIN_SPD	-100
#define TWELVEV_MAX_SPD	100
#define TWELVEV_TURN_SPD_HIGH	35 
#define TWELVEV_TURN_SPD_LOW	25
#define TWELVEV_TURN_LOW_SET_POINT	0.4
//#define TWELVEV_DRIVE_PAST_DELAY	30


class GobbitLineCommand
{
  public: 

    // functions
	
	void setQTRpins(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8); // use to set qtr sensor pins if default Gobbit wiring will not be used
	void setRightMotorPinsDirPWM(int dirPin, int pwmPin);  // sets the Right Motor driver pins for simple direction and PWM style drivers, such as L298 type.
	void setLeftMotorPinsDirPWM(int dirPin, int pwmPin);  // sets the Left Motor driver pins for simple direction and PWM style drivers, such as L298 type.
	void setSonar(int analogPin, float range); // sets the Sonar/obstacle avoidance pin# (zero disables), and safe distance/range to maintain (8 is a good start with no gripper)
	void setGripPinOpenClosed(int pin, int open, int closed); // sets the gripper servo pin#, degree of open position, degree of closed position.
	void setPD(float kp, float kd); // sets the kp and kd values
	void setFastPDmultiplier(float multiplier, float smallRange);  // sets the fast/aggresive/large error kp and kd multiplier and fraction/percentage of sensor range from middle to edge that should engage the multplier when exceeded
	void setDriveMinMaxSpeeds(int minumum, int maximum);  // sets the Minimum and Maximum drive() speeds.  This does not currently affect the move() speeds.
	void setDriveTurnHighLowStoppoint(int highTurn, int lowTurn, float stopPoint); // sets the High and Low turning speeds when using Drive commands and the fraction/percentage of sensor range from middle to edge where the turning stops (useful since robot may continue swinging a little past the commanded stop).
	void setBatteryVolts(int unreadVolts);  // sets the battery voltage which is used for choosing proper pd and motor tunings, this function does not read anything from any pins.
	void beginGobbit(void);  // initializes the sensor.  Run in setup loop.  Run all "set" functions and declare variables before running this function.
	void calibrateLineSensor(int calSpeed); 
	void serialPrintCurrentSettings(void);  // serial prints all of the current settings and values. Do not run this in the main program loop because it will loop the restart or the serial connection.
	void followLine(); // simple line follow without any intersection detection
	void drive(char turn); // receives turn request and returns location/barcode value
	void move(float moveSpeed, float moveTurn); // simple moves without any line following. Typically used with delay statements as sensorless control.
	void setMotors(float leftVelocity, float rightVelocity); // direct motor control.  Must have run beginGobbit first.
	void brakeMotors(void);  // quick reversal of motors to stop fast forward motion.
	void backup(int speed, int delayTime); // backup with declared speed (100 max) and for a period of milliseconds	
	void gripClose(void); // closes the gripper to the declared closed angle
	void gripOpen(void); // opens the gripper to the declared open angle
	void gripPercentOpen(int openPercent);  // opens the gripper to the declared open percent. 0 is same as closed, 100 is same as fully open	
	float readBatteryVolts(int analogPin, float smallResK, float largeResK); // reads the battery voltage on declared analog pin, with voltage divider resistor values in K ohms
	void checkBattery(int analogPin, float minVoltage, float smallResK, float largeResK); // checks the battery voltage on declared analog pin, cutoff voltage, and resistor values in K for voltage divider

		
	// variables
	
	float offCenterSetPoint = NOT_SET; // 0-1, sets how much of sensor from middle to outer to stop the turn function while making drive intersection turns.  Can also be set through setDriveTurnHighLowStoppoint() as the stoppoint value.
	//int drivePastDelay = NOT_SET; // time in milleseconds to drive past intersection for aligning wheels over intersection before turn.  May not be needed if sensor based turning is created.
	
  private:
		
    // functions
    
	//void followLine();
    void turn(char turnDir);
    void noWayFreakOut();
    float speedAdjust(int currentDistance);
	
	
	// variables
	
	// QTR sensor variables
	// default is for sensors 0 through 7 to be connected to digital pins 2 through 10, respectively (pin 3 is skipped and used by the Ardumoto controller)
	// 0 is far Right sensor while 7 is far Left sensor
	unsigned int sensorValues[NUM_SENSORS]; // array with individual sensor reading values
	unsigned int linePosition = 0; // value from 0-7000 to indicate position of line between sensor 0 - 7
	unsigned char sensorPins[8]={2, 4, 5, 6, 7, 8, 9, 10}; // defualt values for Gobbit wiring
	
	// gripper angle limit default values
	int gripPin = NOT_SET;
	int gripClosedAngle = NOT_SET;
	int gripOpenAngle = NOT_SET;

	// Use the Adafruit Motor Shield flag
	byte useAFMS = USE_AFMS;
	
	// ArduMoto motor driver vars
	// pwm_a/b sets speed.  Value range is 0-255.  For example, if you set the speed at 100 then 100/255 = 39% duty cycle = slow
	// dir_a/b sets direction.  LOW is Forward, HIGH is Reverse
	int pwm_a = 3;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
	int pwm_b = 11;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor)
	int dir_a = 12;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
	int dir_b = 13;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)

	// pid loop vars
	float error = 0;
	float lastError = 0;
	float PV = 0 ;
	float _kp = NOT_SET; // good for 7.2v
	float _kd = NOT_SET;  // good for 7.2v
	float multiPD = NOT_SET;
	float smallRangePD = NOT_SET;
	int m1Speed = 0;
	int m2Speed = 0;
	int offTheLine = 0;

	// motor speed variables for line and intersection driving
	int _calSpeed = NOT_SET;
	int minSpeed = NOT_SET;
	int maxSpeed = NOT_SET;
	int turnSpeedHigh = NOT_SET;  // 150 with low 7.2 tune value motors will run while turning (0-255)
	int turnSpeedLow = NOT_SET;  // tune value motors will run as they slow down from turning cycle to avoid overrun (0-255)

	// These variables record whether the robot has seen a line to the
	// left, straight ahead, and right, while examining the current
	// intersection.
	bool findIntersection = false;
	unsigned char found_left = 0;
	unsigned char found_straight = 0;
	unsigned char found_right = 0;
	unsigned char found_end = 0;

	// obstacle avoidance variables
	int useRangeSensor = 0; // flag to apply sensor readings to drive and follow routines
	float DistanceInch = 0.00;
	int SonarPin;
	int sonarValue;
	float obstacleSpeedFactor = 1.0;
	float safeDistanceInch; // 8 works well if no gripper is on front of robot
	float gain = .5;
	
	// battery variables	
	float volts = NOT_SET; // battery voltage.  If the checkBattery() is in use with a voltage sensor/divider, this variable will be the read value.  Without, it can be set to engage default values for various voltage batteries.  If not declared, 9v is the assumed default.
    
};

#endif


