/* 03/24/2017

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

// If the Adafruit motor shield v2.3 is to be used...
// M1 and M2 terminals will be used.  Right motor on M1, Left on M2.
// UnComment the next line if you are using the Adafruit shield
//#define ADAFRUIT_MS

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor(0);

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
