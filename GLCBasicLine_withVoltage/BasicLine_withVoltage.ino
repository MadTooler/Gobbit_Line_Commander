#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  MyBot.readBatteryVolts(A5,33,100);

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor(0);

}

void loop() {
  
  // check the voltage of the battery
  MyBot.checkBattery(A5,3.25*2,33,100);
  
  MyBot.followLine();
  
}
