// GLC Simple obstacle avoidance WITH LED lit and piezo sensor impact target
// 10/28/2017

// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

#define SONAR_PIN A3 // analog pin used for sensor 
#define CLEARANCE_DISTANCE 12 // value in inches
#define BEEPER_PIN A0 // Beeper and Target Green LED
#define TARGET_HOT_PIN A1  // Target Red LED
#define PIEZO_SENSOR_PIN A2 // Piezo input
#define RUN_TIME 30000 // Time in milliseconds for the round
#define ACTIVE_TIME 1500 // Time in milliseconds for the target to be active after being enabled
#define BETWEEN_HITS_TIME 2500 // Time in milliseconds for the target to be disabled after being hit or after ACTIVE_TIME expires

#include <GobbitLineCommand.h>
GobbitLineCommand MyBot;

#include <PinChangeInterrupt.h>

volatile byte targetHits = 0; // counter for number of hits
volatile byte hit = 0; // flag if just got hit
volatile byte activeTarget = 0; // flag if target is active and ready for hit or disabled
volatile unsigned long mTimeLastHit = -BETWEEN_HITS_TIME; // timer reference to see how long since last hit
volatile unsigned long mTimeLastActivated = 0; // timer reference to see how long target has been active
unsigned long timesUp = 0;


void setup() {

  pinMode(PIEZO_SENSOR_PIN, INPUT);
  // Attach the new PinChangeInterrupt and enable event function below
  attachPCINT(digitalPinToPCINT(PIEZO_SENSOR_PIN), gotHit, CHANGE);

  MyBot.setBeeperPin(BEEPER_PIN);
  pinMode(TARGET_HOT_PIN, OUTPUT);

  // Set the Sonar Pin and Safe Distance (followLine and drive functions use the Safe Distance value).
  // Since the safe distance is not used by the functions we will call here,
  // any value can be entered.  We will just use 0.
  MyBot.setSonar(SONAR_PIN, 0);

  // initialize the robot
  MyBot.beginGobbit();

  targetEnable();
}

void loop() {

  timesUp = millis();

  while ((millis() - timesUp) < RUN_TIME) {

    // check if there has been a hit and if so do the dance
    hitDance();

    // if the distance is greater than CLEARANCE_DISTANCE just drive forward ast 100% motor speed
    if (MyBot.readSonarInches() > CLEARANCE_DISTANCE) MyBot.move(100, 0);

    else
      // The distance must be less than CLEARANCE_DISTANCE so turn until
      // the distance is clear for more than the CLEARANCE_DISTANCE
    {
      //turn/spin Right at 50% of motor speed
      MyBot.move(50, 100);

      // keep turning until the distance is more than CLEARANCE_DISTANCE
      while (MyBot.readSonarInches() < CLEARANCE_DISTANCE) {

        // check if there has been a hit and if so do the dance
        hitDance();
        // else do nothing
      }
    }
    targetEnable();
  }


  // time is up, stop and show score

  targetDisable();
  // stop the course
  MyBot.setMotors(0, 0);

  // blink the score
  scoreCard();
}

// interrupt program on sensor pin state change
// DO NOT USE MOTOR COMMANDS HERE
void gotHit(void)
{

  if (activeTarget)
  {
    // set flag
    hit = 1;

    targetHits++;
    targetDisable();
    mTimeLastHit = millis();
  }
}


void targetDisable(void)
{
  // turn off Target RED LED
  digitalWrite(TARGET_HOT_PIN, LOW);
  activeTarget = 0;
}

void targetEnable(void)
{
  if (activeTarget && ((millis() - mTimeLastActivated) > ACTIVE_TIME))
  {
    targetDisable();
    mTimeLastHit = millis();
  }

  else if (!activeTarget && ((millis() - mTimeLastHit) > BETWEEN_HITS_TIME))
  {
    // turn on Target RED LED
    digitalWrite(TARGET_HOT_PIN, HIGH);
    activeTarget = 1;
    mTimeLastActivated = millis();
  }
}

void scoreCard(void)
{
  // disconnect interrupt so no more hits will be counted
  detachPCINT(digitalPinToPCINT(PIEZO_SENSOR_PIN));

  // blink the led for the score forever
  while (1)
  {
    //blink for number of hits
    for (int i = 0; i < targetHits; i++)
    {
      // Switch Led state
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }

    // wait a little bit before blinking number of hits again
    delay(1500);
  }
}

void hitDance(void)
{

  if (hit) {

    // this is done in beep if told to wait
    //MyBot.move(0, 0);

    MyBot.beep(4, 150, 1);

    //turn/spin Left at 100% of motor speed
    MyBot.move(100, -100);

    delay(400);

    MyBot.move(0, 0);

    // reset flag
    hit = 0;
  }


}

