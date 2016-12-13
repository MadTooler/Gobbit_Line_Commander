/*
 *  GobbitLineCommand.h - Library for line following, intersection detection, and basic motor control of Gobbit robot.
 *  Created by Jason Talley 
 *	Last edit 12/13/2016
 *  Released under GNU agreement
 */

/*
 *  PD Line Following for the Gobbit and Zagros Robotics Starter Kit with Pololu Line Sensor
 *
 *  For assembly, wiring, programming, and other examples, see: http://www.primalengineering.com/robots 
 *
 *
 *  Parts list:
 *
 *    1) Zagros Robot Starter Kit - Gobbit
 *       (http://www.zagrosrobotics.com/)
 *       Included components needed:
 *          Gobbit Chassis with motors and battery holder
 *          Sparkfun Redboard
 *          Sparkfun Ardumoto Shield
 *          Jumper wires
 *          Breadboard optional
 *
 *    2) Pololu QTR-8RC RC Reflectance Sensor Array
 *       (http://www.zagrosrobotics.com/shop/item.aspx?itemid=896)
 *
 *    3) Black electrical tape Line course on white background
 *
 *	  4) Batteries
 */
 
 
/*
 * todo
 * 
 *  
	current items todo

		test move, drive, follow now that setmotors is implemented
			both ardumoto and adafruit
		
		retest voltage functions and simple volt meter sample
		
		Test pd and all default values for various voltages
			Does read battery engage the various defaults properly?
				the serialPrintCurrentSettings should verify
		
		Correct waste with variable types
				
		clean up example and show a better name for the instance such as MyRobot or Gobbit
		
		Example with list of all functions, basic and advanced, with description of use.
		
		Fix indention
		
		format for arduino library manager and submit to github and arduino
			current manager does not have capability to load QTRsensors and other libraries if declared as dependants
	
	
	Later Todo's
	
		turn YR,YL (at y right/left) in drive/turn function
	
		****Turn in drive mode maybe switch over to a turn only PID function instead of dual speed looking for turn.  This would also eliminate the need to set those speeds and offcentersetpoint.
	
		advanced tune function for left/right motor values for move command calibrations 
			Probably not, but if so, simple variable that would then have to be applied over the entire move command outputs.
		
		serialPrintCurrentSettings does not show calSpeed unless after calibrateLineSensor(), but it is problematic to get that reading undless robot is actually calibrating on a board.
			Possible improvement on serialPrintCurrentSettings to show calSpeed, but not if have to declare it outside of calibrateLineSensor()
			
		Add a destructor for memory free up at exit... ??
		
		Possible improvement for Drive mode with twirking at intersection crossing if move the intersection detection above the line following PD in the followLine function.
			Tried this with out any noticable improvement.  Smart intersection detection without delay may fix this better.
	
		fix if def statements for only loading Adafruit Motor Shield V2 Library when necessary if there is space or conflict issues.
			possibly use similar to debug procedure to comment out afms stuff.  pretty involved and probably not necessary if memory and conflicts do not become issue.
			only real benefit then would be not needing the additional library.
	
		voltage monitor and adjust PID per?
			this would be for constant monitor and adjust.
			either default range or allow a map entry or series of ranges???
			possibly if the voltage monitor has been enabled.
			if PD values are declared by user, they should supercede any default voltage monitoring adjustments, probably
			*** probably leave this as a project for users to create since all the functionality is really included with access to the variables already.
	
		add timeout for calibrate if not line is found at end of process??
			maybe not since it is somewhat more helpful to determine what is wrong if it does not stop.
	
 * 
 */


#include <Arduino.h>

#include <Servo.h>

//QTRSensors library must be loaded to your arduino libraries folder.
// Easiest to use the library manager to install
// later releases may be able to auto load if this library is made accessible in arduino library manager.
#include <QTRSensors.h>  // Pololu QTR Library 

#include "GobbitLineCommand.h"

//**** this does not seem to work here, only in the .h file
//#include <Adafruit_MotorShield.h> if it was defined in the main program
//#if defined(ADAFRUIT_MS)
//	#include <Wire.h>
//	#include <Adafruit_MotorShield.h>
//	#include "utility/Adafruit_MS_PWMServoDriver.h"
//#endif 

//**** cannot seem to get either USE_AFMS or status of adafruit_ms to work within .cpp until later within the functions.
//#if USE_AFMS
//#if (USE_AFMS == 1)
//#if defined(ADAFRUIT_MS)
	//#include <Wire.h>
	#include <Adafruit_MotorShield.h>
	//#include "utility/Adafruit_MS_PWMServoDriver.h"
		
	// #define ADAFRUIT_MS in main program if the Adafruit motor shield v2.3 is to be used.
		  // M1 and M2 will be used.  Right motor on M1, Left on M2.
		  // No define will default to Sparkfun Ardumoto or other makes of Dir 
		  // and PWM pin driven style drivers similar to the Ardumoto and those
		  // based upon the L298 driver.  The default pin settings will be used 
		  // except where new values have been declared.
		
	// Create the motor shield object with the default I2C address
	Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
	// Or, create it with a different I2C address (say for stacking)
	// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

	// Select which 'port' M1, M2, M3 or M4. In this case, M1
	Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
	// You can also make another motor on port M2
	Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
//#endif 


// line sensor object 
QTRSensorsRC qtrrc;


// object for the gripper servo
Servo gripper;


