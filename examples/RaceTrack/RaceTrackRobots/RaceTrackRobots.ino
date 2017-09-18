/* 09/18/2017

  This program is intended for the Gobbit robot with a line sensor and infrared receiver to be used
  on a lined course with either a scoreboard start/finish line or simple IR remote control for both
  staging robots at the start line, telling them when to start, and when to stop.

  If a scoreboard is not used with the RaceTrackScoreBoard program, then change the IR codes to match 
  those on an IR remote, such as a tv or cable remote.
  
  "IRremote" library is also needed.

  For GobbitLineCommand function definitions, see the GobbitLineCommand "GLC_Functions" example sketch.

  ** Further documentation is not currently available. **
    
  It may be better to revise and use a shorter IR code format for improved speed and timing.
  
*/

//#define ADAFRUIT_MS // uncomment this line if using the Adafruit Motor Shield v2.3 with the GobbitLineCommand library 

// Servos should be used on pins 9 or 10 if needed
// PID settings may be a little off since this will use the higher frequency PWM is used on pins 5 & 6
// to keep all functions availble for IR, Motor PWM, and Servos.

// comment out define LEFT_LANE if using right lane
#define LEFT_LANE

#define FOLLOWLINE_MODE 1 // set followLine mode 0,1,2,3.  1 or 2 are probably best use here.

#define RECV_PIN A0

// IR command values are unique to this setup.  
//   While these are currently sent and recieved in the  NEC format of IR is used for now, 
//   it may be best to switch to a shorter/smaller bit value code for faster communication.

#ifdef LEFT_LANE
  // Left lane IR commands
  #define STOP 0x2BB27000
  #define BACKUP 0x2BB27002
  #define BUMP_IN 0x2BB27003
#else   
  // Right lane IR commands
  #define STOP 0x2BB28000
  #define BACKUP 0x2BB28002
  #define BUMP_IN 0x2BB28003
#endif

// Common IR commands
#define GO_RACE 0x2BB26001
#define ALL_STOP 0x2BB29999


#include <IRremote.h>
IRrecv irrecv(RECV_PIN);
decode_results results;

#include <GobbitLineCommand.h>
GobbitLineCommand MyBot;

int go = -1;
byte moving = 0;
void setup() {

  // most of the setup notes and calls are specifically for the Gobbit robot with other possible configurations in mind, and the GobbitLineCommand library usage

  MyBot.setMinMaxSpeeds(-35, 100);

  // PID tunes here should be uncommmented if the default values are to be improved for course specific.
  //  See the "ArdumotoDefaults.h" or the "AdafruitMSDefaults.h" files in the library "src" folder to view or change the default values.
  // MyBot.setPID(0.1, 0.00, 2); // default at 11.1v is 0.15, 0.002, 4
  // MyBot.setPIDcoarse(0.2, 0.001, 4.4); // default at 11.1v is  0.2, 0.001, 4.4
  // MyBot.setPIDfineRange(0.6); // default at 11.1v is 0.05

  // This keeps both pin 9 and 10 free for servos, but uses A4 so I2C cannot be used.
  // The main down side is some tune values and motor speeds may need to be changed due to the difference in PWM frequency on pins 5 & 6.
  //   NOTE: More pins can be freed up if the motor shield is removed from the arduino and wired instead of stacked.
  MyBot.setQTRpins(2, 4, 7, 8, A1, A2, A3, A4);  
  MyBot.setLeftMotorPinsDirPWM(12, 5);
  MyBot.setRightMotorPinsDirPWM(13, 6);
  
  // OR, if a servo will not be needed, you can use PWM on pins 9 & 10 to keep most tunes and speeds good as default.
  //MyBot.setQTRpins(2, 4, 5, 6, 7, 8, A1, A2);
  //MyBot.setLeftMotorPinsDirPWM(12, 9);
  //MyBot.setRightMotorPinsDirPWM(13, 10); //MyBot.setRightMotorPinsDirPWM(A1, 10);

  MyBot.setBatteryVolts(11.1);

  MyBot.beginGobbit();  // initializes robot with settings

  MyBot.calibrateLineSensor(40);  // probably can leave passed value empty/default if using PWM on pins 9 and 10
  
  irrecv.enableIRIn(); // Start the IR receiver

}

void loop() {

  if (irrecv.decode(&results)) { // this line triggers the beginning of the trouble. if no signal is recieved, the problem does not start.
     
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
