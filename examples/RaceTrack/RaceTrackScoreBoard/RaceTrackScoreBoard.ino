/* 10/25/2018

  This program is for a laser line start/finish line for a Gobbit robot race course with serial monitoring.

  "IRremote" library is also needed.

  ** Further documentation is not currently available. **

  NOTE:  There is an occasional 0.2 second delay for the loser of a near tie race.  
         Better timing, shorter IR codes/format, and delay may improve issue.

  IR codes are 32 bit values, noted in HEX format as 0x00000000 through 0xFFFFFFFF (decimal value 0 to 4294967294)
  We have made up codes here to broadcast and receive in NEC format.  They are not necessarily in compliance with
    the NEC code standard, but they should work just fine.
    
  It may be better to revise and use a shorter IR code format for improved speed and timing.
  
*/

// Left lane IR commands
#define LEFT_STOP 0x2BB27000
#define LEFT_BACKUP 0x2BB27002
#define LEFT_BUMP_IN 0x2BB27003

// Right lane IR commands
#define RIGHT_STOP 0x2BB28000
#define RIGHT_BACKUP 0x2BB28002
#define RIGHT_BUMP_IN 0x2BB28003

// Common IR commands
#define GO_RACE 0x2BB26001
#define ALL_STOP 0x2BB29999

#define MAX_LAPS_IN_RACE 9 // this should not be changed since more cannot be selected with current serial interface.
#define START_WAIT 3000  // time limit for a second lane to cross the line, otherwise will run as single lane race
#define LAP_COUNT_TIME_THRESHOLD 2000  // time to make a full lap should take longer than this
#define END_OF_RACE_DELAY 10000  // delay after the race has finished before it auto restarts

#define LDR_L A0
#define LDR_R A1
#define BEEP_PIN 2
#define LEFT_SERVO_PIN 9
#define GREEN_SERVO_PIN 10
#define RIGHT_SERVO_PIN 11

#define FLAG_UP_ANGLE 8
#define FLAG_SWING_ANGLE 40
#define FLAG_DOWN_ANGLE 120

#define LDR_TRIGGER_LEVEL  500 // 0-1023, probably near 400, when it goes below this level it will trigger

#include <Servo.h>

Servo leftCheckFlagServo; 
Servo greenFlagServo; 
Servo rightCheckFlagServo; 

byte start =0;  // flag for start/stop mode
byte winner = 0;  // flag that race was won by lane (1)left or (2)right
int lapCountL=-1;  // current count of completed laps left lane
int lapCountR=-1;  // current count of completed laps right lane
int totalLaps = MAX_LAPS_IN_RACE;
byte leftStillCrossingLine=0;  // flag to test if robot is still breaking the laser since the last read and has not yet moved past.
byte rightStillCrossingLine=0;  // flag to test if robot is still breaking the laser since the last read and has not yet moved past.

byte resetOnLine=1;  // flag to direct the reset on the starting line sequence

int raceMode=1; // initial race mode as default

long mlastTime = 0;  // ref last time used for timing cycles
long mLastLeftTrigger = 0;  // ref last time used for last laser beam break on left
long mLastRightTrigger = 0;  // ref last time used for last laser beam break on right
long mLeftRaceTime = 0;  // total time in race for left lane
long mRightRaceTime = 0;  // total time in race for right lane
long mLeftLapTime[MAX_LAPS_IN_RACE + 1];  // time for last lap in left lane
long mRightLapTime[MAX_LAPS_IN_RACE + 1];  // time for last lap in right lane
long mStartTime = 0;

// incoming Serial data vars
int incomingByte = 0;  //  for incoming serial data input

#include <IRremote.h>

IRsend irsend;


void setup()
{

  leftCheckFlagServo.attach(LEFT_SERVO_PIN);  
  greenFlagServo.attach(GREEN_SERVO_PIN); 
  rightCheckFlagServo.attach(RIGHT_SERVO_PIN); 

  // test up position of flags
  leftCheckFlagServo.write(FLAG_UP_ANGLE);
  greenFlagServo.write(FLAG_UP_ANGLE);
  rightCheckFlagServo.write(FLAG_UP_ANGLE);

  delay(2000);

  // put the flags away
  leftCheckFlagServo.write(FLAG_DOWN_ANGLE);
  greenFlagServo.write(FLAG_DOWN_ANGLE);
  rightCheckFlagServo.write(FLAG_DOWN_ANGLE);

  pinMode(BEEP_PIN, OUTPUT);

  Serial.begin(115200);   //start the serial monitor

  delay(1000); // time for laser light level to stabilize after servos draw power so a false trigger is not made

  Serial.println();
  Serial.println(F("Typing c and hitting SEND or enter from the terminal prompt at any time will CANCEL the current race"));
  Serial.println();
  Serial.println();
  delay(1500);
  selectRaceMode(1);
}

