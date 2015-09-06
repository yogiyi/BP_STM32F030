//******************************************************************************
/*!
	@file		BPM_MAIN.c
	@brief		Blood-Pressure Monitor System Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	Since no medical approval is obtained as a blood pressure monitor,\n
				all the measurement values are for reference only.
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __BPM_MAIN_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
//#include "r_cg_macrodriver.h"
//#include "r_cg_adc.h"
//#include "r_cg_cgc.h"

#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		Blood Pressure Monitor Main Task
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20

//==============================================================================
	@par		Operations Description:
	This function is the main task of Blood Pressure Monitor.\n
	The process is controlled by the state parameter "bmp_ctrl.state".

//==============================================================================
*/

/*在bpm_idle()判断按键,然后进入connect中*/

uint32_t  YY1=0;

uint32_t  YY2=0;

uint32_t  YY3=0;


uint32 yy1=0;
uint32 yy2=0;
uint32 yy3=0;


void SendData2Uart(void *data,uint16_t length)
{
		
	uint8_t i;
	uint8_t *j;
	j=(uint8_t *)data;
	for( i= 0;i<length;i++)
	{
		/* Send one byte from USART1 to USARTx */
         USART_SendData(USART1, (uint16_t)j[i]);
         
         /* Loop until USART1 DR register is empty */ 
         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
					{;}
	}

}


















void bpm_main(void)
{

	
	switch(bpm_ctrl.state){
		case BPM_STATE_INITIAL:			bpm_initial();		break;
//		case BPM_STATE_IDLE:			bpm_idle();			break;
//		case BPM_STATE_CONNECT:			bpm_connect();		break;
		case BPM_STATE_MEASURE:			bpm_measure();		break;
//		case BPM_STATE_RESULT:			bpm_result();		break;
//		case BPM_STATE_DISPLAY:			bpm_display();		break;
//		case BPM_STATE_ADJUST:			bpm_adjust();		break;
//		case BPM_STATE_SLEEP:			bpm_sleep();		break;
			default:
//			bpm_manage.errorCode = BPM_ERROR_BUG;								// EC:BUG
//			b_BPM_FATAL_ERR = ON;
//			utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);
			break;
	}
}

