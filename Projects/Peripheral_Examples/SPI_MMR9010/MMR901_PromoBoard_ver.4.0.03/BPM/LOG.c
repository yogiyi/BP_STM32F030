//******************************************************************************
/*!
	@file		LOG.c
	@brief		LOG Module Function Source File
				
				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __LOG_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"

/* フラッシュ・セルフ・プログラミング・ライブラリ */
#include "pfdl.h"                /* ライブラリ・ヘッダーファイル             */
#include "pfdl_types.h"          /* ライブラリ・ヘッダーファイル             */

#include "common.h"


//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		LOG Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This function is the main task to manage the measured data.\n
	The process is controlled by the state parameter "log_ctrl.state".

//==============================================================================
*/
void log_task(void)
{
	switch(log_ctrl.state){
		case LOG_STATE_INITIAL:		log_initial();	break;
		case LOG_STATE_READY:		log_ready();	break;
		case LOG_STATE_READ:		log_read();		break;
		case LOG_STATE_WRITE:		log_write();	break;
		case LOG_STATE_VERIFY:		log_verify();	break;
		case LOG_STATE_ERROR:		log_error();	break;
		case LOG_STATE_ERASE:		log_erase();	break;
		default:									break;
	}
}



/*!
//==============================================================================
	@brief		LOG Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module searches for the address where the newest LOG is recorded,\n
	and is updated the LOG management parameter by the address.\n
	The record in EEPROM, it is classified as follows.
	-(A) Not overwriting.
	-(B) The newest LOG data is located the overwritten area.
	-(C) The newest LOG data is located the last address in LOG area.
	LOG management parameter:\n
	- log_info.now: the address of the newest LOG data
	- log_info.next: the address of next recorded LOG data
	- log_info.count: the number of display is updated (0~LOG_DATA_NUMBER)
	- log_info.BIT.full: 0=not overwrite, 1=overwriting
	- log_info.BIT.numb: the management number (0~LOG_NUMBERING_SIZE)
	- LOG area: 0x00-0xEF
	@note
	The adjustment data of solenoid valve is set to the address "0xFF" of EEPROM.

//==============================================================================
*/
void log_initial(void)
{
	uint8 cnt;
	uint8 temp;
	uint8 err_num;
	uint8 old_num, old_cnt;
	uint8 ii;
	uint8 addr;
	uint8 data[LOG_DATA_LENGTH];
	uint8 ok_size;
	
	FLASH_start();																//

	// read initial duty ratio
	log_init_duty = FLASH_read(LOG_INIT_DUTY_ADDR);
	
	// check Error Log Area
	err_num = 0;
	for(cnt=0; cnt<LOG_ERR_FIELD; cnt++){
		temp = FLASH_read(LOG_ERR_START+cnt);
		for(ii=0; ii<8; ii++){
			if((temp&0x01)==0x00){
				err_num++;
			}
			temp>>=1;
		}
	}
	
	ok_size = LOG_DATA_NUMBER - err_num;

	old_cnt = 0xFF;
	old_num = 0x00;
	// search for the address where the newest LOG is recorded
	for(cnt=0; cnt<LOG_DATA_NUMBER; cnt++){

		// read LOG data
		addr = LOG_DATA_LENGTH * cnt;
		for(ii=0; ii<LOG_DATA_LENGTH; ii++){
//			data[ii] = eeprom_read(addr+ii);
			data[ii] = FLASH_read(addr+ii);
		}

		// 
		if(data[0]==0xFF){
			// A pattern
			log_info.now  = old_cnt;
			log_info.next = cnt;
			log_info.BIT.full = 0;
			if(cnt==0){
				log_info.BIT.numb = 0x7F;
			}else{
				log_info.BIT.numb = old_num;
			}
			break;
		}

		// check the CRC-8
		if(crc8_check(&data, LOG_DATA_LENGTH) == TRUE){							// CRC OK
			if(	(data[0] + ok_size > old_num) &&
				(data[0]           < old_num)){
				// B-1 pattern
				log_info.now  = old_cnt;
				log_info.next = cnt;
				log_info.BIT.full = 1;
				log_info.BIT.numb = old_num;
				break;
			}
			if(data[0] > old_num + LOG_NUMBERING_SIZE - ok_size){
				// B-2 pattern
				log_info.now  = old_cnt;
				log_info.next = cnt;
				log_info.BIT.full = 1;
				log_info.BIT.numb = old_num;
				break;
			}
			old_num = data[0];
			old_cnt = cnt;
		}
	}
	if(cnt == LOG_DATA_NUMBER){
		// C pattern
		log_info.now  = old_cnt;
		log_info.next = 0x00;
		log_info.BIT.full = 1;
		log_info.BIT.numb = old_num;
	}
	FLASH_end();	log_status.BYTE = 0x00;
	log_info.count = 0x00;
	log_ctrl.state = LOG_STATE_READY;
}


