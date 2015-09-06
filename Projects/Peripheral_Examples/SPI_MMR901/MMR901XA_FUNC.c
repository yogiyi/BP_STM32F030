//******************************************************************************
/*!
	@file		MMR901XA_FUNC.c
	@brief		MMR901XA Function Source File
				
				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#define __MMR901XA_FUNC_C__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "common.h"

//==============================================================================
//{### FUNCTIONS
//==============================================================================
/*!
//==============================================================================
	@brief		MMR901XA Function Parameter Initialize Module

	@param		void
	@return		void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module initializes the parameter relates to "MMR901XA".

//==============================================================================
*/
void func_calc_initialize(void)
{
	utl_memory_clr(&func_coeff, sizeof(func_coeff));							// clear correction coefficent
	utl_memory_clr(&func_calib_data, sizeof(func_calib_data));					// clear c-parameter data
}


/*!
//==============================================================================
	@brief		Calibration Data Conversion Module

	@param		void
	@retval		0x00 : Checksum OK
	@retval		0x01 : Checksum NG
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module converts EEPROM data into the calibration data,\n
	checks the checksum and returns OK or NG. 

//==============================================================================
*/
uint8 func_read_calib_data(void)
{
	uint8 cnt;
	uint8 read_data[TC_EEP_SIZE];

	// execute LOAD EEPROM command
	cmd_exec_spi_protocol(CMD_LOAD_EEPROM, 0x00);

	// execute READ EEPROM Register command
	for(cnt=0; cnt<TC_EEP_SIZE; cnt++){
		read_data[cnt] = (uint8)cmd_exec_spi_protocol(CMD_READ_EEPROM, cnt);
	}

	// checksum
	if(read_data[TC_EEP_CHECKSUM_ADDR] == 0x00){
		return 0x01;
	}

	// checksum
	if(func_calc_checksum(&read_data[0], 10) != 0xFF){
		return 0x01;
	}
	
	// collect into a function
	func_cnvert_eep_to_calib(&read_data[0]);							// convert

	return 0x00;
}


/*!
//==============================================================================
	@brief		Calibration Data Conversion Module

	@param		regbuf[]
	@return 	void
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:

//==============================================================================
*/
void func_cnvert_eep_to_calib(uint8 read_data[])
{
	func_calib_data.c1 = ((((uint16)read_data[2]) << 6) & 0x3FC0) |				// c1<13:6>
						 ((((uint16)read_data[3]) >> 2) & 0x003F);				// c1<5:0>   = 14bit
	
	func_calib_data.c2 = ((read_data[3] << 4) & 0x30) |							// c2<5:4>
						 ((read_data[4] >> 4) & 0x0F);							// c2<3:0>   = 6bit
	
	func_calib_data.c3 = ((((uint16)read_data[4]) << 9) & 0x1E00) |				// c3<12:9>
						 ((((uint16)read_data[5]) << 1) & 0x01FE) |				// c3<8:1>
						 ((((uint16)read_data[6]) >> 7) & 0x0001);				// c3<0>     = 13bit
	
	func_calib_data.c4 = ((((uint16)read_data[6]) << 2) & 0x01FC) |				// c4<8:2>
						 ((((uint16)read_data[7]) >> 6) & 0x0003);				// c4<1:0>   = 9bit
	
	func_calib_data.c5 = read_data[7] & 0x003F;									// c5<5:0>   = 6bit
}


/*!
//==============================================================================
	@brief		Read ADC Data Module

	@param		*data  : Pointer to the store ADC
	@retval		FUNC_ADC_BUSY : during AD conversion
	@retval		FUNC_ADC_READY: after AD conversion
	@retval		FUNC_ADC_ZERO : result zero
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module reads ADC data when ADRDY becomes Low.

//==============================================================================
*/
uint8 func_read_adc_data(uint16 *data)
{
	if(P_SPI_ADRDY == HIGH){
		return FUNC_ADC_BUSY;
	}else{	// ADRDY = LOW
		// execute Read ADC command
		*data = cmd_exec_spi_protocol(CMD_READ_ADC, 0x00);
		if(*data == 0){
			return FUNC_ADC_ZERO;
		}else{
			return FUNC_ADC_READY;
		}
	}
}


