/*
*	GobbitLineCommand.h - library variables for the Gobbit robot
*	Library for line following, intersection detection, and basic motor control of Gobbit robot.
*	Created by Jason Talley 
*	Last edit 10/25/2018
*	Released under GNU agreement
*/

#ifndef config_h
#define config_h

	// time in milliseconds between processing of PID loop
	// set to 0 to revert back to initial version where processor cycle time was the delay
	#define PROCESS_TIME	0

	// line sensor defines/constants
	#define NUM_SENSORS   8     // number of sensors used
	#define TIME_OUT       2500  // waits for 2500 microseconds for sensor outputs to go low (2.5 milliseconds)
	#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // emitter control pin not used.  If added, replace QTR_NO_EMITTER_PIN with pin#
	
	// Cutoff time in milliseconds to stop sensing for intersection if the end of the intersection has not been found.
	// Used for safety cutoff if bot runs off edge of table or over dark surface and tries to run away.
	// default set high to allow for slow moving speeds or low voltage to fully clear intersections before the search is aborted.
	#define FIND_INTERSECTION_MAXTIME 1000
	
	// Time in milliseconds to evaluate if a marker on the course was found.
	// The marker needs to be a large black area, if using electrical tape, about 3 strips next to each other by wider than 
	// the sensor (total about 2" x 4-1/4").
	// If the robot is in lineFollow mode for detecting intersections, or in drive mode, and it runs over a mark for
	// period of time greater than this value, and passes back onto white background before the FIND_INTERSECTION_MAXTIME, then it 
	// will set the foundMark flag to true.  
	// This can be useful for non intersection indicators on a course, such as the end of a maze.
	// NOTE a marker will also set the foundLeft and foundRight to 1 as well.
	#define FIND_MARK_TIME	85
	
	// Time in milliseconds for brakeMotors to run in reverse
	// Around 30 is good.  Should be less than 60.
	#define BRAKING_TIME	30
	
	// Conversion multiplier to convert analog range reading of sonar sensor to inches
	// Current value is suitable for the MaxBotics EZ-0 and EZ-1 ultrasonic distance sensor on smooth surfaces for near collision detection.
	#define SONAR_CONVERSION_FACTOR 0.497
	
	// Enable use of Servo functions ("true" or 1 enables, "false" or 0 disables).
	// Initial purpose of setting "false" is for where the required TIMER1 that servo.h use could be in conflict with some other libraries.
	// Disabling will also conserve memory and compile time.
	#define SERVO_ENABLE true
	
#endif