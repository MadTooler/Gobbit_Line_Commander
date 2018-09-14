#ifndef Motor_h
#define Motor_h

class Motor {
  public:
    virtual void init();
    virtual void setMotors(float leftVelocity, float rightVelocity);
  private:
};

#endif
