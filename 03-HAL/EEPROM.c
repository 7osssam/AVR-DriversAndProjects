/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: EEPROM.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "EEPROM.h"

ErrorStatus_t EEPROM_writeByte(uint16 u16address, uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(EEPROM_ADDRESS | ((u16address & 0x0700) >> 7) | WRITEMODE));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16address));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte to eeprom */
	TWI_writeByte(u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

ErrorStatus_t EEPROM_readByte(uint16 u16address, uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(EEPROM_ADDRESS | ((u16address & 0x0700) >> 7) | WRITEMODE));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16address));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8)(EEPROM_ADDRESS | ((u16address & 0x0700) >> 7) | READMODE));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

ErrorStatus_t EEPROM_writePage(uint16 u16address, uint8 *u8data, uint8 u8length)
{
	uint8 i;
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;
	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(EEPROM_ADDRESS | ((u16address & 0x0700) >> 7) | WRITEMODE));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16address));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/* write byte to eeprom */
	for (i = 0; i < u8length; i++)
	{
		TWI_writeByte(u8data[i]);
		if (TWI_getStatus() != TWI_MT_DATA_ACK)
			return ERROR;
	}
	/* Send the Stop Bit */
	TWI_stop();
	return SUCCESS;
}

ErrorStatus_t EEPROM_readPage(uint16 u16address, uint8 *u8data, uint8 u8length)
{
	uint8 i;
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;
	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8)(EEPROM_ADDRESS | ((u16address & 0x0700) >> 7) | WRITEMODE));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16address));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;
	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8)((0xA0) | ((u16address & 0x0700) >> 7) | READMODE));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;
	/* Read Byte from Memory without send ACK */
	for (i = 0; i < u8length; i++)
	{
		u8data[i] = TWI_readByteWithACK();
		if (TWI_getStatus() != TWI_MR_DATA_ACK)
			return ERROR;
	}
	/* Send the Stop Bit */
	TWI_stop();
	return SUCCESS;
}
