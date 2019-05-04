/*
*	Gobbit Line Command functions and variables
*
*	GobbitLineCommand.h
*	Library for line following, intersection detection, and basic motor control of Gobbit robot.
*	Created by Jason Talley 
*	Last edit 05/04/2019
*	Released under GNU agreement
*/

/*
*   ** This is a reference list with notes, not an executable program. **
*
*   Functions and Variables are first all listed, then better explained further down.
*
*   Additional settings and information can be found in the library's "src" folder.
*
*   For assembly:  http://primalengineering.com/robots/Downloads/GBT-101-INSTR-R2_LineFollowAssemblyInstructions.PDF
*
*   For wiring:  http://primalengineering.com/robots/Downloads/Gobbit_LineFollowWiring_03222017.pdf
*
*   For additional programming, hardware, and other projects:  http://www.primalengineering.com/robots 
*
*
*	Parts list:
*
*		1) Gobbit Robot with line following configuration
*		    Components:
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


// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, it will default to Sparkfun Ardumoto v14 which is a Dir 
//   and PWM pin driver style driver based upon the L298 driver.  The default pin settings 
//   will be used except where new values have been declared.
// If the Adafruit motor shield v2.3 is to be used,
//   M1 and M2 will be used.  Right motor on M1, Left on M2.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

// include the library in the header of the sketch.
#include <GobbitLineCommand.h>

// Create an object of the GLC library class and give it a name in the header of the sketch.
// I called it MyBot here, but you can name yours whatever you want.
// All calls for library functions and variables will have to start with this name and "."
GobbitLineCommand MyBot;

void setup() {
// These functions should be called before the "beginGobbit" function.

MyBot.setQTRpins(2, 4, 5, 6, 7, 8, 9, 10); 
MyBot.setLeftMotorPinsDirPWM(12, 3);
MyBot.setRightMotorPinsDirPWM(13, 11);
MyBot.setBatteryVolts(9);

MyBot.beginGobbit();
}


void loop(void){
  
// Most of the functions can be called from anywhere, with a couple general exceptions
//   1) Motor related commands are not advisable from within interrupts.  Others may
//      also have unexpected results.
//   2) Most functions that do not have "set" in their name should NOT be called
//      before the "beginGobbit" function.
//
// The declared variable names and values are examples and for reference only.

MyBot.setTurnLowSpeed(25);
MyBot.setTurnHighSpeed(75);
MyBot.setMinMaxSpeeds(-100, 100);
MyBot.setPID(0.05, 0.001, 1);
MyBot.setPIDcoarse(0.05, 0.002, 1);
MyBot.setPIDfineRange(0.2);
MyBot.setBrakeStrength(100);
MyBot.setBrakeSpeed(80);
MyBot.setGripPinOpenClosed(A0,180,68);
MyBot.setSonar(A1,8);
MyBot.setBeeperPin(A0);
MyBot.serialPrintCurrentSettings();

float currentVoltage = MyBot.readBatteryVolts(A2,33,100);
MyBot.checkBattery(A2,3.25*2,33,100);
MyBot.calibrateLineSensor(0);
MyBot.followLine(0); 
MyBot.drive('L');
MyBot.turn('R'); 
MyBot.catchLine();
byte isItFound = MyBot.detectLine('L');
MyBot.move(100, 0); 
MyBot.setMotors(100, 100); 
MyBot.brakeMotors();
MyBot.brakeMotors(100, 'B');
MyBot.backup(100,500);
MyBot.gripOpen();
MyBot.gripClose();
MyBot.gripPercentOpen(50);
float distance = MyBot.readSonarInches();
MyBot.beep(4,200,0);

// variables
byte isItFound7 = MyBot.foundLeft;
byte isItFound8 = MyBot.foundRight;
byte isItFound9 = MyBot.foundForward;
byte isItFound10 = MyBot.foundEnd;
byte isItFound11 = MyBot.foundMark;

float lastDistanceReadBy_readSonarInches = MyBot.distanceInch;




// Detail for indivdual functions and variables


// Sets pins for the QTR-8RC sensor, if default are not to be used.
// All 8 sensors need to be used for this library
// MUST BE RUN BEFORE QTR IS INITIALIZED, OR BEFORE THE beginGobbit() is run in setup.
MyBot.setQTRpins(2, 4, 5, 6, 7, 8, 9, 10); 

// Sets the custom Left Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// Ardumoto pins are set by default.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed.
MyBot.setLeftMotorPinsDirPWM(12, 3);

// Sets the custom Right Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// Ardumoto pins are set by default.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed.
MyBot.setRightMotorPinsDirPWM(13, 11);

// Sets the battery voltage which is used for choosing proper pd and motor tunings.
// This function does not read from any pins.
// 0 will force default of 9volt setttings
MyBot.setBatteryVolts(9);

// Initializes the sensor, motors, and variables.
// All required settings for line following and motor control will be loaded with defaults for 9volt
// supply voltage, unless custom voltage or other values are declared.
// Add on hardware, such as the Gripper, Sonar, and Battery voltage divider, are not loaded here.  
// Call the function in the setup loop.  
// Call setQTRpins, setLeftMotorPinsDirPWM, setRightMotorPinsDirPWM, and setBatteryVolts
// functions before calling this function, if other than defaults are to be used.
MyBot.beginGobbit();

// Sets the Low turning speed used by the turn, Drive, and end of the calibration commands.
// The Low speed is used to reduce turn speed and creep up on the final position more accurately.
// The low speed is also used as a reference in the turn function for controlling motors with PID, 
// although it is further constrained there as well.
// Setting the speeds too low may keep the robot from making full or timely turns.
// Low speed value 0-100. 
MyBot.setTurnLowSpeed(25);

// Sets the High turning speed when the turn or Drive command make the major part of the turn.
// High is used to make the larger portion of the turn quickly, 
// Setting the speeds too low will not allow the robot to make the turns at all.
// High speed value 0-100.  
MyBot.setTurnHighSpeed(75);

// Sets the Minimum and Maximum drive() and followLine() speeds.  This does not currently affect the move() speeds.
// Min and Max speeds -100 to 100.
//   0 for both will lead to no movement.
//   Equal values for both will not allow any corrections by PID.
//   values > 0 for Minimum will make turns while following less sharp.
//   values 0 or less for Maximum may result in unpredictable actions
//     NOTE: When using less than 100 for Max, the robot may run smoother with Min
//           values closer to, or at 0, if PID values are not adjusted.
MyBot.setMinMaxSpeeds(-100, 100);

// Sets the fine/basic/small kp, ki, and kd values
// used by PID motor control calculations.
MyBot.setPID(0.05, 0.001, 1);

// Sets the coarse/fast/aggressive kp, ki, and kd values
// used by PID motor control calculations.
MyBot.setPIDcoarse(0.05, 0.002, 1);

// Sets the range of the sensor from the middle to edge that the fine/small PID values should be used.
// Sensor readings that are in the range from this value to the edge of the sensor will use the 
// coarse/fast/aggressive PID values.
// PDfineRange/fraction/portion value from 0-1.0.  
//    0 will force the coarse/fast/aggressive PID active always
//    1 will leave the fine/basic/small PID always active.
MyBot.setPIDfineRange(0.2);

// Sets the Braking strength for the turn and drive functions.  This does not currently affect the move().
// Brake strength is a percentage of the BRAKING_TIME milliseconds (see config.h).
// 0% to 200%.
// 	 0 will disable braking
//   100 is 100% of BRAKING_TIME milliseconds
//   200 is 200% of BRAKING_TIME milliseconds
MyBot.setBrakeStrength(100);

// Sets the Brake speed.  This does not currently affect move().
// Brake speed can be from 1 to 100, but probably should not be lower than 60 for any voltage.
//   100 will disable braking.
MyBot.setBrakeSpeed(80);

// Setup for the gripper servo.
// Pin#
// Open position in degrees, 0-180
// Closed position in degrees, 0-180
MyBot.setGripPinOpenClosed(A0,180,68);

// Setup for the Sonar/obstacle avoidance sensor.
// Pin# (-1 disables)
// Safe Distance/range to maintain between sensor and obstacle.
//   Setting range to 8 is a good start with no gripper to allow speed adjustments
//   and obstacle avoidance in the followLine function, while 1000 would be so large it 
//   essentially disables any speed adjustments in follow mode, although that could be
//   useful in keeping the sonar enabled for other control logic.
MyBot.setSonar(A1,8);

// Sets the beeper pin#.
//   The beeper functionality is a simple HIGH/LOW output, intended for
//   activating a simple piezo style beeper/buzzer.  However, 
//   devices other than beepers could be controlled by this function.
MyBot.setBeeperPin(A0);

// Serial prints all of the current settings and values.
// Intended as a diagnostic tool.
//   Due to the way the serial print is configured, this will use A LOT of
//   memory and may lead a larger program to run out of resources and crash the arduino.
MyBot.serialPrintCurrentSettings();

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
float currentVoltage1 = MyBot.readBatteryVolts(A2,33,100);


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
MyBot.checkBattery(A2,3.25*2,33,100);

// Run a calibration of the line sensor by sweeping back and forth 
// over a line at the called speed between 0-100.
//   Speed must be a 0 or positive value
//   0 will load the default values based upon voltage if declared.
MyBot.calibrateLineSensor(0);

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
//      everytime it is called
MyBot.followLine(0); 

// Drive will start driving/following the line and continue following until it is able to complete the requested 
//   direction at the next found intersection or end.  If it cannot make the requested direction, it will spin 
//   around fast and stop performing all other commands.
// Turn direction value of ('L')eft, ('R')ight, ('F')orward, ('S')top, or ('U')turn.
//    The U-turn will be evaluated by the found intersection turns available to achieve a 180 degree turn.
MyBot.drive('L');

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
MyBot.turn('R'); 

// Catch the Line by calling detectLine('A') until any sensor sees the line,
// then calling followLine(1) enough times to align the robot with the line.
// This assumes there was already some move or other motor command prior that 
// has the robot moving towards a line. 
// The function does not exit until it has caught the line.
MyBot.catchLine();

// Detect if a line is found in the called part of the QTR sensor, then return a byte of 1 if found, 0 if not.
// Sensor location value of far ('L')eft, far ('R')ight, the two in ('C')enter, or ('A')ny sensor.
byte isItFound6 = MyBot.detectLine('L');

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
MyBot.move(100, 0);

// Set the speed and direction of the motors for both motor shield/driver types.
// Receives Left motor velocity/speed, Right motor velocity/speed.
//   Both are float values:
//    -100 full reverse
//    100 full forward
//    0 stopped 
MyBot.setMotors(100, 100); 

// Brake motors basic function without any arguments. 
// Uses current motor directions to determine direction and make a quick reversal of motors to stop motion. 
MyBot.brakeMotors();

// Brake motors expanded function by a quick reversal of motors to stop motion in the declared direction. 
// Receives an int value as a percentage of the BRAKING_TIME milliseconds, and direction of reversal as character 
//   0% to 200% percentage
//   'F'orward, 'B'ackward, 'R'ight, 'L'eft, 'A'uto ... direction is intended as the opposite of the current direction of motion 
MyBot.brakeMotors(100,'B');
		
// Backup with declared speed (100 max) and for a period of milliseconds.
// This function is intended to be used with the drive() function and expected
// with the gripper as a method to clear the gripper from objects.
// The flags for intersection detection are reset, 
// so the next drive() will begin moving forward again to find an intersection.
// Backup far enough to clear the intersection and use the turn() function
// if you do not want the robot to drive forward before turning around.
MyBot.backup(100,500);

// Opens the gripper to the open angle that was declared with setGripPinOpenClosed
MyBot.gripOpen();

// Closes the gripper to the closed angle that was declared with setGripPinOpenClosed
MyBot.gripClose();

// Opens the gripper to the declared open percent
//   0 is same as closed
//   100 is same as fully open
MyBot.gripPercentOpen(50);

// Read the sonar sensor from the set pin and returns the float current distance in inches.
// This will calculate the range/distance by multiplying the analog reading
// by the SONAR_CONVERSION_FACTOR defined in the config.h file.
//   NOTE: to use readSonarInches, the setSonar must have been called first.
float currentDistance = MyBot.readSonarInches();

// Starts the beeper with values: 
//   Count of how many beeps,
//   Time in milliseconds for duration of beep with equal time between beeps,
//   Wait 0/1 if motors and all functions should be paused and wait till beeping stops.
// The beepCycle function completes the beep cycle and is called from other library 
//   functions to complete the beeps without delay if the Wait was called as 0.
MyBot.beep(4,200,0);


// variables

// These variables are byte "flags" to indicate if the robot has seen a line to the
// left, forward (straight ahead), or right.  Also "flags" to indicate if an 
// end or if a marker was found.
//    1 if found
//    0 if not found
// Calling the drive (not when drive('S')), move, turn, and backup will reset the flags.
byte isItFound1 = MyBot.foundLeft;
byte isItFound2 = MyBot.foundRight;
byte isItFound3 = MyBot.foundForward;
byte isItFound4 = MyBot.foundEnd;
byte isItFound5 = MyBot.foundMark;

// Sonar reading float variable in inches.
// The latest range in inches read by sonar sensor.  The drive(), followline(), and readSonarInches() 
// all update this value, but setSonar must have been called first.
float lastSonarDistanceRead = MyBot.distanceInch;

// QTRtest serial program
// this has been moved within the library to simplify accessing a local QTRsensors library
// use this with the example program to test the functionality of the QTRRC sensor in serial monitor
void GobbitLineCommand::QTRtest(void)
}
