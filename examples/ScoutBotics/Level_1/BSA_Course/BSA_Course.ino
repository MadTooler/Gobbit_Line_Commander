/* 10/20/2018
 *  
 *  Using the Gobbit robot with line sensor on the BSA shaped course...
 *  
 *  The program will...
 *  
 *   1) Use the drive() function to navigate from the start to the end point.
 *   2) Stop at the end and do nothing else.
 *   
 *  *** Change the use of the drive() steps to improve your course time. ***
 *  
 *  How the drive() function works...
 *    Calling drive('L/R/F/S/U') will start driving/following the line and continue following until it 
 *    is able to complete the requested direction/turn at the next found intersection 
 *    or end.  If it cannot make the requested direction/turn, it will spin around fast 
 *    to indicate it had a problem, and stop the robot.
 *    
 *    Turn direction values are ('L')eft, ('R')ight, ('F')orward, ('S')top, or ('U')turn.
 *
 *  To see a video using a similar sketch:  https://youtu.be/c2BB-Bc95Ik
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

// Give your robot a name.
// I called it "MyBot" here, but you call it whatever you want.
// If you make a new name, make sure to find/replace all of the "MyBot" in the sketch with your new name. 
GobbitLineCommand MyBot;

void setup() {
	
  MyBot.setBatteryVolts(9);

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor();

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

  // at the end, stop the robot
  MyBot.drive('S');

  // We need to tell it to do nothing else forever so it doesn't start the loop() over
  while(1);
   
}
