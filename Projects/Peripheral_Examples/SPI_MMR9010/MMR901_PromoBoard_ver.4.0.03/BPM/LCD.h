//******************************************************************************
/*!
	@file		LCD.h
	@brief		LCD Module Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __LCD_H__
#define __LCD_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __LCD_C__
	#define EXT_LCD
#else
	#define EXT_LCD extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define LCD_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define LCD_STATE_CLOCK			0x01										//!< STATE: CLOCK
	#define LCD_STATE_MEASURE		0x02										//!< STATE: MEASURE
	#define LCD_STATE_RESULT		0x03										//!< STATE: RESULT
	#define LCD_STATE_ERROR			0x04										//!< STATE: ERROR
	
	#define LCD_PHASE_CLK_TIME		0x00										//!< 
	#define LCD_PHASE_CLK_HOUR		0x03										//!< 
	#define LCD_PHASE_CLK_MIN		0x04										//!< 

	#define LCD_PHASE_CLK_DATE		0x10										//!< 
	#define LCD_PHASE_CLK_YEAR		0x12										//!< 
	#define LCD_PHASE_CLK_MONTH		0x13										//!< 
	#define LCD_PHASE_CLK_DAY		0x14										//!< 
	
	#define LCD_PHASE_DAT_NONE		0x00										//!< 
	#define LCD_PHASE_DAT_PRESS		0x01										//!< 
	#define LCD_PHASE_DAT_PULSE		0x02										//!< 
	#define LCD_PHASE_DAT_TIME		0x03										//!< 

	#define LCD_MODE_CLK_NONE		0x00										//!< 
	#define LCD_MODE_CLK_UPDATE		0x01										//!< 
	#define LCD_MODE_CLK_BLINK		0x02										//!< 

	#define LCD_MODE_MSR_NONE		0x00										//!< 
	#define LCD_MODE_MSR_UPDATE		0x01										//!< 

	#define LCD_MODE_RSL_NONE		0x00										//!< 
	#define LCD_MODE_RSL_UPDATE		0x01										//!< 

	#define LCD_MODE_ERR_NONE		0x00										//!< 
	#define LCD_MODE_ERR_UPDATE		0x01										//!< 
	#define LCD_MODE_ERR_NOBATT		0x02										//!< 
	#define LCD_MODE_ERR_LOBATT		0x03										//!< 

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint8 year;
	uint8 month;																//!< LOG: 
	uint8 day;																	//!< LOG: 
	uint8 hour;																	//!< LOG: 
	uint8 minute;																//!< LOG: 
	uint8 systolic;																//!< LOG: SYSTOLIC BLOOD PRESSURE
	uint8 diastolic;															//!< LOG: DIASTOLIC BLOOD PRESSURE
	uint8 rate;																	//!< LOG: BEAT RATE
	uint8 age;																	//!< LOG: BLOOD VESSEL AGE
	uint8 press;																//!< MEASURE: PRESS
	uint8 error;																//!< ERROR: ERROR CODE
} lcd_data_t;

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_LCD void lcd_task(void);
	EXT_LCD void lcd_initial(void);

	EXT_LCD void lcd_clock(void);
	EXT_LCD void lcd_measure(void);
	EXT_LCD void lcd_result(void);
	EXT_LCD void lcd_error(void);

	EXT_LCD void lcd_update_module(void);


//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_LCD sint16 lcd_measuring_press;											//!< Pressure for display
	EXT_LCD system_state_t lcd_ctrl;											//!< STATE MACHINE: LCD TASK control
	EXT_LCD lcd_data_t     lcd_data;											//!< LCD display data

	EXT_LCD flag8bit_t lcd_req_period;											//!< 
	#define LCD_REQUEST_P_CLR	(lcd_req_period.BYTE = 0x00)
	#define LCD_REQUEST_P8		lcd_req_period.BIT.B0							//!< 
	#define LCD_REQUEST_P7		lcd_req_period.BIT.B1							//!< 
	#define LCD_REQUEST_P6		lcd_req_period.BIT.B2							//!< 
	#define LCD_REQUEST_P5		lcd_req_period.BIT.B3							//!< 
	#define LCD_REQUEST_P4		lcd_req_period.BIT.B4							//!< 
	#define LCD_REQUEST_P3		lcd_req_period.BIT.B5							//!< 
	#define LCD_REQUEST_P2		lcd_req_period.BIT.B6							//!< 
	#define LCD_REQUEST_P1		lcd_req_period.BIT.B7							//!< 
	
	EXT_LCD flag8bit_t lcd_req_score;											//!< 
	#define LCD_REQUEST_S_CLR	(lcd_req_score.BYTE = 0x00)
	#define LCD_REQUEST_S8		lcd_req_score.BIT.B0							//!< 
	#define LCD_REQUEST_S7		lcd_req_score.BIT.B1							//!< 
	#define LCD_REQUEST_S6		lcd_req_score.BIT.B2							//!< 
	#define LCD_REQUEST_S5		lcd_req_score.BIT.B3							//!< 
	#define LCD_REQUEST_S4		lcd_req_score.BIT.B4							//!< 
	#define LCD_REQUEST_S3		lcd_req_score.BIT.B5							//!< 
	#define LCD_REQUEST_S2		lcd_req_score.BIT.B6							//!< 
	#define LCD_REQUEST_S1		lcd_req_score.BIT.B7							//!< 

	EXT_LCD uint8 lcd_digit[8];
	extern const uint16 LCD_NUMBER[16];
	#define LCD_CHARA_SPACE		10
	#define LCD_CHARA_LETT_E	11
	#define LCD_CHARA_LETT_R	12
	#define LCD_CHARA_HYPHEN	13
	#define LCD_CHARA_LETT_L	14
	#define LCD_CHARA_LETT_O	15

#endif	//__LCD_H__
//[EOF]
