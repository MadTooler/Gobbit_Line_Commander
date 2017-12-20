#ifndef LineSensor_h
#define LineSensor_h

class LineSensor {
  public:
    virtual void init();
    virtual unsigned int readLine();
    virtual void calibrate();
    virtual bool isLineRight();
		virtual bool isLineLeft();
		virtual bool isLineFarRight();
		virtual bool isLineFarLeft();
		virtual bool isLineCenter();
		virtual bool isLineVisible();
  private:
};

#endif
