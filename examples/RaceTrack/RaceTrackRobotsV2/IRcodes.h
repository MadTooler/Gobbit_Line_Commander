// IR command values are unique to this setup.  
//   While these are currently sent and received in the  NEC format of IR is used for now, 
//   it may be best to switch to a shorter/smaller bit value code for faster communication.

//#ifdef LEFT_LANE
#if (LANE_CHOICE == 'L')
  // Left lane IR commands
  #define STOP 0x2BB27000
  #define BACKUP 0x2BB27002
  #define BUMP_IN 0x2BB27003
#else   
  // Right lane IR commands
  #define STOP 0x2BB28000
  #define BACKUP 0x2BB28002
  #define BUMP_IN 0x2BB28003
#endif

// Common IR commands
#define GO_RACE 0x2BB26001
#define ALL_STOP 0x2BB29999
