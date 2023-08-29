
#ifndef SYS_CONFIG_
#define SYS_CONFIG_

/***************************************************************
 * 					System Options Configuration 			   *
 ***************************************************************/
#define MAXIMUM_WRONG_PASSWORDS 			(3) 
#define MAXIMUM_WRONG_REPEATED_PASSWORDS 	(5)

#define LCD_NUMBER_OF_CHARACTERS 			(16)
#define LOCKED_SCREEN_TIME 					(60)

#define PASSWORD_LENGTH 					(5)

#define OPEN_CLOSE_DOOR_TIME 				(15)
#define WAITING_DOOR_TIME 					(3)

#define KEYPAD_PRESS_TIME 					(350)
#define LCD_WAITING_TIME 					(800)

/***************************************************************
 * 					UART Messages Definitions 				   *
 ***************************************************************/
#define UART_HMI_READY 						0xFF
#define UART_CONTROL_READY 					0xF1

#define UART_OPEN_DOOR 						0xFB
#define UART_DOOR_WAITING 					0xFC
#define UART_CLOSE_DOOR 					0xFD

#define UART_OPERATION_SUCCESS 				0xF2
#define UART_OPERATION_FAIL 				0x2F

#define UART_CHANGE_PASSWORD 				0xF5
#define UART_MAX_WRONG_PASSWORD 			0xF6

#define UART_First_time 					0xF3
#define UART_Not_First_time 				0xF4


/***************************************************************
 * 					User Defined Data Types 				   *
 ***************************************************************/
typedef enum
{
	OPEN_DOOR,
	DOOR_WAITING,
	CLOSE_DOOR,
	IDLE
} DoorState_t;

#endif /* SYS_CONFIG_ */