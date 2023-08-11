
/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Description: Header file for the Timer driver
 *
 * Author: Hossam Mohamed
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "STD_TYPES.h"

/********************************************************************************************************
 * 												Timer 0													*
 ********************************************************************************************************/

//======================================== Clock Select =============================================
typedef enum
{
	NO_CLOCK,		  /* No clock Source */
	F_CPU_CLOCK,	  /* System Clock Source */
	F_CPU_8,		  /* System Clock Source divided by 8 */
	F_CPU_64,		  /* System Clock Source divided by 64 */
	F_CPU_256,		  /* System Clock Source divided by 256 */
	F_CPU_1024,		  /* System Clock Source divided by 1024 */
	EXTERNAL_FALLING, /* External clock source on T0 pin. Clock on falling edge. */
	EXTERNAL_RISING	  /* External clock source on T0 pin. Clock on rising edge. */
} Prescaler_type;

//======================================== Waveform Generation Modes ================================
typedef enum
{
	TIMER0_NORMAL_MODE = 0,	  // TOP = 0xFF
	TIMER0_PHASECORRECT_MODE, // TOP = 0xFF
	TIMER0_CTC_MODE,		  // TOP = OCRA
	TIMER0_FASTPWM_MODE		  // TOP = 0xFF

} Timer0Mode_type;

//======================================== Compare Output Modes ====================================
typedef enum
{
	OC0_DISCONNECTED = 0, // Normal port operation, OC0 disconnected.
	OC0_TOGGLE,			  // Toggle OC0 on compare match
	OC0_NON_INVERTING,	  // Clear OC0 on compare match (Set output to low level).
	OC0_INVERTING		  // Set OC0 on compare match (Set output to high level).

} OC0Mode_type;

typedef struct
{
	uint8 initial_value;
	Timer0Mode_type mode;
	Prescaler_type prescaler;
	OC0Mode_type oc_mode;
} Timer0_ConfigType;

//******************************** Initialization **************************************************
/*******************************************************************************
 * Description: Function to initialize the Timer0 driver
 * @param Config_Ptr pointer to Timer0 configuration structure
 * 						- initial_value: initial value for TCNT0 register
 * 						- mode: Waveform Generation Mode
 * 						- prescaler: Clock Select
 * 						- oc_mode: Compare Output Mode
 * @return void
 *******************************************************************************/
void Timer0_init(const Timer0_ConfigType *Config_Ptr);
//******************************** Write/Read ******************************************************
void Timer0_WriteToTCNT0(uint8 a_value);
uint8 Timer0_ReadTCNT0(void);
//******************************** overflow interrupt **********************************************
void Timer0_OV_InterruptEnable(void);
void Timer0_OV_InterruptDisable(void);
//******************************** Output Compare interrupt ****************************************
void Timer0_OC_InterruptEnable(void);
void Timer0_OC_InterruptDisable(void);

/********************************************************************************************************
 * 												Timer 1													*
 ********************************************************************************************************/

//======================================== Waveform Generation Modes ==================================
typedef enum
{
	TIMER1_NORMAL_MODE = 0,					  // TOP = 0xFFFF
	TIMER1_PWM_8BIT_MODE,					  // TOP = 0x00FF
	TIMER1_PWM_9BIT_MODE,					  // TOP = 0x01FF
	TIMER1_PWM_10BIT_MODE,					  // TOP = 0x03FF
	TIMER1_CTC_OCR1A_MODE,					  // TOP = OCR1A
	TIMER1_FASTPWM_8BIT_MODE,				  // TOP = 0x00FF
	TIMER1_FASTPWM_9BIT_MODE,				  // TOP = 0x01FF
	TIMER1_FASTPWM_10BIT_MODE,				  // TOP = 0x03FF
	TIMER1_PWM_PHASE_FREQ_CORRECT_ICR1_MODE,  // TOP = ICR1
	TIMER1_PWM_PHASE_FREQ_CORRECT_OCR1A_MODE, // TOP = OCR1A
	TIMER1_PWM_PHASE_CORRECT_ICR1_MODE,		  // TOP = ICR1
	TIMER1_PWM_PHASE_CORRECT_OCR1A_MODE,	  // TOP = OCR1A
	TIMER1_CTC_ICR_TOP_MODE,				  // TOP = ICR1
	// TIMER1_RESERVED,						  // Reserved
	TIMER1_FASTPWM_ICR_TOP_MODE,  // TOP = ICR1
	TIMER1_FASTPWM_OCRA_TOP_MODE, // TOP = OCR1A
} Timer1Mode_type;

//======================================== Compare Output Modes (OC1A) =====================================
typedef enum
{
	OCRA_DISCONNECTED = 0,
	OCRA_TOGGLE,
	OCRA_NON_INVERTING,
	OCRA_INVERTING

} OC1A_Mode_type;

//======================================== Compare Output Modes (OC1B) =====================================
typedef enum
{
	OCRB_DISCONNECTED = 0,
	OCRB_TOGGLE,
	OCRB_NON_INVERTING,
	OCRB_INVERTING

} OC1B_Mode_type;

typedef struct
{
	uint16 initial_value;
	uint16 compare_value; // in case of CTC mode
	Timer1Mode_type mode;
	Prescaler_type prescaler;
	OC1A_Mode_type oc1a_mode;
	OC1B_Mode_type oc1b_mode;
} Timer1_ConfigType;

//******************************** Initialization **************************************************
/*******************************************************************************
 * Description: Function to initialize the Timer1 driver
 * @param Config_Ptr pointer to Timer1 configuration structure
 * 						- initial_value: initial value for TCNT1 register
 * 						- compare_value: compare value for OCR1A register in case of CTC mode
 * 						- mode: Waveform Generation Mode
 * 						- prescaler: Clock Select
 * 						- oc1a_mode: Compare Output Mode for OC1A
 * 						- oc1b_mode: Compare Output Mode for OC1B
 * @return void
 *******************************************************************************/
void Timer1_init(const Timer1_ConfigType *Config_Ptr);

//******************************** Write/Read ******************************************************
void Timer1_WriteToTCNT1(uint16 Value);
uint16 Timer1_ReadTCNT1(void);

//******************************** overflow interrupt **********************************************
void Timer1_OVF_InterruptEnable(void);
void Timer1_OVF_InterruptDisable(void);
//******************************** Output Compare interrupt ****************************************
void Timer1_OCA_InterruptEnable(void);
void Timer1_OCA_InterruptDisable(void);
void Timer1_OCB_InterruptEnable(void);
void Timer1_OCB_InterruptDisable(void);

//******************************** Call Back Functions *********************************************
void Timer1_OVF_SetCallBack(void (*LocalFptr)(void));
void Timer1_OCA_SetCallBack(void (*LocalFptr)(void));
void Timer1_OCB_SetCallBack(void (*LocalFptr)(void));

#endif /* TIMER_H_ */