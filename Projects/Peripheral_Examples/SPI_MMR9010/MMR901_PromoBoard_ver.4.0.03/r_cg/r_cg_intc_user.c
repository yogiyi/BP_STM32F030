/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/L12 E1.00.00c [23 Mar 2012]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for INTC module.
* Creation Date: 2012/11/08
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTP3 r_intc3_interrupt
#pragma interrupt INTP6 r_intc6_interrupt
#pragma interrupt INTP7 r_intc7_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
#include "common.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc3_interrupt
* Description  : This function is INTP3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    //SW3:SELECT
    if(b_KEY_EVENT_SELECT!=ON){
        b_KEY_EVENT_SELECT=ON;
//		key_manage.countdown5ms = M_T5MS_COUNT_100MS;	// 100msec
		key_manage.interval5ms = M_T5MS_COUNT_100MS;	// 100msec
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc6_interrupt
* Description  : This function is INTP6 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc6_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    //SW1:START/STOP
    if(b_KEY_EVENT_START!=ON){
        b_KEY_EVENT_START=ON;
//		key_manage.countdown5ms = M_T5MS_COUNT_100MS;	// 100msec
		key_manage.interval5ms = M_T5MS_COUNT_100MS;	// 100msec
     }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc7_interrupt
* Description  : This function is INTP7 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_intc7_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    //SW2:MEMORY
    if(b_KEY_EVENT_MEMORY!=ON){
        b_KEY_EVENT_MEMORY=ON;
// 		key_manage.countdown5ms = M_T5MS_COUNT_5S;	// 5sec
 		key_manage.interval5ms = M_T5MS_COUNT_5S;	// 5sec
    }
    /* End user code. Do not edit comment generated here */
}




/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
