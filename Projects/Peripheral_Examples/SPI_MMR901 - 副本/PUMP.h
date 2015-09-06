//******************************************************************************
/*!
	@file		PUMP.h
	@brief		PUMP Operate Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __PUMP_H__
#define __PUMP_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __PUMP_C__
	#define EXT_PUMP
#else
	#define EXT_PUMP extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define PMP_DUTY_CONTROL_EN		FALSE
	
	#define PMP_DUTY_CYCLE			TDR04										//
	#define PMP_DUTY_SLAVE			TDR05										//
	#define PMP_DUTY_OFF()			(TO0 &= 0xFFDF)								//


	#define PMP_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define PMP_STATE_OFF			0x01										//!< STATE: OFF
	#define PMP_STATE_ON			0x02										//!< STATE: ON

	#define PMP_DUTY_RATIO_100		200											// 0.5% per unit
	#define PMP_DUTY_RATIO_START	80											// 0.5% per unit
	#define PMP_DUTY_RATIO_STOP		20											// 0.5% per uint
	#define PMP_DUTY_RATIO_RATE		1											// 0.5% per uint


//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint16 pulseCycle;
	uint16 slavePeriod;
} pump_manage_t;

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_PUMP void pump_task(void);
	EXT_PUMP void pump_initial(void);
	EXT_PUMP void pump_off(void);
	EXT_PUMP void pump_on(void);
	EXT_PUMP void GPIO_pump_INT(void);


//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_PUMP system_state_t pmp_ctrl;											//!< STATE MACHINE: PUMP TASK control
	EXT_PUMP pump_manage_t  pmp_manage;											//!< 

#endif	//__PUMP_H__
//[EOF]
