void deBounce(void) {
  deBounce(1);
}

// deBounce the button presses and make a beep to indicate it was received
void deBounce(byte beep) {
  //tone(PIEZO_PIN, 3000);  // tone is in conflict with IR libary
  if (beep) {
    cheapBeep(60);
    delay(DEBOUNCE_DELAY);
  }
  else delay(60 + DEBOUNCE_DELAY);
  irrecv.resume(); // clear buffer of any extra reads
}
