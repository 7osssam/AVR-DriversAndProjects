/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: ICU.c
 *
 * Description: Source file for the AVR ICU driver
 *
 * Author: Mohamed Tarek & Hosam Mohamed
 *
 *******************************************************************************/

#include "ICU.h"
#include "BIT_MACROS.h"

#include <avr/interrupt.h> /* For ICU ISR */
#include <avr/io.h>		   /* To use ICU/Timer1 Registers */
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER1_CAPT_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const ICU_ConfigType *Config_Ptr)
{
	/* Configure ICP1/PD6 as input pin */
	CLEAR_BIT(DDRD, PD6);

	/* Timer1 always operates in Normal Mode */
	/* FOC1A/B = 1 when WGM1[1:0] = 00 to force compare match and non-PWM mode */
	SET_MASK(TCCR1A, BIT(FOC1A) | BIT(FOC1B)); //!

	/*
	 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
	 * of TCCR1B Register
	 * 0xF8 to make the last 5 bits = 0b11111000
	 */
	Timer1_ConfigType ICU_Config = {0, 0, TIMER1_NORMAL_MODE, Config_Ptr->clock, OCRA_DISCONNECTED, OCRB_DISCONNECTED};
	Timer1_init(&ICU_Config);

	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 * 1 for rising edge and 0 for falling edge on ICP1/PD6 pin
	 * 0xBF to make the 6th bit = 0b10111111
	 */

	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge) << PD6);

	/* Initial Value for Timer1 */
	TCNT1 = 0;

	/* Initial Value for the input capture register */
	ICR1 = 0;

	/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
	SET_BIT(TIMSK, TICIE1);
}

/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBack(void (*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const ICU_EdgeType a_edgeType)
{
	/*
	 * insert the required edge type in ICES1 bit in TCCR1B Register
	 * 0xBF to make the 6th bit = 0b10111111
	 */
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType << PD6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void)
{
	/* Clear All Timer1/ICU Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	CLEAR_BIT(TIMSK, TICIE1);

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;
}
