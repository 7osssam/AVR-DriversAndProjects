/******************************************************************************
 *
 * Module: External Interrupt
 *
 * File Name: External_Interrupt.c
 *
 * Description: Source file for the External Interrupt driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "EXT_Interrupt.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_INT0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_INT1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_INT2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                                    ISR's                                    *
 *******************************************************************************/

ISR(INT0_vect)
{
	if (g_INT0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_INT0_callBackPtr)();
	}
}

ISR(INT1_vect)
{
	if (g_INT1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_INT1_callBackPtr)();
	}
}

ISR(INT2_vect)
{
	if (g_INT2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_INT2_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void EXT_Interrupt_init(Interrupt_ConfigType *Config_Ptr)
{
	if (Config_Ptr->interrupt == EX_INT0)
	{
		switch (Config_Ptr->sense_control)
		{
		case low_level:
			CLEAR_BIT(MCUCR, ISC00);
			CLEAR_BIT(MCUCR, ISC01);
			break;
		case any_logical_change:
			SET_BIT(MCUCR, ISC00);
			CLEAR_BIT(MCUCR, ISC01);
			break;
		case falling_edge:
			CLEAR_BIT(MCUCR, ISC00);
			SET_BIT(MCUCR, ISC01);
			break;
		case rising_edge:
			SET_BIT(MCUCR, ISC00);
			SET_BIT(MCUCR, ISC01);
			break;
		}
	}
	else if (Config_Ptr->interrupt == EX_INT1)
	{
		switch (Config_Ptr->sense_control)
		{
		case low_level:
			CLEAR_BIT(MCUCR, ISC10);
			CLEAR_BIT(MCUCR, ISC11);
			break;
		case any_logical_change:
			SET_BIT(MCUCR, ISC10);
			CLEAR_BIT(MCUCR, ISC11);
			break;
		case falling_edge:
			CLEAR_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
			break;
		case rising_edge:
			SET_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
			break;
		}
	}
	else if (Config_Ptr->interrupt == EX_INT2)
	{
		switch (Config_Ptr->sense_control)
		{
		case falling_edge:
			CLEAR_BIT(MCUCSR, ISC2);
			break;
		case rising_edge:
			SET_BIT(MCUCSR, ISC2);
			break;
		}
	}
}

void EXT_Interrupt_enable(Interrupt_ConfigType *Config_Ptr)
{
	if (Config_Ptr->interrupt == EX_INT0)
	{
		SET_BIT(GICR, INT0);
	}
	else if (Config_Ptr->interrupt == EX_INT1)
	{
		SET_BIT(GICR, INT1);
	}
	else if (Config_Ptr->interrupt == EX_INT2)
	{
		SET_BIT(GICR, INT2);
	}
}

void EXT_Interrupt_disable(Interrupt_ConfigType *Config_Ptr)
{
	if (Config_Ptr->interrupt == EX_INT0)
	{
		CLEAR_BIT(GICR, INT0);
	}
	else if (Config_Ptr->interrupt == EX_INT1)
	{
		CLEAR_BIT(GICR, INT1);
	}
	else if (Config_Ptr->interrupt == EX_INT2)
	{
		CLEAR_BIT(GICR, INT2);
	}
}

/*******************************************************************************
 *                      Callback functions implementations                      *
 * *****************************************************************************/
void EXT_Interrupt_setCallBack(Interrupt_ConfigType *Config_Ptr, void (*a_ptr)(void))
{
	switch (Config_Ptr->interrupt)
	{
	case EX_INT0:
		g_INT0_callBackPtr = a_ptr;
		break;
	case EX_INT1:
		g_INT1_callBackPtr = a_ptr;
		break;
	case EX_INT2:
		g_INT2_callBackPtr = a_ptr;
		break;
	}
}
