/* 03/24/2017
 *  
 *  Using the Gobbit robot with line sensor on a course with multiple turns
 *  between a start and end point...  
 *  
 *  The program will...
 *  
 *   1) Use the drive() function to navigate from the start to the end point.
 *   2) Turn around at the end.
 *   3) Use the drive() function to navigate back to the start point.
 *   4) Stop and do nothing else.
 *   
 *  *** Change the use of the drive() steps to fit your course. ***
 *  
 *  How the drive() function works...
 *    Calling drive('L/R/F/S/U') will start driving/following the line and continue following until it 
 *    is able to complete the requested direction/turn at the next found intersection 
 *    or end.  If it cannot make the requested direction/turn, it will spin around fast 
 *    to indicate it had a problem, and stop the robot.
 *    Turn direction values are ('L')eft, ('R')ight, ('F')orward, ('S')top, or ('U')turn.
 *
 *  To see a video using this sketch:  https://youtu.be/c2BB-Bc95Ik
 * 
*/

// If the Adafruit motor shield v2.3 is to be used...
  // M1 and M2 terminals will be used.  Right motor on M1, Left on M2.
  // UnComment the next line if you are using the Adafruit shield
//#define ADAFRUIT_MS 

#include <GobbitLineCommand.h>

// Give your robot a name.
// I called it "MyBot" here, but you call it whatever you want.
// If you make a new name, make sure to find/replace all of the "MyBot" in the sketch with your new name. 
GobbitLineCommand MyBot;

void setup() {

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor(0);

}

void loop() {

  // Use the drive() function to drive from the start to the end of the course
  // Note: You could use a for() loop to easily repeat drive() statements.
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');

  // at the end, turn around
  MyBot.drive('U');

  // Use the drive() function to drive back to your start point
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('F');

  // back at the start, stop the robot
  MyBot.drive('S');

  // Do nothing else forever or it will repeat all your commands again
  while(1);
   
}
