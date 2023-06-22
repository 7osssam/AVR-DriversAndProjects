#include "Sensor.h"

/******************************************************************************
 *                          Functions Definitions                             *
 * ****************************************************************************/

uint8 LM35_Read(void)
{
	uint8 temp_value = 0;

#ifdef ADC_INTERRUPT_MODE /* If ADC is working in interrupt mode */

	/* Read ADC channel where the temperature sensor is connected */
	ADC_readChannel_INTERRUPT(LM35_CH);
	/* Calculate the temperature from the ADC value*/
	temp_value = (uint8)(((uint32)g_ADC_result * LM35_MAX_TEMPERATURE * ADC_REF_VOLT_VALUE) / (ADC_MAXIMUM_VALUE * LM35_MAX_VOLT));

#else /* If ADC is working in polling mode */
	uint16 adc_value = 0;
	/* Read ADC channel where the temperature sensor is connected */
	adc_value = ADC_readChannel(LM35_CH);
	/* Calculate the temperature from the ADC value*/
	temp_value = (uint8)(((uint32)adc_value * LM35_MAX_TEMPERATURE * ADC_REF_VOLT_VALUE) / (ADC_MAXIMUM_VALUE * LM35_MAX_VOLT));

#endif

	return temp_value;
}