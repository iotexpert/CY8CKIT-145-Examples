/***************************************************************************//**
* \file     capsense_Filter.h
*
* \brief    This file contains the definitions for all firmware filters 
*           implementation.
*
* \version  3.0
*
* \note
*   None.
*
* Firmware Dependency:
*   1. capsense_Customizer.h
*   2. capsense_Configuration.h
*
* Hardware Dependency:
*   None.
*
* \see  1. 001-64846 Getting Started With CapSense
* \see  2. 001-85951 PSoC 4 CapSense Design Guide
* \see  3. CapSense Component Datasheet
* \see  4. PSoC 4 Device Technical Reference Manual
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

#if !defined(CY_CAPSENSE_capsense_FILTER_H)
#define CY_CAPSENSE_capsense_FILTER_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"
    
#include "capsense_Structure.h"    
#include "capsense_Configuration.h"    
 
  
/***************************************
* Function Prototypes 
***************************************/ 

/*******************************************************************************
* LOW LEVEL API 
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_LOW_LEVEL
* \addtogroup group_capsense_low_level
* \{
*/
    
cystatus capsense_UpdateAllBaselines(void);
cystatus capsense_UpdateWidgetBaseline(uint32 widgetId);
cystatus capsense_UpdateSensorBaseline(
                             uint32 widgetId, uint32 sensorId);

void capsense_InitializeAllBaselines(void);    
void capsense_InitializeWidgetBaseline(uint32 widgetId);
void capsense_InitializeSensorBaseline(uint32 widgetId, uint32 sensorId);

/** \}
* \endif */


/*******************************************************************************
* Function Prototypes - internal functions
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

void capsense_FtInitialize(void);

#if (capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN)
    void capsense_RunNoiseEnvelope(uint32 widgetId, uint32 sensorId);
    void capsense_InitializeNoiseEnvelope(uint32 widgetId, uint32 sensorId);
#endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */

#if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN)
    void capsense_InitializeIIR(uint32 widgetId, uint32 sensorId);
    void capsense_RunIIR(uint32 widgetId, uint32 sensorId);
#endif /* (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN) */

#if (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
    void capsense_InitializeMedian(uint32 widgetId, uint32 sensorId);
    void capsense_RunMedian(uint32 widgetId, uint32 sensorId);
#endif /* (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN) */

#if (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN)
    void capsense_InitializeAverage(uint32 widgetId, uint32 sensorId);
    void capsense_RunAverage(uint32 widgetId, uint32 sensorId);
#endif /* (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN) */

void capsense_FtInitializeBaseline(capsense_RAM_SNS_STRUCT *ptrSensor, uint32 wdType);
cystatus capsense_FtUpdateBaseline(
                            capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam,
                            capsense_RAM_SNS_STRUCT *ptrSensor, 
                            uint32 wdType);

#if (capsense_POS_MEDIAN_FILTER_EN || capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
uint32 capsense_FtMedian(uint32 x1, uint32 x2, uint32 x3);
#endif /*capsense_POS_MEDIAN_FILTER_EN || capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN*/

uint32 capsense_FtIIR1stOrder(uint32 input, uint32 prevOutput, uint32 n, uint32 shift);

#if (capsense_POS_JITTER_FILTER_EN)
    uint32 capsense_FtJitter(uint32 input, uint32 prevOutput);
#endif /* capsense_POS_JITTER_FILTER_EN */

void capsense_FtInitializeBaselineChannel(capsense_RAM_SNS_STRUCT *ptrSensor, uint32 wdType, uint32 channel); 

#if (capsense_REGULAR_RC_FILTER_EN || capsense_PROX_RC_FILTER_EN)
    void capsense_FtRunEnabledFilters(uint32 widgetId, uint32 sensorId);
    void capsense_FtRunEnabledFiltersInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj, 
                                                      capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (capsense_REGULAR_RC_FILTER_EN || capsense_PROX_RC_FILTER_EN) */


#if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN)
    void capsense_InitializeIIRInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                  capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
    void capsense_RunIIRInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                           capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN) */

#if (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
    void capsense_InitializeMedianInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                     capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);    
    void capsense_RunMedianInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                              capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);    
#endif /* (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN) */

#if (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN)
    void capsense_InitializeAverageInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                                      capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
    void capsense_RunAverageInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                               capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType);
#endif /* (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN) */


/** \}
* \endif */

/***************************************
* Initial Parameter Constants
***************************************/
#define NOISE_ENVELOPE_SHIFT                        (0x02u)
#define NOISE_ENVELOPE_RUN_DIFF_SHIFT               (0x03u)
#define NOISE_ENVELOPE_SIGN_REG                     (0x0Fu)
#define NOISE_ENVELOPE_SIGN_REG_SHIFT               (0x01u)
#define NOISE_ENVELOPE_RESET_COUNTER                (0x0Au)
#define NOISE_ENVELOPE_4_TIMES                      (0x02u)

#endif /* End CY_CAPSENSE_capsense_FILTER_H */


/* [] END OF FILE */
