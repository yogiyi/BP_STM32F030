//******************************************************************************
/*!
	@file		UTL.c
	@brief		Utilty Calculate Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __UTL_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "common.h"

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		Rounds Data(32bit) Module

	@param		s32   : Rounded value (32 bits)
	@param		val   : Rounding value (31 bits/ 0<val<2^31)
	@return		s32/val
	@author		k.kurata
	@date		create: 2012.07.20
	@date		update: 2013.02.13
	
//==============================================================================
	@par		Operations Description:
	This module rounds off "s32" by "val".\n

//==============================================================================
*/
sint32 utl_round32(sint32 s32, sint32 val)
{
	// Rounding
	if (s32 > 0){																// positive case
		s32 += (val/2);
	}else{																		// negative case
		s32 -= (val/2);
	}

	return (s32/val);															// round off
}


/*!
//==============================================================================
	@brief		Convert HEX to ASCII module

	@param		data    : HEX data
	@param		digit   : Flag that select high or low order bit
	@return		dataout : ASCII data
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module converts HEXADECIMAL data to ASCII data.\n
	The converted digit of "data" is decided by "digit".

//==============================================================================
*/
uint8 utl_hex2ascii(uint8 data, uint8 digit)
{
	uint8 dataout;
	
	if(digit==0x10)	data = (uint8)(data>>4  );									// High-order bits
	else			data = (uint8)(data&0x0F);									// Low-order bits
	
	if(data<= 0x09){
		dataout = (uint8)(data + 0x30);											// 0-9
	}else{
		dataout = (uint8)(data + 0x37);											// A-F
	}
	return(dataout);

}


/*!
//==============================================================================
	@brief		Convert ASCII to HEX module

	@param		str : ASCII data
	@return		0x0~0xF : HEX data
	@return		0xFF    : Illegal data or NULL
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
uint8 utl_ascii2hex(uint8 str)
{
	if(str >= '0' && str <= '9'){
		str -= '0';
	}else if(str >= 'a' && str <= 'f') {
		str -= 'a';
		str += 10;
	}else if(str >= 'A' && str <= 'F') {
		str -= 'A';
		str += 10;
	}else{
		str = 0xFF;
	}
	return(str);
}

/*!
//==============================================================================
	@brief		Convert BCD to DECIMAL module

	@param		bcd : bcd code(0-99)
	@return		result : decimal data
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
uint8 utl_bcd2dec(uint8 bcd)
{
	uint8 result;

	result = bcd >> 4;
	result *= 10;
	result += bcd & 0x0f;

	return result;
}

/*!
//==============================================================================
	@brief		Convert DECIMAL to BCD code module

	@param		dec : decimal(0-99)
	@return		result : BCD code
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
uint8 utl_dec2bcd(uint8 dec)
{
	uint8 result;

	result = dec / 10;
	result <<= 4;
	result += dec % 10;

	return result;
}

/*!
//==============================================================================
	@brief		Memory Clear Module

	@param		*addr : first address
	@param		size  : data size
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module clears the specified size from the first address "*addr".

//==============================================================================
*/


/*
void utl_memory_clr(void *addr, uint16 size)
{
	uint32 *lp;
	uint8  *cp;
	uint8 l_size, cnt;
	
	lp = (uint32 *)addr;
	l_size = (uint8)(size >> 2);
	for(cnt=0; cnt<l_size; cnt++){
		*lp++ = 0x00000000;

	}

	cp = (uint8 *)lp;
	l_size = (uint8)(size & 0x03);
	for(cnt=0; cnt<l_size; cnt++){
		*cp++ = 0x00;
		
	}
	
}

*/


void utl_memory_clr(void *addr, uint16 size)
{
	uint16 *lp;
	uint8  *cp;
	uint8 l_size, cnt;
	
	lp = (uint16 *)addr;
	l_size = (uint8)(size >> 1);
	for(cnt=0; cnt<l_size; cnt++){
		*lp++ = 0x0000;

	}

	cp = (uint8 *)lp;
	l_size = (uint8)(size & 0x01);
	for(cnt=0; cnt<l_size; cnt++){
		*cp++ = 0x00;
		
	}
	
}
























/*!
//==============================================================================
	@brief		Change State Module

	@param		*system : State management variables
	@param		state   : Target State
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module resets the state parameters.

//==============================================================================
*/
void utl_change_state(system_state_t *system, uint8 state)
{
	system->state = state;
	system->phase = 0x00;
	system->count = 0x00;
}

//[EOF]
