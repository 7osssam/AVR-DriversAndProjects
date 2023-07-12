/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART_Services.c
 *
 * Description: Source file for the UART Services driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#include "UART_Services.h"

static uint8 *Send_Str = NULL_PTR;
static uint8 *Receive_str = NULL_PTR;

/*************************************************************************************************************
 *   										Functions Definitions										 	 *
 *************************************************************************************************************/

/**********************************************************************************************
 * 										 	Send Functions									  *
 **********************************************************************************************/
void UART_SendString(const uint8 *Str)
{
	uint8 i = 0;
	while (Str[i] != '\0')
	{
		UART_SendByte(Str[i]);
		i++;
	}
}

void UART_SendString_interrupt(uint8 *str)
{
	// save the string in global pointer
	Send_Str = str;

	// send first byte
	UART_SendByteNoBlock(str[0]);

	// Set Call Back function for TXC interrupt
	UART_TX_SetCallBack(TX_INT);

	// Enable Transmit complete interrupt
	UART_TX_InterruptEnable();
}

/*************************** ISR for UART TX Complete Interrupt ***************************/
static void TX_INT(void)
{
	static uint8 i = 1;
	if (Send_Str[i] != '\0')
	{
		UART_SendByteNoBlock(Send_Str[i]);
		i++;
	}
	else
	{
		i = 1;
		UART_TX_InterruptDisable();
	}
}

/**********************************************************************************************
 * 										 	Receive Functions								  *
 **********************************************************************************************/

void UART_ReceiveString(uint8 *Str) // receive until '#'
{
	uint8 i = 0;
	do
	{
		Str[i] = UART_ReceiveByte();
		i++;
	} while (Str[i] != '#');
	Str[i] = '\0';
	//=========================  Another Method  ==============================
	// Str[i] = UART_ReceiveByte();
	// while (Str[i] != 0)
	//{
	//	i++;
	//	Str[i] = UART_ReceiveByte();
	// }
	// Str[i] = '\0';
}

void UART_ReceiveString_interrupt(uint8 *str)
{
	// save the string in global pointer
	Receive_str = str;

	// Set Call Back function for RXC interrupt
	UART_RX_SetCallBack(RX_INT);

	// Enable Receive complete interrupt
	UART_RX_InterruptEnable();
}

/*************************** ISR for UART RX Complete Interrupt ***************************/
static void RX_INT(void) // receive until '#'
{
	static uint8 i = 0;
	Receive_str[i] = UART_ReceiveByteNoBlock();
	if (Receive_str[i] == '#')
	{
		Receive_str[i] = '\0';
		i = 0;
		UART_RX_InterruptDisable();
	}
	else
	{
		i++;
	}
}
