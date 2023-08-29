/******************************************************************************
 * @file   : HMI_MCU.c
 * @brief  : This file contains the firmware for the Human Machine Interface (HMI) microcontroller.
 *           It implements the user interface for controlling a door locking system using an ATMEGA32 MCU (AVR).
 * @author : Hossam Mohamed
 * @date   : 20/8/2023
 * @version: v1.0
 * @Target : ATMEGA32 MCU (AVR)
 *******************************************************************************/

#include "HMI_MCU.h"

#include "SETTINGS.h"
#include "STD_TYPES.h"
#include "System_config.h"

#include "TIMER.h"
#include "UART_Services.h"

#include "KEYPAD.h"
#include "LCD.h"

#include <avr/interrupt.h> // for sei() function
#include <util/delay.h>	   // for _delay_ms() function

//======================================== Global Variables =====================================

volatile static DoorState_t DoorState = IDLE;

volatile static uint8 Timer1_1sec_counter = 0;
volatile static uint8 Timer1_3sec_counter = 0;

volatile static uint8 TimerFlag = FALSE;

volatile static uint8 UART_INT_ReceivedData = 0;

uint32 FirstPassword = 0;

//================================ Global Configurations Types ===================================
UART_ConfigType UART_HMI_Config = {UART_8_BIT_DATA, UART_1_STOP_BIT, UART_NO_PARITY, BAUD_9600};

// when F_CPU = 8MHz and prescaler = 1024 => 1 tick = 128 us => 1 sec = 7812 ticks
// we don't care here much about the number of interrupts as the system is locked (more accurate progress bar)
Timer1_ConfigType Timer1_syslock_config = {0, 7812U, TIMER1_CTC_OCR1A_MODE, F_CPU_1024, OCRA_DISCONNECTED, OCRB_DISCONNECTED};

// when F_CPU = 8MHz and prescaler = 1024 => 1 tick = 128 us => 3 sec = 23436 ticks
// for less interrupts as the maximum time we want to calculate is 3 seconds (less accurate progress bar)
Timer1_ConfigType static Timer1_Door_config = {0, 23436U, TIMER1_CTC_OCR1A_MODE, F_CPU_1024, OCRA_DISCONNECTED, OCRB_DISCONNECTED};

// ================================ Extra service functions =====================================
void PogressBar_init()
{
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

	LCD_displaySpecialCharacter(Pattern1, 0);
	LCD_displaySpecialCharacter(Pattern2, 1);
	LCD_displaySpecialCharacter(Pattern3, 2);
	LCD_displaySpecialCharacter(Pattern4, 3);
	LCD_displaySpecialCharacter(Pattern5, 4);
}

void ProgressBar(uint8 MaximumValue, uint8 CurrentValue)
{
	uint8 i;
	static uint8 LastPattern = 0; // to keep track of the last pattern displayed

	float32 percent = ((float32)CurrentValue / (float32)MaximumValue) * 100.0;
	float32 bar_width = ((float32)LCD_NUMBER_OF_CHARACTERS / 100.0) * percent;

	if (bar_width >= 1)
	{
		for (i = 1; i < bar_width; i++)
		{
			LCD_displayCharacter(4); // the location of the full 5-bits bar
		}

		bar_width = bar_width - (i - 1); // to get the remaining width of the bar
	}

	LastPattern = bar_width * 5; // multiplying by 5 to get the location of the last pattern

	switch (LastPattern)
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
	}

	// Clearing the remaining line of the bar when decreasing the percentage
	for (uint8 j = LastPattern; j < 5; j++)
	{
		LCD_displayCharacter(' ');
	}

	if (percent == 100)
	{
		LastPattern = 0;
	}
}

//======================================== ISRs =================================================
static void TIMER1_ISR()
{
	// after 60 seconds (for the system locked screen)
	if (Timer1_1sec_counter == LOCKED_SCREEN_TIME)
	{
		TimerFlag = TRUE;
		Timer1_1sec_counter = 0;
		Timer1_3sec_counter = 0;
		Timer1_OCA_InterruptDisable();
	}
	// after 33 seconds (for progress bar of the door operation)
	if (Timer1_3sec_counter >= (OPEN_CLOSE_DOOR_TIME + WAITING_DOOR_TIME + OPEN_CLOSE_DOOR_TIME))
	{
		Timer1_3sec_counter = 0;
	}

	// 1 second counter
	Timer1_1sec_counter++;

	// 3 seconds counter
	Timer1_3sec_counter += 3;
}

