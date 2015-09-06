//******************************************************************************
/*!
	@file		MMR901XA_SPI.h
	@brief		MMR901XA SPI Module Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __MMR901XA_SPI_H__
#define __MMR901XA_SPI_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
	#include "TYPE.h"

#ifdef __MMR901XA_SPI_C__
	#define EXT_SPI
#else
	#define EXT_SPI extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define P_SPI_SCLK_HIGH		GPIO_SetBits(GPIOB, GPIO_Pin_3)	 												//!< MMR901XA SCLK
	#define P_SPI_SCLK_LOW		GPIO_ResetBits(GPIOB, GPIO_Pin_3)
	
	#define P_SPI_DOUT				GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)														//!< MMR901XA DOUT
	
	
	#define P_SPI_ADRDY				GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)										//!< MMR901XA ADRDY
	
	
	
	#define P_SPI_CS_HIGH			GPIO_SetBits(GPIOB, GPIO_Pin_6)													//!< MMR901XA CS
	#define P_SPI_CS_LOW			GPIO_ResetBits(GPIOB, GPIO_Pin_6)
	
	#define P_SPI_DIN_HIGH		GPIO_SetBits(GPIOB, GPIO_Pin_5)											//!< MMR901XA DIN
	#define P_SPI_DIN_LOW			GPIO_ResetBits(GPIOB, GPIO_Pin_5)

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_SPI void spi_init(void);
	EXT_SPI void spi_init_cs(void);
	EXT_SPI void spi_select_cs(void);
	EXT_SPI void spi_cancel_cs(void);
	EXT_SPI void spi_send_data(uint8 data, uint8 clock);
	EXT_SPI uint8 spi_receive_data8(void);
	EXT_SPI uint16 spi_receive_data16(void);
	EXT_SPI void spi_wait(void);

//==============================================================================
//{### VARIABLES
//==============================================================================


#endif	//__SPI_H__
//[EOF]
