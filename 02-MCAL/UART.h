/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART.h
 *
 * Description: Header file for the UART driver for ATmega32
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef UART__H_
#define UART__H_

#include "STD_TYPES.h"

typedef enum
{
	UART_NO_PARITY,
	UART_PARITY_EVEN = 2,
	UART_PARITY_ODD = 3
} UART_Parity;

typedef enum
{
	BAUD_2400 = 2400,
	BAUD_4800 = 4800,
	BAUD_9600 = 9600,
	BAUD_14400 = 14400,
	BAUD_19200 = 19200,
	BAUD_28800 = 28800,
	BAUD_115200 = 115200
} UART_BaudRate;
typedef enum
{
	UART_1_STOP_BIT,
	UART_2_STOP_BIT
} UART_StopBit;
typedef enum
{
	UART_5_BIT_DATA, // 0
	UART_6_BIT_DATA, // 1
	UART_7_BIT_DATA, // 2
	UART_8_BIT_DATA, // 3
	UART_9_BIT_DATA	 // 7
} UART_BitData;
typedef struct
{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
} UART_ConfigType;

/*
 * Description : initialize UART driver with specific baud rate and other static configurations:
 * 			 	 - number of data bits.
 * 			 	 - number of stop bits.
 * 			 	 - parity mode.
 * 			 	 - communication mode.
 * 			 	 - speed mode.
 * arguments   : uint32 a_baud_rate : baud rate of the UART communication
 * Return  : None
 */
void UART_init(UART_ConfigType *a_config_ptr);

/*
 * Description : Functional responsible for send byte to another UART device.
 * arguments   : uint8 a_data : byte to be sent
 * Return  : None
 */
void UART_SendByte(uint8 a_data);

/*
 * Description : Functional responsible for receive byte from another UART device.
 * arguments   : None
 * Return  : uint8 : received byte
 * Note : this function is blocking function until receive byte - busy waiting polling method -
 */
uint8 UART_ReceiveByte(void);

/*
 * Description : Receive byte from another UART device using periodic check on the receive flag.
 * arguments   : uint8 *ptr_data : pointer to the variable which will store the received byte
 * Return : uint8 : status of the function [TRUE, FALSE]
 * Note : this function is non-blocking function - using periodic polling method -
 */
uint8 UART_ReceiveByteCheck(uint8 *ptr_data);

/**************************************** Interrupt Enable/Disable ********************************************/
void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);
void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);

/***************************************** Call Back Functions ***********************************************/
void UART_RX_SetCallBack(void (*LocalFptr)(void));
void UART_TX_SetCallBack(void (*LocalFptr)(void));

/***************************************** Send/Receive No Block - for interrupt -****************************/
void UART_SendByteNoBlock(uint8 a_data);
uint8 UART_ReceiveByteNoBlock(void);

#endif /* UART_H_ */