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
#define ADC_MAXIMUM_VALUE 1023	/* 10-bit resolution (2^10 - 1 = 1023)*/
#define ADC_REF_VOLT_VALUE 2.56 /* ADC reference voltage value (5V or 3.3V)*/

/* static configuration for ADC driver */
#define ADC_INTERRUPT_MODE /* Comment this line if you want polling mode */
#undef ADC_INTERRUPT_MODE
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
extern volatile uint16 g_ADC_result; /* Global variable to store the ADC result in interrupt mode */

typedef enum
{
	ADC_AREF,	 /* AREF, Internal Vref turned off */
	ADC_AVCC,	 /* AVCC with external capacitor at AREF pin */
	ADC_INTERNAL /* Internal 2.56V Voltage Reference with external capacitor at AREF pin */
} ADC_ReferenceVoltage_t;

typedef enum
{
	NO_PRESCALING = 0, // just to make sure :)
	ADC_PRESCALER_2,
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

/*******************************************************************************
 * @fn              - ADC_init
 * @brief           - This function is used to initialize the ADC driver
 * @param[in]       - ADC_ConfigType *config_ptr (Struct members are ref_volt, prescaler)
 * @return          - void
 *******************************************************************************/
void ADC_init(ADC_ConfigType *config_struct);

/*******************************************************************************
 * @fn              - ADC_readChannel
 * @brief           - This function is used to read the ADC channel
 * @param[in]       - uint8 channel_num
 * @return          - void
 *******************************************************************************/
void ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */