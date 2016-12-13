#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  Serial.begin(9600);
}

void loop() {

  // check the voltage of the battery
  MyBot.checkBattery(A5,0,33,100);
  
  Serial.print(MyBot.readBatteryVolts(A5,33,100));
  Serial.println(" Battery Voltage");
  Serial.println("");

  delay(1000);
      
}