// initializes the sensor and motors.  Run in setup loop.  Run all "set" functions and declare variables before running this function.
void GobbitLineCommand::beginGobbit(void)
{
  
  // initialize sensor
  qtrrc.init(sensorPins, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
  delay(500);  // give sensors time to set
  
  if(volts == NOT_SET)
  {
	  volts = 9;
  }
  
  if(volts < 8.3) // use 7.2 and 7.4 settings
  //if(volts > 5 && volts < 7.8) // use 7.2 and 7.4 settings
  {
	 if(_kp == NOT_SET) _kp = SEVENV_KP;
	 if(_kd == NOT_SET) _kd = SEVENV_KD;
	 if(multiPD == NOT_SET) multiPD = SEVENV_MULTIPLIER_PD;
	 if(smallRangePD == NOT_SET) smallRangePD = SEVENV_SMALL_RANGE_PD;
	 if(_calSpeed == NOT_SET) _calSpeed = SEVENV_CAL_SPD;
	 if(minSpeed == NOT_SET) minSpeed = SEVENV_MIN_SPD;
	 if(maxSpeed == NOT_SET) maxSpeed = SEVENV_MAX_SPD;
	 if(turnSpeedHigh == NOT_SET) turnSpeedHigh = SEVENV_TURN_SPD_HIGH;
	 if(turnSpeedLow == NOT_SET) turnSpeedLow = SEVENV_TURN_SPD_LOW;
	 if(offCenterSetPoint == NOT_SET) offCenterSetPoint = SEVENV_TURN_LOW_SET_POINT;
	 //if(drivePastDelay == NOT_SET) drivePastDelay = SEVENV_DRIVE_PAST_DELAY;  
  }
  
  else if(volts > 8.3 && volts < 9.9) // use 9v settings
  {
	 if(_kp == NOT_SET) _kp = NINEV_KP;
	 if(_kd == NOT_SET) _kd = NINEV_KD;
	 if(multiPD == NOT_SET) multiPD = NINEV_MULTIPLIER_PD;
	 if(smallRangePD == NOT_SET) smallRangePD = NINEV_SMALL_RANGE_PD;
	 if(_calSpeed == NOT_SET) _calSpeed = NINEV_CAL_SPD;
	 if(minSpeed == NOT_SET) minSpeed = NINEV_MIN_SPD;
	 if(maxSpeed == NOT_SET) maxSpeed = NINEV_MAX_SPD;
	 if(turnSpeedHigh == NOT_SET) turnSpeedHigh = NINEV_TURN_SPD_HIGH;
	 if(turnSpeedLow == NOT_SET) turnSpeedLow = NINEV_TURN_SPD_LOW;
	 if(offCenterSetPoint == NOT_SET) offCenterSetPoint = NINEV_TURN_LOW_SET_POINT;
	 //if(drivePastDelay == NOT_SET) drivePastDelay = NINEV_DRIVE_PAST_DELAY; 
  }
  
  //else if(volts > 9.9 && volts < 11.9) // use 11.1v settings  // lithiums charge over 12
  else if(volts > 9.9)// && volts < 11.9) // use 11.1v settings
  {
	 if(_kp == NOT_SET) _kp = ELEVENV_KP;
	 if(_kd == NOT_SET) _kd = ELEVENV_KD;
	 if(multiPD == NOT_SET) multiPD = ELEVENV_MULTIPLIER_PD;
	 if(smallRangePD == NOT_SET) smallRangePD = ELEVENV_SMALL_RANGE_PD;
	 if(_calSpeed == NOT_SET) _calSpeed = ELEVENV_CAL_SPD;
	 if(minSpeed == NOT_SET) minSpeed = ELEVENV_MIN_SPD;
	 if(maxSpeed == NOT_SET) maxSpeed = ELEVENV_MAX_SPD;
	 if(turnSpeedHigh == NOT_SET) turnSpeedHigh = ELEVENV_TURN_SPD_HIGH;
	 if(turnSpeedLow == NOT_SET) turnSpeedLow = ELEVENV_TURN_SPD_LOW;
	 if(offCenterSetPoint == NOT_SET) offCenterSetPoint = ELEVENV_TURN_LOW_SET_POINT;
	 //if(drivePastDelay == NOT_SET) drivePastDelay = ELEVENV_DRIVE_PAST_DELAY;  
  }
 
	// maybe not need this, or make for custom values
/*   else if(volts > 11.9) // use 12v settings
  {
	 if(_kp == NOT_SET) _kp = TWELVEV_KP;
	 if(_kd == NOT_SET) _kd = TWELVEV_KD;
	 if(multiPD == NOT_SET) multiPD = TWELVEV_MULTIPLIER_PD;
	 if(smallRangePD == NOT_SET) smallRangePD = TWELVEV_SMALL_RANGE_PD;
	 if(_calSpeed == NOT_SET) _calSpeed = TWELVEV_CAL_SPD;
	 if(minSpeed == NOT_SET) minSpeed = TWELVEV_MIN_SPD;
	 if(maxSpeed == NOT_SET) maxSpeed = TWELVEV_MAX_SPD;
	 if(turnSpeedHigh == NOT_SET) turnSpeedHigh = TWELVEV_TURN_SPD_HIGH;
	 if(turnSpeedLow == NOT_SET) turnSpeedLow = TWELVEV_TURN_SPD_LOW;
	 if(offCenterSetPoint == NOT_SET) offCenterSetPoint = TWELVEV_TURN_LOW_SET_POINT;
	 //if(drivePastDelay == NOT_SET) drivePastDelay = TWELVEV_DRIVE_PAST_DELAY;  
  } */
  
  if(useAFMS)
  {
	// Adafruit Motorshield v2.3

	//set both motors to stop
	setMotors(0,0);
	
  }
  
  else
  {
	// ardumoto or similar driver Dir and PWM based driver  
	  
	//Set motor driver control pins to be outputs
	pinMode(pwm_a, OUTPUT);
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a, OUTPUT);
	pinMode(dir_b, OUTPUT);

	//set both motors to stop
	setMotors(0,0);	 
  }

  
}

