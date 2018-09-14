/*  10/28/2017
 *  
 *  Using the Gobbit robot with line sensor on a lined course
 *  this program will control the robot to follow lines...  
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
#include <ZumoMotor.h>
#include <ZumoLineSensor.h>

ZumoMotor zumoMotor;
ZumoLineSensor zumoLineSensor;
GobbitLineCommand MyBot(zumoMotor, zumoLineSensor);

void setup() {

  MyBot.setPID(0.035, 0, 0.035 * 12);
  MyBot.setPIDcoarse(0.08, 0, 0.08 * 12);
  MyBot.setBatteryVolts(12);

  MyBot.beginGobbit();

  delay(1000);
  MyBot.calibrateLineSensor(25);
  delay(1000);
}

void loop() {
  
  MyBot.followLine(0);
  
}