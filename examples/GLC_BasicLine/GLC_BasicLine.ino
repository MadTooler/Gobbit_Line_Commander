/*  09/18/2017
 *  
 *  Using the Gobbit robot with line sensor on a lined course
 *  this program will control the robot to follow lines...  
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

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor();

}

void loop() {
  
  MyBot.followLine(0);
  
}
