#include "Arduino.h"
#include "AdafruitMotor.h"
#include "Adafruit_MotorShield.h"

// #define ADAFRUIT_MS in main program if the Adafruit motor shield v2.3 is to be used.
// M1 and M2 will be used.  Right motor on M1, Left on M2.
// No define will default to Sparkfun Ardumoto or other makes of Dir
// and PWM pin driven style drivers similar to the Ardumoto and those
// based upon the L298 driver.  The default pin settings will be used
// except where new values have been declared.

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor* rightMotor = AFMS.getMotor(1);
// and another motor on port M2
Adafruit_DCMotor* leftMotor = AFMS.getMotor(2);

void AdafruitMotor::init(){
  // create with the default frequency 1.6KHz
  AFMS.begin();
}

void AdafruitMotor::setMotors(float leftVelocity, float rightVelocity){
  // set speed with ratio of max possible for controller of 255 / max speed value
	leftVelocity = leftVelocity * 2.55;
	rightVelocity = rightVelocity * 2.55;

	//	limit velocities to 255 range of PWM
	leftVelocity = constrain(leftVelocity, -255, 255);
	rightVelocity = constrain(rightVelocity, -255, 255);

	// if Adafruit Motorshield v2.3 was defined
	// Set the speed to start, from 0 (off) to 255 (max speed)
  // Left stop motor
  if (leftVelocity == 0) {
    leftMotor->setSpeed(0);
    leftMotor->run(FORWARD);
    //leftMotor->run(RELEASE);
  }

  // Left forward
  else if (leftVelocity > 0) {
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(leftVelocity);
  }

  // Left Backward
  else if (leftVelocity < 0) {
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(abs(leftVelocity));
  }

  // Right stop motor
  if (rightVelocity == 0) {
    rightMotor->setSpeed(0);
    rightMotor->run(FORWARD);
  }

  // Right forward
  else if (rightVelocity > 0) {
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(rightVelocity);
  }

  // Right Backward
  else if (rightVelocity < 0) {
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(abs(rightVelocity));
  }

}
