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

#ifndef ADC_H_
#define ADC_H_

#include "STD_TYPES.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE 1023
#define ADC_REF_VOLT_VALUE 5

#define ADC_INTERRUPT_MODE

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
#ifdef ADC_INTERRUPT_MODE
extern volatile uint16 g_ADC_result;
#endif

typedef enum
{
	ADC_AREF,	 /* AREF, Internal Vref turned off */
	ADC_AVCC,	 /* AVCC with external capacitor at AREF pin */
	ADC_INTERNAL /* Internal 2.56V Voltage Reference with external capacitor at AREF pin */
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

typedef struct
{
	ADC_ReferenceVoltage_t ref_volt;
	ADC_Prescaler_t prescaler;
} ADC_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void ADC_init(ADC_ConfigType *config_struct);

#ifdef ADC_INTERRUPT_MODE
void ADC_readChannel_INTERRUPT(uint8 channel_num);
#else
uint16 ADC_readChannel(uint8 channel_num);
#endif

#endif /* ADC_H_ */