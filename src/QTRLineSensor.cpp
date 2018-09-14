#include "QTRLineSensor.h"
#include "Arduino.h"
#include "config.h"
//QTRSensors library must be loaded to your arduino libraries folder.
// Easiest to use the library manager to install
// later releases may be able to auto load if this library is made accessible in arduino library manager.
#include <QTRSensors.h> // Pololu QTR Library

// line sensor object
QTRSensorsRC qtrrc;

void QTRLineSensor::init(){
  unsigned char sensorPins[8]={2, 4, 5, 6, 7, 8, 9, 10}; // defualt values for Gobbit wiring
  qtrrc.init(sensorPins, NUM_SENSORS, TIME_OUT, EMITTER_PIN);
  sensorValues = new unsigned int[NUM_SENSORS];
	delay(500); // give sensors time to set
}

void QTRLineSensor::calibrate(){
  qtrrc.calibrate(); // reads all sensors with the define timeout set at 2500 microseconds (2.5 milliseconds) for sensor outputs to go low.
}

unsigned int QTRLineSensor::readLine(){
  return qtrrc.readLine(sensorValues);
}

bool QTRLineSensor::isLineFarRight(){
	return sensorValues[0] > 200;
}

bool QTRLineSensor::isLineFarLeft(){
	return sensorValues[7] > 200;
}

bool QTRLineSensor::isLineRight(){
	return sensorValues[1] > 200 || sensorValues[2] > 200;
}

bool QTRLineSensor::isLineLeft(){
	return sensorValues[5] > 200 || sensorValues[6] > 200;
}

bool QTRLineSensor::isLineCenter(){
	return sensorValues[3] > 200 || sensorValues[4] > 200;
}

bool QTRLineSensor::isLineVisible(){
	return (isLineFarRight() || isLineRight() || isLineCenter() || isLineLeft() || isLineFarLeft());
}