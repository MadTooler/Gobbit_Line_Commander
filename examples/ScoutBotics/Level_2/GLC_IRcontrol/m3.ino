// Mode 3, Line following with intersection detection based programming

// See video example: https://www.youtube.com/watch?v=0xryqQvQNyA
  
/*  USING THE MODE

      Start by entering Mode 3 from the main control menu by pressing the number 3 key
        (If another mode is active, press the back button several times until no mode is active)

        NOTE:   MAKE SURE THE ROBOT IS SITTING ON THE LINED COURSE BEFORE ENTERING THE MODE.  IT NEEDS
                TO CALIBRATE WHEN IT FIRST ENTERS THE MODE.

      Begin programming steps

        If Forward/Left Turn/Right Turn/Turn around(down button):
          1) Press the directional key desired
          2) Enter the speed
            If you want it to run at the speed of the last command, simply press enter
            Else If you want a new speed, use the number pad buttons for 0-100 then press enter
              NOTE: The initial speed at the first step is 100 (full speed) if no speed is entered.

        Else If Delay:
          1) Enter the time in milliseconds with the number pad then press enter
            NOTE: Maximum time value should be less than what can be stored in an unsigned int value = 65,535

        Else if Stop:
          1) Press the stop direction button
          2) Press enter

      When done, press the enter button again and the program will run.

      To exit the mode, press the back button until the main menu beep is heard.


      LIMITATIONS:  This Mode does not provide any off the line navigation.

                    Any speed values less than 100 may not work well. The PID values and/or the
                    formulas inside the GLC library will need work for this functionality to
                    perform at an acceptable level.

      NOTES:  The robot will continue your last command at the end of your program.
              Make sure to give it a stop command at the end if your program.

              OR press the stop button when you want it to cancel all the current motion.

              The Stop button will also cancel a running program.

              Do NOT enter new commands while the robot is executing commands until the
              last command has been executed.

              It is not advisable to make your last command a delay.

              During a command entry, the back button can be used to cancel the current entry.

              The programmed steps can be run again, or added onto while you are still in Mode 2.
              Exiting with the back key to the main menu will delete the program.

              This program will allow for 50 steps to be recorded.
              Change the array dir[50] declaration size to increase the number of steps, but
              memory may be exceeded with unexpected results if you push it too far.

              See the Config.h tab to adjust the calibration speed. 

              If the robot is moving very roughly, it may not have calibrated well.  
              Try turning it off, turn it on, reset it over the line, then enter the mode again.
                
              Signal beeps used in mode:
                -mode entered
                -valid button press recognized
                -cancel recording step/command before enter pressed
                -center veer position if adjusting/trimming back and forth
                -step recorded after enter pressed
                -end of recorded program
                -canceled mode and back at main mode select
        
*/



