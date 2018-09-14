#ifndef ZumoLineSensor_h
#define ZumoLineSensor_h

#include "LineSensor.h"

class ZumoLineSensor : public LineSensor {
  public:
    void init();
    unsigned int readLine();
    void calibrate();
    bool isLineRight();
    bool isLineLeft();
    bool isLineFarRight();
    bool isLineFarLeft();
    bool isLineCenter();
    bool isLineVisible();
  private:
    unsigned int* sensorValues;
};

#endif