// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20
//#define ADAFRUIT_MS

// choose your lane with either 'R' for right or 'L' for left
#define LANE_CHOICE 'R'

// What voltage is your battery (whole numbers only)
#define BATTERY_VOLTS 9

// **** comment out define LEFT_LANE if using right lane
//#define LEFT_LANE

// If you will use an IO pin to power the IR receiver +5v
//   then declare the define to the pin number.
//   Otherwise, comment it at out.
// This is a shortcut for low mAmp power where you may have available IO pins and
// where the +5v pin is already used and you do not have a breadboard or splitter on hand.
#define IR_POWER_PIN A0  // comment out to disable

#define RECV_PIN A1 // pin your IR receiver is using

#define PIEZO_PIN A2  // piezo beeper/sensor pin

#define CALIBRATE_SPEED 0 // speed 0-100 for line follow calibration mode. 0 uses default values.

const int min_speed = -35; //  speed -100 to 100 for line follow motor speed PID. -100 is the default, but
                           //  a little higher might work better here

const int max_speed = 100; //  speed -100 to 100 for line follow motor speed PID. 100 is the default. 

#define FOLLOWLINE_MODE 1 // set followLine mode 0,1,2,3.  1 or 2 are probably best use here.

// set these values ONLY if using battery monitoring
// to disable, comment out the define ANALOG_DIVIDER_PIN
//#define ANALOG_DIVIDER_PIN A5 // analog pin your voltage divider is connected
#define CUTOFF 9 // cut off voltage... for example, for a 3 cell lithium, set at 3.0v x 3 cells = 9 
#define SMALL_RES_KOHMS 33 // K ohm value of your small resistor in the divider
#define LARGE_RES_KOHMS 100 // K ohm value of your small resistor in the divider


