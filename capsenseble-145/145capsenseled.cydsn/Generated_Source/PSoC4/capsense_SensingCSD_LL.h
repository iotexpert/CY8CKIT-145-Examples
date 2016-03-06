/***************************************************************************//**
* \file capsense_SensingCSD_LL.h
* \version 3.0
*
* \brief
*   This file provides the headers of APIs specific to CSD sensing implementation.
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

#if !defined(CY_CAPSENSE_capsense_SENSINGCSD_LL_H)
#define CY_CAPSENSE_capsense_SENSINGCSD_LL_H

#include <CyLib.h>
#include <cyfitter.h>
#include "cytypes.h"
#include "capsense_Structure.h"
#include "capsense_Configuration.h"

/***************************************
* Function Prototypes
**************************************/

/**
* \if SECTION_CAPSENSE_LOW_LEVEL
* \addtogroup group_capsense_low_level
* \{
*/

void capsense_CSDConnectSns(capsense_FLASH_IO_STRUCT const *snsAddrPtr);
void capsense_CSDDisconnectSns(capsense_FLASH_IO_STRUCT const *snsAddrPtr);
void capsense_CSDScanExt(void);
void capsense_CSDScan(void);
#if ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) || \
     (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))
cystatus capsense_CSDCalibrateWidget(uint32 wdgtIndex, uint32 target);
#endif /* ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) || \
           (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))  */
void capsense_CSDSetupWidget(uint32 wdgtIndex);
void capsense_CSDSetupWidgetExt(uint32 wdgtIndex, uint32 snsIndex);

/** \}
* \endif */

/*****************************************************
* Function Prototypes - internal Low Level functions
*****************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

void capsense_CSDInitialize(void);
void capsense_CSDStartSample(void);
void capsense_SetUpIdacs(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt);
void capsense_CSDElectrodeCheck(void);
#if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
    void capsense_CSDConfigurePrsMode(void);
#endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */
#if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
     (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
    void capsense_DisableShieldElectrodes(void);
#endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
           (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */

/** \}
* \endif */

/***************************************
* Global software variables
***************************************/
extern uint32 capsense_configCsd;
/* Interrupt handler */
extern CY_ISR_PROTO(capsense_CSDPostSingleScan);
extern CY_ISR_PROTO(capsense_CSDPostMultiScan);
#if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
extern CY_ISR_PROTO(capsense_CSDPostMultiScanGanged);
#endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
#if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
    extern uint8 capsense_badConversionsNum;
#endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

#endif /* End CY_CAPSENSE_capsense_SENSINGCSD_LL_H */


/* [] END OF FILE */