/*!
//==============================================================================
	@brief		Tempareture Calculation Module

	@param		d2      : ADC value of the temperature sensor
	@param		method  : Correction method
	@return		delta_t : Difference from the reference temperature[x10 deg]
	@return		tx10    : Corrected temperature[x10 deg]
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module converts ADC data into the temperature.
	@note
	Please refer to the product specification of MMR901XA for the content of the calculation. 

//==============================================================================
*/
sint16 func_calc_temperature(uint16 d2, uint8 method)
{
	sint32 trefc;
	sint32 d2ref;
	sint32 stc;
	sint32 d2c;
	sint32 ta;
	sint32 tx10;
	sint32 txd;
	sint32 et1;
	sint32 tad;
	sint32 tref;

	// (1) calculate the reference temperature
//	trefc = M_POW2_14 * TC_T_RANGE_X10 * (((sint32)func_calib_data.c5) - M_POW2_5) + M_POW2_20 * TC_T0_X10;
	trefc = (uint32)M_POW2_14 * (50) * func_calib_data.c5;
	trefc += 26214400;
		
	// (2) calculate the ADC value in the temperature sensor at the reference temperature
//	d2ref = TC_D2TREF_TYP_X10 + ( (((sint32)func_calib_data.c3) - M_POW2_12) * TC_D2TREF_RANGE_X10) / M_POW2_13;
//	d2ref = (322150) + ( (((sint32)func_calib_data.c3) - M_POW2_12) * (196600) ) / M_POW2_13;
	d2ref = 223850 + ((sint32)func_calib_data.c3 * 24575) / M_POW2_10;
		
	// (3) calculate the slope of the temperature conversion equation
//	stc = TC_ST_TARGET_X1024 * (((sint32)func_calib_data.c4) + 768);
	stc =  (sint32)func_calib_data.c4 * 40 + 30720;
		
	// (4) calculation the difference the value of (2) from the ADC value in the temperature sensor
	d2c = (sint32)d2 * 10  - d2ref;

	// (5) convert the ADC value into the temperature
	tad = stc * d2c + trefc;	
	
	// select the 2nd order correction
	if(method == TC_COMP_METHOD_2ND){
//		txd = (tad/ TC_POW2_20) / 10.0;
//		tref = (trefc/ TC_POW2_20) / 10.0;
//		et1 = TC_ET1_K1 * (txd - ((2*TC_ET1_TAPEX)-tref) ) * (txd - tref);
//		tad = ((txd - et1) * 10.0 * TC_POW2_20);

		txd  = utl_round32(tad,   M_POW2_20);						// 0.1 degrees C
		tref = utl_round32(trefc, M_POW2_20);						// 0.1 degrees C
		et1 = (sint32)TC_ET1_K2 * (txd + tref - TC_ET1_TAPEX * 20) * (txd - tref);
		tad = tad + et1;
	}

	// prevent the memory overflow
	if(tad > TC_TA_MAX){
		ta = TC_TA_MAX;
	}else if(tad < TC_TA_MIN){
		ta = TC_TA_MIN;
	}else{
		ta = (sint32)tad;	
	}

	// (6) convert into the uints of 0.1 degrees C
	tx10 = ta / (sint32)M_POW2_20;
	
	// (7) calculate the difference from the reference temperature[x10 deg.]
	func_coeff.delta_t = (sint16)((ta - trefc) / M_POW2_20);
	
	return (sint16)tx10;

}

/*!
//==============================================================================
	@brief		Temperature Coefficient of Sensitivity Calculation Module

	@param		delta_t : Difference from the reference temperature[x10 deg]
	@return		spt25x  : The sensitivity of the pressure sensor with temperature correction [x25]
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module calculates the sensitivity of the pressure sensor with temperature correction.
	@note
	Please refer to the product specification of MMR901XA for the content of the calculation. 

//==============================================================================
*/
sint32 func_calc_sensitivity(sint16 delta_t)
{

	sint32 spct_a;
	sint32 spct_b;
	sint32 spct;
	sint32 spc;
	sint32 spt25x;
	
//	spct_a = ((sint32)func_calib_data.c2 + 32) * TC_TCSP_X16384 * delta_t;
	spct_a = ((sint32)func_calib_data.c2 + 32) * (-36) * delta_t;
	
//	spc = M_POW2_9 * TC_SP_TARGET_X1024 * ( ((sint32)func_calib_data.c1) + 24576);
	spc = 13312 * ((sint32)func_calib_data.c1 + 24576);    

//	spct_b = spct_a / (10 * M_POW2_4);
	spct_b = spct_a / 160;
	spct = M_POW2_16 + spct_b;
	
	spt25x = (spc / spct) * 25;

	return spt25x;
}


/*!
//==============================================================================
	@brief		Pressure Calculation Module

	@param		d1     : ADC value of the pressure sensor
	@param		spt25x : The sensitivity of the pressure sensor with temperature correction [x25]
	@param		zero   : ADC value of the pressure sensor when gauge pressure is zero
	@return		px100  : Calculated pressure[x100 mmHg]
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module calculates the pressure value.
	@note
	Please refer to the product specification of MMR901XA for the content of the calculation. 

//==============================================================================
*/
sint16 func_calc_pressure(uint16 d1, sint32 spt25x, uint16 zero)
{
	uint32_st px100d;
	uint16    px100;
	uint16    delta_ad;

	if(d1 >= zero){
		delta_ad = d1 - zero;
	}else{
		delta_ad = zero - d1;
	}

	px100d.UDWORD = (uint32)spt25x * delta_ad;
	px100 = px100d.WORD.HI;
	if(px100 > TC_PX100_MAX){
		px100 = TC_PX100_MAX;
	}
	if(d1 >= zero){
		return px100;
	}else{
		return -px100;
	}
}


/*!
//==============================================================================
	@brief		Checksum Module

	@param		*data : Start pointer
	@param		len   : Data length
	@retval		0xFF  : checksum OK
	@retval		other : checksum NG
	@author		k.kurata
	@date		create: 2012.07.20
	
//==============================================================================
	@par		Operations Description:
	This module computes the checksum.\n
	The 1's complement sum of all data every 8bits is computed.\n
	If the result is 0xFF, it is judged the data don't have error.

//==============================================================================
*/
uint8 func_calc_checksum(uint8 *data, uint8 len)
{
	uint16 sum;
	
	sum = 0;
	
	// calculate every 8bits
	while(len > 0){
		sum += *data++;	
		len--;
	}
	
	// calculate until becoming the 8 bit length
	while(sum >> 8){
		sum = (sum & 0xFF) + (sum >> 8);
	}
	
	// return the 1's complement number
	// prevent the sum data from becoming zero
	if(sum == 0xFF){
		return (uint8)(sum);
	}else{
		return (uint8)(~sum);
	}
}



//[EOF]