/*!
//==============================================================================
	@brief		Blood Pressure Monitor Initialize State
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/



void bpm_initial(void)
{
	
switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) wait for the initialization of all tasks to be compeled
		// -----------------------------
//			if(key_ctrl.state != KEY_STATE_INITIAL) break;
//			if(pmp_ctrl.state != PMP_STATE_INITIAL) break;
//			if(vlv_ctrl.state != VLV_STATE_INITIAL) break;
//			if(rtc_ctrl.state != RTC_STATE_INITIAL) break;
//			if(lcd_ctrl.state != LCD_STATE_INITIAL) break;
//			if(log_ctrl.state != LOG_STATE_INITIAL) break;
			bpm_ctrl.phase = 0x01;
			break;
		case 0x01:
		// -----------------------------
		// (01) Software Initialize
		// -----------------------------
			bpm_disp_tx10 = 250;												// Initialize:25.0 degree

			func_calc_initialize();												// clear func-parameter
		
		
			utl_memory_clr(&bpm_bpm_para, sizeof(bpm_bpm_para));				// clear blood pressure monitor data
			utl_memory_clr(&bpm_prt_para, sizeof(bpm_prt_para));				// clear pulse rate data		
			utl_memory_clr(&bpm_bva_para, sizeof(bpm_bva_para));				// clear blood vessel age data

			b_BPM_PULSE_CLR;
			b_BPM_STATUS_CLR;
			bpm_manage.errorCode = BPM_ERROR_NO_ERROR;							// EC: No Error

//yy		utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);						// change state : RTC LOAD

			bpm_ctrl.phase = 0x02;
			break;
		case 0x02:
		// -----------------------------
		// (02) wait for RTC IDLE STATE
		// -----------------------------
//YY			if(rtc_ctrl.state != RTC_STATE_IDLE) break;

			bpm_ctrl.phase = 0x03;
			break;
		case 0x03:
		// -----------------------------
		// (03) Load EEPROM
		// -----------------------------
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP command
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP command
			spi_init_cs();

			if(func_read_calib_data())
				{
				bpm_manage.errorCode = BPM_ERROR_CALIB_DATA;				// EC:Calib Data Error
				b_BPM_FATAL_ERR = ON;
				}
//			utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);				// change state : BPM IDLE
		
				utl_change_state(&bpm_ctrl, BPM_STATE_MEASURE);		 	//
				
				
			break;
		default:
			break;
	}
	
}








void bpm_measure(void)
{
	uint16 data;
	
	uint8_t buffer[3] = {0x67,0x93,0x04};
	
	

	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (0) start the measurement of the temperature
		// -----------------------------
			cmd_exec_spi_protocol(CMD_MEAS_TEMP_SEQ, 0x00);						// execute MEASURE TEMPERATURE SEQUENTIAL command
			bpm_ctrl.phase = 0x01;												// next phase
			bpm_ctrl.count = 0x00;

			bpm_calc.data_max = 0x0000;
			bpm_calc.data_min = 0xFFFF;
			bpm_calc.data_ave = 0;

			bpm_measuringTime.year   = bpm_currentTime.year;
			bpm_measuringTime.month  = bpm_currentTime.month;
			bpm_measuringTime.day    = bpm_currentTime.day;
			bpm_measuringTime.hour   = bpm_currentTime.hour;
			bpm_measuringTime.minute = bpm_currentTime.minute;
		
		
			bpm_manage.countdown5ms	=M_T5MS_COUNT_2S;	//YY

//yy			utl_change_state(&lcd_ctrl, LCD_STATE_MEASURE);						// change state : LCD 
//yy			utl_change_state(&key_ctrl, KEY_STATE_ENABLE);						// change state : KEY 

			break;

		case 0x01:
			if(bpm_manage.countdown5ms == 0x00)
			{
			
			
		// -----------------------------
		// (01) calculate the sensitivity of the pressure sensor with temperature correnction
		// -----------------------------
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data
				bpm_calc.data_ave += data;
				if(data>=bpm_calc.data_max) bpm_calc.data_max = data;
				if(data<=bpm_calc.data_min) bpm_calc.data_min = data;
				bpm_ctrl.count++;
				
				if(bpm_ctrl.count == M_BPM_GET_CNT){
					// averaging (except for max & min value)
					bpm_calc.data_ave -= ((uint32)bpm_calc.data_min + (uint32)bpm_calc.data_max);
					bpm_calc.data_ave /= M_BPM_AVE_CNT;

					// the calculation of the difference from the reference temperature (delta_t)
					bpm_disp_tx10 = func_calc_temperature((uint16)bpm_calc.data_ave, TC_COMP_METHOD_2ND);

					// the calculation of the sensitivity of the pressure sensor with temperature correnction (spt25x)
					func_coeff.spt25x = func_calc_sensitivity(func_coeff.delta_t);

					cmd_exec_spi_protocol(CMD_STOP, 0x00);						// execute STOP command

					bpm_ctrl.phase = 0x02;										// next phase
					bpm_ctrl.count = 0x00;
				}
			}
		}
			break;
		
		case 0x02:
		// -----------------------------
		// (02) close the solenoid valve
		// -----------------------------
			utl_change_state(&vlv_ctrl, VLV_STATE_CLOSE);						// change state : 
			bpm_ctrl.phase = 0x03;												// next phase
			bpm_ctrl.count = 0x00;
//YY			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)
			bpm_manage.countdown5ms = M_T5MS_COUNT_2S;						// wait 100ms(for reference)
			
			
			break;

		case 0x03:
		// -----------------------------
		// (03) wait until the pressure is stable and start the measurement of the pressure 
		// -----------------------------
			if(bpm_manage.countdown5ms == 0x00){
				cmd_exec_spi_protocol(CMD_MEAS_PRES_SEQ, 0x00);					// execute MEASURE PRESSURE SEQUENTIAL command
				bpm_ctrl.phase = 0x04;
				bpm_ctrl.count = 0x00;

				bpm_calc.data_max = 0x0000;
				bpm_calc.data_min = 0xFFFF;
				bpm_calc.data_ave = 0x0000;
				bpm_manage.countdown5ms = M_T5MS_COUNT_2S;	
			}
			break;

		case 0x04:
		// -----------------------------
		// (04) measure the reference pressure
		// -----------------------------
		if(bpm_manage.countdown5ms ==0)
		{
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data
				bpm_calc.data_ave += data;
				if(data>=bpm_calc.data_max) bpm_calc.data_max = data;
				if(data<=bpm_calc.data_min) bpm_calc.data_min = data;
				bpm_ctrl.count++;				
				
				if(bpm_ctrl.count == M_BPM_GET_CNT){
					// averaging (except for max & min value)
					bpm_calc.data_ave -= ((uint32)bpm_calc.data_min + (uint32)bpm_calc.data_max);
					bpm_calc.data_ave /= M_BPM_AVE_CNT;

					func_coeff.zero = (uint16)bpm_calc.data_ave;

					bpm_ctrl.phase = 0x05;										// next phase
					bpm_ctrl.count = 0x00;
				}
			}
			}
			break;
			
		case 0x05:
		// -----------------------------
		// (05) start the increasing pressure
		// -----------------------------
			utl_change_state(&pmp_ctrl, PMP_STATE_ON);							// change state : PUMP ON
			
			bpm_ctrl.phase = 0x06;												
			bpm_ctrl.count = 0x00;

			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)

//yy			key_ctrl.state = KEY_STATE_ENABLE;
			bpm_calc.data_ave = 0x00;
			
			break;
			
		case 0x06:
		// -----------------------------
		// (06) wait until the pressure is stable
		// -----------------------------
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_ctrl.phase = 0x07;
				bpm_ctrl.count = 0x00;
				bpm_manage.countdown5ms = M_T5MS_COUNT_30S;						// set timeout 30s(for reference) 
			}
			break;

		case 0x07:
		// -----------------------------
		// (07) increases of the pressure until upper limits is reached
		// -----------------------------
			if(func_read_adc_data(&data)==FUNC_ADC_READY){						// read ADC data

				// convert the pressure to ADC value
				bpm_calc.data_ave = func_calc_pressure(data, func_coeff.spt25x, func_coeff.zero);
				
				
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
				
//				SendData2Uart(buffer,3);
//				SendData2Uart(&bpm_calc.data_ave,4);
				
				
				
				
				
				
				
				
				
////////////////////////////XH/////////////////////////////////////////////////////////////////////////////////////				
				// reach the upper limits
				if((sint16)bpm_calc.data_ave >= BPM_PRESS_MAX_LIMIT){			//

					// pressure rise sharply: the tube is twisted
					if(bpm_manage.countdown5ms > M_T5MS_COUNT_30S - M_T5MS_COUNT_5S){

						bpm_manage.errorCode = BPM_ERROR_PRESSURE;				// EC:Pressure Error
//							bpm_ctrl.phase = 0x08;
						bpm_ctrl.phase = 0xFF;									// end phase
						bpm_ctrl.count = 0x00;

					}else{
						utl_change_state(&pmp_ctrl, PMP_STATE_OFF);				// change state : PUMP OFF
//						bpm_ctrl.phase = 0x07;
						bpm_ctrl.phase = 0x08;									// next phase
						
						bpm_ctrl.count = 0x00;
					}
				}
			}
			
			// ***** LCD update *****
			if(bpm_manage.countdown5ms % M_T5MS_COUNT_500MS == 0){				// interval 500ms(for reference)
//				lcd_measuring_press = (sint16)bpm_calc.data_ave;
//yy				lcd_data.press = (uint8)(bpm_calc.data_ave/100);
//yy				lcd_ctrl.mode  = LCD_MODE_MSR_UPDATE;
			}

			// ***** Error *****
			if(bpm_manage.countdown5ms==0){										// timeout ?
				// not wear the Cuff or break the tube
				bpm_manage.errorCode = BPM_ERROR_TIMEOUT1;						// EC:Timeout Error
				bpm_ctrl.phase = 0xFF;											// end phase
//				bpm_ctrl.phase = 0x08;											// end phase
				bpm_ctrl.count = 0x00;
			}
			break;

		case 0x08:
		// -----------------------------
		// (08) half open the solenoid valve            
		// -----------------------------
		
//			utl_change_state(&vlv_ctrl, VLV_STATE_OPEN);
			utl_change_state(&vlv_ctrl, VLV_STATE_LEAK);						// change state : VAVLE LEAK
			bpm_ctrl.phase = 0x09;												// next phase
			bpm_ctrl.count = 0x00;

			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)

			utl_memory_clr(&bpm_bpm_para, sizeof(bpm_bpm_para));				// clear blood pressure monitor data
			utl_memory_clr(&bpm_prt_para, sizeof(bpm_prt_para));				// clear pulse rate data
			utl_memory_clr(&bpm_bva_para, sizeof(bpm_bva_para));				// clear vascular hardness data
			break;

		case 0x09:
		// -----------------------------
		// (09) wait until the pressure is stable
		// -----------------------------
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_ctrl.phase = 0x0A;											// next phase
				bpm_ctrl.mode  = 0x00;
				
				bpm_manage.countdown5ms = M_T5MS_COUNT_120S;					// Set timeout 120s(for reference)
				bpm_bpm_para.time = bpm_manage.countdown5ms;
				bpm_pulse_status.BYTE = 0x00;									// clear flag
				bpm_bpm_para.base = BPM_PRESS_MAX_LIMIT;
				bpm_bpm_para.peak = BPM_PRESS_MAX_LIMIT;

				bpm_bpm_para.rise[0] = BPM_PRESS_MAX_LIMIT;						// bug fix
				bpm_bpm_para.rise[1] = BPM_PRESS_MAX_LIMIT;

				b_BPM_PULSE_START = ON;
			}
			break;

		case 0x0A:
		// -----------------------------
		// (0A) measure the blood pressure
		// -----------------------------
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data

				// convert the pressure to ADC value
				bpm_calc.data_ave = func_calc_pressure(data, func_coeff.spt25x, func_coeff.zero);
				bpm_ctrl.count++;
	





				
				
				
				

				
				
	#if 1
					if(bpm_ctrl.count == BPM_MEAS_INTERVAL){
						bpm_get_pulse_data((sint16)bpm_calc.data_ave);				// masure the pulse rate
						bpm_ctrl.count = 0;
					}
#endif
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					if(bpm_calc.data_ave < BPM_PRESS_1ST_LEAK_LIMIT){
						bpm_ctrl.mode++;
						if(bpm_ctrl.mode >= (uint8)BPM_VALVE_LEAK_KEEP){			// 
							bpm_ctrl.mode = 0x00;
							vlv_ctrl.mode = 1;										// request a change of duty ratio
						}
					}
		
			}

			// ***** LCD update *****
			// request the display of the pressure
			if(bpm_manage.countdown5ms%100 == 0){								// interval 500ms(for reference)
//				lcd_measuring_press = (sint16)bpm_calc.data_ave;
//yy				lcd_data.press = (uint8)(bpm_calc.data_ave/100);
//yy				lcd_ctrl.mode  = LCD_MODE_MSR_UPDATE;
			}
			// ***** end *****
			if(b_BPM_PULSE_END){												// end of the measurement
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;
			}
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_manage.errorCode = BPM_ERROR_TIMEOUT2;						// EC:Timeout Error
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;

			}
			if(	(bpm_bpm_para.count <= 2) && 
				(bpm_calc.data_ave < BPM_PRESS_NO_BEAT)){
				bpm_manage.errorCode = BPM_ERROR_NO_BEAT;						// EC:No Beat Error
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;
			}
			break;

		case 0xFF:
		// -----------------------------
		// (FF) 廔椆張棟
		// -----------------------------
			if(pmp_ctrl.state != PMP_STATE_OFF){
				utl_change_state(&pmp_ctrl, PMP_STATE_OFF);						// change state : PUMP OFF
			}
			if(vlv_ctrl.state != VLV_STATE_OPEN){
				utl_change_state(&vlv_ctrl, VLV_STATE_OPEN);					// change state : VALVE OPEN
			}
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP commad
			
			utl_change_state(&bpm_ctrl, BPM_STATE_RESULT);						// change state : BPM RESULT
			break;
		default:
			break;
	}
}




























































# if 0
void bpm_initial(void)
{
	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) wait for the initialization of all tasks to be compeled
		// -----------------------------
			if(key_ctrl.state != KEY_STATE_INITIAL) break;
			if(pmp_ctrl.state != PMP_STATE_INITIAL) break;
			if(vlv_ctrl.state != VLV_STATE_INITIAL) break;
			if(rtc_ctrl.state != RTC_STATE_INITIAL) break;
			if(lcd_ctrl.state != LCD_STATE_INITIAL) break;
			if(log_ctrl.state != LOG_STATE_INITIAL) break;
			bpm_ctrl.phase = 0x01;
			break;
		case 0x01:
		// -----------------------------
		// (01) Software Initialize
		// -----------------------------
			bpm_disp_tx10 = 250;												// Initialize:25.0 degree

			func_calc_initialize();												// clear func-parameter

			utl_memory_clr(&bpm_bpm_para, sizeof(bpm_bpm_para));				// clear blood pressure monitor data
			utl_memory_clr(&bpm_prt_para, sizeof(bpm_prt_para));				// clear pulse rate data
			utl_memory_clr(&bpm_bva_para, sizeof(bpm_bva_para));				// clear blood vessel age data

			b_BPM_PULSE_CLR;
			b_BPM_STATUS_CLR;
			bpm_manage.errorCode = BPM_ERROR_NO_ERROR;							// EC: No Error

			utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);						// change state : RTC LOAD

			bpm_ctrl.phase = 0x02;
			break;
		case 0x02:
		// -----------------------------
		// (02) wait for RTC IDLE STATE
		// -----------------------------
			if(rtc_ctrl.state != RTC_STATE_IDLE) break;

			bpm_ctrl.phase = 0x03;
			break;
		case 0x03:
		// -----------------------------
		// (03) Load EEPROM
		// -----------------------------
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP command
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP command
			spi_init_cs();

			if(func_read_calib_data()){
				bpm_manage.errorCode = BPM_ERROR_CALIB_DATA;					// EC:Calib Data Error
				b_BPM_FATAL_ERR = ON;
			}
			utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);						// change state : BPM IDLE
		
			break;
		default:
			break;
	}
}


/*!
//==============================================================================
	@brief		Blood Pressure Monitor Idle State
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_idle(void)
{
	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) Initialize
		// -----------------------------
			if(b_BPM_FATAL_ERR){
				bpm_ctrl.phase = 0xFF;
				lcd_data.error = bpm_manage.errorCode;
				lcd_ctrl.state = LCD_STATE_ERROR;
				lcd_ctrl.mode  = LCD_MODE_ERR_UPDATE;
				utl_change_state(&key_ctrl, KEY_STATE_DISABLE);					// change state : KEY 
				break;
			}

			//KEY ENABLE
			utl_change_state(&key_ctrl, KEY_STATE_ENABLE);						// change state : KEY 
			utl_change_state(&lcd_ctrl, LCD_STATE_CLOCK);						// change state : LCD 

			bpm_manage.errorCode = BPM_ERROR_NO_ERROR;							// EC:No Error
			bpm_manage.countdown5ms = M_T5MS_COUNT_250MS;						// interval 250ms(for reference)
			bpm_manage.battStatus = BPM_BATT_NO_CHECK;							

    		bpm_ctrl.phase = 0x01;
			break;

		case 0x01:
		// -----------------------------
		// (01) IDLE
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_START){						// request START ?
				bpm_manage.battStatus = BPM_BATT_NO_CHECK;
				bpm_ctrl.phase = 0x02;
				bpm_ctrl.mode  = 0x00;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ERASE){
				utl_change_state(&log_ctrl, LOG_STATE_ERASE);					//  change state : LOG ERASE
				bpm_ctrl.phase = 0x03;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_DATE){
				if(lcd_ctrl.count){
					lcd_ctrl.count = 0;
					lcd_ctrl.phase = LCD_PHASE_CLK_TIME;
				}else{
					lcd_ctrl.count = 1;
					lcd_ctrl.phase = LCD_PHASE_CLK_DATE;
				}
				lcd_ctrl.mode = LCD_MODE_CLK_UPDATE;
				bpm_ctrl.phase = 0x04;

			}else if(bpm_manage.keyRequest != BPM_KEY_REQ_NONE){
				bpm_ctrl.phase = 0x04;

			}else if(bpm_manage.countdown5ms == 0x00){							// check interval 250ms ?
				bpm_manage.countdown5ms = M_T5MS_COUNT_250MS;
				bpm_ctrl.count++;

				if(bpm_ctrl.count >= 120){

					utl_change_state(&bpm_ctrl, BPM_STATE_SLEEP);

				}
				if(bpm_ctrl.count%4 == 0){
					utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);				//  change state : RTC 
				}else if(bpm_ctrl.count%4 == 1){
					lcd_data.year   = bpm_currentTime.year;
					lcd_data.month  = bpm_currentTime.month;
					lcd_data.day    = bpm_currentTime.day;
					lcd_data.hour   = bpm_currentTime.hour;
					lcd_data.minute = bpm_currentTime.minute;

					lcd_ctrl.mode = LCD_MODE_CLK_UPDATE;

				}else if(bpm_ctrl.count%4 == 2){
					utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);				//  change state : RTC 

				}else{
					lcd_data.year   = bpm_currentTime.year;
					lcd_data.month  = bpm_currentTime.month;
					lcd_data.day    = bpm_currentTime.day;
					lcd_data.hour   = bpm_currentTime.hour;
					lcd_data.minute = bpm_currentTime.minute;

					lcd_ctrl.mode = LCD_MODE_CLK_BLINK;
				}
			}
			break;

		case 0x02:
		// -----------------------------
		// (02) check Main Battery
		// -----------------------------
			bpm_check_battery();
			if(bpm_manage.battStatus != BPM_BATT_NO_CHECK){
				bpm_manage.countdown5ms = M_T5MS_COUNT_30S;						// interval 30s(for reference)
				bpm_ctrl.phase = 0x04;
			}
			break;
		case 0x03:
		// -----------------------------
		// (03) wait for LOG ERASE
		// -----------------------------
			if(log_ctrl.state == LOG_STATE_READY){
				bpm_ctrl.phase = 0x04;
			}
			break;

		case 0x04:
		// -----------------------------
		// (04) select State
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_DSP_START){					// request DISPLAY
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				utl_change_state(&bpm_ctrl, BPM_STATE_DISPLAY);					//   change state : BPM DISPLAY

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_START){			// request ADJUST CLOCK
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				utl_change_state(&bpm_ctrl, BPM_STATE_ADJUST);					//   change state : BPM ADJUST

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_START){				// request START
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;						// 

				if(bpm_manage.battStatus == BPM_BATT_NONE){
					utl_change_state(&bpm_ctrl, BPM_STATE_SLEEP);				//  change state : BPM 

				}else if(bpm_manage.battStatus == BPM_BATT_LOW){
					utl_change_state(&bpm_ctrl, BPM_STATE_SLEEP);				//  change state : BPM 

				}else{
					utl_change_state(&bpm_ctrl, BPM_STATE_CONNECT);				//  change state : BPM 
				}
			}else{
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_manage.countdown5ms = M_T5MS_COUNT_250MS;					// interval 250ms(for reference)
				bpm_manage.battStatus = BPM_BATT_NO_CHECK;						
				bpm_ctrl.count = 0x00;											
				bpm_ctrl.phase = 0x01;											// to idle phase
			}
			break;

		case 0xFF:
		// -----------------------------
		// (FF) idle for displaying the error code
		// -----------------------------
			//FATAL ERROR
			break;
		default:
			break;
	}
}



/*!
//==============================================================================
	@brief		Blood Pressure Monitor Connection State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_connect(void)
{
	utl_change_state(&bpm_ctrl, BPM_STATE_MEASURE);								// to MEASURE STATE
}


/*!
//==============================================================================
	@brief		Blood Pressure Monitor Measure State
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_measure(void)
{
	uint16 data;
	
	if(bpm_manage.keyRequest == BPM_KEY_REQ_STOP){
		bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
		bpm_manage.errorCode = BPM_ERROR_EMERGENCY;								// EC:Emergency Stop
		bpm_ctrl.phase = 0xFF;													// end phase
		bpm_ctrl.count = 0x00;
	}

	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (0) start the measurement of the temperature
		// -----------------------------
			cmd_exec_spi_protocol(CMD_MEAS_TEMP_SEQ, 0x00);						// execute MEASURE TEMPERATURE SEQUENTIAL command
			bpm_ctrl.phase = 0x01;												// next phase
			bpm_ctrl.count = 0x00;

			bpm_calc.data_max = 0x0000;
			bpm_calc.data_min = 0xFFFF;
			bpm_calc.data_ave = 0;

			bpm_measuringTime.year   = bpm_currentTime.year;
			bpm_measuringTime.month  = bpm_currentTime.month;
			bpm_measuringTime.day    = bpm_currentTime.day;
			bpm_measuringTime.hour   = bpm_currentTime.hour;
			bpm_measuringTime.minute = bpm_currentTime.minute;

			utl_change_state(&lcd_ctrl, LCD_STATE_MEASURE);						// change state : LCD 
			utl_change_state(&key_ctrl, KEY_STATE_ENABLE);						// change state : KEY 

			break;

		case 0x01:
		// -----------------------------
		// (01) calculate the sensitivity of the pressure sensor with temperature correnction
		// -----------------------------
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data
				bpm_calc.data_ave += data;
				if(data>=bpm_calc.data_max) bpm_calc.data_max = data;
				if(data<=bpm_calc.data_min) bpm_calc.data_min = data;
				bpm_ctrl.count++;
				
				if(bpm_ctrl.count == M_BPM_GET_CNT){
					// averaging (except for max & min value)
					bpm_calc.data_ave -= ((uint32)bpm_calc.data_min + (uint32)bpm_calc.data_max);
					bpm_calc.data_ave /= M_BPM_AVE_CNT;

					// the calculation of the difference from the reference temperature (delta_t)
					bpm_disp_tx10 = func_calc_temperature((uint16)bpm_calc.data_ave, TC_COMP_METHOD_2ND);

					// the calculation of the sensitivity of the pressure sensor with temperature correnction (spt25x)
					func_coeff.spt25x = func_calc_sensitivity(func_coeff.delta_t);

					cmd_exec_spi_protocol(CMD_STOP, 0x00);						// execute STOP command

					bpm_ctrl.phase = 0x02;										// next phase
					bpm_ctrl.count = 0x00;
				}
			}
			break;

		case 0x02:
		// -----------------------------
		// (02) close the solenoid valve
		// -----------------------------
			utl_change_state(&vlv_ctrl, VLV_STATE_CLOSE);						// change state : 
			bpm_ctrl.phase = 0x03;												// next phase
			bpm_ctrl.count = 0x00;
			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)
			
			
			break;

		case 0x03:
		// -----------------------------
		// (03) wait until the pressure is stable and start the measurement of the pressure 
		// -----------------------------
			if(bpm_manage.countdown5ms == 0x00){
				cmd_exec_spi_protocol(CMD_MEAS_PRES_SEQ, 0x00);					// execute MEASURE PRESSURE SEQUENTIAL command
				bpm_ctrl.phase = 0x04;
				bpm_ctrl.count = 0x00;

				bpm_calc.data_max = 0x0000;
				bpm_calc.data_min = 0xFFFF;
				bpm_calc.data_ave = 0x0000;
			}
			break;

		case 0x04:
		// -----------------------------
		// (04) measure the reference pressure
		// -----------------------------
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data
				bpm_calc.data_ave += data;
				if(data>=bpm_calc.data_max) bpm_calc.data_max = data;
				if(data<=bpm_calc.data_min) bpm_calc.data_min = data;
				bpm_ctrl.count++;				
				
				if(bpm_ctrl.count == M_BPM_GET_CNT){
					// averaging (except for max & min value)
					bpm_calc.data_ave -= ((uint32)bpm_calc.data_min + (uint32)bpm_calc.data_max);
					bpm_calc.data_ave /= M_BPM_AVE_CNT;

					func_coeff.zero = (uint16)bpm_calc.data_ave;

					bpm_ctrl.phase = 0x05;										// next phase
					bpm_ctrl.count = 0x00;
				}
			}
			break;
			
		case 0x05:
		// -----------------------------
		// (05) start the increasing pressure
		// -----------------------------
			utl_change_state(&pmp_ctrl, PMP_STATE_ON);							// change state : PUMP ON
			
			bpm_ctrl.phase = 0x06;												
			bpm_ctrl.count = 0x00;

			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)

			key_ctrl.state = KEY_STATE_ENABLE;
			bpm_calc.data_ave = 0x00;
			
			break;
			
		case 0x06:
		// -----------------------------
		// (06) wait until the pressure is stable
		// -----------------------------
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_ctrl.phase = 0x07;
				bpm_ctrl.count = 0x00;
				bpm_manage.countdown5ms = M_T5MS_COUNT_30S;						// set timeout 30s(for reference) 
			}
			break;

		case 0x07:
		// -----------------------------
		// (07) increases of the pressure until upper limits is reached
		// -----------------------------
			if(func_read_adc_data(&data)==FUNC_ADC_READY){						// read ADC data

				// convert the pressure to ADC value
				bpm_calc.data_ave = func_calc_pressure(data, func_coeff.spt25x, func_coeff.zero);
			
				// reach the upper limits
				if((sint16)bpm_calc.data_ave >= BPM_PRESS_MAX_LIMIT){			//

					// pressure rise sharply: the tube is twisted
					if(bpm_manage.countdown5ms > M_T5MS_COUNT_30S - M_T5MS_COUNT_5S){

						bpm_manage.errorCode = BPM_ERROR_PRESSURE;				// EC:Pressure Error
						bpm_ctrl.phase = 0xFF;									// end phase
						bpm_ctrl.count = 0x00;

					}else{
						utl_change_state(&pmp_ctrl, PMP_STATE_OFF);				// change state : PUMP OFF
						bpm_ctrl.phase = 0x08;									// next phase
						bpm_ctrl.count = 0x00;
					}
				}
			}
			
			// ***** LCD update *****
			if(bpm_manage.countdown5ms % M_T5MS_COUNT_500MS == 0){				// interval 500ms(for reference)
//				lcd_measuring_press = (sint16)bpm_calc.data_ave;
				lcd_data.press = (uint8)(bpm_calc.data_ave/100);
				lcd_ctrl.mode  = LCD_MODE_MSR_UPDATE;
			}

			// ***** Error *****
			if(bpm_manage.countdown5ms==0){										// timeout ?
				// not wear the Cuff or break the tube
				bpm_manage.errorCode = BPM_ERROR_TIMEOUT1;						// EC:Timeout Error
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;
			}
			break;

		case 0x08:
		// -----------------------------
		// (08) half open the solenoid valve            
		// -----------------------------
			utl_change_state(&vlv_ctrl, VLV_STATE_LEAK);						// change state : VAVLE LEAK
			bpm_ctrl.phase = 0x09;												// next phase
			bpm_ctrl.count = 0x00;

			bpm_manage.countdown5ms = M_T5MS_COUNT_100MS;						// wait 100ms(for reference)

			utl_memory_clr(&bpm_bpm_para, sizeof(bpm_bpm_para));				// clear blood pressure monitor data
			utl_memory_clr(&bpm_prt_para, sizeof(bpm_prt_para));				// clear pulse rate data
			utl_memory_clr(&bpm_bva_para, sizeof(bpm_bva_para));				// clear vascular hardness data
			break;

		case 0x09:
		// -----------------------------
		// (09) wait until the pressure is stable
		// -----------------------------
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_ctrl.phase = 0x0A;											// next phase
				bpm_ctrl.mode  = 0x00;
				
				bpm_manage.countdown5ms = M_T5MS_COUNT_120S;					// Set timeout 120s(for reference)
				bpm_bpm_para.time = bpm_manage.countdown5ms;
				bpm_pulse_status.BYTE = 0x00;									// clear flag
				bpm_bpm_para.base = BPM_PRESS_MAX_LIMIT;
				bpm_bpm_para.peak = BPM_PRESS_MAX_LIMIT;

				bpm_bpm_para.rise[0] = BPM_PRESS_MAX_LIMIT;						// bug fix
				bpm_bpm_para.rise[1] = BPM_PRESS_MAX_LIMIT;

				b_BPM_PULSE_START = ON;
			}
			break;

		case 0x0A:
		// -----------------------------
		// (0A) measure the blood pressure
		// -----------------------------
			if(func_read_adc_data(&data) == FUNC_ADC_READY){					// read ADC data

				// convert the pressure to ADC value
				bpm_calc.data_ave = func_calc_pressure(data, func_coeff.spt25x, func_coeff.zero);
					bpm_ctrl.count++;
					if(bpm_ctrl.count == BPM_MEAS_INTERVAL){
						bpm_get_pulse_data((sint16)bpm_calc.data_ave);				// masure the pulse rate
						bpm_ctrl.count = 0;
					}

					if(bpm_calc.data_ave < BPM_PRESS_1ST_LEAK_LIMIT){
						bpm_ctrl.mode++;
						if(bpm_ctrl.mode >= (uint8)BPM_VALVE_LEAK_KEEP){			// 
							bpm_ctrl.mode = 0x00;
							vlv_ctrl.mode = 1;										// request a change of duty ratio
						}
					}
			}

			// ***** LCD update *****
			// request the display of the pressure
			if(bpm_manage.countdown5ms%100 == 0){								// interval 500ms(for reference)
//				lcd_measuring_press = (sint16)bpm_calc.data_ave;
				lcd_data.press = (uint8)(bpm_calc.data_ave/100);
				lcd_ctrl.mode  = LCD_MODE_MSR_UPDATE;
			}
			// ***** end *****
			if(b_BPM_PULSE_END){												// end of the measurement
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;
			}
			if(bpm_manage.countdown5ms == 0){									// timeout
				bpm_manage.errorCode = BPM_ERROR_TIMEOUT2;						// EC:Timeout Error
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;

			}
			if(	(bpm_bpm_para.count <= 2) && 
				(bpm_calc.data_ave < BPM_PRESS_NO_BEAT)){
				bpm_manage.errorCode = BPM_ERROR_NO_BEAT;						// EC:No Beat Error
				bpm_ctrl.phase = 0xFF;											// end phase
				bpm_ctrl.count = 0x00;
			}
			break;

		case 0xFF:
		// -----------------------------
		// (FF) 廔椆張棟
		// -----------------------------
			if(pmp_ctrl.state != PMP_STATE_OFF){
				utl_change_state(&pmp_ctrl, PMP_STATE_OFF);						// change state : PUMP OFF
			}
			if(vlv_ctrl.state != VLV_STATE_OPEN){
				utl_change_state(&vlv_ctrl, VLV_STATE_OPEN);					// change state : VALVE OPEN
			}
			cmd_exec_spi_protocol(CMD_STOP, 0x00);								// execute STOP commad
			
			utl_change_state(&bpm_ctrl, BPM_STATE_RESULT);						// change state : BPM RESULT
			break;
		default:
			break;
	}
}



/*!
//==============================================================================
	@brief		Blood Pressure Monitor Result State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_result(void)
{
	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) Error Check
		// -----------------------------
			if(bpm_manage.errorCode==BPM_ERROR_NO_ERROR){						// NO ERROR ?
				bpm_ctrl.phase = 0x01;											// next phase
			}else{
				bpm_ctrl.phase = 0xFF;
			}
			utl_change_state(&key_ctrl, KEY_STATE_DISABLE);						// change state : KEY DISABLE
			break;

		case 0x01:
		// -----------------------------
		// (01) calculate the blood pressure
		// -----------------------------
			bpm_calc_blood_pressure();
			bpm_ctrl.phase++;													// next phase
			break;

		case 0x02:
		// -----------------------------
		// (02) calculate the blood vessel age
		// -----------------------------
			bpm_calc_blood_vessel_age();
			bpm_ctrl.phase++;													// next phase
			break;

		case 0x03:
		// -----------------------------
		// (03) calculate the vascular hardness
		// -----------------------------
			bpm_calc_vascular_hardness();
			if(log_info.BIT.full){
				utl_change_state(&log_ctrl, LOG_STATE_ERASE);					// change state : LOG ERASE
			}
			bpm_ctrl.phase++;													// next phase
			break;

		case 0x04:
		// -----------------------------
		// (04) update the measurement data & request the FALSH ROM save
		// -----------------------------
			if(log_ctrl.state == LOG_STATE_READY){
				bpm_update_blood_pressure();
				utl_change_state(&log_ctrl, LOG_STATE_WRITE);					//  change state : LOG ERASE
				bpm_ctrl.phase++;												// next phase
			}
			break;

		case 0x05:
		// -----------------------------
		// (05) wait for saving to FLASH ROM
		// -----------------------------
			if(log_ctrl.state == LOG_STATE_READY){
				bpm_ctrl.phase = 0xFF;
			}
			break;

		case 0xFF:
		// -----------------------------
		// (FF) END
		// -----------------------------
			utl_change_state(&bpm_ctrl, BPM_STATE_DISPLAY);						// change state : BPM DIPSLAY
			break;
		default:
			break;
	}
}


/*!
//==============================================================================
	@brief		Blood Pressure Monitor Display State
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_display(void)
{
	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) 昞帵儕僋僄僗僩僠僃僢僋
		// -----------------------------
			if(bpm_manage.errorCode == BPM_ERROR_NO_ERROR){						// NO ERROR ?
				utl_change_state(&lcd_ctrl, LCD_STATE_RESULT);					// change state : LCD RESULT
				bpm_ctrl.phase = 0x01;
			}else if(bpm_manage.errorCode == BPM_ERROR_EMERGENCY){				// EMERGENCY ?
				bpm_ctrl.phase = 0xFF;											// end
			}else{
				utl_change_state(&lcd_ctrl, LCD_STATE_ERROR);					// change state : LCD ERROR
				bpm_ctrl.phase = 0x11;
			}
			utl_change_state(&key_ctrl, KEY_STATE_ENABLE);						// change state : KEY ENABLE
			break;

		case 0x01:
		// -----------------------------
		// (01) 嵟怴儘僌偺弨旛
		// -----------------------------
			log_ctrl.state = LOG_STATE_READ;
			log_ctrl.phase = 0x00;
			log_ctrl.count++;
			log_ctrl.mode  = 0x00;
			
			bpm_ctrl.phase = 0x02;
			break;

		case 0x02:
		// -----------------------------
		// (02) 儘乕僪懸偪
		// -----------------------------
			// wait for the log processing to end	
			if(log_ctrl.state != LOG_STATE_READY){
				return;
			}
			if(b_LOG_NO_DATA){
				bpm_ctrl.phase = 0xFF;
			}else{
				lcd_data.month     = log_data.month;
				lcd_data.day       = log_data.day;
				lcd_data.hour      = log_data.hour;
				lcd_data.minute    = log_data.minute;
				lcd_data.systolic  = log_data.bp_max;							//!< LOG: SYSTOLIC BLOOD PRESSURE
				lcd_data.diastolic = log_data.bp_min;							//!< LOG: DIASTOLIC BLOOD PRESSURE
				lcd_data.rate      = log_data.rate;								//!< LOG: BEAT RATE
				lcd_data.age       = log_data.age;								//!< LOG: BLOOD VESSEL AGE

				lcd_ctrl.mode      = LCD_MODE_RSL_UPDATE;
				
				bpm_manage.countdown5ms = M_T5MS_COUNT_10S;						//  reset interval timer

				bpm_ctrl.phase = 0x03;
			}
			break;

		case 0x03:
		// -----------------------------
		// (03) 昞帵
		// -----------------------------
			if(bpm_manage.countdown5ms == 0x00){								// TIMEOUT ?
				bpm_ctrl.phase = 0xFF;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_DSP_END){				// request display end
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;						//  clear request flag
				bpm_ctrl.phase = 0xFF;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_DSP_SHIFT){			// request display shift
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;						//  clear request flag
				bpm_manage.countdown5ms = M_T5MS_COUNT_10S;						//  reset interval timer
				lcd_ctrl.phase++;												//  next phase
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_DSP_BACK){			// request display back
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;						//  clear request flag
				bpm_ctrl.phase = 0x04;
			}
			break;

		case 0x04:
		// -----------------------------
		// (04) 夁嫀儘僌傪儘乕僪弨旛
		// -----------------------------
			log_ctrl.state = LOG_STATE_READ;
			log_ctrl.phase = 0x00;
			log_ctrl.count++;
			log_ctrl.mode  = 0x01;

			bpm_ctrl.phase = 0x02;
			break;

		case 0x11:
		// -----------------------------
		// (11) set ERROR CODE
		// -----------------------------
			bpm_ctrl.phase = 0x12;
			bpm_manage.countdown5ms = M_T5MS_COUNT_10S;							//昞帵僞僀儅乕傪嵞愝掕
			lcd_data.error = bpm_manage.errorCode;
			lcd_ctrl.mode  = LCD_MODE_ERR_UPDATE;
			break;

		case 0x12:
		// -----------------------------
		// (12) display ERROR CODE
		// -----------------------------
			if(bpm_manage.countdown5ms == 0x00){								// TIMEOUT ?
				bpm_ctrl.phase = 0xFF;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_DSP_END){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;						//昞帵撪梕偺愗懼梫媮僋儕傾
				bpm_ctrl.phase = 0xFF;
			}
			break;
		case 0xFF:
		// -----------------------------
		// (FF) END
		// -----------------------------
			utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);					// change state : BPM IDLE
			break;
		default:
			break;
	}
}


/*!
//==============================================================================
	@brief		Blood Pressure Monitor Adjust Clock State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	帪寁崌偣偺忬懺


//==============================================================================
*/
void bpm_adjust(void)
{
	uint8 mday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00)弶婜壔
		// -----------------------------
			utl_change_state(&lcd_ctrl, LCD_STATE_CLOCK);						// change state : LCD 

			lcd_data.year   = bpm_currentTime.year;
			lcd_data.month  = bpm_currentTime.month;
			lcd_data.day    = bpm_currentTime.day;
			lcd_data.hour   = bpm_currentTime.hour;
			lcd_data.minute = bpm_currentTime.minute;

			lcd_ctrl.phase = LCD_PHASE_CLK_YEAR;
			lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			bpm_ctrl.phase = 0x01;
			break;

		case 0x01:
		// -----------------------------
		// (01) YEAR/START:month/MEMORY:countup/SELECT:countdown
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_NEXT){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_ctrl.phase++;												//next phase
				lcd_ctrl.phase = LCD_PHASE_CLK_MONTH;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_UP){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				lcd_data.year++;
				if(lcd_data.year > 99){
					lcd_data.year = 0;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_YEAR;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_DOWN){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.year > 0){
					lcd_data.year--;
				}else{
					lcd_data.year = 99;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_YEAR;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}
			break;

		case 0x02:
		// -----------------------------
		// (02) MONTH/START:day/MEMORY:countup/SELECT:countdown
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_NEXT){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_ctrl.phase++;												//next phase
				lcd_ctrl.phase = LCD_PHASE_CLK_DAY;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_UP){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				lcd_data.month++;
				if(lcd_data.month > 12){
					lcd_data.month = 1;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_MONTH;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_DOWN){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.month > 1){
					lcd_data.month--;
				}else{
					lcd_data.month = 12;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_MONTH;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}
			break;

		case 0x03:
		// -----------------------------
		// (03) DAY/START:hour/MEMORY:countup/SELECT:countdown
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_NEXT){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_ctrl.phase++;												// next phase
				lcd_ctrl.phase = LCD_PHASE_CLK_HOUR;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_UP){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.year%4 == 0){										// leap year ?
					mday[1]=29;
				}
				lcd_data.day++;
				if(lcd_data.day > mday[lcd_data.month-1]){
					lcd_data.day = 1;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_DAY;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_DOWN){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.year%4 == 0){										// leap year ?
					mday[1]=29;
				}
				if(lcd_data.day > 1){
					lcd_data.day--;
				}else{
					lcd_data.day = mday[lcd_data.month-1];
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_DAY;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}
			break;

		case 0x04:
		// -----------------------------
		// (04) HOUR/START:minute/MEMORY:countup/SELECT:countdown
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_NEXT){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_ctrl.phase++;												// next phase
				lcd_ctrl.phase = LCD_PHASE_CLK_MIN;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_UP){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				lcd_data.hour++;
				if(lcd_data.hour > 23){											// 
					lcd_data.hour = 0;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_HOUR;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_DOWN){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.hour > 0){											// 
					lcd_data.hour--;
				}else{
					lcd_data.hour = 23;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_HOUR;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}
			break;

		case 0x05:
		// -----------------------------
		// (05) MINUTE/START:decide/MEMORY:countup/SELECT:countdown
		// -----------------------------
			if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_NEXT){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				bpm_ctrl.phase = 0x06;
			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_UP){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				lcd_data.minute++;
				if(lcd_data.minute > 59){										// 
					lcd_data.minute = 0;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_MIN;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;

			}else if(bpm_manage.keyRequest == BPM_KEY_REQ_ADJ_DOWN){
				bpm_manage.keyRequest = BPM_KEY_REQ_NONE;
				if(lcd_data.minute > 0){										// 
					lcd_data.minute--;
				}else{
					lcd_data.minute = 59;
				}
				lcd_ctrl.phase = LCD_PHASE_CLK_MIN;
				lcd_ctrl.mode  = LCD_MODE_CLK_UPDATE;
			}
			break;

		case 0x06:
		// -----------------------------
		// (06) 帪寁偁傢偣姰椆仺RTC愝掕
		// -----------------------------
			if(rtc_ctrl.state == RTC_STATE_IDLE){

				bpm_currentTime.year   = lcd_data.year;
				bpm_currentTime.month  = lcd_data.month;
				bpm_currentTime.day    = lcd_data.day;
				bpm_currentTime.hour   = lcd_data.hour;
				bpm_currentTime.minute = lcd_data.minute;

				utl_change_state(&rtc_ctrl, RTC_STATE_SET);						// change state : RTC SET
				bpm_ctrl.phase = 0xFF;
			}
			break;
		case 0xFF:
		// -----------------------------
		// (FF) 帪寁偁傢偣姰椆懸偪
		// -----------------------------
			if(rtc_ctrl.state == RTC_STATE_IDLE){
				utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);					// change state : BPM IDLE
			}
			break;
		default:
			break;
	}
}



