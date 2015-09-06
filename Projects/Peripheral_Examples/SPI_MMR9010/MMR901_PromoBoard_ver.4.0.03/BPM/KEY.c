//******************************************************************************
/*!
	@file		KEY.c
	@brief		KEY Operate Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __KEY_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"

#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		KEY Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void key_task(void)
{
	switch(key_ctrl.state){
		case KEY_STATE_INITIAL:		key_initial();		break;
		case KEY_STATE_ENABLE:		key_enable();		break;
		case KEY_STATE_DISABLE:		key_disable();		break;
		default:										break;
	}
	bpm_key_management();														// KEY Management
}


/*!
//==============================================================================
	@brief		KEY Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This function initializes the KEY parameters.

//==============================================================================
*/
void key_initial(void)
{
	KEY_EVENT_CLR;
	KEY_STATUS_CLR;
	key_ctrl.state = KEY_STATE_DISABLE;
	key_ctrl.phase = 0x00;
	key_ctrl.count = 0x00;

}


/*!
//==============================================================================
	@brief		KEY Enable State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void key_enable(void)
{
	switch(key_ctrl.phase){
		case 0x00:
			R_INTC3_Start();
			R_INTC6_Start();
			R_INTC7_Start();
			key_ctrl.phase = 0x01;
			break;
		case 0x01:
			// check START & SELECT button
			if(b_KEY_EVENT_START==ON && b_KEY_EVENT_SELECT==ON && b_KEY_EVENT_MEMORY==OFF){
				if(b_KEY_EVENT_ERASE == OFF){
					b_KEY_EVENT_ERASE = ON;
					key_manage.interval5ms = M_T5MS_COUNT_5S;					// 5sec
				}else{
					if(P_KEY_START==LOW && P_KEY_SELECT==LOW){
						if(key_manage.interval5ms==0){
							b_KEY_EVENT_ERASE = OFF;
							b_KEY_STATUS_ERASE = ON;							// request the ERASE operation
						}
					}else{
						b_KEY_EVENT_ERASE = OFF;
						b_KEY_STATUS_ERASE = OFF;
					}
				}
			}

			// push START button
			if(b_KEY_EVENT_START==ON && b_KEY_EVENT_ERASE==OFF && b_KEY_EVENT_MEMORY==OFF){
				if(key_manage.interval5ms == 0){
					if(P_KEY_START==HIGH){
						b_KEY_EVENT_START = OFF;
						b_KEY_STATUS_START = ON;								// request the START operation
					}
				}else{
					if(P_KEY_START==HIGH){
						b_KEY_EVENT_START = OFF;
					}
				}
			} 

			// push SELECT button
			if(b_KEY_EVENT_SELECT==ON && b_KEY_EVENT_ERASE==OFF && b_KEY_EVENT_MEMORY==OFF){
				if(key_manage.interval5ms == 0){
					if(P_KEY_SELECT==HIGH){
						b_KEY_EVENT_SELECT = OFF;
						b_KEY_STATUS_SELECT = ON;								// request the SELECT operation
					}
				}else{
					if(P_KEY_SELECT==HIGH){
						b_KEY_EVENT_SELECT = OFF;
					}
				}
			} 

			// push MEMORY button
			if(b_KEY_EVENT_START==OFF && b_KEY_EVENT_SELECT==OFF && b_KEY_EVENT_MEMORY==ON){
				if(key_manage.interval5ms == 0){
					b_KEY_EVENT_MEMORY = OFF;
					b_KEY_STATUS_ADJUST = ON;									// request the ADJUST operation
				}else if(key_manage.interval5ms < M_T5MS_COUNT_5S - M_T5MS_COUNT_100MS){
					if(P_KEY_MEMORY==HIGH){
						b_KEY_EVENT_MEMORY = OFF;
						b_KEY_STATUS_MEMORY = ON;								// request the MEMORY operation
					}
				}else{
					if(P_KEY_MEMORY==HIGH){
						b_KEY_EVENT_MEMORY = OFF;
					}
				}
			}
			// push START & MEMORY & SELECT button
			if(b_KEY_EVENT_START==ON && b_KEY_EVENT_SELECT==ON && b_KEY_EVENT_MEMORY==ON){
				KEY_EVENT_CLR;
			}
			break;
		default:
			break;
	}
}


/*!
//==============================================================================
	@brief		KEY Disable State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void key_disable(void)
{
	switch(key_ctrl.phase){
		case 0x00:
			R_INTC3_Stop();
			R_INTC6_Stop();
			R_INTC7_Stop();
			key_ctrl.phase = 0x01;
			break;
		case 0x01:
			// idle
			break;
		default:
			break;
	}
}


//[EOF]
