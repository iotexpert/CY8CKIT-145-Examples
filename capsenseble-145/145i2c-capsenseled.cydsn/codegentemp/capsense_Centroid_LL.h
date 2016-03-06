/***************************************************************************//**
* \file capsense_Centroid_LL.h
* \version 3.0
*
* \brief
*   This file provides the function prototypes for the centroid calculation 
*   methods.
*
* \see CapSense P4 v3.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#if !defined(CY_CAPSENSE_capsense_CENTROID_LL_H)
#define CY_CAPSENSE_capsense_CENTROID_LL_H

#include "capsense_Configuration.h"
#include "capsense_Structure.h"
    
/*******************************************************************************
* Definitions
*******************************************************************************/
    
/* Centroid-related definitions */
#define capsense_CENTROID_POS_PREV                  (0u) 
#define capsense_CENTROID_POS                       (1u)
#define capsense_CENTROID_POS_NEXT                  (2u)
#define capsense_CENTROID_ROUND_VALUE               (0x7Fu)
#define capsense_CENTROID_MAXIMUM                   (0xFFu)
#define capsense_CENTROID_DIPLEX_SECTION_LENGTH     (0x02u)

/*******************************************************************************
* Function Prototypes 
*******************************************************************************/

/*******************************************************************************
* Function Prototypes - internal functions
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

uint32 capsense_DpFindLocalMaxSd(
                capsense_RAM_SNS_STRUCT *ptrSns, 
                uint32 snsCount, 
                uint32 fingerThreshold);

#if capsense_TOTAL_DIPLEXED_SLIDERS
    uint32 capsense_DpFindLocalMaxDiplex(
                    capsense_RAM_SNS_STRUCT *ptrSns, 
                    uint32 snsCount, 
                    uint8 const *diplexTable, 
                    uint32 fingerThreshold);
#endif /* #if capsense_TOTAL_DIPLEXED_SLIDERS */

uint32 capsense_DpCalcLinearCentroid(
                    uint16 *position, 
                    uint32 multiplier, 
                    uint32 snsCount);

#if capsense_TOTAL_RADIAL_SLIDERS
    uint32 capsense_DpCalcRadialCentroid(
                    uint16 *position, 
                    uint32 multiplier, 
                    uint32 snsCount);
#endif /* #if capsense_TOTAL_RADIAL_SLIDERS */

#if capsense_CSX_TOUCHPAD_WIDGET_EN        
    void capsense_DpFindLocalMaxDd(
                        capsense_FLASH_WD_STRUCT const *ptrFlashWdgt);
    
    void capsense_DpCalcTouchPadCentroid(
                        capsense_FLASH_WD_STRUCT const *ptrFlashWdgt);
    
    void capsense_DpTouchTracking(
                        capsense_FLASH_WD_STRUCT const *ptrFlashWdgt);

    uint32 capsense_DpFilterTouchRecord(
                        capsense_FLASH_WD_STRUCT const *ptrFlashWdgt);
#endif /* #if capsense_CSX_TOUCHPAD_WIDGET_EN */

/** \}
* \endif */

#endif /* End CY_CAPSENSE_capsense_CENTROID_LL_H */


/* [] END OF FILE */
