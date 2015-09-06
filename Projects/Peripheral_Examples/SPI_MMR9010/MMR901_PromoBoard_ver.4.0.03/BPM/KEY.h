//******************************************************************************
/*!
	@file		KEY.h
	@brief		KEY Operate Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __KEY_H__
#define __KEY_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __KEY_C__
	#define EXT_KEY
#else
	#define EXT_KEY extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define P_KEY_START			P14.0											//!< START  BTN
	#define P_KEY_MEMORY		P14.1											//!< MEMORY BTN
	#define P_KEY_SELECT		P3.1											//!< SELECT BTN
//	#define P_KEY_START			INTP6											//!< START  BTN
//	#define P_KEY_MEMORY		INTP7											//!< MEMORY BTN
//	#define P_KEY_SELECT		INTP3											//!< SELECT BTN

	#define KEY_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define KEY_STATE_ENABLE		0x01										//!< STATE: ENABLE
	#define KEY_STATE_DISABLE		0x02										//!< STATE: DISABLE

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint16		countdown5ms;
	uint16		interval5ms;
	flag8bit_t	event;
	flag8bit_t	status;
} key_manage_t;


//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_KEY void key_task(void);

	EXT_KEY void key_initial(void);
	EXT_KEY void key_enable(void);
	EXT_KEY void key_disable(void);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_KEY system_state_t key_ctrl;												//!< STATE MACHINE: KEY TASK control
	
	EXT_KEY key_manage_t key_manage;
	#define KEY_EVENT_CLR			(key_manage.event.BYTE=0x00)					//!< all clear
	#define b_KEY_EVENT_START		key_manage.event.BIT.B0							//!< FLAG: START
	#define b_KEY_EVENT_MEMORY		key_manage.event.BIT.B1							//!< FLAG: MEMORY
	#define b_KEY_EVENT_SELECT		key_manage.event.BIT.B2							//!< FLAG: SELECT
//	#define b_KEY_EVENT_ADJUST		key_manage.event.BIT.B3							//!< FLAG: ADJUST
	#define b_KEY_EVENT_ERASE		key_manage.event.BIT.B4							//!< FLAG: ERASE

	#define KEY_STATUS_CLR			(key_manage.status.BYTE=0x00)					//!< all clear
	#define b_KEY_STATUS_START		key_manage.status.BIT.B0						//!< FLAG: START
	#define b_KEY_STATUS_MEMORY		key_manage.status.BIT.B1						//!< FLAG: MEMORY
	#define b_KEY_STATUS_SELECT		key_manage.status.BIT.B2						//!< FLAG: SELECT
	#define b_KEY_STATUS_ADJUST		key_manage.status.BIT.B3						//!< FLAG: ADJUST
	#define b_KEY_STATUS_ERASE		key_manage.status.BIT.B4						//!< FLAG: ERASE
	
#endif	//__KEY_H__
//[EOF]
