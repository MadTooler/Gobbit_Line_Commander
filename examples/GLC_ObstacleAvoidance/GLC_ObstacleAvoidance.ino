// 10/28/2017
//
// Gobbit Line Command Simple obstacle avoidance 
// with the Gobbit and a Maxbotics EZ-0 or EZ-1 ultrasonic sensor

// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

#define ANALOG_PIN A3 // analog pin used for sensor 
#define CLEARANCE_DISTANCE 12 // value in inches

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  // Set the Sonar Pin and Safe Distance (followLine and drive functions use the Safe Distance value).
  // Since the safe distance is not used by the functions we will call here,
  // any value can be entered.  We will just use 0.
  MyBot.setSonar(ANALOG_PIN, 0);

  // initialize the robot
  MyBot.beginGobbit();

}

void loop() {

  // if the distance is greater than CLEARANCE_DISTANCE just drive forward ast 100% motor speed
  if(MyBot.readSonarInches() > CLEARANCE_DISTANCE) MyBot.move(100, 0);

  else
  // The distance must be less than CLEARANCE_DISTANCE so turn until 
  // the distance is clear for more than the CLEARANCE_DISTANCE
  {
    //turn/spin Right at 50% of motor speed
    MyBot.move(50, 100);

    // do nothing else until the distance is more than CLEARANCE_DISTANCE
    while(MyBot.readSonarInches() < CLEARANCE_DISTANCE);

  }

}
