//******************************************************************************
/*!
	@file		MMR901XA_CMD.h
	@brief		MMR901XA Command Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __MMR901XA_CMD_H__
#define __MMR901XA_CMD_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __MMR901XA_CMD_C__
	#define EXT_CMD
#else
	#define EXT_CMD extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define CMD_MEAS_TEMP_ONE			0x03									//!< Command: Measure Temperature (one shot)
	#define CMD_READ_EEPROM				0x04									//!< Command: Read EEPROM Register
	#define CMD_MEAS_PRES_ONE			0x05									//!< Command: Measure Pressure (one shot)
	#define CMD_LOAD_EEPROM				0x06									//!< Command: Load EEPROM
	#define CMD_SET_DELAY				0x07									//!< Command: Set Delay(n<=3)
	#define CMD_MEAS_TEMP_SEQ			0x08									//!< Command: Measure Temperature (sequential)
	#define CMD_MEAS_PRES_SEQ			0x09									//!< Command: Measure Pressure (sequential)
	#define CMD_STOP					0x0A									//!< Command: STOP
	#define CMD_READ_ADC				0x0C									//!< Command: Read ADC

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_CMD uint16 cmd_exec_spi_protocol(uint8 inst, uint8 param);

//==============================================================================
//{### VARIABLES
//==============================================================================


#endif	//__CMD_H__
//[EOF]
