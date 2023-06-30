/************************************************************************************************************
 * Description: Ultrasonic sensor application to measure the distance using the ultrasonic sensor.
 * 
 * Author: Hossam Mohamed
 * 
 * Date: 30 jun 2023
 * 
 ************************************************************************************************************/ 

#include "Ultrasonic_sensor_APP.h"
#include <avr/interrupt.h>
void Ultrasonic_sensor(void)
{
	uint16 distance = 0;
	/* INITIALIZATION */
	Ultrasonic_init();
	LCD_init();

	LCD_displayString("Distance = ");

	//! enable global interrupt
	sei();
	while (1)
	{
		/* APPLICATION CODE */

		/* read the distance */
		distance = Ultrasonic_readDistance();

		//_delay_ms(50);
		/* display the distance on the LCD */
		LCD_Goto_XY(0, 10);
		LCD_displayInteger(distance);
		LCD_displayString(" cm");
	}
}