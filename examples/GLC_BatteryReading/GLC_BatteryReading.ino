// 10/28/2017
// Reads the battery voltage on declared analog pin and returns the voltage as a float value.
// Requires a voltage divider to avoid over voltage to the analog input.
// 
//    33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
//
//    CAUTION:  If using Lipo batteries, also use the checkBattery function to keep from running 
//              the battery too low and damaging your cells or causing a fire risk.  
//
//    ALWAYS   be careful if you are using Lithium batteries and do not rely on this or any
//             other software solution for absolute battery safety.


// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

#define ANALOG_DIVIDER_PIN A0 // analog pin your voltage divider is connected
#define SMALL_RES_KOHMS 33 // K ohm value of your small resistor in the divider
#define LARGE_RES_KOHMS 100 // K ohm value of your small resistor in the divider

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  Serial.begin(9600);
  
}

void loop() {

  // read the voltage divider and serial print the value
  Serial.print(MyBot.readBatteryVolts(ANALOG_DIVIDER_PIN,SMALL_RES_KOHMS,LARGE_RES_KOHMS));
  Serial.println(" Battery Voltage");
  Serial.println("");

  delay(1000);
      
}
