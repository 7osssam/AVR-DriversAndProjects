/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: SPI_services.c
 *
 * Description: Source file for the SPI services driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#include "SPI_services.h"

#include "SETTINGS.h" // for F_CPU

/*************************************************************************************************************
 *   										Functions Definitions										 	 *
 *************************************************************************************************************/

void SPI_sendString(const uint8 *str)
{
	uint8 i = 0;
	uint8 received_data = 0;

	/* Send the whole string */
	while (str[i] != '\0')
	{
		/*
		 * received_data contains the received data from the other device.
		 * It is a dummy data variable as we just need to send the string to other device.
		 */
		received_data = SPI_sendReceiveByte(str[i]);
		i++;
	}
}

void SPI_receiveString(uint8 *str) // receive until '#'
{
	uint8 i = 0;

	/* Receive the first byte */
	str[i] = SPI_sendReceiveByte(SPI_DEFAULT_DATA_VALUE);

	/* Receive the whole string until the '#' */
	while (str[i] != '#')
	{
		i++;
		str[i] = SPI_sendReceiveByte(SPI_DEFAULT_DATA_VALUE);
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	str[i] = '\0';
}