/*!
//==============================================================================
	@brief		Blood Pressure Monitor Sleep State
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_sleep(void)
{
	switch(bpm_ctrl.phase){
		case 0x00:
		// -----------------------------
		// (00) stop OnChip OSC 
		// -----------------------------
			CSS = 1;															// fCLK = fSUB 

			HIOSTOP = 1;														// stop OnChip OSC

			bpm_manage.interval5ms = M_T5MS_COUNT_250MS;						// interval 250ms(for reference)

			if(key_ctrl.state != KEY_STATE_ENABLE){
				utl_change_state(&key_ctrl, KEY_STATE_ENABLE);					// change state : KEY ENABLE
			}
			if(lcd_ctrl.state != LCD_STATE_CLOCK){
				utl_change_state(&lcd_ctrl, LCD_STATE_CLOCK);					// change state : LCD CLOCK
			}
			
			bpm_ctrl.mode = 0x00;
			if(bpm_manage.battStatus == BPM_BATT_NONE){
				P_BPM_LED_BATT_ON();
				bpm_ctrl.mode = 0x01;
				utl_change_state(&lcd_ctrl, LCD_STATE_ERROR);					// change state : LCD ERROR
				lcd_ctrl.mode = LCD_MODE_ERR_NOBATT;

			}else if(bpm_manage.battStatus == BPM_BATT_LOW){
				P_BPM_LED_BATT_ON();
				bpm_ctrl.mode = 0x02;
				utl_change_state(&lcd_ctrl, LCD_STATE_ERROR);					// change state : LCD ERROR
				lcd_ctrl.mode = LCD_MODE_ERR_LOBATT;
			}
			
			bpm_ctrl.phase = 0x01;
			break;

		case 0x01:
		// -----------------------------
		// (01) idle
		// -----------------------------
			if(bpm_manage.keyRequest != BPM_KEY_REQ_NONE){						// check KEY REQUEST ?
				P_BPM_LED_BATT_OFF();
				bpm_ctrl.phase = 0xFF;
				bpm_ctrl.mode  = 0x00;

			}else if(bpm_manage.interval5ms == 0x00){							// check interval 250ms ?
				bpm_manage.interval5ms = M_T5MS_COUNT_250MS;
				bpm_ctrl.count++;
				if(bpm_ctrl.count >= 40){										// 10s ?
					bpm_ctrl.count = 0;
					bpm_ctrl.mode = 0x00;										// end LED ACTION
					if(lcd_ctrl.state != LCD_STATE_CLOCK){
						utl_change_state(&lcd_ctrl, LCD_STATE_CLOCK);			// change state : LCD CLOCK
					}
				}

				if(bpm_ctrl.count%4 == 0){
					utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);				//  change state : RTC 
					if(bpm_ctrl.mode == 1){										//  request LED ACTION
						P_BPM_LED_BATT_ON();
						lcd_ctrl.mode = LCD_MODE_ERR_NOBATT;
					}else if(bpm_ctrl.mode == 2){								//  request LED ACTION
						P_BPM_LED_BATT_ON();
						lcd_ctrl.mode = LCD_MODE_ERR_LOBATT;
					} 

				}else if(bpm_ctrl.count%4 == 1){
					lcd_data.year   = bpm_currentTime.year;
					lcd_data.month  = bpm_currentTime.month;
					lcd_data.day    = bpm_currentTime.day;
					lcd_data.hour   = bpm_currentTime.hour;
					lcd_data.minute = bpm_currentTime.minute;

					lcd_ctrl.mode = LCD_MODE_CLK_UPDATE;

					if(bpm_ctrl.mode == 1){										//  request LED ACTION
						P_BPM_LED_BATT_OFF();
						lcd_ctrl.mode = LCD_MODE_ERR_NOBATT;
					}else if(bpm_ctrl.mode==2){									//  request LED ACTION
						P_BPM_LED_BATT_ON();
						lcd_ctrl.mode = LCD_MODE_ERR_LOBATT;
					} 

				}else if(bpm_ctrl.count%4 == 2){
					utl_change_state(&rtc_ctrl, RTC_STATE_LOAD);				//  change state : RTC 

					if(bpm_ctrl.mode==1){										//  request LED ACTION
						P_BPM_LED_BATT_ON();
						lcd_ctrl.mode = LCD_MODE_ERR_NOBATT;
					}else if(bpm_ctrl.mode==2){									//  request LED ACTION
						P_BPM_LED_BATT_OFF();
						lcd_ctrl.mode = LCD_MODE_ERR_LOBATT;
					} 

				}else{
					lcd_data.year   = bpm_currentTime.year;
					lcd_data.month  = bpm_currentTime.month;
					lcd_data.day    = bpm_currentTime.day;
					lcd_data.hour   = bpm_currentTime.hour;
					lcd_data.minute = bpm_currentTime.minute;

					lcd_ctrl.mode = LCD_MODE_CLK_BLINK;

					if(bpm_ctrl.mode==1){										//  request LED ACTION
						P_BPM_LED_BATT_OFF();
						lcd_ctrl.mode = LCD_MODE_ERR_NOBATT;
					}else if(bpm_ctrl.mode==2){									//  request LED ACTION
						P_BPM_LED_BATT_OFF();
						lcd_ctrl.mode = LCD_MODE_ERR_LOBATT;
					} 

				}
			}
			break;
		case 0xFF:
		// -----------------------------
		// (FF) start OnChip OSC
		// -----------------------------
			HIOSTOP = 0;														// start OnChip OSC
			CSS = 0;															// fCLK = fMAIN
			utl_change_state(&bpm_ctrl, BPM_STATE_IDLE);						// change state : BPM IDLE
			break;

		default:
			break;
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
	@brief		Detection Pulse Module

	@param		data : Pressure Data [x100mmHg]
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module detects the pulse wave.\n
	The pulse wave is the wave from the pressure rizing up to the next one.\n
	The base pressure is the pressure when the first rizinig up is detected.\n
	The pulse level is the difference between the peak level and the base leve in the pulse wave.\n
	When the pulse level is max value, the value is stored as the max pulse level.\n
	When the measured pressure is lower than the pressure of the max pulse level \n
	and the pulse level is lower than the 50% of the max pulse level,\n
	the measured pressure(i.e. Diastolic Blood Pressure) is stored and the measurement is ended.\n

//==============================================================================
*/
#endif



