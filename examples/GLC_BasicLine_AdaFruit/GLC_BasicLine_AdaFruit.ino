// Use the Adafruit motor shield v2.3
  // M1 and M2 will be used.  Right motor on M1, Left on M2.
#define ADAFRUIT_MS 

#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor(0);

}

void loop() {
  
  MyBot.followLine();
  
}