void modeThree(void) {
 
  // the robot will calibrate as soon as the mode is entered if it was not already calibrated
  if(!sensorCalibrated){
    MyBot.calibrateLineSensor(CALIBRATE_SPEED);    
    sensorCalibrated = 1;
  }

  // The dir variable will store the direction that the robot should take at the next intersection or end of line.
  // It is stored as an array of characters, each of which represents one of
  // the following:
  //  'L' for turn left
  //  'R' for turn right
  //  'F' for go forward/findNext (going straight through an intersection)
  //  'B' for making a U-turn (not always smart enough to be correct)
  //  'S' for stop
  //  'D' delay, probably should not do this after a "Forward"
  char dir[50] = "";

  // time in milliseconds to run current step
  // max time = 65,535 milliseconds = 65.535 seconds
  unsigned int duration[50];

  // speed for current program step
  byte speedThisStep[50];

  // the number of recorded steps
  byte recordLength = 0;

  // flag if play of recorded steps to continue or cancel
  byte keepPlaying = 0;

  // flag to keep waiting for rest of step entries
  byte waitForEntry = 0;

  // flag to note if a step was correctly recorded or aborted
  byte receivedStep = 0;

  while (mode) {

    // record steps
    if (keepPlaying == 0) {
      if (buttonDecode()) {

        long currentDuration = 0;
        int speedEntry = 0;

        switch (lastButtonValue) {

          case 12: // forward/findNext
            dir[recordLength] = 'F';
            deBounce();

            // wait for other values for command
            waitForEntry = 1;
            while (waitForEntry) {
              if (buttonDecode()) {
                switch (lastButtonValue) {

                  case 0 ...9: // speed is being entered
                    speedEntry = getKeyedValue(lastButtonValue);
                    if (speedEntry > -1) { // -1 means it was canceled within the key value process
                      constrain(speedEntry, 0, 100);
                      currentSpeed = speedEntry;
                      receivedStep = 1;
                    }
                    break;

                  case 10 : // enter pressed so same speed as last step
                    deBounce();
                    receivedStep = 1;
                    break;

                  default: // bad entry... ignore
                    deBounce(0);
                    break;

                  case 11: // back button... cancel/leave entry
                    waitForEntry = 0;
                    deBounce(0);
                    break;
                }

                if (receivedStep) {
                  duration[recordLength] = 0;
                  speedThisStep[recordLength] = currentSpeed;
                  recordLength++;
                  stepRecordedBeep();
                  waitForEntry = 0;
                  receivedStep = 0;
                }
                else if (waitForEntry == 0) { // indicate step was canceled and/or null
                  deBounce(0);
                  cancelStepBeep();
                }
              }
            }
            break;

 
          case 13: // backward
          case 14:  // right
          case 15:  // left
          case 16:  // stop
            if (lastButtonValue == 13) dir[recordLength] = 'B';
            else if (lastButtonValue == 14) dir[recordLength] = 'R';
            else if (lastButtonValue == 15) dir[recordLength] = 'L';
            else dir[recordLength] = 'S';
            deBounce();

            // wait for other values for command (either enter or back/cancel)
            waitForEntry = 1;

            while (waitForEntry) {
              if (buttonDecode()) {
                if (lastButtonValue == 10) {
                  duration[recordLength] = 0;
                  speedThisStep[recordLength] = 0;
                  recordLength++;
                  deBounce();
                  stepRecordedBeep();
                  waitForEntry = 0;
                }
                else if (lastButtonValue == 11) { // back button... cancel/leave entry
                  waitForEntry = 0;
                  deBounce(0);
                  cancelStepBeep();
                }
                else {
                  deBounce(0);  // not a valid button so ignore
                }
              }
            }
            break;


          case 0 ...9:  // if numbers then start delay recording
            currentDuration = getKeyedValue(lastButtonValue);
            if (currentDuration > -1) { // -1 means it was canceled within the key value process
              dir[recordLength] = 'D';
              duration[recordLength] = currentDuration;
              speedThisStep[recordLength] = currentSpeed;
              recordLength++;
              deBounce(0);
              stepRecordedBeep();
            }
            else {
              deBounce(0);
              cancelStepBeep();  // indicate step was canceled and/or null
            }
            break;

          case 10: // enter button or
          case 25: // play program button to start recorded program
            keepPlaying = 1;
            deBounce();
            break;

          case 11: // Back button cancels mode
            mode = 0;
            currentSpeed = 100;
            deBounce(0);
            mainMenuBeep(); // indicates mode was exited
            break;

          default: // bad entry... ignore
            deBounce(0);
            break;
        }
      }
    }


    // play the recorded steps
    else if (keepPlaying) {
      // reset the found flags
      // this means the robot can re-run the program with the same turn actions
      MyBot.foundLeft = 0;
      MyBot.foundRight = 0;
      MyBot.foundForward = 0;
      MyBot.foundEnd = 0;
      MyBot.foundMark = 0;

      for (int i = 0; i < recordLength; i++)
      {
      
        MyBot.setMinMaxSpeeds(-speedThisStep[i], speedThisStep[i]);

        unsigned long currentMillis = millis();
        unsigned long lastMillis = currentMillis;

        if (keepPlaying) {
          switch (dir[i]) {

            case 'L' : // left
              MyBot.turn('L');
              break;

            case 'R' : // right
              MyBot.turn('R');
              break;

            case 'F' : // forward/findNext intersection or end of line
              if(!MyBot.foundEnd) keepPlaying = findNext(keepPlaying);
              else {
                notFoundSpin();
                keepPlaying = 0;
              }
              break;

            case 'B' : // U turn/ go back the way it came
              
              // if there are no found flags, as with an initial back command before any following,
              // assume the robot is on a line, not an intersection, and make a single turn
              if(!MyBot.foundRight && !MyBot.foundLeft && !MyBot.foundEnd) MyBot.turn('L');
              
              // else at least one flag was found so figure out the best turn
              
              else if (!MyBot.foundRight) {
                MyBot.turn('R');
              }
              else if (!MyBot.foundLeft) {
                MyBot.turn('L');
              }
              else { // must be at a tee or full intersection
                MyBot.turn('U'); // this will make two lefts but smoother since it will not slow down at the first
              }
              break;

            case 'S' : // stop
              MyBot.brakeMotors();
              break;

            case 'D': // play a delay while checking if a cancel or stop was pressed
              currentMillis = millis();
              lastMillis = currentMillis;
              while ((currentMillis - lastMillis < duration[i]) && keepPlaying) {
                keepPlaying = keepGoing();
                currentMillis = millis();
              }
              break;

          }
        }

        if (keepPlaying) keepPlaying = keepGoing();

        // leave play loop if found a stop IR signal
        if (!keepPlaying) i = recordLength;
      }
      playBackDoneBeep();
      keepPlaying = 0;
    }

  }

}


// spin in error if cannot make requested turn at found intersection
void notFoundSpin(void) {
  MyBot.move(100, 100);
  delay(1000);
  MyBot.move(0, 0);
}

// follow the line until next intersection or end of line is found while a stop remote command has not been seen
byte findNext(byte _keepPlaying) {
  MyBot.followLine(2);  // clears flags
  while (!MyBot.foundLeft && !MyBot.foundForward && !MyBot.foundRight && !MyBot.foundEnd && _keepPlaying) {
    MyBot.followLine(2);
    _keepPlaying = keepGoing();
    }
    return _keepPlaying;
}