void bpm_get_pulse_data(sint16 data)
{
	sint32 time;
	uint16 p_level;
	uint16 p_level2;
    uint8 cnt;
    
	flag8bit_t skip_flag;

	// ---------------------------------
	// update the maximum pressure value
	// ---------------------------------
	if(data > bpm_bpm_para.peak){
		bpm_bpm_para.peak = data;
	}
	p_level = bpm_bpm_para.base;
	p_level2 = data;

	// ---------------------------------
	// detect the pulse rise up
	// ---------------------------------
	if( (p_level2             < p_level) &&										// smaller than the last based value
		(bpm_bpm_para.rise[0] < bpm_bpm_para.rise[1]) &&						// check the pressure rise up
		(bpm_bpm_para.rise[1] < data) &&										// check the pressure rise up
		(b_BPM_PULSE_RISE  == OFF) ){											// not detect the pulse rise up

		if(b_BPM_PULSE_START){													// the first pulse
			// initialize
			bpm_bpm_para.count   = 0;
			bpm_bpm_para.pl[0]   = 0;
			bpm_bpm_para.plp[0]  = bpm_bpm_para.peak;
			bpm_prt_para.save[0] = 0;
			bpm_bpm_para.pl_max  = 0;
			b_BPM_PULSE_START    = OFF;
		}else{
			// prevent the memory overflow
			if(bpm_bpm_para.count  >= (BPM_PULSE_NUM-2)){
				bpm_bpm_para.count = BPM_PULSE_NUM-2;
			}

			skip_flag.BYTE = 0x00;												// flag initialize

			p_level = (uint16)(bpm_bpm_para.peak - bpm_bpm_para.base);			// pulse level

			// to prevent devide-by-zero
			if(bpm_bpm_para.time > bpm_manage.countdown5ms){
				// calculate the Pulse Rate
				time = utl_round32(M_T5MS_COUNT_60S, (sint32)(bpm_bpm_para.time - bpm_manage.countdown5ms));
				bpm_prt_para.save[bpm_bpm_para.count] = (uint8)time;			// store Pulse Rate
			}else{
				// to perform illegal procesing
				time = BPM_HEART_RATE_MAX + 1;
			}

			// Arrhythmia(Irregular pulse) check
			if(time < BPM_HEART_RATE_MIN || time > BPM_HEART_RATE_MAX){
				skip_flag.BIT.B0 = ON;											// error flag on
			}
			// Body Motion check
			if(bpm_bpm_para.count){												// n-th pulse
				if( (p_level < bpm_bpm_para.pl[bpm_bpm_para.count]/2)			// 50%(for reference)
				  ||(p_level > bpm_bpm_para.pl[bpm_bpm_para.count]*2) ) {		// 200%(for reference)
					skip_flag.BIT.B1 = ON;										// error flag on
				}
			}else{																// 1st pulse
				if( (p_level < BPM_PULSE_1ST_MIN)
				  ||(p_level > BPM_PULSE_1ST_MAX) ){
					skip_flag.BIT.B2 = ON;										// error flag on
				}
			}

			// --------------------------------------------------
			// detect the valid pulse data
			// --------------------------------------------------
			if(skip_flag.BYTE == 0x00){
				bpm_bpm_para.count++;											// countup: number of the detected pulse
				bpm_bpm_para.pl[bpm_bpm_para.count] = p_level;					// store the pulse level
				bpm_bpm_para.plp[bpm_bpm_para.count] = bpm_bpm_para.peak;		// store the pressure of the timing detected the peak value

				// update the pulse maximum level
				if(p_level > bpm_bpm_para.pl_max){
					bpm_bpm_para.pl_max = p_level;								// store the max level of the pulse levels
					bpm_bpm_para.pl_max_num = bpm_bpm_para.count;				// store the pulse wave number detected max pulse level

					// store the pulse wave of the max pulse level
					for(cnt=0; cnt<BPM_VALID_PRESS_DATA; cnt++){
						bpm_bva_para.ph_max[cnt] = bpm_bva_para.ph_temp[cnt];
					}
					bpm_bva_para.ph_min = bpm_bpm_para.rise[0];					// store the base pressure of pulse wave detected max pulse level
					bpm_bva_para.period = bpm_bva_para.count+1;					// store the sampling number of pulse wave detected max pulse level
				}
				// judge the measurement end 
				if(	(bpm_bpm_para.pl_max > BPM_PULSE_PEAK_LEVEL) &&				// detect the max pulse level is valid(for reference)
					(p_level < bpm_bpm_para.pl_max * BPM_DIASTOLIC_BP_TH / BPM_BP_RESOLUTION) ){	// detect 50亾 of the max pulse level(for reference)
					b_BPM_PULSE_END = ON;										// decide the measurement end
				}
			}
		}
			
		// update the measuring data
		bpm_bpm_para.time = bpm_manage.countdown5ms;							// store the time detected pulse rise edge
		bpm_bpm_para.base = bpm_bpm_para.rise[0];								// update the base pressure of pulse wave
		bpm_bpm_para.peak = data;												// initialize the max pressure
		b_BPM_PULSE_RISE  = ON;													// FLAG: pulse rise edge detection flag ON
		b_BPM_PULSE_BEAT  = ON;													// FLAG: pulse detection flag ON

		utl_memory_clr(&bpm_bva_para.ph_temp[0], sizeof(bpm_bva_para.ph_temp));	// clear last pulse wave
		bpm_bva_para.ph_temp[0] = bpm_bpm_para.rise[0];							// restart the pulse detection
		bpm_bva_para.ph_temp[1] = bpm_bpm_para.rise[1];
		bpm_bva_para.count = 2;

	}else{
		// ---------------------------------
		// don't detect the pulse rise up
		// ---------------------------------
		if(data < BPM_PRESS_MIN_LIMIT){											// detect the lower limit pressure
			bpm_bpm_para.count++;												// 
			bpm_bpm_para.pl[bpm_bpm_para.count]   = 0x00;						// 
			bpm_bpm_para.plp[bpm_bpm_para.count]  = bpm_bpm_para.peak;			// 
			bpm_prt_para.save[bpm_bpm_para.count] = 0x00;						// 
			b_BPM_PULSE_END = ON;												// decide the measurement end
		}
	}

	// prevent the memory overflow: 
	// After the pulse rizing up is detected, the pressure data is stored up to number of "BPM_VALID_PRESS_DATA".
	if(bpm_bva_para.count < BPM_VALID_PRESS_DATA){
		bpm_bva_para.ph_temp[bpm_bva_para.count] = data;
	}
	if(bpm_bva_para.count < BPM_LIMIT_PRESS_DATA){
		bpm_bva_para.count++;
	}

	// ---------------------------------
	// update the pulse rizing up
	// ---------------------------------
	bpm_bpm_para.rise[0] = bpm_bpm_para.rise[1];
	bpm_bpm_para.rise[1] = data;

	// ---------------------------------
	// prepare to detect the next pulse rizing up
	// ---------------------------------
	p_level = bpm_bpm_para.base;
	
	if(data < p_level && b_BPM_PULSE_RISE){
		b_BPM_PULSE_RISE = OFF;													// FLAG: pulse rise edge detection flag ON
	}
}







