#include "KEYPAD.h"
#include "LCD.h"
#include "SETTINGS.h"
#include <util/delay.h>

#include "calculator.h"

int main(void)
{

	LCD_init();

	LCD_displayStringCenter(1, "CALCULATOR");
	_delay_ms(100);
	LCD_displayStringCenter(0, "2 Modes");

	_delay_ms(500);
	LCD_clearScreen(); /* clear LCD at the beginning */
	while (1)
	{
		menu();
	}
}
