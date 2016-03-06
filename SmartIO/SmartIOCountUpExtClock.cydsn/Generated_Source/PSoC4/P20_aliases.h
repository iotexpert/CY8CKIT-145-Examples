/*******************************************************************************
* File Name: P20.h  
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

#if !defined(CY_PINS_P20_ALIASES_H) /* Pins P20_ALIASES_H */
#define CY_PINS_P20_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define P20_0			(P20__0__PC)
#define P20_0_PS		(P20__0__PS)
#define P20_0_PC		(P20__0__PC)
#define P20_0_DR		(P20__0__DR)
#define P20_0_SHIFT	(P20__0__SHIFT)
#define P20_0_INTR	((uint16)((uint16)0x0003u << (P20__0__SHIFT*2u)))

#define P20_INTR_ALL	 ((uint16)(P20_0_INTR))


#endif /* End Pins P20_ALIASES_H */


/* [] END OF FILE */
