#ifndef ZumoMotor_h
#define ZumoMotor_h

#include "Motor.h"

class ZumoMotor : public Motor {
  public:
    void init();
    void setMotors(float leftVelocity, float rightVelocity);
  private:
};

#endif
