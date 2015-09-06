//******************************************************************************
/*!
	@file		VALVE.c
	@brief		VALVE Operate Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __VALVE_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"

#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		VALVE Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void valve_task(void)
{
	switch(vlv_ctrl.state){
		case VLV_STATE_INITIAL:		valve_initial();	break;
		case VLV_STATE_OPEN:		valve_open();		break;
		case VLV_STATE_CLOSE:		valve_close();		break;
		case VLV_STATE_LEAK:		valve_leak();		break;
		default:										break;
	}
}


/*!
//==============================================================================
	@brief		VALVE Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void valve_initial(void)
{
	vlv_manage.pulseCycle = VLV_DUTY_CYCLE + 1;									//Ý’è‚µ‚Ä‚¢‚éDUTY CYCLE‚Ì“Ç‚Ýo‚µ
	vlv_manage.slavePeriod = 0x0000;
	vlv_ctrl.state = VLV_STATE_OPEN;
	vlv_ctrl.phase = 0x00;
}


/*!
//==============================================================================
	@brief		VALVE OPEN State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void valve_open(void)
{
	switch(vlv_ctrl.phase){
		case 0x00:
			vlv_manage.slavePeriod = 0x0000;
			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
			R_TAU0_Channel0_Stop();
			VLV_DUTY_OFF();
			vlv_ctrl.phase = 0xFF;
			break;
		case 0xFF:
			//no operation
			break;
		default:
			break;
	}

}


/*!
//==============================================================================
	@brief		VALVE CLOSE State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void valve_close(void)
{
	switch(vlv_ctrl.phase){
		case 0x00:
			vlv_manage.slavePeriod = vlv_manage.pulseCycle;
			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
			R_TAU0_Channel0_Start();
			vlv_ctrl.phase = 0xFF;
			break;
		case 0xFF:
			//no operation
			break;
		default:
			break;
	}

}


/*!
//==============================================================================
	@brief		VALVE LEAK State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void valve_leak(void)
{
	uint32 temp;

	switch(vlv_ctrl.phase){
		case 0x00:
			//Initialize
			vlv_manage.dutyRatio = VLV_DUTY_RATIO_START;
			temp = (uint32)vlv_manage.pulseCycle * (uint32)vlv_manage.dutyRatio;
			vlv_manage.slavePeriod = (uint16)(temp / VLV_DUTY_RATIO_100);
			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
			vlv_ctrl.mode  = 0x00;
			vlv_ctrl.phase = 0xFF;
			break;
		case 0xFF:	//leak
			if(vlv_ctrl.mode){
				if(vlv_manage.dutyRatio > VLV_DUTY_RATIO_STOP){
					vlv_manage.dutyRatio -= VLV_DUTY_RATIO_RATE;
					temp = (uint32)vlv_manage.pulseCycle * (uint32)vlv_manage.dutyRatio;
					vlv_manage.slavePeriod = (uint16)(temp / VLV_DUTY_RATIO_100);
					VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
				}
				vlv_ctrl.mode = 0x00;
			}
			break;
		default:
			break;
	}

}


//[EOF]
