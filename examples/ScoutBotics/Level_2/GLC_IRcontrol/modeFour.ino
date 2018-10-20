// **** full instructions needed

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

