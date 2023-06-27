/******************************************************************************
 *
 * Module: External Interrupt
 *
 * File Name: External_Interrupt.h
 *
 * Description: Header file for External Interrupt Module.
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef EXT_INTERRUPT_H_
#define EXT_INTERRUPT_H_

#include "STD_TYPES.h"
#include <avr/io.h>

/*******************************************************************************
 * 					       Types Declaration                               	   *
 * *****************************************************************************/
typedef enum
{
	EX_INT0,
	EX_INT1,
	EX_INT2
} EXT_Interrupt;

typedef enum
{
	low_level,
	any_logical_change,
	falling_edge,
	rising_edge
} EXT_State;

/*******************************************************************************
 * 								Configuration structure                        *
 * *****************************************************************************/
typedef struct
{
	EXT_Interrupt interrupt; // EX_INT0, EX_INT1, EX_INT2
	EXT_State sense_control; // low_level, any_logical_change, falling_edge, rising_edge
} Interrupt_ConfigType;

/*******************************************************************************
 * 								Functions Prototypes                           *
 * *****************************************************************************/
void EXT_Interrupt_init(Interrupt_ConfigType *Config_Ptr);
void EXT_Interrupt_enable(Interrupt_ConfigType *Config_Ptr);
void EXT_Interrupt_disable(Interrupt_ConfigType *Config_Ptr);

/****************************** Call Back function *****************************/
void EXT_Interrupt_setCallBack(Interrupt_ConfigType *Config_Ptr, void (*a_ptr)(void));

#endif /* EXT_INTERRUPT_H_ */