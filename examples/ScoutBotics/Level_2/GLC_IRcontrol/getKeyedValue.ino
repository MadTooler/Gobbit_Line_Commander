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
 

  if (initialValue > -1){
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
          if(newValue > 65535) newValue = 65535;
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

