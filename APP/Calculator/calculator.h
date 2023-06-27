#include "KEYPAD.h"
#include "LCD.h"
#include "SETTINGS.h"
#include "STD_TYPES.h"

#include <util/delay.h>

/*
 * Description: This function is responsible for displaying the main menu on the LCD
 * and wait for the user to press 1 or 2 to choose the calculator mode (Normal or Programmer)
 * Inputs: void
 * Output: void
 */
void CalculatorMenu(void);