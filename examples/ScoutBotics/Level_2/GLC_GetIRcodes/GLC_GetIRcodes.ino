/*  10/22/2018
   This program uses the IRremote library to find IR codes with the
   Gobbit robot wired and configured for use by the GLC_IRcontrol examples.
   
   Upload this program then use the serial monitor to test and capture IR codes.
   
      Make sure the baud in the serial monitor is set to 9600

   This is a small mod of the original example:

      IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
      An IR detector/demodulator must be connected to the input RECV_PIN.
      Version 0.1 July, 2009
      Copyright 2009 Ken Shirriff
      http://arcfn.com
*/

#include <IRremote.h>


// Here we are using an IO pin to supply the IR receiver with +5v.
// This is a shortcut for low mAmp power where you may have available IO pins and
// where the +5v is already used and you do not have a breadboard or splitter.
//   Comment out this next line if you do not want to use an IO pin as supply.
#define IR_POWER_PIN A0  // declare pin number used for IR receiver power

int RECV_PIN = A1;  // declare pin number connected to signal pin of IR receiver


IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{

  // If IR_POWER_PIN was defined, this will set the pin to power the IR receiver,
#ifdef IR_POWER_PIN
  pinMode(IR_POWER_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);
#endif

  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
