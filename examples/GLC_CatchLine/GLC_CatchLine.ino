/* 03/24/2017
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

// If the Adafruit motor shield v2.3 is to be used...
  // M1 and M2 terminals will be used.  Right motor on M1, Left on M2.
  // UnComment the next line if you are using the Adafruit shield
//#define ADAFRUIT_MS 


#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {
  
  MyBot.setBatteryVolts(9);
 
  // initialize the robot
  MyBot.beginGobbit();

  // calibrate the line sensor on the line
  MyBot.calibrateLineSensor(0);   
}


void loop() {

  MyBot.followLine(3);
  MyBot.move(100,65);
  MyBot.catchLine();
  
}
