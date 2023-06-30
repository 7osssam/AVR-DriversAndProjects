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

#include "Ultrasonic.h"
#include "BIT_MACROS.h"
#include "GPIO.h"
#include "ICU.h"

#include "SETTINGS.h"	// for F_CPU
#include <util/delay.h> // for _delay_ms()

/************************************************************************/
/*					Private global variables                            */
/************************************************************************/
static volatile uint16 Ultrasonic_distance = 0;
static volatile uint16 Ultrasonic_read = 0;
static volatile uint8 edgeFlag = 0;

/************************************************************************/
/*					Functions Definitions                                */
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
void Ultrasonic_init(void)
{
	/* ICU configuration structure */
	ICU_ConfigType ICU_Config = {F_CPU_8, RISING};
	/* Set Trigger pin as output */
	GPIO_setupPinDirection(ULTRASONIC_TRIG_PORT_ID, ULTRASONIC_TRIG_PIN_ID, PIN_OUTPUT);
	/* Set Echo pin as input */
	GPIO_setupPinDirection(ULTRASONIC_ECHO_PORT_ID, ULTRASONIC_ECHO_PIN_ID, PIN_INPUT);
	/* Set Trigger pin to low */
	GPIO_writePin(ULTRASONIC_TRIG_PORT_ID, ULTRASONIC_TRIG_PIN_ID, LOGIC_LOW);
	/* Initialize ICU */
	ICU_init(&ICU_Config);
	/* Set ICU callback function */
	ICU_setCallBack(Ultrasonic_edgeProcessing);
}

/*
 * Description: Function to trigger the ultrasonic module by:
 *              1. Send 10us pulse to trigger pin.
 * @param void
 * @return void
 *
 * Note: This function is a private function used only by Ultrasonic_readDistance().
 */
static void Ultrasonic_Trigger(void)
{
	/* Send 10us pulse to trigger pin */
	GPIO_writePin(ULTRASONIC_TRIG_PORT_ID, ULTRASONIC_TRIG_PIN_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_TRIG_PORT_ID, ULTRASONIC_TRIG_PIN_ID, LOGIC_LOW);
}

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
void Ultrasonic_edgeProcessing(void)
{
	if (edgeFlag == 0)
	{

		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);
		edgeFlag = 1;
	}
	else if (edgeFlag == 1)
	{
		/* get the edge time */
		Ultrasonic_read = ICU_getInputCaptureValue();
		ICU_setEdgeDetectionType(RISING);
		edgeFlag = 0;
	}
}

/*
 * Description: Function to read the distance by:
 *              1. Trigger the ultrasonic module.
 * 				2. Wait for the echo to be received.
 * 				3. Calculate distance.
 * @param void
 * @return uint16
 */
uint16 Ultrasonic_readDistance(void)
{
	/* Trigger the ultrasonic module */
	Ultrasonic_Trigger();
	/* wait for the echo to be received */
	_delay_ms(100);

	/* Calculate distance */
	Ultrasonic_distance = 1 + (((double)Ultrasonic_read * TIME_OF_EACH_CYCLE * SOUND_SPEED) / 2.0);
	// Ultrasonic_distance = 1 + (Ultrasonic_read / 58);

	return Ultrasonic_distance;
}