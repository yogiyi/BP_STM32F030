//******************************************************************************
/*!
	@file		LOG.h
	@brief		LOG Module Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __LOG_H__
#define __LOG_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"
#include "pfdl_types.h"


#ifdef __LOG_C__
	#define EXT_LOG
#else
	#define EXT_LOG extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define LOG_STATE_INITIAL		0x00										//!< STATE: INITIALIZE
	#define LOG_STATE_READY			0x01										//!< STATE: READY
	#define LOG_STATE_READ			0x02										//!< STATE: READ
	#define LOG_STATE_WRITE			0x03										//!< STATE: WRITE
	#define LOG_STATE_VERIFY		0x04										//!< STATE: VERIFY
	#define LOG_STATE_ERROR			0x05										//!< STATE: ERROR
	#define LOG_STATE_ERASE			0x06										//!< STATE: ERASE

	#define LOG_DATA_LENGTH			10											//!< LOG DATA SIZE (sizeof(log_data_t))
#if(1)
	#define LOG_DATA_FIELD			1000										//!< LOG AREA SIZE 0x0000-0x3E7
	#define LOG_DATA_NUMBER			100											//!< =LOG_DATA_FIELD/LOG_DATA_LENGTH
	#define LOG_ERR_START			0x3F0										// 
	#define LOG_ERR_FIELD			13											//!< LOG ERROR AREA SIZE 
#else
	#define LOG_DATA_FIELD			30											//!< LOG AREA SIZE 0x0000-0x3E7
	#define LOG_DATA_NUMBER			3											//!< =LOG_DATA_FIELD/LOG_DATA_LENGTH
	#define LOG_ERR_START			0x3F0										// 
	#define LOG_ERR_FIELD			1											//!< LOG ERROR AREA SIZE
#endif
	#define LOG_NUMBERING_SIZE		128											//!< LOG MANAGEMENT NUMBER SIZE
	#define LOG_INIT_DUTY_ADDR		0x3FF										//!< 

//------------------------------------------------------------------------------
	#define FLASH_BLOCK_SIZE		0x400L										// 標準ブロック・サイズ
	#define FLASH_TARGET_BLOCK		0											// 書き込み開始ブロック(0 : F1000番地)
	#define FLASH_WRITE_SIZE		10											// 書き込みデータサイズ
	#define FLASH_DREAD_OFSET		0x1000										// 直接読み込みオフセット・アドレス

	// PFDL初期設定値
	#define FLASH_FDL_FRQ			24											// 周波数設定(24Mhz)
	#define FLASH_FDL_VOL			0x00										// 電圧モード(フルスピードモード)




//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint8 now;																	//!< Address of the recent data
	uint8 next;																	//!< Address of the next recorded data
	uint8 count;																//!< Number to which MEMORY button is pushed
	struct {
		uint8 full:1;															//!< FLAG: all LOG area is recorded
		uint8 numb:7;															//!< control number
	} BIT;
}log_manage_t;																	//!< log management table

typedef struct{
	uint8 number;																//!< CONTROL NUMBER
	uint8 month;																//!< MONTH
	uint8 day;																	//!< DAY
	uint8 hour;																	//!< HOUR
	uint8 minute;																//!< MINUTE
	uint8 bp_max;																//!< SYSTOLIC BLOOD PRESSURE
	uint8 bp_min;																//!< DIASTOLIC BLOOD PRESSURE
	uint8 rate;																	//!< BEAT RATE
	uint8 age;																	//!< BLOOD VESSEL AGE
	uint8 crc;																	//!< CRC
}log_data_t;																	//!< log data table

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_LOG void log_task(void);
	EXT_LOG void log_initial(void);
	EXT_LOG void log_ready(void);
	EXT_LOG void log_read(void);
	EXT_LOG void log_write(void);
	EXT_LOG void log_verify(void);
	EXT_LOG void log_error(void);
	EXT_LOG void log_erase(void);

	EXT_LOG uint8 crc8_byte(uint8 crc8, uint8 indata);
	EXT_LOG uint8 crc8_block(void *ptr, uint8 len);
	EXT_LOG uint8 crc8_check(void *ptr, uint8 size);

	EXT_LOG pfdl_status_t FLASH_start(void);
	EXT_LOG void FLASH_end(void);
	EXT_LOG uint8 FLASH_read(uint16 addr);
	EXT_LOG void FLASH_write(uint16 addr, uint8 data);
	EXT_LOG void FLASH_erase(void);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_LOG log_manage_t log_info;												//!< LOG management parameter
	EXT_LOG log_data_t   log_data;												//!< LOG data

	EXT_LOG system_state_t log_ctrl;											//!< STATE MACHINE: LOG TASK Control
	EXT_LOG flag8bit_t log_status;												//!< STATUS BIT: LOG status
	#define b_LOG_NO_DATA	log_status.BIT.B0									//!< FLAG: There is no log data.

	EXT_LOG uint8 log_init_duty;												//!< LOG initial duty ratio

#endif	//__LOG_H__
//[EOF]
