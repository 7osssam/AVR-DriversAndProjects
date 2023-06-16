
#include "lcd.h"
#include "SETTINGS.h"

#include <util/delay.h>
int main(void)
{
	LCD_init();
	LCD_displayString("Hello World");



	char num = 0;
	while (1)
	{
		num = KEYPAD_getPressedKey();

		if (num != 0)
		{
			LCD_clearScreen();
			LCD_displayNumber(num);
		}
	}
}