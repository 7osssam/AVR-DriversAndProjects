/******************************************************************************
 * @file   : CTRL_ECU.c
 * @brief  : This file contains the firmware for the Control ECU (ATMEGA32 MCU).
 *           It implements the user interface for controlling a door locking system using an ATMEGA32 MCU (AVR).
 * @author : Hossam Mohamed
 * @date   : 20/8/2023
 * @version: v1.0
 * @Target : ATMEGA32 MCU (AVR)
 *******************************************************************************/

#include "CTRL_MCU.h"

#include "SETTINGS.h"
#include "STD_TYPES.h"
#include "System_config.h"

#include "Buzzer.h"
#include "DCMOTOR.h"
#include "EEPROM.h"

#include "TIMER.h"
#include "UART_Services.h"

#include <avr/interrupt.h> // for sei() function
#include <util/delay.h>	   // for _delay_ms() function

//======================================== EEPROM Addresses =====================================

	#define PASSWORD_ADDRESS 	0x0300	// 0x0300 - 0x0303 (4 bytes)
	#define isFirstTime_ADDRESS 0x0200  // 0x0200 (1 byte) 0x01 -> first time, 0x00 -> not first time

//======================================== Global Variables =====================================
volatile DoorState_t DoorState = IDLE;
volatile uint8 TimerFlag = FALSE;

volatile uint8 UARTflag = FALSE;
volatile uint8 UART_INT_ReceivedData = 0;

uint32 SavedPassword; // variable to save password from EEPROM

//================================ Global Configurations Types ===================================
UART_ConfigType UART_CONTROL_Config = {UART_8_BIT_DATA, UART_1_STOP_BIT, UART_NO_PARITY, BAUD_9600};

// when F_CPU = 8MHz and prescaler = 1024 => 1 tick = 128 us => 3 sec = 23436 ticks
// for less interrupts as the maximum time we want to calculate is 3 seconds
Timer1_ConfigType Timer1_Door_config = {0, 23436U, TIMER1_CTC_OCR1A_MODE, F_CPU_1024, OCRA_DISCONNECTED, OCRB_DISCONNECTED};

//============================================= ISRs ============================================
void Timer1_Motor_ISR()
{
	static uint8 Timer1_3sec_counter = 0;

	// every interrupt we add 3 seconds to the counter
	Timer1_3sec_counter += 3;

	//!!!!!
	// if (Timer1_3sec_counter == 0)
	//{
	//	DoorState = OPEN_DOOR;
	// }
	if (Timer1_3sec_counter == OPEN_CLOSE_DOOR_TIME)
	{
		DoorState = DOOR_WAITING;
	}
	else if (Timer1_3sec_counter == OPEN_CLOSE_DOOR_TIME + WAITING_DOOR_TIME)
	{
		DoorState = CLOSE_DOOR;
	}
	else if (Timer1_3sec_counter == (OPEN_CLOSE_DOOR_TIME + WAITING_DOOR_TIME + OPEN_CLOSE_DOOR_TIME))
	{ // Reset counter after 33 seconds (15 + 3 + 15)
		Timer1_3sec_counter = 0;
		TimerFlag = TRUE; // set flag to exit the loop
	}
}

void UART_RECEIVE_ISR()
{
	UART_INT_ReceivedData = UART_ReceiveByteNoBlock();
}

//====================================== Lock System Functions ==================================
void SystemLocked_CTRL()
{
	UART_RX_InterruptEnable();
	while (UART_INT_ReceivedData != UART_OPERATION_SUCCESS)
	{
		Buzzer_Alarm();
	}
	UART_INT_ReceivedData = 0;
	Buzzer_off();
	UART_RX_InterruptDisable();
}

//=================================== EEPROM services Functions =================================
void EEPROM_WritePassword(uint32 a_data)
{
	uint8 i;

	for (i = 0; i < 4; i++)
	{
		EEPROM_writeByte(PASSWORD_ADDRESS + i, (uint8)(a_data >> (i * 8)));
		_delay_ms(10);
	}
}

void EEPROM_ReadPassword(uint32 *a_data)
{
	uint8 i;
	*a_data = 0;
	for (i = 0; i < 4; i++)
	{
		uint8 byteValue;
		EEPROM_readByte(PASSWORD_ADDRESS + i, &byteValue);
		_delay_ms(10);

		*a_data |= ((uint32)byteValue << (i * 8));
	}
}

void EEPROM_resetPassword()
{
	uint8 i;
	for (i = 0; i < 4; i++)
	{
		EEPROM_writeByte(PASSWORD_ADDRESS + i, 0);
		_delay_ms(10);
	}
}

//=================================== System Options Functions =================================
void DoorOperation_CTRL()
{
	DoorState = OPEN_DOOR; // set initial state to open door

	Timer1_init(&Timer1_Door_config);
	Timer1_OCA_InterruptEnable();

	//=================== Wait for HMI Ready ==============
	while (UART_ReceiveByte() != UART_HMI_READY)
	{
		/* code */
	}
	//=======================================================

	while (TimerFlag == FALSE)
	{
		if (DoorState != IDLE)
		{
			switch (DoorState)
			{
			case OPEN_DOOR:
				UART_SendByte(UART_OPEN_DOOR);
				DCMOTOR_Rotate(DCMOTOR_CLOCKWISE, 100);
				DoorState = IDLE; // reset state
				break;
			case DOOR_WAITING:
				UART_SendByte(UART_DOOR_WAITING);
				DCMOTOR_Rotate(DCMOTOR_STOP, 0);
				DoorState = IDLE; // reset state
				break;
			case CLOSE_DOOR:
				UART_SendByte(UART_CLOSE_DOOR);
				DCMOTOR_Rotate(DCMOTOR_ANTI_CLOCKWISE, 100);
				DoorState = IDLE; // reset state
				break;
			}
		}
	}
	TimerFlag = FALSE; // reset flag

	UART_SendByte(UART_OPERATION_SUCCESS);
	DCMOTOR_Rotate(DCMOTOR_STOP, 0);
	Timer1_OCA_InterruptDisable();
}

