//******************************************************************************
/*!
	@file		BPM_MAIN.h
	@brief		Blood-Pressure Monitor System Function Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2012.07.20
	@attention	Since no medical approval is obtained as a blood pressure monitor,\n
				all the measurement values are for reference only.
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __BPM_MAIN_H__
#define __BPM_MAIN_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "TYPE.h"

#ifdef __BPM_MAIN_C__
	#define EXT_BPM
#else
	#define EXT_BPM extern
#endif

//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
	#define P_BPM_LED_HEART				P1.3									//!< LED: Blink Heart Rate
	#define P_BPM_LED_HEART_ON()		(P1.3=0)								//!< ON
	#define P_BPM_LED_HEART_OFF()		(P1.3=1)								//!< OFF
	#define P_BPM_LED_BATT				P1.4									//!< LED: Alert Battery Empty
	#define P_BPM_LED_BATT_ON()			(P1.4=0)								//!< ON
	#define P_BPM_LED_BATT_OFF()		(P1.4=1)								//!< OFF

// State
	#define BPM_STATE_INITIAL			0x00									//!< STATE: INITIALIZE
	#define BPM_STATE_IDLE				0x01									//!< STATE: IDLE
	#define BPM_STATE_CONNECT			0x02									//!< STATE: CONNECT
	#define BPM_STATE_MEASURE			0x03									//!< STATE: MEASURE
	#define BPM_STATE_RESULT			0x04									//!< STATE: RESULT
	#define BPM_STATE_DISPLAY			0x05									//!< STATE: DISPLAY
	#define BPM_STATE_ADJUST			0xFE									//!< STATE: ADJUST CLOCK
	#define BPM_STATE_SLEEP				0xFF									//!< STATE: SLEEP

// Error Code
	#define BPM_ERROR_NO_ERROR			0x00									//!< ERR CODE: NO ERROR
	#define BPM_ERROR_CALIB_DATA		0x01									//!< ERR CODE: CALIB DATA ERROR
	#define BPM_ERROR_EMERGENCY			0x02									//!< ERR CODE: EMERGENCY STOP ERROR
	#define BPM_ERROR_TIMEOUT1			0x03									//!< ERR CODE: TIMEOUT ERROR 1
	#define BPM_ERROR_TIMEOUT2			0x04									//!< ERR CODE: TIMEOUT ERROR 2
	#define BPM_ERROR_PRESSURE			0x05									//!< ERR CODE: PRESSURE ERROR
	#define BPM_ERROR_NO_BEAT			0x06									//!< ERR CODE: NO BEAT ERROR
	#define BPM_ERROR_NO_CONNECT		0x07									//!< ERR CODE: NO CONNECT
	#define BPM_ERROR_DUTY_RATIO		0x08									//!< ERR CODE: DUTY RATIO ERROR
	#define BPM_ERROR_BUG				0xFF									//!< ERR CODE: BUG ERROR

// Key Request
	#define BPM_KEY_REQ_NONE			0x00
	#define BPM_KEY_REQ_START			0x01
	#define BPM_KEY_REQ_STOP			0x02
	#define BPM_KEY_REQ_DSP_START		0x03
	#define BPM_KEY_REQ_DSP_END			0x04	//終了
	#define BPM_KEY_REQ_DSP_BACK		0x05	//過去ログを遡る
	#define BPM_KEY_REQ_DSP_SHIFT		0x06	//表示変更
	#define BPM_KEY_REQ_ADJ_START		0x07	//
	#define BPM_KEY_REQ_ADJ_NEXT		0x08	//NEXT
	#define BPM_KEY_REQ_ADJ_UP			0x09	//カウントアップ
	#define BPM_KEY_REQ_ADJ_DOWN		0x0A	//カウントダウン
	#define BPM_KEY_REQ_ERASE			0x0B	//
	#define BPM_KEY_REQ_DATE			0x0C

// Battery Status
	#define BPM_BATT_NO_CHECK			0x00
	#define BPM_BATT_NONE				0x01
	#define BPM_BATT_LOW				0x02
	#define BPM_BATT_HIGH				0x03

// Battery Level
	#define BPM_BATT_LV_NONE			1000									//!< 
	#define BPM_BATT_LV_LOW				2000									//!< 
	#define BPM_BATT_LV_VDD				3000									//!< 
	#define BPM_BATT_LV_10BIT			1024									//!< 

// ==== The following setting values are for reference only ====
// Vascular hardness RANK
	#define BPM_BVS_RANK_08				80										//!< Vascular hardness Threshold: rank8
	#define BPM_BVS_RANK_07				70										//!< Vascular hardness Threshold: rank7
	#define BPM_BVS_RANK_06				60										//!< Vascular hardness Threshold: rank6
	#define BPM_BVS_RANK_05				50										//!< Vascular hardness Threshold: rank5
	#define BPM_BVS_RANK_04				40										//!< Vascular hardness Threshold: rank4
	#define BPM_BVS_RANK_03				30										//!< Vascular hardness Threshold: rank3
	#define BPM_BVS_RANK_02				20										//!< Vascular hardness Threshold: rank2
	#define BPM_BVS_RANK_01				0										//!< Vascular hardness Threshold: rank1

	#define M_BPM_AVE_CNT				0x03									//!< Number of sample for averaging
	#define M_BPM_GET_CNT				0x05									//!< Number of sample for average,max, and min

//	#define BPM_PRESS_MAX_LIMIT			18000									//!< Increasing pressure limit: 180mmHg[0.01mmHg/LSB]
	#define BPM_PRESS_MAX_LIMIT			17000									//!< Increasing pressure limit: 170mmHg[0.01mmHg/LSB]
	#define BPM_PRESS_MIN_LIMIT			5500									//!< Decreasing pressure limit:  55mmHg[0.01mmHg/LSB]
	#define BPM_PRESS_NO_BEAT			7000									//!< Decreasing pressure limit for no pulse detection: 70mmHg[0.01mmHg/LSB]
	#define BPM_PULSE_PEAK_LEVEL		100										//!< Pulse level lower limit: 1mmHg[0.01mmHg/LSB]

	#define BPM_PULSE_1ST_MAX			100										//!< 1st Pulse level MAX limit: 1.0mmHg[0.01mmHg/LSB]
	#define BPM_PULSE_1ST_MIN			20										//!< 1st Pulse level MIN limit: 0.2mmHg[0.01mmHg/LSB]

	#define BPM_PRESS_FIXED1			7000									//!< Decreasing pressure limit:  700mmHg[0.01mmHg/LSB]
	#define BPM_PRESS_FIXED2			10000									//!< Decreasing pressure limit: 1000mmHg[0.01mmHg/LSB]

	#define BPM_SYSTOLIC_BP_TH			8										//!< Threshold for  SYSTOLIC BLOOD PRESSURE :40% 
	#define BPM_DIASTOLIC_BP_TH			10										//!< Threshold for DIASTOLIC BLOOD PRESSURE :50% 
	#define BPM_BP_RESOLUTION			20										//!< divede 20

	#define BPM_MEAS_INTERVAL			2										//!< Interval of measuring pressure: 1/2
	#define BPM_VALID_PRESS_DATA		128										//!< Number of saved pressure data (Pulse Rate 46 equiv.)
	#define BPM_LIMIT_PRESS_DATA		250										//!< Number of pressure data when the pulse is the slowest(Pulse Rate 23 equiv.)
	#define BPM_PULSE_NUM				40										//!< Number of pulse wave
	#define BPM_RISE_CHK_NUM			2										//!< Number of sample for Pulse Rise Edge Detection

	#define BPM_MOVE_AVE_NUM			5										//!< Number of sample for the moving average

//	#define BPM_VALVE_LEAK_KEEP			24										//!< Duty Ratio Keep Time: 120[ms] = 24[*5ms]
	#define BPM_VALVE_LEAK_KEEP			20										//!< Duty Ratio Keep Time: 100[ms] = 20[*5ms]

//	#define BPM_PRESS_1ST_LEAK_LIMIT	14000									//!< Duty Ratio 
//	#define BPM_PRESS_1ST_LEAK_LIMIT	15000									//!< Duty Ratio 
	#define BPM_PRESS_1ST_LEAK_LIMIT	17000									//!< Duty Ratio 

	#define BPM_HEART_RATE_MAX			100										//!< 
	#define BPM_HEART_RATE_MIN			50										//!< 
//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef struct{
	uint16 countdown5ms;
	uint16 interval5ms;
	uint8  errorCode;
	uint8  keyRequest;
	uint8  battStatus;
	uint8  battPeriod;
} bpm_manage_t;


typedef struct{
	sint16 rise[BPM_RISE_CHK_NUM];												//!< the recent data for detecting pulse rise edge
	sint16 peak;																//!< the peak value of pulse wave
	sint16 base;																//!< the base value of pulse wave
	uint16 pl[BPM_PULSE_NUM];													//!< the pulse level (= peak - base)
	sint16 plp[BPM_PULSE_NUM];													//!< the pressure of the timing detected the peak value
	uint16 pl_max;																//!< the max level of the pulse levels
	uint16 bp_max;																//!<  systolic blood pressure
	uint16 bp_min;																//!< diastolic blood pressure
	uint16 time;																//!< time detected pulse rise edge
	uint8  count;																//!< pulse wave number
	uint8  pl_max_num;															//!< pulse wave number detected max pulse level
} bpm_blood_press_monitor_t;													//!< Blood Pressure Monitor Parameter Table

typedef struct{
	uint8 ave;																	//!< Average of Pulse Rate
	uint8 save[BPM_PULSE_NUM];													//!< Pulse Rate
} bpm_pulse_rate_t;																//!< Pulse Rate Parameter

typedef struct{
	sint16 ph_temp[BPM_VALID_PRESS_DATA];										//!< the temporary data array of pulse wave
	sint16 ph_max[BPM_VALID_PRESS_DATA];										//!< the data array of pulse wave detected max pulse level
	sint16 ph_min;																//!< the base pressure of pulse wave detected max pulse level
	sint16 age;																	//!< Blood Vessel Age
	sint16 wave_a;																//!< 2nd order derivative pulse wave: a-wave
	sint16 wave_b;																//!< 2nd order derivative pulse wave: b-wave
	sint16 wave_c;																//!< 2nd order derivative pulse wave: c-wave
	sint16 wave_d;																//!< 2nd order derivative pulse wave: d-wave
	sint16 wave_e;																//!< 2nd order derivative pulse wave: e-wave
	uint8  count;																//!< pulse wave counter
	uint8  period;																//!< sampling number of pulse wave detected max pulse level
} bpm_blood_vessel_age_t;														//!< Blood Vessel Age Parameter Table

typedef struct{
	uint32 data_ave;															//!< ave
	uint16 data_max;															//!< max
	uint16 data_min;															//!< min
} bpm_calculation_t;															//!< generic parameter for calculation

typedef struct{
	uint8 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 minute;
} bpm_time_t;

#if(0)//debug
typedef struct{
	uint8 number;																//!< CONTROL NUMBER
	uint8 month;																//!< MONTH
	uint8 day;																	//!< DAY
	uint8 hour;																	//!< HOUR
	uint8 minute;																//!< MINUTE
	uint8 bp_max;																//!< SYSTOLIC BLOOD PRESSURE
	uint8 bp_min;																//!< DIASTOLIC BLOOD PRESSURE
	uint8 rate;																	//!< BEAT RATE
	uint8 age;																	//!< BLOOD VESSEL AGE
	uint8 crc;																	//!< CRC
}log_data_t;																	//!< log data table
#endif

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================
	EXT_BPM void bpm_main(void);
	EXT_BPM void bpm_initial(void);
	EXT_BPM void bpm_idle(void);
	EXT_BPM void bpm_connect(void);
	EXT_BPM void bpm_measure(void);
	EXT_BPM void bpm_result(void);
	EXT_BPM void bpm_display(void);
	EXT_BPM void bpm_adjust(void);
	EXT_BPM void bpm_sleep(void);

	EXT_BPM void bpm_key_management(void);

	EXT_BPM void bpm_get_pulse_data(sint16 data);
	EXT_BPM void bpm_calc_blood_pressure(void);
	EXT_BPM void bpm_calc_blood_vessel_age(void);
	EXT_BPM void bpm_calc_vascular_hardness(void);
	EXT_BPM void bpm_update_blood_pressure(void);

	EXT_BPM void bpm_check_battery(void);

//==============================================================================
//{### VARIABLES
//==============================================================================
	EXT_BPM sint16 bpm_disp_tx10;												//!< Temperature for display
	EXT_BPM bpm_blood_press_monitor_t	bpm_bpm_para;							//!< Blood Pressure Monitor Parameter
	EXT_BPM bpm_pulse_rate_t			bpm_prt_para;							//!< Pulse Rata Parameter
	EXT_BPM bpm_blood_vessel_age_t		bpm_bva_para;							//!< Blood Vessel Age Parameter
	EXT_BPM bpm_calculation_t			bpm_calc;								//!< Generic Parameter for calculation
	EXT_BPM bpm_manage_t				bpm_manage;								//!< Management Parameter

	EXT_BPM bpm_time_t					bpm_currentTime;						//!<
	EXT_BPM bpm_time_t					bpm_measuringTime;						//!< 

	EXT_BPM uint16 bpm_valve_duty_rate;											//!< 

	EXT_BPM system_state_t bpm_ctrl;											//!< STATE MACHINE: BPM main control

	EXT_BPM flag8bit_t bpm_pulse_status;										//!< STATUS BIT: BPM measurement status
	#define b_BPM_PULSE_CLR		(bpm_pulse_status.BYTE = 0x00)					//!< clear flag
	#define b_BPM_PULSE_START	bpm_pulse_status.BIT.B0							//!< FLAG: BPM start request
	#define b_BPM_PULSE_RISE	bpm_pulse_status.BIT.B1							//!< FLAG: pulse rise edge detection flag
	#define b_BPM_PULSE_BEAT	bpm_pulse_status.BIT.B2							//!< FLAG: pulse detection flag
	#define b_BPM_PULSE_END		bpm_pulse_status.BIT.B3							//!< FLAG: BPM end request
	#define b_BPM_PULSE_HEART	bpm_pulse_status.BIT.B4							//!< FLAG: pulse display flag

	EXT_BPM flag8bit_t bpm_status;												//!< STATUS BIT: BPM peripheral status
	#define b_BPM_STATUS_CLR	(bpm_status.BYTE = 0x00)						//!< clear flag
	#define b_BPM_FATAL_ERR		bpm_status.BIT.B0								//!< FLAG: Fatal Error flag
	#define b_BPM_ADC_COMP		bpm_status.BIT.B1								//!< FLAG: ADC Complete flag
	#define b_BPM_WAKE_UP		bpm_status.BIT.B2								//!< FLAG: Wake Up flag

#endif	//__BPM_MAIN_H__
//[EOF]