static void UART_RECEIVE_ISR()
{
	UART_INT_ReceivedData = UART_ReceiveByteNoBlock();
}

//====================================== Lock System Functions ==================================
void SystemLocked_HMI()
{
	LCD_clearScreen();
	Timer1_OCA_InterruptEnable();
	Timer1_init(&Timer1_syslock_config);
	Timer1_1sec_counter = 0;

	LCD_displayStringCenter(0, "SYSTEM LOCKED");
	_delay_ms(LCD_WAITING_TIME / 2);
	LCD_clearScreen();
	LCD_displayString("UNLOCKED IN");

	while (TimerFlag != TRUE)
	{
		LCD_Goto_XY(0, 12);
		LCD_displayInteger(LOCKED_SCREEN_TIME - Timer1_1sec_counter);
		LCD_displayString("s ");

		LCD_Goto_XY(1, 0);

		ProgressBar(LOCKED_SCREEN_TIME, Timer1_1sec_counter);

		// Delay for a short period of time
		//_delay_ms(100);
	}

	TimerFlag = FALSE; // reset the flag

	LCD_clearScreen();
}

//================================ Password Operation Functions ==================================
uint32 GetPassword()
{
	LCD_Goto_XY(1, 0);

	uint32 Password = 0;
	uint8 PressedKey = -1; // to store the pressed key number (initially = -1 to avoid invalid input)
	uint8 InputLength = PASSWORD_LENGTH;

	while (InputLength > 0)
	{

		LCD_sendCommand(LCD_CURSOR_ON);
		PressedKey = KEYPAD_getPressedKey();
		LCD_sendCommand(LCD_CURSOR_OFF);

		if (PressedKey >= 0 && PressedKey <= 9) // Handle the number keys press
		{
			Password = Password * 10 + PressedKey;

			LCD_displayInteger(PressedKey);

			_delay_ms(300); // Display the number for 300ms

			// Display '*' instead of the number
			LCD_Goto_XY(1, PASSWORD_LENGTH - InputLength); // Go to the next digit
			LCD_displayCharacter('*');

			InputLength--; // Decrement the allowed number of digits of the password
		}
		else if (PressedKey == 'C') // Handle the 'C' key press
		{
			Password /= 10;								   // Remove the last entered digit
			InputLength++;								   // Increment the allowed number of digits of the password
			LCD_Goto_XY(1, PASSWORD_LENGTH - InputLength); // Go back to the removed digit position
			LCD_displayCharacter(' ');					   // Display a space to clear the removed digit
			LCD_Goto_XY(1, PASSWORD_LENGTH - InputLength); // Go back to the removed digit position
		}
		else if (PressedKey == '=')
		{
			_delay_ms(LCD_WAITING_TIME / 2); // Wait for 600ms to fix the skipping of the next screen
			break;
		}

		_delay_ms(KEYPAD_PRESS_TIME); // delay for 100ms to avoid debouncing
	}
	return Password;
}

boolean EnterPassword()
{
	uint32 RepeatPassword = 0;

	LCD_clearScreen();
	LCD_displayStringCenter(0, "ENTER PASSWORD:");
	FirstPassword = GetPassword();

	LCD_clearScreen();

	LCD_displayStringCenter(0, "REPEAT PASSWORD");
	RepeatPassword = GetPassword();

	if (FirstPassword == RepeatPassword)
	{
		LCD_clearScreen();
		LCD_displayStringCenter(0, "PASSWORD SET :)");

		_delay_ms(LCD_WAITING_TIME);
		return TRUE;
	}
	else
	{
		LCD_clearScreen();
		LCD_displayStringCenter(0, "PASSWORDS");
		LCD_displayStringCenter(1, "DO NOT MATCH :(");

		_delay_ms(LCD_WAITING_TIME);
		return FALSE;
	}
}

boolean CreatePassword()
{
	uint8 PasswordsAreEqual = FALSE;
	uint8 WrongPasswordCounter = 0;

	LCD_clearScreen();
	LCD_displayStringCenter(0, "NEW USER");
	LCD_displayStringCenter(1, "CREATE PASS");
	_delay_ms(LCD_WAITING_TIME);

	while (PasswordsAreEqual == FALSE)
	{
		PasswordsAreEqual = EnterPassword();
		if (PasswordsAreEqual == TRUE)
		{
			_delay_ms(100);

			UART_SendByte(UART_OPERATION_SUCCESS); // send success signal to CONTROL MCU to be ready to receive the password

			UART_SendFourBytes(FirstPassword);

			return TRUE;
		}
		else
		{
			WrongPasswordCounter++;

			if (WrongPasswordCounter == MAXIMUM_WRONG_PASSWORDS)
			{

				UART_SendByte(UART_MAX_WRONG_PASSWORD);

				SystemLocked_HMI();

				// signal to CONTROL to stop the buzzer
				UART_SendByte(UART_OPERATION_SUCCESS);

				WrongPasswordCounter = 0;
				return FALSE;
			}
		}
	}
}