void ChangePasswordOperation_CTRL()
{
	uint8 isPasswordCorrect = 0;
	uint32 OldPassword = 0;
	uint32 NewPassword = 0; // we can use the same variable for both old and new passwords but for more readability we use two variables

	uint8 HMI_Response = 0;
	while (isPasswordCorrect == 0)
	{
		//=================== Wait for HMI Ready ==============
		while (UART_ReceiveByte() != UART_CHANGE_PASSWORD) // first signal from HMI
		{
		}
		UART_SendByte(UART_CHANGE_PASSWORD); // send signal to HMI to start sending password
		//=======================================================
		//_delay_ms(50);
		UART_ReceiveFourBytes(&OldPassword);
		EEPROM_ReadPassword(&SavedPassword);

		if (OldPassword == SavedPassword)
		{
			UART_SendByte(UART_OPERATION_SUCCESS);
			isPasswordCorrect = 1; // set flag to exit the loop

			// when HMI return from CheckPassword

			HMI_Response = UART_ReceiveByte();

			if (HMI_Response == UART_OPERATION_SUCCESS) // second signal from HMI (user entered password twice correctly)
			{
				UART_ReceiveFourBytes(&NewPassword);
				EEPROM_resetPassword(); // reset password in EEPROM
				EEPROM_WritePassword(NewPassword);
			}
			else
			{
				return; // (MAXIMUM_WRONG_REPEATED_PASSWORDS) exit this function and return to main
			}
		}
		else
		{
			UART_SendByte(UART_OPERATION_FAIL); // when both passwords are not equal
			isPasswordCorrect = 0;				// set flag to continue the loop

			// reset password variables (not reseting variables can cause overwriting the new password with the old one)
			OldPassword = 0;
			SavedPassword = 0;

			if (UART_ReceiveByte() == UART_MAX_WRONG_PASSWORD)
			{
				SystemLocked_CTRL();
				return; // (MAXIMUM_WRONG_PASSWORDS) exit this function and return to main
			}
		}
	}
}

//==================================== System Functions =========================================
void System_init_CTRL()
{
	UART_init(&UART_CONTROL_Config);
	EEPROM_init();

	DCMOTOR_init();
	Buzzer_init();

	UART_RX_SetCallBack(UART_RECEIVE_ISR);
	Timer1_OCA_SetCallBack(Timer1_Motor_ISR);

	sei();
}

void SystemOptions_CTRL()
{
	uint8 option;

	option = UART_ReceiveByte();

	switch (option)
	{
	case '+':
		DoorOperation_CTRL();
		break;

	case '-':
		ChangePasswordOperation_CTRL();
		break;
	}
}

//========================================== Main ==============================================
void CTRL_MCU()
{
	//=================== System Initialization ==============
	System_init_CTRL();
	uint8 HMI_response = 0;
	uint8 isFirstTime = 0;

	//=================== Wait for HMI Ready ==============
	UART_SendByte(UART_CONTROL_READY);
	while (UART_ReceiveByte() != UART_HMI_READY)
	{
		/* Do nothing */
	}
	//=======================================================

	// get isFirstTime from EEPROM to check if it's the first time to run the system
	EEPROM_readByte(isFirstTime_ADDRESS, &isFirstTime);

	// if isFirstTime == 1 then it's not the first time to run the system
	if (isFirstTime == 1)
	{
		UART_SendByte(UART_Not_First_time);

		// /* ============================= Testing ================================ */
		// EEPROM_ReadPassword(&SavedPassword);
		//_delay_ms(10);
		// while (UART_ReceiveByte() != UART_HMI_READY)
		//{
		//	/* Wait for HMI Ready to send password */
		//}
		// UART_SendFourBytes(SavedPassword);
	}
	else
	{
		UART_SendByte(UART_First_time);

		_delay_ms(10);

		while (HMI_response != UART_OPERATION_SUCCESS)
		{
			// Wait for HMI response to act opon it
			HMI_response = UART_ReceiveByte();

			if (HMI_response == UART_OPERATION_SUCCESS) // if user create password successfully
			{
				UART_ReceiveFourBytes(&SavedPassword); // receive password from HMI and save it in SavedPassword variable

				EEPROM_WritePassword(SavedPassword); // write SavedPassword in EEPROM

				EEPROM_writeByte(isFirstTime_ADDRESS, 1); // set isFirstTime to 1
			}
			else if (HMI_response == UART_MAX_WRONG_PASSWORD) // if user fail to create password (exceed maximum wrong passwords)
			{
				SystemLocked_CTRL(); 
			}
		}
	}

	while (1)
	{
		SystemOptions_CTRL();
	}
}
