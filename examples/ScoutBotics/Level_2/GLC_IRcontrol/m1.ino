// Mode 1, Simple RC control mode

// See video example: https://www.youtube.com/watch?time_continue=1&v=ifJDFvL9Avc

/*  USING THE MODE
 
      Start by entering Mode 1 from the main control menu by pressing the number 1 key
        (If another mode is active, press the back button several times until no mode is active)

      Drive the robot:
        Hold down the remote keys to move.  Use the number keys 1,2,3,4,6,7,8 & 9 or the directional
        arrows and the optional FORWARD_RIGHT,FORWARD_LEFT,BACKWARD_RIGHT,BACKWARD_RIGHT if set.

      To change the speed:
        1) Press the enter key
        2) Use the number pad buttons for 0-100
        3) Press enter again

        NOTE: The initial speed is 100 (full speed).

      To exit the mode, press the back button until the main menu beep is heard.
      

      NOTES:      
            Make sure you keep the remote pointed at the robot.

            See the Config.h tab to adjust the speeds of the Turning and Veering 
            (keys 1,3,7,9 or the FORWARD_RIGHT,FORWARD_LEFT,BACKWARD_RIGHT,BACKWARD_RIGHT)
  
            Signal beeps used in mode:
              -mode entered
              -valid button press recognized
              -cancel recording step/command before enter pressed
              -center veer position if adjusting/trimming back and forth
      				-step recorded after enter pressed
              -end of recorded program
      				-canceled mode and back at main mode select
*/
 

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
		  
		/* breaking does not work well here since it always assumes forward motion
		case 16:  // stop button will brake the motors
          MyBot.brakeMotors();
          // reset time
          previousMillis = currentMillis;
          break;  
		*/

#ifdef GRIPPER_PIN
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
#endif

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