//************************
// run calibration sweep back and forth over line at speed of 0-100
// speed must be a 0 or positive value
// 0 will load the default values based upon voltage if declared.
void GobbitLineCommand::calibrateLineSensor(int calSpeed)
{
	if(calSpeed < 0) calSpeed = abs(calSpeed);
	
	if(!calSpeed) calSpeed = _calSpeed;
	else _calSpeed = calSpeed;
	
	// set speed with ratio of max possible for controller of 255 / max speed value
	//calSpeed=(calSpeed*255)/100;

  // calibrate line sensor; determines min/max range of sensed values for the current course
  for (int i = 0; i <= 100; i++)  // begin calibration cycle to last about 2.5 seconds (100*25ms/call)
  {

	// auto calibration sweeping left/right, tune 'calSpeed' motor speed at declaration
	// just high enough all sensors are passed over the line. Not too fast.
	if (i == 0 || i == 60) // slow sweeping turn right to pass sensors over line
	{
	  //digitalWrite(dir_a, LOW);
	  //analogWrite(pwm_a, calSpeed);
	  //digitalWrite(dir_b, HIGH);
	  //analogWrite(pwm_b, calSpeed);
		setMotors(calSpeed,-calSpeed);
	}

	else if (i == 20 || i == 100) // slow sweeping turn left to pass sensors over line
	{
	  //digitalWrite(dir_a, HIGH);
	  //analogWrite(pwm_a, calSpeed);
	  //digitalWrite(dir_b, LOW);
	  //analogWrite(pwm_b, calSpeed);
	  setMotors(-calSpeed,calSpeed);
	}

	qtrrc.calibrate(); // reads all sensors with the define set 2500 microseconds (25 milliseconds) for sensor outputs to go low.

  }  // end calibration cycle

  // read calibrated sensor values and obtain a measure of the line position from 0 to 7000
  linePosition = qtrrc.readLine(sensorValues);

  // read the value of only a single sensor to see the line.
  // when the value is greater than 200 the sensor sees the line.
  while (sensorValues[6] < 200)  // wait for line position to near center
  {
	linePosition = qtrrc.readLine(sensorValues);
  }

  // find near center
  while (linePosition > (3500 + offCenterSetPoint*3500)) // was 4350 continue loop until line position is near center
  {
	linePosition = qtrrc.readLine(sensorValues);
  }

	// brake left and stop right motors
	setMotors(100,0);
	
	//brake delay
	delay(15);
	
	// stop both motors
	setMotors(0,0);

	// delay as indicator setup and calibration is complete
	delay(1000);

} // end calibration


//*******************************
void GobbitLineCommand::drive(char turnDir) // drive loop receives turn request
{

  int incomplete = 1;

  while (incomplete) // re-run until turn instruction completed
  {

	if (turnDir == 'L')
	{
		
		// this was problematic for tracking.
		//if(!found_end)  // only becuase this was already done when a dead end was found
			// stop motors with full braking
		//	brakeMotors();
		
	  if (found_left)
	  {
		incomplete = 0;
		turn('L');
	  }
	  else if (found_end) noWayFreakOut();
	}

	else if (turnDir == 'R')
	{
		// this was problematic for tracking.
		//if(!found_end)  // only becuase this was already done when a dead end was found
			// stop motors with full braking
		//	brakeMotors();
	  
	  if (found_right)
	  {
		incomplete = 0;
		turn('R');
	  }
	  else if (found_end) noWayFreakOut();
	}

	// this would be a mistake so freak out and lock up
	else if (turnDir == 'F')
	{
	  if (found_end) noWayFreakOut();
	  else incomplete = 0;
	}

	else if (turnDir == 'S' && (found_left || found_right || found_straight || found_end))
	{
		
		if(!found_end)  // only becuase this was already done when a dead end was found
			// stop motors with full braking
			brakeMotors();

		return;
	}

	else if (turnDir == 'U')
	{
		if(!found_end)  // only becuase this was already done when a dead end was found
		// stop motors with full braking
			brakeMotors();
		
	  if (found_left && found_right)
	  {
		incomplete = 0;
		turn('R');
		turn('R');
	  }
	  else if (found_left)
	  {
		incomplete = 0;
		turn('R');
	  }
	  else if (found_right)
	  {
		incomplete = 0;
		turn('L');
	  }

	  else if (found_end)
	  {
		incomplete = 0;
		turn('R');
	  }

	}


	// reset flags
	found_left = 0;
	found_straight = 0;
	found_right = 0;
	found_end = 0;

	// read calibrated sensor values and obtain a measure of the line position from 0 to 7000
	unsigned int linePosition = qtrrc.readLine(sensorValues);

	findIntersection=true; // 
	followLine();

	if(!found_end)
	{
		byte left = 0;
		byte right = 0;
		
		// Drive straight a bit.  This helps us in case we entered the
		// intersection at an angle.
		setMotors(maxSpeed,maxSpeed);
		//delay(25);
		long mLastCommTime = millis();
		while(millis() - mLastCommTime < 200)
		{

			
			// Now read the sensors and check the intersection type.
			linePosition = qtrrc.readLine(sensorValues);

			// Check for left and right exits found.
			if (sensorValues[0] > 200 && !found_right)
			{
				found_right = 1;
				right = 1;
				//setMotors(0,0);
			}
			if (sensorValues[7] > 200 && !found_left)
			{
				found_left = 1;
				left = 1;
				//setMotors(0,0);
			}

			// check if sensor has passed either/both of the found lines
			if (sensorValues[0] < 200 && found_right)
			  right = 0;
			if (sensorValues[7] < 200 && found_left)
			  left = 0;
		  
			// if passed, exit loop
			if((found_right || found_left) && !right && !left)
				mLastCommTime = mLastCommTime + 200;
			else mLastCommTime = millis();

			// Drive straight a bit more - this is enough to line up our
			// wheels with the intersection.
			//setMotors(maxSpeed,maxSpeed);
			//delay(drivePastDelay);
			
			
		}

		linePosition = qtrrc.readLine(sensorValues);
		if (sensorValues[1] > 200 || sensorValues[2] > 200 || sensorValues[3] > 200 || sensorValues[4] > 200 || sensorValues[5] > 200 || sensorValues[6] > 200)
		  found_straight = 1;

		// check for tee
		if (found_left && found_right && !found_straight)
		{
		  // found tee
		  found_end = 1;
			// stop motors with full braking
			brakeMotors();
		}

		// stop both motors
		setMotors(0,0);

	}
	
	

  } // end while


}  // end drive function


