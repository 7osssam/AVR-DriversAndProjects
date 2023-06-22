
/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.c
 *
 * Description: source file for the ADC driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/
#include "ADC.h"
#include "BIT_MACROS.h"
#include "avr/io.h" /* To use the IO Ports Registers */

#ifdef ADC_INTERRUPT_MODE
	#include <avr/interrupt.h> /* For ADC ISR */
#endif
/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
#ifdef ADC_INTERRUPT_MODE
volatile uint16 g_ADC_result = 0;
#endif
/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*******************************************************************************
 * @fn              - ADC_init
 * @brief           - This function is used to initialize the ADC driver
 * @param[in]       - ADC_ReferenceVoltage_t ref_volt
 * @param[in]       - ADC_Prescaler_t prescaler
 * @return          - void
 * @note            - none
 *******************************************************************************/
void ADC_init(ADC_ConfigType *config_ptr)
{
	// ADMUX | Voltage Reference Selections for ADC
	switch (config_ptr->ref_volt)
	{
	case ADC_AREF:
		CLEAR_BIT(ADMUX, REFS0);
		CLEAR_BIT(ADMUX, REFS1);
		break;
	case ADC_AVCC:
		SET_BIT(ADMUX, REFS0);
		CLEAR_BIT(ADMUX, REFS1);
		break;
	case ADC_INTERNAL:
		SET_BIT(ADMUX, REFS0);
		SET_BIT(ADMUX, REFS1);
		break;
	}

	// ADMUX | Assume channel 0  11100000
	ADMUX = ADMUX & 0xE0;

	// ADCSRA | ADC Prescaler Selections 11111000
	ADCSRA = (ADCSRA & 0xF8) | (config_ptr->prescaler & 0x07);

	// ADCSRA | ADC enable
	SET_BIT(ADCSRA, ADEN);

#ifdef ADC_INTERRUPT_MODE
	// ADCSRA | enable ADC Interrupt
	SET_BIT(ADCSRA, ADIE);
#else
	// ADCSRA | disable ADC Interrupt
	CLEAR_BIT(ADCSRA, ADIE);
#endif
}

#ifdef ADC_INTERRUPT_MODE

/* ADC Interrupt Service Routine executed when ADC conversion completes */
ISR(ADC_vect)
{
	g_ADC_result = ADC; /* Read ADC Data after conversion complete */
}

/*******************************************************************************
 * @fn              - ADC_readChannel
 * @brief           - This function is used to read the ADC channel in interrupt mode
 * @param[in]       - uint8 channel_num
 * @return          - void
 * @note            - none
 *******************************************************************************/
void ADC_readChannel_INTERRUPT(uint8 channel_num)
{
	// insert channel num 00000111
	ADMUX = (ADMUX & 0xE0) | (0x07 & channel_num); /* Input channel number must be from 0 --> 7 */

	// start conversion
	SET_BIT(ADCSRA, ADSC);
}

#else
/*******************************************************************************
 * @fn              - ADC_readChannel
 * @brief           - This function is used to read the ADC channel in polling mode
 * @param[in]       - uint8 channel_num
 * @return          - uint16
 * @note            - none
 *******************************************************************************/
uint16 ADC_readChannel(uint8 channel_num)
{
	// insert channel num 00000111
	ADMUX = (ADMUX & 0xE0) | (0x07 & channel_num); /* Input channel number must be from 0 --> 7 */

	// start conversion
	SET_BIT(ADCSRA, ADSC);

	// polling until flag ADCIF == 1
	while (IS_BIT_CLEAR(ADCSRA, ADIF))
		;

	// clear flag
	SET_BIT(ADCSRA, ADIF);

	// return date
	return ADC;
}

#endif
