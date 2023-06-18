#include "SETTINGS.h"
#include <util/delay.h>

#include "KEYPAD.h"
#include "LCD.h"

/*
 * Description: This function is responsible for displaying the main menu on the LCD
 * and wait for the user to press 1 or 2 to choose the calculator mode (Normal or Programmer)
 * Inputs: void
 * Output: void
 */
void menu(void);

/*
 * Description: This function is responsible for the normal mode of the calculator (+ - * /)
 * Inputs: void
 * Output: void
 */
void NormalMode(void);

/*
 * Description: This function is responsible for the programmer mode of the calculator (Binary and Hex)
 * Inputs: void
 * Output: void
 */
void ProgrammerMode(void);