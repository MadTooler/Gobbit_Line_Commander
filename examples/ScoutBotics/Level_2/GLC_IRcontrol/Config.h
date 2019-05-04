// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20
//#define ADAFRUIT_MS

// What voltage is your battery (whole numbers only)
#define BATTERY_VOLTS 9

// If you will use an IO pin to power the IR receiver +5v
//   then declare the define to the pin number.
//   Otherwise, comment it at out.
// This is a shortcut for low mAmp power where you may have available IO pins and
// where the +5v pin is already used and you do not have a breadboard or splitter on hand.
#define IR_POWER_PIN A0  // comment out to disable

#define RECV_PIN A1 // pin your IR receiver is using

#define PIEZO_PIN A2  // piezo beeper/sensor pin

// veering turn values have stronger turning with larger value, max 99 where 100 is spinning
// Used with Mode 1 and 2 only
#define MINOR_VEER_STRENGTH 30
#define MODERATE_VEER_STRENGTH 60  // used for FORWARD_RIGHT/LEFT turns/veering
#define MAJOR_VEER_STRENGTH 90

#define CALIBRATE_SPEED 0 // speed 0-100 for line follow calibration mode. 0 uses default values.

#define RC_SPINSPEED 45 // speed 0-100 for right/left turn/spin in Mode 1 only. 

#define BUTTON_HOLD_DELAY  110  // (was 110) delay for hold down repeat to continue last command to allow for time between IR transmissions

#define DEBOUNCE_DELAY 250 // debounce time to allow finger off of key

#define TRIM_MAX_TIME 4000 // maximum time in milliseconds in Trim mode (4) to continue the last command without a new command


// set these values ONLY if using battery monitoring
// to disable, comment out the define ANALOG_DIVIDER_PIN
//#define ANALOG_DIVIDER_PIN A5 // analog pin your voltage divider is connected
#define CUTOFF 9 // cut off voltage... for example, for a 3 cell lithium, set at 3.0v x 3 cells = 9 
#define SMALL_RES_KOHMS 33 // K ohm value of your small resistor in the divider
#define LARGE_RES_KOHMS 100 // K ohm value of your small resistor in the divider

// set these values ONLY if using the gripper
// to disable and conserve memory, comment out #define GRIPPER_PIN
//#define GRIPPER_PIN A5
#define GRIPPER_OPEN 180   // degree of gripper servo where gripper is open
#define GRIPPER_CLOSED 75  // degree of gripper servo where gripper is closed
