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
