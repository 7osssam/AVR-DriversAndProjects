
/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: Source file for the Timer driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#include "TIMER.h"

#include "BIT_MACROS.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/****************************Pointer to functions to be assigned to ISR*********************************/

static void (*g_Timer1_OVF_callBackPtr)(void) = NULL_PTR;
static void (*g_Timer1_OCA_callBackPtr)(void) = NULL_PTR;
static void (*g_Timer1_OCB_callBackPtr)(void) = NULL_PTR;

static void (*g_Timer0_OVF_callBackPtr)(void) = NULL_PTR;
static void (*g_Timer0_OC_callBackPtr)(void) = NULL_PTR;

/********************************************************************************************************
 * 												Timer 0													*
 ********************************************************************************************************/

/********************************* Timer 0 ISR functions ****************************************************/
ISR(TIMER0_OVF_vect)
{
	if (g_Timer0_OVF_callBackPtr != NULL_PTR)
	{
		g_Timer0_OVF_callBackPtr();
	}
}
ISR(TIMER0_COMP_vect)
{
	if (g_Timer0_OC_callBackPtr != NULL_PTR)
	{
		g_Timer0_OC_callBackPtr();
	}
}

/********************************* Timer 0 initialization function ******************************************/
// void Timer0_init(uint8 initial_value, Timer0Mode_type mode, Prescaler_type Prescaler, OC0Mode_type oc_mode)
void Timer0_init(const Timer0_ConfigType *Config_Ptr)
{
	//*********************************************** Waveform Generation Modes ********************************/
	switch (Config_Ptr->mode)
	{
	case TIMER0_NORMAL_MODE:
		CLEAR_BIT(TCCR0, WGM00);
		CLEAR_BIT(TCCR0, WGM01);
		break;
	case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0, WGM00);
		CLEAR_BIT(TCCR0, WGM01);
		break;
	case TIMER0_CTC_MODE:
		CLEAR_BIT(TCCR0, WGM00);
		SET_BIT(TCCR0, WGM01);
		break;
	case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0, WGM00);
		SET_BIT(TCCR0, WGM01);
		break;
	}

	//*********************************************** Clock Select ************************************************/
	TCCR0 = (TCCR0 & 0XF8) | (Config_Ptr->prescaler & 0X07);

	//*********************************************** Compare Output Modes ****************************************/
	switch (Config_Ptr->oc_mode)
	{
	case OC0_DISCONNECTED:
		CLEAR_BIT(TCCR0, COM00);
		CLEAR_BIT(TCCR0, COM01);
		break;
	case OC0_TOGGLE:
		SET_BIT(TCCR0, COM00);
		CLEAR_BIT(TCCR0, COM01);
		break;
	case OC0_NON_INVERTING:
		CLEAR_BIT(TCCR0, COM00);
		SET_BIT(TCCR0, COM01);
		break;
	case OC0_INVERTING:
		SET_BIT(TCCR0, COM00);
		SET_BIT(TCCR0, COM01);
		break;
	}

	//*********************************************** Initial Value ************************************************/
	if (Config_Ptr->mode == TIMER0_FASTPWM_MODE || Config_Ptr->mode == TIMER0_PHASECORRECT_MODE)
	{
		TCNT0 = 0;
		OCR0 = Config_Ptr->initial_value;
	}
	else
	{
		TCNT0 = Config_Ptr->initial_value;
	}
}

/*********************************************** Timer 0 Write/Read *******************************************/
void Timer0_WriteToTCNT0(uint8 a_value)
{
	TCNT0 = a_value;
}
uint8 Timer0_ReadTCNT0(void)
{
	return TCNT0;
}
void Timer0_WriteToOCR0(uint8 a_value)
{
	OCR0 = a_value;
}
/*********************************************** Interrupt Enable/Disable ************************************/
void Timer0_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK, TOIE0);
}
void Timer0_OV_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK, TOIE0);
}
void Timer0_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK, OCIE0);
}
void Timer0_OC_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK, OCIE0);
}

/*********************************************** Call Back functions ******************************************/
void Timer0_OVF_SetCallBack(void (*LocalFptr)(void))
{
	g_Timer0_OVF_callBackPtr = LocalFptr;
}
void Timer0_Oc_SetCallBack(void (*LocalFptr)(void))
{
	g_Timer0_OC_callBackPtr = LocalFptr;
}

/********************************************************************************************************
 * 													Timer 1												*
 ********************************************************************************************************/

