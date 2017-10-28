/*
*	GobbitLineCommand.h - Tuning variables for the Gobbit robot while using the Adafruit Motor Shield v2.3
*	Library for line following, intersection detection, and basic motor control of Gobbit robot.
*	Created by Jason Talley 
*	Last edit 10/28/2017
*	Released under GNU agreement
*/

#ifndef AdafruitMSDefaults_h
#define AdafruitMSDefaults_h

	// adafruit motor shield v2.3

	// default values
	// speeds are based on +/-100 max
	
	#define BRAKE_STRENGTH_AF 100

	// 7.2 and 7.4v
	#define SEVENV_AF_KP	0.05
	#define SEVENV_AF_KI	0.001
	#define SEVENV_AF_KD	0.9
	#define SEVENV_AF_KP_COARSE	0.1
	#define SEVENV_AF_KI_COARSE	0.001
	#define SEVENV_AF_KD_COARSE	1.4
	#define SEVENV_AF_PD_FINE_RANGE	0.04
	#define SEVENV_AF_CAL_SPD	47
	#define SEVENV_AF_MIN_SPD	-100
	#define SEVENV_AF_MAX_SPD	100
	#define SEVENV_AF_BRAKE_SPD	100
	#define SEVENV_AF_TURN_SPD_HIGH	100 
	#define SEVENV_AF_TURN_SPD_LOW	30

	// 9 and 9.6v
	#define NINEV_AF_KP	0.05
	#define NINEV_AF_KI	0.0005
	#define NINEV_AF_KD	0.80
	#define NINEV_AF_KP_COARSE	0.1
	#define NINEV_AF_KI_COARSE	0.001
	#define NINEV_AF_KD_COARSE	1.6
	#define NINEV_AF_PD_FINE_RANGE	0.05
	#define NINEV_AF_CAL_SPD	40
	#define NINEV_AF_MIN_SPD	-100
	#define NINEV_AF_MAX_SPD	100
	#define NINEV_AF_BRAKE_SPD	80
	#define NINEV_AF_TURN_SPD_HIGH	90
	#define NINEV_AF_TURN_SPD_LOW	30

	// 11.1v
	#define ELEVENV_AF_KP	0.05
	#define ELEVENV_AF_KI	0.0005
	#define ELEVENV_AF_KD	0.80
	#define ELEVENV_AF_KP_COARSE	0.1
	#define ELEVENV_AF_KI_COARSE	0.001
	#define ELEVENV_AF_KD_COARSE	1.6 
	#define ELEVENV_AF_PD_FINE_RANGE	0.05
	#define ELEVENV_AF_CAL_SPD	30
	#define ELEVENV_AF_MIN_SPD	-100
	#define ELEVENV_AF_MAX_SPD	100
	#define ELEVENV_AF_BRAKE_SPD	65
	#define ELEVENV_AF_TURN_SPD_HIGH	60 
	#define ELEVENV_AF_TURN_SPD_LOW	20
	
	// pin defaults
	//#define AFV23_DIRA	
	//#define AFV23_PWMA
	//#define AFV23_DIRB
	//#define AFV23_PWMB
	#define AFV23_QTR0	2
	#define AFV23_QTR1	4
	#define AFV23_QTR2	5
	#define AFV23_QTR3	6
	#define AFV23_QTR4	7
	#define AFV23_QTR5	8
	#define AFV23_QTR6	9
	#define AFV23_QTR7	10
	
#endif