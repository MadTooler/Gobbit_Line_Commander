// Mode 2, Dead Reckoning programming

// See video examples: https://www.youtube.com/watch?v=o2p5wF-MNAs
//                     https://www.youtube.com/watch?v=ZEsQPePo8oQ

/*  USING THE MODE

      Start by entering Mode 2 from the main control menu by pressing the number 2 key
        (If another mode is active, press the back button several times until no mode is active)

      Begin programming steps

        If Forward/Backward:
          1) Press Forward or Backward
          2) If a veering path is desired,
            Press the left/right arrow
              0 times leaves direction as straight forward/backward
              1 time is minor veer
              2 times is moderate veer (same as Mode1 button 1,3,7, and 9)
              3 times is major veer (may leave one wheel none turning)
              NOTE:  you can press the other direction as well to go back to center or to the other direction
          3) Enter the speed
            If you want it to run at the speed of the last command, simply press enter
            Else If you want a new speed, use the number pad buttons for 0-100 then press enter
            NOTE: The initial speed at the first step is 100 (full speed) if no speed is entered.

        Else if Left/Right:
          1) Press Left/Right
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
      

      NOTES:  
              The robot will continue your last command at the end of your program.
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

              See the Config.h tab to adjust the speeds of the Turning and Veering 
              (keys 1,3,7,9 or the FORWARD_RIGHT,FORWARD_LEFT,BACKWARD_RIGHT,BACKWARD_RIGHT)
			  			  
              Signal beeps used in mode:
                -mode entered
                -valid button press recognized
                -cancel recording step/command before enter pressed
                -center veer position if adjusting/trimming back and forth
                -step recorded after enter pressed
                -end of recorded program
                -canceled mode and back at main mode select
				
*/


void modeTwo(void) {
  // The dir variable will store the direction that the robot should take.  It
  // is stored as an array of characters, each of which represents one of
  // the following:
  //  'L' for left spin
  //  'R' for right spin
  //  'F' for go forward, veering strength is set by steer[] array
  //  'B' for backward, veering strength is set by steer[] array
  //  'S' for stop
  //  'D' delay
  char dir[50] = "";

  // time in milliseconds to run current step
  // max time = 65,535 milliseconds = 65.535 seconds
  unsigned int duration[50];

  // speed for current program step
  byte speedThisStep[50];

  // steering veer strength for current step
  //   value -100 to +100 where
  //     0 is straight and
  //     +/-100 is full spin left or right
  char steer[50];

  // the number of recorded steps
  byte recordLength = 0;

  // flag if play of recorded steps to continue or cancel
  byte keepPlaying = 0;

  // flag to keep waiting for rest of step entries
  byte waitForEntry = 0;

  // flag to note if a step was correctly recorded or aborted
  byte receivedStep = 0;

  while (mode) {

    /*
      Key codes
      0 ZERO_BUTTON
      1 ONE_BUTTON
      2 TWO_BUTTON
      3 THREE_BUTTON
      4 FOUR_BUTTON
      5 FIVE_BUTTON
      6 SIX_BUTTON
      7 SEVEN_BUTTON
      8 EIGHT_BUTTON
      9 NINE_BUTTON
      10 ENTER_BUTTON
      11 BACK_BUTTON

      12 FORWARD_BUTTON
      13 BACKWARD_BUTTON
      14 RIGHT_BUTTON
      15 LEFT_BUTTON
      16 STOP_BUTTON

      17 FORWARD_RIGHT
      18 FORWARD_LEFT
      19 BACKWARD_RIGHT
      20 BACKWARD_LEFT
      21 RC_MODE
      22 FREE_PROGRAM_MODE
      23 LINE_PROGRAM_MODE
      24 TRIM_MODE
      25 PLAY_PROGRAM
      26 STOP_PROGRAM
    */


    // record steps
    if (keepPlaying == 0) {
      if (buttonDecode()) {

        long currentDuration = 0;
        int speedEntry = 0;
        int veerStrength = 0;
        byte veerLoop = 0;

        switch (lastButtonValue) {

          case 12: // forward
          case 13: // backward
            veerStrength = 0;
            if (lastButtonValue == 12) dir[recordLength] = 'F';
            else dir[recordLength] = 'B';
            deBounce();

            // wait for other values for command
            waitForEntry = 1;
            while (waitForEntry) {
              if (buttonDecode()) {
                switch (lastButtonValue) {

                  case 14: // right veer
                    if (veerStrength < 3) {
                      veerStrength++;
                      if(veerStrength == 0){
                        cheapBeep(30);
                        delay(35);
                        cheapBeep(30);
                        deBounce(0);
                      }
                      else deBounce();
                    }
                    else deBounce(0);
                    break;

                  case 15: // left veer
                    if (veerStrength > -3) {
                      veerStrength--;
                      if(veerStrength == 0){
                        cheapBeep(30);
                        delay(35);
                        cheapBeep(30);
                        deBounce(0);
                      }
                      else deBounce();
                    }
                    else deBounce(0);
                    break;

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
                  switch (veerStrength) {

                    case -1:
                      veerStrength = -MINOR_VEER_STRENGTH;
                      break;

                    case -2:
                      veerStrength = -MODERATE_VEER_STRENGTH;
                      break;

                    case -3:
                      veerStrength = -MAJOR_VEER_STRENGTH;
                      break;

                    case 1:
                      veerStrength = MINOR_VEER_STRENGTH;
                      break;

                    case 2:
                      veerStrength = MODERATE_VEER_STRENGTH;
                      break;

                    case 3:
                      veerStrength = MAJOR_VEER_STRENGTH;
                      break;
                  }
                  steer[recordLength] = veerStrength;
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

          case 14:  // right
          case 15:  // left
            if (lastButtonValue == 14) dir[recordLength] = 'R';
            else dir[recordLength] = 'L';
            deBounce();

            // wait for enter to store step or other key to cancel it
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
                  steer[recordLength] = 0;
                  recordLength++;
                  stepRecordedBeep();
                  waitForEntry = 0;
                }
                else if (waitForEntry == 0) { // indicate step was canceled and/or null
                  deBounce(0);
                  cancelStepBeep();
                }
              }
            }
            break;

          case 16:  // stop
            deBounce();
            waitForEntry = 1;

            while (waitForEntry) {
              if (buttonDecode()) {
                if (lastButtonValue == 10) {
                  dir[recordLength] = 'S';
                  duration[recordLength] = 0;
                  speedThisStep[recordLength] = 0;
                  steer[recordLength] = 0;
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
              steer[recordLength] = 0;
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
      for (int i = 0; i < recordLength; i++)
      {
        unsigned long currentMillis = millis();
        unsigned long lastMillis = currentMillis;

        switch (dir[i]) {

          case 'L' : // left spin
            MyBot.move(speedThisStep[i], -100);
            break;

          case 'R' : // right spin
            MyBot.move(speedThisStep[i], 100);
            break;

          case 'F' : // forward
            MyBot.move(speedThisStep[i], steer[i] + steerTrimF);
            break;

          case 'B' : // backward
            MyBot.move(-speedThisStep[i], steer[i] + steerTrimB);
            break;

          case 'S' : // stop
            //MyBot.move(0, 0);
            MyBot.brakeMotors();
            break;

          case 'D': // play a delay while checking if a cancel or stop was pressed
            while ((currentMillis - lastMillis < duration[i]) && keepPlaying) {
              keepPlaying = keepGoing();
              currentMillis = millis();
            }
            break;

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
