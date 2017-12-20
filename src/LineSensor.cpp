#include "Arduino.h"
#include "LineSensor.h"

void LineSensor::init(){
  Serial.println("Line Sensor init()");
}

unsigned int LineSensor::readLine(){
  Serial.println("Reading Sensors");
  if(millis() / 100 % 2 == 0){
    return 3500;
  } else {
    return 1500;
  }
  
}

void LineSensor::calibrate(){
  Serial.println("calabrating...");
}

bool LineSensor::isLineFarRight(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}

bool LineSensor::isLineRight(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}

bool LineSensor::isLineFarLeft(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}

bool LineSensor::isLineLeft(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}

bool LineSensor::isLineCenter(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}

bool LineSensor::isLineVisible(){
  //Switch every millisecond
  return millis() / 100 % 2 == 0;
}