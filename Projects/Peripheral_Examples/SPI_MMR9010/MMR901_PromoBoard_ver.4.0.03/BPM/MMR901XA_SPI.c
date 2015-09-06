//******************************************************************************
/*!
	@file		MMR901XA_SPI.c
	@brief		MMR901XA SPI Module Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __MMR901XA_SPI_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		SPI Interface Initialize Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	none operation.

	@note
	If CS is High when you turn on the power supply, MMR901XA is supllied the power from CS.\n
	CS is High at "spi_init_cs()" function.

//==============================================================================
*/
void spi_init(void)
{
	// port_init()@CPU.c

}


/*!
//==============================================================================
	@brief		SPI CS Terminal Initialize Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module makes CS High.

//==============================================================================
*/
void spi_init_cs(void)
{
	P_SPI_SCLK = LOW;
	spi_wait();
	spi_wait();
	P_SPI_CS = HIGH;
}


/*!
//==============================================================================
	@brief		SPI Select MMR901XA

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module makes CS Low.
	@note
	It is necessary for the falling edge of CS that SCLK is Low.

//==============================================================================
*/
void spi_select_cs(void)
{
	P_SPI_SCLK = LOW;
	spi_wait();
	P_SPI_CS = LOW;
	spi_wait();
}


/*!
//==============================================================================
	@brief		SPI Cancel MMR901XA

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module makes CS High.
	@note
	It is necessary for the rising edge of CS that SCLK is Low.

//==============================================================================
*/
void spi_cancel_cs(void)
{
	P_SPI_SCLK = LOW;
	spi_wait();
	P_SPI_CS = HIGH;
	spi_wait();
}


/*!
//==============================================================================
	@brief		SPI Send Data Module

	@param		data  : Transfer data
	@param		clock : Number of serial clock
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module transmits the data of the number of specified clocks from MSB.

//==============================================================================
*/
void spi_send_data(uint8 data, uint8 clock)
{
    uint8 cnt;
    uint8 sbit;

    for(cnt = 0; cnt < clock; cnt++) {
		sbit = (uint8)(data & 0x80);
		if (sbit == 0x80){
			P_SPI_DIN = HIGH;
		}else{
			P_SPI_DIN = LOW;
		}
		spi_wait();
		P_SPI_SCLK = HIGH;
		spi_wait();
		P_SPI_SCLK = LOW;
		data = (uint8)(data << 1);
	}
}


/*!
//==============================================================================
	@brief		SPI Receive Data(8bit) Module

	@param		void
	@return		data : Received data(8bit)
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module receives the SPI data of 8 bits from MSB.

//==============================================================================
*/
uint8 spi_receive_data8(void)
{
	uint8 cnt;
	uint8 rbit;
	uint8 data;

	data = 0;

	for(cnt = 0; cnt < 8; cnt++){
		P_SPI_SCLK = HIGH;
		data = data << 1;
		rbit = P_SPI_DOUT;
		if(rbit == HIGH){
			data |= 1;
		}
		spi_wait();
		P_SPI_SCLK = LOW;
		spi_wait();
	}

	return data;

}


/*!
//==============================================================================
	@brief		SPI Receive Data(16bit) Module

	@param		void
	@return		data : Received data(16bit)
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module receives the SPI data of 16 bits from MSB.

//==============================================================================
*/
uint16 spi_receive_data16(void)
{
	uint8 cnt;
	uint8 rbit;
	uint16 data;

	data = 0;

	// make SCLK Low
	P_SPI_SCLK = LOW;
	spi_wait();

	for(cnt = 0; cnt < 16; cnt++){
		// Low --> High
		P_SPI_SCLK = HIGH;

		// get
		data = data << 1;

		rbit = P_SPI_DOUT;
		if(rbit == HIGH){
			data |= 1;
		}

		spi_wait();

		//  High --> Low
		P_SPI_SCLK = LOW;
		spi_wait();

	}
	return data;

}


/*!
//==============================================================================
	@brief		SPI Wait Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module waits for 200ns or more.

//==============================================================================
*/
void spi_wait(void)
{
	NOP();
	NOP();
}


//[EOF]
