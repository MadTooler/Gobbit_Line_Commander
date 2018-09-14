#include "Motor.h"
#include "Arduino.h"

void Motor::init(){
  Serial.begin(9600);
}

void Motor::setMotors(float leftVelocity, float rightVelocity){
  Serial.print("Left: ");
  Serial.print(leftVelocity);
  Serial.print("   Right: ");
  Serial.println(rightVelocity);
}
