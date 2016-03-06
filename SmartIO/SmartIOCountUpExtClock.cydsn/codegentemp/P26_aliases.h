/*******************************************************************************
* File Name: P26.h  
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

#if !defined(CY_PINS_P26_ALIASES_H) /* Pins P26_ALIASES_H */
#define CY_PINS_P26_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define P26_0			(P26__0__PC)
#define P26_0_PS		(P26__0__PS)
#define P26_0_PC		(P26__0__PC)
#define P26_0_DR		(P26__0__DR)
#define P26_0_SHIFT	(P26__0__SHIFT)
#define P26_0_INTR	((uint16)((uint16)0x0003u << (P26__0__SHIFT*2u)))

#define P26_INTR_ALL	 ((uint16)(P26_0_INTR))


#endif /* End Pins P26_ALIASES_H */


/* [] END OF FILE */
