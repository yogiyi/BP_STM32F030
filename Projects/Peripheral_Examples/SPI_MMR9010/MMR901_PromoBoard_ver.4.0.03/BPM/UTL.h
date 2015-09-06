//******************************************************************************
/*!
	@file		UTL.h
	@brief		Utility Calculate Function Header File
				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __UTL_H__
#define __UTL_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __UTL_C__
	#define EXT_UTL
#else
	#define EXT_UTL extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_UTL sint32 utl_round32(sint32 s32, sint32 val);
	EXT_UTL uint8 utl_hex2ascii(uint8 data, uint8 digit);
	EXT_UTL uint8 utl_ascii2hex(uint8 str);
	EXT_UTL uint8 utl_bcd2dec(uint8 bcd);
	EXT_UTL uint8 utl_dec2bcd(uint8 dec);

	EXT_UTL void utl_memory_clr(void *addr, uint16 size);
	EXT_UTL void utl_change_state(system_state_t *system, uint8 state);

//==============================================================================
//{### VARIABLES
//==============================================================================



#endif	//__UTL_H__

//[EOF]
