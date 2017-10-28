#define SKETCH_VERSION "10282017a"
/*
 *  Motor Direction Testing sketch to determine correct wiring
 *  of Left and Right gear motors with the Gobbit robot chassis.
 *
 *  For a video using this sketch: https://youtu.be/5EnlZLv1E80
 *
 *  For assembly:  http://primalengineering.com/robots
 *
 *  For wiring:  http://primalengineering.com/robots
 *
 *  For additional programming, hardware, and other projects:  http://www.primalengineering.com/robots 
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
 *			       http://www.zagrosrobotics.com/
 *
 *		2) Black electrical tape line course on white background
 *
 *		4) Batteries
 *
 *  The motor engagement code within this sketch should work without
 *  changes if the Gobbit chassis is used and wired following the
 *  instructions.  This sketch will help confirm wiring is correct, or
 *  help determine what wires may need to be swapped.
 *
 *  To Use: 
 *    1) Uncomment the motor driver define that matches your hardware.
 *    2) Upload sketch to Arduino/Redboard.
 *    3) Run the Serial Monitor with your Arduino/Redboard connected to your computer via USB.
 *    4) Test the motors individually first.  
 *        ***MAKE SURE BOTH WIRES FROM A SINGLE MOTOR ARE CONNECTED TO ONLY A SINGLE CHANNEL (M1/M2/M3/M4 on Adafruit or A/B on Ardumoto)***
 *        a) If the wrong motor moves, swap the pairs of wires for your left motor with right motor.
 *        b) If the correct motor moves, but the wrong direction, swap the black and red 
 *           motor wires for just that motor.
 *        c) Re-test individual motors after any wiring changes.
 *    5) Test both motors together to confirm individual setup is correct.
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

GobbitLineCommand MyBot;

// incoming Serial data vars
int incomingByte = 0;	//  for incoming serial data input


void setup()
{
  MyBot.beginGobbit();

  Serial.begin(115200);

  Serial.println("Gobbit Line Command");
  Serial.println("Motor Direction Test PC Control");
  Serial.print("Version ");
  Serial.println(SKETCH_VERSION);
  Serial.println();
  incomingByte = '*';
}


void loop() // start main loop
{

  Serial.println("Which motor do you want to test?");
  Serial.println("  (l)eft");
  Serial.println("  (r)ight");
  Serial.println("  (b)oth");
  Serial.println();
  incomingByte = '*';

  while (Serial.available() == 0); // wait for serial input

  // read the incoming serial byte:
  incomingByte = Serial.read();

  // Left Motor only
  if (incomingByte == 'l')
  {

    incomingByte = '*';

    while (incomingByte != 'x')
    {

      Serial.println("Left motor only.");
      Serial.println("What direction?");
      Serial.println("  (f)orward");
      Serial.println("  (b)ackward");
      Serial.println("  (s)top");
      Serial.println("   e(x)it to motor selection menu.");
      Serial.println();
      incomingByte = '*';

      while (Serial.available() == 0); // wait for serial input

      // read the incoming serial byte:
      incomingByte = Serial.read();

      switch (incomingByte) // start left motor command switch
      {
        // stop
        case 's':
          MyBot.setMotors(0,0);
          Serial.println("Left Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          MyBot.setMotors(100,0);
          Serial.println("Left Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          MyBot.setMotors(-100,0);
          Serial.println("Left Backward\n");
          incomingByte = '*';
          break;

      }  // end Left motor command switch

    }  // end while right motor to return to prior menu at "x" input

    // stop
    MyBot.setMotors(0,0);

  } // end left motor option from main menu


  // Right Motor only
  else if (incomingByte == 'r')
  {
    incomingByte = '*';

    while (incomingByte != 'x')
    {

      Serial.println("Right motor only.");
      Serial.println("What direction?");
      Serial.println("  (f)orward");
      Serial.println("  (b)ackward");
      Serial.println("  (s)top");
      Serial.println("   e(x)it to motor selection menu.");
      Serial.println();
      incomingByte = '*';

      while (Serial.available() == 0); // wait for serial input

      // read the incoming serial byte:
      incomingByte = Serial.read();

      switch (incomingByte) // start right motor command switch
      {
        // stop
        case 's':
          MyBot.setMotors(0,0);
          Serial.println("Right Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          MyBot.setMotors(0,100);
          Serial.println("Right Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          MyBot.setMotors(0,-100);
          Serial.println("Right Backward\n");
          incomingByte = '*';
          break;

      }  // end Right motor command switch

    }  // end while right motor to return to prior menu at "x" input

    // stop
    MyBot.setMotors(0,0);

  } // end right motor option from main menu


  // Both Motors
  else if (incomingByte == 'b')
  {
    incomingByte = '*';

    while (incomingByte != 'x')
    {

      Serial.println("Both motors.");
      Serial.println("What direction?");
      Serial.println("  (f)orward");
      Serial.println("  (b)ackward");
      Serial.println("  (l)eft");
      Serial.println("  (r)ight");
      Serial.println("  (s)top");
      Serial.println("   e(x)it to motor selection menu.");
      Serial.println();
      incomingByte = '*';

      while (Serial.available() == 0); // wait for serial input

      // read the incoming serial byte:
      incomingByte = Serial.read();

      switch (incomingByte) // start both motor command switch
      {
        // stop
        case 's':
          MyBot.setMotors(0,0);
          Serial.println("Both Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          MyBot.setMotors(100,100);
          Serial.println("Both Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          MyBot.setMotors(-100,-100);
          Serial.println("Both Backward\n");
          incomingByte = '*';
          break;

        // Rotate left
        case 'l':
          MyBot.setMotors(-100,100);
          Serial.println("Rotate Left\n");
          incomingByte = '*';
          break;

        // Rotate right
        case 'r':
          MyBot.setMotors(100,-100);
          Serial.println("Rotate Right\n");
          incomingByte = '*';
          break;

      }  // end both motor command switch

    }  // end while both motors to return to prior menu at "x" input

    // stop
    MyBot.setMotors(0,0);

    incomingByte = '*';

  } // end both motor option from main menu

  else incomingByte = '*';

  delay(10);

}  // end main loop