/*!
//==============================================================================
	@brief		LOG Ready State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	None operation.

//==============================================================================
*/
void log_ready(void)
{

}


/*!
//==============================================================================
	@brief		LOG Read State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This state reads the specified LOG data in EEPROM.
	- log_ctrl.mode=0 : LOG data is the latest data
	- log_ctrl.mode=1 : LOG data is the next data
	By checking CRC-8, the state parameter is changed as follows.
	- OK : to LOG_STATE_READY(Update or No data)
	- NG : retry

//==============================================================================
*/
void log_read(void)
{
	uint8 addr;
	uint8 remain;

	if(log_ctrl.mode == 0x01){
		log_info.count++;
	}else{
		log_info.count = 0x00;
	}

	if(log_info.now == 0xFF){													// No data in EEPROM
		b_LOG_NO_DATA = ON;
		log_ctrl.state = LOG_STATE_READY;										// to READY state
		return;
	}

	// ring counter
	if(log_info.count >= LOG_DATA_NUMBER){
		log_info.count -= LOG_DATA_NUMBER;
	}

	// calculate the address of the request LOG
	if(log_info.count <= log_info.now){
		addr = log_info.now - log_info.count;
	}else{
		remain = log_info.count - log_info.now - 1;
		if(log_info.BIT.full){													// overwriting
			addr = (LOG_DATA_NUMBER -1) - remain;
		}else{																	// not overwriting
			log_info.count = remain;
			addr = log_info.now - remain;
		}
	}

	FLASH_start();

	// read the requested data from DATA FLASH
	addr *= LOG_DATA_LENGTH;
	log_data.number = FLASH_read(addr);
	log_data.month  = FLASH_read(addr+1);
	log_data.day    = FLASH_read(addr+2);
	log_data.hour   = FLASH_read(addr+3);
	log_data.minute = FLASH_read(addr+4);
	log_data.bp_max = FLASH_read(addr+5);
	log_data.bp_min = FLASH_read(addr+6);
	log_data.rate   = FLASH_read(addr+7);
	log_data.age    = FLASH_read(addr+8);
	log_data.crc    = FLASH_read(addr+9);

	FLASH_end();

	// check CRC-8
	if(crc8_check(&log_data, LOG_DATA_LENGTH) == TRUE){
		log_ctrl.state = LOG_STATE_READY;										// to READY state
		b_LOG_NO_DATA = OFF;
	}

	// else retry

}


/*!
//==============================================================================
	@brief		LOG Write State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This state records LOG information at a specified address.
	After recording, the state prameter is changed into VERIFY state.

//==============================================================================
*/
void log_write(void)
{
	uint8 addr;

	// calculate the next writing address
	addr = log_info.next;
	addr *= LOG_DATA_LENGTH;

	// calculate LOG number
	log_data.number = (uint8)log_info.BIT.numb + 1;								// 
	if(log_data.number == LOG_NUMBERING_SIZE){
		log_data.number = 0;
	}

	FLASH_start();

	// write the LOG to DATA FLASH
	FLASH_write(addr,   log_data.number);
	FLASH_write(addr+1, log_data.month);
	FLASH_write(addr+2, log_data.day);
	FLASH_write(addr+3, log_data.hour);
	FLASH_write(addr+4, log_data.minute);
	FLASH_write(addr+5, log_data.bp_max);
	FLASH_write(addr+6, log_data.bp_min);
	FLASH_write(addr+7, log_data.rate);
	FLASH_write(addr+8, log_data.age);
	log_data.crc = crc8_block(&log_data,LOG_DATA_LENGTH-1);
	FLASH_write(addr+9, log_data.crc);

	FLASH_end();
	

	// update the LOG management parameter
	log_info.count = 0x01;
	log_info.now = log_info.next;
	log_info.next++;

	// check the free spece
	if(log_info.next == LOG_DATA_NUMBER){
		log_info.next = 0x00;
		if(!log_info.BIT.full){
			log_info.BIT.full = 1;
		}
	}

	log_ctrl.state = LOG_STATE_VERIFY;											// to VERIFY state

}