boolean CheckOldPassword_int()
{
	uint32 OldPassword;

	uint8 WrongPasswordCounter = 0;

	UART_RX_InterruptEnable();

	while (1)
	{
		UART_INT_ReceivedData = 0;
		LCD_clearScreen();
		LCD_displayStringCenter(0, "ENTER OLD PASS");
		OldPassword = GetPassword();

		// first UART_CHANGE_PASSWORD signal
		UART_SendByte(UART_CHANGE_PASSWORD); // send signal to CONTROL to start receiving the password
		//=================== Wait for CTRL Ready ==============
		while (UART_INT_ReceivedData != UART_CHANGE_PASSWORD)
		{
		}
		//_delay_ms(50); //!!!

		UART_SendFourBytes(OldPassword);
		_delay_ms(50); // delay to make sure that the CONTROL MCU received the password //!!!

		if (UART_INT_ReceivedData == UART_OPERATION_SUCCESS)
		{
			UART_RX_InterruptDisable();
			return TRUE;
		}
		else if (UART_INT_ReceivedData == UART_OPERATION_FAIL)
		{
			LCD_clearScreen();
			LCD_displayStringCenter(0, "WRONG PASS");
			LCD_displayStringCenter(1, "TRY AGAIN");

			_delay_ms(LCD_WAITING_TIME);

			WrongPasswordCounter++;

			if (WrongPasswordCounter == MAXIMUM_WRONG_PASSWORDS)
			{
				UART_SendByte(UART_MAX_WRONG_PASSWORD);

				SystemLocked_HMI(); // interrupt will be disabled inside this function

				// signal to CONTROL to stop the buzzer
				UART_SendByte(UART_OPERATION_SUCCESS);

				WrongPasswordCounter = 0;
				return FALSE;
			}
			else
			{
				// signal indicating that we didn't exceed the maximum wrong passwords yet
				UART_SendByte(UART_OPERATION_SUCCESS);
			}
		}
	}
}

//=================================== System Options Functions ==================================
void DoorOperation_HMI()
{
	Timer1_init(&Timer1_Door_config);

	//=================== Send Ready to Control MCU =========
	UART_SendByte(UART_HMI_READY);
	//=======================================================
	Timer1_OCA_InterruptEnable();
	UART_RX_InterruptEnable();

	while (UART_INT_ReceivedData != UART_OPERATION_SUCCESS)
	{

		if (UART_INT_ReceivedData == UART_OPEN_DOOR)
		{
			LCD_clearScreen();
			LCD_displayStringCenter(0, "OPENING DOOR =>");
			DoorState = OPEN_DOOR;
			UART_INT_ReceivedData = 0; // Clear the received data
		}
		else if (UART_INT_ReceivedData == UART_DOOR_WAITING)
		{
			LCD_clearScreen();
			LCD_displayStringCenter(0, "   WAITING...  ");
			DoorState = DOOR_WAITING;
			UART_INT_ReceivedData = 0; // Clear the received data
		}
		else if (UART_INT_ReceivedData == UART_CLOSE_DOOR)
		{
			LCD_clearScreen();
			LCD_displayStringCenter(0, "<= CLOSING DOOR");
			DoorState = CLOSE_DOOR;
			UART_INT_ReceivedData = 0; // Clear the received data
		}

		LCD_Goto_XY(1, 0);
		if (DoorState == OPEN_DOOR)
		{
			ProgressBar(OPEN_CLOSE_DOOR_TIME, Timer1_3sec_counter); // Display the progress bar
		}
		else if (DoorState == CLOSE_DOOR)
		{
			ProgressBar(OPEN_CLOSE_DOOR_TIME, Timer1_3sec_counter - (OPEN_CLOSE_DOOR_TIME + WAITING_DOOR_TIME)); // Display the progress bar
		}
	}
	UART_INT_ReceivedData = 0; // Clear the received data
	DoorState = IDLE;

	UART_RX_InterruptDisable();
	Timer1_OCA_InterruptDisable();
	LCD_clearScreen();
}

