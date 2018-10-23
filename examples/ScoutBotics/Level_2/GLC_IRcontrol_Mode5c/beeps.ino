//**** confirm comments and order
  
  // varying beeps to signal:
  //    button press recognized
  //    cancel within a command
  //    back at main mode select
  //    mode entered
  //    center veer position if adjusting back and forth
  //    cancel a step
  //    end of program
  

// main beeper function that does not use any additional timers
void cheapBeep(int beepTime) {
  // with 500 microseconds up and down, = 1000 microsends = 1 millisecond per pulse = 1000 hertz
  for (int i = 0; i < beepTime; i++) {
    digitalWrite(PIEZO_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIEZO_PIN, LOW);
    delayMicroseconds(500);
  }
}



void cancelSubBeep(void){
  for (int i = 0; i < 3; i++) {
    cheapBeep(40);
    delay(40);
  }
}



void cancelStepBeep(void){
  for (int i = 0; i < 10; i++) {
    cheapBeep(40);
    delay(40);
  }
}



void mainMenuBeep(void){
  cheapBeep(400);
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



void modeStartedBeep(void){
  cheapBeep(100);
  delay(170);
  cheapBeep(40);
  delay(40);
  cheapBeep(40);
  delay(170);
  cheapBeep(100);
}
