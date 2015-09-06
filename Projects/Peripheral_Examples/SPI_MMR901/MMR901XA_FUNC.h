//******************************************************************************
/*!
	@file		MMR901XA_FUNC.h
	@brief		MMR901XA Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __MMR901XA_FUNC_H__
#define __MMR901XA_FUNC_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
	#include "TYPE.h"

#ifdef __MMR901XA_FUNC_C__
	#define EXT_FUNC
#else
	#define EXT_FUNC extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define TC_COMP_METHOD_1ST		0											//!< Option that selects the 1st order correction
	#define TC_COMP_METHOD_2ND		1											//!< Option that selects the 2nd order correction

	#define TC_T0_X10				50											//!< Reference Temperature[deg] x10(e.g. 5[deg]x10 = 50)
	#define TC_T_RANGE_X10			50											//!< Range of Reference Temperature[deg] x10(e.g. 5[deg]x10 = 50)

	#define TC_D2TREF_TYP_X10		322150										//!< Typical ADC value of Temperature Sensor at Reference Temperature x10
	#define TC_D2TREF_RANGE_X10		196600										//!< Max - Min ADC value of Temperatute Sensor at Reference Temperature x10

	#define TC_ST_TARGET_X1024		40											//!< The design slope of temperature conversion formula at Reference Temperature x2^10

	#define TC_TCSP_X16384			(-36)										//!< Temperature Coefficient of Sensitivity x2^14(e.g. -0.0022[1/deg]~2^14)

	#define TC_SP_TARGET_X1024		26											//!< The design slope of pressure conversion formula at Reference Temperature x2^10

	#define TC_ET1_K1				((double)-8.056181e-4)						//!< The 2nd order correction coefficient of temperature: K1
	#define TC_ET1_K2				(84)										//!< The 2nd order correction coefficient of temperarure: K2 = INT(8.056181e-4 * 2^20 / 10)

	#define TC_ET1_TAPEX			25											//!< Central value of 2nd order correction of temperature

	#define TC_TA_MAX				(M_POW2_20 * 1000)							//!< Upper Limit of Parameter(ta) for correction calculation of temperature (equivalent to +100deg)
	#define TC_TA_MIN				(M_POW2_20 * (-1000))						//!< Lower Limit of Parameter(ta) for correction calculation of temperature (equivalent to -100deg)

	#define TC_PX100_MAX			((sint32)32767)								//!< Upper Limit of Parameter(px100) for correction calculation of pressure (Limited to 16 bit Length)
	#define TC_PX100_MIN			((sint32)-32768)							//!< Lower Limit of Parameter(px100) for correction calculation of pressure (Limited to 16 bit Length)


	#define TC_EEP_CAL_ADDR			0x02										//!< Start address of calibration data area in EEPROM
	#define TC_EEP_CAL_SIZE			6											//!< Data size of calibration data area in EEPROM [byte]

	#define TC_EEP_SIZE				10											//!< Size of EEPROM Memory blocks [byte]

	#define TC_EEP_CHECKSUM_ADDR	(TC_EEP_SIZE-1)								//!< The placement address of the checksum field

	#define FUNC_ADC_BUSY			0x00										//!< during AD conversion
	#define FUNC_ADC_READY			0x01										//!< after AD conversion
	#define FUNC_ADC_ZERO			0x02										//!< result zero
	#define FUNC_ADC_ERROR			0x03										//!< result error

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint16 c1;																	//!< CALIB DATA C1
	uint16 c3;																	//!< CALIB DATA C3
	uint16 c4;																	//!< CALIB DATA C4
	uint8  c2;																	//!< CALIB DATA C2
	uint8  c5;																	//!< CALIB DATA C5
} func_cpara_t;																	//!< CALIB DATA

typedef struct{
	sint32 spt25x;																//!< the sensitivity of the pressure sensor with temperature correnction (spt25x)
	sint16 delta_t;																//!< the difference from the reference temperature (delta_t)
	uint16 zero;																//!< the ADC value of the pressure sensor when gauge pressure is zero
} func_correction_coeff_t;														//!< Coefficient for Pressure Correction

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_FUNC void func_calc_initialize(void);
	EXT_FUNC uint8 func_read_calib_data(void);
	EXT_FUNC void func_cnvert_eep_to_calib(uint8 read_data[]);
	EXT_FUNC uint8 func_read_adc_data(uint16 *data);
	EXT_FUNC sint16 func_calc_temperature(uint16 d2, uint8 method);
	EXT_FUNC sint32 func_calc_sensitivity(sint16 delta_t);
	EXT_FUNC sint16 func_calc_pressure(uint16 d1, sint32 spt25x, uint16 zero);
	EXT_FUNC uint8 func_calc_checksum(uint8 *data, uint8 len);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_FUNC func_cpara_t				func_calib_data;						//!< calibration data
	EXT_FUNC func_correction_coeff_t	func_coeff;								//!< Coefficient for Pressure Correction

#endif	//__FUNC_H__
//[EOF]
