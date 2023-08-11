/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.h
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "Buzzer.h"

void Buzzer_init()
{
	// Set the direction of the Buzzer pin to be output pin
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	// Set the initial value of the Buzzer pin to be LOW
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
void Buzzer_on()
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}
void Buzzer_off()
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}