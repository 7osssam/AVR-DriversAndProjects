/******************************************************************************
 *
 * Module: Ultrasonic
 *
 * File Name: Ultrasonic.c
 *
 * Description: Source file for the Ultrasonic driver 
 * 				- HC-SR04 Ultrasonic Sensor
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "SETTINGS.h" // F_CPU
#include "STD_TYPES.h" // uint16

/************************************************************************/
/*					static Configurations                               */
/************************************************************************/
#define ULTRASONIC_TRIG_PORT_ID 		PORTB_ID
#define ULTRASONIC_TRIG_PIN_ID 			PIN5_ID

#define ULTRASONIC_ECHO_PORT_ID 		PORTB_ID
#define ULTRASONIC_ECHO_PIN_ID 			PIN6_ID

/************************************************************************/
/*					constant definitions                                */
/************************************************************************/
#define SOUND_SPEED 					34300 // cm/s
#define PRESCALER 						8	  // ICU prescaler
#define TIME_OF_EACH_CYCLE 				((double)(PRESCALER) / (F_CPU)) // F_CPU from SETTINGS.h

/************************************************************************/
/*					Functions Prototypes                                */
/************************************************************************/

/*
 * Description: Function to initialize the ultrasonic module by:
 *              1. Set Trigger pin as output.
 * 				2. Set Echo pin as input.
 * 				3. Set Trigger pin to low.
 * 				4. Initialize ICU.
 * 				5. Set ICU callback function.
 * @param void
 * @return void
 */
void Ultrasonic_init(void);

/*
 * Description: Function to calculate the distance by:
 *              1. Check if the edge is rising or falling.
 * 				2. If rising, save the time.
 * 				3. If falling, calculate the distance.
 * @param void
 * @return void
 *
 * Note: This function is a callback function used by ICU.
 */
void Ultrasonic_edgeProcessing(void);

/*
 * Description: Function to read the distance by:
 *              1. Trigger the ultrasonic module.
 * 				2. Wait for the echo to be received.
 * 				3. Calculate distance.
 * @param void
 * @return uint16
 */
uint16 Ultrasonic_readDistance(void);

#endif /* ULTRASONIC_H_ */