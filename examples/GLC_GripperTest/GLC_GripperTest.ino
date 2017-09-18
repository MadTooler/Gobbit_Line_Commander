/*  09/18/2017
 *  
 *  This program will help you setup and test the gripper with the Gobbit robot.
 *  We will use the GobbitLineCommand library, but you can also directly
 *  use the gripper with the standard servo library.
 *  
 *  When this runs, the gripper should 
 *  
 *    Open
 *    Close a little bit
 *    Close all the way
 *    Open a little bit
 *    Repeat
 *    
 *  Read the comments at each step below to determine if your gripper is properly setup.
 *
 *  To see a video using this sketch:  https://youtu.be/Hh2pGYhL6_o?list=PLawtTl1V11Fz_OZYQnIneA7F-aO4kcc6E&t=62
 *  
*/

#include <GobbitLineCommand.h>

// Declare an object name for your robot.  Here we are calling ours MyBot.
GobbitLineCommand MyBot;

void setup() {

  // Setup the gripper.
  // The first variable is the pin the servo is attached (we are using A0 here), 
  // the next variable is the angle for the Open position (start with 180), 
  // then the angle for the Closed Postion (start around 85).
  // The Closed position may vary between about 70 and 85, depending on where you set
  // the drving gripper arm.
  MyBot.setGripPinOpenClosed(A0,180,85);  

}

void loop() {

  // Open gripper.
  MyBot.gripOpen();
  
  // Wait 1.5 seconds (1500 milliseconds)
  delay(1500);

  // Set the gripper to 98% open to see if the chosen degree for the open position is good.
  // If it is, the gripper should just barely close a little.
  // If not, you need to make the open angle you set with MyBot.setGripPinOpenClosed smaller.
  MyBot.gripPercentOpen(98);

  // Wait 1 second (1000 milliseconds)
  delay(1000);
  
  // Close gripper.
  // If the gripper is not all the way closed, you will need 
  // to make the close angle you set with MyBot.setGripPinOpenClosed smaller.
  MyBot.gripClose();
  
  // Wait 1.5 seconds (1500 milliseconds)
  delay(1500);

  // Set the gripper 2% open to see if the chosen degree for the closed position is good.
  // If it is, the gripper should just barely open a little.
  // If not, you need to make the close angle you set with MyBot.setGripPinOpenClosed bigger.
  MyBot.gripPercentOpen(2);

  // Wait 1 second (1000 milliseconds)
  delay(1000);
  
}
