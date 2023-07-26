
/******************************************************************************
 *
 * Module: Watchdog Timer
 *
 * File Name: WDT.c
 *
 * Description: Source file for the Watchdog Timer driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "WDT.h"

void WDT_set(timeout_type time)
{

	uint8 wdt_value = 0x08; // WDE SET & WDTOE CLEARED
	wdt_value |= time;
	WDTCR |= (1 << WDTOE) | (1 << WDE);
	WDTCR = wdt_value;
}
void WDT_stop(void)
{
	WDTCR = (1 << WDTOE) | (1 << WDE);
	WDTCR = 0;
}