/* 10/28/2017
 *  
 *  Using the Gobbit robot with line sensor on a single straight line
 *  course about 20" long.
 *  
 *  The program will...
 *  
 *   1) Follow along a line until it reaches the end
 *   2) Veer right and continue until it intercepts the line again
 *   3) It will "Catch" the line and begin following again.
 *   4) Repeat
 *
 *   ** You may have to adjust the turn strength variable in the
 *      MyBot.move(100,(adjust this value)); line to get your robot
 *      to circle back to the line properly. **
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
  
  MyBot.setBatteryVolts(9);
 
  // initialize the robot
  MyBot.beginGobbit();

  // calibrate the line sensor on the line
  MyBot.calibrateLineSensor();   
}


void loop() {

  MyBot.followLine(3);
  MyBot.move(100,65);
  MyBot.catchLine();
  
}