void loop()
{
  while(raceMode){
    if(checkIfCancel()) return;
  
    // ---- initial approach and reset of robots to start line
    if(resetOnLine && raceMode){
  
      mlastTime = 0; // clear timer reference
  
      Serial.println();
      Serial.println(F("****    ****    ****    ****    ****    ****"));
      Serial.println(F("   NEW RACE NOW STAGING AT THE START LINE"));
      Serial.println(F("****    ****    ****    ****    ****    ****"));
      Serial.println();
      
      while((lapCountR + lapCountL < 0) && (lapCountR + lapCountL > -3)){
  
        if(checkIfCancel()) return;
        
        // check if left lane has crossed line and stop it  
        if(analogRead(LDR_L) > LDR_TRIGGER_LEVEL && lapCountL < 0){
          // beep buzzer
          digitalWrite(BEEP_PIN, HIGH);
           
          // send stop signal to left lane
          irsend.sendNEC(LEFT_STOP, 32); 
          delay(20);
          irsend.sendNEC(LEFT_STOP, 32); 
          delay(20);
      
          digitalWrite(BEEP_PIN, LOW);
    
          lapCountL=0;
          
          // start timer if not already going
          if(mlastTime == 0) 
            mlastTime = millis();
  
          // Delay here is to keep a short off for beeper if both lanes are sitting at the line already.
          // Otherwise, only a long beep would be heard.
          delay(20);
        }
    
        // check if right lane has crossed line and stop it
        if(analogRead(LDR_R) > LDR_TRIGGER_LEVEL && lapCountR < 0){
          // beep buzzer
          digitalWrite(BEEP_PIN, HIGH);
           
          // send stop signal to right lane
          irsend.sendNEC(RIGHT_STOP, 32); 
          delay(20);
          irsend.sendNEC(RIGHT_STOP, 32); 
          delay(20);
      
          digitalWrite(BEEP_PIN, LOW);
    
          lapCountR=0;
  
          // start timer if not already going
          if(mlastTime == 0) 
            mlastTime = millis();
  
          // Delay here is to keep a short off for beeper if both lanes are sitting at the line already.
          // Otherwise, only a long beep would be heard.
          delay(20);
        }
  
        // check time limit for other lane to arrive before disqualifying or making it a single lane race
        if(mlastTime){
          if(millis()-mlastTime > START_WAIT){
            if(lapCountL <0)
              // -3 is indicator that this lane will not be used in race
              lapCountL=-3; 
            else
              lapCountR=-3;
          }
        }
          
      }
  
      if(lapCountL != -3)
        Serial.println(F("   LEFT  lane... will RACE!"));
      else Serial.println(F("   LEFT  lane... is a NO SHOW!"));
      Serial.println();
      if(lapCountR != -3)
        Serial.println(F("   RIGHT lane... will RACE!"));
      else Serial.println(F("   RIGHT lane... is a NO SHOW!"));
      Serial.println();
      // move the robots back and have them stop close to the line
      if(lapCountL != -3){
        leftResetAtLine();
        if(!raceMode) return;
        Serial.println(F("   LEFT  lane... is READY!"));
        Serial.println();
      }
      if(lapCountR != -3){
        rightResetAtLine();
        if(!raceMode) return;
        Serial.println(F("   RIGHT lane... is READY!"));
        Serial.println();
      }
      resetOnLine=0; 
      
      start=1;
      // wait a little while robot sits on start line before race starts
      delay(2000);
    } // end of reset
      
  
    // ---- start race
    if(start){
      // get the flag moving first
      greenFlagServo.write(FLAG_UP_ANGLE);
      delay(200);
  
      Serial.println(F("GO, GO, GO!!!!!!"));
      Serial.println();
  
      mStartTime = millis();
      
      // send start signal to robot
      irsend.sendNEC(GO_RACE, 32);
      delay(20);
      irsend.sendNEC(GO_RACE, 32);
      
      // wave the start flag
      for(int i=0;i<8;i++){
        greenFlagServo.write(FLAG_SWING_ANGLE);
        delay(200);
        greenFlagServo.write(FLAG_UP_ANGLE);
        delay(200);
      }
      greenFlagServo.write(FLAG_DOWN_ANGLE);
      start=0;
	  
	  delay(3000); // delay to allow power fluctuations from servos to stabilize and not cause false trigger
    } // end of start
    
    
  
    
    // ----- detect laps here
    // Left lane lap count
    if(raceMode){
      if(analogRead(LDR_L) > LDR_TRIGGER_LEVEL){
        if(lapCountL < totalLaps && lapCountL != -3){
          if((millis()-mLastLeftTrigger > LAP_COUNT_TIME_THRESHOLD) && !leftStillCrossingLine){
            lapCountL++;
            leftStillCrossingLine=1;
            
            // beep buzzer
            digitalWrite(BEEP_PIN, HIGH);
            delay(40);
            digitalWrite(BEEP_PIN, LOW);
    
            if(raceMode == 3){
              mLeftLapTime[1] = (millis()- mStartTime);
            }
            else{
              mLeftLapTime[lapCountL] = (millis()- mStartTime);
            }
            
            Serial.print(F("   LEFT  lane... LAP "));
            Serial.print(lapCountL);
            Serial.print(F(" TIME: "));
            if(raceMode == 3){
              Serial.print(float((mLeftLapTime[1] - mLeftLapTime[0])/1000.0));
              Serial.print(F("   TOTAL RUN TIME: "));
              Serial.println(float((millis()- mStartTime)/1000.0));
              mLeftLapTime[0] = mLeftLapTime[1];
            }
            else Serial.println(float((mLeftLapTime[lapCountL] - mLeftLapTime[lapCountL-1])/1000.0));
            Serial.println();
          }
          mLastLeftTrigger=millis();
        } 
      }
      // else it is no longer breaking the laser beam
      else leftStillCrossingLine=0;
        
      // Right lane lap count
      if(analogRead(LDR_R) > LDR_TRIGGER_LEVEL){
        if(lapCountR < totalLaps && lapCountR != -3){
          if((millis()-mLastRightTrigger > LAP_COUNT_TIME_THRESHOLD) && !rightStillCrossingLine){
            lapCountR++;
            rightStillCrossingLine=1;
            
            // beep buzzer
            digitalWrite(BEEP_PIN, HIGH);
            delay(40);
            digitalWrite(BEEP_PIN, LOW);
    
            if(raceMode == 3){
              mRightLapTime[1] = (millis()- mStartTime);
            }
            else{
              mRightLapTime[lapCountR] = (millis()- mStartTime);
            }
            
            Serial.print(F("   RIGHT lane... LAP "));
            Serial.print(lapCountR);
            Serial.print(F(" TIME: "));
            if(raceMode == 3){
              Serial.print(float((mRightLapTime[1] - mRightLapTime[0])/1000.0));
              Serial.print(F("   TOTAL RUN TIME: "));
              Serial.println(float((millis()- mStartTime)/1000.0));
              mRightLapTime[0] = mRightLapTime[1];
            }
            else Serial.println(float((mRightLapTime[lapCountR] - mRightLapTime[lapCountR-1])/1000.0));
            Serial.println();
          }
          mLastRightTrigger=millis();
        } 
      }
      // else it is no longer breaking the laser beam
      else rightStillCrossingLine=0;
      
    }// end of lap detection/counting
  
  
    // -------- 
    // detect win and stop both robots when they have completed the laps
    if(raceMode != 3){
      
      if(lapCountL == totalLaps){
        if(!winner){      
          // Left lane won !!
          mLeftRaceTime = millis()- mStartTime;  
          leftCheckFlagServo.write(FLAG_UP_ANGLE);
          irsend.sendNEC(LEFT_STOP, 32);
          delay(20);
          irsend.sendNEC(LEFT_STOP, 32);
          delay(20); 
          winner = 1;
          Serial.println(F("   LEFT  lane... WINNER!!!"));
          Serial.println();
        }
        else if(winner == 2){ // this lane lost but still will need to stop
          mLeftRaceTime = millis()- mStartTime;  
          irsend.sendNEC(LEFT_STOP, 32);
          delay(20);
          irsend.sendNEC(LEFT_STOP, 32);
          delay(20); 
          Serial.println(F("   LEFT  lane... finished FASHIONABLY LATE"));
          Serial.println();
        }
      }
      if(lapCountR == totalLaps){
        if(!winner){      
          // Right lane won !! 
          mRightRaceTime = millis()- mStartTime;  
          rightCheckFlagServo.write(FLAG_UP_ANGLE);
          irsend.sendNEC(RIGHT_STOP, 32);
          delay(20);
          irsend.sendNEC(RIGHT_STOP, 32);
          delay(20);
          winner = 2;
          Serial.println(F("   RIGHT lane... WINNER!!!"));
          Serial.println();
        }
        else if(winner == 1){ // this lane lost but still will need to stop
          mRightRaceTime = millis()- mStartTime; 
          irsend.sendNEC(RIGHT_STOP, 32);
          delay(20);
          irsend.sendNEC(RIGHT_STOP, 32);
          delay(20); 
          Serial.println(F("   RIGHT lane... finished FASHIONABLY LATE"));
          Serial.println();
        }
      } 
    } // end of win and stop
  
    // ----- check if the race is completed and initialize restart if it is
    if (winner && (lapCountL == -3 || lapCountL == totalLaps) && (lapCountR == -3 || lapCountR == totalLaps)){ 
  
      Serial.println(F("RACE RESULTS"));
      Serial.println(F("____________________________"));
      Serial.println();
  
      if(lapCountL != -3){
        Serial.print(F("   LEFT  lane"));
        if(winner == 1) Serial.println(F("     WINNER"));
        else Serial.println();
        
        for(int lap=1; lap <= totalLaps; lap++){
          Serial.print(F("     LAP "));
          Serial.print(lap);
          Serial.print(F("________"));
          Serial.print(float((mLeftLapTime[lap] - mLeftLapTime[lap-1])/1000.0));
          Serial.println(F("s"));
        }
    
          Serial.print(F("     Total Time___"));
          Serial.print(float(mLeftRaceTime/1000.0));
          Serial.println(F("s"));
          Serial.println();
      }
  
  
      if(lapCountR != -3){
        Serial.print(F("   RIGHT lane"));
        if(winner == 2) Serial.println(F("     WINNER"));
        else Serial.println();
    
        for(int lap=1; lap <= totalLaps; lap++){
          Serial.print(F("     LAP "));
          Serial.print(lap);
          Serial.print(F("________"));
          Serial.print(float((mRightLapTime[lap] - mRightLapTime[lap-1])/1000.0));
          Serial.println(F("s"));
        }
         
          Serial.print(F("     Total Time___"));
          Serial.print(float(mRightRaceTime/1000.0));
          Serial.println(F("s"));
          Serial.println();
      }
      Serial.println();
              
      resetRace();
      if(raceMode == 1)
        selectRaceMode(0);
    }
  }
  selectRaceMode(0);
}


