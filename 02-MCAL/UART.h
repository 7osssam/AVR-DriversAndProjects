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
void UART_init(uint32 a_baud_rate);

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