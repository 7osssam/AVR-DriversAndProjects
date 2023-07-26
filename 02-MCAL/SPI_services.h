/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: SPI_services.h
 *
 * Description: Header file for the SPI Services driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef SPI_SERVICES_H_
#define SPI_SERVICES_H_

#include "SPI.h"
#include "STD_TYPES.h"

#define SPI_DEFAULT_DATA_VALUE 0xFF

/*******************************************************************************
 *                    Functions Prototypes                                     *
 *******************************************************************************/
void SPI_sendString(const uint8 *str);
void SPI_receiveString(uint8 *str);

#endif /* SPI_SERVICES_H_ */