/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: SPI.c
 *
 * Description: Source file for the SPI AVR driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "SPI.h"
#include "SPI_config.h"

#include "BIT_MACROS.h"
#include "GPIO.h"
#include <avr/interrupt.h>
#include <avr/io.h>

static void (*SPI_callBackPtr)(void) = NULL_PTR;

/******************************************* initialization  *********************************************/
void SPI_init(Master_Slave_Select_t Master_Slave_Select, Clock_Rate_t Clock_Rate)
{
	//***************************** Data Order *****************************************
#if Data_Order == LSB
	SET_BIT(SPCR, DORD);
#elif Data_Order == MSB
	CLEAR_BIT(SPCR, DORD);
#endif

	//***************************** Master/Slave Select ********************************/
	if (Master_Slave_Select == SPI_Master)
	{
		/******** Configure SPI Master Pins *********
		 * SS(PB4)   --> Output
		 * MOSI(PB5) --> Output
		 * MISO(PB6) --> Input
		 * SCK(PB7)  --> Output
		 ********************************************/
		GPIO_setupPinDirection(PORTB_ID, PIN4_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_INPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);
		SET_BIT(SPCR, MSTR);
	}
	else if (Master_Slave_Select == SPI_Slave)
	{
		/******** Configure SPI Slave Pins *********
		 * SS(PB4)   --> Input
		 * MOSI(PB5) --> Input
		 * MISO(PB6) --> Output
		 * SCK(PB7)  --> Input
		 ********************************************/
		GPIO_setupPinDirection(PORTB_ID, PIN4_ID, PIN_INPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_INPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_OUTPUT);
		GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_INPUT);
		CLEAR_BIT(SPCR, MSTR);
	}

	//***************************** Operation level (Clock polarity) ********************/
#if OPERATING_LEVEL == SPI_IDLE_LOW
	CLEAR_BIT(SPCR, CPOL);
#elif OPERATING_LEVEL == SPI_IDLE_HIGH
	SET_BIT(SPCR, CPOL);
#endif

	//***************************** Sampling Egde (Clock Phase) ************************/
#if SAMPLING_EDGE == SPI_RISING
	SET_BIT(SPCR, CPHA);
#elif SAMPLING_EDGE == SPI_FALLING
	CLEAR_BIT(SPCR, CPHA);
#endif

	//***************************** Clock Rate *****************************************/
	SPCR = (SPCR & 0xFC) | (Clock_Rate & 0x03);

	//***************************** SPI Double Speed ***********************************/
#if SPI_Double_SPEED == SPI_Double_SPEED_ON
	SET_BIT(SPSR, SPI2X);
#elif SPI_Double_SPEED == SPI_Double_SPEED_OFF
	CLEAR_BIT(SPSR, SPI2X);
#endif

	//***************************** SPI Enable/Disable *********************************/
#if SPI_STATE == SPI_ENABLE
	SET_BIT(SPCR, SPE);
#elif SPI_STATE == SPI_DISABLE
	CLEAR_BIT(SPCR, SPE);
#endif
}

/**************************************** Interrupt Enable/Disable ********************************************/
void SPI_EnableInterrupt(void)
{
	SET_BIT(SPCR, SPIE);
}

void SPI_DisableInterrupt(void)
{
	CLEAR_BIT(SPCR, SPIE);
}

/***************************************** Call Back functions ***********************************************/
void SPI_SetCallBack(void (*LocalFptr)(void))
{
	SPI_callBackPtr = LocalFptr;
}

/********************************************** ISR ************************************************************/
ISR(SPI_STC_vect)
{
	if (SPI_callBackPtr != NULL_PTR)
	{
		SPI_callBackPtr();
	}
}

/********************************** Send and receive functions with polling **************************************/
uint8 SPI_sendReceiveByte(uint8 a_data)
{
	SPDR = a_data; // send data

	while (IS_BIT_CLEAR(SPSR, SPIF)) // wait until data is received/sent
		;

	return SPDR; // received data
}

void SPI_SendByte(uint8 a_data)
{
	SPDR = a_data;
	while (IS_BIT_CLEAR(SPSR, SPIF)) // wait until data is sent
		;
}

uint8 SPI_ReceiveByte(void)
{
	while (IS_BIT_CLEAR(SPSR, SPIF)) // wait until data is received
		;
	return SPDR;
}

uint8 SPI_ReceiveByteCheck(uint8 *ptr_data)
{
	uint8 status = FALSE;
	if (IS_BIT_CLEAR(SPSR, SPIF))
	{
		*ptr_data = SPDR;
		status = TRUE;
	}
	return status;
}

/********************************* Send and receive functions with no ckecking - for interrupt - *******************/
void SPI_SendByteNoBlock(uint8 a_data)
{
	SPDR = a_data;
}

uint8 SPI_ReceiveByteNoBlock(void)
{
	return SPDR;
}
