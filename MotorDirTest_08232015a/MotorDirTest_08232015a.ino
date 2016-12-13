#define sketchVersion "08232015a"
/*
 *  Motor Direction Testing sketch to determine correct wiring
 *  of Left and Right gear motors with basic robot chassis.
 *
 *  For wiring diagram see:
 *  http://www.zagrosrobotics.com/files/ZagrosLineMazeFollowWiring_08232015a.pdf
 *  
 *  For detailed instructions on installing the Arduino IDE, Redboard drivers,  
 *  uploading sketches, and testing motors for proper wiring for this sketch see:
 *  http://www.zagrosrobotics.com/files/ZagrosArduinoInstallation_08232015a.pdf 
 *  
 *  Parts list:
 *
 *    1) Zagros Robot Starter Kit - Gobbit or Magician Version
 *       (http://www.zagrosrobotics.com/)
 *       Included components needed:
 *          Gobbit or Magician Robot Chassis with motors and battery holder
 *          Sparkfun Redboard
 *          Sparkfun Ardumoto Shield
 *          Jumper wires
 *          Breadboard optional
 *
 *  The motor engagement code within this sketch should work without
 *  changes if the Gobbit chassis is used and wired following Zagros Robotics'
 *  instructions.  This sketch will help confirm wiring is correct, or
 *  with different chassis, help determine what wires may need to be
 *  swapped.
 *
 *  To Use: 
 *    1) Upload sketch to Arduino/Redboard.
 *    2) Run the Serial Monitor with your Arduino/Redboard connected to your computer via USB.
 *    3) Test the motors individually first.  
 *        a) If the wrong motor moves, swap the Ardumoto A1-A2 terminal wires with the B3-B4.
 *        b) If the correct motor moves, but the wrong direction, swap the black and red 
 *           motor wires at the Ardumoto A or B terminals for just that motor.
 *        c) Re-test individual motors after any wiring changes.
 *    4) Test both motors together to confirm individual setup is correct.
 *    
 */


// ArduMoto motor driver vars
// pwm_a/b sets speed.  Value range is 0-255.  For example, if you set the speed at 100 then 100/255 = 39% duty cycle = slow
// dir_a/b sets direction.  LOW should be Forward, HIGH should be Reverse
int pwm_a = 3;  //PWM control for Ardumoto outputs A1 and A2 is on digital pin 10  (Left motor)
int pwm_b = 11;  //PWM control for Ardumoto outputs B3 and B4 is on digital pin 11  (Right motor)
int dir_a = 12;  //direction control for Ardumoto outputs A1 and A2 is on digital pin 12  (Left motor)
int dir_b = 13;  //direction control for Ardumoto outputs B3 and B4 is on digital pin 13  (Right motor)

// incoming Serial data vars
int incomingByte = 0;	//  for incoming serial data input


void setup()
{
  //Set control pins to be outputs
  pinMode(pwm_a, OUTPUT);
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);

  //set both motors to stop
  analogWrite(pwm_a, 0);
  analogWrite(pwm_b, 0);

  Serial.begin(115200);

  Serial.println("Zagros Robotics, Inc.");
  Serial.println("Motor Direction Test PC Control");
  Serial.print("Version ");
  Serial.println(sketchVersion);
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

      switch (incomingByte) // start both motor command switch
      {
        // stop
        case 's':
          digitalWrite(dir_a, LOW);
          analogWrite(pwm_a, 0);
          Serial.println("Left Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          digitalWrite(dir_a, LOW);
          analogWrite(pwm_a, 255);
          Serial.println("Left Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          digitalWrite(dir_a, HIGH);
          analogWrite(pwm_a, 255);
          Serial.println("Leftt Backward\n");
          incomingByte = '*';
          break;

      }  // end Left motor command switch

    }  // end while right motor to return to prior menu at "x" input

    // stop
    digitalWrite(dir_a, LOW);
    analogWrite(pwm_a, 0);
    digitalWrite(dir_b, LOW);
    analogWrite(pwm_b, 0);

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

      switch (incomingByte) // start both motor command switch
      {
        // stop
        case 's':
          digitalWrite(dir_b, LOW);
          analogWrite(pwm_b, 0);
          Serial.println("Right Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          digitalWrite(dir_b, LOW);
          analogWrite(pwm_b, 255);
          Serial.println("Right Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          digitalWrite(dir_b, HIGH);
          analogWrite(pwm_b, 255);
          Serial.println("Right Backward\n");
          incomingByte = '*';
          break;

      }  // end Right motor command switch

    }  // end while right motor to return to prior menu at "x" input

    // stop
    digitalWrite(dir_a, LOW);
    analogWrite(pwm_a, 0);
    digitalWrite(dir_b, LOW);
    analogWrite(pwm_b, 0);

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
          digitalWrite(dir_a, LOW);
          analogWrite(pwm_a, 0);
          digitalWrite(dir_b, LOW);
          analogWrite(pwm_b, 0);
          Serial.println("Both Stop\n");
          incomingByte = '*';

          break;

        // forward
        case 'f':
          digitalWrite(dir_a, LOW);
          analogWrite(pwm_a, 255);
          digitalWrite(dir_b, LOW);
          analogWrite(pwm_b, 255);
          Serial.println("Both Forward\n");
          incomingByte = '*';
          break;

        // backward
        case 'b':
          digitalWrite(dir_a, HIGH);
          analogWrite(pwm_a, 255);
          digitalWrite(dir_b, HIGH);
          analogWrite(pwm_b, 255);
          Serial.println("Both Backward\n");
          incomingByte = '*';
          break;

        // Rotate left
        case 'l':
          digitalWrite(dir_a, HIGH);
          analogWrite(pwm_a, 255);
          digitalWrite(dir_b, LOW);
          analogWrite(pwm_b, 255);
          Serial.println("Rotate Left\n");
          incomingByte = '*';
          break;

        // Rotate right
        case 'r':
          digitalWrite(dir_a, LOW);
          analogWrite(pwm_a, 255);
          digitalWrite(dir_b, HIGH);
          analogWrite(pwm_b, 255);
          Serial.println("Rotate Right\n");
          incomingByte = '*';
          break;

      }  // end both motor command switch

    }  // end while both motors to return to prior menu at "x" input

    // stop
    digitalWrite(dir_a, LOW);
    analogWrite(pwm_a, 0);
    digitalWrite(dir_b, LOW);
    analogWrite(pwm_b, 0);

    incomingByte = '*';

  } // end both motor option from main menu

  else incomingByte = '*';

  delay(10);

}  // end main loop





