#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {

  // reads the battery voltage on declared analog pin, with voltage divider resistor values in K ohms
  // 33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
  // if using Lipo batteries, also use the checkBattery function to keep from running the battery too low and damaging your cells.
  MyBot.readBatteryVolts(A5,33,100);

  // attaches the servo on pin A0 and sets the angles for Open and Closed Postions
  MyBot.setGripPinOpenClosed(A0,180,68);  
 
  MyBot.beginGobbit();

  // open gripper
  MyBot.gripOpen();

  // serial monitor this if something seems wrong
  // MyBot.serialPrintCurrentSettings();

  MyBot.calibrateLineSensor(0); 

  // Get it started moving forward
  MyBot.drive('F');
}

void loop() {
  // check the voltage of the battery so I don't hurt my lipo cells
  //MyBot.checkBattery(A5,3.25*3,33,100); // 3cell
  MyBot.checkBattery(A5,3.25*2,33,100);

  // first block
  MyBot.drive('F');
  MyBot.drive('R');
  MyBot.drive('S');

  // close gripper
  MyBot.gripClose();

  // goin' for it!
  MyBot.move(100,-3);
  delay(1150);
  
  // re-engage track, hopefully
  MyBot.drive('R');
  MyBot.drive('R');
  MyBot.move(0,0);
  MyBot.gripPercentOpen(70);
  MyBot.drive('S');

  // goin' for it double time!
  MyBot.move(100,-3);
  delay(1150);

  // re-engage track again
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('S');

  // open gripper
  MyBot.gripOpen();
  MyBot.backup(55,300);

  // just gotta get the one the other guy probably got first
  MyBot.drive('U');
  MyBot.drive('L');
  MyBot.drive('F');
  MyBot.drive('F');
  MyBot.drive('L');
  MyBot.drive('S');

  // close gripper
  MyBot.gripClose();
  
  // home stretch, if the other guy wasn't trying too hard.
  MyBot.drive('U');
  MyBot.drive('R');
  MyBot.drive('R'); 
  MyBot.drive('S'); 
  
  // open gripper
  MyBot.gripOpen();
  MyBot.backup(80,600); // bigger back up

  // is this a victory dance??
  MyBot.move(100,100);
  delay (3000);
  MyBot.move(0,0);

  // stop and wait forever
  while(1);

  
}