#if 0
/*!
//==============================================================================
	@brief		Calculation Blood Pressure Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module calculates the Blood Pressure and the Pulse Rate.\n

//==============================================================================
*/
void bpm_calc_blood_pressure(void)
{
	uint32 temp;
	uint16 thresh_max;
	uint16 pr_ave;
	uint8 pr_max;
	uint8 pr_min;
	uint8 cnt;
	
	thresh_max = bpm_bpm_para.pl_max * BPM_SYSTOLIC_BP_TH / BPM_BP_RESOLUTION;

	// ---------------------------------
	// calculate Systolic Blood Pressure
	// ---------------------------------
	cnt = 0;
	while(bpm_bpm_para.pl[++cnt] < thresh_max && cnt < bpm_bpm_para.pl_max_num){};
#if(0)
	bpm_bpm_para.bp_max = bpm_bpm_para.plp[cnt];
#else
	temp = (bpm_bpm_para.pl[cnt] - thresh_max)*(bpm_bpm_para.plp[cnt-1] - bpm_bpm_para.plp[cnt]);
	temp/= (bpm_bpm_para.pl[cnt] - bpm_bpm_para.pl[cnt-1]);
	
	bpm_bpm_para.bp_max = bpm_bpm_para.plp[cnt] + (uint16)temp;

#endif

	// ---------------------------------
	// calculate Diastolic Blood Pressure
	// ---------------------------------
	// Diastoric Blood Pressure is stored at the end of the memory
	bpm_bpm_para.bp_min = bpm_bpm_para.plp[bpm_bpm_para.count];

	// ---------------------------------
	// calculate Pulse Rate
	// ---------------------------------
	pr_ave = 0;
	pr_max = 0;
	pr_min = 255;
	// calculate the mean value except for first, last, max and min
	for(cnt = 1; cnt < bpm_bpm_para.count; cnt++){								// except for first & last
		pr_ave += (uint16)bpm_prt_para.save[cnt];
		if(bpm_prt_para.save[cnt] >= pr_max) pr_max = bpm_prt_para.save[cnt];
		if(bpm_prt_para.save[cnt] <= pr_min) pr_min = bpm_prt_para.save[cnt];
	}
	pr_ave -= ((uint16)pr_max + (uint16)pr_min);										// except for max & min
	bpm_prt_para.ave = (uint8)(pr_ave / (bpm_bpm_para.count-3));

}


