/******************************************************************************
 *
 * Module: TWI (I2C)
 *
 * File Name: TWI.c
 *
 * Description: Source file for the TWI (Two Wire Interface) (I2C) AVR driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "TWI.h"
#include "BIT_MACROS.h"
#include "SETTINGS.h" // for F_CPU

#include <avr/io.h>

#define TWBR_VALUE(SCL_freq, TWPS_value) ((F_CPU / SCL_freq) - 16) / (2 * (4 ^ TWPS_value))

#define WRITEMODE (0x00)
#define READMODE (0x01)

void TWI_init(const TWI_ConfigType *Config_Ptr)
{
	//***************************** Prescaler *************************
	/*
	 * we can get rid of the insertion part as the other bits are Read Only anyway
	 */
	TWSR = (TWSR & 0xFC) | (Config_Ptr->prescaler);

	//***************************** Bit Rate *************************
	/*
	 * Calculate the division factor for the bit rate generator
	 * then Insert it on TWBR Register ( It will occupy the whole Register )
	 */
	// TWBR = ((F_CPU / Config_Ptr->SCL_Frq) - 16) / (2 * (4 ^ Config_Ptr->prescaler)); //!
	TWBR = TWBR_VALUE(Config_Ptr->SCL_Frq, Config_Ptr->prescaler);

	//***************************** Address *************************
	/*
	 * Insert Two Wire Bus address: my address if any master device want to call me
	 * The address occupy only last 7 Bits So we shift the address one to the left
	 * As General Call Recognition: Off >> TWGCE (bit 0) = 0
	 */
	TWAR = (Config_Ptr->slaveAddress) << 1;

	//***************************** Enable *************************
	SET_BIT(TWCR, TWEN); /* enable TWI */
}

void TWI_start(void)
{
	///* save values of TWEN & TWIE and clear other bits */
	// TWCR &= 0x07; //!

	/*
	 * - Enable the start bit (TWSTA)
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	SET_BIT_S(TWCR, BIT(TWSTA) | BIT(TWINT) | BIT(TWEN));

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;
}

void TWI_stop(void)
{
	///* save values of TWEN & TWIE and clear other bits */
	// TWCR &= 0x07; //!

	/*
	 * - Enable the stop bit (TWSTO)
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	SET_BIT_S(TWCR, BIT(TWSTO) | BIT(TWINT) | BIT(TWEN));

	/* Wait for TWINT flag set in TWCR Register (stop bit is send successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT)) //! not sure
		;
}

uint8 TWI_getStatus(void)
{
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}

void TWI_writeByte(uint8 a_data)
{
	///* save values of TWEN & TWIE and clear other bits */
	// TWCR &= 0x07;

	TWDR = a_data;

	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	SET_BIT_S(TWCR, BIT(TWINT) | BIT(TWEN));

	/* Wait data to be transmitted */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;
}

uint8 TWI_readByteWithACK(void)
{
	/* save values of TWEN & TWIE and clear other bits */
	// TWCR &= 0x07; //!

	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable sending ACK after reading or receiving data TWEA=1
	 * - Enable TWI Module TWEN=1
	 */
	SET_BIT_S(TWCR, BIT(TWINT) | BIT(TWEA) | BIT(TWEN));

	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;

	/* Read Data */
	return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/* save values of TWEN & TWIE and clear other bits */
	// TWCR &= 0x07; //!

	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module TWEN=1
	 */
	SET_BIT_S(TWCR, BIT(TWINT) | BIT(TWEN));

	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;

	/* Read Data */
	return TWDR;
}
