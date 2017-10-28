/* 10/28/2017
 *  
 *  Using the Gobbit robot with line sensor and gripper on pin A0, this program will...
 *  
 *   1) Drive Forward along a line
 *   2) Stop at the end of the line (or any found intersection)
 *   3) Close the gripper to grab something that was placed there
 *   4) Turn around and drive the other direction
 *   5) Stop at the end of the line (or any found intersection)
 *   6) Open the gripper to release the something
 *   7) Backup a little away from the something
 *   8) Wait there forever
 *   
*/

// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {
  
  // load default tuning values for battery voltage 
  MyBot.setBatteryVolts(9);
  
  // attaches the servo on pin 14 which is A0 and sets the angles for Open and Closed Postions
  MyBot.setGripPinOpenClosed(A0,180,70);  
 
  // initialize the robot
  MyBot.beginGobbit();

  // open gripper
  MyBot.gripOpen();

  // calibrate the line sensor on the line
  MyBot.calibrateLineSensor(); 

}

void loop() {
  
  // Drive Forward
  MyBot.drive('F');

  // Stop at next end or intersection
  MyBot.drive('S');

  // close gripper
  MyBot.gripClose();
  
  // Make a U-Turn and drive the other direction
  MyBot.drive('U');
  
  // Stop at next end or intersection
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();

  // backup a little (speed 0-100, milliseconds to drive before stopping)
  MyBot.backup(65,300); 

  // wait forever
  while(1);
}