void ChangePasswordOperation_HMI()
{
	uint8 dummyUserCounter = 0;

	uint8 isOldPasswordCorrect;

	isOldPasswordCorrect = CheckOldPassword_int();

	if (isOldPasswordCorrect == TRUE) ///!!!
	{
		LCD_clearScreen();
		LCD_displayStringCenter(0, "CORRECT :)");
		_delay_ms(LCD_WAITING_TIME);
	}
	else
	{
		// at this point, user returned with (FALSE) from CheckOldPassword_int() function for exceeding the maximum wrong passwords
		// He will exit this function and return to system options function to display the main menu again
		return;
	}

	// if the user didn't return from the previous if statement, then he entered the correct password
	// and he can change the password now

	while (EnterPassword() == FALSE) // infinite loop until the user enters both passwords correctly
	{
		dummyUserCounter++;
		LCD_clearScreen();
		LCD_displayStringCenter(0, "TRY AGAIN");

		if (dummyUserCounter == MAXIMUM_WRONG_REPEATED_PASSWORDS)
		{
			UART_SendByte(UART_OPERATION_FAIL);

			LCD_clearScreen();
			LCD_displayStringCenter(0, "I DON'T THINK U");
			LCD_displayStringCenter(1, "HAV GOOD MEMORY");
			_delay_ms(LCD_WAITING_TIME);
			return;
		}
		_delay_ms(LCD_WAITING_TIME);
	}

	// second UART_OPERATION_SUCCESS signal
	UART_SendByte(UART_OPERATION_SUCCESS);
	_delay_ms(50);

	UART_SendFourBytes(FirstPassword);

	LCD_clearScreen();
	LCD_displayStringCenter(0, "DONE");
	LCD_displayStringCenter(1, "PASS CHANGED :)");
	_delay_ms(LCD_WAITING_TIME);
}

//==================================== System Functions =========================================
void System_init_HMI()
{
	LCD_init();
	PogressBar_init();
	UART_init(&UART_HMI_Config);

	UART_RX_SetCallBack(UART_RECEIVE_ISR);
	Timer1_OCA_SetCallBack(TIMER1_ISR);

	sei(); // enable global interrupts in MC.
}

void SystemOptions_HMI()
{
	uint8 option; /* to store the pressed key number */

	LCD_displayStringRowColumn(0, 0, "+ : OPEN DOOR");
	LCD_displayStringRowColumn(1, 0, "- : CHANGE PASS");

	option = KEYPAD_getPressedKey(); /* get the pressed key number */
	_delay_ms(KEYPAD_PRESS_TIME);	 /* Press time */

	if (option == '-' || option == '+')
	{
		UART_SendByte(option);

		switch (option)
		{
		case '+':
			DoorOperation_HMI();
			break;

		case '-':
			ChangePasswordOperation_HMI();
			break;
		}
	}
	else
	{
		LCD_clearScreen();
		LCD_displayStringCenter(0, "INVALID OPTION");
		_delay_ms(LCD_WAITING_TIME);
	}
}

//========================================== Main ==============================================
void HMI_MCU(void)
{
	//=================== System Initialization ==============
	System_init_HMI();

	LCD_displayStringCenter(0, "DOOR LOCKER");
	_delay_ms(LCD_WAITING_TIME);

	//=================== Send Ready to Control MCU =========
	UART_SendByte(UART_HMI_READY);

	switch (UART_ReceiveByte())
	{
	case UART_First_time:

		while (CreatePassword() != TRUE) // infinite loop until the user creates a password successfully
		{
		}

		LCD_clearScreen();
		LCD_displayStringCenter(0, "WELCOME");
		LCD_displayStringCenter(1, ":)");
		_delay_ms(LCD_WAITING_TIME);
		break;

	case UART_Not_First_time:

		LCD_clearScreen();
		LCD_displayStringCenter(0, "WELCOME");
		LCD_displayStringCenter(1, "AGAIN :)");
		_delay_ms(LCD_WAITING_TIME);

		//============================= Testing ================================
		// LCD_Goto_XY(1, 0);
		// UART_SendByte(UART_HMI_READY); // send ready signal to CONTROL
		// UART_ReceiveFourBytes(&pass);

		// LCD_displayInteger(pass);
		//_delay_ms(1000);
		break;
	}

	while (1)
	{
		LCD_clearScreen();
		SystemOptions_HMI();
	}
}