//*******************************
void GobbitLineCommand::followLine() //follow the line
{

	float kpCurrent;
	float kdCurrent;

  lastError = 0;

  while (1)
  {

	// read calibrated sensor values and obtain a measure of the line position from 0 to 7000
	unsigned int linePosition = qtrrc.readLine(sensorValues);
	
	// simple line following portion
	// 0 is far Right sensor while 7 (7000 return) is far Left sensor
	switch (linePosition)
	{

	  // Line has moved off the left edge of sensor
	  // This will make it turn fast to the left
	  case 7000:
		if(!findIntersection)
		{
			setMotors(-turnSpeedHigh,turnSpeedHigh);
			offTheLine = 1;
		}
		break;

	  // Line had moved off the right edge of sensor
	  // This will make it turn fast to the right
	  case 0:
	    if(!findIntersection)
		{
			setMotors(turnSpeedHigh,-turnSpeedHigh);
			offTheLine = 1;
		}
		break;

	  // The line is still within the sensors.
	  // This will calculate adjusting speed to keep the line in center.
	  default:
		error = (float)linePosition - 3500; // 3500 is center measure of 7000 far left and 0 on far right

		// This sets the motor speed based on a proportional only formula.
		// _kp is the floating-point proportional constant you need to tune.
		// Maybe start with a _kp value around 0.05 and kd about 0.75, tuned in start functions or by setPD().
		// Note that it's very important you get your signs right, or else the
		// control loop will be unstable.
		
		// determine how great error is and set kp and kd aggressive if needed
		if(abs(error)>3500*smallRangePD)
		{
			kpCurrent=_kp*multiPD;
			kdCurrent=_kd*multiPD;

		}
		else
		{
			kpCurrent=_kp;
			kdCurrent=_kd;	
		}

		// calculate the new Process Variable
		// this is the value that will be used to alter the speeds
		//PV = _kp * error + _kd * (error - lastError);
		PV = kpCurrent * error + kdCurrent * (error - lastError);
		lastError = error;

		//this codes limits the PV (motor speed pwm value)
		// limit PV to maxSpeed - minSpeed
		if (PV > (maxSpeed-minSpeed))
		{

		  PV = (maxSpeed-minSpeed);
		}

		if (PV < -(maxSpeed-minSpeed))
		{
		  PV = -(maxSpeed-minSpeed);
		} 
		
		
		if(useRangeSensor)
		{
		sonarValue = analogRead(SonarPin);
		DistanceInch = (sonarValue * 0.497);
		obstacleSpeedFactor = speedAdjust(DistanceInch);
		}
		else obstacleSpeedFactor = 1;

		if (PV > 0)
		{
		  m1Speed = maxSpeed * obstacleSpeedFactor;
		  //m2Speed = (maxSpeed - PV) * obstacleSpeedFactor;
		  m2Speed = (maxSpeed - abs(PV))*obstacleSpeedFactor;
		  
		}

		if (PV < 0)
		{
		  //m1Speed = (maxSpeed + PV) * obstacleSpeedFactor;
		  m1Speed = (maxSpeed - abs(PV))*obstacleSpeedFactor;
		  m2Speed = maxSpeed * obstacleSpeedFactor;
		  
		}

		//set motor speeds
		setMotors(m2Speed,m1Speed);
		
		/* if(m2Speed>0)
		{
			digitalWrite(dir_a, LOW);
			analogWrite(pwm_a, m2Speed);
		}
		else
		{
			m2Speed=-m2Speed;
			digitalWrite(dir_a, HIGH);
			analogWrite(pwm_a, m2Speed);
		}			

		
		
		if(m1Speed>0)
		{
			digitalWrite(dir_b, LOW);
			analogWrite(pwm_b, m1Speed);
		}
		else
		{
			m1Speed=-m1Speed;
			digitalWrite(dir_b, HIGH);
			analogWrite(pwm_b, m1Speed);
		}			 */

		break;
	}

	
	// If this was called from the drive(), then we should be also looking for intersections.
	// if this was called from the main arduino loop, then it should not look for intersections and perform simple following
	// ...
	// We use the inner six sensors (1 thru 6) to
	// determine if there is a line straight ahead, and the
	// sensors 0 and 7 if the path turns.
	if(findIntersection)
	{
		if (sensorValues[1] < 100 && sensorValues[2] < 100 && sensorValues[3] < 100 && sensorValues[4] < 100 && sensorValues[5] < 100 && sensorValues[6] < 100)
		{
		  // There is no line visible ahead, and we didn't see any
		  // intersection.  Must be a dead end.
		  found_end = 1;

		  // stop both motors
		  //setMotors(0,0);
			// stop motors with full braking
			brakeMotors();

		  return;
		} 

		else if (sensorValues[0] > 200 || sensorValues[7] > 200 && !offTheLine)
		{
		  // Found an intersection.
		  return;
		}
	
	}
	
	offTheLine = 0;

  } // end while

} // end followLine


