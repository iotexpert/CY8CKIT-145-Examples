/*******************************************************************************
* File Name: P21.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_P21_ALIASES_H) /* Pins P21_ALIASES_H */
#define CY_PINS_P21_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define P21_0			(P21__0__PC)
#define P21_0_PS		(P21__0__PS)
#define P21_0_PC		(P21__0__PC)
#define P21_0_DR		(P21__0__DR)
#define P21_0_SHIFT	(P21__0__SHIFT)
#define P21_0_INTR	((uint16)((uint16)0x0003u << (P21__0__SHIFT*2u)))

#define P21_INTR_ALL	 ((uint16)(P21_0_INTR))


#endif /* End Pins P21_ALIASES_H */


/* [] END OF FILE */
