/*******************************************************************************
* File Name: indicate.h  
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

#if !defined(CY_PINS_indicate_ALIASES_H) /* Pins indicate_ALIASES_H */
#define CY_PINS_indicate_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define indicate_0			(indicate__0__PC)
#define indicate_0_PS		(indicate__0__PS)
#define indicate_0_PC		(indicate__0__PC)
#define indicate_0_DR		(indicate__0__DR)
#define indicate_0_SHIFT	(indicate__0__SHIFT)
#define indicate_0_INTR	((uint16)((uint16)0x0003u << (indicate__0__SHIFT*2u)))

#define indicate_INTR_ALL	 ((uint16)(indicate_0_INTR))


#endif /* End Pins indicate_ALIASES_H */


/* [] END OF FILE */