//*****************
// Turns to the sent variable of
// 'L' (left), 'R' (right), 'F' (straight), or 'U' (back)
// Tune 'turnSpeed' at declaration
void GobbitLineCommand::turn(char dir)
{
  switch (dir)
  {
	// Turn left 90deg
	case 'L':
	  //digitalWrite(dir_a, HIGH);
	  //analogWrite(pwm_a, turnSpeedHigh);
	  //digitalWrite(dir_b, LOW);
	  //analogWrite(pwm_b, turnSpeedHigh);
	  setMotors(-turnSpeedHigh,turnSpeedHigh);

	  linePosition = qtrrc.readLine(sensorValues);

	  while (sensorValues[6] < 200) // wait for outer most sensor to find the line
	  {					// was using sensor 6 but u turns where not going full 180
		linePosition = qtrrc.readLine(sensorValues);
	  }

	  // slow down speed
	  //analogWrite(pwm_a, turnSpeedLow);
	  //analogWrite(pwm_b, turnSpeedLow);
	  setMotors(-turnSpeedLow,turnSpeedLow);

	  // find center
	  while (linePosition > (3500 + offCenterSetPoint*3500))  // tune - wait for line position to find near center
	  {
		linePosition = qtrrc.readLine(sensorValues);
	  }

	  // stop both motors
		setMotors(0,0);
	  break;

	// Turn right 90deg
	case 'R':
	  //digitalWrite(dir_a, LOW);
	  //analogWrite(pwm_a, turnSpeedHigh);
	  //digitalWrite(dir_b, HIGH);
	  //analogWrite(pwm_b, turnSpeedHigh);
	  setMotors(turnSpeedHigh,-turnSpeedHigh);

	  linePosition = qtrrc.readLine(sensorValues);

	  while (sensorValues[1] < 200) // wait for outer most sensor to find the line
	  {					// was using sensor 1 but u turns where not going full 180
		linePosition = qtrrc.readLine(sensorValues);
	  }

	  // slow down speed
	  //analogWrite(pwm_a, turnSpeedLow);
	  //analogWrite(pwm_b, turnSpeedLow);
	  setMotors(turnSpeedLow,-turnSpeedLow);

	  // find center
	  while (linePosition < (3500 - offCenterSetPoint*3500))  // was 3250 tune - wait for line position to find near center
	  {
		linePosition = qtrrc.readLine(sensorValues);
	  }

	  // stop both motors
		setMotors(0,0);
	  break;

	// Straight ahead
	case 'F':
	  // do nothing
	  break;
  }
} // end turn


//******************
// spin around and stop forever.  Used to indicate could not do requested task.
void GobbitLineCommand::noWayFreakOut()
{

	setMotors(turnSpeedHigh,-turnSpeedHigh);
	delay(3000);

	// stop both motors
	setMotors(0,0);

  while (1);
}

//*******************************
//generate adjusting factor for speed based upon distance of object closer than safeDistanceInch
float GobbitLineCommand::speedAdjust(int currentDistance) 
{

  float speedTemp_var = 0;

  if (currentDistance < safeDistanceInch)
  {

	speedTemp_var = ((currentDistance / safeDistanceInch) * gain);

	// compare to value for shutoff of motors
	if (speedTemp_var > 1)
	{
	  speedTemp_var = 1;
	}

	else if (speedTemp_var < .35)
	{
	  speedTemp_var = 0;
	}

	return speedTemp_var;
  }

  else
  {
	return 1;
  }

}


//*******************************
// move function for basic motion without any line following or any sensor feedback.
// this will need to be used with moveSpeed and moveTurn values of -100 to 100
// -100 moveSpeed = reverse full speed; 100 = forward full speed; 0 = stopped;
// -100 moveTurn = full left; 100 = full right; 0 = straight 
void GobbitLineCommand::move(float moveSpeed, float moveTurn) // move function receives forward/reverse moveSpeed and left/right moveTurn value
{
	// reset flags
	found_left = 0;
	found_straight = 0;
	found_right = 0;
	found_end = 0;
	
	float thisTurnSpeed = 0;	
	
	if (moveSpeed != 0)
	{
	
		// moveTurn is full right
		if (moveTurn == 100) // adjust right
		{

			// forward motion as if wheels are turned to right
			if (moveSpeed > 0)
			{
				setMotors(moveSpeed,-moveSpeed);
			}
		
			// reverse motion as if backing up with wheels turned to right
			else  
			{	
				setMotors(moveSpeed,abs(moveSpeed));
			}
		}
	
		// moveTurn is full left
		else if (moveTurn == -100) // adjust left
		{
			// forward motion as if wheels are turned to left
			if (moveSpeed > 0)
			{
				setMotors(-moveSpeed,moveSpeed);
			}
		
			// reverse motion as if backing up with wheels turned to right
			else 
			{
				setMotors(abs(moveSpeed),moveSpeed);
			}			
		}

		// move turn is not full left or right
		else
		{
			// calculate slower motor speed
			thisTurnSpeed = abs(moveSpeed)*((100-abs(moveTurn))/100);
			
			// steer while moving forward
			if (moveSpeed > 0)
			{
				// steer to the right while moving forward
				if(moveTurn > 0)
				{							
					setMotors(moveSpeed, thisTurnSpeed);
				}
				
				// steer to the left while moving forward
				else if(moveTurn < 0)
				{	
					setMotors(thisTurnSpeed, moveSpeed);					
				}

				// turn is 0 so go straight forward
				else
				{
					setMotors(moveSpeed, moveSpeed);
				}			
			}
			
			// steer while moving in reverse
			else if (moveSpeed < 0)
			{
				// reverse motion as if backing up with wheels turned to right
				if(moveTurn > 0)
				{								
					setMotors(moveSpeed, -thisTurnSpeed);
				}
				
				// reverse motion as if backing up with wheels turned to left
				else if(moveTurn < 0)
				{
					setMotors(moveSpeed, -thisTurnSpeed);
				}

				// turn is 0 so go straight reverse
				else
				{
					setMotors(moveSpeed, moveSpeed);
				}	
			}	
		}
	}
  
	// must be 0 moveSpeed so stop both motors
	else  
	{
		setMotors(0,0);
	}
	
} // end move function


