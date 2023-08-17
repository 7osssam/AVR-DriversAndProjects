/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART.c
 *
 * Description: Source file for the UART driver for ATmega32
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#include "UART.h"
#include "BIT_MACROS.h"
#include "UART_config.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "SETTINGS.h" // for F_CPU

#define BAUD_RATE_ASYNC_NORMAL(baud_rate) ((F_CPU / (16UL * (baud_rate))) - 1)
#define BAUD_RATE_ASYNC_DOUBLE(baud_rate) ((F_CPU / (8UL * (baud_rate))) - 1)
#define BAUD_RATE_SYNC_MASTER(baud_rate) ((F_CPU / (2UL * (baud_rate))) - 1)

/*************************** Pointer to functions to be assigned to ISR ********************************/
static void (*UART_RX_callBackPtr)(void) = NULL_PTR;
static void (*UART_TX_callBackPtr)(void) = NULL_PTR;

volatile uint8 receivedByte; // Global variable to store the received byte

/************************* Array of  UBRR values ***************************/

// const uint16 BaudRateArray[TOTAL_SPEED_MODE][TOTAL_CPU_F][TOTAL_BAUD_RATE] =
//	{{{103, 51, 25, 16, 12, 8, 0}, {207, 103, 51, 34, 25, 16, 3}, {416, 207, 103, 68, 51, 34, 8}},
//	 {{207, 103, 51, 34, 25, 16, 0}, {416, 207, 103, 68, 51, 34, 8}, {832, 416, 207, 138, 103, 68, 16}}};

/******************************************* initialization  *********************************************/
void UART_init(UART_ConfigType *a_config_ptr)
{

	volatile uint8 UCSRC_var = 0; // write UCSRC settings in one step then write it to UCSRC register
	uint16 UBRR_var = 0;

	//***************************** transmission speed ***************************/
#if (SPEED_MODE == NORMAL_SPEED)
	CLEAR_BIT(UCSRA, U2X);
#elif (SPEED_MODE == DOUBLE_SPEED)
	SET_BIT(UCSRA, U2X);
#endif

	//********************* Communication mode *******************************/
#if (SYNCH_MODE == SYNCH)
	SET_BIT(UCSRC_var, UMSEL)
#elif (SYNCH_MODE == ASYNCH)
	CLEAR_BIT(UCSRC_var, UMSEL);
#endif

	//**************************** Parity bits *******************************/
	UCSRC_var = (UCSRC_var & 0xCF) | ((a_config_ptr->parity) << 4);

	//*********************** Number of data bits *******************************/
	UCSRC_var = (UCSRC_var & 0xF9) | ((a_config_ptr->bit_data) << 1);
	if (a_config_ptr->bit_data == UART_9_BIT_DATA)
		SET_BIT(UCSRB, UCSZ2);

	//*********************** Number of stop bits *******************************/
	UCSRC_var = (UCSRC_var & 0xF7) | ((a_config_ptr->stop_bit) << 3);

	//****************** Set URSEL to access UCSRC register *********************/
	SET_BIT(UCSRC, URSEL);

	//***************** Write UCSRC register *******************************/
	UCSRC = UCSRC_var;

	//************************ Set baud rate *******************************/
#if (SYNCH_MODE == ASYNCH)
	#if (SPEED_MODE == NORMAL_SPEED)
	UBRR_var = BAUD_RATE_ASYNC_NORMAL(a_config_ptr->baud_rate)
	#elif (SPEED_MODE == DOUBLE_SPEED)
	UBRR_var = (uint16)BAUD_RATE_ASYNC_DOUBLE(a_config_ptr->baud_rate);
	#endif
#elif (SYNCH_MODE == SYNCH)
	UBRR_var = BAUD_RATE_SYNC_MASTER(a_config_ptr->baud_rate);
#endif

		UBRRH = (uint8)(UBRR_var >> 8);
	UBRRL = (uint8)UBRR_var;

	//************************ Enable receiver and transmitter *******************************/
	// enable UART  receiver.
	SET_BIT(UCSRB, RXEN);
	// enable UART  transmitter .
	SET_BIT(UCSRB, TXEN);
}

/**************************************** Interrupt Enable/Disable ********************************************/
void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB, RXCIE);
}

void UART_RX_InterruptDisable(void)
{
	CLEAR_BIT(UCSRB, RXCIE);
}

void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB, TXCIE);
}

void UART_TX_InterruptDisable(void)
{
	CLEAR_BIT(UCSRB, TXCIE);
}

/**************************************** Set Call Back Functions ********************************************/
void UART_RX_SetCallBack(void (*LocalFptr)(void))
{
	UART_RX_callBackPtr = LocalFptr;
}

void UART_TX_SetCallBack(void (*LocalFptr)(void))
{
	UART_TX_callBackPtr = LocalFptr;
}

/********************************************** ISR ************************************************************/
ISR(USART_RXC_vect) // ISR for receive complete interrupt
{
	if (UART_RX_callBackPtr != NULL_PTR)
	{
		UART_RX_callBackPtr();
	}
}

ISR(USART_TXC_vect) // ISR for transmit complete interrupt
{
	if (UART_TX_callBackPtr != NULL_PTR)
	{
		UART_TX_callBackPtr();
	}
}

/********************************** Send and receive functions with polling **************************************/
void UART_SendByte(uint8 a_data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready
	for transmitting a new byte so wait until this flag is set to one */
	while (IS_BIT_CLEAR(UCSRA, UDRE)) // Busy wait polling
		;
	/* the UDRE flag will be cleared by hardware when u write new data to buffer.*/
	UDR = a_data;

	// ================== Another Method ==================
	// UDR = a_data;
	// while (BIT_IS_CLEAR(UCSRA, TXC)) // Wait until the transmission is complete TXC = 1
	//	;
	// SET_BIT(UCSRA, TXC); // Clear the TXC flag
}

uint8 UART_ReceiveByte(void)
{
	/* RXC flag is set when the UART receive data so  wait until this flag is set to one
	and it will cleared by hardware when u read the data */
	while (IS_BIT_CLEAR(UCSRA, RXC)) // Busy wait polling
		;

	/************************* Error Checkings *************************
	 * FE: Frame Error (Stop Bit)
	 * DOR: Data OverRun
	 * PE: Parity Error
	 *******************************************************************/
	// if (IS_BIT_SET(UCSRA, FE) || IS_BIT_SET(UCSRA, DOR) || IS_BIT_SET(UCSRA, PE))
	//{
	//	return 0; // return 0 if there is error in receiving
	// }
	// else
	//{
	//	return UDR;
	// }
	return UDR;
}

uint8 UART_ReceiveByteCheck(uint8 *ptr_data)
{
	uint8 status = FALSE;
	if (IS_BIT_SET(UCSRA, RXC))
	{
		*ptr_data = UDR;
		status = TRUE;
	}
	return status;
}

/********************************* Send and receive functions with no ckecking - for interrupt - *******************/
void UART_SendByteNoBlock(uint8 a_data)
{
	UDR = a_data;
}

uint8 UART_ReceiveByteNoBlock(void)
{
	return UDR;
}
