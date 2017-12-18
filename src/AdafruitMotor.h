#ifndef AdafruitMotor_h
#define AdafruitMotor_h

#include "Motor.h"

class AdafruitMotor : public Motor {
  public:
    void foo();
    void init();
    void setMotors(float leftVelocity, float rightVelocity);
  private:
};

#endif