/*!
//==============================================================================
	@brief		LOG Verify State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This state verifies the recorded LOG and the recording LOG.
	By the result of checking EEPROM, the state parameter is changed as follows.
	- OK : to READY state
	- NG : to ERROR state

//==============================================================================
*/
void log_verify(void)
{
	uint8 addr;
	uint8 data;
	uint8 *p_log;
	uint8 ii = LOG_DATA_LENGTH;

	// calculate the addres of the newest LOG
	p_log = (uint8 *)&log_data;
	addr = LOG_DATA_LENGTH * log_info.now;										// 

	FLASH_start();

	// verfy the LOG
	do{
		data = FLASH_read(addr++);												// read DATA FLASH
		if(data != *p_log++){
			log_ctrl.state = LOG_STATE_ERROR;									// to ERROR state
			return;
		}
	}while(--ii);

	FLASH_end();
	
	// OK: update the LOG management parameter
	b_LOG_NO_DATA = OFF;
	log_info.BIT.numb = log_data.number;										// 
	log_ctrl.state = LOG_STATE_READY;											// to READY state
}


/*!
//==============================================================================
	@brief		LOG Error State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This state records the position of the error LOG and requests to rewrite.

//==============================================================================
*/
void log_error(void)
{
	uint16 addr;
	uint8 mask;
	uint8 data;

	FLASH_start();

	addr = log_info.now/8;
	addr += LOG_DATA_FIELD;
	
	data = FLASH_read(addr);													// read DATA FLASH
	mask = 1;
	mask <<= (log_info.now%8);
	
	// The bit that corresponds to the error area is cleared.
	data &= ~mask;

	FLASH_write(addr, data);

	FLASH_end();
	
	log_ctrl.state = LOG_STATE_WRITE;											// to WRITE state

}


/*!
//==============================================================================
	@brief		LOG Erase State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This state deletes all LOG in EEPROM.
	@note
	The place to erase is changed into LOG DATA AREA and LOG MANAGEMENT AREA from all areas.

//==============================================================================
*/
void log_erase(void)
{
	uint16 cnt;

	FLASH_start();																//
	
	// erase
#if(1)
    FLASH_erase();

#else
	for(cnt=0; cnt < LOG_DATA_FIELD; cnt++){									// LOG DATA FIELD
		FLASH_write(cnt, 0xFF);
	}
	for(cnt=0; cnt<LOG_ERR_FIELD; cnt++){										// LOG ERROR FIELD
		FLASH_write(LOG_ERR_START + cnt, 0xFF);
	}
#endif
	FLASH_end();																//

	// initialize the LOG Management parameter
	log_info.now  = 0xFF;
	log_info.next = 0x00;
	log_info.BIT.full = 0;
	log_info.BIT.numb = 0;
	log_info.count = 0x00;

	// clear LOG data
	utl_memory_clr(&log_data, sizeof(log_data));								// 

	b_LOG_NO_DATA = ON;
	log_ctrl.state = LOG_STATE_READY;
}


/*!
//==============================================================================
	@brief		LOG Calculation CRC-8 Module

	@param		crc8   : Last CRC-8 data
	@param		indata : New data
	@return		data   : CRC-8 data
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	CRC-8 is defined to be x^8 + x^2 + x + 1.\n
	To use this function use the following templete:\n
	 crc = crc8_byte(crc, data);

//==============================================================================
*/
uint8 crc8_byte(uint8 crc8, uint8 indata)
{
	uint8 ii;
	uint8 data;

	data = (uint8)(crc8^indata);
	for(ii=0; ii<8; ii++){
		if((data&0x80) != 0){
			data <<= 1;
			data ^= 0x07;
		}else{
			data <<= 1;
		}
	}
	return data;
}


/*!
//==============================================================================
	@brief		LOG Calculation CRC-8 over a block of memory Module

	@param		*ptr : Start pointer
	@param		len  : Block size
	@return		crc8 : CRC-8 data
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module caluclates the CRC-8 of the "len" size from the first address "*ptr".

//==============================================================================
*/
uint8 crc8_block(void *ptr, uint8 len)
{
	uint8 crc8=0;
	uint8 *pdata;

	pdata = ptr;
	while(len>0){
		crc8 = crc8_byte(crc8, *pdata++);
		len--;
	}
	return crc8;
}


