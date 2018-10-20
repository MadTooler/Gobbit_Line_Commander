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

