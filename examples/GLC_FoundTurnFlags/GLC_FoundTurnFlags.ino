/* 10/28/2017

    Using the Gobbit robot with line sensor on a course with an "h" shaped path
    similar to this:
    
      Start here  ________
                          |
        end here  ________|_______ or start here
                               
    The program will...

     1) Follow the line from either start point until it reaches the next intersection.
     2) Check if a forward path was found.
     3) If a forward path was found, then continue driving forward.
     4) Else, make a right turn and drive to the next and make another right.
     5) Stop at the end of the line.
     6) Do nothing else.    

   How the found turn flags work...
      There are five different flags, or indicators, that are updated by the drive() and
      followLine(mode 2 or 3) functions as an intersection or end of line is found. 

        MyBot.foundLeft;
        MyBot.foundRight;
        MyBot.foundForward;
        MyBot.foundEnd;
        MyBot.foundMark; // a Mark is a thick line, about 3 widths of electrical tape.
      
      These are temporary indicators that are either updated or reset each time the drive()
      (except drive('S')), move(), turn(), and backup() functions are called.

      The value of the flag is either:
        1 if found
        0 if not found
    
   To see a video using this sketch:  https://youtu.be/mZhiuMsTS5U

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

  // since we only want to run this once, we will keep it all in the setup()
  
  // follow the line until the next intersection is found
  MyBot.followLine(3);

  // if a forward path was found, drive forward
  if(MyBot.foundForward) MyBot.drive('F');
  
  // else it must be at the other end of the h and needs to make two rights
  else{
    MyBot.drive('R'); // this one could also be MyBot.turn('R');
    MyBot.drive('R');
  }

  // stop at the end of the line
  MyBot.drive('S');
}

void loop() {

}
