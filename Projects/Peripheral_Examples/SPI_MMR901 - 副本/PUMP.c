//******************************************************************************
/*!
	@file		PUMP.c
	@brief		PUMP Operate Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __PUMP_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
//YY #include "r_cg_macrodriver.h"
//YY #include "r_cg_timer.h"

#include "stm32f0xx.h"
#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		PUMP Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/















void pump_task(void)
{
	switch(pmp_ctrl.state){
		case PMP_STATE_INITIAL:		pump_initial();		break;
		case PMP_STATE_OFF:			pump_off();			break;
		case PMP_STATE_ON:			pump_on();			break;
		default:										break;
	}
}


/*!
//==============================================================================
	@brief		PUMP Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void pump_initial(void)
{
//#if(PMP_DUTY_CONTROL_EN == TRUE)
//	pmp_manage.pulseCycle = PMP_DUTY_CYCLE + 1;
//	pmp_manage.slavePeriod = 0x0000;
//	PMP_DUTY_SLAVE = pmp_manage.slavePeriod;
//#else
//	// DUTY=100%Ç…ÇµÇƒÅADUTY_CYCLEÇÃON/OFFÇ≈êßå‰
//	pmp_manage.pulseCycle = PMP_DUTY_CYCLE + 1;
//	pmp_manage.slavePeriod = pmp_manage.pulseCycle;
//	PMP_DUTY_SLAVE = pmp_manage.slavePeriod;
//#endif
	
	GPIO_pump_INT();
	
	
	
	
	pmp_ctrl.state = PMP_STATE_OFF;
	pmp_ctrl.phase = 0x00;
}


/*!
//==============================================================================
	@brief		PUMP OFF State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void pump_off(void)
{
	switch(pmp_ctrl.phase){
		case 0x00:
//#if(PMP_DUTY_CONTROL_EN == TRUE)
//			pmp_manage.slavePeriod = 0x0000;
//			PMP_DUTY_SLAVE = pmp_manage.slavePeriod;
//			R_TAU0_Channel4_Stop();
//			PMP_DUTY_OFF();
////			TO0 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;
//#else
//			// DUTY=100%Ç…ÇµÇƒÅADUTY_CYCLEÇÃON/OFFÇ≈êßå‰
//			R_TAU0_Channel4_Stop();
//			PMP_DUTY_OFF();
////			TO0 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;
//#endif
		
		
		
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		
 			pmp_ctrl.phase = 0xFF;
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
	@brief		PUMP ON State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void pump_on(void)
{
	switch(pmp_ctrl.phase){
		case 0x00:
//#if(PMP_DUTY_CONTROL_EN == TRUE)
//			pmp_manage.slavePeriod = pmp_manage.pulseCycle;
//			PMP_DUTY_SLAVE = pmp_manage.slavePeriod;
//			R_TAU0_Channel4_Start();
//#else
//			// DUTY=100%Ç…ÇµÇƒÅADUTY_CYCLEÇÃON/OFFÇ≈êßå‰
//			R_TAU0_Channel4_Start();
//#endif
		
		
		
		GPIO_SetBits(GPIOA, GPIO_Pin_7);

			pmp_ctrl.phase = 0xFF;
			break;
		case 0xFF:
			//no operation
			break;
		default:
			break;
	}
}



void GPIO_pump_INT(void)
{



	GPIO_InitTypeDef        GPIOb_InitStructure;


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIOb_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIOb_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIOb_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOb_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOb_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIOb_InitStructure);
	
	

}

//[EOF]