void leftResetAtLine(void){
  
  // signal the robot to backup
  irsend.sendNEC(LEFT_BACKUP, 32); 
  delay(20);
  irsend.sendNEC(LEFT_BACKUP, 32);
  
  // keep going until the laser is clear
  while(analogRead(LDR_L) > LDR_TRIGGER_LEVEL){
    if(checkIfCancel()) return;
  }

  // signal the robot to bump forward
  irsend.sendNEC(LEFT_BUMP_IN, 32);
  delay(20);
  irsend.sendNEC(LEFT_BUMP_IN, 32);

  // the robot should continue to bump until the laser is again tripped
  while(analogRead(LDR_L) < LDR_TRIGGER_LEVEL){
    if(checkIfCancel()) return;
  }

  // signal the robot to stop
  irsend.sendNEC(LEFT_STOP, 32);
  delay(20);
  irsend.sendNEC(LEFT_STOP, 32);
  delay(20);
}


void rightResetAtLine(void){
  
  // signal the robot to backup
  irsend.sendNEC(RIGHT_BACKUP, 32); 
  delay(20);
  irsend.sendNEC(RIGHT_BACKUP, 32);
  
  // keep going until the laser is clear
  while(analogRead(LDR_R) > LDR_TRIGGER_LEVEL){
    if(checkIfCancel()) return;
  }

  // signal the robot to bump forward
  irsend.sendNEC(RIGHT_BUMP_IN, 32);
  delay(20);
  irsend.sendNEC(RIGHT_BUMP_IN, 32);

  // the robot should continue to bump until the laser is again tripped
  while(analogRead(LDR_R) < LDR_TRIGGER_LEVEL){
    if(checkIfCancel()) return;
  }

  // signal the robot to stop
  irsend.sendNEC(RIGHT_STOP, 32);
  delay(20);
  irsend.sendNEC(RIGHT_STOP, 32);
  delay(20);
}


