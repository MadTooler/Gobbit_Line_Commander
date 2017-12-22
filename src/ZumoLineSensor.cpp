#include "Arduino.h"
#include "ZumoLineSensor.h"
#include <Wire.h>
#include <Zumo32U4.h>

#define LINE_THRESHOLD 500
Zumo32U4LineSensors lineSensors;
#define NUM_SENSORS 5
uint16_t lineSensorValues[NUM_SENSORS];

void ZumoLineSensor::init(){
    lineSensors.initFiveSensors();
}

void ZumoLineSensor::calibrate(){
    lineSensors.calibrate();
}

unsigned int ZumoLineSensor::readLine(){
  // Read the line sensors.lineSensors.readCalibrated(lineSensorValues)
  //lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);  
  //int16_t position = lineSensors.readLine(lineSensorValues);
  return map(lineSensors.readLine(lineSensorValues), 0, 4000, 7000, 0);
}

bool ZumoLineSensor::isLineFarRight(){
	return lineSensorValues[4] > LINE_THRESHOLD;
}

bool ZumoLineSensor::isLineFarLeft(){
	return lineSensorValues[0] > LINE_THRESHOLD;
}

bool ZumoLineSensor::isLineRight(){
	return lineSensorValues[3] > LINE_THRESHOLD;
}

bool ZumoLineSensor::isLineLeft(){
	return lineSensorValues[1] > LINE_THRESHOLD;
}

bool ZumoLineSensor::isLineCenter(){
	return lineSensorValues[2] > LINE_THRESHOLD;
}

bool ZumoLineSensor::isLineVisible(){
	return (isLineFarRight() || isLineRight() || isLineCenter() || isLineLeft() || isLineFarLeft());
}