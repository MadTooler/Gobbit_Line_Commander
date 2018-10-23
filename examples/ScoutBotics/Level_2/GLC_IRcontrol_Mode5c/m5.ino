void modeFive(void) {

// this will receive tile commands and run them
  




  /* recording method

      Start by entering mode5 from the main control menu
        (Hit the back button several times until no mode is active)

      Begin programming steps

****

      When done, press the enter button again and the program will run.

      NOTES:  The robot will stop at the next transition found after executing all of the called tiless.

              No speed or delay functionality is in this mode.
              
              The Stop button will cancel the running program.

              Do NOT enter new commands while the robot is executing commands until the
              last command has been executed.

              During a command entry, the back button can be used to cancel the current entry.

              The programmed steps can be run again, or added onto while you are still in Mode 5.
              Exiting with the back key to the main menu will delete the program.

              Signal beeps used in mode:
                -mode entered
                -valid button press recognized
                -cancel a step before enter pressed
                -step recorded after enter pressed
                -end of recorded program

  */

  // the robot will calibarate as soon as the mode is entered
  MyBot.calibrateLineSensor(CALIBRATE_SPEED);  

  // The tileNumber variable will store the tile that the robot will need to navigate.
  // It is stored as an array of bytes (0-255), each of which represents the number/style of the tile.
  //   This will allow for 50 steps in the program.
  //   Change the array declared size to increase, but
  //   memory may be exceeded.
  byte tileNumber[50] = "";

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

        byte currentTile = 0;

        switch (lastButtonValue) {

          case 0 ...9:  // if numbers then record tile number
            currentTile = getKeyedValue(lastButtonValue);
            if (currentTile > -1) { // -1 means it was cancelled within the key value process
              tileNumber[recordLength] = currentTile;
              recordLength++;
              deBounce(0);
              stepRecordedBeep();
            }
            else {
              deBounce(0);
              cancelStepBeep();  // indicate step was cancelled and/or null
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
      
      // start following the line and find the next transition
      MyBot.followLine(3);

      // at the first transition a tile must have been entered, so start
      //   the tile navigating calls
      for (int i = 0; i < recordLength; i++)
      {

        switch (tileNumber[i]) {

          case 1 : // tile 1
            Tile1();
            break;

          case 2 : // tile 2
            Tile1();
            break;


          case 3 : // tile 3
            Tile3();
            break;

          case 4 : // tile 4
            Tile4();
            break;

          case 5 : // tile 5
            Tile5();
            break;

          case 6 : // tile 6
            Tile1();
            break;   

        }

        if (keepPlaying) keepPlaying = keepGoing();

        // leave play loop if found a stop IR signal
        if (!keepPlaying){
          i = recordLength;
          MyBot.brakeMotors();
        }       
      }
     
      // when it is here, the the tile functions have been completed 
      //   and it should stop at the next transition
      MyBot.brakeMotors();
      playBackDoneBeep();
      keepPlaying = 0;
    }

  }

}




