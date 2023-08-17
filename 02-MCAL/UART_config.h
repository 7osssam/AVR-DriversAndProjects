/******************************************************************************
 *
 * Module: UART
 *
 * File Name: UART_config.h
 *
 * Description: Static configuration file for the UART driver for ATmega32
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#ifndef UART_config_H_
#define UART_config_H_
#include "SETTINGS.h" // for F_CPU

/******************* UART configuration *********************************/
#define SPEED_MODE 			DOUBLE_SPEED
//#define CPU_F 				_16_MHZ		// not used
//#define BUAD_RATE 			BAUD_9600 	// not used
#define SYNCH_MODE 			ASYNCH
//#define PARITY_MODE 		EVEN_PARITY // not used
//#define N_DATA_BITS 		_8_DATA_BITS // not used
//#define N_STOP_BITS 		ONE_STOP_BIT // not used

/******************* Interrupt configuration *********************************/
#define MODE 				POLLING

#define POLLING 				0
#define INTERRUPT 				1

/******************** speed mode *************************/
#define NORMAL_SPEED 			0
#define DOUBLE_SPEED 			1
#define TOTAL_SPEED_MODE 		2 // unused

///*******************parity mode*************************/
//#define NO_PARITY 				0
//#define EVEN_PARITY 			1
//#define ODD_PARITY 				2

///*************** number of data bits********************/
//#define _5_DATA_BITS 			5
//#define _6_DATA_BITS 			6
//#define _7_DATA_BITS 			7
//#define _8_DATA_BITS 			8
//#define _9_DATA_BITS 			9

/*************** Communication mode********************/
#define SYNCH 					0
#define ASYNCH					1

///************ Number of stop bits**********************/
//#define ONE_STOP_BIT 			1
//#define TWO_STOP_BIT 			2

#endif /* UART_config_H_ */