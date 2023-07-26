/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART_Services.h
 *
 * Description: Header file for the UART Services driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#ifndef UART_SERVICES_H_
#define UART_SERVICES_H_

#include "STD_TYPES.h"
#include "UART.h"

/*************************************************************************************************************
 *   									      Static configurations										 	 *
 * ***********************************************************************************************************/
#define STACK_SIZE 10

/*************************************************************************************************************
 *   										User defined data types										 	 *
 * ***********************************************************************************************************/
typedef enum
{
	STACK_EMPTY, // 0
	STACK_FULL,	 // 1
	DONE		 // 2
} Stack_Status_Type;

/*************************************************************************************************************
 *   										Functions Prototypes										 	 *
 * ***********************************************************************************************************/

/*
 * Description : Push the required data to the stack.
 * arguments   : uint8 a_data : data to be pushed
 * Return      : Stack_Status_Type : status of the stack
 */
Stack_Status_Type UART_PUSH(uint8 a_data);

/*
 * Description : Pop the required data from the stack.
 * arguments   : uint8 *Ptr_data : pointer to the variable to store the popped data
 * Return      : Stack_Status_Type : status of the stack
 */
Stack_Status_Type UART_POP(uint8 *Ptr_data);

/*
 * Description : Send the required string through UART to the other UART device.
 * arguments   : const uint8 *Str : pointer to the string to be sent
 * Return      : None
 * Note        : The string should be ended with '#' to be received correctly
 */
void UART_SendString(const uint8 *Str);

/*
 * Description : Send the required string through UART to the other UART device using interrupt.
 * arguments   : uint8 *Str : pointer to the string to be sent
 * Return      : None
 * Note        : The string should be ended with '#' to be received correctly
 */
void UART_SendString_interrupt(uint8 *str);

/*
 * Description : Receive the required string until the '#' symbol through UART from the other UART device.
 * arguments   : uint8 *Str : pointer to the string to store the received string
 * Return      : None
 * Note        : The string should be ended with '#' to be received correctly
 */
void UART_ReceiveString(uint8 *Str); // Receive until #

/*
 * Description : Receive the required string until the '#' symbol through UART from the other UART device using interrupt.
 * arguments   : uint8 *Str : pointer to the string to store the received string
 * Return      : None
 * Note        : The string should be ended with '#' to be received correctly
 */
void UART_ReceiveString_interrupt(uint8 *str);
#endif /* UART_SERVICES_H_ */