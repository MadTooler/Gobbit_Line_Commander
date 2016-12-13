#include <GobbitLineCommand.h>

GobbitLineCommand MyBot;

void setup() {
  // put your setup code here, to run once:

  MyBot.readBatteryVolts(A5,33,100);
  
  // attaches the servo on pin 14 which is A0 and sets the angles for Open and Closed Postions
  MyBot.setGripPinOpenClosed(A0,180,68);  
 
  MyBot.beginGobbit();

  // open gripper
  MyBot.gripOpen();

  MyBot.serialPrintCurrentSettings();

  MyBot.calibrateLineSensor(0); 

  //start move from bottom left corner facing to center
  MyBot.drive('F');
  MyBot.drive('L');

}

void loop() {
  
  // check the voltage of the battery and stop if too low
  //MyBot.checkBattery(A5,3.25*3,33,100); // 3cell
  MyBot.checkBattery(A5,3.25*2,33,100); // 2cell
  
  //MyBot.followLine();

  // first move

  MyBot.drive('L');
  MyBot.drive('S');

  // close gripper
  MyBot.gripClose();
  
  MyBot.drive('U');
  MyBot.drive('R');
  MyBot.drive('R');
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();
  MyBot.backup(55,300); 

// second move
  MyBot.drive('U');
  MyBot.drive('L');
  MyBot.drive('R');
  MyBot.drive('S');
  
  // close gripper
  MyBot.gripClose();
  
  MyBot.drive('U');
  MyBot.drive('F');
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();
  MyBot.backup(55,300); 


// third move
  MyBot.drive('U');
  MyBot.drive('R');
  MyBot.drive('L');
  MyBot.drive('S');
  
  // close gripper
  MyBot.gripClose();
  
  MyBot.drive('U');
  MyBot.drive('R');
  MyBot.drive('R');
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();
  MyBot.backup(55,300);


// fourth move
  MyBot.drive('U');
  MyBot.drive('L');
  MyBot.drive('R');
  MyBot.drive('S');
  
  // close gripper
  MyBot.gripClose();
  
  MyBot.drive('U');
  MyBot.drive('F');
  MyBot.drive('S');
  
  // open gripper
  MyBot.gripOpen();
  MyBot.backup(55,300);;

 
// reset move
  MyBot.drive('U');
  MyBot.drive('R');
  
}
