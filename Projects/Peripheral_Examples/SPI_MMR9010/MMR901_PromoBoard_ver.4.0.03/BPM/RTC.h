//******************************************************************************
/*!
	@file		RTC.h
	@brief		RTC Operate Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __RTC_H__
#define __RTC_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __RTC_C__
	#define EXT_RTC
#else
	#define EXT_RTC extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define RTC_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define RTC_STATE_IDLE			0x01										//!< STATE: IDLE
	#define RTC_STATE_LOAD			0x02										//!< STATE: LOAD
	#define RTC_STATE_SET			0x03										//!< STATE: SET

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================


//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_RTC void rtc_task(void);
	EXT_RTC void rtc_initial(void);
	EXT_RTC void rtc_idle(void);
	EXT_RTC void rtc_load(void);
	EXT_RTC void rtc_set(void);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_RTC system_state_t rtc_ctrl;											//!< STATE MACHINE: RTC TASK control

#endif	//__RTC_H__
//[EOF]