/********************************* Timer 1 ISR functions ****************************************************/
ISR(TIMER1_OVF_vect)
{
	if (g_Timer1_OVF_callBackPtr != NULL_PTR)
	{
		g_Timer1_OVF_callBackPtr();
	}
}
ISR(TIMER1_COMPA_vect)
{
	if (g_Timer1_OCA_callBackPtr != NULL_PTR)
	{
		g_Timer1_OCA_callBackPtr();
	}
}
ISR(TIMER1_COMPB_vect)
{
	if (g_Timer1_OCB_callBackPtr != NULL_PTR)
	{
		g_Timer1_OCB_callBackPtr();
	}
}
/********************************* Timer 1 initialization function ******************************************/
// void Timer1_init(uint16 compare_value, uint16 initial_value, Timer1Mode_type mode, Prescaler_type Prescaler, OC1A_Mode_type oc1a_mode, OC1B_Mode_type oc1b_mode)
void Timer1_init(const Timer1_ConfigType *Config_Ptr)
{
	//*********************************************** Waveform Generation Modes ********************************/
	TCCR1A = (TCCR1A & 0xFC) | ((Config_Ptr->mode) & 0x03);
	TCCR1B = (TCCR1B & 0xE7) | (((Config_Ptr->mode) & 0x0C) << 1);

	//******************************************** OC1A & OC1B Modes ********************************************//
	switch (Config_Ptr->oc1a_mode)
	{
	case OCRA_DISCONNECTED:
		CLEAR_BIT(TCCR1A, COM1A0);
		CLEAR_BIT(TCCR1A, COM1A1);
		break;
	case OCRA_TOGGLE:
		SET_BIT(TCCR1A, COM1A0);
		CLEAR_BIT(TCCR1A, COM1A1);
		break;
	case OCRA_NON_INVERTING:
		CLEAR_BIT(TCCR1A, COM1A0);
		SET_BIT(TCCR1A, COM1A1);
		break;
	case OCRA_INVERTING:
		SET_BIT(TCCR1A, COM1A0);
		SET_BIT(TCCR1A, COM1A1);
		break;
	}
	switch (Config_Ptr->oc1b_mode)
	{
	case OCRB_DISCONNECTED:
		CLEAR_BIT(TCCR1A, COM1B0);
		CLEAR_BIT(TCCR1A, COM1B1);
		break;
	case OCRB_TOGGLE:
		SET_BIT(TCCR1A, COM1B0);
		CLEAR_BIT(TCCR1A, COM1B1);
		break;
	case OCRB_NON_INVERTING:
		CLEAR_BIT(TCCR1A, COM1B0);
		SET_BIT(TCCR1A, COM1B1);
		break;
	case OCRB_INVERTING:
		SET_BIT(TCCR1A, COM1B0);
		SET_BIT(TCCR1A, COM1B1);
		break;
	}

	//*********************************************** Timer 1 Prescaler ******************************************/
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);

	//*********************************************** Initial Value ************************************************/
	switch (Config_Ptr->mode)
	{
	case TIMER1_NORMAL_MODE:
		TCNT1 = Config_Ptr->initial_value;
	case TIMER1_CTC_OCR1A_MODE:
	case TIMER1_CTC_ICR_TOP_MODE:
		TCNT1 = Config_Ptr->initial_value;
		OCR1A = Config_Ptr->compare_value;
		break;
	case TIMER1_PWM_8BIT_MODE:
	case TIMER1_PWM_9BIT_MODE:
	case TIMER1_PWM_10BIT_MODE:
	case TIMER1_FASTPWM_8BIT_MODE:
	case TIMER1_FASTPWM_9BIT_MODE:
	case TIMER1_FASTPWM_10BIT_MODE:
	case TIMER1_PWM_PHASE_FREQ_CORRECT_ICR1_MODE:
	case TIMER1_PWM_PHASE_FREQ_CORRECT_OCR1A_MODE:
	case TIMER1_PWM_PHASE_CORRECT_ICR1_MODE:
	case TIMER1_PWM_PHASE_CORRECT_OCR1A_MODE:
	case TIMER1_FASTPWM_ICR_TOP_MODE:
	case TIMER1_FASTPWM_OCRA_TOP_MODE:
		TCNT1 = Config_Ptr->initial_value;
		OCR1A = Config_Ptr->compare_value;
		break;
	}
}

/*********************************************** Timer 1 Write/Read *******************************************/
void Timer1_WriteToTCNT1(uint16 a_value)
{
	TCNT1 = a_value;
}

uint16 Timer1_ReadTCNT1(void)
{
	return TCNT1;
}

void Timer1_WriteToOCR1A(uint16 a_value)
{
	OCR1A = a_value;
}

void Timer1_WriteToOCR1B(uint16 a_value)
{
	OCR1B = a_value;
}
/*********************************************** Interrupt Enable/Disable ************************************/
//=========================== Oveflow Interrupt Enable/Disable ==========================
void Timer1_OVF_InterruptEnable(void)
{
	SET_BIT(TIMSK, TOIE1);
}
void Timer1_OVF_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK, TOIE1);
}
//=========================== Output Compare A Interrupt Enable/Disable =================
void Timer1_OCA_InterruptEnable(void)
{
	SET_BIT(TIMSK, OCIE1A);
}
void Timer1_OCA_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK, OCIE1A);
}
//=========================== Output Compare B Interrupt Enable/Disable =================
void Timer1_OCB_InterruptEnable(void)
{
	SET_BIT(TIMSK, OCIE1B);
}
void Timer1_OCB_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK, OCIE1B);
}

/*********************************************** Call Back functions ******************************************/
//=========================== overflow Call Back function ================================
void Timer1_OVF_SetCallBack(void (*LocalFptr)(void))
{
	g_Timer1_OVF_callBackPtr = LocalFptr;
}
//=========================== Output Compare A Call Back function ========================
void Timer1_OCA_SetCallBack(void (*LocalFptr)(void))
{
	g_Timer1_OCA_callBackPtr = LocalFptr;
}
//=========================== Output Compare B Call Back function ========================
void Timer1_OCB_SetCallBack(void (*LocalFptr)(void))
{
	g_Timer1_OCB_callBackPtr = LocalFptr;
}