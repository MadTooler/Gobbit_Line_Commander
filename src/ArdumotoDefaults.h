/*
*	GobbitLineCommand.h - Tuning variables for the Gobbit robot while using the Ardumoto Motor Shield from SparkFun
*	Library for line following, intersection detection, and basic motor control of Gobbit robot.
*	Created by Jason Talley 
*	Last edit 10/28/2017
*	Released under GNU agreement
*/

#ifndef ArdumotoDefaults_h
#define ArdumotoDefaults_h

	// ardumoto motor shield or L298 style motor driver
		
	// default values
	// speeds are based on +/-100 max
	
	#define BRAKE_STRENGTH 100

	// 7.2 and 7.4v
	#define SEVENV_KP	0.15
	#define SEVENV_KI	0.002
	#define SEVENV_KD	4
	#define SEVENV_KP_COARSE	0.25
	#define SEVENV_KI_COARSE	0.001
	#define SEVENV_KD_COARSE	2.5
	#define SEVENV_PD_FINE_RANGE	0.04
	#define SEVENV_CAL_SPD	50
	#define SEVENV_MIN_SPD	-100
	#define SEVENV_MAX_SPD	100
	#define SEVENV_BRAKE_SPD	100
	#define SEVENV_TURN_SPD_HIGH	100 
	#define SEVENV_TURN_SPD_LOW	50

	// 9 and 9.6v 
	#define NINEV_KP	0.15
	#define NINEV_KI	0.002
	#define NINEV_KD	4
	#define NINEV_KP_COARSE	0.2
	#define NINEV_KI_COARSE	0.001
	#define NINEV_KD_COARSE	3.4
	#define NINEV_PD_FINE_RANGE	0.04
	#define NINEV_CAL_SPD	40
	#define NINEV_MIN_SPD	-100
	#define NINEV_MAX_SPD	100
	#define NINEV_BRAKE_SPD	80
	#define NINEV_TURN_SPD_HIGH	90
	#define NINEV_TURN_SPD_LOW	45

	// 11.1v and above
	#define ELEVENV_KP	0.15
	#define ELEVENV_KI	0.002
	#define ELEVENV_KD	4
	#define ELEVENV_KP_COARSE	0.2
	#define ELEVENV_KI_COARSE	0.001
	#define ELEVENV_KD_COARSE	4.4
	#define ELEVENV_PD_FINE_RANGE	0.05
	#define ELEVENV_CAL_SPD	30
	#define ELEVENV_MIN_SPD	-100
	#define ELEVENV_MAX_SPD	100
	#define ELEVENV_BRAKE_SPD	65
	#define ELEVENV_TURN_SPD_HIGH	60 
	#define ELEVENV_TURN_SPD_LOW	25
	
	// ardumoto v14 pin defaults
	#define AMV14_DIRA	12
	#define AMV14_PWMA	3
	#define AMV14_DIRB	13
	#define AMV14_PWMB	11
	#define AMV14_QTR0	2
	#define AMV14_QTR1	4
	#define AMV14_QTR2	5
	#define AMV14_QTR3	6
	#define AMV14_QTR4	7
	#define AMV14_QTR5	8
	#define AMV14_QTR6	9
	#define AMV14_QTR7	10
	
	// ardumoto v20 pin defaults
	#define AMV20_DIRA	4
	#define AMV20_PWMA	11
	#define AMV20_DIRB	2
	#define AMV20_PWMB	3
	#define AMV20_QTR0	5
	#define AMV20_QTR1	6
	#define AMV20_QTR2	7
	#define AMV20_QTR3	8
	#define AMV20_QTR4	9
	#define AMV20_QTR5	10	
	#define AMV20_QTR6	12
	#define AMV20_QTR7	13
		


#endif