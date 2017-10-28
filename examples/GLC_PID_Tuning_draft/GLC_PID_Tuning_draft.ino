// PID line following tuning using the Gobbit Line Command library
// and Gobbit robot.
//
// 10/28/2017 rough draft, missing aggressive/coarse tuning instructions.

// Choose your Motor Driver...
// To load default settings for either an Ardumoto version 14 or 20, or an Adafruit v2.3,
// uncomment only the following motor driver define that matches.
// If none are uncommented, Ardumoto v14 values will be used.
//
// DO NOT UNCOMMENT MORE THAN ONE
//#define ARDUMOTO_14
//#define ARDUMOTO_20  
//#define ADAFRUIT_MS

/*
 *  Line follow PD tuning directions for the Gobbit robot chassis.
 *  
 *  You should use a line course that has a long straight, large gradual curve,
 *  a short straight, and a tighter curve.  Overall course size could fit on a standard
 *  poster board (about 22" x 28") with lines kept 5" from the edge of the poster board.
 *  See: http://www.instructables.com/id/Line-following-Robot-with-Arduino/?ALLSTEPS
 *
 *  For assembly:  http://primalengineering.com/robots/Downloads/GBT-101-INSTR-R2_LineFollowAssemblyInstructions.PDF
 *
 *  For wiring:  http://primalengineering.com/robots/Downloads/Gobbit_LineFollowWiring_03222017.pdf
 *
 *  For additional programming, hardware, and other projects:  http://www.primalengineering.com/robots 
 *
 *  Parts list:
 *
 *   1) Gobbit Robot with line following configuration
 *       Components:
 *       Gobbit Chassis with motors, switch, and battery holder
 *       Sparkfun Redboard (arduino uno clone)
 *       Motor shield
 *         Sparkfun Ardumoto Shield
 *         or
 *         Adafruit v 2.3 motor shield
 *       Jumper wires
 *       Breadboard optional
 *       Pololu QTR-8RC RC Reflectance Sensor Array
 *
 *       see: Zagros Robot Starter Kit - Gobbit
 *            http://www.zagrosrobotics.com/
 *
 *   2) Black electrical tape line course on white background
 *
 *   4) Batteries
 *
 *  The directions here are guidelines that should allow you to improve
 *  the performance of your robot.  
 *  
 *  The GLC library uses PID (Proportional, Integral, and Derivative) calculations to help
 *  correct how well the robot is following a black line on a white course, 
 *  based upon the line position (the "setpoint") it is given by the QTR infrared sensor.
 *  
 *  For in depth explanation of how P, PI, PD, and PID control loops work, look
 *  up "PID controller" online.  Wikipedia has a lot of good information on PID:
 *  http://en.wikipedia.org/wiki/PID_controller
 *  
 *  While the averaged default settings of this library may work well enough on most
 *  Gobbit robots, every robot is unique and may need testing, and what is referred to 
 *  as "tuning," to determine the most accurate values.  Making only one change at a time, 
 *  taking a controlled approach at tuning, will better help you reach positive results 
 *  quicker and with a better understanding of how your robot reacts to adjustments.  
 *  This gained understanding can be very valuable in troubleshooting what may be wrong
 *  when you are working on more advanced projects with your robot.
 *  
 *  NOTE: The Gobbits can run on 6-12 volts.  The battery voltage makes very big difference
 *        in the correct PD values.  The GLC library defaults all values to 9volts, if not declared,  
 *        but using "MyBot.setBatteryVolts(YourBatteryVoltageHere);" in the setup, before 
 *        "MyBot.beginGobbit():,"will load other PD values that should work better for other 
 *        voltages.  However, this tune process may still be needed for best results.
 *
 *  The approach that is usually most successful is reducing all of the PID 
 *  variables to 0 or 1 where they will have limited or no effect on the correcting 
 *  calculations.  From there, the P will be refined, then the D will be reintroduced
 *  and refined, next a little I, then the coarse (more aggressive) factors for large 
 *  following errors can be turned back on and refined as well.
 *  
 *  The size of the values of P, I, and D in different control systems, whether 1 or 1,000,
 *  is all relative to the math and values being processed from sensors and drives.
 *  A perfectly tuned set of values with other robots may not have any direct
 *  application with any other systems.  For example, with this system, good P values are 
 *  probably less than 1, but you will have to try your robot and decide for yourself.
 *
 *  The initial PID settings of this sketch are reduced to function only with P, 
 *  (Proportional only).  You should start with only adjusting that value
 *  while the Integral and Derivative are set to 0.  The "setPIDfineRange()," which is used
 *  as a threshold where the finer PID and PIDcoarse transition, can also be tuned, but the noted 
 *  value of 0.05 is probably pretty close as is.
 *
 *  Tuning Steps: 
 *     
 *   Tune P value:
 *        The Proportional variable is multiplied against the measured error from the setpoint 
 *        (the center of the sensor) to the current position.
 *        
 *    1) Upload sketch to Arduino/Redboard.
 *    2) Place the robot on the line of your lined course and turn it on.
 *        It should sweep back and forth to calibrate, then begin running the course.
 *    3) Observe the robot and note how well it corrects direction...
 *        a) When it is near the line
 *        b) When it is far off center to either side
 *        c) While following a longer straight line
 *        d) While following along curves
 *        e) When it is transitioning from curves to straight and then to curves again
 *     4) Adjust the P variable from 0.5 to 1   
 *     5) Upload the sketch to the robot and test it on the line again.
 *     6) Observe the performance again and note if it is improved.
 *     7) Change the P variable again to 0.001, upload, and observe again.
 *     8) Now you will really start tuning P
 *        a) You want to start tuning P from a value where it is not adjusting fast
 *          enough, like at the 0.001 value, then keep increasing the value
 *          until it starts to adjust too much and quickly over corrects, 
 *          which results in oscillation.
 *        b) After it has reached the point of a lot of oscillation, reduce the P
 *           value around 20% and test if it is good.  If not, adjust it again in the
 *           direction you believe is best until you have found the best value.
 *           NOTE: While you are only using a P value with the others disables, 
 *                 you will NOT be able to find a perfect P value that makes your robot 
 *                 follow perfectly.  You are simply finding the value that
 *                 performs best. It will have moderate oscillation.
 *                 
 *   Tune D value:
 *        The Derivative variable is multiplied against the difference of the
 *        current error and the previous error.
 *      
 *     1) You should have already found a P value that works well enough.  Now
 *        let's try re-introducing the D.  
 *        a) Set the D value to 0.1
 *        b) Upload, run on the course, and observe.
 *     2) Keep increasing the D value like you did for the P until you find 
 *        the smoothest following.
 *           NOTE:  The Derivative works kind of like applying the brakes while  
 *                  also pushing on the gas with the Proportional.  If you over
 *                  drive the D, your robot may actually sound and move like it is 
 *                  struggling with the brakes on.  If you notice that kind of 
 *                  result, you have added too much D.
 *                  
 *    Tune I value:             
 *        Where the P and D are based upon only the current and previous error, the
 *        Integral value is multiplied against the accumulation of error.  Therefore,
 *        this has the potential to become a very large value very fast.  It can 
 *        also be very destabilizing.  While too small will have no noticeable impact,
 *        a little increase can easily be too big with major oscillation.
 *        
 *        The robot can run pretty good without Integral, especially if it is not running 
 *        at slow or near stall speed, but let's see if you can make it work better with it.
 *        
 *     1) Start with a very small value.  
 *        a) Set the I value to 0.0001
 *        b) Upload, run on the course, and observe.
 *     2) Keep increasing the I value like you did for the P and D until you find 
 *        the smoothest following.  At or above .001 is probably going to start  
 *        running unstable.
 *              
 *    ***** a section on the setPIDfineRange  and the setPIDcoarse needs to be added here *****
 *
 *      
 *      Sometimes, tuning D leads to then needing a little fine re-tune to P as well.  
 *      Same for returning for subtle changes to P and D when applying the
 *      multiplier/portion.  Remember, the multiplier is exaggerating the P and D.
 *      
 *      Hopefully, you have found it both educational and fun to see how smooth you
 *      can get your robot following the line.  While the process can be a challenge, 
 *      and a bit of an acquired art, it can also be very rewarding when you find the
 *      right combination.
 */


