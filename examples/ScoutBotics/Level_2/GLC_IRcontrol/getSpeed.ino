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
