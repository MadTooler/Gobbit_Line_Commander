/* 04/19/2018

  This program will use the Gobit robot as a repeater of knocks it senses. 
  Whatever beat you knock on the robot's surface that has the piezo, it will play back in beeps.

  The Gobbit robot only needs an arduino Uno, a piezo beeper (passive), a 1 MegaOhm resistor, 
  and maybe a couple of jumpers for this project.

  Wire the resistor between the leads of the piezo.
  Attach the black wire of the piezo to ground.
  Attachd the red wire of the piezo to an analog pin.
  Screw or double stick tape the piezo to the surface you will be knocking on.
  Update the define values to match your setup and tunings.
*/


#define PIEZO_PIN A2  // piezo beeper/sensor pin

#define MAXKNOCKS 50  // maximum number of knocks that will be recorded at a time

#define MAXGAP 2000 // maximum number of milliseconds to wait for a following knock before playback starts

#define BEEP_LENGTH 40// duration in milliseconds of the beep sound

#define KNOCK_LEVEL 40// analog read value threshold to detect a knock

unsigned long previousMillis = 0;
unsigned long timeGaps[MAXKNOCKS];
unsigned long currentMillis = millis();

void setup() {

  Serial.begin(9600);

  // set the beeper as an output
  pinMode(PIEZO_PIN, OUTPUT);

  cheapBeep(BEEP_LENGTH);
  delay(100);
  cheapBeep(BEEP_LENGTH);
  delay(100);
  cheapBeep(BEEP_LENGTH);
  
  // a little delay after beeping before making the sensor an input
  delay(200);

  pinMode(PIEZO_PIN, INPUT);
}

void loop() {

  if (analogRead(PIEZO_PIN) > KNOCK_LEVEL) {

    byte keepRecording = 1;  // flag to indicate if it is time to playback
    int i = 0; // counter
    currentMillis = millis();
    previousMillis = currentMillis;

    while (keepRecording && i < MAXKNOCKS ) {

      if (currentMillis - previousMillis > MAXGAP) {
        // exit and replay the recording
        keepRecording = 0;
      }

      else if (analogRead(PIEZO_PIN) > KNOCK_LEVEL || i == 0) {
        // get the duration since last knock
        if (i > 0) {
          timeGaps[i - 1] = currentMillis - previousMillis;
          previousMillis = currentMillis;
        }

        i++;
        delay(90);  // debounce
      }

      // get the current time
      currentMillis = millis();
    }

    pinMode(PIEZO_PIN, OUTPUT);

    Serial.print(i);
    Serial.println(" knocks heard");

    for (int k = 0; k < i; k++) {
      Serial.print("Repeat beep ");
      Serial.println(k + 1);

      cheapBeep(BEEP_LENGTH);

      if (k < i - 1) {
        long int gap;
        if (timeGaps[k] < BEEP_LENGTH) gap = timeGaps[k];
        else gap = timeGaps[k] - BEEP_LENGTH;
        delay(gap);
      }

    }

    // a little delay after beeping before back to input
    delay(200);
    pinMode(PIEZO_PIN, INPUT);

  }
}

// beeper function that does not use any additional timers
// cheap since it does not cost too much on resources, but it is super limited
void cheapBeep(int beepTime) {
  // with 500 microseconds up and down, = 1000 microsends = 1 millisecond per pulse = 1000 hertz
  for (int i = 0; i < beepTime; i++) {
    digitalWrite(PIEZO_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(PIEZO_PIN, LOW);
    delayMicroseconds(500);
  }
}