/*!
//==============================================================================
	@brief		Calculation Blood Vessel Age Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module calculates Blood Vessel Age.\n
	Blood Vessel Age is calculated based on the 2nd order derivative pulse waves.\n
	After detecting a,b,c,d and e-wave in the 2rd order derivative pulse waves,\n
	the Aging Index is calculated as follows: X=(b-c-d-e)/a. \n
	Blood Vessel Age is calculated as follows: age = 43.478*X+67.391.
	@attention
	These calculating formulas are quoted from following homepage.
	- http://was.fukuda.co.jp/medical/solution/blood_vessel/outline.html
	- http://qubiterz.com/silver-net/kenk0013.htm
	@attention
	Please note, however, that we provide no support for algorithms to calculate blood vessel age.\n
	Please conduct close study by your company before implementing software.

//==============================================================================
*/
void bpm_calc_blood_vessel_age(void)
{
	sint32_st age;
	sint16 move[BPM_MOVE_AVE_NUM];
	sint16 moveSUM[3];
	sint16 moveDIF;

	uint8 ii;
	uint8 jj;
	uint8 data_end_num;

	// ---------------------------------
	// chech the number of the valid data
	// ---------------------------------
	if(bpm_bva_para.period<BPM_VALID_PRESS_DATA){
		data_end_num = bpm_bva_para.period;
	}else{
		data_end_num = BPM_VALID_PRESS_DATA;
	}

	// ---------------------------------
	// calculate the moving average
	// ---------------------------------
	for(ii=0;ii<BPM_MOVE_AVE_NUM;ii++){
		move[ii] = 0;															// m[0:N-1] = x[0]-x[0]
	}
	bpm_bva_para.ph_temp[0] = 0;												// s[0] = m[0]+乧+m[N-1] = 0*N = 0
	
	for(ii=1; ii<data_end_num; ii++){
		moveDIF = bpm_bva_para.ph_max[ii] - bpm_bva_para.ph_max[0];				// m[0]=x[n]-x[0]

		bpm_bva_para.ph_temp[ii] = (bpm_bva_para.ph_temp[ii-1] - move[BPM_MOVE_AVE_NUM-1] + moveDIF);
																				// s[n] = s[n-1] - m[N] + m[0]

		for(jj=BPM_MOVE_AVE_NUM-1;jj>0;jj--){
			move[jj] = move[jj-1];												// m[n] = m[n-1]
		}
		move[0] = moveDIF;														// update m[0]
	}

	// ---------------------------------
	// calculate the 2nd order derivative pulse waves
	// ---------------------------------
	for(ii=0;ii<data_end_num-1;ii++){
		bpm_bva_para.ph_max[ii] = bpm_bva_para.ph_temp[ii+1] - bpm_bva_para.ph_temp[ii];
	}
	for(ii=0;ii<data_end_num-2;ii++){
		bpm_bva_para.ph_temp[ii] = bpm_bva_para.ph_max[ii+1] - bpm_bva_para.ph_max[ii];
	}

	// ---------------------------------
	// calculate the moving average
	// ---------------------------------
	for(ii=0;ii<BPM_MOVE_AVE_NUM;ii++){
		move[ii] = bpm_bva_para.ph_temp[0];										// initialize m[0:N]
	}
	bpm_bva_para.ph_max[0] = bpm_bva_para.ph_temp[0] * BPM_MOVE_AVE_NUM;		// s[0] = m[0]*N = m[0]+乧+m[N-1]

	for(ii=1;ii<data_end_num-2;ii++){
		bpm_bva_para.ph_max[ii] = (bpm_bva_para.ph_max[ii-1] - move[BPM_MOVE_AVE_NUM-1] + bpm_bva_para.ph_temp[ii]);
																				// s[n] = s[n-1] - m[N] + m[0]

		for(jj=BPM_MOVE_AVE_NUM-1;jj>0;jj--){
			move[jj] = move[jj-1];												// m[n] = m[n-1]
		}
		move[0] = bpm_bva_para.ph_temp[ii];										// update m[0]
	}

	// ---------------------------------
	// calculate the 3rd order derivative pulse waves
	// ---------------------------------
	for(ii=0;ii<data_end_num-3;ii++){
		bpm_bva_para.ph_temp[ii] = bpm_bva_para.ph_max[ii+1] - bpm_bva_para.ph_max[ii];
	}

	// ---------------------------------
	// calculate the moving average
	// ---------------------------------
	moveSUM[2]=0;
	moveSUM[1]=0;
	for(ii=0;ii<5;ii++){
		moveSUM[1] += bpm_bva_para.ph_temp[ii];
	}
	
	jj=0;

	// ---------------------------------
	// detect a,b,c,d and e-wave in the 2rd order derivative pulse waves
	// ---------------------------------
	for(ii=3;ii<data_end_num-5;ii++){

		moveSUM[0] = moveSUM[1] - bpm_bva_para.ph_temp[ii-3] + bpm_bva_para.ph_temp[ii+2];

		if(moveSUM[1]==0){
			if(moveSUM[0]<0 && moveSUM[2]>0){
				if(jj==0){
					bpm_bva_para.wave_a = bpm_bva_para.ph_max[ii-1];
					jj++;
				}else if(jj==2){
					bpm_bva_para.wave_c = bpm_bva_para.ph_max[ii-1];
					jj++;
				}else if(jj==4){
					bpm_bva_para.wave_e = bpm_bva_para.ph_max[ii-1];
					jj++;
				}
			}
			if(moveSUM[0]>0 && moveSUM[2]<0){
				if(jj==1){
					bpm_bva_para.wave_b = bpm_bva_para.ph_max[ii-1];
					jj++;
				}else if(jj==3){
					bpm_bva_para.wave_d = bpm_bva_para.ph_max[ii-1];
					jj++;
				}
			}
		}else{
			if(moveSUM[0]<0 && moveSUM[1]>0){
				if(jj==0){
					bpm_bva_para.wave_a = bpm_bva_para.ph_max[ii];
					jj++;
				}else if(jj==2){
					bpm_bva_para.wave_c = bpm_bva_para.ph_max[ii];
					jj++;
				}else if(jj==4){
					bpm_bva_para.wave_e = bpm_bva_para.ph_max[ii];
					jj++;
				}
			}
			if(moveSUM[0]>0 && moveSUM[1]<0){
				if(jj==1){
					bpm_bva_para.wave_b = bpm_bva_para.ph_max[ii];
					jj++;
				}else if(jj==3){
					bpm_bva_para.wave_d = bpm_bva_para.ph_max[ii];
					jj++;
				}
			}
		}

		moveSUM[2] = moveSUM[1];
		moveSUM[1] = moveSUM[0];

		if(jj>=5){
			// detect e-wave
			break;
		}
	}

	// ---------------------------------
	// calculate the 2rd order derivative pulse waves aging index
	// ---------------------------------
	// X=(b-c-d-e)/a
	moveSUM[0] = (bpm_bva_para.wave_b - bpm_bva_para.wave_c - bpm_bva_para.wave_d - bpm_bva_para.wave_e)*256/bpm_bva_para.wave_a;
	
	// ---------------------------------
	// calculate Blood Vessel Age
	// ---------------------------------
	// age = 43.478*X+67.391
	age.SDWORD = (sint32)moveSUM[0]*11130 + 4416537;
	bpm_bva_para.age = (sint16)age.WORD.HI;
	if(bpm_bva_para.age>127){
		bpm_bva_para.age = 127;
	}else if(bpm_bva_para.age<0){
		bpm_bva_para.age = 0;
	}
}


