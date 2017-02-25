#define SKETCH_VERSION "01282017a"
/*
 *  QTR-8RC line sensor Testing sketch to determine correct wiring
 *  of sensor with the Gobbit robot chassis.
 *
 *  For assembly, wiring, programming, and other examples, see: http://www.primalengineering.com/robots 
 *
 *	Parts list:
 *
 *		1) Gobbit Robot with line following configuration
 *		    Components:
 *				Gobbit Chassis with motors, switch, and battery holder
 *				Sparkfun Redboard (arduino uno clone)
 *				Motor shield
 *					Sparkfun Ardumoto Shield
 *					or
 *					Adafruit v 2.3 motor shield
 *				Jumper wires
 *				Breadboard optional
 *				Pololu QTR-8RC RC Reflectance Sensor Array
 *
 *			  see: Zagros Robot Starter Kit - Gobbit
 *			       (http://www.zagrosrobotics.com/)
 *
 *		2) Black electrical tape line course on white background
 *
 *		4) Batteries
 *
 *  The code within this sketch should work without
 *  changes if the Gobbit chassis is used and wired following the
 *  instructions.  This sketch will help confirm wiring is correct.
 *
 *  To Use:
 *    1) Upload sketch to Arduino/Redboard.
 *    2) Run the Serial Monitor at 115200 baud with your Arduino/Redboard connected to your computer via USB.
 *    3) Place the robot on a lined track with the sensor over the line.
 *    4) Slowly wave the sensor over the line, keeping all the wheels of the robot touching or close to the track.
 *      a) The serial monitor should read __ at each sensor that does not see the line.
 *      b) The serial monitor should read -- where some line is partially seen.
 *      c) The serial monitor should read XX where the line can be fully seen.
 *    5) If your readings do not match what you would expect:
 *      a) Unplug the robot
 *      b) Check your wiring and fix any errors.
 *      c) Re-run the test again and repeat until it works properly.
 *    6) It may be helpful to uncomment the sensor numbering or raw data if you have further errors to debug.
 */

//QTRSensors folder must be placed in your arduino libraries folder
#include <QTRSensors.h>  // Pololu QTR Library 

//line sensor defines
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN  // emitter control pin not used.  If added, replace QTR_NO_EMITTER_PIN with pin#

// line sensor declarations
// sensors 0 through 7 are connected to digital pins 2 through 10, respectively (pin 3 is skipped and used by the Ardumoto controller)
// 0 is far Right sensor while 7 is far Left sensor
// On the QTR sensor, the sensors are labeled 1-8.
// 2,4,5,6,7,8,9,10 are the default pin numbers.  Change them, if needed, to your pin selections.
QTRSensorsRC qtrrc((unsigned char[]) {
  2, 4, 5, 6, 7, 8, 9, 10
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS]; // array with individual sensor reading values
unsigned int line_position = 0; // value from 0-7000 to indicate position of line between sensor 0 - 7


void setup()
{

  Serial.begin(115200);

  Serial.println("Gobbit Line Command");
  Serial.println("QTR line sensor Test");
  Serial.print("Version ");
  Serial.println(SKETCH_VERSION);
  Serial.println();

  delay(2500);

}


void loop() // start main loop
{

  // This will print the sensor numbers. If needed uncomment.
  //Serial.println("8 7 6 5 4 3 2 1 ");

  // read raw sensor values.
  qtrrc.read(sensorValues);

  // print indicators if the sensor sees the line
  for (int i = NUM_SENSORS - 1; i >= 0; i--)
  {
    if (sensorValues[i] > 600)
    {
      if (sensorValues[i] > 1000)
      {
        Serial.print("XX");
      }
      else Serial.print("--");
    }
    else Serial.print("__");
  }
  Serial.println();

  // This will print the raw sensor values. If needed, uncomment.
  //   for (int i = NUM_SENSORS-1; i >= 0; i--)
  //  {
  //    Serial.print(sensorValues[i]);
  //    Serial.print("  ");
  //  }
  //  Serial.println();

  delay(20);

}  // end main loop





