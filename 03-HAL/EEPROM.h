/******************************************************************************
 *
 * Module: EEPROM
 *
 * File Name: EEPROM.h
 *
 * Description: Header file for the External EEPROM driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "STD_TYPES.h"
#include "TWI.h"

#define EEPROM_ADDRESS 		(0xA0)
#define WRITEMODE 			(0x00)
#define READMODE 			(0x01)
/*******************************************************************************
 *                      User Defined Types                                     *
 *******************************************************************************/
typedef enum
{
	ERROR,
	SUCCESS
} ErrorStatus_t;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to write a byte in the external EEPROM
 * Input       : - u16address -> the address of the location to write in
 * 				 - u8data -> the data to write
 * Output      : ErrorStatus_t
 */
ErrorStatus_t EEPROM_writeByte(uint16 u16address, uint8 u8data);

/*
 * Description : Function to read a byte from the external EEPROM
 * Input       : - u16address -> the address of the location to read from
 * 				 - u8data -> pointer to the variable that will hold the read data
 * Output      : ErrorStatus_t
 */
ErrorStatus_t EEPROM_readByte(uint16 u16address, uint8 *u8data);

/*
 * Description : Function to write a page in the external EEPROM
 * Input       : - u16address -> the address of the location to write in
 * 				 - u8data -> pointer to the data to write
 * 				 - u8length -> the length of the data to write
 * Output      : ErrorStatus_t
 */
ErrorStatus_t EEPROM_writePage(uint16 u16address, uint8 *u8data, uint8 u8length);

/*
 * Description : Function to read a page from the external EEPROM
 * Input       : - u16address -> the address of the location to read from
 * 				 - u8data -> pointer to the variable that will hold the read data
 * 				 - u8length -> the length of the data to read
 * Output      : ErrorStatus_t
 */
ErrorStatus_t EEPROM_readPage(uint16 u16address, uint8 *u8data, uint8 u8length);
#endif // _EEPROM_H_
