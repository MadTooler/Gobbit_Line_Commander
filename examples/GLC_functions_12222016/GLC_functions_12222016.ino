// comments and instructions are found in GobbitLineCommand.h
// additional comments and cleanup are todo within this example.


// #define ADAFRUIT_MS if the Adafruit motor shield v2.3 is to be used.
  // M1 and M2 will be used.  Right motor on M1, Left on M2.
  // No define will default to Sparkfun Ardumoto or other makes of Dir 
  // and PWM pin driven style drivers similar to the Ardumoto and those
  // based upon the L298 driver.  The default pin settings will be used 
  // except where new values have been declared.
  // UnComment the next line if you are using the Adafruit shield
//#define ADAFRUIT_MS 

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  // Functions with "set" in the name should be called before the "beginGobbit" 


  MyBot.setGripPinOpenClosed(A0,180,68);  // attaches the servo on pin 14 which is A0 and sets the angles for Open and Closed Postions

  // open gripper
  MyBot.gripOpen();

  MyBot.gripPercentOpen(50);

  MyBot.setDriveTurnHighLowStoppoint(200, 100, 0.1);

   MyBot.setDriveMinMaxSpeeds(200, 100);
  
  MyBot.setQTRpins(2, 4, 5, 6, 7, 8, 9, 10);

  MyBot.setSonar(A1,8);

  MyBot.setBatteryVolts(9); 

  MyBot.readBatteryVolts(A5,33,100);
  
  MyBot.setPD(0.05, 1);
  
  MyBot.setFastPDmultiplier(4, 0.1);



  MyBot.setRightMotorPinsDirPWM(5, 5);  // sets the Right Motor driver pins for simple direction and PWM style drivers, such as L298 type.
  MyBot.setLeftMotorPinsDirPWM(5, 5);  // sets the Left Motor driver pins for simple direction and PWM style drivers, such as L298 type.
  
  MyBot.beginGobbit();// need min speed too???  make offcentersetpoint private and only through setdriveturnhighlow...???
  
  MyBot.serialPrintCurrentSettings();
  
  MyBot.calibrateLineSensor(55);
}


void loop() {

  // check the voltage of the battery
  MyBot.checkBattery(A5,3.25*2,33,100);

  // move command is better for direction control, but this allows
  // direct motor control if needed.
  // Must have run beginGobbit first.
  // left/rightVelocity -100 (full backwards) to +100 (full forward) where 0 means stop.
  MyBot.setMotors(100, 100); // this would be full speed forward on both left and right
  
  //Drive with F,B,R,L,U,S

  MyBot.drive('L');
  MyBot.drive('S');

  MyBot.brakeMotors();
 
  // close gripper
  MyBot.gripClose();
  
  MyBot.drive('U');
  MyBot.drive('R');
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();
  
  MyBot.backup(100,500);



}
