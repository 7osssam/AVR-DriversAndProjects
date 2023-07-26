/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: SPI.h
 *
 * Description: Header file for the SPI AVR driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include "STD_TYPES.h"

/*******************************************************************************
 *                    Module Data Types                                        *
 * *****************************************************************************/

typedef enum
{
	SPI_Master,
	SPI_Slave
} Master_Slave_Select_t;

typedef enum
{
	SPI_FOSC_4,
	SPI_FOSC_16,
	SPI_FOSC_64,
	SPI_FOSC_128,
} Clock_Rate_t;

/*******************************************************************************
 *                    Functions Prototypes                                     *
 *******************************************************************************/

/*
 * Description : initialize SPI driver with specific configurations:
 * 			 	 - Data Order.
 * 			 	 - Master/Slave Select.
 * 			 	 - Clock Polarity.
 * 			 	 - Clock Phase.
 * 			 	 - SPI State.
 * 			 	 - SPI Double Speed.
 * arguments   : Master_Slave_Select_t Master_Slave_Select : Master/Slave Select
 * 				 Clock_Rate_t Clock_Rate : Clock Rate
 * Return  : None
 * Note : you should call this function before using the SPI driver
 */
void SPI_init(Master_Slave_Select_t Master_Slave_Select, Clock_Rate_t Clock_Rate);

/*
 * Description : Send and Receive byte (at the same time) from another SPI device.
 * arguments   : uint8 a_data : byte to be sent
 * Return  : uint8 : received byte
 */
uint8 SPI_sendReceiveByte(uint8 a_data);

/*
 * Description : Send byte to another SPI device.
 * arguments   : uint8 a_data : byte to be sent
 * Return  : None
 * Note : this function is blocking function until send byte - busy waiting polling method -
 */
void SPI_SendByte(uint8 a_data);

/*
 * Description : Receive byte from another SPI device.
 * arguments   : None
 * Return  : uint8 : received byte
 * Note : this function is blocking function until receive byte - busy waiting polling method -
 */
uint8 SPI_ReceiveByte(void);

/*
 * Description : Receive byte from another SPI device using periodic check on the receive flag.
 * arguments   : uint8 *ptr_data : pointer to the variable which will store the received byte
 * Return : uint8 : status of the function [TRUE, FALSE]
 * Note : this function is non-blocking function - using periodic polling method -
 */
uint8 SPI_ReceiveByteCheck(uint8 *ptr_data);

/**************************************** Interrupt Enable/Disable ********************************************/
void SPI_EnableInterrupt(void);
void SPI_DisableInterrupt(void);

/***************************************** Call Back Function ***********************************************/
void SPI_SetCallBack(void (*LocalFptr)(void));

/********************************* Send and receive functions with no ckecking - for interrupt - *******************/
void SPI_SendByteNoBlock(uint8 a_data);
uint8 SPI_ReceiveByteNoBlock(void);
#endif /* SPI_H_ */