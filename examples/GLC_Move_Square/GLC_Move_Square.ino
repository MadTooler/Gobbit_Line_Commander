// 10/28/2017
// Example moving in a square path using the Gobbit Line Command library
// with move() motor commands and no sensors.


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

  MyBot.beginGobbit();

}


void loop() {

  // run this loop 3 times
  for(int i=0; i<3; i++){
  
    // move forward at 60% speed, slight left adjustment
    MyBot.move(60, -13);
	
    // wait a little
    delay (600);
	
    // stop the motors
    MyBot.move(0, 0);
	
    // wait a little for the robot to stop rolling
    delay (150);

    // turn to the right at 50% speed
    MyBot.move(50, 100);
	
    // wait a little for robot to turn
    delay (345);
	
    // stop the motors
    MyBot.move(0, 0);
	
    // wait a little for the robot to stop rolling
    delay (150);
  }
  

  // final move back to around the start position
  MyBot.move(60, -13);
  delay (600);

  // stop motors	
  MyBot.move(0, 0);


  // wait here forever
  while (1);

}



