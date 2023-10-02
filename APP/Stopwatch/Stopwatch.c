/***********************************************************************************************
 * creator: Hossam Mohamed
 * Description: Stopwatch application using Timer1 and 7-segment display (common anode) and 3 push buttons:
 * 				- Reset button (external interrupt 0)
 * 				- Pause button (external interrupt 1)
 * 				- Resume button (external interrupt 2)
 * Date: 23/05/2023
 * Version: 2.0 (Modular code)
 ************************************************************************************************/

#define F_CPU 1000000UL // 1MHz Clock frequency

#include "BIT_MACROS.h"
#include "EXTI.h"
#include "TIMER.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define Timer1_TOP 976U // exactly 1 second

#define SET_FIRST_DIGIT(PORT, x) PORT = ((PORT & 0xF0) | (((x) % 10) & 0x0F))  // display the first digit of the number on 7-segment
#define SET_SECOND_DIGIT(PORT, x) PORT = ((PORT & 0xF0) | (((x) / 10) & 0x0F)) // display the second digit of the number on 7-segment

// =========================== User-defined data types ================================ //
typedef struct Time
{
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
} Time_t;

Time_t time = {0, 0, 0}; // initialize time struct

// =========================== Interrupt Service Routines (ISR) ======================== //
void TIMER1_COMPA_ISR(void) // timer 1 - compare match interrupt (every 1 second)
{

	time.seconds++;

	if (time.seconds == 60)
	{
		time.seconds = 0;
		time.minutes++;

		if (time.minutes == 60)
		{
			time.minutes = 0;
			time.hours++;
		}
	}
}

void INT0_ISR(void) // reset button
{
	Timer1_WriteToTCNT1(0); // reset timer1
	time.seconds = 0;
	time.minutes = 0;
	time.hours = 0;
}

void INT1_ISR(void) // pause button
{
	Timer1_OCA_InterruptDisable(); // disable Timer1 compare match interrupt
}

void INT2_ISR(void) // resume button
{
	Timer1_OCA_InterruptEnable(); // enable Timer1 compare match interrupt
}

// =========================== Initialization Functions ================================ //
void SevenSegment_init(void)
{
	//========================== 7-segment configuration ===========================
	SET_MASK(DDRC, 0x0F); // 0b0000 1111 (decoder output pins)
	SET_MASK(DDRA, 0x3F); // 0b0011 1111 (7-segment display pins)
}

void Stopwatch_init()
{
	//========================== Configuration Structures =============================
	Timer1_ConfigType Stopwatch_Config = {
		.initial_value = 0,
		.compare_value = Timer1_TOP,
		.mode = TIMER1_CTC_OCR1A_MODE,
		.prescaler = F_CPU_1024,
		.oc1a_mode = OCRA_DISCONNECTED,
		.oc1b_mode = OCRB_DISCONNECTED,
	};
	Interrupt_ConfigType INT0_Config = {
		.interrupt = EXTI_INT0,
		.sense_control = falling_edge};
	Interrupt_ConfigType INT1_Config = {
		.interrupt = EXTI_INT1,
		.sense_control = rising_edge};
	Interrupt_ConfigType INT2_Config = {
		.interrupt = EXTI_INT2,
		.sense_control = falling_edge};

	//========================== Setting CallBacks ====================================
	Timer1_OCA_SetCallBack(TIMER1_COMPA_ISR);
	EXTI_setCallBack(&INT0_Config, INT0_ISR);
	EXTI_setCallBack(&INT1_Config, INT1_ISR);
	EXTI_setCallBack(&INT2_Config, INT2_ISR);

	//========================== Seven Segment initialization ===========================
	SevenSegment_init();

	//========================== Modules initialization ================================
	EXTI_init(&INT0_Config); // initialize external interrupt 0 (reset button)
	EXTI_init(&INT1_Config); // initialize external interrupt 1 (pause button)
	EXTI_init(&INT2_Config); // initialize external interrupt 2 (resume button)
	Timer1_init(&Stopwatch_Config);

	//========================== Modules enable =========================================
	Timer1_OCA_InterruptEnable(); // enable Timer1 compare match interrupt
	EXTI_enable(&INT0_Config);	  // enable external interrupt 0 (reset button)
	EXTI_enable(&INT1_Config);	  // enable external interrupt 1 (pause button)
	EXTI_enable(&INT2_Config);	  // enable external interrupt 2 (resume button)
}

void Stopwatch_main(void)
{
	Stopwatch_init();

	//========================== Global Interrupt Enable =============================
	sei(); // enable global interrupts in MC

	//========================== Application code ====================================
	while (1)
	{
		//=========================== Seconds ===========================
		PORTA = (PORTA & 0xc0) | (1 << 0); // enable 1st Digit (0b00000001)
		SET_FIRST_DIGIT(PORTC, time.seconds);
		_delay_ms(1);

		PORTA = (PORTA & 0xc0) | (1 << 1); // enable 2nd Digit (0b00000010)
		SET_SECOND_DIGIT(PORTC, time.seconds);
		_delay_ms(1);

		//=========================== Minutes ===========================
		PORTA = (PORTA & 0xc0) | (1 << 2); // enable 3rd Digit (0b00000100)
		SET_FIRST_DIGIT(PORTC, time.minutes);
		_delay_ms(1);

		PORTA = (PORTA & 0xc0) | (1 << 3); // enable 4th Digit (0b00001000)
		SET_SECOND_DIGIT(PORTC, time.minutes);
		_delay_ms(1);

		//=========================== Hours ===========================
		PORTA = (PORTA & 0xc0) | (1 << 4); // enable 5th Digit (0b00010000)
		SET_FIRST_DIGIT(PORTC, time.hours);
		_delay_ms(1);

		PORTA = (PORTA & 0xc0) | (1 << 5); // enable 6th Digit (0b00100000)
		SET_SECOND_DIGIT(PORTC, time.hours);
		_delay_ms(1);
	}
}
