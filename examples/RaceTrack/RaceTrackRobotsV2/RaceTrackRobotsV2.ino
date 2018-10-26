/* 10/25/2018

  This program is intended for the Gobbit robot with a line sensor and infrared receiver to be used
  on a lined course with either a scoreboard start/finish line or simple IR remote control for both
  staging robots at the start line, telling them when to start, and when to stop.

    NOTE: The wiring of the Gobbit here matches the wiring used in the ScoutBotics Level 2 IRcontrol example.

          SEE Config.h TAB TO CONFIRM SETTINGS AND LANE BEFORE UPLOADING

          SEE IRcodes.h if you need to change the working IR command codes

  If a scoreboard is not used with the RaceTrackScoreBoard program, then change the IR codes to match 
  those on an IR remote, such as a tv or cable remote.
  
  "IRremote" library is also needed.

  For GobbitLineCommand function definitions, see the GobbitLineCommand "GLC_Functions" example sketch.

  ** Further documentation is not currently available. **
    
  It may be better to revise and use a shorter IR code format for improved speed and timing.
  
*/

#include "Config.h"
#include "IRcodes.h"

#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

#include <GobbitLineCommand.h>
GobbitLineCommand MyBot;

int go = -1;
byte moving = 0;
void setup() {

  // most of the setup notes and calls are specifically for the Gobbit robot with other possible configurations in mind, and the GobbitLineCommand library usage

  MyBot.setMinMaxSpeeds(min_speed, max_speed);

  // If IR_POWER_PIN was defined, this will set the pin to power the IR receiver,
#ifdef IR_POWER_PIN
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);
#endif

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

#if (BATTERY_VOLTS < 10)
  // use these for 9v, not above
  // PID tunes here vary from the default due to pin changes and larger overhead timing of processor cycles.
  //  See the "ArdumotoDefaults.h" or the "AdafruitMSDefaults.h" files in the library "src" folder to view or change the default values.
  MyBot.setPID(0.1, 0.00, 2); // default at 9v is 0.15, .002, 411.1v is 0.15, 0.002, 4
  MyBot.setPIDcoarse(0.3, 0.001, 4.4); // default at 9v is  0.2, 0.001, 3.4
  MyBot.setPIDfineRange(0.6); // default at 9v is 0.04
#endif

#if (BATTERY_VOLTS > 9)
  // PID tunes here should be uncommented if the default values are to be improved for course specific.
  //  See the "ArdumotoDefaults.h" or the "AdafruitMSDefaults.h" files in the library "src" folder to view or change the default values.
  MyBot.setPID(0.1, 0.00, 2); // default at 11.1v is 0.15, 0.002, 4
  MyBot.setPIDcoarse(0.2, 0.001, 4.4); // default at 11.1v is  0.2, 0.001, 4.4
  MyBot.setPIDfineRange(0.6); // default at 11.1v is 0.05
#endif

  MyBot.setBatteryVolts(BATTERY_VOLTS);

  MyBot.beginGobbit();  // initializes robot with settings

  MyBot.calibrateLineSensor(CALIBRATE_SPEED);  // calibrate the sensor
  
  irrecv.enableIRIn(); // Start the IR receiver

#ifdef ANALOG_DIVIDER_PIN
  MyBot.checkBattery(ANALOG_DIVIDER_PIN, CUTOFF, SMALL_RES_KOHMS, LARGE_RES_KOHMS);
#endif

}

void loop() {

#ifdef ANALOG_DIVIDER_PIN
  // check battery status, if enabled, and halt the program if the voltage is too low
  MyBot.checkBattery(ANALOG_DIVIDER_PIN, CUTOFF, SMALL_RES_KOHMS, LARGE_RES_KOHMS);
#endif

  if (irrecv.decode(&results)) { // this line triggers the beginning of the fun. if no signal is received, nothing will start.
     
     switch (results.value){

      case STOP: // stop ir command
        //if (go == -1) MyBot.setMinMaxSpeeds(-100, 100);  // reset if initial approach was set slower
        go = 0;
        break;

      case ALL_STOP: // All stop ir command.  Different from stop because all lanes should stop.
        go = 0;
        break;
     
      case GO_RACE:  // go race ir command
        go = 1;
        break;

      case BACKUP: // stop ir command
        go = 2;
        break;

      case BUMP_IN: // stop ir command
        go = 3;
        break;
     }
       
    irrecv.resume(); // Receive the next value
  }

  switch (go) {
    
    case -1:  // initial approach to start line after calibration.  Change this area if manual start will be used.
      // MyBot.setMinMaxSpeeds(-70, 70);
      MyBot.followLine(FOLLOWLINE_MODE);
      moving=1;
      break;

    case 0:  // stop with quick motor reversal for reduced overrun
      if(moving){
        MyBot.brakeMotors(200,'B');
        moving=0;
      }
      break;

    case 1:  // race begins
      MyBot.followLine(FOLLOWLINE_MODE);
      moving=1;
      break; 

    case 2:  // backup until told to stop
      MyBot.move(-42,0);
      break;

    case 3:  // bump in with delay until told to stop
      MyBot.brakeMotors(35,'F'); // much less than 100% of brake time needed.  35% seems good for higher voltage setups.
      delay(350);  // give the scoreboard time to react and send a stop message
      break;
  }

}
