/*
*  Gobbit Line Command Maze Solving example using "Left Hand Rule"
*
* Created by Jason Talley 
* Last edit 10/28/2017
* Released under GNU agreement
*/

/*
*  For assembly:  http://primalengineering.com/robots/Downloads/GBT-101-INSTR-R2_LineFollowAssemblyInstructions.PDF
*
*  For wiring:  http://primalengineering.com/robots/Downloads/Gobbit_LineFollowWiring_03222017.pdf
*
*  For additional programming, hardware, and other projects:  http://www.primalengineering.com/robots 
*
*
*  Parts list:
*
*   1) Gobbit Robot with line following configuration
*      Components:
*       Gobbit Chassis with motors, switch, and battery holder
*       Sparkfun Redboard (arduino uno clone)
*       Motor shield
*         Sparkfun Ardumoto Shield
*         or
*         Adafruit v 2.3 motor shield
*       Jumper wires
*       Breadboard optional
*       Pololu QTR-8RC RC Reflectance Sensor Array
*
*       see: Zagros Robot Starter Kit - Gobbit
*            http://www.zagrosrobotics.com/
*
*   2) Black electrical tape line course on white background
*
*   4) Batteries
*/

// Gobbit Line Command Maze Solving example using "Left Hand Rule"
//
//    To see how the turns and line following are being done, look into
//    the GobbitLineCommand.h and .cpp library files.

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

// The path variable will store the path that the robot has taken.  It
// is stored as an array of characters, each of which represents the
// turn that should be made at one intersection in the sequence:
//  'L' for left
//  'R' for right
//  'F' for go foward (going straight through an intersection)
//  'B' for making a U-turn and going Back where it came from last
// You should check to make sure that the pathLength of your
// maze design does not exceed the bounds of the array.
char path[100] = "";
unsigned char pathLength = 0; // the length of the path


void setup() {

  // Set your battery voltage for the GLC to load the best default settings
  MyBot.setBatteryVolts(9);

  // Start the GLC with declared and/or default settings
  MyBot.beginGobbit();

  MyBot.calibrateLineSensor();
}


void loop() {

  // Begin maze solving
  MazeSolve();
}


// This function is called once, from the main loop
void MazeSolve()
{
  // Loop until we have solved the maze.
  while (1)
  {
    // Follow line with intersection detection mode
    MyBot.followLine(3);

    // Check if it found the mark at end of maze then exit this while loop
    // NOTE:  If the mark was found, MyBot.foundMark will == 1. Otherwise it will == 0.
    if (MyBot.foundMark) break;

    // If not at the end, we need to determine which turn to make
    // with the left hand rule by sending the intersection results from the GLC followLine(3)
    // function to our selectTurn function.
    // NOTE:  If a left turn possibility was found, MyBot.foundLeft == 1.  Otherwise it will == 0.
    unsigned char dir = selectTurn(MyBot.foundLeft, MyBot.foundForward, MyBot.foundRight);

    // Store the intersection in the path variable.
    path[pathLength] = dir;
    pathLength ++;

    // determine
    pathTurn(dir);
    
    // Simplify the learned path.
    simplifyPath();
  }

  // It should have already SOLVED the maze, if it has reached this line!

  // Now enter an infinite loop that will re-run the solved maze as many
  // times as you place the robot back to the start.
  while (1)
  {
    //  Maybe you would like to add a blinking led or a beeper to indicate it made it.
    //  Here we will only have it wait quietly for you to pick it up and place it
    //  back on the starting line.
    
    // Stop both motors quickly with motor braking (quick reversal of direction)
    MyBot.brakeMotors(100, 'B');

    // Do nothing while robot is sitting past the end mark on the white of the course, 
    // waiting to be picked up.
    // NOTE: MyBot.detectLine(L,R,C or A) returns 1 if line is found, 0 if not.
    while (!MyBot.detectLine('A'));
    {
      delay(50);
    }

    // You must pick up the robot now and bring it back to the start
    
    // While it is not on the course, the sensors have the same result as 
    // if they all see the line, so let's do nothing while the far Left or Right
    // sensors see the line or open air.
    while (MyBot.detectLine('L') || MyBot.detectLine('R'));
    {
      delay(50);
    }    

    // Before the robot starts moving again, make sure it sees the line.
    // Nothing will happen while the robot does not sense the line on any sensor.
    // Remember, if the robot is picked up, open air will also be sensed as a line.
    while (!MyBot.detectLine('A'))
    {
      delay(50);
    }

    // Delay to give you time to let go of the robot
    delay(2000);

    // THIS PART IS WHERE IT RUNS THE SOLUTION
    // Re-run the now solved maze.  All the turns are known,
    // so this loop is really simple.
    int i;
    for (i = 0; i < pathLength; i++)
    {
      // Follow the line and detect the next intersection
      MyBot.followLine(3);

      // Make the turn or remain heading forward according to the steps stored in
      // path[i] 
      pathTurn(path[i]);
      
    }

    // The last turn has been made
    // Follow the last segment up to the finish.
    MyBot.followLine(3);

    // Now we should be at the finish!  If you move the robot to the start again, it will re-run this loop with the solution again.

  } // end running solved

} // end MazeSolve


// This function decides which way to turn during the learning phase of
// maze solving.  It uses the received variables _foundLeft, _foundForward,
// and _foundRight, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char selectTurn(byte _foundLeft, byte _foundForward, byte _foundRight)
{
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if (_foundLeft)
    return 'L';
  else if (_foundForward)
    return 'F';
  else if (_foundRight)
    return 'R';
  else
    return 'B';
} // end selectTurn


// Path turn
// Turn along the path by the turn called by _dir
void pathTurn(unsigned char _dir)
{
    // Make the turn called by _dir
    
    // If it was Back then just turn right (this will result in 180 turn U-turn since you must be at the end of a line)
    if(_dir == 'B') MyBot.turn('R');

    // else if Forward no turn, same as do nothing
    else if(_dir == 'F'); 
    
    // else turn Right or Left.
    else MyBot.turn(_dir);
}


// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the B dead end.  For
// example, LBL -> F, because a single F would have bypassed the dead end
// represented by LBL.
void simplifyPath()
{
  // Only simplify the path if the second-to-last turn was a 'B'
  if (pathLength < 3 || path[pathLength - 2] != 'B')
    return;

  // We will represent the turns as angles so we can easily make adjustments
  // mathmatically instead of comparing strings.
  int totalAngle = 0;
  int i;
  for (i = 1; i <= 3; i++)
  {
    switch (path[pathLength - i])
    {
      case 'R':
        totalAngle += 90;
        break;
      case 'L':
        totalAngle += 270;
        break;
      case 'B':
        totalAngle += 180;
        break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  totalAngle = totalAngle % 360;

  // Replace all of those turns with a single one.
  switch (totalAngle)
  {
    case 0:
      path[pathLength - 3] = 'F';
      break;
    case 90:
      path[pathLength - 3] = 'R';
      break;
    case 180:
      path[pathLength - 3] = 'B';
      break;
    case 270:
      path[pathLength - 3] = 'L';
      break;
  }

  // The path is now two steps shorter.
  pathLength -= 2;

} // end simplifyPath