/*!
//==============================================================================
	@brief		LOG CRC Check Module

	@param		*ptr  : Start pointer
	@param		size  : Block size
	@retval		TRUE  : OK
	@retval		FALSE : NG
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	The CRC-8 calclation result is compared with the value of the recorded memory,\n
	and "TRUE" is returned when these are matching.
	@note
	The comparison value of CRC must be set to the final address of the checked memory.

//==============================================================================
*/
uint8 crc8_check(void *ptr, uint8 size)
{
	uint8 crc8;
	uint8 *pdata;
	
	pdata = ptr;
	crc8 = crc8_block(ptr, size-1);
	if(crc8 == *(pdata+size-1)){
		return TRUE;
	}else{
		return FALSE;
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//
//{### SUB MODULE FUNCTIONS
//
//******************************************************************************
//******************************************************************************
//******************************************************************************
/*!
//==============================================================================
	@brief		FLASH START Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
pfdl_status_t FLASH_start(void)
{
	pfdl_status_t       dtyFdlResult;
	pfdl_descriptor_t   dtyDescriptor;
	
	dtyDescriptor.fx_MHz_u08            = FLASH_FDL_FRQ;						// 周波数設定
	dtyDescriptor.wide_voltage_mode_u08 = FLASH_FDL_VOL;						// 電圧モード設定
	
	dtyFdlResult = PFDL_Open( &dtyDescriptor );									// 初期化関数実行
	
	return dtyFdlResult;
}

/*!
//==============================================================================
	@brief		FLASH END Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void FLASH_end(void)
{
	/* フラッシュ・セルフ・プログラミング・ライブラリ終了処理 */
	PFDL_Close();
}

/*!
//==============================================================================
	@brief		FLASH READ Module

	@param		uint16
	@return		uint8
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
uint8 FLASH_read(uint16 addr)
{
	pfdl_status_t   dtyFdlResult;												// リターン値
	pfdl_request_t  dtyRequester;												// PFDL制御用変数(リクエスタ)
	pfdl_u08        dubReadBuffer;												// データ読み込み用バッファ
	
	dtyRequester.index_u16     = addr;											// Readコマンド用のパラメータを設定
	dtyRequester.data_pu08     = &dubReadBuffer;								// リード用バッファに切り替え
	dtyRequester.bytecount_u16 = 1;
	dtyRequester.command_enu   = PFDL_CMD_READ_BYTES;							// Readコマンドを設定

	dtyFdlResult = PFDL_Execute( &dtyRequester );								// Readコマンド実行処理

	return dubReadBuffer;
}

/*!
//==============================================================================
	@brief		FLASH WRITE Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void FLASH_write(uint16 addr, uint8 data)
{
	pfdl_status_t   dtyFdlResult;												// リターン値
	pfdl_request_t  dtyRequester;												// PFDL制御用変数(リクエスタ)

	dtyRequester.index_u16     = addr;
	dtyRequester.data_pu08     = &data;
	dtyRequester.bytecount_u16 = 1;
	dtyRequester.command_enu   = PFDL_CMD_WRITE_BYTES;


	dtyFdlResult = PFDL_Execute( &dtyRequester );								// コマンド実行処理
	
	while( dtyFdlResult == PFDL_BUSY ){											// コマンド終了待ち
		/* 何かしたい作業があれば行う(バック・グラウンド・オペレーション) */
		NOP();
		NOP();
		dtyFdlResult = PFDL_Handler();											// 終了確認
	}
}



/*!
//==============================================================================
	@brief		FLASH ERASE Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void FLASH_erase(void)
{
	pfdl_status_t   dtyFdlResult;												// リターン値
	pfdl_request_t  dtyRequester;												// PFDL制御用変数(リクエスタ)


	dtyRequester.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;						// ブランクチェックコマンド

	dtyRequester.index_u16     = FLASH_TARGET_BLOCK * FLASH_BLOCK_SIZE;			// 開始アドレスセット
	dtyRequester.bytecount_u16 = FLASH_BLOCK_SIZE;								// チェックレンジセット

	dtyFdlResult = PFDL_Execute( &dtyRequester );								// コマンド実行処理

	while( dtyFdlResult == PFDL_BUSY ){											// コマンド終了待ち
		NOP();
		NOP();
		dtyFdlResult = PFDL_Handler();											// 終了確認処理
	}
	

	if( dtyFdlResult == PFDL_ERR_MARGIN ){
		dtyRequester.command_enu = PFDL_CMD_ERASE_BLOCK;						// 対象ブロックイレースコマンド
		dtyRequester.index_u16   = FLASH_TARGET_BLOCK;							// 対象ブロック番号セット

		dtyFdlResult = PFDL_Execute( &dtyRequester );							// コマンド実行処理
		
		while( dtyFdlResult == PFDL_BUSY ){										// コマンド終了待ち
			NOP();
			NOP();
			dtyFdlResult = PFDL_Handler();										// 終了確認処理
		}
	}
}






//[EOF]
