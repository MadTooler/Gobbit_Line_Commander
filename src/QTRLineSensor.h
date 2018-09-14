#ifndef QTRLineSensor_h
#define QTRLineSensor_h

#include "LineSensor.h"

class QTRLineSensor : public LineSensor {
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
