// **** full instructions needed

void modeOne(void) {
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
          break;

        case 2:  // forward
        case 12:
          MyBot.move(currentSpeed , steerTrimF);
          //MyBot.setMotors(currentSpeed , currentSpeed);
          // reset time
          previousMillis = currentMillis;
          break;

        case 3:  // forward right
        case 17:
          MyBot.move( currentSpeed, steerTrimF + MODERATE_VEER_STRENGTH);
          //MyBot.setMotors(currentSpeed, (currentSpeed * MODERATE_VEER_STRENGTH / 100));
          // reset time
          previousMillis = currentMillis;
          break;

        case 4:  // spin/turn left
        case 15:
          MyBot.move( RC_SPINSPEED, -100);
          //MyBot.setMotors(-currentSpeed, currentSpeed);
          // reset time
          previousMillis = currentMillis;
          break;

        case 5:  // stop or in this case do nothing
        case 26:
          // MyBot.brakeMotors(); // this does not really do any good at this point due to operator trigger speed
          MyBot.move( 0, 0); // not actually needed for current code
          // reset time
          previousMillis = currentMillis;
          break;

        case 6:  // spin/turn right
        case 14:
          MyBot.move( RC_SPINSPEED, 100);
          //MyBot.setMotors(currentSpeed, -currentSpeed);
          // reset time
          previousMillis = currentMillis;
          break;

        case 7:  // backward left
        case 20:
          MyBot.move( -currentSpeed, steerTrimB - MODERATE_VEER_STRENGTH);
          //MyBot.setMotors(-currentSpeed, (-currentSpeed * MODERATE_VEER_STRENGTH / 100));
          // reset time
          previousMillis = currentMillis;
          break;

        case 8:  // backward
        case 13:
          MyBot.move(-currentSpeed , steerTrimB);
          //MyBot.setMotors(-currentSpeed , -currentSpeed);
          // reset time
          previousMillis = currentMillis;
          break;

        case 9:  // backward right
        case 19:
          MyBot.move( -currentSpeed, steerTrimB + MODERATE_VEER_STRENGTH);
          //MyBot.setMotors( (-currentSpeed * MODERATE_VEER_STRENGTH / 100), -currentSpeed);
          // reset time
          previousMillis = currentMillis;
          break;
       
        case 27:  // GRIP OPEN       
          MyBot.gripOpen();
          // reset time
          previousMillis = currentMillis;
          break;

        case 28:  // GRIP CLOSE       
          MyBot.gripClose();
          // reset time
          previousMillis = currentMillis;
          break;

        case 10:  // enter button
          currentSpeed = getSpeed();
          // reset time
          //previousMillis = currentMillis;
          break;

        case 11:  // back button cancels input
          mode = 0;
          currentSpeed = 100;
          deBounce(0);
          mainMenuBeep();
          break;
      }
    }

    else {
      if (currentMillis - previousMillis >= BUTTON_HOLD_DELAY) {
        MyBot.move( 0, 0);
        //MyBot.setMotors(0, 0);
      }
    }

  }
}
