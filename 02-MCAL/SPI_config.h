/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: SPI_config.h
 *
 * Description: Static configurations for SPI driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#include "SETTINGS.h" // for F_CPU

#define Data_Order MSB
#define OPERATING_LEVEL SPI_IDLE_LOW
#define SAMPLING_EDGE SPI_RISING
#define SPI_STATE SPI_ENABLE
#define SPI_Double_SPEED SPI_Double_SPEED_OFF

/********** Data Order**************/
#define LSB 0
#define MSB 1
/**********Master/Slave Select********/
#define SPI_Master 0
#define SPI_Slave 1
/**********Clock Polarity*************/
#define SPI_IDLE_LOW 0
#define SPI_IDLE_HIGH 1
/**********Clock Phase***************/
#define SPI_RISING 0
#define SPI_FALLING 1
/*********SPI STATE****************/
#define SPI_ENABLE 0
#define SPI_DISABLE 1
/**********SPI Double Speed*********/
#define SPI_Double_SPEED_ON 0
#define SPI_Double_SPEED_OFF 1

#endif /* SPI_CONFIG_H_ */