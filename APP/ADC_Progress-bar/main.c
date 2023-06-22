#include "ADC.h"
#include "LCD.h"
#include "SETTINGS.h"
#include "util/delay.h"

#define LCD_NUMBER_OF_CHARACTERS 	16
#define ADC_MAX_VALUE 				1023
#define Vref 						5.0

uint8 g_lastPattern; // to keep track of the last pattern displayed
uint8 g_counter;	 // to keep track of the number of bits displayed

uint8 Pattern1[8] = {
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10};

uint8 Pattern2[8] = {
	0x18,
	0x18,
	0x18,
	0x18,
	0x18,
	0x18,
	0x18,
	0x18};

uint8 Pattern3[8] = {
	0x1C,
	0x1C,
	0x1C,
	0x1C,
	0x1C,
	0x1C,
	0x1C,
	0x1C};

uint8 Pattern4[8] = {
	0x1E,
	0x1E,
	0x1E,
	0x1E,
	0x1E,
	0x1E,
	0x1E,
	0x1E};

uint8 Pattern5[8] = {
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F};

int main(void)
{
	volatile double percent = 100.0; // 100% by default
	uint16 res_value;
	uint8 x; // iterator

	volatile double volt;

	LCD_init();							   /* initialize LCD driver */
	ADC_init(ADC_AVCC, ADC_PRESCALER_128); /* initialize ADC driver */

	LCD_displaySpecialCharacter(Pattern1, 0);
	LCD_displaySpecialCharacter(Pattern2, 1);
	LCD_displaySpecialCharacter(Pattern3, 2);
	LCD_displaySpecialCharacter(Pattern4, 3);
	LCD_displaySpecialCharacter(Pattern5, 4);
	LCD_Goto_XY(1, 0);

	while (1)
	{
		/* Read ADC channel 0 where the potentiometer is connected */
		res_value = ADC_readChannel(0);

		percent = (res_value / (double)ADC_MAX_VALUE) * 100.0;

		volt = (percent / 100.0) * 5.0;

		volt = (Vref / ADC_MAX_VALUE) * res_value;

		LCD_Goto_XY(0, 0);
		LCD_displayNumber(res_value);
		LCD_displayCharacter(':');
		LCD_displayFloat(volt, 2);
		LCD_displayString("V|");
		LCD_displayNumber(percent);
		LCD_displayCharacter('%');

		LCD_Goto_XY(1, 0);

		double bar_width = ((double)LCD_NUMBER_OF_CHARACTERS / 100.0) * percent;

		if (bar_width >= 1)
		{
			for (x = 1; x < bar_width; x++)
			{
				LCD_displayCharacter(4); // the location of the full 5-bits bar

				// g_counter = x;			 // to keep track of the number of bits displayed
			}

			bar_width = bar_width - (x - 1); // to get the remaining width of the bar
		}

		g_lastPattern = bar_width * 5; // multiplying by 5 to get the location of the last pattern

		switch (g_lastPattern)
		{
		case 0:
			break;
		case 1: // the location of the 1-bit bar
			LCD_displayCharacter(0);
			break;
		case 2: // the location of the 2-bits bar
			LCD_displayCharacter(1);
			break;
		case 3: // the location of the 3-bits bar
			LCD_displayCharacter(2);
			break;
		case 4: // the location of the 4-bits bar
			LCD_displayCharacter(3);
			break;
			// case 5: // the location of the full 5-bits bar
			//	LCD_displayCharacter(4);
			//	break;
		}

		// Clearing the remaining line of the bar when decreasing the percentage
		for (int i = g_lastPattern; i < 5; i++)
		{
			LCD_displayCharacter(' ');
		}

		// Delay for a short period of time
		_delay_ms(100);
	}
}
