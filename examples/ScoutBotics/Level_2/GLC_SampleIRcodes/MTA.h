// MTA branded remote from the parts bin, for reference where an ALT code was needed

// IR commands... change these to match your buttons/keys
// ** If your remote button only outputs one code when it is pushed multiple times, 
//   set the XX_BUTTON value to that code with "0x" before it since it is a HEX value
//   and leave the XX_BUTTN_ALT value as "0x0" (zero).  
// If it toggles between two different codes, as you tap the button,
//   set XX_BUTTON value to one code and
//   set the XX_BUTTN_ALT value to the other code.

// Basic required 17 remote buttons
#define ZERO_BUTTON         0x11544827
#define ZERO_BUTTN_ALT      0xF1574245
#define ONE_BUTTON          0xA871F8E6
#define ONE_BUTTN_ALT       0x74D29C44
#define TWO_BUTTON          0X2781070B
#define TWO_BUTTN_ALT       0xC5D55143
#define THREE_BUTTON        0xA12C0CAF
#define THREE_BUTTN_ALT     0xC501BB0D
#define FOUR_BUTTON         0xB053820C
#define FOUR_BUTTN_ALT      0xABE11C8D
#define FIVE_BUTTON         0x7D37A47A
#define FIVE_BUTTN_ALT      0x4AFA16F2
#define SIX_BUTTON          0x9564D619
#define SIX_BUTTN_ALT       0xDF897749
#define SEVEN_BUTTON        0xD1D48F0F
#define SEVEN_BUTTN_ALT     0x5D302205
#define EIGHT_BUTTON        0xFD7941F0
#define EIGHT_BUTTN_ALT     0x52DB1BCB
#define NINE_BUTTON         0x19D49086
#define NINE_BUTTN_ALT      0xD3F2027C
#define ENTER_BUTTON        0x7E8AEB41    // button at the bottom right of the number pad
#define ENTER_BUTTN_ALT     0x5BACB2CB
#define BACK_BUTTON         0x93735FEE     // button at the bottom left of the number pad
#define BACK_BUTTN_ALT      0x18CC29C1

#define FORWARD_BUTTON      0x8EAEB9BE     // forward/up direction button/arrow
#define FORWARD_BUTTN_ALT   0x23A557EA     // alternate forward/up direction button/arrow
#define BACKWARD_BUTTON     0x501D512A     // backward/down direction button/arrow
#define BACKWARD_BUTTN_ALT  0x39C5E4A4     // alternate backward/down direction button/arrow
#define RIGHT_BUTTON        0xE666C92B     // right direction button/arrow
#define RIGHT_BUTTN_ALT     0x9BE817D8     // alternate right direction button/arrow
#define LEFT_BUTTON         0xCA0A713F     // left direction button/arrow
#define LEFT_BUTTN_ALT      0x99B63213     // alternate left direction button/arrow
#define STOP_BUTTON         0x8F6018A1     // stop button in the middle of the direction keys
#define STOP_BUTTN_ALT      0xF3B5079F     // alternate stop button in the middle of the direction keys

// Optionally set these buttons only if you have available buttons and want to expand
// beyond the number and arrow keys for clarity.
// ** These MUST be left with "0x0" values if they are not used. **
#define FORWARD_RIGHT       0x0
#define FORWARD_RT_ALT      0x0
#define FORWARD_LEFT        0x0
#define FORWARD_LFT_ALT     0x0
#define BACKWARD_RIGHT      0x0
#define BACKWARD_RT_ALT     0x0
#define BACKWARD_LEFT       0x0
#define BACKWARD_LFT_ALT    0x0
#define RC_MODE             0x0
#define RC_MOD_ALT          0x0
#define FREE_PROGRAM_MODE   0x0
#define FREE_PROG_MOD_ALT   0x0
#define LINE_PROGRAM_MODE   0x0
#define LINE_PROG_MOD_ALT   0x0
#define TRIM_MODE           0x0
#define TRIM_MOD_ALT        0x0
#define PLAY_PROGRAM        0x0
#define PLAY_PROG_ALT       0x0
#define STOP_PROGRAM        0x0
#define STOP_PROG_ALT       0x0

// Gripper buttons
#define    GRIP_OPEN     0xB7314D70
#define    GRIP_OPN_ALT  0x64F85AA8
#define    GRIP_CLOSE    0xEC74B5A2
#define    GRIP_CLOS_ALT 0x744B96E7

#define REPEAT_CODE         0xFFFFFFFF  // this probably does not need any change

