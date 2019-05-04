/* 05/04/2019

  This program will setup the Gobbit robot to be controlled and programmable by an infrared (IR) remote.

    See videos and project directions : http://www.primalengineering.com/robots/irprogram.html

    ** See the Config.h tab for hardware and other settings **
        (the tabs are at the top of this text area)

    "IRremote" library is also needed.  Make sure it is installed through the Library Manager

  The Gobbit robot needs an arduino Uno, motor driver, line sensor, infrared receiver, and piezo beeper (element).
  The IR remote needs a minimum of 17 buttons.  Ideally, the buttons include a 10 key number pad with
  two extra buttons on either side of the 0 button, and the typical four directional arrows with an additional
  button in the middle for a stop button.  The robot will then be able to be controlled by the remote
  in three primary modes (see tabs m1, m2, m3, m4, & m5):
    1) Simple RC type control where the robot moves while the direction button is held down
    2) A direction/speed/time based programming mode where either individual or a series of
       direction, speed, and timing commands are entered and executed via the remote.  This is also
       known as "Dead Reckoning."
    3) Line following with intersection detection based programming mode where either individual or a series of
       direction, speed, and timing commands are entered and executed via the remote.
       A lined course is needed for this mode.
    4) This mode is a minor Trimming mode for mode 1 & 2.  It is only used for "trimming" the motors (full forward and
       backward directions only) if needed for Mode 1 & 2 to function better.
    5) Tiled line solving mode.  This receives IR number commands only which call, in the order entered,
       the matching functions in the Tiles, tab to navigate the tiled path, regardless of which end of the
       tiles the robot enters first.


    Use the example program to find the IR codes from your remote:
      GobbitLineCommand->ScoutBotics->Level_2->GLC_GetIRcodes

        In GLC_IRgetCodes, Make sure to set your RECV_PIN and your IR_POWER_PIN if used.

      Update the #define IR button values here in the tab "IRcodes.h" to match your remote's buttons
      (the tabs are at the top of this text area)

      There are examples of some IR remote control codes in the various tabs of the example
        GobbitLineCommand->ScoutBotics->Level_2->GLC_SampleIRcodes

  For GobbitLineCommand function definitions, see the GobbitLineCommand "GLC_Functions" example sketch.

  ** See comments within the code and modes (m1-5 tabs) for further details **
*/

#include "Config.h"
#include "IRcodes.h"

#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

#include <GobbitLineCommand.h>
GobbitLineCommand MyBot;


int mode = 0;
int moving = 0;
byte sensorCalibrated = 0;
int lastButtonValue;
int currentSpeed = 100;
unsigned long previousMillis = 0;
int steerTrimF = 0; // steering trim forward motion
int steerTrimB = 0; // steering trim backward motion


void setup() {

#ifdef GRIPPER_PIN
  MyBot.setGripPinOpenClosed(GRIPPER_PIN, GRIPPER_OPEN, GRIPPER_CLOSED);
#endif

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
  // PID tunes here vary from the default due to pin changes and larger overhead timing of processor cycles.
  //  See the "ArdumotoDefaults.h" or the "AdafruitMSDefaults.h" files in the library "src" folder to view or change the default values.
#if (BATTERY_VOLTS < 10)
  MyBot.setPID(0.25, 0.001, 1); // default at 9v is 0.15, .002, 411.1v is 0.15, 0.002, 4
  MyBot.setPIDcoarse(0.3, 0.001, 4.4); // default at 9v is  0.2, 0.001, 3.4
  MyBot.setPIDfineRange(0.6); // default at 9v is 0.04
#endif

  MyBot.setBatteryVolts(BATTERY_VOLTS);

  MyBot.beginGobbit();  // initializes robot with settings

#ifdef GRIPPER_PIN
  MyBot.gripOpen();
#endif

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

      case 5:  // Start Tile Mode
        modeStartedBeep();
        modeFive();
        break;
    }
  }
}
