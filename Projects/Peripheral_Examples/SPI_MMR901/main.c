/**
  ******************************************************************************
  * @file    IO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-October-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "common.h"
#include "main.h"



// 	PB3   SPI1_SCLK			o
// 	PB4		SPI1_MISO    	i
// 	PB5		SPI1_MOSI    	o
//	PB6		SPI1_CS				o
//
//	PB7		SPI1_ADRDY		i
//

/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BSRR_VAL        0x0300
#define USART1_TX_PIN 	GPIO_Pin_9
#define USART1_RX_PIN		GPIO_Pin_10



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef        GPIOb_InitStructure;

//USART_InitTypeDef				HUART1;

static __IO uint32_t TimingDelay;
/*
TIM2 for  PWM
*/


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);



__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
uint16_t PrescalerValue = 0;

















/* Private function prototypes -----------------------------------------------*/






static void TIM_Config(void);

static void USART1_Config(void);

////////////////////////////////////////////////////////////////////////////////////////
void GPIO_INT(void);




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
	
	
	
	
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

	  /* GPIOB Periph clock enable */

	uint8_t i = 0;
	uint8_t buffptr[5] = "ABCD";
	
	GPIO_INT();
	USART1_Config();
	
	
		if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
    while (1);
  }
	
	
	
	
	

	
	
	
	    /* Start user code. Do not edit comment generated here */
    	bpm_ctrl.state = BPM_STATE_INITIAL;
//    	key_ctrl.state = KEY_STATE_INITIAL;
    	pmp_ctrl.state = PMP_STATE_INITIAL;
    	vlv_ctrl.state = VLV_STATE_INITIAL;
//    	rtc_ctrl.state = RTC_STATE_INITIAL;
//    	lcd_ctrl.state = LCD_STATE_INITIAL;
//    	log_ctrl.state = LOG_STATE_INITIAL;
//    	R_TAU0_Channel7_Start();
//   	R_IT_Start();
	

	
#if 1
    
    while (1U)
    {

      bpm_main(); //include sensor task 
//        key_task();
     	  pump_task();
        valve_task();
//        rtc_task();
//        lcd_task();
//        log_task();
			



/* TIM3 Main Output Enable */
  
/*yy test*******************************************************************************/
//				vlv_ctrl.phase = 0x00;
//				valve_open();
//				vlv_ctrl.phase = 0x00;
//				valve_close();
			
//			pmp_ctrl.phase = 0x00;
//			pump_on();
//			pmp_ctrl.phase = 0x00;
//			pump_off();
			
//			
//		Delay(1);
//	
//		STM_EVAL_LEDToggle(LED3);			
			
			
//TIM_ForcedOC1Config( TIM3,TIM_ForcedAction_InActive );	

//TIM_ForcedOC1Config( TIM3,TIM_OCMode_PWM1 );				

//{TIM3->CCER&=0xFFFE;}	
/*yy test********************************************************************************/		
			
    }	
		
#endif 
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
//	TIM_Config();
	




  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PC8 and PC9 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */
#if 0	 


#endif
	
	
	
}












void GPIO_INT(void)
{






  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIOb_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIOb_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIOb_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOb_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOb_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIOb_InitStructure);
	
	
	GPIOb_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_7;
  GPIOb_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIOb_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOb_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOb_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIOb_InitStructure);
	
	
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIOb_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
  GPIOb_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIOb_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOb_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIOb_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIOb_InitStructure);
	

}






 /**
   * @brief  Configures the USART Peripheral.
   * @param  None
   * @retval None
   */
 static void USART1_Config(void)
 {
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   
   /* Enable GPIO clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
   
   /* Enable USART clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   

   /* Connect PXx to USARTx_Tx */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
   
   /* Connect PXx to USARTx_Rx */
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
   
   /* Configure USART Tx and Rx as alternate function push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 
   GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
 
   /* USARTx configuration ----------------------------------------------------*/
   /* USARTx configured as follow:
   - BaudRate = 230400 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Hardware flow control disabled (RTS and CTS signals)
   - Receive and transmit enabled
   */
   USART_InitStructure.USART_BaudRate = 9600;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART1, &USART_InitStructure);
   
   /* NVIC configuration */
   /* Enable the USARTx Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
   
   /* Enable USART */
   USART_Cmd(USART1, ENABLE);
 }
 









/**
  * @brief  Configure the TIM IRQ Handler.
  * @param  None
  * @retval None
 **/

static void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),  
      => TIM3CLK = PCLK1 = SystemCoreClock = 48 MHz
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                                  
    CC3 update rate = TIM3 counter clock / CCR3_Val = 439.4 Hz
    ==> Toggling frequency = 219.7 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f2xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
   
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

















/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */





void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}




















#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

