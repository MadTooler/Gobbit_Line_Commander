#include "ZumoMotor.h"
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;

void ZumoMotor::init(){

}

void ZumoMotor::setMotors(float leftVelocity, float rightVelocity){
  motors.setLeftSpeed(leftVelocity * 4);
  motors.setRightSpeed(rightVelocity * 4);
}
