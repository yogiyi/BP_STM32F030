//******************************************************************************
/*!
	@file		MMR901XA_CMD.c
	@brief		MMR901XA Command Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __MMR901XA_CMD_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		SPI Command Execute module

	@param		inst  : SPI Instruction Code
	@param		param : Input Pattern Data
	@return		rdata : Output Data
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This function executes the SPI command. 

//==============================================================================
*/
uint16 cmd_exec_spi_protocol(uint8 inst, uint8 param)
{
	uint8 tdata;
	uint16 rdata;
	
	tdata = (inst<<4);
	tdata |= param;
	rdata = 0x00;
	spi_select_cs();
	switch(inst){
		case CMD_READ_EEPROM:													// '4'
			spi_send_data(tdata, 0x08);											// 8
			spi_send_data(0x00, 0x01);											// 9=8+1
			rdata = (uint16)spi_receive_data8();								// 17=9+8
			break;
		case CMD_MEAS_TEMP_ONE:													// '3'
		case CMD_MEAS_PRES_ONE:													// '5'
		case CMD_LOAD_EEPROM:													// '6'
		case CMD_MEAS_TEMP_SEQ:													// '8'
		case CMD_MEAS_PRES_SEQ:													// '9'
		case CMD_STOP:															// 'A'
			spi_send_data(tdata, 0x04);											// 4
			spi_send_data(0x00, 0x03);											// 7=4+3
			break;
		case CMD_READ_ADC:														// 'C'
			spi_send_data(tdata, 0x04);											// 4
			spi_send_data(0x00, 0x02);											// 6=4+2
			rdata = spi_receive_data16();										// 22=6+16
			break;
		case CMD_SET_DELAY:														// '7'
			spi_send_data(tdata, 0x08);											// 8
			spi_send_data(0x00, 0x01);											// 9=8+1
			break;
		default:
			break;
	}
	spi_cancel_cs();
	return rdata;
}

//[EOF]
