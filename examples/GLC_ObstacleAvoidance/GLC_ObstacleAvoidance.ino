// Gobbit Line Command Simple obstacle avoidance 
// with the Gobbit and a Maxbotics EZ-0 ultrasonic sensor

// If the Adafruit motor shield v2.3 is to be used...
  // M1 and M2 terminals will be used.  Right motor on M1, Left on M2.
  // UnComment the next line if you are using the Adafruit shield
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
