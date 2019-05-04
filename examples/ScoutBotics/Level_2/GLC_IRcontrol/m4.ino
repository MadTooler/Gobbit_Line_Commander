// Mode 4, Trimming mode for mode 1 & 2

// See video example: https://www.youtube.com/watch?v=QZYdgpcy4Q0

/*  USING THE MODE
   
      Start by entering Mode 4 from the main control menu by pressing the number 4 key
        (If another mode is active, press the back button several times until no mode is active)

        NOTE: MAKE SURE YOU ARE IN A LARGE ENOUGH AREA THAT YOUR ROBOT CAN DRIVE LONG ENOUGH TO 
              ALLOW YOU TO MAKE THE ADJUSTMENTS.

      Trim the robot:

        1) Set the speed you would primarily like to trim:
          a) Press the enter button
          b) Use the number pad buttons for 0-100
          c) Press enter again

        2) Point the robot in an open direction and press the forward/up button

        3) While the robot is moving forward, press left or right as needed until the robot
           is tracking straight.

        4) Press the stop key to stop the robot if needed.

        5) Press the backward/down button, press left or right as needed until the robot
           is tracking straight.

        6) Repeat the steps 2-5 as needed until you find the best trim value.  You can also 
           change the speed again if needed.
        
      To exit the mode, press the back button until the main menu beep is heard.
      
      
      LIMITATIONS:  The trim settings are applied to all speeds.

                    The trim settings are reset when you power off your robot.

                    As your battery runs down, and your motors heat up during operation,
                    the trim values may be less accurate.  The only way to improve this
                    is with more sensor data, such as encoders or an IMU.
      
      NOTES:  See the Config.h tab to adjust the TRIM_MAX_TIME. 
               
              Signal beeps used in this mode:
                -mode entered
                -valid button press recognized
                -center veer position if adjusting/trimming back and forth
                -canceled mode and back at main mode select
*/

void modeFour(void) {
  while (mode) {
#ifdef ANALOG_DIVIDER_PIN
    MyBot.checkBattery(ANALOG_DIVIDER_PIN, CUTOFF, SMALL_RES_KOHMS, LARGE_RES_KOHMS);
#endif
    unsigned long currentMillis = millis();

    if (buttonDecode()) {

      switch (lastButtonValue) {

        case 1: // forward left
        case 18:
          MyBot.move( currentSpeed, steerTrimF - MODERATE_VEER_STRENGTH);
          //MyBot.setMotors( currentSpeed * MODERATE_VEER_STRENGTH / 100, currentSpeed);
          // reset time
          previousMillis = currentMillis;
          moving = 0;
          break;

        case 2:  // forward
        case 12:
          MyBot.move(currentSpeed , steerTrimF);
          //MyBot.setMotors(currentSpeed , currentSpeed);
          // reset time
          previousMillis = currentMillis;
          moving = 1;
          break;

        case 3:  // forward right
        case 17:
          MyBot.move( currentSpeed, steerTrimF + MODERATE_VEER_STRENGTH);
          //MyBot.setMotors(currentSpeed, (currentSpeed * MODERATE_VEER_STRENGTH / 100));
          // reset time
          previousMillis = currentMillis;
          moving = 0;
          break;

        case 4:  // spin/turn left
        case 15:
          if (moving == 1) {
            deBounce();
            steerTrimF--;
            MyBot.move(currentSpeed, steerTrimF);
          }
          else if (moving == -1) {
            deBounce();
            steerTrimB++;  // this is actually reverse of what the robot thinks, but easier to understand as an operator
            MyBot.move(-currentSpeed, steerTrimB);
          }
          else {
            MyBot.move( currentSpeed, -100);
            //MyBot.setMotors(-currentSpeed, currentSpeed);
            // reset time
            previousMillis = currentMillis;
          }
          break;

        case 5:  // stop or in this case do nothing
        case 16: 
        case 26:
          MyBot.move( 0, 0);
          // reset time
          previousMillis = currentMillis;
          moving = 0;
          break;

        case 6:  // spin/turn right
        case 14:
          if (moving == 1) {
            deBounce();
            steerTrimF++;
            MyBot.move(currentSpeed, steerTrimF);
          }
          else if (moving == -1) {
            deBounce();
            steerTrimB--; // this is actually reverse of what the robot thinks, but easier to understand as an operator
            MyBot.move(-currentSpeed, steerTrimB);
          }
          else {
            MyBot.move( currentSpeed, 100);
            //MyBot.setMotors(currentSpeed, -currentSpeed);
            // reset time
            previousMillis = currentMillis;
          }
          break;

        case 7:  // backward left
        case 20:
          MyBot.move( -currentSpeed, steerTrimB - MODERATE_VEER_STRENGTH);
          //MyBot.setMotors(-currentSpeed, (-currentSpeed * MODERATE_VEER_STRENGTH / 100));
          // reset time
          previousMillis = currentMillis;
          moving = 0;
          break;

        case 8:  // backward
        case 13:
          MyBot.move(-currentSpeed , steerTrimB);
          //MyBot.setMotors(-currentSpeed , -currentSpeed);
          // reset time
          previousMillis = currentMillis;
          moving = -1;
          break;

        case 9:  // backward right
        case 19:
          MyBot.move( -currentSpeed, steerTrimB + MODERATE_VEER_STRENGTH);
          //MyBot.setMotors( (-currentSpeed * MODERATE_VEER_STRENGTH / 100), -currentSpeed);
          // reset time
          previousMillis = currentMillis;
          moving = 0;
          break;

        case 10:  // enter button
          currentSpeed = getSpeed();
          // reset time
          //previousMillis = currentMillis;
          break;

        case 11:  // back button cancels input
          MyBot.move( 0, 0);
          mode = 0;
          currentSpeed = 100;
          moving = 0;
          deBounce(0);
          mainMenuBeep();
          break;
      }
    }

    else {
      if (moving == 0) { // stop of no signal received in none forward or backward
        if (currentMillis - previousMillis >= BUTTON_HOLD_DELAY) {
          MyBot.move( 0, 0);
          //MyBot.setMotors(0, 0);
        }
      }
      // stop in case it has gone out of range
      else if (currentMillis - previousMillis > TRIM_MAX_TIME ) {
        MyBot.move( 0, 0);
        //MyBot.setMotors(0, 0);
        moving = 0;
      }
    }
  }


}
