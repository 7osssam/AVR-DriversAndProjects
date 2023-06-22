/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.h
 *
 * Description: Header file for the ADC drivers
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "STD_TYPES.h"

typedef enum
{
	ADC_AREF,
	ADC_AVCC,
	ADC_INTERNAL
} ADC_ReferenceVoltage_t;

typedef enum
{
	ADC_PRESCALER_2 = 1,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128
} ADC_Prescaler_t;

void ADC_init(ADC_ReferenceVoltage_t ref_volt, ADC_Prescaler_t prescaler);
uint16 ADC_readChannel(uint8 channel_num);