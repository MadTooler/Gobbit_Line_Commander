// these are some of the functions used to select modes, by the modes to get key strokes from the IR remote,
// and checking status while running programs




// called by main loop to set flags for mode requested
byte modeSelect(void) {

  byte mode;

  switch (lastButtonValue) {

    case 1: // RC Mode 1
    case 21:
      mode = 1;
      break;

    case 2:  // Free Program Mode 2
    case 22:
      mode = 2;
      break;

    case 3:  // Line Program Mode 3
    case 23:
      mode = 3;
      break;

    case 4:  // Trim Mode 4
    case 24:
      mode = 4;
      break;

    case 5:  // Tile Mode 5
    //case ??: // no alternate button code 
      mode = 5;
      break;

    case 11: // Back button cancels input
      mode = 0;
      deBounce(0);
      mainMenuBeep();
      return mode;
      break;

    default:
      deBounce(0);
      return 0;
      break;
  }

  deBounce(0);
  return mode;
}





long getKeyedValue() {
  return getKeyedValue(-1);
}

// initialValue as -1 if no prior received key press.  Otherwise send lastButtonValue
long getKeyedValue(int initialValue) {

  deBounce();

  byte  entries[10];
  int numberEntries = 0;
  long newValue = 0;
  long multiplyBy = 1;


  if (initialValue > -1) {
    entries[numberEntries] = lastButtonValue;
    numberEntries++;
  }

  while (1) { // loop until it either receives a return or back
    if (buttonDecode()) {
      //Serial.println(lastButtonValue);
      switch (lastButtonValue) {

        case 0 ...9:
          entries[numberEntries] = lastButtonValue;
          deBounce();
          break;

        case 10 : // ENTER button when finished entering values
          //Serial.println("number of entries");
          //Serial.println(numberEntries);
          for (int i = numberEntries - 1; i >= 0 ; i--) {
            newValue = newValue + (entries[i] * multiplyBy);
            multiplyBy = multiplyBy * 10;
          }

          //constrain(newValue, 0, 65535);  // may not be working per serial print
          if (newValue > 65535) newValue = 65535;
          else if (newValue < 0) newValue = 0;
          //Serial.println("newValue in getKeyedValue");
          //Serial.println(newValue);
          deBounce();
          return newValue;
          break;

        case 11 : // BACK button, cancel process
          deBounce(0);
          cancelSubBeep();
          return -1;
          break;

        default:
          // keep the index from tracking an invalid value
          numberEntries--;
          deBounce(0);
          break;
      }
      numberEntries++;
    }
  }
}





int getSpeed() {
  return getSpeed(-1);
}

// gets speed from keyed value or returns last speed if keyed is null
// initialValue as -1 if no prior received key press.  Otherwise send lastButtonValue
int getSpeed(int initialValue) {

  unsigned int newSpeed = getKeyedValue(initialValue);

  if (newSpeed < 0) {
    return currentSpeed;
  }

  constrain(newSpeed, 0, 100);
  //Serial.println(newSpeed);
  return newSpeed;
}





void deBounce(void) {
  deBounce(1);
}

// deBounce the button presses and make a beep to indicate it was received
void deBounce(byte beep) {
  //tone(PIEZO_PIN, 3000);  // tone is in conflict with IR library
  if (beep) {
    cheapBeep(60);
    delay(DEBOUNCE_DELAY);
  }
  else delay(60 + DEBOUNCE_DELAY);
  irrecv.resume(); // clear buffer of any extra reads
}





// check if it is still OK to keep playing the recording.
// if there are any stop commands, it will return 0 and stop the motors
int keepGoing(void) {
  if (buttonDecode()) {
    if (lastButtonValue == 16 || lastButtonValue == 11 || lastButtonValue == 26) { // back button cancels input of step
      MyBot.move(0, 0);
      deBounce(0);
      cancelStepBeep();
      return 0;
    }
  }
  else return 1;
}