void cancelRace(void){
  Serial.println();
  Serial.println();
  Serial.println(F("     ...canceling active races............"));
  Serial.println();
  Serial.println();
  Serial.println(F("////////////-------------------\\\\\\\\\\\\"));
  Serial.println(F("REMOVE ALL ROBOTS FROM THE START LINE"));
  Serial.println(F("\\\\\\\\\\\\-------------------////////////"));
  Serial.println();  
  raceMode=0; 
  resetRace();
}


void selectRaceMode(byte timeout){
  Serial.println(F("Please select and enter your race mode:"));
  Serial.println(F("  (1) Single race and stop"));
  Serial.println(F("  (2) Repeat until canceled"));
  Serial.println(F("  (3) Lap times and counting only, will not stop until canceled."));
  Serial.println();
  incomingByte = '*';

  long mToAutoStart = millis();

  int notDone = 1;

  while(notDone){
    while (Serial.available() == 0){ // wait for serial input or timeout and start if set to timeout mode
      if((millis() - mToAutoStart > 10000) && timeout){
        raceMode = 2;
        totalLaps = 2;
        return;    
      }
    }
    
    // read the incoming serial byte:
    incomingByte = Serial.read();
  
    switch (incomingByte)
    {
      case '1': case '2':
        notDone = 1;

        if(incomingByte == '1') raceMode=1;
        else raceMode=2;
              
        while(notDone){
          Serial.println(F("How many laps?"));
          Serial.println(F("   Enter between 1 and 9")); 
          //Serial.println(MAX_LAPS_IN_RACE);
          Serial.println();
          incomingByte = '*';
          
          while (Serial.available() == 0); // wait for serial input
          
          // read the incoming serial byte:
          incomingByte = Serial.read();
  
          switch (incomingByte)
          {
            case '1':
              totalLaps = 1;
              notDone = 0;
              break;
    
            case '2':
              totalLaps = 2;
              notDone = 0;
              break;
    
            case '3':
              totalLaps = 3;
              notDone = 0;
              break;
    
            case '4':
              totalLaps = 4;
              notDone = 0;
              break;
    
            case '5':
              totalLaps = 5;
              notDone = 0;
              break; 
    
            case '6':
              totalLaps = 6;
              notDone = 0;
              break; 
    
            case '7':
              totalLaps = 7;
              notDone = 0;
              break; 
    
            case '8':
              totalLaps = 8;
              notDone = 0;
              break; 
    
            case '9':
              totalLaps = 9;
              notDone = 0;
              break;                                   
            
            case 'c':
              cancelRace();
              return;
              break;
    
            default:
              Serial.println(F("INVALID entry.  Please try again."));
              Serial.println();
              break;                 
          }
          if(!notDone){
            if(raceMode == 1)
              Serial.println(F("Single race will begin and run for: "));
            else Serial.println(F("Repeating race will begin and run for: "));
            Serial.print(totalLaps); 
            Serial.println(F(" Lap(s)"));
            delay(500);
            Serial.println();
          }
        }
        break;
  
      case '3':
        totalLaps = 9999;
        raceMode = 3;
        notDone=0;
        Serial.print(F("Lap time and count mode will begin... "));
        delay(1000);
        Serial.println();
        break;
  
      case 'c':
        cancelRace();
        return;
        break;

      default:
        Serial.println(F("INVALID entry.  Please try again."));
        Serial.println();
        break; 
    }
  }  
}


void resetRace(void){
    // signal all robots to stop
    irsend.sendNEC(ALL_STOP, 32);
    delay(20);
    irsend.sendNEC(ALL_STOP, 32);
    delay(20);
    irsend.sendNEC(ALL_STOP, 32);
    delay(20);
    
    delay(END_OF_RACE_DELAY); // let the winner sit a moment
    resetOnLine=1;
    start=0; 
    lapCountL =-1;
    lapCountR =-1;
    winner = 0;
    mLeftRaceTime = 0; 
    mRightRaceTime = 0;  
    mLeftLapTime[0]=0;
    mRightLapTime[0]=0;
    
    
    // reset the flags
    leftCheckFlagServo.write(FLAG_DOWN_ANGLE);
    rightCheckFlagServo.write(FLAG_DOWN_ANGLE);
    greenFlagServo.write(FLAG_DOWN_ANGLE);
    delay(500);  
}


byte checkIfCancel(void){

  if(Serial.available() > 0){
    incomingByte = Serial.read();
    if(incomingByte == 'c'){
      cancelRace();
      return 1;
    }
  }

  return 0;
  
}

