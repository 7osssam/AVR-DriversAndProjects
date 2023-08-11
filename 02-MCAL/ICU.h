/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: ICU.h
 *
 * Description: Header file for the AVR ICU driver
 *
 * Author: Mohamed Tarek & Hosam Mohamed
 *
 *******************************************************************************/
#ifndef ICU_H_
#define ICU_H_

#include "STD_TYPES.h"
#include "timer.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	FALLING,
	RISING

} ICU_EdgeType;

typedef struct
{
	Prescaler_type clock;
	ICU_EdgeType edge;
} ICU_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const ICU_ConfigType *Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBack(void (*a_ptr)(void));

/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const ICU_EdgeType edgeType);

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 * return: The Timer1 Value when the input is captured 16-bit
 */
uint16 ICU_getInputCaptureValue(void);

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void);

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void);

#endif /* ICU_H_ */
