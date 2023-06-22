
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

// void ADC_init(void)
//{
//	// ADMUX | Voltage Reference Selections for ADC
//	SET_BIT(ADMUX, REFS0);
//	CLEAR_BIT(ADMUX, REFS1);
//	// ADMUX | Assume channel 0  11100000
//	ADMUX = ADMUX & 0xE0;
//	// ADCSRA | ACD enable
//	SET_BIT(ADCSRA, ADEN);
//	// ADCSRA | disable ADC Interrupt
//	CLEAR_BIT(ADCSRA, ADIE);
//	// ADCSRA | ADC Prescaler Selections
//	SET_BIT(ADCSRA, ADPS0);
//	SET_BIT(ADCSRA, ADPS1);
//	SET_BIT(ADCSRA, ADPS2);
//}

void ADC_init(ADC_ReferenceVoltage_t ref_volt, ADC_Prescaler_t prescaler)
{
	// ADMUX | Voltage Reference Selections for ADC
	switch (ref_volt)
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
	ADCSRA = (ADCSRA & 0xF8) | (prescaler & 0x07);

	// ADCSRA | ACD enable
	SET_BIT(ADCSRA, ADEN);
	// ADCSRA | disable ADC Interrupt
	CLEAR_BIT(ADCSRA, ADIE);
}

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
