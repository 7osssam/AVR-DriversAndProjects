#include "Fan_Controller_system.h"

ADC_ConfigType ADC_Config = {ADC_INTERNAL, ADC_PRESCALER_8};
uint8 temp = 0;

void Fan_Controller_system(void)
{
	/* INITIALIZATION */

	LCD_init();
	ADC_init(&ADC_Config);
	DCMOTOR_Init();

	LCD_displayStringRowColumn(1, 2, "Temp =    C");

	while (1)
	{
		temp = LM35_Read();

		if (temp < 30)
		{
			DCMOTOR_Rotate(DCMOTOR_STOP, 0);
			LCD_displayStringCenter(0, "FAN OFF");
		}
		else
		{
			LCD_displayStringCenter(0, "FAN ON ");

			if ((temp >= 30) && (temp < 60))
			{
				DCMOTOR_Rotate(DCMOTOR_CLOCKWISE, 25);
			}
			else if ((temp >= 60) && (temp < 90))
			{
				DCMOTOR_Rotate(DCMOTOR_CLOCKWISE, 50);
			}
			else if ((temp >= 90) && (temp < 120))
			{
				DCMOTOR_Rotate(DCMOTOR_CLOCKWISE, 75);
			}
			else if (temp >= 120)
			{
				DCMOTOR_Rotate(DCMOTOR_CLOCKWISE, 100);
			}
		}

		if (temp < 100)
		{
			LCD_Goto_XY(1, 9);
			LCD_displayInteger(temp);
			LCD_displayCharacter(' ');
		}
		else
		{
			LCD_Goto_XY(1, 9);
			LCD_displayInteger(temp);
		}
	}
}