/* 05/04/2018

  This program will setup the Gobbit robot to be controlled and programmable by an infrared (IR) remote.

  The Gobbit robot needs an arduino Uno, motor driver, line sensor, infrared receiver, and piezo beeper.
  The IR remote needs a minimum of 17 buttons.  Ideally, the buttons include a 10 key number pad with
  two extra buttons on either side of the 0 button, and the typical four directional arrows with an additional
  button in the middle for a stop button.  The robot will then be able to be controlled by the remote
  in three primary modes:
    1) Simple RC type control where the robot moves while the direction button is held down
    2) A direction/speed/time based programming mode where either individual or a series of
       direction, speed, and timing commands are entered and executed via the remote.
    3) Line following with intersection detection based programming mode where either individual or a series of
       direction, speed, and timing commands are entered and executed via the remote.
       A lined course is needed for this mode.
  There is also one minor mode:
    4) This mode is only used for "trimming" the motors (full forward and backward directions only) if needed
       for Mode 1&2 to function better.


  "IRremote" library is also needed.

    Use the IRremote example "IRrecvDemo" to find the IR codes from your remote.

    Make sure to set your RECV_PIN and turn on your IR_POWER_PIN if used.

    Update the #define IR button values in the tab "IRcodes.h" to match your remote's buttons
    (the tab is at the top of this text area)

  For GobbitLineCommand function definitions, see the GobbitLineCommand "GLC_Functions" example sketch.

  ** See comments within the code for further details **

*/

// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20
#define ADAFRUIT_MS

// What voltage is your battery (whole numbers only)
#define BATTERY_VOLTS 11

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

#define TRIM_MAX_TIME 4000 // maximumm time in milliseconds in Trim mode (4) to continue the last command without a new command

// set these values ONLY if using battery monitoring
// to disable, comment out the define ANALOG_DIVIDER_PIN
#define ANALOG_DIVIDER_PIN A3 // analog pin your voltage divider is connected
#define CUTOFF 9 // cut off voltage... set here at 3.0v x 3 cells = 9 
#define SMALL_RES_KOHMS 33 // K ohm value of your small resistor in the divider
#define LARGE_RES_KOHMS 100 // K ohm value of your small resistor in the divider

#include "IRcodes.h"

#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

#include <GobbitLineCommand.h>
GobbitLineCommand MyBot;


int mode = 0;
int moving = 0;
int lastButtonValue;
int currentSpeed = 100;
unsigned long previousMillis = 0;
int steerTrimF = 0; // steering trim forward motion
int steerTrimB = 0; // steering trim backward motion


void setup() {

  MyBot.setGripPinOpenClosed(10,180,75); 
  

  pinMode(PIEZO_PIN, OUTPUT);

  //  Serial.begin(9600);

  // If IR_POWER_PIN was defined, this will set the pin to power the IR receiver,
#ifdef IR_POWER_PIN
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);
#endif


  // Due to limited resources and motor shield pin usage, you need to shift some pins around to avoid PWM and timer
  // conflicts between IRremote and some motor driver shield pins.
  // This also requires using jumpers between the standard driver pins and the locations called here.
  //   NOTES:
  //      -More pins can be freed up if the motor shield is removed from the arduino and wired instead of stacked.
  //      -Since both A4 and A5 are being used, I2C cannot be used in this configuration since an ardumoto style shield is stacked due to wasted pins.
  //      -TO free up pins, use an I2C based driver like the Adafruit v2.

  // ArduMoto v14
#ifdef ARDUMOTO_14
  MyBot.setQTRpins(2, 4, 5, 6, 7, 8, A3, A4); // **
  MyBot.setLeftMotorPinsDirPWM(12, 9);  // on ardumoto v14, these were 12,3; use a jumper from 3 to 9
  MyBot.setRightMotorPinsDirPWM(13, 10);  // on ardumoto v14, these were 13,11; use a jumper from 11 to 10
#endif

  // ArduMoto v20
#ifdef ARDUMOTO_20
  MyBot.setQTRpins(5, 6, 7, 8, 12, 13, A3, A4); // **
  MyBot.setLeftMotorPinsDirPWM(4, 10);  // on ardumoto v20, these were 4,11; use a jumper from 11 to 10
  MyBot.setRightMotorPinsDirPWM(2, 9);  // on ardumoto v20, these were 2,3; use a jumper from 3 to 9
#endif

  // use these for 9v, not above
  // PID tunes here vary from the default due to pin changes and larger overhead of processor cycles.
  //  See the "ArdumotoDefaults.h" or the "AdafruitMSDefaults.h" files in the library "src" folder to view or change the default values.
#if (BATTERY_VOLTS < 10)
 // MyBot.setPID(0.25, 0.001, 1); // default at 9v is 0.15, .002, 411.1v is 0.15, 0.002, 4
 // MyBot.setPIDcoarse(0.3, 0.001, 4.4); // default at 11.1v is  0.2, 0.001, 4.4
 // MyBot.setPIDfineRange(0.6); // default at 11.1v is 0.05 //was .6 start fix
#endif

  MyBot.setBatteryVolts(BATTERY_VOLTS);

  MyBot.beginGobbit();  // initializes robot with settings

  MyBot.gripOpen();

  irrecv.enableIRIn(); // Start the IR receiver

#ifdef ANALOG_DIVIDER_PIN
  MyBot.checkBattery(ANALOG_DIVIDER_PIN, CUTOFF, SMALL_RES_KOHMS, LARGE_RES_KOHMS);
#endif

}

void loop() {

#ifdef ANALOG_DIVIDER_PIN
  MyBot.checkBattery(ANALOG_DIVIDER_PIN, CUTOFF, SMALL_RES_KOHMS, LARGE_RES_KOHMS);
#endif

  MyBot.move(0, 0);

  // wait for the mode selection to be received
  if (buttonDecode()) {
    mode = modeSelect();
  }

  // run in the selected mode
  if (mode) {
    switch (mode) {

      case 1:  // Start Simple RC Mode
        modeStartedBeep();
        modeOne();
        break;

      case 2:  // Start Free Programming Mode
        modeStartedBeep();
        modeTwo();
        break;

      case 3:  // Start Line Following Programming Mode
        modeStartedBeep();
        modeThree();
        break;

      case 4:  // Start Trim Mode
        modeStartedBeep();
        modeFour();
        break;
    }
  }
}











