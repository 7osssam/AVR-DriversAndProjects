
/******************************************************************************
 *
 * Module: Watchdog Timer
 *
 * File Name: WDT.h
 *
 * Description: Header file for the Watchdog Timer driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef WDT_H_
#define WDT_H_

#include "STD_TYPES.h"
#include <avr/io.h>

/*******************************************************************************
 *                    User defined data types                                  *
 *******************************************************************************/
typedef enum
{
	timeout_16ms = 0,
	timeout_32ms,
	timeout_65ms,
	timeout_130ms,
	timeout_260ms,
	timeout_520ms,
	timeout_1000ms,
	timeout_2100ms,

} timeout_type;

/*******************************************************************************
 *                    Functions Prototypes                                     *
 *******************************************************************************/
/*
 * Description : Set the required timeout for the Watchdog Timer.
 * arguments   : timeout_type time : required timeout
 * Return      : None
 */
void WDT_set(timeout_type time);

/*
 * Description : Stop the Watchdog Timer.
 * arguments   : None
 * Return      : None
 */
void WDT_stop(void);

#endif /* WDT_H_ */