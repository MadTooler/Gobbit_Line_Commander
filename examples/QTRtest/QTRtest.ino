/*
    QTR-8RC line sensor Testing sketch to determine correct wiring
    of sensor with the Gobbit robot chassis.

    For a video using this sketch: https://youtu.be/Y3DGQTP38wU

    For assembly:  http://primalengineering.com/robots/

    For wiring:  http://primalengineering.com/robots/

    For additional programming, hardware, and other projects:  http://www.primalengineering.com/robots

 	Parts list:

 		1) Gobbit Robot with line following configuration
 		    Components:
 				Gobbit Chassis with motors, switch, and battery holder
 				Sparkfun Redboard (arduino uno clone)
 				Motor shield
 					Sparkfun Ardumoto Shield
 					or
 					Adafruit v 2.3 motor shield
 				Jumper wires
 				Breadboard optional
 				Pololu QTR-8RC RC Reflectance Sensor Array

 			  see: Zagros Robot Starter Kit - Gobbit
 			       http://www.zagrosrobotics.com/

 		2) Black electrical tape line course on white background

 		4) Batteries

    The code within this sketch should work without
    changes if the Gobbit chassis is used and wired following the
    instructions.  This sketch will help confirm wiring is correct.

    To Use:
      1) Uncomment the motor driver define that matches your hardware.
      2) Upload sketch to Arduino/Redboard.
      3) Run the Serial Monitor at 115200 baud with your Arduino/Redboard connected to your computer via USB.
      4) Stand the robot up on the end with the ball caster, with the sensor facing away from you.  From this
         perspective, the readout on the serial monitor will match left to right with the robot left/right.
      5) Slowly wave your finger or a pencil/pen close to the sensor from side to side.
        a) The serial monitor should read __ at each sensor that detects the IR light refleced from your finger.
        b) The serial monitor should read -- where only some reflected light is detected.
        c) The serial monitor should read XX where very little reflection is detected.
        d) If it appears to be working properly, place the robot on your course.
           i) The sensors over the line should read XX since it is black and does not reflect much light.
           ii) The sensors over the white of the course should read __ since the white reflects a lot of light.
      6) If your readings do not match what you would expect:
        a) Unplug the robot
        b) Check your wiring and fix any errors.
        c) Re-run the test again and repeat until it works properly.
      7) It may be helpful to uncomment the sensor numbering or raw data if you have further errors to debug.
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


void setup()
{
  // line sensor pin setup
  // sensors 1 through 8 are connected to your arduino or motor driver shield
  // on the QTR sensor, 1 is far Right sensor while 8 is far Left sensor.
  // The default pin numbers on the arduino or motor driver shield are loaded for whichever motor driver you #define'd.
  // If needed, uncomment the next line and change the pin numbers to match your wiring.
  // MyBot.setQTRpins(2, 4, 5, 6, 7, 8, 9, 10);

  MyBot.beginGobbit();  // initializes robot with settings
}


void loop() // start main loop
{

  MyBot.QTRtest();

}  // end main loop