//*******************************
// sets the gripper servo pin#, degree of open position, degree of closed position.
void GobbitLineCommand::setGripPinOpenClosed(int pin, int open, int closed) 
{
	// set pin mode for gripper servo
	pinMode(pin, INPUT);
	
	gripPin = pin;  // only used as flag and feedback for serialPrintCurrentSettings
	gripOpenAngle = open;
	gripClosedAngle = closed;
	
	// attaches the servo on pin declared
	gripper.attach(pin);  
		
}


//*******************************
// closes the gripper to the declared closed angle
void GobbitLineCommand::gripClose(void) 
{
	// close gripper
	gripper.write(gripClosedAngle);
	delay(700);
	
}


//*******************************
// opens the gripper to the declared open angle
void GobbitLineCommand::gripOpen(void) 
{
	// open gripper
	gripper.write(gripOpenAngle);
	delay(700);
	
}


//*******************************
// backup with declared speed (100 max) and for a period of milliseconds
void GobbitLineCommand::backup(int speed, int delayTime)
{				
	
	// set speed with ratio of max possible for controller of 255 / max speed value
	//speed=speed*(255/100);
	
	// go straight reverse
	setMotors(-abs(speed),-abs(speed));
	
	delay(delayTime);
	
	// stop the motors
	setMotors(0,0);
}	


//*******************************
// sets the battery voltage which is used for choosing proper pd and motor tunings
// this function does not read from any pins
// 0 will force default of 9volt setttings
void GobbitLineCommand::setBatteryVolts(int unreadVolts) 
{
	
	volts = unreadVolts;  

}



//*******************************
// reads the battery voltage on declared analog pin, with voltage divider resistor values in K ohms
// 33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
// if using Lipo batteries, also use the checkBattery function to keep from running the battery too low and damaging your cells.
float GobbitLineCommand::readBatteryVolts(int analogPin, float smallResK, float largeResK)
{
	
  // set pin for battery reading
  pinMode(analogPin, INPUT);	
	
  volts = analogRead(analogPin); // read voltage on analog pin
  volts = (volts*5/1024)*((smallResK + largeResK) / smallResK); // convert read steps x volts/steps of DAC (5/1024) by volts with (total divider resistance of large + small resister) to small resistor multiplier ratio
	  //Serial.print(volts);
	  //Serial.println("Battery Voltage");
	  //Serial.println("");
	  
	return volts;
  
}


//*******************************
// checks the battery voltage on declared analog pin, cutoff voltage, and resistor values in K for voltage divider
// 33k and 100k resistors work well for a simple voltage divider up to 18 volts, conservatively.
// if using Lipo batteries, a value of 3.25 x (Number of cells) is a good value for minVoltage to avoid low voltage battery damage.
// alkaline, NiCAD, and NmHd batteries could be set lower, or even 0 if there is to be no cutoff.
void GobbitLineCommand::checkBattery(int analogPin, float minVoltage, float smallResK, float largeResK)
{
  
  // if voltage is below cutoff voltage, flash the onboard LED on pin 13 fast and do not run anything else 
  if (readBatteryVolts(analogPin, smallResK, largeResK) < minVoltage) while (1) // if using Lipo batteries, 3.25 per cell would be conservatively safe.
	{
	  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	  delay(200);              // wait for a second
	  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	  delay(120);              // wait for a second
	  //Serial.println("LOW VOLTAGE HOLD!!!!");
	  //Serial.print(volts);
	  //Serial.println("Battery Voltage");
	  //Serial.println("");
	}
}


//*******************************
// sets the Sonar/obstacle avoidance pin# (-1 disables), and safe distance/range to maintain.
void GobbitLineCommand::setSonar(int analogPin, float range) 
{
	
	if (analogPin>-1)
	{
		// set pin mode for sonar pin
		pinMode(analogPin, INPUT);
		
		SonarPin = analogPin;
		
		safeDistanceInch = range;
		
		// enable sonar range finder in line following
		useRangeSensor = 1;
		
	}	
	
	else useRangeSensor = 0;
		
}


//*******************************
// sets the kp and kd values
void GobbitLineCommand::setPD(float kp, float kd) 
{
	_kp = kp;
	_kd = kd;	

}


//*******************************
// sets the fast/aggresive/large error kp and kd multiplier and fraction/percentage of sensor range from middle to edge that should engage the multplier
// setting the multiplier to 1 disables.  less than 1 will be adjusted to result in same as 1.
// the fraction/percentage is from 0-1.0.  0 will force the multiplier active always, while 1 will leave the less aggressive always active.
void GobbitLineCommand::setFastPDmultiplier(float multiplier, float smallRange) 
{
	if (multiplier<1) multiplier =1;
	multiPD = multiplier;
	smallRangePD = smallRange;	

	//***** not default something indicator here so no auto voltage version???
}



