/* 10/28/2017

    Using the Gobbit robot with line sensor on a course with three parallel lines
    where the three lines are like this:
    
      Start here  --------
                  --------------------
                               ------- end point of initial loop
                               
    The program will...

     1) Follow the starting line until it reaches the end of that line.
     2) Keep moving forward while slightly veering to the right.
     3) Keep the same heading as it begins to detect the middle line.
     4) As soon as it detects it has past the 2nd line, adjust heading to straight ahead.
     5) Catch the next line.
     6) After the target line has been caught, follow it until it reaches the end.
     7) Turn around at the end of line.
     8) Repeat all of the steps forever.

     ** You may have to adjust the turn strength variable in the first
        MyBot.move(100,(adjust this value)); line to get your robot
        to veer to the right enough to achieve a suitable angle for
        your actual course. **

   How the detectLine() function works...
     It detects if a line is found in the requested part of the QTR sensor, then returns 
     a byte of 1 if found, 0 if not.    
     The Sensor location value of far ('L')eft, far ('R')ight, the two in ('C')enter, or ('A')ny sensor.
    
     Example: 
       if(MyBot.detectLine('L')){ // looks for a line at the far left sensor and returns 1 if found
          // do something
       }
   To see a video using this sketch:  https://youtu.be/pnqzhV0mpnI

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

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor();

}

void loop() {

  // follow the line until the end is found
  MyBot.followLine(3);

  // move forward full speed and slightly to the right
  MyBot.move(100, 45);

  // continue the current heading while no line is detected
  while (!MyBot.detectLine('A'));

  // continue the current heading while the line is being detected and crossed
  while (MyBot.detectLine('A'));
  
  // adjust current heading to full speed straight ahead
  MyBot.move(100, 0);
  
  // catch the next line
  MyBot.catchLine();
  
  // drive to the end of the line and then turn around
  // NOTE: drive('U') is the same as calling followLine(3) then turn('L')
  MyBot.drive('U');
  
}
