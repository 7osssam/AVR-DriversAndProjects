/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name:  DCMOTOR.c
 *
 * Description: Source file for the DC Motor driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#include "DCMOTOR.h"
#include "TIMER.h"

void DCMOTOR_init(void)
{
	// set Direction of DCMOTOR_PIN_1 and DCMOTOR_PIN_2 as output pins
	GPIO_setupPinDirection(DCMOTOR_PORT, DCMOTOR_PIN_1, PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_PORT, DCMOTOR_PIN_2, PIN_OUTPUT);

	// set initial state of DCMOTOR_PIN_1 and DCMOTOR_PIN_2 as low
	GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_1, LOGIC_LOW);
	GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_2, LOGIC_LOW);
}

void DCMOTOR_Rotate(DCMOTOR_State_t state, uint8 speed)
{
	uint8 duty_cycle = (speed * 255) / 100;

	Timer0_ConfigType PWM_config = {duty_cycle, TIMER0_FASTPWM_MODE, F_CPU_8, OC0_NON_INVERTING};

	Timer0_init(&PWM_config);

	GPIO_setupPinDirection(PWM_pin_PORT_ID, PWM_pin_PIN_ID, PIN_OUTPUT);

	switch (state)
	{
	case DCMOTOR_STOP:
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_1, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_2, LOGIC_LOW);
		break;
	case DCMOTOR_CLOCKWISE:
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_1, LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_2, LOGIC_LOW);
		break;
	case DCMOTOR_ANTI_CLOCKWISE:
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_1, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT, DCMOTOR_PIN_2, LOGIC_HIGH);
		break;
	}
}