//*******************************
// sets pins for qtr sensors, if default are not to be used
// all 8 sensors need to be used for this library
// MUST BE RUN BEFORE QTR IS INITIALIZED, OR BEFORE THE START() is run in setup
void GobbitLineCommand::setQTRpins(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8) 
{

	unsigned char qtrPins[]={pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8};

	unsigned char i;
	for (i = 0; i < NUM_SENSORS; i++)
	{
		sensorPins[i] = qtrPins[i];
	}
}



//*******************************
// opens the gripper to the declared open percent
// 0 is same as closed, 100 is same as fully open
void GobbitLineCommand::gripPercentOpen(int openPercent) 
{
	if (openPercent>100) openPercent=100;
	else if(openPercent<0) openPercent=0;
	
	// set gripper angle to position percent open
	int angle = (((abs(gripOpenAngle - gripClosedAngle))*openPercent)/100)+gripClosedAngle;
	gripper.write(angle);
	delay(700);
	
}


//*******************************
// sets the High and Low turning speeds when using Drive commands and the fraction/percentage of sensor range from middle to edge where the turning stops (useful since robot may continue swinging a little past the commanded stop).
// High and Low speeds 0-100.  High is used to make the larger portion of the turn quickly, while the Low is to reduce turn speed and creep up on the final position more accurately.
// Setting the speeds too low will not allow the robot to make the turn at all.
// the stopPoint fraction/percentage is from 0-1.0.  0 will force the High speed always, while 1 will always use the low speed.
void GobbitLineCommand::setDriveTurnHighLowStoppoint(int highTurn, int lowTurn, float stopPoint) 
{
	if (stopPoint>1) stopPoint =1;
	else if(stopPoint<0) stopPoint=0;
	
	turnSpeedHigh = highTurn;  
	turnSpeedLow = lowTurn; 
	offCenterSetPoint = stopPoint;

	//***** not default something indicator here so no auto voltage version???
}



//*******************************
// sets the Minimum and Maximum drive() speeds.  This does not currently affect the move() speeds.
// Min and Max speeds -100 to 100.  
// 0 for both will lead to no movement.
// values > 0 for Minimum will make turns while following less sharp.
// values 0 or less for Maximum may result in unpredictable actions
void GobbitLineCommand::setDriveMinMaxSpeeds(int minumum, int maximum) 
{
	if (minumum>100) minumum =100;
	else if(minumum<-100) minumum=-100;
	
	if (maximum>100) maximum =100;
	else if(maximum<-100) maximum=-100;
	
	// set speed with ratio of max possible for controller of 255 / max speed value
	//minumum=minumum*(255/100);
	
	// set speed with ratio of max possible for controller of 255 / max speed value
	//maximum=maximum*(255/100);
	
	minSpeed = minumum;
	maxSpeed = maximum;

	//***** not default something indicator here so no auto voltage version???
}



//*******************************
// sets the custom Left Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed.
void GobbitLineCommand::setLeftMotorPinsDirPWM(int dirPin, int pwmPin) 
{
	
	// set pin variables
	dir_a = dirPin;  //direction control for Left motor
	pwm_a = pwmPin;  //PWM speed control for Left motor	
}



//*******************************
// sets the custom Right Motor driver pin variables for simple direction and PWM style drivers, such as L298 type.
// dirPin sets direction.  LOW is Forward, HIGH is Reverse
// pwmPin sets speed. 
void GobbitLineCommand::setRightMotorPinsDirPWM(int dirPin, int pwmPin) 
{
	
	// set pin variables
	dir_b = dirPin;  //direction control for Right motor
	pwm_b = pwmPin;  //PWM speed control for Right motor
		
}