#include <GobbitLineCommand.h>

// Create an object of the GLC library and give it a name.
// I called it MyBot here, but you can name yours whatever you want.
GobbitLineCommand MyBot;

void setup() {

  // You probably want to leave the speeds alone while tuning, unless your robot
  //   is very fast and hard to observe.
  // If you notice your robot moving in reverse while your P variable is very
  //   aggressive, you could make the Min speed 0 instead of -100 to see the effect.
  // This will set the Minimum and Maximum speeds used by the followLine() function.
  // Min and Max speeds -100 to 100.
  //   0 for both will lead to no movement.
  //   values > 0 for Minimum will make turns while following less sharp.
  //   values 0 or less for Maximum may result in unpredictable actions
  // Uncomment the next line and adjust the speed values if you want to use it.
  // MyBot.setMinMaxSpeeds(-100, 100);

  // Sets the fine Proportional, Integral, and Derivative values.
  //   Proportional probably between 0.01 and 0.1
  //   Integral very small, probably between 0.0001 and 0.001
  //   Derivitive probably between 0.1 and 3.5
  MyBot.setPID(0.5, 0, 0);
 

  // Sets the range of the sensor from then middle to edge that the fine/small PID 
  // values should be used. PDfineRange/fraction/portion value from 0 to 1.0.
  //   0 will force the PIDcoarse values active always
  //   1 will force the finer PID values active always
  //   0.1 may be a good place to start.
  // We will start at 1 to disable the coarse values until the setPID values are good. 
  MyBot.setPIDfineRange(1);

  // Sets the coarse/fast/Aggressive Proportional, Integral, and Derivative values.
  //   Proportional probably between 0.1 and 0.5
  //   Integral still very small, probably less than 0.005
  //   Derivitive probably between 1.5 and 5
  //MyBot.setPIDcoarse(0.2, 0.002, 3);

  // Initializes and starts the GLC settings
  MyBot.beginGobbit();

  // This calibrates the sensor to the course by sweeping back and forth over the line.
  // Ideally, the sensor should just pass the line to the left and right.
  // That is about the same as turning and stopping to go back the other 
  //   direction when the front corner of the robot is directly over the line. 
  // To adjust the speed of the calibration, adjust the value between 1-100.
  //   0 loads default values
  //   1 may be too low for any movement.
  //   Somewhere between 25 and 65 is probably best.
  MyBot.calibrateLineSensor(0);

}

void loop() {

  // Follow the line with mode 0 (0 = follow forever no matter what it finds)
  // If the robot tries to run completely off the line, it should spin quickly back towards the line
  //   and begin to try an follow again.  The spinning is not using the PID values, just turning
  //   quickly until the sensor sees the line again. 
  MyBot.followLine(0);
  
}
