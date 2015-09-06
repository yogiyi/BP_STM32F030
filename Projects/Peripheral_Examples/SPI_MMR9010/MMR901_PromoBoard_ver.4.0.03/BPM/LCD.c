//******************************************************************************
/*!
	@file		LCD.c
	@brief		LCD Module Function Source File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __LCD_C__



//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"

#include "common.h"



//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		LCD Main Task

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2011.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_task(void)
{
	switch(lcd_ctrl.state){
		case LCD_STATE_INITIAL:		lcd_initial();		break;
		case LCD_STATE_CLOCK:		lcd_clock();		break;
		case LCD_STATE_MEASURE:		lcd_measure();		break;
		case LCD_STATE_RESULT:		lcd_result();		break;
		case LCD_STATE_ERROR:		lcd_error();		break;
		default:										break;
	}

}


/*!
//==============================================================================
	@brief		LCD Initialize State

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_initial(void)
{
	lcd_digit[0] = LCD_CHARA_SPACE;
	lcd_digit[1] = LCD_CHARA_SPACE;
	lcd_digit[2] = LCD_CHARA_SPACE;
	lcd_digit[3] = LCD_CHARA_SPACE;
	lcd_digit[4] = LCD_CHARA_SPACE;
	lcd_digit[5] = LCD_CHARA_SPACE;
	lcd_digit[6] = LCD_CHARA_SPACE;
	lcd_digit[7] = LCD_CHARA_SPACE;
	lcd_update_module();		// LCD UPDATE

	R_LCD_Set_VoltageOn();
	R_LCD_Start();
	lcd_ctrl.state = LCD_STATE_CLOCK;
}



/*!
//==============================================================================
	@brief		LCD Clock state

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_clock(void)
{
	if(lcd_ctrl.mode != LCD_MODE_CLK_NONE){

		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;
		
		if((lcd_ctrl.phase & 0xF0) == LCD_PHASE_CLK_DATE){						// カレンダー
			lcd_digit[0] = 2;
			lcd_digit[1] = 0;
			lcd_digit[2] = lcd_data.year/10;
			lcd_digit[3] = lcd_data.year%10;
			lcd_digit[4] = lcd_data.month/10;
			lcd_digit[5] = lcd_data.month%10;
			lcd_digit[6] = lcd_data.day/10;
			lcd_digit[7] = lcd_data.day%10;
			LCD_REQUEST_P4 = 1;
			LCD_REQUEST_P6 = 1;
		}else{
			lcd_digit[0] = LCD_CHARA_SPACE;
			lcd_digit[1] = LCD_CHARA_SPACE;
			lcd_digit[2] = LCD_CHARA_SPACE;
			lcd_digit[3] = LCD_CHARA_SPACE;
			lcd_digit[4] = lcd_data.hour/10;
			lcd_digit[5] = lcd_data.hour%10;
			lcd_digit[6] = lcd_data.minute/10;
			lcd_digit[7] = lcd_data.minute%10;

			if(lcd_ctrl.mode == LCD_MODE_CLK_UPDATE){
				LCD_REQUEST_P6 = 1;
			}
		}

		if(lcd_ctrl.phase == LCD_PHASE_CLK_YEAR){								//時計あわせ：年
			LCD_REQUEST_S3 = 1;
			LCD_REQUEST_S4 = 1;
		}else if(lcd_ctrl.phase == LCD_PHASE_CLK_MONTH){						//時計あわせ：月
			LCD_REQUEST_S5 = 1;
			LCD_REQUEST_S6 = 1;
		}else if(lcd_ctrl.phase == LCD_PHASE_CLK_DAY){							//時計あわせ：日
			LCD_REQUEST_S7 = 1;
			LCD_REQUEST_S8 = 1;
		}else if(lcd_ctrl.phase == LCD_PHASE_CLK_HOUR){							//時計あわせ：時
			LCD_REQUEST_S5 = 1;
			LCD_REQUEST_S6 = 1;
		}else if(lcd_ctrl.phase == LCD_PHASE_CLK_MIN){							//時計あわせ：分
			LCD_REQUEST_S7 = 1;
			LCD_REQUEST_S8 = 1;
		}
		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	}
}


/*!
//==============================================================================
	@brief		LCD Measure state

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_measure(void)
{
	uint8 press;

	if(lcd_ctrl.mode){
		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;

		lcd_digit[0] = LCD_CHARA_SPACE;
		lcd_digit[1] = LCD_CHARA_SPACE;
		lcd_digit[2] = LCD_CHARA_SPACE;
		lcd_digit[3] = LCD_CHARA_SPACE;
		lcd_digit[4] = LCD_CHARA_SPACE;

		press = lcd_data.press;
		lcd_digit[5] = (uint8)(press/100);
		if(lcd_digit[5] == 0){
			lcd_digit[5] = LCD_CHARA_SPACE;
		}
		press %= 100;
		lcd_digit[6] = (uint8)(press/10);
		if((lcd_digit[5] == 0) && (lcd_digit[6] == 0)){
			lcd_digit[6] = LCD_CHARA_SPACE;
		}

		lcd_digit[7] = (uint8)(press%10);

		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	}
}

/*!
//==============================================================================
	@brief		LCD Data state

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/

void lcd_result(void)
{
	uint8 calc;

	if(lcd_ctrl.mode){
		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;

		if( (lcd_ctrl.phase == LCD_PHASE_DAT_NONE) ||
			(lcd_ctrl.phase >  LCD_PHASE_DAT_TIME)){
			lcd_ctrl.phase = LCD_PHASE_DAT_PRESS;
		}

		if(lcd_ctrl.phase == LCD_PHASE_DAT_PRESS){
			lcd_digit[0] = LCD_CHARA_SPACE;

			calc = lcd_data.systolic;
			lcd_digit[1] = calc/100;
			if(lcd_digit[1]==0){
				lcd_digit[1] = LCD_CHARA_SPACE;
			}

			calc %= 100;
			lcd_digit[2] = calc/10;
			if((lcd_digit[1] == 0) && (lcd_digit[2] == 0)){
				lcd_digit[2] = LCD_CHARA_SPACE;
			}

			lcd_digit[3] = calc%10;

			lcd_digit[4] = LCD_CHARA_SPACE;

			LCD_REQUEST_P4 = 1;

			calc = lcd_data.diastolic;
			lcd_digit[5] = calc/100;
			if(lcd_digit[5]==0){
				lcd_digit[5] = LCD_CHARA_SPACE;
			}
			calc %= 100;
			lcd_digit[6] = calc/10;
			if((lcd_digit[5] == 0) && (lcd_digit[6] == 0)){
				lcd_digit[6] = LCD_CHARA_SPACE;
			}

			lcd_digit[7] = calc%10;

		}else if(lcd_ctrl.phase == LCD_PHASE_DAT_PULSE){
			lcd_digit[0] = LCD_CHARA_SPACE;

			calc = lcd_data.rate;
			lcd_digit[1] = calc/100;
			if(lcd_digit[1]==0){
				lcd_digit[1] = LCD_CHARA_SPACE;
			}

			calc %= 100;
			lcd_digit[2] = calc/10;
			if((lcd_digit[1] == 0) && (lcd_digit[2] == 0)){
				lcd_digit[2] = LCD_CHARA_SPACE;
			}

			lcd_digit[3] = calc%10;

			LCD_REQUEST_P4 = 1;

			calc = lcd_data.age;
			if(calc&0x80){
				calc -= 0x80;
				lcd_digit[4] = LCD_CHARA_SPACE;
				lcd_digit[5] = LCD_CHARA_SPACE;
				lcd_digit[6] = LCD_CHARA_SPACE;
				lcd_digit[7] = calc%10;
			}else{
				lcd_digit[4] = LCD_CHARA_SPACE;

				lcd_digit[5] = calc/100;
				if(lcd_digit[5]==0){
					lcd_digit[5] = LCD_CHARA_SPACE;
				}

				calc %= 100;
				lcd_digit[6] = calc/10;
				if((lcd_digit[5] == 0) && (lcd_digit[6] == 0)){
					lcd_digit[6] = LCD_CHARA_SPACE;
				}

				lcd_digit[7] = calc%10;
			}
		}else{
			lcd_digit[0] = lcd_data.month/10;
			lcd_digit[1] = lcd_data.month%10;
			lcd_digit[2] = lcd_data.day/10;
			lcd_digit[3] = lcd_data.day%10;
			lcd_digit[4] = lcd_data.hour/10;
			lcd_digit[5] = lcd_data.hour%10;
			lcd_digit[6] = lcd_data.minute/10;
			lcd_digit[7] = lcd_data.minute%10;
			LCD_REQUEST_P2 = 1;
			LCD_REQUEST_P4 = 1;
			LCD_REQUEST_P6 = 1;
		}
		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	}
}


/*!
//==============================================================================
	@brief		LCD convert data module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_error(void)
{
	uint8 err;

	if(lcd_ctrl.mode == LCD_MODE_ERR_UPDATE){
		err = lcd_data.error;

		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;

		lcd_digit[0] = LCD_CHARA_SPACE;
		lcd_digit[1] = LCD_CHARA_LETT_E;
		lcd_digit[2] = LCD_CHARA_LETT_R;
		lcd_digit[3] = LCD_CHARA_LETT_R;
		lcd_digit[4] = LCD_CHARA_HYPHEN;
		lcd_digit[5] = err/100;

		err %= 100;
		lcd_digit[6] = err/10;

		err %= 10;
		lcd_digit[7] = err;

		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	}else if(lcd_ctrl.mode == LCD_MODE_ERR_NOBATT){
		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;

		lcd_digit[0] = LCD_CHARA_HYPHEN;
		lcd_digit[1] = LCD_CHARA_HYPHEN;
		lcd_digit[2] = LCD_CHARA_HYPHEN;
		lcd_digit[3] = LCD_CHARA_HYPHEN;
		lcd_digit[4] = LCD_CHARA_HYPHEN;
		lcd_digit[5] = LCD_CHARA_HYPHEN;
		lcd_digit[6] = LCD_CHARA_HYPHEN;
		lcd_digit[7] = LCD_CHARA_HYPHEN;

		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	
	}else if(lcd_ctrl.mode == LCD_MODE_ERR_LOBATT){
		LCD_REQUEST_P_CLR;
		LCD_REQUEST_S_CLR;
		lcd_digit[0] = LCD_CHARA_SPACE;
		lcd_digit[1] = LCD_CHARA_SPACE;
		lcd_digit[2] = LCD_CHARA_SPACE;
		lcd_digit[3] = LCD_CHARA_SPACE;
		lcd_digit[4] = LCD_CHARA_SPACE;
		lcd_digit[5] = LCD_CHARA_SPACE;
		lcd_digit[6] = LCD_CHARA_LETT_L;
		lcd_digit[7] = LCD_CHARA_LETT_O;
	
		lcd_update_module();		// LCD UPDATE

		lcd_ctrl.mode = 0x00;
	}
}



/*!
//==============================================================================
	@brief		LCD Update module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void lcd_update_module(void)
{
	uint16 disp;
	
	disp  = LCD_NUMBER[lcd_digit[0]];
	disp += (uint16)LCD_REQUEST_S1 * 0x100;
	disp += (uint16)LCD_REQUEST_P1;
	SEG0  = (uint8)(disp>>8)&0x0F;
	SEG1  = (uint8)(disp>>4)&0x0F;
	SEG2  = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[1]];
	disp += (uint16)LCD_REQUEST_S2 * 0x100;
	disp += (uint16)LCD_REQUEST_P2;
	SEG3  = (uint8)(disp>>8)&0x0F;
	SEG4  = (uint8)(disp>>4)&0x0F;
	SEG5  = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[2]];
	disp += (uint16)LCD_REQUEST_S3 * 0x100;
	disp += (uint16)LCD_REQUEST_P3;
	SEG6  = (uint8)(disp>>8)&0x0F;
	SEG7  = (uint8)(disp>>4)&0x0F;
	SEG8  = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[3]];
	disp += (uint16)LCD_REQUEST_S4 * 0x100;
	disp += (uint16)LCD_REQUEST_P4;
	SEG9  = (uint8)(disp>>8)&0x0F;
	SEG10 = (uint8)(disp>>4)&0x0F;
	SEG11 = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[4]];
	disp += (uint16)LCD_REQUEST_S5 * 0x100;
	disp += (uint16)LCD_REQUEST_P5;
	SEG12 = (uint8)(disp>>8)&0x0F;
	SEG13 = (uint8)(disp>>4)&0x0F;
	SEG14 = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[5]];
	disp += (uint16)LCD_REQUEST_S6 * 0x100;
	disp += (uint16)LCD_REQUEST_P6;
	SEG15 = (uint8)(disp>>8)&0x0F;
	SEG16 = (uint8)(disp>>4)&0x0F;
	SEG17 = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[6]];
	disp += (uint16)LCD_REQUEST_S7 * 0x100;
	disp += (uint16)LCD_REQUEST_P7;
	SEG20 = (uint8)(disp>>8)&0x0F;
	SEG21 = (uint8)(disp>>4)&0x0F;
	SEG24 = (uint8)disp&0x0F;

	disp  = LCD_NUMBER[lcd_digit[7]];
	disp += (uint16)LCD_REQUEST_S8 * 0x100;
	disp += (uint16)LCD_REQUEST_P8;
	SEG25 = (uint8)(disp>>8)&0x0F;
	SEG28 = (uint8)(disp>>4)&0x0F;
	SEG38 = (uint8)disp&0x0F;
}


/*!
//==============================================================================
	@brief		LCD NUMBER TABLE

	@author		k.kurata
	@date		create: 2012.07.20
	@note

//==============================================================================
*/
							//     0,     1,     2,     3,     4,     5,     6,    7,     8,     9,   ' ',   'E',   'r',   '-',   'L',   'o'
const uint16 LCD_NUMBER[16] = {0x656, 0x006, 0x274, 0x076, 0x426, 0x472, 0x672,0x446, 0x676, 0x476, 0x000, 0x670, 0x220, 0x020, 0x610, 0x232};


// LCD:S5671C
//  _    _    _    _    _    _    _    _      
// |_|  |_|  |_|  |_|  |_|  |_|  |_|  |_|     
// |_|. |_|. |_|. |_|. |_|. |_|. |_|. |_|.<-P 
//  _    _    _    _    _    _    _    _<-S   
//  1    2    3    4    5    6    7    8      
//[EOF]
