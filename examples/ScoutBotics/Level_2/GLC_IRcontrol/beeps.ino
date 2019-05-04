/* 
  Beeps here use cheapBeep() to signal robot recognition of varying inputs and/or status
  
  Signal beeps in the various modes use either cheapBeep() directly or one of the 
  additional beep function to indicate:
    -mode entered
    -valid button press recognized
    -cancel recording step/command before enter pressed
    -center veer position if adjusting/trimming back and forth
    -step recorded after enter pressed
    -end of recorded program
    -canceled mode and back at main mode select

*/

  

// main beeper function that does not use any additional timers
void cheapBeep(int beepTime) {
  // with 500 microseconds up and down, = 1000 microseconds = 1 millisecond per pulse = 1000 hertz
  for (int i = 0; i < beepTime; i++) {
    digitalWrite(PIEZO_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIEZO_PIN, LOW);
    delayMicroseconds(500);
  }
}




void modeStartedBeep(void){
  cheapBeep(100);
  delay(170);
  cheapBeep(40);
  delay(40);
  cheapBeep(40);
  delay(170);
  cheapBeep(100);
}




void cancelStepBeep(void){
  for (int i = 0; i < 10; i++) {
    cheapBeep(40);
    delay(40);
  }
}




void stepRecordedBeep(void){
  cheapBeep(200);
  delay(100);
  cheapBeep(200);
}




void playBackDoneBeep(void){
  cheapBeep(40);
  delay(170);
  cheapBeep(40);
  delay(40);
  cheapBeep(40);
  delay(40);
  cheapBeep(40);
  delay(170);
  cheapBeep(40);
}




void cancelSubBeep(void){
  for (int i = 0; i < 3; i++) {
    cheapBeep(40);
    delay(40);
  }
}




void mainMenuBeep(void){
  cheapBeep(400);
}
