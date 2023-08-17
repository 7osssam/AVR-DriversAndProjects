/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name:  DCMOTOR.h
 *
 * Description: Header file for the DC Motor driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "BIT_MACROS.h"
#include "GPIO.h"
#include "STD_TYPES.h"
#include <avr/io.h>
/*********************** Configuration Macros ************************************/

#define DCMOTOR_PORT 		PORTB_ID

#define DCMOTOR_PIN_1 		PIN0_ID
#define DCMOTOR_PIN_2 		PIN1_ID
/********************** Data Types Definition ***********************************/

typedef enum
{
	DCMOTOR_STOP,
	DCMOTOR_CLOCKWISE,
	DCMOTOR_ANTI_CLOCKWISE
} DCMOTOR_State_t;

/*************************Functions Prototypes *********************************/

/*******************************************************************************
 * Description: Function to initialize the DC Motor driver by:
 * 				- Set the direction of the GPIO pins connected to the DC Motor as output pins
 * 				- Set the initial value of the GPIO pins connected to the DC Motor as low
 * @param void
 * @return void
 *******************************************************************************/
void DCMOTOR_init(void);

/*******************************************************************************
 * Description: Function to rotate the DC Motor
 * @param state
 * 		  Description: State of the DC Motor
 * 		  Type: DCMOTOR_State_t
 * 		  Values: DCMOTOR_STOP, DCMOTOR_CLOCKWISE, DCMOTOR_ANTI_CLOCKWISE
 * @param speed
 * 		  Description: Speed of the DC Motor
 * 		  Type: uint8
 * 		  Values: 0 ~ 100
 * @return void
 *******************************************************************************/
void DCMOTOR_Rotate(DCMOTOR_State_t state, uint8 speed);

#endif /* DCMOTOR_H_ */