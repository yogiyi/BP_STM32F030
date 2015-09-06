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
//YY #include "r_cg_macrodriver.h"
//YY #include "r_cg_timer.h"
#include "stm32f0xx.h"
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





TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
TIM_OCInitTypeDef  TIM3_OCInitStructure;
uint16_t Timer3Period = 0;
uint16_t Timer3Channel1Pulse = 0;







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
//YY	vlv_manage.pulseCycle = VLV_DUTY_CYCLE + 1;									//Ý’è‚µ‚Ä‚¢‚éDUTY CYCLE‚Ì“Ç‚Ýo‚µ
	
	TIM3_vlv_Config();
	
	
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
//YY			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
//YY			R_TAU0_Channel0_Stop();
//YY			VLV_DUTY_OFF();
		
		
		
			TIM_ForcedOC1Config( TIM3,TIM_ForcedAction_Active );	
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
//YY			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
//YY			R_TAU0_Channel0_Start();
		
		
		
		
			TIM_ForcedOC1Config( TIM3,TIM_ForcedAction_InActive );	
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
			
		

//			vlv_manage.dutyRatio = VLV_DUTY_RATIO_START;
//			temp = (uint32)vlv_manage.pulseCycle * (uint32)vlv_manage.dutyRatio;
//		
//			vlv_manage.slavePeriod = (uint16)(temp / VLV_DUTY_RATIO_100);
//		
//			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
			TIM_ForcedOC1Config( TIM3,TIM_OCMode_PWM2 );
		
			vlv_manage.dutyRatio = VLV_DUTY_RATIO_START;
			
		
			vlv_manage.slavePeriod = (uint16_t) (((uint32_t) vlv_manage.dutyRatio * (Timer3Period - 1)) / VLV_DUTY_RATIO_100);
		
			VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
		
		
		
		
		
			vlv_ctrl.mode  = 0x00;
			vlv_ctrl.phase = 0xFF;
			break;
		case 0xFF:	//leak
			if(vlv_ctrl.mode){
				if(vlv_manage.dutyRatio > VLV_DUTY_RATIO_STOP){
					vlv_manage.dutyRatio -= VLV_DUTY_RATIO_RATE;
//					temp = (uint32)vlv_manage.pulseCycle * (uint32)vlv_manage.dutyRatio;
//					vlv_manage.slavePeriod = (uint16)(temp / VLV_DUTY_RATIO_100);
					vlv_manage.slavePeriod = (uint16_t) (((uint32_t) vlv_manage.dutyRatio * (Timer3Period - 1)) / VLV_DUTY_RATIO_100); 
					
					VLV_DUTY_SLAVE = vlv_manage.slavePeriod;
				}
				vlv_ctrl.mode = 0x00;
			}
			break;
		default:
			break;
	}

}



 void TIM3_vlv_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
  
  
  /* TIM3 Configuration ---------------------------------------------------
   Generate PWM signals with 4 different duty cycles:
   TIM3 input clock (TIM1CLK) is set to APB1 clock (PCLK1)    
    => TIM3CLK = PCLK1 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices
   
   The objective is to generate 4 PWM signal at 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
   
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  Timer3Period = (SystemCoreClock / 31250 ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
  Timer3Channel1Pulse = (uint16_t) (((uint32_t) 5 * (Timer3Period - 1)) / 10);
  /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 */
 // Timer2Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
  /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 */
 // Timer2Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
  /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
//  Timer2Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  
  /* Time Base configuration */
  TIM3_TimeBaseStructure.TIM_Prescaler = 0;
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM3_TimeBaseStructure.TIM_Period = Timer3Period;
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);

  /* Channel 1, 2, 3 and 4 Configuration in PWM mode */
  TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM3_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM3_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM3_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM3_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM3_OCInitStructure.TIM_Pulse = Timer3Channel1Pulse;
  TIM_OC1Init(TIM3, &TIM3_OCInitStructure);

//  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  
//  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 counter enable */
  TIM_Cmd(TIM3, ENABLE);
	
	

}


















//[EOF]
