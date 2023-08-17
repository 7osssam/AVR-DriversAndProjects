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

#include "SETTINGS.h"
#include <util/delay.h>

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

void Buzzer_Alarm(void)
{
	// Generate alarm sound pattern
	for (int i = 0; i < 3; i++)
	{
		Buzzer_on();
		_delay_ms(100);
		Buzzer_off();
		_delay_ms(100);
	}

	_delay_ms(1000); // Pause between alarm patterns
}