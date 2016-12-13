#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  MyBot.beginGobbit();

  MyBot.calibrateLineSensor(0);

}

void loop() {
  
  MyBot.followLine();
  
}