/*!
//==============================================================================
	@brief		Calculation Vascular hardness Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module calculates Vascular Hardness.
	@attention
	Simply, Vascular Hardness divides Blood Vessel Age into 8 steps.\n
	These thresholds are for reference only.\n
	Please note, however, that we provide no support for algorithms to calculate vascular hardness.\n
	Please conduct close study by your company before implementing software.

//==============================================================================
*/
void bpm_calc_vascular_hardness(void)
{
	// convert Blood Vessel Age into Vascular Hardness
	if(bpm_bva_para.age >= BPM_BVS_RANK_08){
		bpm_bva_para.age = 8;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_07){
		bpm_bva_para.age = 7;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_06){
		bpm_bva_para.age = 6;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_05){
		bpm_bva_para.age = 5;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_04){
		bpm_bva_para.age = 4;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_03){
		bpm_bva_para.age = 3;
	}else if(bpm_bva_para.age >= BPM_BVS_RANK_02){
		bpm_bva_para.age = 2;
	}else{
		bpm_bva_para.age = 1;
	}
	bpm_bva_para.age += 0x80;													// the 8th bit is HI
}


/*!
//==============================================================================
	@brief		Update Blood Pressure Data Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module updates the blood pressure data.

//==============================================================================
*/
void bpm_update_blood_pressure(void)
{
	log_data.month  = bpm_measuringTime.month;
	log_data.day    = bpm_measuringTime.day;
	log_data.hour   = bpm_measuringTime.hour;
	log_data.minute = bpm_measuringTime.minute;
	log_data.bp_max = (uint8)(bpm_bpm_para.bp_max/100);
	log_data.bp_min = (uint8)(bpm_bpm_para.bp_min/100);
	log_data.rate   = bpm_prt_para.ave;
	log_data.age    = (uint8)bpm_bva_para.age;
}


