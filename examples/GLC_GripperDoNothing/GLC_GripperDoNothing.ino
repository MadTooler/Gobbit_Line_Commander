/* 10/28/2017
 *  
 *  Using the Gobbit robot with line sensor and gripper on pin A0, 
 *  on a simple line course with a 90 degree X where an object is at
 *  the end of three of the line ends, and the robot is at the other
 *  facing the middle of the X, this program will perpetually shift
 *  the objects around the X.
 *  
 *  The program will...
 *  
 *   One time in setup:
 *   1) Drive Forward along a line
 *   2) Turn right at the intersection and continue driving
 *   3) Stop at the end of the line (or any found intersection)
 *   4) Close the gripper to grab something that was placed there
 *   5) Turn around and drive the other direction
 *   6) Turn left at the intersection and continue driving 
 *   7) Stop at the end of the line (or any found intersection)
 *   8) Open the gripper to release the something
 *   9) Backup a little away from the something
 *   
 *   Then do this over and over:
 *   1) Turn around and drive the other direction
 *   2) Drive Forward at the next intersection along the line
 *   3) Stop at the end of the line (or any found intersection)
 *   4) Close the gripper to grab something that was placed there
 *   5) Turn around and drive the other direction
 *   6) Turn left at the intersection and continue driving 
 *   7) Stop at the end of the line (or any found intersection)
 *   8) Open the gripper to release the something
 *   9) Backup a little away from the something
 *
 *  To see a video using this sketch:  https://youtu.be/ap2OHx_hRoU?list=PLawtTl1V11Fz_OZYQnIneA7F-aO4kcc6E
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

  // Start move begins from the leg of X with no object, 
  // with the robot facing middle of X.
  // This will get and move the first something into a sequence
  // for the main loop to repeat.
  
  MyBot.drive('R');
  MyBot.drive('S');
  MyBot.gripClose();
  MyBot.drive('U');
  MyBot.drive('L');
  MyBot.drive('S');
  MyBot.gripOpen();
  MyBot.backup(65,300);
  
}

void loop() {
  
  // This will repeat forever, just moving the three objects
  // one by one around the legs of the X perpetually.

  MyBot.drive('U');
  MyBot.drive('F');
  MyBot.drive('S');
  MyBot.gripClose();
  MyBot.drive('U');
  MyBot.drive('L');
  MyBot.drive('S');
  MyBot.gripOpen();
  MyBot.backup(65,300);
}
