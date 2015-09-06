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
* File Name    : r_cg_intc.c
* Version      : CodeGenerator for RL78/L12 E1.00.00c [23 Mar 2012]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for INTC module.
* Creation Date: 2012/11/08
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_INTC_Create
* Description  : This function initializes INTP module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC_Create(void)
{
    PMK0 = 1U;    /* disable INTP0 operation */
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
    PMK1 = 1U;    /* disable INTP1 operation */
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
    PMK2 = 1U;    /* disable INTP2 operation */
    PIF2 = 0U;    /* clear INTP2 interrupt flag */
    PMK3 = 1U;    /* disable INTP3 operation */
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
    PMK4 = 1U;    /* disable INTP4 operation */
    PIF4 = 0U;    /* clear INTP4 interrupt flag */
    PMK5 = 1U;    /* disable INTP5 operation */
    PIF5 = 0U;    /* clear INTP5 interrupt flag */
    PMK6 = 1U;    /* disable INTP6 operation */
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
    PMK7 = 1U;    /* disable INTP7 operation */
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
    /* Set INTP3 low priority */
    PPR13 = 1U;
    PPR03 = 1U;
    /* Set INTP6 low priority */
    PPR16 = 1U;
    PPR06 = 1U;
    /* Set INTP7 low priority */
    PPR17 = 1U;
    PPR07 = 1U;
    EGN0 = _08_INTP3_EDGE_FALLING_SEL | _40_INTP6_EDGE_FALLING_SEL | _80_INTP7_EDGE_FALLING_SEL;
    /* Set INTP3 pin */
    PFSEG2 &= 0xFBU;
    PM3 |= 0x02U;
    /* Set INTP6 pin */
    PFSEG3 &= 0xF7U;
    PM14 |= 0x01U;
    /* Set INTP7 pin */
    PFSEG3 &= 0xFBU;
    PM14 |= 0x02U;
}

/***********************************************************************************************************************
* Function Name: R_INTC3_Start
* Description  : This function clears INTP3 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC3_Start(void)
{
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
    PMK3 = 0U;    /* enable INTP3 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_INTC3_Stop
* Description  : This function disables INTP3 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC3_Stop(void)
{
    PMK3 = 1U;    /* disable INTP3 interrupt */
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_INTC6_Start
* Description  : This function clears INTP6 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC6_Start(void)
{
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
    PMK6 = 0U;    /* enable INTP6 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_INTC6_Stop
* Description  : This function disables INTP6 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC6_Stop(void)
{
    PMK6 = 1U;    /* disable INTP6 interrupt */
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_INTC7_Start
* Description  : This function clears INTP7 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC7_Start(void)
{
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
    PMK7 = 0U;    /* enable INTP7 interrupt */
}

/***********************************************************************************************************************
* Function Name: R_INTC7_Stop
* Description  : This function disables INTP7 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC7_Stop(void)
{
    PMK7 = 1U;    /* disable INTP7 interrupt */
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