/*!
//==============================================================================
	@brief		Blood Pressure Monitor KEY Management Module
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_key_management(void)
{

	if(bpm_manage.keyRequest != BPM_KEY_REQ_NONE){								// wait for accepting KEY request
		return;
	}

	if(b_KEY_STATUS_START == ON){												// push START button
		b_KEY_STATUS_START = OFF;
		
		switch(bpm_ctrl.state){
			case BPM_STATE_IDLE:
				bpm_manage.keyRequest = BPM_KEY_REQ_START;
				break;
			case BPM_STATE_MEASURE:
				bpm_manage.keyRequest = BPM_KEY_REQ_STOP;
				break;
			case BPM_STATE_DISPLAY:			
				bpm_manage.keyRequest = BPM_KEY_REQ_DSP_END;
				break;
			case BPM_STATE_ADJUST:
				bpm_manage.keyRequest = BPM_KEY_REQ_ADJ_NEXT;
				break;
			case BPM_STATE_SLEEP:
				bpm_manage.keyRequest = BPM_KEY_REQ_START;
				break;
			default:
//			case BPM_STATE_INITIAL:
//			case BPM_STATE_RESULT:
//			case BPM_STATE_CONNECT:
				// no operation
				break;
		}
	}

	if(b_KEY_STATUS_MEMORY == ON){												// push MEMORY button
		b_KEY_STATUS_MEMORY = OFF;

		switch(bpm_ctrl.state){
			case BPM_STATE_IDLE:
				bpm_manage.keyRequest = BPM_KEY_REQ_DSP_START;
				break;
			case BPM_STATE_DISPLAY:
				bpm_manage.keyRequest = BPM_KEY_REQ_DSP_BACK;
				break;
			case BPM_STATE_ADJUST:
				bpm_manage.keyRequest = BPM_KEY_REQ_ADJ_UP;
				break;
			case BPM_STATE_SLEEP:
				bpm_manage.keyRequest = BPM_KEY_REQ_DSP_START;
				break;
			default:
//			case BPM_STATE_INITIAL:
//			case BPM_STATE_MEASURE:
//			case BPM_STATE_RESULT:
//			case BPM_STATE_CONNECT:
				// no operation
				break;
		}
	}

	if(b_KEY_STATUS_SELECT == ON){												// push SELECT button
		b_KEY_STATUS_SELECT = OFF;
	
		switch(bpm_ctrl.state){
			case BPM_STATE_IDLE:
			case BPM_STATE_SLEEP:
				bpm_manage.keyRequest = BPM_KEY_REQ_DATE;
				break;
			case BPM_STATE_DISPLAY:
				bpm_manage.keyRequest = BPM_KEY_REQ_DSP_SHIFT;
				break;
			case BPM_STATE_ADJUST:
				bpm_manage.keyRequest = BPM_KEY_REQ_ADJ_DOWN;
				break;
			default:
//			case BPM_STATE_INITIAL:
//			case BPM_STATE_MEASURE:
//			case BPM_STATE_RESULT:
//			case BPM_STATE_CONNECT:
				// no operation
				break;
		}
	}

	if(b_KEY_STATUS_ADJUST == ON){												// long push MEMORY button
		b_KEY_STATUS_ADJUST = OFF;

		switch(bpm_ctrl.state){
			case BPM_STATE_IDLE:
			case BPM_STATE_SLEEP:
				bpm_manage.keyRequest = BPM_KEY_REQ_ADJ_START;
				break;
			default:
//			case BPM_STATE_INITIAL:
//			case BPM_STATE_MEASURE:
//			case BPM_STATE_RESULT:
//			case BPM_STATE_CONNECT:
//			case BPM_STATE_DISPLAY:
//			case BPM_STATE_ADJUST:
				// no operation
				break;
		}
	}

	if(b_KEY_STATUS_ERASE == ON){												// long push START & SELECT button
		b_KEY_STATUS_ERASE = OFF;
	
		switch(bpm_ctrl.state){
			case BPM_STATE_IDLE:
			case BPM_STATE_SLEEP:
				bpm_manage.keyRequest = BPM_KEY_REQ_ERASE;
				break;
			default:
//			case BPM_STATE_INITIAL:
//			case BPM_STATE_MEASURE:
//			case BPM_STATE_RESULT:
//			case BPM_STATE_CONNECT:
//			case BPM_STATE_DISPLAY:
//			case BPM_STATE_ADJUST:
				// no operation
				break;
		}
	}

}



/*!
//==============================================================================
	@brief		Blood Pressure Monitor Battery Check Module
	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void bpm_check_battery(void)
{
	uint16 data;

	switch(bpm_ctrl.mode){
		case 0x00:
		// ---------------------------------
		// (00) ready
		// ---------------------------------
			R_ADC_Set_OperationOn();
			bpm_ctrl.mode++;
			break;

		case 0x01:
		// ---------------------------------
		// (01) start
		// ---------------------------------
			R_ADC_Start();

			bpm_calc.data_max = 0x0000;
			bpm_calc.data_min = 0xFFFF;
			bpm_calc.data_ave = 0x0000;
			bpm_ctrl.count = 0x00;
			bpm_ctrl.mode++;
			break;

		case 0x02:
		// ---------------------------------
		// (02) ADC
		// ---------------------------------
			if(b_BPM_ADC_COMP){
				b_BPM_ADC_COMP = OFF;
				R_ADC_Get_Result(&data);
				bpm_calc.data_ave += (uint32)data;
				if(data>=bpm_calc.data_max) bpm_calc.data_max = data;
				if(data<=bpm_calc.data_min) bpm_calc.data_min = data;
				
				bpm_ctrl.count++;

				if(bpm_ctrl.count == M_BPM_GET_CNT){
					// averaging (except for max & min value)
					bpm_calc.data_ave -= ((uint32)bpm_calc.data_min + (uint32)bpm_calc.data_max);
					bpm_calc.data_ave /= M_BPM_AVE_CNT;

					// calculate voltage
					bpm_calc.data_ave *= BPM_BATT_LV_VDD;
					bpm_calc.data_ave /= BPM_BATT_LV_10BIT;

					// stop ADC 
					R_ADC_Stop();
					R_ADC_Set_OperationOff();

					bpm_ctrl.mode = 0xFF;
				}
			}
			break;

		case 0xFF:
		// ---------------------------------
		// (FF) end
		// ---------------------------------
			if((uint16)bpm_calc.data_ave < BPM_BATT_LV_NONE){
				bpm_manage.battStatus = BPM_BATT_NONE;
			}else if((uint16)bpm_calc.data_ave < BPM_BATT_LV_LOW){
				bpm_manage.battStatus = BPM_BATT_LOW;
			}else{
				bpm_manage.battStatus = BPM_BATT_HIGH;
			}

			break;
		default:
			break;
	}
}

#endif
//[EOF]
