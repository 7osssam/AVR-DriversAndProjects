#ifndef SENSOR_H_
#define SENSOR_H_

#include "ADC.h"
#include "std_types.h"

/******************************************************************************
 *                                LM35 Definitions                            *
 * ****************************************************************************/
#define LM35_CH 				0
#define LM35_MAX_TEMPERATURE 	150
#define LM35_MAX_VOLT 			1.5

/******************************************************************************
 *                                Functions Prototypes                        *
 * ****************************************************************************/

/******************************************************************************
 * Description: Function to read the temperature from the LM35 sensor
 * @param void
 * @return uint8
 * 		   Description: Temperature value
 * 		   Range: 0 ~ 150
 ******************************************************************************/
uint8 LM35_Read(void);

#endif /* SENSOR_H_ */