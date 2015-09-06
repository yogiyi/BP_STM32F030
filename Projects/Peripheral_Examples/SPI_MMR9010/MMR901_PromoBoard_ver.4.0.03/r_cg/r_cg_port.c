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
* File Name    : r_cg_port.c
* Version      : CodeGenerator for RL78/L12 E1.00.00c [23 Mar 2012]
* Device(s)    : R5F10RLC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for PORT module.
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
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    PFSEG2 = _01_PFSEG16_SEG | _02_PFSEG17_SEG | _04_PFSEG18_SEG | _08_PFSEG19_SEG | _10_PFSEG20_SEG | _20_PFSEG21_SEG |
             _00_PFSEG22_PORT | _80_PFSEG23_SEG;
    PFSEG3 = _01_PFSEG24_SEG | _02_PFSEG25_SEG | _04_PFSEG26_SEG | _08_PFSEG27_SEG | _10_PFSEG28_SEG | _20_PFSEG29_SEG |
             _40_PFSEG30_SEG | _00_PFSEG31_PORT;
    PFSEG4 = _00_PFSEG32_PORT | _00_PFSEG33_PORT | _00_PFSEG34_PORT | _00_PFSEG35_PORT | _00_PFSEG36_PORT |
             _00_PFSEG37_PORT | _40_PFSEG38_SEG;
    ISCLCD = _02_ISCVL3_INPUT_EFFECTIVE;
    P1 = _00_Pn3_OUTPUT_0 | _10_Pn4_OUTPUT_1;
    P2 = _00_Pn1_OUTPUT_0;
    P4 = _00_Pn3_OUTPUT_0;
    P12 = _00_Pn5_OUTPUT_0;
    P13 = _00_Pn0_OUTPUT_0;
    P14 = _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn6_OUTPUT_0;
    PMC1 = _00_PMCn3_DI_ON | _00_PMCn4_DI_ON | _E7_PMC1_DEFAULT;
    PMC14 = _00_PMCn2_DI_ON | _00_PMCn3_DI_ON | _00_PMCn4_DI_ON | _00_PMCn5_DI_ON | _C3_PMC14_DEFAULT;
    ADPC = _02_ADPC_DI_ON;
    PM1 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
          _20_PMn5_NOT_USE | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE;
    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _FC_PM2_DEFAULT;
    PM4 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _00_PMn3_MODE_OUTPUT | _F0_PM4_DEFAULT;
    PM12 = _01_PMn0_NOT_USE | _00_PMn5_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE | _1E_PM12_DEFAULT;
    PM14 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _10_PMn4_MODE_INPUT |
           _20_PMn5_MODE_INPUT | _00_PMn6_MODE_OUTPUT | _80_PMn7_NOT_USE;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
