/*******************************************************************************
* File Name: led3.h  
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

#if !defined(CY_PINS_led3_ALIASES_H) /* Pins led3_ALIASES_H */
#define CY_PINS_led3_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define led3_0			(led3__0__PC)
#define led3_0_PS		(led3__0__PS)
#define led3_0_PC		(led3__0__PC)
#define led3_0_DR		(led3__0__DR)
#define led3_0_SHIFT	(led3__0__SHIFT)
#define led3_0_INTR	((uint16)((uint16)0x0003u << (led3__0__SHIFT*2u)))

#define led3_INTR_ALL	 ((uint16)(led3_0_INTR))


#endif /* End Pins led3_ALIASES_H */


/* [] END OF FILE */
