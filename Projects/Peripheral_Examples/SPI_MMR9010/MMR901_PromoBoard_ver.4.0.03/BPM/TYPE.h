//******************************************************************************
/*!
	@file		TYPE.h
	@brief		typedef Header File

				copyright: MITSUMI Electric Co.,LTD
	@author		k.kurata
	@date		2011.02.18
	@attention	none
	@warning	MITSUMI CONFIDENTIAL
*/
//******************************************************************************
#ifndef __TYPE_H__
#define __TYPE_H__

//==============================================================================
//{### MODULES USED
//==============================================================================
#include "r_cg_macrodriver.h"


//==============================================================================
//{### DEFINES AND MACROS
//==============================================================================
#define HIGH			1					//!< I/O PORT LEVEL: High
#define LOW				0					//!< I/O PORT LEVEL: Low

#define ON				1					//!< FLAG: ON 
#define OFF				0					//!< FLAG: OFF

#define TRUE			1					//!< BOOLEAN: TRUE
#define FALSE			0					//!< BOOLEAN: FALSE

#define M_POW2_4		0x10				//!< 2^4  = 16      = 0x10
#define M_POW2_5		0x20				//!< 2^5  = 32      = 0x20
#define M_POW2_6		0x40				//!< 2^6  = 64      = 0x40
#define M_POW2_7		0x80				//!< 2^7  = 128     = 0x80
#define M_POW2_8		0x100				//!< 2^8  = 256     = 0x100
#define M_POW2_9		0x200				//!< 2^9  = 512     = 0x200
#define M_POW2_10		0x400				//!< 2^10 = 1024    = 0x400
#define M_POW2_11		0x800				//!< 2^11 = 2048    = 0x800
#define M_POW2_12		0x1000				//!< 2^12 = 4096    = 0x1000
#define M_POW2_13		0x2000				//!< 2^13 = 8192    = 0x2000
#define M_POW2_14		0x4000				//!< 2^14 = 16384   = 0x4000
#define M_POW2_16		0x10000				//!< 2^16 = 65536   = 0x10000
#define M_POW2_19		0x80000				//!< 2^19 = 524288  = 0x80000
#define M_POW2_20		0x100000			//!< 2^20 = 1048576 = 0x100000
#define M_POW2_21		0x200000			//!< 2^21 = 2097152 = 0x200000

// 5ms Countdown timer macro
#define M_T5MS_COUNT_50MS		10			//!< 50ms
#define M_T5MS_COUNT_100MS		20			//!< 100ms
#define M_T5MS_COUNT_250MS		50			//!< 250ms
#define M_T5MS_COUNT_300MS		60			//!< 300ms
#define M_T5MS_COUNT_500MS		100			//!< 500ms
#define M_T5MS_COUNT_1S			200			//!< 1s
#define M_T5MS_COUNT_2S			400			//!< 2s
#define M_T5MS_COUNT_3S			600			//!< 3s
#define M_T5MS_COUNT_5S			1000		//!< 5s
#define M_T5MS_COUNT_10S		2000		//!< 10s
#define M_T5MS_COUNT_30S		6000		//!< 30s
#define M_T5MS_COUNT_60S		12000		//!< 60s
#define M_T5MS_COUNT_120S		24000		//!< 120s

//==============================================================================
//{### TYPEDEFS AND STRUCTURES
//==============================================================================
typedef unsigned char	uint8;				//!< unsigned byte/integer                0 ~ 255
typedef unsigned short	uint16;				//!< unsigned word (2 bytes)              0 ~ 65,535
typedef unsigned long	uint32;				//!< unsigned long (4 bytes)              0 ~ 4,294,967,295

typedef signed char		sint8;				//!< signed byte/integer               -127 ~ 127
typedef signed short	sint16;				//!< signed word (2 bytes)          -32,767 ~ 32,767
typedef signed long		sint32;				//!< signed long (4 bytes)   -2,147,483,647 ~ 2,147,483,647

// Little Endian
typedef union {
	uint32		UDWORD;
	struct {
		uint16	LO;
		uint16	HI;
	} WORD;
	struct {
		uint8	LL;
		uint8	LH;
		uint8	UL;
		uint8	UH;
	} BYTE;
} uint32_st;								//!< typedef union unsigned 32 bits

typedef union {
	sint32		SDWORD;
	struct {
		uint16	LO;
		uint16	HI;
	} WORD;
	struct {
		uint8	LL;
		uint8	LH;
		uint8	UL;
		uint8	UH;
	} BYTE;
} sint32_st;								//!< typedef union signed 32 bits

typedef union {
	uint16		UWORD;
	struct {
		uint8	LO;
		uint8	HI;
	} BYTE;
} uint16_st;								//!< typedef union unsigned 16 bits

typedef union {
	sint16		SWORD;
	struct {
		uint8   LO;
		uint8   HI;
	} BYTE;
} sint16_st;								//!< typedef union signed 16 bits

typedef union {
	uint8 BYTE;
	struct {
		uint8 B0: 1;
		uint8 B1: 1;
		uint8 B2: 1;
		uint8 B3: 1;
		uint8 B4: 1;
		uint8 B5: 1;
		uint8 B6: 1;
		uint8 B7: 1;
	} BIT;
} flag8bit_t;								//!< typedef union unsigned 8 bits

typedef struct{
	uint8 state;							//!< state
	uint8 phase;							//!< phase
	uint8 count;							//!< counter
	uint8 mode;								//!< mode flag
} system_state_t;							//!< state machine table

//==============================================================================
//{### PROTOTYPE / GLOBAL FUNCTIONS
//==============================================================================

//==============================================================================
//{### VARIABLES
//==============================================================================


#endif	//__TYPE_H__
//[EOF]


