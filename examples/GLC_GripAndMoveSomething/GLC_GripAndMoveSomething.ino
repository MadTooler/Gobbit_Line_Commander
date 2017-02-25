/* 02/25/2017
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

// If the Adafruit motor shield v2.3 is to be used...
  // M1 and M2 terminals will be used.  Right motor on M1, Left on M2.
  // UnComment the next line if you are using the Adafruit shield
//#define ADAFRUIT_MS 

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {
  
  // attaches the servo on pin 14 which is A0 and sets the angles for Open and Closed Postions
  MyBot.setGripPinOpenClosed(A0,180,70);  
 
  // initialize the robot
  MyBot.beginGobbit();

  // open gripper
  MyBot.gripOpen();

  // calibrate the line sensor on the line
  MyBot.calibrateLineSensor(0); 

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
