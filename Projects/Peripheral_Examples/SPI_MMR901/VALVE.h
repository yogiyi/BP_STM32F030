//******************************************************************************
/*!
	@file		VALVE.h
	@brief		VALVE Operate Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __VALVE_H__
#define __VALVE_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __VALVE_C__
	#define EXT_VALVE
#else
	#define EXT_VALVE extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define VLV_DUTY_CYCLE			TDR00										//!< 
	
	
//yy	#define VLV_DUTY_SLAVE			TDR01										//!< 
	#define VLV_DUTY_SLAVE	TIM3->CCR1
	
	
	#define VLV_DUTY_OFF()			(TO0 &= 0xFFFD)								//!< 

	#define VLV_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define VLV_STATE_OPEN			0x01										//!< STATE: OPEN
	#define VLV_STATE_CLOSE			0x02										//!< STATE: CLOSE
	#define VLV_STATE_LEAK			0x03										//!< STATE: LEAK

	#define VLV_DUTY_RATIO_100		1000										//!< 0.1% per unit
//	#define VLV_DUTY_RATIO_START	350											//!< 0.1% per unit
		#define VLV_DUTY_RATIO_START	300	
//YY 	#define VLV_DUTY_RATIO_START	300											//!< 0.1% per unit
//	#define VLV_DUTY_RATIO_STOP		250											//!< 0.1% per uint
	#define VLV_DUTY_RATIO_STOP		180											//!< 0.1% per uint
	
	#define VLV_DUTY_RATIO_RATE		1
	
	
//	#define VLV_DUTY_RATIO_RATE		1											//!< 0.1% per uint

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint16 pulseCycle;
	uint16 slavePeriod;
	uint16 dutyRatio;
} valve_manage_t;

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_VALVE void valve_task(void);
	EXT_VALVE void valve_initial(void);
	EXT_VALVE void valve_open(void);
	EXT_VALVE void valve_close(void);
	EXT_VALVE void valve_leak(void);
	EXT_VALVE	void TIM3_vlv_Config(void);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_VALVE system_state_t vlv_ctrl;											//!< STATE MACHINE: VALVE TASK control
	EXT_VALVE valve_manage_t vlv_manage;										//!< 


#endif	//__VALVE_H__
//[EOF]
