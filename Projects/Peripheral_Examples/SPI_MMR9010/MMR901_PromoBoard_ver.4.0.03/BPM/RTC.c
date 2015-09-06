//******************************************************************************
/*!
	@file		RTC.c
	@brief		RTC Operate Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __RTC_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"
#include "r_cg_rtc.h"

#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		RTC Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void rtc_task(void)
{
	switch(rtc_ctrl.state){
		case RTC_STATE_INITIAL:		rtc_initial();		break;
		case RTC_STATE_IDLE:		rtc_idle();			break;
		case RTC_STATE_LOAD:		rtc_load();			break;
		case RTC_STATE_SET:			rtc_set();			break;
		default:										break;
	}
}


/*!
//==============================================================================
	@brief		RTC Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void rtc_initial(void)
{
	R_RTC_Start();
	rtc_ctrl.state = RTC_STATE_IDLE;
	rtc_ctrl.phase = 0x00;
}


/*!
//==============================================================================
	@brief		RTC Idle State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void rtc_idle(void)
{

}



/*!
//==============================================================================
	@brief		RTC Load State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void rtc_load(void)
{
#if(1)	//R_RTC_Get_CounterValue()ÇégópÇµÇ»Ç¢ÅB

	rtc_counter_value_t time;

	switch(rtc_ctrl.phase){
		case 0x00:
			RTCC1 |= _01_RTC_COUNTER_PAUSE;
			rtc_ctrl.phase = 0x01;												// next phase
			break;
		case 0x01:
			if(RWST == 1){
				time.sec = SEC;
				time.min = MIN;
				time.hour = HOUR;
				time.week = WEEK;
				time.day = DAY;
				time.month = MONTH;
				time.year = YEAR;
				bpm_currentTime.year   = utl_bcd2dec(time.year);
				bpm_currentTime.month  = utl_bcd2dec(time.month);
				bpm_currentTime.day    = utl_bcd2dec(time.day);
				bpm_currentTime.hour   = utl_bcd2dec(time.hour);
				bpm_currentTime.minute = utl_bcd2dec(time.min);

				RTCC1 &= (uint8_t)~_01_RTC_COUNTER_PAUSE;
				rtc_ctrl.phase = 0x02;											// next phase
			}
			break;
		case 0x02:
			if(RWST == 0){
				utl_change_state(&rtc_ctrl, RTC_STATE_IDLE);
			}
			break;
		default:
			break;
	}
#else
	MD_STATUS status;
	rtc_counter_value_t time;

	switch(rtc_ctrl.phase){
		case 0x00:
			//
			rtc_ctrl.phase = 0x01;	//next phase
			break;
		case 0x01:
			status = R_RTC_Get_CounterValue(&time);
			if(status == MD_OK){
				bpm_currentTime.year   = utl_bcd2dec(time.year);
				bpm_currentTime.month  = utl_bcd2dec(time.month);
				bpm_currentTime.day    = utl_bcd2dec(time.day);
				bpm_currentTime.hour   = utl_bcd2dec(time.hour);
				bpm_currentTime.minute = utl_bcd2dec(time.min);
				utl_change_state(&rtc_ctrl, RTC_STATE_IDLE);
			}
			//else{
			// retry
			//}
			break;
		default:
			break;
	}
#endif

}


/*!
//==============================================================================
	@brief		RTC Set State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void rtc_set(void)
{
	MD_STATUS status;
	rtc_counter_value_t time;

	switch(rtc_ctrl.phase){
		case 0x00:
			rtc_ctrl.phase = 0x01;	//next phase
			break;
		case 0x01:
			time.year  = utl_dec2bcd(bpm_currentTime.year);
			time.month = utl_dec2bcd(bpm_currentTime.month);
			time.day   = utl_dec2bcd(bpm_currentTime.day);
			time.hour  = utl_dec2bcd(bpm_currentTime.hour);
			time.min   = utl_dec2bcd(bpm_currentTime.minute);
			time.sec   = 0;
			status = R_RTC_Set_CounterValue(time);
			if(status == MD_OK){
				utl_change_state(&rtc_ctrl, RTC_STATE_IDLE);
			}
			//else{
			// retry
			//}
			break;
		default:
			break;
	}
}


//[EOF]
