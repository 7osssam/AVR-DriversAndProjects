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
#include "SETTINGS.h" /* For F_CPU */
#include <avr/io.h>

// Calculate the TWBR value for a given SCL frequency and prescaler value
#define TWBR_VALUE(SCL_freq, TWPS_value) ((F_CPU / SCL_freq) - 16) / (2 * (1 << TWPS_value))

/**
 * @brief Initialize the TWI (I2C) module based on the provided configuration.
 *
 * This function initializes the TWI module based on the configuration provided
 * through the \c Config_Ptr parameter. It sets the TWBR value, prescaler,
 * slave address, and enables the TWI module.
 *
 * @param Config_Ptr A pointer to the configuration structure.
 */
void TWI_init(const TWI_configType *Config_Ptr)
{
	//***************************** Bit Rate *************************
	TWBR_VALUE(Config_Ptr->SCL_Frq, Config_Ptr->prescaler);

	//***************************** Prescaler *************************
	TWSR = Config_Ptr->prescaler;

	//**************** Configure Slave Address and General Call Recognition Mode ***************
	TWAR = ((Config_Ptr->TWI_slaveAddress) << 1);

	TWAR = (TWAR & 0b11111110) | ((Config_Ptr->generalCallRecognitionEnableModeConfig & 0x01) << (TWGCE));

	//***************************** Enable TWI *************************
	SET_BIT(TWCR, TWEN);

	/* Enable pull up resistors at SCL and SDA Pins */
	// SET_BIT(PORTC, 4);
	// SET_BIT(PORTC, 5);
}

/**
 * @brief Start the TWI communication.
 *
 * This function generates a start condition on the TWI bus. It waits until the
 * start bit is sent successfully before returning.
 */
void TWI_start(void)
{
	/*
	 * - Enable the start bit (TWSTA)
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	// SET_MASK(TWCR, BIT(TWSTA) | BIT(TWINT) | BIT(TWEN));
	TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWSTA);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;
}

/**
 * @brief Stop the TWI communication.
 *
 * This function generates a stop condition on the TWI bus. It waits until the
 * stop bit is sent successfully before returning.
 */
void TWI_stop(void)
{

	/*
	 * - Enable the stop bit (TWSTO)
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	// SET_MASK(TWCR, BIT(TWSTO) | BIT(TWINT) | BIT(TWEN));

	TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWSTO);
}

/**
 * @brief Write a byte to the TWI bus.
 *
 * This function writes a byte of data to the TWI bus. It puts the data into
 * the TWI data register, starts the data transmission, and waits until the
 * data is sent successfully before returning.
 *
 * @param data The byte of data to be written.
 */
void TWI_writeByte(uint8 data)
{
	///* Put data On TWI data Register */
	TWDR = data;

	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module (TWEN)
	 */
	TWCR = BIT(TWINT) | BIT(TWEN);

	/* Wait for data is send successfully */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;
}

/**
 * @brief Read a byte from the TWI bus with ACK.
 *
 * This function reads a byte of data from the TWI bus with an acknowledgment (ACK).
 * It starts reading, sends an ACK after receiving the data, and waits until the
 * data is received before returning.
 *
 * @return The byte of data received.
 */
uint8 TWI_readByteWithACK(void)
{
	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable sending ACK after reading or receiving data TWEA=1
	 * - Enable TWI Module TWEN=1
	 */
	TWCR = BIT(TWINT) | BIT(TWEN) | BIT(TWEA);

	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;

	/* Read Data */
	return TWDR;
}

/**
 * @brief Read a byte from the TWI bus with NACK.
 *
 * This function reads a byte of data from the TWI bus with a not-acknowledgment (NACK).
 * It starts reading, does not send an ACK after receiving the data, and waits until
 * the data is received before returning.
 *
 * @return The byte of data received.
 */
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * - clear the interrupt flag by setting (TWINT)
	 * - Enable TWI Module TWEN=1
	 */
	TWCR = BIT(TWINT) | BIT(TWEN);

	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (IS_BIT_CLEAR(TWCR, TWINT))
		;

	/* Read Data */
	return TWDR;
}

/**
 * @brief Get the current status of the TWI bus.
 *
 * This function returns the current status of the TWI bus by masking and extracting
 * the relevant status bits from the TWSR register.
 *
 * @return The current status of the TWI bus.
 */
uint8 TWI_getStatus(void)
{
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}
