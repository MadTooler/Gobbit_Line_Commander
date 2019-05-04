// decode the received IR code to the corresponding key code if valid
int buttonDecode(void) {
  /*
    Key codes
    0 ZERO_BUTTON
    1 ONE_BUTTON
    2 TWO_BUTTON
    3 THREE_BUTTON
    4 FOUR_BUTTON
    5 FIVE_BUTTON
    6 SIX_BUTTON
    7 SEVEN_BUTTON
    8 EIGHT_BUTTON
    9 NINE_BUTTON
    10 ENTER_BUTTON
    11 BACK_BUTTON

    12 FORWARD_BUTTON
    13 BACKWARD_BUTTON
    14 RIGHT_BUTTON
    15 LEFT_BUTTON
    16 STOP_BUTTON

    17 FORWARD_RIGHT
    18 FORWARD_LEFT
    19 BACKWARD_RIGHT
    20 BACKWARD_LEFT
    21 RC_MODE
    22 FREE_PROGRAM_MODE
    23 LINE_PROGRAM_MODE
    24 TRIM_MODE
    25 PLAY_PROGRAM
    26 STOP_PROGRAM

    27 GRIP_OPEN
    28 GRIP_CLOSE
  */

  if (irrecv.decode(&results)) {
    switch (results.value) {

      case ZERO_BUTTON:
#if ZERO_BUTTN_ALT
      case ZERO_BUTTN_ALT:
#endif      
        lastButtonValue = 0;
        break;

      case ONE_BUTTON:
#if ONE_BUTTN_ALT
      case ONE_BUTTN_ALT:
#endif      
        lastButtonValue = 1;
        break;

      case TWO_BUTTON:
#if TWO_BUTTN_ALT
      case TWO_BUTTN_ALT:
#endif      
        lastButtonValue = 2;
        break;

      case THREE_BUTTON:
#if THREE_BUTTN_ALT
      case THREE_BUTTN_ALT:
#endif      
        lastButtonValue = 3;
        break;

      case FOUR_BUTTON:
#if FOUR_BUTTN_ALT
      case FOUR_BUTTN_ALT:
#endif     
        lastButtonValue = 4;
        break;

      case FIVE_BUTTON:
#if FIVE_BUTTN_ALT
      case FIVE_BUTTN_ALT:
#endif      
        lastButtonValue = 5;
        break;

      case SIX_BUTTON:
#if SIX_BUTTN_ALT
      case SIX_BUTTN_ALT:
#endif      
        lastButtonValue = 6;
        break;

      case SEVEN_BUTTON :
#if SEVEN_BUTTN_ALT
      case SEVEN_BUTTN_ALT:
#endif      
        lastButtonValue = 7;
        break;

      case EIGHT_BUTTON :
#if EIGHT_BUTTN_ALT
      case EIGHT_BUTTN_ALT:
#endif      
        lastButtonValue = 8;
        break;

      case NINE_BUTTON :
#if NINE_BUTTN_ALT
      case NINE_BUTTN_ALT:
#endif      
        lastButtonValue = 9;
        break;

      case ENTER_BUTTON :
#if ENTER_BUTTN_ALT
      case ENTER_BUTTN_ALT:
#endif      
        lastButtonValue = 10;
        break;

      case BACK_BUTTON :
#if BACK_BUTTN_ALT
      case BACK_BUTTN_ALT: 
#endif     
        lastButtonValue = 11;
        break;

      case FORWARD_BUTTON:
#if FORWARD_BUTTN_ALT
      case FORWARD_BUTTN_ALT:
#endif      
        lastButtonValue = 12;
        break;

      case BACKWARD_BUTTON :
#if BACKWARD_BUTTN_ALT
      case BACKWARD_BUTTN_ALT:      
#endif      
        lastButtonValue = 13;
        break;

      case RIGHT_BUTTON :
#if RIGHT_BUTTN_ALT
      case RIGHT_BUTTN_ALT:      
#endif
        lastButtonValue = 14;
        break;

      case LEFT_BUTTON:
#if LEFT_BUTTN_ALT
      case LEFT_BUTTN_ALT:      
#endif
        lastButtonValue = 15;
        break;

      case STOP_BUTTON:
#if STOP_BUTTN_ALT
      case STOP_BUTTN_ALT:      
#endif
        lastButtonValue = 16;
        break;

      case REPEAT_CODE :
        // do nothing and last code will remain as it was
        break;

#if FORWARD_RIGHT
      case FORWARD_RIGHT:
        lastButtonValue = 17;
        break;
#endif

#if FORWARD_RT_ALT
      case FORWARD_RT_ALT:
        lastButtonValue = 17;
        break;
#endif


#if FORWARD_LEFT
      case FORWARD_LEFT:
        lastButtonValue = 18;
        break;
#endif

#if FORWARD_LFT_ALT
      case FORWARD_LEFT_ALT:
        lastButtonValue = 18;
        break;
#endif

#if BACKWARD_RIGHT
      case BACKWARD_RIGHT:
        lastButtonValue = 19;
        break;
#endif

#if BACKWARD_RT_ALT
      case BACKWARD_RT_ALT:
        lastButtonValue = 19;
        break;
#endif

#if BACKWARD_LEFT
      case BACKWARD_LEFT:
        lastButtonValue = 20;
        break;
#endif

#if BACKWARD_LFT_ALT
      case BACKWARD_LFT_ALT:
        lastButtonValue = 20;
        break;
#endif

#if RC_MODE
      case RC_MODE:
        lastButtonValue = 21;
        break;
#endif

#if RC_MOD_ALT
      case RC_MOD_ALT:
        lastButtonValue = 21;
        break;
#endif

#if FREE_PROGRAM_MODE
      case FREE_PROGRAM_MODE:
        lastButtonValue = 22;
        break;
#endif

#if FREE_PROG_MOD_ALT
      case FREE_PROG_MOD_ALT:
        lastButtonValue = 22;
        break;
#endif

#if LINE_PROGRAM_MODE
      case LINE_PROGRAM_MODE:
        lastButtonValue = 23;
        break;
#endif

#if LINE_PROG_MOD_ALT
      case LINE_PROG_MOD_ALT:
        lastButtonValue = 23;
        break;
#endif

#if TRIM_MODE
      case TRIM_MODE:
        lastButtonValue = 24;
        break;
#endif

#if TRIM_MOD_ALT
      case TRIM_MOD_ALT:
        lastButtonValue = 24;
        break;
#endif

#if PLAY_PROGRAM
      case PLAY_PROGRAM:
        lastButtonValue = 25;
        break;
#endif

#if PLAY_PROG_ALT
      case PLAY_PROG_ALT:
        lastButtonValue = 25;
        break;
#endif


#if STOP_PROGRAM
      case STOP_PROGRAM:
        lastButtonValue = 26;
        break;
#endif

#if STOP_PROG_ALT
      case STOP_PROG_ALT:
        lastButtonValue = 26;
        break;
#endif

#if GRIP_OPEN
      case GRIP_OPEN:
        lastButtonValue = 27;
        break;
#endif

#if GRIP_OPN_ALT
      case GRIP_OPN_ALT:
        lastButtonValue = 27;
        break;
#endif

#if GRIP_CLOSE
      case GRIP_CLOSE:
        lastButtonValue = 28;
        break;
#endif

#if GRIP_CLOS_ALT
      case GRIP_CLOS_ALT:
        lastButtonValue = 28;
        break;
#endif

      default :
        lastButtonValue = -1;
        break;
    }

    irrecv.resume(); // Receive the next value
    return 1;
  }

  return 0;

}