//*******************************
// serial prints all of the current settings and values
// do not run this in the main program loop.
void GobbitLineCommand::serialPrintCurrentSettings(void)
{
	  Serial.begin(9600);
	  
	  Serial.println("THESE ARE YOUR CURRENT SETTINGS");
	  Serial.println();
	  
	  //if(ADAFRUIT_MS) Serial.println("Adafruit MotorShield");
	  if(useAFMS) Serial.println("Adafruit MotorShield"); 
	  else 
	  {
		Serial.println("Ardumoto MotorShield or similar with Output Pins:"); 
		Serial.print(dir_a); 
		Serial.println("   Left Motor Direction Pin"); 
		Serial.print(pwm_a); 
		Serial.println("   Left Motor PWM/Speed Pin"); 
		Serial.print(dir_b); 
		Serial.println("   Right Motor Direction Pin");
		Serial.print(pwm_b);
		Serial.println("   Right Motor PWM/Speed Pin");		
	  }
	  Serial.println();
	  
	  Serial.println("QTR-8RC Input Pins");
	  Serial.print(sensorPins[0]);
	  Serial.print(", ");
	  Serial.print(sensorPins[1]);
	  Serial.print(", ");
	  Serial.print(sensorPins[2]);
	  Serial.print(", ");
	  Serial.print(sensorPins[3]);
	  Serial.print(", ");
	  Serial.print(sensorPins[4]);
	  Serial.print(", ");
	  Serial.print(sensorPins[5]);
	  Serial.print(", ");
	  Serial.print(sensorPins[6]);
	  Serial.print(", ");
	  Serial.println(sensorPins[7]); 
	  Serial.println();
	  
	  if(useRangeSensor)
	  {
		  Serial.println("Sonar Range Finder Enabled.");
		  Serial.print(SonarPin);
		  Serial.println("   Input Pin");
		  Serial.print(safeDistanceInch);
		  Serial.println("   Inches");  
	  }
	  else Serial.println("Sonar Range Finder is Disabled.");
	  Serial.println();	
	  
	  if(gripPin != NOT_SET)
	  {
		  Serial.println("Gripper is Enabled.");
		  Serial.print(gripPin);
		  Serial.println("   Input Pin");
		  Serial.print(gripOpenAngle);
		  Serial.println("   Open Angle"); 
		  Serial.print(gripClosedAngle);
		  Serial.println("   Closed Angle");  
		  }
	  else Serial.println("Gripper is Disabled.");
	  Serial.println();	
	    
	  Serial.print(volts);
	  Serial.println("   Volts");
	  Serial.println();
	  
	  Serial.print(_kp);
	  Serial.println("   P variable");
	  Serial.println();
	  
	  Serial.print(_kd);
	  Serial.println("   D variable");
	  Serial.println();
	  
	  Serial.print(multiPD);
	  Serial.println("   PD Multiplier");
	  Serial.println();
	  
	  Serial.print(smallRangePD);
	  Serial.println("   Small PD Range of Sensor");
	  Serial.println();
	  
	  Serial.print(_calSpeed);
	  Serial.println("   DEFAULT Calibration Motor Speed, not value entered at calibrateLineSensor('value')");
	  Serial.println();
	  
	  Serial.print(minSpeed);
	  Serial.println("   Minimum Following and Drive Motor Speed");
	  Serial.println();
	  
	  Serial.print(maxSpeed);
	  Serial.println("   Maximum Following and Drive Motor Speed");
	  Serial.println();
	  
	  Serial.print(turnSpeedHigh);
	  Serial.println("   High Motor Turn Speed for Drive Command");
	  Serial.println();
	  
	  Serial.print(turnSpeedLow);
	  Serial.println("   Low Motor Turn Speed for Drive Command");
	  Serial.println();
	  
	  
	  Serial.print(offCenterSetPoint);
	  Serial.println("   Drive Turning Stop Point Range of Sensor");
	  Serial.println();
	  
	  //Serial.print(drivePastDelay);
	 // Serial.println("   Drive Past Time in Milliseconds");
	  //Serial.println();	  
	
}


//*******************************
// set the speed and direction of the motors for both motor shield/driver types 
// left/rightVelocity -100 (full backwards) to +100 (full forward) where 0 means stop.
void GobbitLineCommand::setMotors(float leftVelocity, float rightVelocity)
{
	// set speed with ratio of max possible for controller of 255 / max speed value
	leftVelocity=leftVelocity*2.55;
	rightVelocity=rightVelocity*2.55;
	
	//	limit velocities to 255 range of PWM
	if (leftVelocity > 255) leftVelocity = 255;
	else if (leftVelocity < -255) leftVelocity = -255;
	if (rightVelocity > 255) rightVelocity = 255;
	else if (rightVelocity < -255) rightVelocity = -255;	

	
	// if Adafruit Motorshield v2.3 was defined
	// Set the speed to start, from 0 (off) to 255 (max speed)
	if(useAFMS)
	{
		// Left stop motor
		if (leftVelocity == 0)
		{
			leftMotor->setSpeed(0);
			leftMotor->run(FORWARD);
			//leftMotor->run(RELEASE);
		}
	
		// Left forward
		else if(leftVelocity > 0)
		{								
			leftMotor->run(FORWARD);			
			leftMotor->setSpeed(leftVelocity);
		}
		
		// Left Backward
		else if(leftVelocity < 0)
		{	
			leftMotor->run(BACKWARD);			
			leftMotor->setSpeed(abs(leftVelocity));		
		}

		// Right stop motor
		if (rightVelocity == 0)
		{
			rightMotor->setSpeed(0);
			rightMotor->run(FORWARD);
			//rightMotor->run(RELEASE);
		}
	
		// Right forward
		else if(rightVelocity > 0)
		{								
			rightMotor->run(FORWARD);			
			rightMotor->setSpeed(rightVelocity);
		}
		
		// Right Backward
		else if(rightVelocity < 0)
		{	
			rightMotor->run(BACKWARD);			
			rightMotor->setSpeed(abs(rightVelocity));		
		}
	}
  
	// ardumoto or similar driver Dir and PWM based driver
	else
	{
		
		// Left stop motor
		if (leftVelocity == 0)
		{
			analogWrite(pwm_a, 0);
		}
	
		// Left forward
		else if(leftVelocity > 0)
		{							
			digitalWrite(dir_a, LOW);
			analogWrite(pwm_a, leftVelocity);
		}
		
		// Left Backward
		else if(leftVelocity < 0)
		{	
			digitalWrite(dir_a, HIGH);
			analogWrite(pwm_a, abs(leftVelocity));		
		}

		
		// Right stop motor
		if (rightVelocity == 0)
		{
			analogWrite(pwm_b, 0);
		}
	
		// Right forward
		else if(rightVelocity > 0)
		{							
			digitalWrite(dir_b, LOW);
			analogWrite(pwm_b, rightVelocity);
		}
		
		// Right Backward
		else if(rightVelocity < 0)
		{	
			digitalWrite(dir_b, HIGH);
			analogWrite(pwm_b, abs(rightVelocity));		
		}
	}
	
} 


//*******************************
// set the speed and direction of the motors for both motor shield/driver types 
// left/rightVelocity -100 (full backwards) to +100 (full forward) where 0 means stop.
void GobbitLineCommand::brakeMotors(void)
{
	
	// stop both motors
	setMotors(0,0);
	
	// brake both motors
	setMotors(-100,-100);
		
	delay(23);
		
	// brake left motor a little longer
	setMotors(-100,0);
		
	delay(5);
		
	// stop both motors
	setMotors(0,0);
}

