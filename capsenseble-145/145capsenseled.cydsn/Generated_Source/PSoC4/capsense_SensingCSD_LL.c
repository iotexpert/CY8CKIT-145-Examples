/***************************************************************************//**
* \file capsense_SensingCSD_LL.c
* \version 3.0
*
* \brief
*   This file defines the data structure global variables and provides 
*   implementation for the low-level APIs of the CSD part of 
*   the Sensing module. The file contains the APIs used for the CSD block
*   initialization, calibration, and scanning.
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
#include "capsense_Structure.h"
#include "capsense_Configuration.h"
#include "capsense_SensingCSD_LL.h"
#include "capsense_Sensing.h"
#include "cyapicallbacks.h"

#if (capsense_ENABLE == capsense_CSD_EN)

/***************************************
* API Constants
***************************************/
#define capsense_MOD_CSD_CLK_12MHZ                      (12000000uL)
#define capsense_MOD_CSD_CLK_24MHZ                      (24000000uL)
#define capsense_MOD_CSD_CLK_48MHZ                      (48000000uL)

#define capsense_DELAY_EXTRACYCLES_NUM                  (9u)
#define capsense_GLITCH_ELIMINATION_CYCLES_CALC         (capsense_CSD_PRESCAN_SETTLING_TIME * CYDEV_BCLK__SYSCLK__MHZ)
#define capsense_CMOD_DISCHARGE_TIME                    (2u)
#define capsense_CMOD_DISCHARGE_CYCLES_CALC             (capsense_CMOD_DISCHARGE_TIME * CYDEV_BCLK__SYSCLK__MHZ)

#if(capsense_GLITCH_ELIMINATION_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM)
    #define capsense_GLITCH_ELIMINATION_CYCLES          (capsense_GLITCH_ELIMINATION_CYCLES_CALC -\
                                                                capsense_DELAY_EXTRACYCLES_NUM)
#else
    #define capsense_GLITCH_ELIMINATION_CYCLES          (capsense_GLITCH_ELIMINATION_CYCLES_CALC)
#endif /* (capsense_GLITCH_ELIMINATION_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM) */

#if(capsense_CMOD_DISCHARGE_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM)
    #define capsense_CMOD_DISCHARGE_CYCLES              (capsense_CMOD_DISCHARGE_CYCLES_CALC -\
                                                                capsense_DELAY_EXTRACYCLES_NUM) 
#else
    #define capsense_CMOD_DISCHARGE_CYCLES              (capsense_CMOD_DISCHARGE_CYCLES_CALC)
#endif /* (capsense_CMOD_DISCHARGE_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM) */

#define capsense_COARSE_INIT_WATCHDOG_TIME              (3u)
#define capsense_COARSE_INIT_WATCHDOG_CYCLES_CALC       (capsense_COARSE_INIT_WATCHDOG_TIME * CYDEV_BCLK__SYSCLK__MHZ)

#if(capsense_COARSE_INIT_WATCHDOG_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM)
    #define capsense_COARSE_INIT_WATCHDOG_CYCLES        (capsense_COARSE_INIT_WATCHDOG_CYCLES_CALC -\
                                                                 capsense_DELAY_EXTRACYCLES_NUM) 
#else
    #define capsense_COARSE_INIT_WATCHDOG_CYCLES        (capsense_COARSE_INIT_WATCHDOG_CYCLES_CALC)
#endif /* (capsense_COARSE_INIT_WATCHDOG_CYCLES_CALC >= capsense_DELAY_EXTRACYCLES_NUM) */

#if (capsense_ENABLE == capsense_CSDV2)
    #if(capsense_CMOD_PAD == capsense_CMOD_CONNECTION)
        #define capsense_SW_DSI_SEL_CMODPAD                (0x00000010uL)
        #define capsense_CSD_SW_FW_MOD_SEL_INIT            (0x00010101uL)
    #else
        #define capsense_SW_DSI_SEL_CMODPAD                (0x00000000uL)
        #define capsense_CSD_SW_FW_MOD_SEL_INIT            (0x00000000uL)
    #endif /* (capsense_CMOD_PAD == capsense_CMOD_CONNECTION) */

    #if((capsense_CSH_TANK_PAD == capsense_CMOD_CONNECTION) || \
        (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION))
        #define capsense_SW_DSI_SEL_TANKPAD                (0x00000001uL)
        #define capsense_CSD_SW_FW_TANK_SEL_INIT           (0x00001110uL)
    #else
        #define capsense_SW_DSI_SEL_TANKPAD                (0x00000000uL)
        #define capsense_CSD_SW_FW_TANK_SEL_INIT           (0x00000000uL)
    #endif /* ((capsense_CSH_TANK_PAD == capsense_CMOD_CONNECTION) || \
               (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION)) */

    /* SW_FW_MOD_SEL switches state for Coarse initialization of CMOD (sense path) */
    #define capsense_CSD_SW_FW_MOD_SEL_SCAN                (0x00000000uL)

    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
         (0u != capsense_CSD_SHIELD_TANK_EN) && \
         (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION))
        #define capsense_CSD_SW_FW_TANK_SEL_SCAN            (0x00030010uL)
        #define capsense_DEFAULT_SW_SHIELD_SEL              (0x00000600uL)
    #elif((capsense_ENABLE == capsense_CSD_SHIELD_EN) && (0u != capsense_CSD_SHIELD_TANK_EN))
        #define capsense_CSD_SW_FW_TANK_SEL_SCAN            (0x00000000uL)
        #define capsense_DEFAULT_SW_SHIELD_SEL              (0x00000600uL)
    #elif(capsense_ENABLE == capsense_CSD_SHIELD_EN)
        #define capsense_CSD_SW_FW_TANK_SEL_SCAN            (0x00000000uL)
        #define capsense_DEFAULT_SW_SHIELD_SEL              (0x00002500uL)
    #else
        #define capsense_CSD_SW_FW_TANK_SEL_SCAN            (0x00000000uL)
        #define capsense_DEFAULT_SW_SHIELD_SEL              (0x00000000uL)        
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
               (0u != capsense_CSD_SHIELD_TANK_EN) && \
               (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION)) */

    #define capsense_DEFAULT_CSD_SW_DSI_SEL                 (capsense_SW_DSI_SEL_CMODPAD | capsense_SW_DSI_SEL_TANKPAD)

    #define capsense_CSD_SW_RES_INIT                        (capsense_CSD_INIT_SWITCH_RES << CYFLD_CSD_RES_HCAV__OFFSET)
    #define capsense_CSD_SW_RES_SCAN                        ((capsense_CSD_SHIELD_SWITCH_RES << CYFLD_CSD_RES_HCBV__OFFSET) | \
                                                                     (capsense_CSD_SHIELD_SWITCH_RES << CYFLD_CSD_RES_HCBG__OFFSET))

    #define capsense_DEFAULT_CSD_INTR_SET                   (0x00000000uL)
    #define capsense_DEFAULT_SW_HS_P_SEL                    (0x00000000uL)
    #define capsense_DEFAULT_SW_HS_N_SEL                    (0X00000000uL)
    #define capsense_DEFAULT_CSD_SW_FW_TANK_SEL             (0x00000000uL)
    #define capsense_DEFAULT_CSD_ADC_CTL                    (0x00000000uL)
    #define capsense_DEFAULT_HSCMP_CFG                      (0x00000000uL)

    #define capsense_CSD_ON_DELAY_TIME                      (3u)
    #define capsense_CSD_ON_DELAY                           (capsense_CSD_ON_DELAY_TIME * CYDEV_BCLK__SYSCLK__MHZ)
#endif /* (capsense_ENABLE == capsense_CSDV2) */ 

/***************************************
* Variables
***************************************/

uint32 capsense_configCsd = capsense_DEFAULT_CSD_CONFIG;
#if (capsense_DISABLE == capsense_CSDV2)
    static uint32 capsense_counterResolution = capsense_CNT_RESOLUTION_12_BITS;
#endif /* (capsense_DISABLE == capsense_CSDV2) */
/*  Pointer to Flash structure to hold Sns electrode that was connected previously  */
static capsense_FLASH_IO_STRUCT const *capsense_curSnsIOPtr;
/* Flag to indicate electrodes that were connected previously */
static uint8 capsense_eleCsdDisconnectFlag = 0u;
#if ((capsense_ENABLE == capsense_CSDV2) && (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN))
    /* Number of re-samplings when the bad conversion occurs */
    uint8 capsense_badConversionsNum = capsense_BAD_CONVERSIONS_NUM;
#endif /* ((capsense_ENABLE == capsense_CSDV2) && (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)) */

/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

#if (capsense_ENABLE == capsense_CSD_SHIELD_EN) 
    static void capsense_SsSetShieldDelay(uint32 delay);
    #if (0u != capsense_CSD_TOTAL_SHIELD_COUNT)
        static void capsense_SsEnableShieldElectrodes(void);
    #endif /* (0u != capsense_CSD_TOTAL_SHIELD_COUNT) */
#endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
static void capsense_SsClearCSDSensors(void);
#if ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) || \
     (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))        
    static cystatus capsense_SsCSDCalibrateCheck(uint32 rawLevel, uint32 rawData);
    static cystatus capsense_SsCSDCalibrateOneSensor(uint32 rawLevel, 
                                                             uint8 *ptrIdac, 
                                                             uint8 *iDAC8Max);                      
#endif /* ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) || \
           (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))  */
#if ((capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) && \
     (capsense_ENABLE == capsense_CSDV2))
    static uint32 capsense_SsCsdCalcLfsr(uint32 conversionsNum, uint32 snsClkDivider);
    static uint32 capsense_SsCsdCalcPrsSize(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt);
#endif /* ((capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) && \
           (capsense_ENABLE == capsense_CSDV2)) */
#if (capsense_ENABLE == capsense_CSDV2)
    static void capsense_SsCSDv2Initialize(void);
    static uint32 capsense_GetNumberOfConversions(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt);
#else
    static void capsense_SsCmodPrecharge(void);
    static void capsense_SsCSDv1Initialize(void);
#endif /* (capsense_ENABLE == capsense_CSDV2) */

/** \}
* \endif */

   
#if (capsense_ENABLE == capsense_CSD_SHIELD_EN) 
/*******************************************************************************
* Function Name: capsense_SsSetShieldDelay
****************************************************************************//**
*
* \brief
*   This is an internal function that sets a shield delay.
*
* \details
*   The function updates the CSD configuration register bits that define the shield 
*   signal delay relatively to the sense signal.
*
* \param delay 
*   Specifies the shield delay value:
*                               0 - no delay
*                               1 - 5ns delay (50 ns for CSDv1 block)
*                               2 - 10ns delay (100 ns for CSDv1 block)
*                               3 - 20ns delay
*
*******************************************************************************/
static void capsense_SsSetShieldDelay(uint32 delay)
{    
    /* Update CSD config register shield delay bits with shield delay value */
    capsense_configCsd &= (uint32)(~capsense_CONFIG_SHIELD_DELAY_MASK);
    capsense_configCsd |= (delay << capsense_SHIELD_DELAY_SHIFT);
}   


    #if (0u != capsense_CSD_TOTAL_SHIELD_COUNT)
    /*******************************************************************************
    * Function Name: capsense_SsEnableShieldElectrodes
    ****************************************************************************//**
    *
    * \brief
    *   This internal function initializes Shield Electrodes.
    *
    * \details
    *   The function sets the bit in the HSIOM register which enables the shield electrode
    *   functionality on the pin. The port and pin configurations are stored in  
    *   the capsense_shieldIoList structure.
    * 
    *******************************************************************************/
    static void capsense_SsEnableShieldElectrodes(void)
    {
        uint32 newRegValue;
        uint32 shieldIndex;

        for (shieldIndex = 0u; shieldIndex < capsense_CSD_TOTAL_SHIELD_COUNT; shieldIndex++)
        {
            /* Set drive mode to Analog */
            CY_SET_REG32(capsense_shieldIoList[shieldIndex].pcPtr, CY_GET_REG32(capsense_shieldIoList[shieldIndex].pcPtr) &
                       ~(capsense_GPIO_PC_MASK <<(capsense_shieldIoList[shieldIndex].shift)));

            /* Set HSIOM register mode to CSD_SHIELD or AMUXBUS B connection */
            newRegValue = CY_GET_REG32(capsense_shieldIoList[shieldIndex].hsiomPtr);
            newRegValue &= ~(capsense_HSIOM_SEL_MASK << (capsense_shieldIoList[shieldIndex].hsiomShift));
            
            /* Set HSIOM register mode to CSD_SHIELD */
            newRegValue |= (capsense_HSIOM_SEL_CSD_SHIELD << (capsense_shieldIoList[shieldIndex].hsiomShift));

            CY_SET_REG32(capsense_shieldIoList[shieldIndex].hsiomPtr, newRegValue);
        }         
    }


    /*******************************************************************************
    * Function Name: capsense_DisableShieldElectrodes
    ****************************************************************************//**
    *
    * \brief
    *   This internal function disables Shield Electrodes.
    *
    * \details
    *   The function resets the bit in the HSIOM register which disables the shield 
    *   electrode functionality on the pin. The port and pin configurations are  
    *   stored in  the capsense_shieldIoList structure.
    * 
    *******************************************************************************/
    void capsense_DisableShieldElectrodes(void)
    {
        uint32 newRegValue;
        uint32 shieldIndex;

        for (shieldIndex = 0u; shieldIndex < capsense_CSD_TOTAL_SHIELD_COUNT; shieldIndex++)
        {
            /* Reset HSIOM register (to GPIO state) */
            CY_SET_REG32(capsense_shieldIoList[shieldIndex].hsiomPtr, CY_GET_REG32(capsense_shieldIoList[shieldIndex].hsiomPtr) & 
                         (uint32)~(uint32)(capsense_HSIOM_SEL_MASK << capsense_shieldIoList[shieldIndex].hsiomShift));          
            
            /* Update port configuration register (drive mode to ground)  */
            newRegValue = CY_GET_REG32(capsense_shieldIoList[shieldIndex].pcPtr);
            newRegValue &= ~(capsense_GPIO_PC_MASK << capsense_shieldIoList[shieldIndex].shift);
            newRegValue |= (capsense_SNS_GROUND_CONNECT << capsense_shieldIoList[shieldIndex].shift);   
            CY_SET_REG32(capsense_shieldIoList[shieldIndex].pcPtr, newRegValue);
            
            /* Set logic 0 to port data register */
            CY_SET_REG32(capsense_shieldIoList[shieldIndex].drPtr, CY_GET_REG32(capsense_shieldIoList[shieldIndex].drPtr) &
                       (uint32)~(uint32)((uint32)1u << capsense_shieldIoList[shieldIndex].drShift));
        }         
    }
    #endif /* (0u != capsense_CSD_TOTAL_SHIELD_COUNT) */
#endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */


/*******************************************************************************
* Function Name: capsense_SsClearCSDSensors
****************************************************************************//**
*
* \brief
*  Resets all the CSD sensors to the non-sampling state by sequentially 
*  disconnecting all the sensors from the Analog MUX bus and putting them to
*  an inactive state. 
*
* \details
*   The function goes through all the widgets and updates appropriate bits in
*   the IO HSIOM, PC and DR registers depending on the Inactive sensor connection
*   parameter. DR register bits are set to zero when the Inactive sensor 
*   connection is Ground or Hi-Z.
* 
*******************************************************************************/
static void capsense_SsClearCSDSensors(void)
{
    uint32 wdgtIndex;
    uint32 snsIndex;
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        /* Declare ptr to sensor IO structure */  
        capsense_FLASH_IO_STRUCT const *curDedicatedSnsIOPtr;
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    for (wdgtIndex = 0u; wdgtIndex < capsense_TOTAL_WIDGETS; wdgtIndex++)
    {
        if (capsense_SENSE_METHOD_CSD_E ==
            (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))
        {
            capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                            capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash;
  
            /* Go through all sensors in widget */            
            for (snsIndex = 0u; snsIndex < (uint8)capsense_dsFlash.wdgtArray[(wdgtIndex)].totalNumSns; snsIndex++)
            {
                #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
                    /* Check ganged sns flag  */
                    if (capsense_GANGED_SNS_MASK == (capsense_dsFlash.wdgtArray[wdgtIndex].staticConfig & 
                                                             capsense_GANGED_SNS_MASK))
                    {     
                        /* Get sns pointer */     
                        capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                           capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash + 
                                                           snsIndex;

                        /* Get IO pointer for dedicated pin */
                        curDedicatedSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];
                        
                        /* Disconnect dedicated pin */
                        capsense_CSDDisconnectSns(curDedicatedSnsIOPtr);
                    }
                    else
                    {
                        /* Disable sensor */
                        capsense_CSDDisconnectSns(&capsense_curSnsIOPtr[snsIndex]); 
                    }
                #else
                    /* Disable sensor */
                    capsense_CSDDisconnectSns(&capsense_curSnsIOPtr[snsIndex]);    
                #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */
            }
        }
    }   
}


#if ((capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) && \
     (capsense_ENABLE == capsense_CSDV2))
/*******************************************************************************
* Function Name: capsense_SsCsdCalcLfsr
****************************************************************************//**
*
* \brief
*   The function finds SSC polynomial size in the Auto mode.
*
* \details
*   The SSC polynomial size in the Auto mode is found based on the following
*   requirements: 
*   - LFSR value should be selected such as max clock dither is limited with +/-10%
*   - at least one full spread spectrum polynomial should pass during scan time.
*
* \param conversionsNum    Number of conversions.
* \param snsClkDivider     Sense Clock Divider.
* \return lfsrSize LFSRSIZE value for SENSE_PERIOD register.
*
*******************************************************************************/
static uint32 capsense_SsCsdCalcLfsr(uint32 conversionsNum, uint32 snsClkDivider)
{    
    uint32 lfsrSize;

    if ((capsense_SNSCLK_SSC5_LENGTH <= conversionsNum) && 
        (capsense_SNSCLK_SSC5_THRESHOLD < snsClkDivider))
    {
        lfsrSize = capsense_CLK_SOURCE_SSC5;
    }
    else if ((capsense_SNSCLK_SSC4_LENGTH <= conversionsNum) && 
             (capsense_SNSCLK_SSC4_THRESHOLD < snsClkDivider))
    {
        lfsrSize = capsense_CLK_SOURCE_SSC4;
    }    
    else if ((capsense_SNSCLK_SSC3_LENGTH <= conversionsNum) && 
             (capsense_SNSCLK_SSC3_THRESHOLD < snsClkDivider))
    {
        lfsrSize = capsense_CLK_SOURCE_SSC3; 
    }
    else
    {
        lfsrSize = capsense_CLK_SOURCE_SSC2;
    }
    
    return lfsrSize << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT;
}


/*******************************************************************************
* Function Name: capsense_SsCsdCalcPrsSize
****************************************************************************//**
*
* \brief
*   The function finds SSC polynomial size in the Auto mode.
*
* \details
*   The SSC polynomial size in the Auto mode is found based on the following
*   requirements: 
*   - LFSR value should be selected such as max clock dither is limited with +/-10%
*   - at least one full spread spectrum polynomial should pass during scan time.
*
* \param ptrWdgt The pointer to the RAM_WD_BASE_STRUCT structure.  
* \return lfsrSize LFSRSIZE value for SENSE_PERIOD register.
*
*******************************************************************************/
static uint32 capsense_SsCsdCalcPrsSize(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt)
{    
    uint32 prsSize;
    uint32 senseClkDivMath;

    #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
        senseClkDivMath = (uint32)capsense_dsRam.snsCsdClk;
    #elif (capsense_CSD_MATRIX_WIDGET_EN  | capsense_CSD_TOUCHPAD_WIDGET_EN)
        if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
        { 
            senseClkDivMath = (uint32)ptrWdgt->rowSnsClk;    
        }
        else
        {
            senseClkDivMath = (uint32)ptrWdgt->snsClk;   
        } 
    #else
        senseClkDivMath = (uint32)ptrWdgt->snsClk;
    #endif /* (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */

    if ((senseClkDivMath * capsense_PRS_LENGTH_12_BITS) <
        ((uint32)((0x00000001Lu << ptrWdgt->resolution) - 1u)))
    {
        prsSize = (capsense_CLK_SOURCE_PRS12 << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT) |
                   capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK;
    }
    else
    {
        prsSize = (capsense_CLK_SOURCE_PRS8 << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT) | 
                   capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK;
    }

    return prsSize;
}
#endif /* ((capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) && \
           (capsense_ENABLE == capsense_CSDV2)) */


#if (capsense_ENABLE == capsense_CSDV2)        
/*******************************************************************************
* Function Name: capsense_SsCSDv2Initialize
****************************************************************************//**
*
* \brief
*   This API initializes the CSDv2 module.
*
* \details
*
*   The function performs the following steps for CSDv2 HW block:
*   1) Sets GPIO output to "0" for all sensor pins;
*   2) Connects CMOD to AMUXBUS-A and to CSDBUS-A;
*   3) Connects CMOD to (sense path) to CSDCOMP;
*   4) Connects Csh_tank to AMUXBUS-B and to CSDBUS-B;
*   5) Connects VREF to REFGEN;
*   6) Configures REFGEN and sets the reference voltage;
*   7) Connects VREF to CSDCOMP and HSCOMP;
*   8) Configures IDAC and connect to CSDBUS-A (to drive CMOD);
*   9) Configures ModClk;
*   10) Configure SnsClk source;
*   11) Sets other CSD configurations (Csd Auto Zero time,
*       Sample Init period, interrupts, 
*       CMOD and Csh_tank/shield initialization switch resistance).
*
*******************************************************************************/
static void capsense_SsCSDv2Initialize(void)
{
    uint32 newRegValue;

    /* Set all sensor pins and shield electrodes to the unscanned state */
    capsense_SsClearCSDSensors();
    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
         (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
        capsense_DisableShieldElectrodes();
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
               (0u != capsense_CSD_TOTAL_SHIELD_COUNT))) */

    /* Initialize the unused CSD IP registers to defaut state */
    CY_SET_REG32(capsense_INTR_SET_PTR,         capsense_DEFAULT_CSD_INTR_SET);
    CY_SET_REG32(capsense_SW_FW_TANK_SEL_PTR,   capsense_DEFAULT_CSD_SW_FW_TANK_SEL);
    CY_SET_REG32(capsense_SW_DSI_SEL_PTR,       capsense_DEFAULT_CSD_SW_DSI_SEL);
    CY_SET_REG32(capsense_ADC_CTL_PTR,          capsense_DEFAULT_CSD_ADC_CTL);
    CY_SET_REG32(capsense_AMBUF_PTR,            capsense_AMBUF_PWR_MODE_OFF);
    CY_SET_REG32(capsense_SW_SHIELD_SEL_PTR,    capsense_DEFAULT_SW_SHIELD_SEL);
    CY_SET_REG32(capsense_SW_HS_P_SEL_PTR,      capsense_DEFAULT_SW_HS_P_SEL);
    CY_SET_REG32(capsense_SW_HS_N_SEL_PTR,      capsense_DEFAULT_SW_HS_N_SEL);
    CY_SET_REG32(capsense_HSCMP_PTR,            capsense_DEFAULT_HSCMP_CFG);
    

    /* Connect CMOD to AMUXBUS-A */
    newRegValue = CY_GET_REG32(capsense_CMOD_HSIOM_PTR); 
    newRegValue &= (uint32)(~(uint32)capsense_CMOD_HSIOM_MASK);
    newRegValue |= (capsense_HSIOM_SEL_AMUXA << capsense_CMOD_HSIOM_SHIFT);
    CY_SET_REG32(capsense_CMOD_HSIOM_PTR, newRegValue);

    /* Connect AMUXBUS-A to CSDBUS-A */
    newRegValue = CY_GET_REG32(capsense_SW_BYP_SEL_PTR);
    newRegValue |= capsense_SW_BYP_SEL_SW_BYA_MASK;
    CY_SET_REG32(capsense_SW_BYP_SEL_PTR, newRegValue);

    /* Connect CMOD to (sense path) to CSDCOMP */
    #if (capsense_CMOD_PAD == capsense_CMOD_CONNECTION)
        CY_SET_REG32(capsense_SW_CMP_P_SEL_PTR, capsense_SW_CMP_P_SEL_SW_SFPM_STATIC_CLOSE);
    #elif (capsense_CSHIELD_PAD == capsense_CMOD_CONNECTION)
        CY_SET_REG32(capsense_SW_CMP_P_SEL_PTR, capsense_SW_CMP_P_SEL_SW_SFPS_STATIC_CLOSE);
    #else
        CY_SET_REG32(capsense_SW_CMP_P_SEL_PTR, capsense_SW_CMP_P_SEL_SW_SFPT_STATIC_CLOSE);
    #endif /* (capsense_CMOD_PAD == capsense_CMOD_CONNECTION) */

    /* Connect Csh to AMUXBUS-B */
    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
         (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN))
        newRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)capsense_CSH_HSIOM_MASK);
        newRegValue |= (capsense_CSH_TO_AMUXBUS_B_MASK << capsense_CSH_HSIOM_SHIFT);
        CY_SET_REG32(capsense_CSH_HSIOM_PTR, newRegValue);

        newRegValue = CY_GET_REG32(capsense_CSH_PC_PTR);
        newRegValue &= (uint32)(~(capsense_CSH_PC_MASK << capsense_CSH_PC_SHIFT));
        newRegValue |= (capsense_CSH_PC_STRONG_MODE << capsense_CSH_PC_SHIFT);
        CY_SET_REG32(capsense_CSH_PC_PTR, newRegValue);

        newRegValue = CY_GET_REG32(capsense_CSH_DR_PTR);
        newRegValue &= (uint32)(~(capsense_DR_MASK << capsense_CSH_SHIFT));
        newRegValue |= (uint32)capsense_CSH_DR_CONFIG;
        CY_SET_REG32(capsense_CSH_DR_PTR, newRegValue);
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
               (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN)) */

    /* Configure shield driving path */
    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
        /* Connect AMUXBUS-B to CSDBUS-B (and AMUXBUS-A to CSDBUS-A ) */
        CY_SET_REG32(capsense_SW_BYP_SEL_PTR, capsense_SW_BYP_SEL_SW_BYA_MASK | capsense_SW_BYP_SEL_SW_BYB_MASK);
        
        /* Connect AMUXBUS-B to HSCMP positive input */
        CY_SET_REG32(capsense_SW_HS_P_SEL_PTR, capsense_SW_HS_P_SEL_SW_HMMB_STATIC_CLOSE);
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */

    /* Configure VREF */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)
        /* Connect VREF to REFGEN. Connect IDACB to CSDBUSA */
        newRegValue = CY_GET_REG32(capsense_SW_REFGEN_SEL_PTR);
        newRegValue |= capsense_SW_REFGEN_SEL_SW_SGR_MASK | capsense_SW_REFGEN_SEL_SW_IAIB_MASK;
        CY_SET_REG32(capsense_SW_REFGEN_SEL_PTR, newRegValue);
    #else
        /* Connect VREF to REFGEN (IAIB switch is open) */
        newRegValue = CY_GET_REG32(capsense_SW_REFGEN_SEL_PTR);
        newRegValue |= capsense_SW_REFGEN_SEL_SW_SGR_MASK;
        CY_SET_REG32(capsense_SW_REFGEN_SEL_PTR, newRegValue);
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN */

    #if (capsense_2000_MV > capsense_CYDEV_VDDA_MV)

        /* Connect VREFLOW (from AMBUF and RefGen is in bypass mode) to CSDCOMP when Vdda < 2 V */
        CY_SET_REG32(capsense_SW_CMP_N_SEL_PTR, capsense_SW_CMP_N_SEL_SW_SCRL_STATIC_CLOSE);
        
        /* Configure REFGEN to BYPASS mode when Vdda < 2 V */
        CY_SET_REG32(capsense_REFGEN_PTR, capsense_REFGEN_REFGEN_EN_MASK | 
                                                  capsense_REFGEN_BYPASS_MASK | 
                                                  capsense_REFGEN_VREFLO_INT_MASK);                              
                                                  
        /*  Connect Vrefhi to AMUBUF positive input when Vdaa < 2V
         *  Connect AMUBUF to SCDCMP negative input when Vdaa < 2V
         */
        CY_SET_REG32(capsense_SW_AMUXBUF_SEL_PTR, capsense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE |
                                                          capsense_SW_AMUXBUF_SEL_SW_IRLB_STATIC_CLOSE);
    
        /* Turn on CSD_AMBUF normal power level when Vdaa < 2V */
        CY_SET_REG32(capsense_AMBUF_PTR, capsense_AMBUF_PWR_MODE_NORM);                                           
    #else

        /* Connect VREFHI (from RefGen) to CSDCOMP when Vdda >= 2 V */
        CY_SET_REG32(capsense_SW_CMP_N_SEL_PTR, capsense_SW_CMP_N_SEL_SW_SCRH_STATIC_CLOSE);

        #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)

            /* Configure REFGEN. Set reference voltage when Vdda >= 2 V */
            CY_SET_REG32(capsense_REFGEN_PTR, capsense_REFGEN_WITH_SHIELD);
            
            /* Turn on CSD_AMBUF high power level when Vdaa >= 2V */
            CY_SET_REG32(capsense_AMBUF_PTR, capsense_AMBUF_PWR_MODE_HI);
        #else

            /* Configure REFGEN. Set reference voltage when Vdda >= 2 V */
            CY_SET_REG32(capsense_REFGEN_PTR, capsense_REFGEN_NO_SHIELD);

            /* Connect VREFHI to HSCOMP */       
            CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE);
        #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */

        /* Make sure that CSD_AMBUF is unconnected from Vrefhi and from Vreflo */
        CY_SET_REG32(capsense_SW_AMUXBUF_SEL_PTR, capsense_SW_AMUXBUF_SEL_SW_DEFAULT);

    #endif /* (capsense_2000_MV > capsense_CYDEV_VDDA_MV) */

    /* Configure IDACs mode */
    CY_SET_REG32(capsense_IDAC_MOD_PTR, capsense_DEFAULT_IDAC_MOD); 
    #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
         (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN))
        CY_SET_REG32(capsense_IDAC_COMP_PTR, capsense_DEFAULT_IDAC_COMP);
    #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
               (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)) */
    
    /* Configure ModClk */
   capsense_ConfigureModClkClock((uint32)capsense_dsRam.modCsdClk);

    #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
         #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
            /* Configure SnsClk source and divider */ 
            CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | ((uint32)capsense_dsRam.snsCsdClk - 1uL));
         #else   
            /* Configure PRS mode */
            capsense_CSDConfigurePrsMode();
         #endif /* (capsense_ENABLE == capsense_CSDV2) */
    #else
        /* Configure SnsClk source */
        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD);
    #endif /* (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
    
    /* Set other CSD configurations */
    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN) 
        capsense_SsSetShieldDelay(capsense_CSD_SHIELD_DELAY);
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
    
    /* Set Csd Auto Zero time (set AZ_TIME bitmask) */
    CY_SET_REG32(capsense_SEQ_TIME_PTR, capsense_CSD_AUTO_ZERO_TIME);

    /* Select CMOD and Csh_tank/shield initialization switch resistance */
    CY_SET_REG32(capsense_SW_RES_PTR, 0x00000000);
    
    /* Set the number of dummy fine initialization cycles */
    CY_SET_REG32(capsense_SEQ_INIT_CNT_PTR, capsense_SEQ_INIT_CNT_CONV_CNT_FINE_INIT);
 
    /* Set Phi1 and Phi2 switches to non-overlapping (no low EMI)*/
    CY_SET_REG32(capsense_SENSE_DUTY_PTR, capsense_SENSE_DUTY_OVERLAP_PHI1_MASK | 
                                                  capsense_SENSE_DUTY_OVERLAP_PHI2_MASK);
}
#endif /* (capsense_ENABLE == capsense_CSDV2) */
 
 
#if (capsense_DISABLE == capsense_CSDV2)        
/*******************************************************************************
* Function Name: capsense_SsCSDv1Initialize
****************************************************************************//**
*
* \brief
*   This API initializes the CSDv1 module.
*
* \details
*
*   The function performs the following steps for CSDv1 HW block:
*   1) Trims IDAC1 and IDAC2 for the CSD Mode;
*   2) Enables Shield Electrodes;
*   3) Configures the CSD block and IDACs;
*   4) Connects Cmod to AMUXBUS-A;
*   5) Enables the clocks;
*   6) Sets the shield delay;
*   7) Enables the CSD block; connects Vref Buffer to the AMUX bus;
*   8) Enables the CSD interrupt;
*   9) Sets all the sensors to the inactive state.
*
*******************************************************************************/
static void capsense_SsCSDv1Initialize(void)
{
    uint32 newRegValue;

    /* Load trim values from SFLASH rows to calibrate IDAC1 and IDAC2 for CSD Mode */
    #if (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG)
        /* iDAC1 Sinking Mode */ 
        newRegValue = CY_GET_REG32(capsense_TRIM2_PTR) & ~capsense_IDAC_TRIM2_MOD_SNK_MASK;
        newRegValue |= (uint32)((uint32)CY_GET_REG8(capsense_SFLASH_TRIM2_PTR) & 
                       (uint32)capsense_SFLASH_TRIM_IDAC_MOD_MASK) ;

        #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
            /* iDAC2 Sinking Mode */ 
            newRegValue &= ~capsense_IDAC_TRIM2_COMP_SNK_MASK;
            newRegValue |= (uint32)((uint32)CY_GET_REG8(capsense_SFLASH_TRIM2_PTR) & 
                           (uint32)capsense_SFLASH_TRIM_IDAC_COMP_MASK); 
        #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
        capsense_TRIM2_REG = newRegValue;
    #else   
        /* iDAC1 Sourcing Mode */ 
        newRegValue = CY_GET_REG32(capsense_TRIM1_PTR) & ~capsense_IDAC_TRIM1_MOD_SRC_MASK;
        newRegValue |= (uint32)((uint32)CY_GET_REG8(capsense_SFLASH_TRIM1_PTR) & 
                       (uint32)capsense_SFLASH_TRIM_IDAC_MOD_MASK);

        #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
             /* iDAC2 Sourcing Mode */ 
            newRegValue &= ~capsense_IDAC_TRIM1_COMP_SRC_MASK;
            newRegValue |= (uint32)((uint32)CY_GET_REG8(capsense_SFLASH_TRIM1_PTR) & 
                           (uint32)capsense_SFLASH_TRIM_IDAC_COMP_MASK);
        #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */      
        CY_SET_REG32(capsense_TRIM1_PTR, newRegValue);
    #endif /* (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG) */
 
     /* Configure Shield electrodes */
    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
        #if (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN)
            newRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR);
            newRegValue &= (uint32)(~(uint32)capsense_CSH_HSIOM_MASK);
            newRegValue |= (capsense_CSH_TO_AMUXBUS_B_MASK << capsense_CSH_HSIOM_SHIFT);
            CY_SET_REG32(capsense_CSH_HSIOM_PTR, newRegValue);
            #if (capsense_CSH_PRECHARGE_IO_BUF == capsense_CSD_CSH_PRECHARGE_SRC)
                newRegValue = CY_GET_REG32(capsense_CSH_PC_PTR);
                newRegValue &= (uint32)(~(capsense_CSH_PC_MASK << capsense_CSH_PC_SHIFT));
                newRegValue |= (capsense_CSH_PC_STRONG_MODE << capsense_CSH_PC_SHIFT);
                CY_SET_REG32(capsense_CSH_PC_PTR, newRegValue);

                newRegValue = CY_GET_REG32(capsense_CSH_DR_PTR);
                newRegValue &= (uint32)(~(capsense_DR_MASK << capsense_CSH_SHIFT));
                newRegValue |= (uint32)capsense_CSH_DR_CONFIG;
                CY_SET_REG32(capsense_CSH_DR_PTR, newRegValue);
            #endif /* (capsense_CSH_PRECHARGE_IO_BUF == capsense_CSD_CSH_PRECHARGE_SRC) */
        #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN) */
    
        #if (0u != capsense_CSD_TOTAL_SHIELD_COUNT)
            capsense_SsEnableShieldElectrodes();
        #endif /* (0u != capsense_CSD_TOTAL_SHIELD_COUNT) */
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */

    /* Configure IDAC */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        CY_SET_REG32(capsense_IDAC_PTR, capsense_DEFAULT_CSD_IDAC_CONFIG);
    #else
        CY_SET_REG32(capsense_IDAC_PTR, CY_GET_REG32(capsense_IDAC_PTR) &
                                                            ~(capsense_IDAC_MOD_MODE_MASK | capsense_IDAC_MOD_MASK));
        CY_SET_REG32(capsense_IDAC_PTR, CY_GET_REG32(capsense_IDAC_PTR) | capsense_DEFAULT_CSD_IDAC_CONFIG);
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
    
    /* Connect Cmod to AMUXBUS-A using HSIOM registers */ 
    newRegValue = CY_GET_REG32(capsense_CMOD_HSIOM_PTR); 
    newRegValue &= (uint32)(~(uint32)capsense_CMOD_HSIOM_MASK);
    newRegValue |= (capsense_HSIOM_SEL_AMUXA << capsense_CMOD_HSIOM_SHIFT);
    CY_SET_REG32(capsense_CMOD_HSIOM_PTR, newRegValue);

    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN) 
        capsense_SsSetShieldDelay(capsense_CSD_SHIELD_DELAY);
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */

    /* Enable CSD block. Connect Vref Buffer to AMUX bus to make sure that Cmod is charged before scanning in active mode */
    #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
    #else
        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CTANK_PRECHARGE_CONFIG_CSD_EN);
    #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */

    #if ((capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
         (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE))
        /* Configure PRS mode */
        capsense_CSDConfigurePrsMode();
    #endif /* ((capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
         (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)) */
    
    /* Set all sensors to inactive state */
    capsense_SsClearCSDSensors();
}
#endif /* (capsense_DISABLE == capsense_CSDV2) */          


/*******************************************************************************
* Function Name: capsense_CSDInitialize
****************************************************************************//**
*
* \brief
*   This API initializes the CSD module.
*
* \details
*   Depend on the configuration this function calls the correspondent function
*   to configure CSDv1 or CSDv2 HW IP block.
*
*******************************************************************************/
void capsense_CSDInitialize(void)
{
#if (capsense_ENABLE == capsense_CSDV2)
    /* Configure CSDv2 HW IP block */                       
    capsense_SsCSDv2Initialize();
#else
    /* Configure CSDv2 HW IP block */
    capsense_SsCSDv1Initialize();
#endif /* (capsense_ENABLE == capsense_CSDV2) */
}


/*******************************************************************************
* Function Name: capsense_CSDElectrodeCheck
****************************************************************************//**
*
* \brief
*   Checks if electrodes were previously connected using 
 * capsense_CSDSetupWidgetExt() API and if yes disconnects them.
*
* \details
*   This function checks if capsense_eleCsdDisconnectFlag is set.
*   If it set, the function disconnects the previously connected electrode. 
*
*******************************************************************************/
void capsense_CSDElectrodeCheck(void)
{
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  
        uint32 tempVal;
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    if (capsense_ENABLE == capsense_eleCsdDisconnectFlag)  
    {
        /* Disconnect if electrodes were previous connected by CSDSetupWidgetExt() API */
        #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)        
            /* Check ganged sns flag  */
            if (capsense_GANGED_SNS_MASK == (capsense_curFlashWdgtPtr->staticConfig & 
                capsense_GANGED_SNS_MASK))
            { 
                /* 
                 *   Disable ganged sensor
                 *   1. Get sns pointer
                 *   2. Reset electrode counter  
                 *   3. Get CSD pointer
                 *   4. Connect all ganged sensors   
                 */        
                capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                   capsense_curFlashWdgtPtr->ptr2SnsFlash;
                tempVal = (uint32)capsense_curFlashSnsPtr->numPins;
                capsense_curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];
                
                do
                {
                    capsense_CSDDisconnectSns(capsense_curSnsIOPtr);
                    capsense_curSnsIOPtr++;
                    tempVal--;
                } while (0u != tempVal);
            }
            else
            {
                /* Disable sensor */
                capsense_CSDDisconnectSns(capsense_curSnsIOPtr);
            }
        #else
            /* Disable sensor */
            capsense_CSDDisconnectSns(capsense_curSnsIOPtr);    
        #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

        capsense_eleCsdDisconnectFlag = 0u;
    }
}


/*******************************************************************************
* Function Name: capsense_SetUpIdacs
****************************************************************************//**
*
* \brief
*  This internal function changes the IDACs values. 
*
* \details
*  If Compensation IDAC is enabled, it updates two IDACs.
*  The Modulator IDAC is common for all the sensors of the widget.   
*  The Compensation IDAC is updated for the sensor of the widget which 
*  is available in the RAM_SNS_STRUCT structure.
*  If the Compensation IDAC is disabled, the function updates the Modulator IDAC 
*  value only in the RAM_WD_BASE_STRUCT structure.
*
* \param
*  ptrWdgt The pointer to the RAM_WD_BASE_STRUCT structure.  
*
*******************************************************************************/
void capsense_SetUpIdacs(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt)
{   

    uint8 interruptState;

#if (capsense_ENABLE == capsense_CSDV2)
    uint32 idacARegValue;
    #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
         (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN))
        uint32 idacBRegValue;
    #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
               (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)) */

    interruptState = CyEnterCriticalSection();

    /* Get IDACA Value */
    idacARegValue = CY_GET_REG32(capsense_IDAC_MOD_PTR);
    
    /* Set IDACA Value mask */
    idacARegValue &= ~(capsense_IDAC_MOD_VAL_MASK);
    
    /* Set IDACA value */
    #if (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
        if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
        {
            idacARegValue |= (uint32)ptrWdgt->rowIdacMod[capsense_scanFreqIndex];
        }
        else
        {
            idacARegValue |= (uint32)ptrWdgt->idacMod[capsense_scanFreqIndex];
        }
    #else
        idacARegValue |= (uint32)ptrWdgt->idacMod[capsense_scanFreqIndex];
    #endif /* (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN) */
    
    /* Update IDACA register with new value */
    CY_SET_REG32(capsense_IDAC_MOD_PTR, idacARegValue); 
    
    #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
         (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN))
        /* Get IDACB Value */
        idacBRegValue = CY_GET_REG32(capsense_IDAC_COMP_PTR);
        
        /* Set IDACB Value mask */
        idacBRegValue &= ~(capsense_IDAC_COMP_VAL_MASK);
        
        /* Set IDACB value */
        idacBRegValue |= (uint32)capsense_curRamSnsPtr->idacComp[capsense_scanFreqIndex];
        
        /* Update IDACA register with new value */
        CY_SET_REG32(capsense_IDAC_COMP_PTR, idacBRegValue);
    #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
               (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)) */
               
    CyExitCriticalSection(interruptState);

#else
    
    uint32 newRegValue;
    uint8 const *ptrIdacMod = ptrWdgt->idacMod;

    #if (capsense_CSD_MATRIX_WIDGET_EN  | capsense_CSD_TOUCHPAD_WIDGET_EN)
        if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
        {
            ptrIdacMod = &ptrWdgt->rowIdacMod[0u];
        }
    #endif /* (capsense_CSD_MATRIX_WIDGET_EN  | capsense_CSD_TOUCHPAD_WIDGET_EN) */

    interruptState = CyEnterCriticalSection();

    /* Get Idac Value */
    newRegValue = CY_GET_REG32(capsense_IDAC_PTR);
    
    /* Set Idac Value mask */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) 
        newRegValue &= ~(capsense_IDAC_MOD_MASK | capsense_IDAC_COMP_MASK);
    #else
        newRegValue &= ~(capsense_IDAC_MOD_MASK);
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
    
    /* Set Idac value */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        newRegValue |= (ptrIdacMod[capsense_scanFreqIndex] |        
                       (uint32)((uint32)capsense_curRamSnsPtr->idacComp[capsense_scanFreqIndex] <<
                       capsense_IDAC_COMP_DATA_OFFSET));
    #else
        newRegValue |= ptrIdacMod[capsense_scanFreqIndex]; 
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */       
    CY_SET_REG32(capsense_IDAC_PTR, newRegValue);
    
    CyExitCriticalSection(interruptState);

#endif /* (capsense_ENABLE == capsense_CSDV2) */
}


#if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
    /*******************************************************************************
    * Function Name: capsense_CSDConfigurePrsMode
    ****************************************************************************//**
    *
    * \brief
    *   This internal function configures PRS-AUTO mode.
    *
    * \details
    *   The function updates snsClk only. The function does not update snsClk 
    *   when the Enable common sense clock parameter is set in the customizer.
    *
    *   In the PRS mode, an appropriate bit is set in the CSD configuration register.
    *   For CSDv1 In the PRSAuto mode, the PRS12 mode is switched on automatically 
    *   basing on the following equation:
    *   2^12/Fsense < 2^N/Fmod,
    *   where 
    *   N - the scanning resolution
    *   Fsense - the sense clock frequency
    *   Fmod - the modulator clock frequency
    *
    *   In the SmartSense mode, the function additionally checks the status in 
    *   the prescalersTuningDone variable because the Auto-tune algorithm always  
    *   performs the first calibration in the PWM mode and the second calibration
    *   is performed in the sense clock mode which is set by the customizer.
    *
    *******************************************************************************/
    void capsense_CSDConfigurePrsMode(void)
    {
    #if ((capsense_ENABLE == capsense_CSDV2) && (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE))
        uint32 newRegValue;
        uint32 snsClkDivider;
    #elif (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)
        uint32 senseClkDivMath;
        #if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
             uint32 modClkDivMath;
        #endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */
    #else
        /* Not supported mode */
    #endif /* ((capsense_ENABLE == capsense_CSDV2) && (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)) */
        
    #if (capsense_ENABLE == capsense_CSDV2)
       
        /* Code below  for CSDv2 HW block */ 

        #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) || \
             (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE))
            capsense_RAM_WD_BASE_STRUCT const *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) || \
                   (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)) */
        
        #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
            /* Use direct clock if pre-scalers have not been tuned */
            if (capsense_ENABLE == capsense_prescalersTuningDone)
            {
        #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
        
                /* Get dividers */
                #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
                    snsClkDivider = (uint32)capsense_dsRam.snsCsdClk;
                #elif (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
                    if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
                    {
                        snsClkDivider = (uint32)ptrWdgt->rowSnsClk;
                    }
                    else
                    {
                        snsClkDivider = (uint32)ptrWdgt->snsClk;  
                    } 
                #else
                    snsClkDivider = (uint32)ptrWdgt->snsClk;  
                #endif /*  (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
        
                /* Get PRS size */
                #if (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)
                     newRegValue = snsClkDivider * (uint32)capsense_dsRam.modCsdClk;
                
                     if (capsense_HFCLK_SNSCLK_FACTOR < newRegValue)
                     {
                         /* Set SSC mode for the low sense clock frequencies */ 
                         newRegValue = capsense_GetNumberOfConversions(ptrWdgt);  
                         newRegValue = capsense_SsCsdCalcLfsr(newRegValue, snsClkDivider);
                     }
                     else
                     {
                         /* Set PRS8/PRS12 mode for the high sense clock frequencies */ 
                         newRegValue = capsense_SsCsdCalcPrsSize(ptrWdgt);
                     }
                #else
                    newRegValue = capsense_DEFAULT_SENSE_PERIOD;
                #endif /* (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) */

                /* Update reg value with divider */
                newRegValue |= (snsClkDivider - 1u);
                
                /*  Set PRS size and sense clock divider */
                CY_SET_REG32(capsense_SENSE_PERIOD_PTR, newRegValue);
        #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
            }
        #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */

    #else

        /* Code below for CSDv1 HW block */

        #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
            (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE))
            capsense_RAM_WD_BASE_STRUCT const *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
            (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)) */

        #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
            /* Use direct clock if pre-scalers have not been tuned */
            if (capsense_ENABLE == capsense_prescalersTuningDone)
            {
        #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */                
                #if (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)
                    
                    capsense_configCsd &= ~(capsense_PRS_MODE_MASK);
                    capsense_configCsd |= capsense_CONFIG_PRS_SELECT_MASK;

                    #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
                        senseClkDivMath = (uint32)capsense_dsRam.snsCsdClk;
                    #elif (capsense_CSD_MATRIX_WIDGET_EN  | capsense_CSD_TOUCHPAD_WIDGET_EN)
                        if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
                        { 
                            senseClkDivMath = (uint32)ptrWdgt->rowSnsClk;    
                        }
                        else
                        {
                            senseClkDivMath = (uint32)ptrWdgt->snsClk;   
                        } 
                    #else
                        senseClkDivMath = (uint32)ptrWdgt->snsClk;
                    #endif /* (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
                    
                    #if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
                         modClkDivMath = (uint32)capsense_dsRam.modCsdClk;
                    #endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */ 

                    #if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
                        if ((senseClkDivMath * capsense_PRS_LENGTH_12_BITS) <
                           (modClkDivMath * (capsense_counterResolution >> capsense_RESOLUTION_OFFSET)))
                        {
                            capsense_configCsd |= capsense_CSD_PRS_12_BIT;
                        }     
                    #else
                        if ((senseClkDivMath * capsense_PRS_LENGTH_12_BITS) <
                            (capsense_counterResolution >> capsense_RESOLUTION_OFFSET))
                        {
                            capsense_configCsd |= capsense_CSD_PRS_12_BIT;
                        }
                    #endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */ 
                #else
                    capsense_configCsd |= capsense_DEFAULT_MODULATION_MODE;  
                #endif /* (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE) */
        #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) 
            }
        #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */

    #endif /* (capsense_ENABLE == capsense_CSDV2) */
    }
#endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */


#if (capsense_ENABLE == capsense_CSDV2)
/*******************************************************************************
* Function Name: capsense_GetNumberOfConversions
****************************************************************************//**
*
* \brief
*  This function gets Number of conversions. 
*
* \details
*  This function gets Number of conversions.
*
* \param
*  ptrWdgt The pointer to the RAM_WD_BASE_STRUCT structure.  
*
* \return Returns the Number of conversions.
*
*******************************************************************************/
static uint32 capsense_GetNumberOfConversions(capsense_RAM_WD_BASE_STRUCT const *ptrWdgt)
{   
    uint32 snsClkDivider;
    uint32 conversionsNum;
    #if (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ)
        uint32 sampleClkFreqHz;
    #endif /* (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ) */
        
    /* Get dividers */
    #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
        snsClkDivider = (uint32)capsense_dsRam.snsCsdClk;
    #elif (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
        if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
        {
            snsClkDivider = (uint32)ptrWdgt->rowSnsClk;
        }
        else
        {
            snsClkDivider = (uint32)ptrWdgt->snsClk;  
        } 
    #else
        snsClkDivider = (uint32)ptrWdgt->snsClk;  
    #endif /*  (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
    
    /* Calculate scanning resolution value in register*/
    if (ptrWdgt->resolution < capsense_RES16BIT)
    {
        conversionsNum = (uint32)((1uL << ptrWdgt->resolution) - 1uL);
    }
    else
    {
        conversionsNum = (uint32)((1uL << ptrWdgt->resolution) - 1uL) - ((snsClkDivider + 1u) / 2uL);
        
        #if (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ)
            sampleClkFreqHz = CYDEV_BCLK__HFCLK__HZ / (uint32)capsense_dsRam.modCsdClk;

            if (sampleClkFreqHz <= capsense_MOD_CSD_CLK_12MHZ)
            {
                conversionsNum -= capsense_CONFIG_FILTER_DELAY_2_CYCLES;
            }
            else if (sampleClkFreqHz <= capsense_MOD_CSD_CLK_24MHZ)
            {
                conversionsNum -= capsense_CONFIG_FILTER_DELAY_3_CYCLES;
            }
            else
            {
                conversionsNum -= capsense_CONFIG_FILTER_DELAY_4_CYCLES;
            }
        #else
            conversionsNum -= capsense_CONFIG_FILTER_DELAY_2_CYCLES;
        #endif /* (INSTANCE_NAME`_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ) */
    }
    
    conversionsNum /= snsClkDivider;
       
    #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
        /* Use direct clock if pre-scalers have not been tuned */
        if (capsense_ENABLE == capsense_prescalersTuningDone)
        {
    #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
        #if ((capsense_CLK_SOURCE_PRS12 == capsense_CSD_SNS_CLK_SOURCE) || \
             (capsense_CLK_SOURCE_PRS8 == capsense_CSD_SNS_CLK_SOURCE))
             /* Divide by 4 for PRS8/PRS12 mode */
             conversionsNum >>= 2u;
        #elif (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)
             snsClkDivider = snsClkDivider * (uint32)capsense_dsRam.modCsdClk;

             if (capsense_HFCLK_SNSCLK_FACTOR >= snsClkDivider)
             {
                 /* Divide by 4 for PRS8/PRS12 mode */
                 conversionsNum >>= 2u;
             }
        #else 
             /* SSC or Direct Clock mode  */
        #endif /* ((capsense_CLK_SOURCE_PRS12 == capsense_CSD_SNS_CLK_SOURCE) || \
                   (capsense_CLK_SOURCE_PRS8 == capsense_CSD_SNS_CLK_SOURCE)) */
    #if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
        }
    #endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
  
    return conversionsNum;
}
#endif /* (capsense_ENABLE == capsense_CSDV2) */


/*******************************************************************************
* Function Name: capsense_CSDSetupWidget
****************************************************************************//**
*
* \brief
*   Performs hardware and firmware initialization required for scanning sensors
*   in a specific widget using the CSD sensing method.
*
* \details
*   This function initializes the widgets specific common parameters to perform  
*   the CSD scanning. This function does not connect any specific sensors to
*   the scanning hardware and also does not start a scanning process.
*
*   This function is called by the capsense_SetupWidget() API if the given 
*   widget uses the CSD sensing method. The capsense_Scan() API must be 
*   called after initializing the widget to start scanning.
*
*   It is not recommended to call this function directly from the application layer.
* 
*   This API does not disconnect the sensor that was connected by 
*   capsense_CSXSetupWidgetExt() API.
*
* \param  wdgtIndex 
*  Specifies the ID of the widget for which the hardware tuning
*  parameters should be initialized in the CSD HW block.
*
*******************************************************************************/
void capsense_CSDSetupWidget(uint32 wdgtIndex)
{
    #if (capsense_ENABLE == capsense_CSDV2)
        uint32 counterResolution;
    #endif /* (capsense_ENABLE == capsense_CSDV2) */

    capsense_RAM_WD_BASE_STRUCT const *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;

    /* Disconnect previous electrode if it has been connected */
    capsense_CSDElectrodeCheck();
   
    /* Save widget Id to have assess to it after scanning  */
    capsense_widgetIndex = (uint8)wdgtIndex;
    
    /* Initialize IO Sns electrode structure pointer to current widget */
    capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                    capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash;   

    /* Update status register in Data structure */
    capsense_dsRam.status &= ~capsense_STATUS_WDGT0_MASK;
    capsense_dsRam.status |= capsense_widgetIndex;

    #if (capsense_ENABLE == capsense_CSDV2)
        /* Code below for CSDv2 HW block */

        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
            /* Connect shield pins to CSD block */
            capsense_SsEnableShieldElectrodes();
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */
        
        /* Save scanning resolution value */
        capsense_curWidgetResolution = ptrWdgt->resolution;
        
        counterResolution = capsense_GetNumberOfConversions(ptrWdgt);

        /* Set scanning resolution */
        CY_SET_REG32(capsense_SEQ_NORM_CNT_PTR, (counterResolution & capsense_SEQ_NORM_CNT_CONV_CNT_MASK));
    #else 
        /* Code below for CSDv1 HW block */
        #if ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_DISABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
            /* Configure PRS mode */
            capsense_CSDConfigurePrsMode();
        #endif /* ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_DISABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */

        /*  Set up scanning resolution  */
        capsense_counterResolution = (uint32)((0x00000001Lu << ptrWdgt->resolution) - 1u) << capsense_RESOLUTION_OFFSET;    
    #endif /* (capsense_ENABLE == capsense_CSDV2) */
}


/*******************************************************************************
* Function Name: capsense_CSDSetupWidgetExt
****************************************************************************//**
*
* \brief
*   Performs extended initialization for widget scanning. This API initializes
*   the widget and also connects a particular sensor in the widget to the scanning
*   hardware.
*
* \details
*   This function does the same tasks as capsense_SetupWidget() and 
*   also connects a sensor in the widget for scanning.
*
*   Once this function is called to initialize a widget and a sensor,
*   the capsense_CSXScanExt() function should be called to scan the sensor.
*
*   This API does not disconnect the sensor that was configured by 
*   capsense_CSXSetupWidgetExt() API. The following tasks should be 
*   performed before calling this API if CSX widget was configured before: 
*   1. Disconnects previous CSX electrodes
*   2. Disconnects CintA and CintB from AMUXBUS-A using HSIOM registers
*   3. Initializes the HW block.
*
* \param  wdgtIndex
*   Specifies the ID of the widget for which the hardware tuning
*   parameters should be initialized in the capsense CSD HW block.
* \param snsIndex 
*   Indicates the sensor which should be connected to the CapSense block.
*
*******************************************************************************/
void capsense_CSDSetupWidgetExt(uint32 wdgtIndex, uint32 snsIndex)
{
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        uint32 tempVal; 
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    capsense_RAM_WD_BASE_STRUCT const *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;
                                       
    /* Save widget and sensor Ids to have access to it after scanning  */
    capsense_sensorIndex = (uint8)snsIndex;

    /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                              capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsRam
                                              + capsense_sensorIndex;

    #if (capsense_ENABLE == capsense_CSD_CSX_EN)
        /*  
         * Check if CSD block is  already initialized for CSD operation
         * If not, initialize CSD hardware block for CSD measurement   
         */
        if (capsense_SENSE_METHOD_CSD_E != capsense_currentSenseMethod)
        {
            capsense_SwitchSensingMode(capsense_SENSE_METHOD_CSD_E);
        }
    #endif /* (capsense_ENABLE == capsense_CSD_CSX_EN) */
    
    capsense_CSDSetupWidget(wdgtIndex);
    
    #if (capsense_ENABLE == capsense_CSDV2)
        /* Code below for CSDv2 HW block */
    
        #if (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN)
             #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
                #if (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))
                    /*  Get SnsClck divider */
                    if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
                    {
                        /*  Set sense clock divider  */
                        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->rowSnsClk - 1u));
                    }
                    else
                    {
                        /*  Set sense clock divider  */
                        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
                    }
                #else
                    /*  Set sense clock divider  */
                    CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
                #endif /*  (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)) */
             #else   
                /* Configure PRS mode */
                capsense_CSDConfigurePrsMode();
             #endif /* (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE) */
        #endif /* (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
    #else 
        /* Code below for CSDv1 HW block */
        #if ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
            /* Configure PRS mode */
            capsense_CSDConfigurePrsMode();
        #endif /* ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */
    #endif /* (capsense_ENABLE == capsense_CSDV2) */
    
    /* Setup Idac Value */
   capsense_SetUpIdacs(ptrWdgt);

    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        /*  Initialise access pointers for current pointer to widget configuration in Flash */
        capsense_curFlashWdgtPtr = &capsense_dsFlash.wdgtArray[wdgtIndex];
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        /* Check ganged sns flag  */
        if (capsense_GANGED_SNS_MASK == (capsense_curFlashWdgtPtr->staticConfig & 
            capsense_GANGED_SNS_MASK))
        {         
            /* Get sns pointer */     
            capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                               capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash + 
                                               capsense_sensorIndex;
        
            /* Get number of ganged pins */
            tempVal = capsense_curFlashSnsPtr->numPins;

            /* Get IO pointer  */
            capsense_curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];
        
            /* Connect all ganged sensors  */
            do
            {
                capsense_CSDConnectSns(capsense_curSnsIOPtr);
                capsense_curSnsIOPtr++;
                tempVal--;
            } while (0u != tempVal);
        }
        else
        {
            /* Enable sensor */
            capsense_curSnsIOPtr += capsense_sensorIndex;
            capsense_CSDConnectSns(capsense_curSnsIOPtr);
        }
    #else
        /* Enable sensor */
        capsense_curSnsIOPtr += capsense_sensorIndex;
        capsense_CSDConnectSns(capsense_curSnsIOPtr);
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    capsense_eleCsdDisconnectFlag = capsense_DISCONNECT_IO_FLAG;
        
    /*  Setup ISR handler to single-sensor scan function */
    capsense_ISR_StartEx(&capsense_CSDPostSingleScan);
}


/*******************************************************************************
* Function Name: capsense_CSDStartSample
****************************************************************************//**
*
* \brief
*   Starts the CSD conversion.
*
* \details
*   This function assumes that the CSD block is already set up using 
*   the capsense_CSDSetupWidget API and the sensor port-pin is connected to the CSD 
*   block using capsense_CSDConnectSns.
*   The function performs the following tasks:
*   1. Disconnects the Vref buffer from AMUX;
*   2. Makes the PreSettling delay to have a stable Vref voltage;
*   3. Sets the resolution to the Counter register to start scanning.
*
*   This function has two Macro Callbacks that allow to call user code from macros 
*   specified in a component's generated code. Refer to Macro Callbacks section 
*   of PSoC Creator™ User Guide for details.
*
*******************************************************************************/
void capsense_CSDStartSample(void)
{
#if (capsense_ENABLE == capsense_CSDV2)
    /* CSDv2 HW IP block section */

    uint32 watchdogCounter;
    #if (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ)
        uint32 sampleClkFreqHz;
    #endif /* (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ) */

    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && (0u != capsense_CSD_SHIELD_TANK_EN))
        uint32 tmpRegValue;
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && (0u != capsense_CSD_SHIELD_TANK_EN)) */

    CyIntDisable(capsense_ISR_NUMBER);

    capsense_configCsd &= ~capsense_CONFIG_FILTER_DELAY_MASK;
    
    #if (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ)
        sampleClkFreqHz = CYDEV_BCLK__HFCLK__HZ / (uint32)capsense_dsRam.modCsdClk;
        if(sampleClkFreqHz <= capsense_MOD_CSD_CLK_12MHZ)
        {
            capsense_configCsd |= capsense_CONFIG_FILTER_DELAY_12MHZ;
        }
        else if(sampleClkFreqHz <= capsense_MOD_CSD_CLK_24MHZ)
        {
            capsense_configCsd |= capsense_CONFIG_FILTER_DELAY_24MHZ;
        }
        else
        {
            capsense_configCsd |= capsense_CONFIG_FILTER_DELAY_48MHZ;
        }
    #else
        capsense_configCsd |= capsense_CONFIG_FILTER_DELAY_12MHZ;   
    #endif /* (capsense_MOD_CSD_CLK_12MHZ < CYDEV_BCLK__HFCLK__HZ) */

    /* Enable power to sub-blocks */
    CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | 
                                              capsense_CONFIG_SENSE_EN_MASK |
                                              capsense_CONFIG_ENABLE_MASK);

    #if (capsense_ENABLE == capsense_CSD_AUTO_ZERO_EN)
        CY_SET_REG32(capsense_HSCMP_PTR, capsense_HSCMP_EN_MASK | 
                                                 capsense_CSD_AZ_EN_MASK);
    #else
        CY_SET_REG32(capsense_HSCMP_PTR, capsense_HSCMP_EN_MASK);
    #endif /* ((capsense_ENABLE == capsense_CSD_AUTO_ZERO_EN) */

    /*  Wait until CSD block powers ON */ 
    CyDelayCycles(capsense_CSD_ON_DELAY);
    
    /******************************
    * Coarse initialization for CMOD Section 
    * The coarse initialization is performed by HSCOMP. 
    * The HSCOMP monitors the Cmod voltage via Cmod sense path
    * and charges the Cmod using HCAV switch via CSDBUS-A, AMUXBUS-A 
    * and static connection of Cmod to AMUXBUS-A.
    *********************************/
    
    CY_SET_REG32(capsense_SW_FW_MOD_SEL_PTR,  capsense_CSD_SW_FW_MOD_SEL_INIT);
    CY_SET_REG32(capsense_SW_FW_TANK_SEL_PTR, capsense_CSD_SW_FW_TANK_SEL_INIT);
    CY_SET_REG32(capsense_SW_SHIELD_SEL_PTR,  capsense_SW_SHIELD_SEL_SW_HCAV_HSCMP);
    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
        /* 
         * Connect CMOD to (sense path) to HSCOMP: HMPM or HMPS or HMPT switches depend on Cmod connects to certain pad 
         * Connect AMUXBUS-B to HSCMP positive input 
         */
        CY_SET_REG32(capsense_SW_HS_P_SEL_PTR, capsense_SW_HS_P_SEL_COARSE);
        CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE);
        
        #if(0u != capsense_CSD_SHIELD_TANK_EN)
            /* Connect CTANK to AMUXBUS-A */
            tmpRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR); 
            tmpRegValue &= (uint32)(~(uint32)capsense_CSH_HSIOM_MASK);
            tmpRegValue |= (capsense_HSIOM_SEL_AMUXA << capsense_CSH_HSIOM_SHIFT);
            CY_SET_REG32(capsense_CSH_HSIOM_PTR, tmpRegValue);
        #endif /* (0u != capsense_CSD_SHIELD_TANK_EN) */
    #else
        /* Connect CMOD to (sense path) to HSCOMP: HMPM or HMPS or HMPT switches depend on Cmod connects to certain pad */
        CY_SET_REG32(capsense_SW_HS_P_SEL_PTR, capsense_SW_HS_P_SEL_COARSE);
        CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE);
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
    
    CY_SET_REG32(capsense_SW_RES_PTR, capsense_CSD_SW_RES_INIT);

    /* Clear all interrupt pending requests */
    CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_SAMPLE_MASK |
                                            capsense_INTR_SET_INIT_MASK |
                                            capsense_INTR_SET_ADC_RES_MASK);
                   
    /* Enable INIT interrupt */
    CY_SET_REG32(capsense_INTR_MASK_PTR, capsense_INTR_MASK_INIT_MASK);

    /* Start SEQUENCER for coarse initialization for Cmod */
    CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_SEQ_MODE_MASK |
                                                 capsense_SEQ_START_START_MASK);
                                 
    /* Init Watchdog Counter to prevent a hang */ 
    watchdogCounter = capsense_SOFTWARE_WDT_CYCLES_NUM;

    /* Wait for INTR.INIT goes high */ 
    while((0u == (CY_GET_REG32(capsense_INTR_PTR) & capsense_INTR_INIT_MASK)) && (0u != watchdogCounter))
    {
        watchdogCounter--;
    } 
    
    if (0u == watchdogCounter)
    {
        /* Set sequencer to idle state if coarse initialization fails */  
        CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_ABORT_MASK);
    }

    
    CY_SET_REG32(capsense_SW_FW_MOD_SEL_PTR, capsense_CSD_SW_FW_MOD_SEL_SCAN);
    CY_SET_REG32(capsense_SW_FW_TANK_SEL_PTR,capsense_CSD_SW_FW_TANK_SEL_SCAN);

    
    /******************************
    * Fine initialization for CMOD and Start scan Section 
    * For the fine initialization and sampling, Cmod is static connected to AMUXBUS-A 
    * and in every conversion (one cycle of SnsClk), the sensor capacitance is charged 
    * from Cmod and discharged to ground using the switches in GPIO cell. 
    * The CSDCOMP monitors voltage on Cmod using the sense path and charges Cmod back 
    * to Vref using IDACs by connecting IDAC to CSDBUS-A and then the AMUXBUS-A.
    *********************************/

    /* Clear previous interrupts */
    CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_SAMPLE_MASK |
                                            capsense_INTR_SET_INIT_MASK |
                                            capsense_INTR_SET_ADC_RES_MASK);
                                            
    /* Clear pending interrupts  */
    CyIntClearPending(capsense_ISR_NUMBER);

    /* Enable CSD interrupt */
    CyIntEnable(capsense_ISR_NUMBER);
    
    #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
        CY_SET_REG32(capsense_SW_SHIELD_SEL_PTR,    capsense_DEFAULT_SW_SHIELD_SEL);
        CY_SET_REG32(capsense_SW_HS_P_SEL_PTR, capsense_SW_HS_P_SEL_SCAN);
        #if(0u != capsense_CSD_SHIELD_TANK_EN)
            CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE);
            
            /* Connect CTANK to AMUXBUS-B */
            tmpRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR); 
            tmpRegValue &= (uint32)(~(uint32)capsense_CSH_HSIOM_MASK);
            tmpRegValue |= (capsense_HSIOM_SEL_AMUXB << capsense_CSH_HSIOM_SHIFT);
            CY_SET_REG32(capsense_CSH_HSIOM_PTR, tmpRegValue);
        #else
            CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_SW_HS_N_SEL_SW_HCRL_STATIC_CLOSE);
        #endif /* (0u != capsense_CSD_SHIELD_TANK_EN) */
    #else
        CY_SET_REG32(capsense_SW_SHIELD_SEL_PTR,    0x00000000uL);
        CY_SET_REG32(capsense_SW_HS_P_SEL_PTR, capsense_STATIC_OPEN);
        CY_SET_REG32(capsense_SW_HS_N_SEL_PTR, capsense_STATIC_OPEN);
    #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
    
    CY_SET_REG32(capsense_SW_RES_PTR, capsense_CSD_SW_RES_SCAN);

    #if (capsense_ENABLE == capsense_CSD_AUTO_ZERO_EN)
        /* Enable CSDCMP */ 
        CY_SET_REG32(capsense_CSDCMP_PTR, capsense_CSDCMP_CSDCMP_EN_MASK | 
                                                  capsense_CSDCMP_AZ_EN_MASK);
    #else
        /* Enable CSDCMP */ 
        CY_SET_REG32(capsense_CSDCMP_PTR, capsense_CSDCMP_CSDCMP_EN_MASK);
    #endif /* (capsense_ENABLE == capsense_CSD_AUTO_ZERO_EN) */

    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
         (capsense_2000_MV <= capsense_CYDEV_VDDA_MV))
        /*  Connect Vrefhi to AMUBUF positive input. Connect AMUBUF output to CSDBUSB with sych PHI2  */
        CY_SET_REG32(capsense_SW_AMUXBUF_SEL_PTR, capsense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE | 
                                                          capsense_SW_AMUXBUF_SEL_SW_ICB_PHI2);
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
               (capsense_2000_MV <= capsense_CYDEV_VDDA_MV)) */         

    #if (capsense_DISABLE == capsense_CSD_SHIELD_EN)
        /* Disable HSCOMP during the sampling phase when shield is disabled */
        CY_SET_REG32(capsense_HSCMP_PTR, 0u);
    #endif /* (capsense_DISABLE == capsense_CSD_SHIELD_EN) */
    
    #if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
        /*  Force the LFSR to it's initial state (all ones) */
        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, CY_GET_REG32(capsense_SENSE_PERIOD_PTR) |
                                                       capsense_SENSE_PERIOD_LFSR_CLEAR_MASK);
    #endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */

    /* Enable SAMPLE interrupt */
    CY_SET_REG32(capsense_INTR_MASK_PTR, capsense_INTR_MASK_SAMPLE_MASK);

    /* Init Watchdog Counter to prevent a hang */ 
    watchdogCounter = capsense_SOFTWARE_WDT_CYCLES_NUM;

    /* Wait for IDLE state of the CSDv2 HW sequencer */ 
    while((0u != (CY_GET_REG32(capsense_STAT_SEQ_PTR) & capsense_STAT_SEQ_SEQ_STATE_MASK)) && (0u != watchdogCounter))
    {
        watchdogCounter--;
    }
    
    /* Start SEQUENCER for fine initialization scan for Cmod and then for normal scan */
    CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_AZ0_SKIP_MASK |
                                                 capsense_SEQ_START_AZ1_SKIP_MASK |
                                                 capsense_SEQ_START_START_MASK);

#else
    /* CSDv1 HW IP block section */

    uint8 interruptState;

    interruptState = CyEnterCriticalSection();

    capsense_SsCmodPrecharge();

    #if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
        /* PreSettling delay */
        CyDelayCycles(capsense_GLITCH_ELIMINATION_CYCLES);
    #endif /* (capsense_CSD_SNS_CLK_SOURCE != capsense_CLK_SOURCE_DIRECT) */
  
    /* Set resolution to Counter register to start scanning */
    CY_SET_REG32(capsense_COUNTER_PTR, capsense_counterResolution);

    CyExitCriticalSection(interruptState);
#endif /* (capsense_ENABLE == capsense_CSDV2) */
}


/*******************************************************************************
* Function Name: capsense_CSDScanExt
****************************************************************************//**
*
* \brief
*   Starts the CSD conversion on the preconfigured sensor.
*
* \details
*   This function assumes that the CSD block is already set up using 
*   the capsense_CSDSetupWidgetExt() API and the sensor pin is connected to  
*   the CSD block using capsense_CSDConnectSns(). If this API is used,  
*   the sensor port is not disconnected from the CSD block at the end of scan.
*
*   The sensor pin is not disconnected from CSD block after calling this API.
*   capsense_CSDDisconnectSns() API can be called to disconnect it.
*   This API should be used together with the capsense_CSDSetupWidgetExt() 
*   API. 
*
*******************************************************************************/
void capsense_CSDScanExt(void)
{
    /* Set Start of sensor scan flag */
    capsense_dsRam.status |= (capsense_SW_STS_BUSY | capsense_WDGT_SW_STS_BUSY);

    capsense_CSDStartSample();
}


/*******************************************************************************
* Function Name: capsense_CSDScan
****************************************************************************//**
*
* \brief
*   This API initiates the scan for the last sensor of the widget initialized by 
*   the capsense_SetupWidget() API and also sets ISR to scan all sensors in 
*   the widget.
*
* \details
*   This function assumes that the CSD block is already set up using 
*   the capsense_CSDSetupWidget() API. If this API is used, the sensor 
*   port is disconnected from the CSD block at the end of scan.
*   Depending on the Ganged sensors availably, the function initialises different 
*   pointers to the ISR handlers: CSDPostMultiScanGanged or CSDPostMultiScan.
*   The other sensors in the widget are configured in the ISR handlers. 
*
*******************************************************************************/
void capsense_CSDScan(void)
{
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        uint32 tempVal; 
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
    
    capsense_RAM_WD_BASE_STRUCT const *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;

     /* 
      * Update capsense_sensorIndex with last sensor in 
      * widget to use it in ISR handler to configure next sensor  
      */
    capsense_sensorIndex = (uint8)capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].totalNumSns - 1u;
    
    /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                              capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                              + capsense_sensorIndex;
    
    #if (capsense_ENABLE == capsense_CSDV2)
        /* Code below for CSDv2 HW block */

        #if (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN)
             #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
                #if (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))
                    /*  Get SnsClck divider */
                    if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
                    {
                        /*  Set sense clock divider  */
                        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->rowSnsClk - 1u));
                    }
                    else
                    {
                        /*  Set sense clock divider  */
                        CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
                    }
                #else
                    /*  Set sense clock divider  */
                    CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
                #endif /*  (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)) */
             #else   
                /* Configure PRS mode */
                capsense_CSDConfigurePrsMode();
             #endif /* (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE) */
        #endif /* (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
    #else 
        /* Code below for CSDv1 HW block */
        #if ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
            /* Configure PRS mode */
            capsense_CSDConfigurePrsMode();
        #endif /* ((capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) && \
             (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
             (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */
    #endif /* (capsense_ENABLE == capsense_CSDV2) */
    
    /* Setup Idac Value */
   capsense_SetUpIdacs(ptrWdgt);
    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
        /* Check ganged sns flag  */
        if (capsense_GANGED_SNS_MASK == (capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig & 
            capsense_GANGED_SNS_MASK))
        { 
            /* Get sns pointer */     
            capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                               capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash + 
                                               capsense_sensorIndex;
        
            /* Get number of ganged pins */
            tempVal = capsense_curFlashSnsPtr->numPins;

            /* Get IO pointer  */
            capsense_curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];
        
            /* Connect all ganged sensors  */
            do
            {
                capsense_CSDConnectSns(capsense_curSnsIOPtr);
                capsense_curSnsIOPtr++;
                tempVal--;
            } while (0u != tempVal);

            /*  Setup ISR handler to multiple sensor scan function with ganged sensors */
            capsense_ISR_StartEx(&capsense_CSDPostMultiScanGanged);
        }
        else
        {
            /* Initialise ptr to sensor IO structure to appropriate address */        
            capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                            + capsense_sensorIndex;
            
            /* Enable sensor */
            capsense_CSDConnectSns(capsense_curSnsIOPtr);

            /*  Set up ISR handler to multiple sensor scan function without ganged sensors */
            capsense_ISR_StartEx(&capsense_CSDPostMultiScan);
        }
    #else
        /* Initialise ptr to sensor IO structure to appropriate address */        
        capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                        capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                        + capsense_sensorIndex;
    
        /* Enable sensor */
        capsense_CSDConnectSns(capsense_curSnsIOPtr);

        /*  Set-up ISR handler to multiple sensor scan function without ganged sensors */
        capsense_ISR_StartEx(&capsense_CSDPostMultiScan);
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    /* Start scanning */
    capsense_CSDScanExt();
}


/*******************************************************************************
* Function Name: capsense_CSDConnectSns
****************************************************************************//**
*
* \brief
*  Connects a sensor port-pin to the CapSense block via the AMUX bus.
*
* \details
*   The function gets the IO configuration registers addresses, their shifts and 
*   masks from the FLASH_IO_STRUCT object. Basing on this data, it updates the HSIOM and 
*   PC registers.
*
* \param  snsAddrPtr
*   Indicates the pointer to the FLASH_IO_STRUCT object belonging
*   to a sensor which should be connected to the CapSense block.
*
*******************************************************************************/
void capsense_CSDConnectSns(capsense_FLASH_IO_STRUCT const *snsAddrPtr)
{
    uint32 newRegisterValue;
    uint8  interruptState;
    uint32 pinModeShift;
    uint32 pinHSIOMShift;
    
    /* Get offsets for sensor */
    pinModeShift = (uint32)snsAddrPtr->shift;
    pinHSIOMShift = (uint32)snsAddrPtr->hsiomShift;

    interruptState = CyEnterCriticalSection();
    
    /* Use temporary variable to update registers without multiple writings to them */
    newRegisterValue = CY_GET_REG32(snsAddrPtr->hsiomPtr);
    newRegisterValue &= ~(capsense_HSIOM_SEL_MASK << pinHSIOMShift);
    newRegisterValue |= (uint32)((uint32)capsense_HSIOM_SEL_CSD_SENSE << pinHSIOMShift);
    
    /* Update port configuration register (drive mode) for sensor */
    CY_SET_REG32(snsAddrPtr->pcPtr, CY_GET_REG32(snsAddrPtr->pcPtr) & (uint32)~((uint32)capsense_GPIO_PC_MASK << pinModeShift));

    /* Update HSIOM register for sensor */
    CY_SET_REG32(snsAddrPtr->hsiomPtr, newRegisterValue);
    
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: capsense_CSDDisconnectSns
****************************************************************************//**
*
* \brief
*  Disconnects a sensor port-pin from the CapSense block and AMUX bus. Sets the
*   default state of the un-scanned sensor.
*
* \details
*   The function gets the IO configuration registers addresses, their shifts and 
*   masks from the FLASH_IO_STRUCT object. Basing on this data and Inactive sensor  
*   connection parameter, it updates the HSIOM, PC and DR registers. 
*   The HSIOM register is updated only when the Inactive sensor connection parameter
*   is set to Shield.
*
* \param  snsAddrPtr
*   Indicates the pointer to the FLASH_IO_STRUCT object belonging
*   to a sensor which should be disconnected to the CapSense block.
*
*******************************************************************************/
void capsense_CSDDisconnectSns(capsense_FLASH_IO_STRUCT const *snsAddrPtr)
{
    uint32 pinHSIOMShift;
    uint32 newRegisterValue;
    uint32 pinModeShift;

    #if ((capsense_DISABLE == capsense_CSD_SHIELD_EN) || \
        (capsense_SNS_CONNECTION_SHIELD != capsense_CSD_INACTIVE_SNS_CONNECTION))
        uint8  interruptState;
    #endif /* ((capsense_DISABLE == capsense_CSD_SHIELD_EN) && \
                (capsense_SNS_CONNECTION_SHIELD != capsense_CSD_INACTIVE_SNS_CONNECTION))) */
    
    pinHSIOMShift = (uint32)snsAddrPtr->hsiomShift;
    pinModeShift = (uint32)snsAddrPtr->shift;
        
    #if ((capsense_DISABLE == capsense_CSD_SHIELD_EN) ||  \
        (capsense_SNS_CONNECTION_SHIELD != capsense_CSD_INACTIVE_SNS_CONNECTION))
        /* Update HSIOM register to disconnect sensor */
        CY_SET_REG32(snsAddrPtr->hsiomPtr, CY_GET_REG32(snsAddrPtr->hsiomPtr) & ~(capsense_HSIOM_SEL_MASK << pinHSIOMShift));
        
        interruptState = CyEnterCriticalSection();

        /* Update port configuration register (drive mode) for sensor */
        newRegisterValue = CY_GET_REG32(snsAddrPtr->pcPtr);
        newRegisterValue &= ~(capsense_GPIO_PC_MASK << pinModeShift);
        newRegisterValue |= capsense_CSD_INACTIVE_SNS_CONNECTION << pinModeShift;
        CY_SET_REG32(snsAddrPtr->pcPtr, newRegisterValue);

        CyExitCriticalSection(interruptState);

        /* Set logic 0 to port data register */
        CY_SET_REG32(snsAddrPtr->drPtr, CY_GET_REG32(snsAddrPtr->drPtr) & (uint32)~(uint32)((uint32)1u << snsAddrPtr->drShift));
    #else
        /* Update port configuration register (drive mode to HiZ Analog) for sensor */
        CY_SET_REG32(snsAddrPtr->pcPtr, CY_GET_REG32(snsAddrPtr->pcPtr) & (uint32)~((uint32)capsense_GPIO_PC_MASK << pinModeShift));

        /* Connect to Shield */
        newRegisterValue = CY_GET_REG32(snsAddrPtr->hsiomPtr) & ~(capsense_HSIOM_SEL_MASK << pinHSIOMShift);
        newRegisterValue |= (capsense_HSIOM_SEL_CSD_SHIELD << pinHSIOMShift);
        CY_SET_REG32(snsAddrPtr->hsiomPtr, newRegisterValue);
    #endif /* ((capsense_DISABLE == capsense_CSD_SHIELD_EN) && \
                (capsense_SNS_CONNECTION_SHIELD != capsense_CSD_INACTIVE_SNS_CONNECTION)) */
}


#if ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) || \
     (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))
/*******************************************************************************
* Function Name: capsense_SsCSDCalibrateCheck
****************************************************************************//**
*
* \brief
*  This internal function checks if the calibration is performed successfully.
*
* \details
*   The function shifts the rawLevel to 16 to implement the integer mathematics.
* 
* \param rawLevel The raw data level which should be reached during the 
*   calibration procedure.
* \param rawData  Calibrated raw counts.
*
* \return Returns the status of operation: 
*   - Zero     - All the sensors in the widget are calibrated successfully.
*   - Non-Zero - Calibration failed for any sensor in the widget.
*
*******************************************************************************/
static cystatus capsense_SsCSDCalibrateCheck(uint32 rawLevel, uint32 rawData)
{
    cystatus calibrateStatus;
    uint32 minRawLevel;
    uint32 maxRawLevel;

    /* Check if calibration is in valid range */
    minRawLevel = (uint32)((uint32)rawLevel * capsense_IDAC_MOD_MIN_CALIB_ERROR_MULT);
    maxRawLevel = (uint32)((uint32)rawLevel * capsense_IDAC_MOD_MAX_CALIB_ERROR_MULT);

    if (((rawData << capsense_IDAC_MOD_MAX_INT_MATH_SHIFT) < minRawLevel) || 
        ((rawData << capsense_IDAC_MOD_MAX_INT_MATH_SHIFT) > maxRawLevel))
    {
        calibrateStatus = CYRET_BAD_DATA;
    }
    else
    {
        calibrateStatus = CYRET_SUCCESS;
    }

    return (calibrateStatus);
}


/*******************************************************************************
* Function Name: capsense_SsCSDCalibrateOneSensor
****************************************************************************//**
*
* \brief
*  Computes the Modulator IDAC value for one sensor that provides raw signal at
*  the specified level.
*
* \details
*   The function performs the following tasks:
*   1. Initialises The Compensation IDAC to zero
*   2. Enables the calibrated sensor
*   3. Performs eight iteration steps to get the target = rawLevel
*      setting and resetting the IDAC value bits beginning from the
*      MSB bit to an MSB bit.
*   4. Disables the calibrated sensor
*   5. Checks if the IDAC is in the correct range
*   6. Updates the maximum IDAC value in the widget
*   7. Stores the temporary ModIDAC8 value (in the single-IDAC mode).
*
*  The CSD block should be initialized with the capsense_CSDInitialize()
*  API before using this function.
*
* \param  rawLevel
*   The raw data level which should be reached during the calibration procedure.
* \param  ptrIdac  The pointer to IDAC.
* \param  iDAC8Max The pointer to the maximum IDAC value in the widget.
* \return Returns the status of operation: 
*  - Zero     - All the sensors in the widget are calibrated successfully.
*  - Non-Zero - Calibration failed for any sensor in the widget.
*
*******************************************************************************/
static cystatus capsense_SsCSDCalibrateOneSensor(uint32 rawLevel,
                                                       uint8 *ptrIdac, 
                                                       uint8 *iDAC8Max)
{
    cystatus calibrateStatus;
    uint32 rawData0;
    uint8 calMask = capsense_CAL_MIDDLE_BIT;
    uint32 watchdogCounter;
    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        uint32 rawData1;
        uint32 rawData2;
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

    /*  Declare and initialise ptr to widget and sensor structures to appropriate address        */
    capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                    capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        uint32 tempVal;
    
        /* Get sns pointer */     
        capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                           capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash + 
                                           capsense_sensorIndex;
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */


    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                                capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                                + capsense_sensorIndex; 

    /* Setup CSD widget */
    capsense_CSDSetupWidget((uint32)capsense_widgetIndex);  
       
    #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
         (capsense_ENABLE == capsense_CSDV2))
         #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
            #if (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))
                /*  Get SnsClck divider */
                if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
                {
                    /*  Set sense clock divider  */
                    CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->rowSnsClk - 1u));
                }
                else
                {
                    /*  Set sense clock divider  */
                    CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
                }
            #else
                /*  Set sense clock divider  */
                CY_SET_REG32(capsense_SENSE_PERIOD_PTR, capsense_DEFAULT_SENSE_PERIOD | (uint32)(ptrWdgt->snsClk - 1u));
            #endif /*  (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)) */
         #elif (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
               /* Configure PRS mode */
              capsense_CSDConfigurePrsMode();
         #else   
               /* Do not configure PRS for Direct mode */
         #endif /* (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE) */
    #endif /* (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) */
                                        
    /* Init compensation IDAC */
    ptrIdac[0u] = 0x00u;
    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        ptrIdac[capsense_FREQ_CHANNEL_1] = 0x00u;
        ptrIdac[capsense_FREQ_CHANNEL_2] = 0x00u;
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
    
    /* Connect calibrated sensor in widget */
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)        
        /* Check ganged sns flag  */
        if (capsense_GANGED_SNS_MASK == (capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig & 
            capsense_GANGED_SNS_MASK))
        { 
            /* Get IO pointer  */
            capsense_curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

            /* Get number of ganged pins */
            tempVal = capsense_curFlashSnsPtr->numPins;
            
            /* Connect all ganged sensors  */
            do
            {
                capsense_CSDConnectSns(capsense_curSnsIOPtr);
                capsense_curSnsIOPtr++;
                tempVal--;
            } while (0u != tempVal);
        }
        else
        {
            /*  Initialise pointer to sensor IO structure    */     
            capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                            + capsense_sensorIndex;
            
            /* Enable sensor */
            capsense_CSDConnectSns(capsense_curSnsIOPtr);
        }
    #else
        /*  Initialise pointer to sensor IO structure    */     
        capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                        capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                        + capsense_sensorIndex;
    
        /* Enable sensor */
        capsense_CSDConnectSns(capsense_curSnsIOPtr);
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */
    
    /* Perform eight iteration steps to get target = rawLevel */
    do
    {
        /* Set bit for null channel */
        ptrIdac[0u] |= calMask;
        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            ptrIdac[capsense_FREQ_CHANNEL_1] |= calMask;
            ptrIdac[capsense_FREQ_CHANNEL_2] |= calMask;     
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        
        /* Update IDAC */
        capsense_SetUpIdacs(ptrWdgt);

        /* Scan sensor */
        capsense_CSDScanExt();

        /* Initialize Watchdog Counter with time interval which is enough to charge 100 nF capacitor */
        watchdogCounter = capsense_CALIBR_WATCHDOG_CYCLES_NUM;
        while (((*(volatile uint8 *)&capsense_dsRam.status & capsense_SW_STS_BUSY) != 0u) && 
               (0u != watchdogCounter))
        {
            /* Wait until scan complete and decrement Watchdog Counter to prevent unending loop */
            watchdogCounter--;
        }

        /* Read rawData */
        rawData0 = capsense_curRamSnsPtr->raw[0u];
        #if(capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            rawData1 = capsense_curRamSnsPtr->raw[capsense_FREQ_CHANNEL_1];
            rawData2 = capsense_curRamSnsPtr->raw[capsense_FREQ_CHANNEL_2];       
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

        /* Decrease IDAC until rawData reaches rawLevel */ 
        if (rawData0 < rawLevel)
        {
            /* Reset bit for null channel  */ 
            ptrIdac[0u] &= (uint8)(~calMask);
        }
        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            if (rawData1 < rawLevel)
            {
                /* Reset bit for null channel  */ 
                ptrIdac[capsense_FREQ_CHANNEL_1] &= (uint8)(~calMask);
            }
            if (rawData2 < rawLevel)
            {
                /* Reset bit for null channel  */ 
                ptrIdac[capsense_FREQ_CHANNEL_2] &= (uint8)(~calMask);
            }
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

        /* Shift calMask to set or reset next bit */
        calMask >>= 1u;
    }
    while (calMask > 0u);
    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        /* Check ganged sns flag  */
        if (capsense_GANGED_SNS_MASK == (capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig & 
            capsense_GANGED_SNS_MASK))
        {
            /* Get IO pointer  */
            capsense_curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

            /* Get number of ganged pins */
            tempVal = capsense_curFlashSnsPtr->numPins;
            
            /* Disconnect all ganged sensors */
            do
            {
                capsense_CSDDisconnectSns(capsense_curSnsIOPtr);
                capsense_curSnsIOPtr++;
                tempVal--;
            } while (0u != tempVal);
        }
        else
        {
            /*  Initialise pointer to sensor IO structure    */     
            capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                            + capsense_sensorIndex;
            
            /* Disable sensor */
            capsense_CSDDisconnectSns(capsense_curSnsIOPtr);
        }
    #else
        /*  Initialise pointer to sensor IO structure    */     
        capsense_curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                        capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                        + capsense_sensorIndex;
    
        /* Disable sensor */
        capsense_CSDDisconnectSns(capsense_curSnsIOPtr);   
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */

    /* Check if IDAC is in correct range */
    calibrateStatus = capsense_SsCSDCalibrateCheck(rawLevel, rawData0);
    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        calibrateStatus |= capsense_SsCSDCalibrateCheck(rawLevel, rawData1);
        calibrateStatus |= capsense_SsCSDCalibrateCheck(rawLevel, rawData2);
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
 
    /* 
     * Update maximum IDAC value in widget. iDAC8Max variable will contain 
     * maximum IDAC value after all sensors in widget have been scanned.  
     */
    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        if (iDAC8Max[capsense_FREQ_CHANNEL_0] < ptrIdac[capsense_FREQ_CHANNEL_0])
        {
            iDAC8Max[capsense_FREQ_CHANNEL_0] = ptrIdac[capsense_FREQ_CHANNEL_0];
        }   
        if (iDAC8Max[capsense_FREQ_CHANNEL_1] < ptrIdac[capsense_FREQ_CHANNEL_1])
        {
            iDAC8Max[capsense_FREQ_CHANNEL_1] = ptrIdac[capsense_FREQ_CHANNEL_1];
        }
        if (iDAC8Max[capsense_FREQ_CHANNEL_2] < ptrIdac[capsense_FREQ_CHANNEL_2])
        {
            iDAC8Max[capsense_FREQ_CHANNEL_2] = ptrIdac[capsense_FREQ_CHANNEL_2];
        }
    #else
        if (*iDAC8Max < ptrIdac[0u])
        {
            *iDAC8Max = ptrIdac[0u];
        }
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
    
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        /* 
         * Store temporary ModIDAC8 value (in single-IDAC mode) in idacComp variable.
         * We need this value to calculate idacMod value the in the dual-IDAC mode.  
         */
        capsense_curRamSnsPtr->idacComp[0u] = ptrIdac[0u];
        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] = ptrIdac[capsense_FREQ_CHANNEL_1];
            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] = ptrIdac[capsense_FREQ_CHANNEL_2];
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */ 
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
 
    return (calibrateStatus);
}


/*******************************************************************************
* Function Name: capsense_CSDCalibrateWidget
****************************************************************************//**
*
* \brief
*  Executes IDAC calibration for all sensors in the widget specified in an input.
*
* \details
*   There are two IDAC modes: the single-IDAC mode and the dual-IDAC mode.
*   In the single-IDAC mode Modulator IDAC is enabled only. 
*   In the dual-IDAC mode both Modulator and Compensation IDACs are enabled. 
*
*   The calibration in the single-IDAC mode:
*   1. Calibrate the IDAC for all the sensors (IDAC in in 8-bit mode);
*   2. Identify the maximum IDAC value (highest Cp sensor), then apply the same 
*      values for all the sensors.
*   
*   The calibration in the dual-IDAC mode:
*   1. Find the 8-bit IDAC values (iDAC8(i)) for each sensor to set 
*      Raw Count of respective sensor to 85% of full-scale value.
*   2. Identify the maximum IDAC value (highest Cp sensor – iDAC8Max).
*   3. Calculate modulator IDAC for all sensors in the widget to be equal to
*      50% max iDAC8Max value (idacMod = iDAC8Max/2 + 1). 
*      Note: we add 1 to always have idacMod >= idacComp;
*   4. Find compensation IDAC for each sensor using following equation:
*      idacComp(i) = iDAC8(i) – idacMod.
*   For the highest Cp sensor, idacMod = idacComp = iDAC8/2
*   For all other sensors, idacMod = iDAC8/2 and idacComp must be different 
*   to maintain the same raw count level for all the sensors.
*
*  This function does not check the CSD or CSX widget type.
*   
* \param wdgtIndex The Id of the widget which should be calibrated.
* \param target    Percentage of the maximum to calibrate to.
* \return Returns the status of operation: 
*   - Zero     - All the sensors in the widget are calibrated successfully.
*   - Non-Zero - Calibration failed for any sensor in the widget.
*
*******************************************************************************/
cystatus capsense_CSDCalibrateWidget(uint32 wdgtIndex, uint32 target)
{
    uint32 rawLevel;
        uint32 newRegValue;
    cystatus calibrateStatus = CYRET_SUCCESS;

    /*  Declare and initialise ptr to widget and sensor structures to appropriate address        */
    capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                          capsense_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;
    uint8 iDAC8Max[capsense_NUM_SCAN_FREQS] = {0u};
    uint8 *ptrIdac;

    /* Calibrate maximum level for given resolution */
    rawLevel = (0x00000001Lu << ptrWdgt->resolution);
    
    /* Calculate percentage of maximum value */
    rawLevel = (rawLevel * target) / capsense_PERCENTAGE_100;

    /* Update capsense_widgetIndex */
    capsense_widgetIndex = (uint8)wdgtIndex;
  
    #if (capsense_ENABLE == capsense_CSDV2)
        /* Enable IDACA LEG1 in CSD mode, IDACA LEG2 in GP mode */
        CY_SET_REG32(capsense_IDAC_MOD_PTR, capsense_CALIBRATE_IDAC_MOD);
        #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
            (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)
            CY_SET_REG32(capsense_IDAC_COMP_PTR, capsense_CALIBRATE_IDAC_COMP);
        #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                  (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN */
    #else
        /* Enable Modulator IDAC only in variable mode */
        newRegValue = CY_GET_REG32(capsense_IDAC_PTR) & 
                      ~(capsense_IDAC_MOD_MODE_MASK | capsense_IDAC_COMP_MODE_MASK);
        newRegValue |= capsense_IDAC_MOD_MODE_VARIABLE;
         CY_SET_REG32(capsense_IDAC_PTR, newRegValue);
    #endif /* (capsense_ENABLE == capsense_CSDV2) */

    /* Setup ISR handler to single-sensor scan function */
    capsense_ISR_StartEx(&capsense_CSDPostSingleScan);

    /* Init pointer to Modulator IDAC */
    ptrIdac = &ptrWdgt->idacMod[0u];
    
    /* Go through all sensors (or columns of touchpad) of widget to calibrate them */
    for (capsense_sensorIndex = 0u; 
         capsense_sensorIndex < capsense_dsFlash.wdgtArray[(wdgtIndex)].numCols;
         capsense_sensorIndex++)
    {
        /* Calibrate current sensor in widget */
        calibrateStatus |= capsense_SsCSDCalibrateOneSensor(rawLevel, ptrIdac, iDAC8Max);  
    }

    /* Normalize idacMod and idacComp values */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        /* Calculate Modulator value: idacMod = (iDAC8Max + 1)/2  */ 
        ptrIdac[0u] = (iDAC8Max[0u] + 1u) >> 1u;

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            /* Do same for first frequency channel  */
            ptrIdac[capsense_FREQ_CHANNEL_1] =  (iDAC8Max[capsense_FREQ_CHANNEL_1] + 1u) >> 1u;

            /* Do same for second frequency channel  */
            ptrIdac[capsense_FREQ_CHANNEL_2] = (iDAC8Max[capsense_FREQ_CHANNEL_2] + 1u) >> 1u;
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

        /* Go through all columns of touchpad (or all sensors in button) to normalize them */ 
        for (capsense_sensorIndex = 0u; 
             capsense_sensorIndex < capsense_dsFlash.wdgtArray[(wdgtIndex)].numCols;
             capsense_sensorIndex++)
        { 
            capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                     capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsRam + capsense_sensorIndex;
            /* Calculate Compensation IDAC value: idacComp(i) = iDAC8(i) - idacMod */
            if (capsense_curRamSnsPtr->idacComp[0u] >= ptrIdac[0u])
            {
                capsense_curRamSnsPtr->idacComp[0u] = capsense_curRamSnsPtr->idacComp[0u] - ptrIdac[0u];
            }
            else
            {
                capsense_curRamSnsPtr->idacComp[0u] = 0u;
            }
            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                /* Do same for first frequency channel  */
                if (capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] >= ptrIdac[capsense_FREQ_CHANNEL_1])
                {
                    capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] = 
                    capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] - ptrIdac[capsense_FREQ_CHANNEL_1];
                }
                else
                {
                   capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] = 0u; 
                }
                /* Do same for second frequency channel  */
                if (capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] >= ptrIdac[capsense_FREQ_CHANNEL_2])
                {
                    capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] = 
                    capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] - ptrIdac[capsense_FREQ_CHANNEL_2];
                }
                else
                {
                    capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] = 0u;
                }
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */   
        }
    #else
        /* Normalize widget for single-IDAC mode*/    
        ptrIdac[0u] = iDAC8Max[0u];
        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            ptrIdac[capsense_FREQ_CHANNEL_1] = iDAC8Max[capsense_FREQ_CHANNEL_1];
            ptrIdac[capsense_FREQ_CHANNEL_2] = iDAC8Max[capsense_FREQ_CHANNEL_2];
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */ 
   
    #if (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
        if ((capsense_WD_TOUCHPAD_E == (capsense_WD_TYPE_ENUM)capsense_dsFlash.wdgtArray[(wdgtIndex)].wdgtType) ||
            (capsense_WD_MATRIX_BUTTON_E == (capsense_WD_TYPE_ENUM)capsense_dsFlash.wdgtArray[(wdgtIndex)].wdgtType))
        {
            /* Init pointer to Modulator IDAC for columns */
            ptrIdac = &ptrWdgt->rowIdacMod[0u]; 
               
            /* Go through all columns of touchpad to calibrate them */   
            for (capsense_sensorIndex = capsense_dsFlash.wdgtArray[(wdgtIndex)].numCols;
                 capsense_sensorIndex < (uint8)capsense_dsFlash.wdgtArray[(wdgtIndex)].totalNumSns;
                 capsense_sensorIndex++)
            {
                /* Calibrate current sensor in widget */
                calibrateStatus |= capsense_SsCSDCalibrateOneSensor(rawLevel, ptrIdac, iDAC8Max);
            }
        
            /* Normalize idacMod and idacComp values */
            #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
                /* Calculate Modulator value: idacMod = (iDAC8Max + 1)/2  */ 
                ptrIdac[0u] = (iDAC8Max[0u] + 1u) >> 1u;
                #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                    /* Do same for first frequency channel  */
                    ptrIdac[capsense_FREQ_CHANNEL_1] =  (iDAC8Max[capsense_FREQ_CHANNEL_1] + 1u) >> 1u;

                    /* Do same for second frequency channel  */
                    ptrIdac[capsense_FREQ_CHANNEL_2] = (iDAC8Max[capsense_FREQ_CHANNEL_2] + 1u) >> 1u;
                #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

                /* Go through all columns of touchpad to normalize them */ 
                for (capsense_sensorIndex = capsense_dsFlash.wdgtArray[(wdgtIndex)].numCols; 
                     capsense_sensorIndex < (uint8)capsense_dsFlash.wdgtArray[(wdgtIndex)].totalNumSns;
                     capsense_sensorIndex++)
                { 
                    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                             capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsRam + capsense_sensorIndex;
                
                    /* Calculate Compensation IDAC value: idacComp(i) = iDAC8(i) - idacMod */
                    if (capsense_curRamSnsPtr->idacComp[0u] >= ptrIdac[0u])
                    {
                        capsense_curRamSnsPtr->idacComp[0u] = capsense_curRamSnsPtr->idacComp[0u] - ptrIdac[0u];
                    }
                    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                        /* Do same for first frequency channel  */
                        if (capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] >= ptrIdac[capsense_FREQ_CHANNEL_1])
                        {
                            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] = 
                            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_1] - ptrIdac[capsense_FREQ_CHANNEL_1];
                        }
                        /* Do same for second frequency channel  */
                        if (capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] >= ptrIdac[capsense_FREQ_CHANNEL_2])
                        {
                            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] = 
                            capsense_curRamSnsPtr->idacComp[capsense_FREQ_CHANNEL_2] - ptrIdac[capsense_FREQ_CHANNEL_2];
                        }
                    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */   
                }
            #else
                /* Normalize widget for single-IDAC mode*/    
                ptrIdac[0u] = iDAC8Max[0u];
                #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                    ptrIdac[capsense_FREQ_CHANNEL_1] = iDAC8Max[capsense_FREQ_CHANNEL_1];
                    ptrIdac[capsense_FREQ_CHANNEL_2] = iDAC8Max[capsense_FREQ_CHANNEL_2];
                #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
            #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
        }
    #endif /* (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN) */
 
    /* Restore IDAC configuration */
    #if (capsense_ENABLE == capsense_CSDV2)
        newRegValue = CY_GET_REG32(capsense_SW_REFGEN_SEL_PTR);
        newRegValue |= capsense_DEFAULT_SW_REFGEN_SEL;
        CY_SET_REG32(capsense_SW_REFGEN_SEL_PTR, newRegValue);
        CY_SET_REG32(capsense_IDAC_MOD_PTR, capsense_DEFAULT_IDAC_MOD);
        #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
            (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)    
            CY_SET_REG32(capsense_IDAC_COMP_PTR, capsense_DEFAULT_IDAC_COMP);
        #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                  (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN */
    #else
        #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
            CY_SET_REG32(capsense_IDAC_PTR, capsense_DEFAULT_CSD_IDAC_CONFIG);
        #else
            CY_SET_REG32(capsense_IDAC_PTR, CY_GET_REG32(capsense_IDAC_PTR) &
                                                                ~(capsense_IDAC_MOD_MODE_MASK | capsense_IDAC_MOD_MASK));
            CY_SET_REG32(capsense_IDAC_PTR, CY_GET_REG32(capsense_IDAC_PTR) | capsense_DEFAULT_CSD_IDAC_CONFIG);
        #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
    #endif /* (capsense_ENABLE == capsense_CSDV2) */
    return calibrateStatus;
}
#endif /* ((capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)) || \
            (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))  */


#if (capsense_DISABLE == capsense_CSDV2)
/*******************************************************************************
* Function Name: capsense_SsCmodPrecharge
****************************************************************************//**
*
* \brief
*  This function enables the CSD block and initializes the Cmod charging to Vref.
*
* \details
*  The function enables the CSD block and initializes the Cmod charging to Vref. 
*  Then it waits the even when Cmod is completely charged
*  to Vref to have stable Rawcounts. Software Watchdog Counter is implemented to 
*  prevent the project hanging.
*
*******************************************************************************/
static void capsense_SsCmodPrecharge(void)
{   
    uint32 newRegValue;
    uint32 watchdogCounter;
    #if (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN)
        capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                        capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
    #endif /* (capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) */

    /*  Enable CSD block. Disconnect Vref Buffer from AMUX */  
    CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CTANK_PRECHARGE_CONFIG_CSD_EN);

    /*  Get SnsClck divider */
    #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
        newRegValue = (uint32)capsense_dsRam.snsCsdClk;
    #else
        #if (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
            if (capsense_dsFlash.wdgtArray[(capsense_widgetIndex)].numCols <= capsense_sensorIndex)
            {
                newRegValue = (uint32)ptrWdgt->rowSnsClk;
            }
            else
            {
                newRegValue = (uint32)ptrWdgt->snsClk; 
            } 
        #else
            newRegValue = (uint32)ptrWdgt->snsClk;  
        #endif /* (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN) */  
    #endif /* (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */

    /*  Stop clocks, set dividers, and start clock to align clock phase */
    capsense_ConfigureClocks(newRegValue, (uint32)capsense_dsRam.modCsdClk);

    #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
        /*  Scan one cycle to reset the flip-flop for Direct clock mode */
        CyIntDisable(capsense_ISR_NUMBER);
        CY_SET_REG32(capsense_COUNTER_PTR, capsense_ONE_CYCLE);

        while(0u != (CY_GET_REG32(capsense_COUNTER_PTR) & capsense_RESOLUTION_16_BITS))
        {
            /* Wait until scanning is complete */ 
        }
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);
        CyIntClearPending(capsense_ISR_NUMBER);
    #endif /* (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE) */
    
    /*  Save the current IDAC configuration */ 
    newRegValue = CY_GET_REG32(capsense_IDAC_PTR);

    /*  Wait until Cmod discharges below Vref */ 
    CyDelayCycles(capsense_CMOD_DISCHARGE_CYCLES);
    
    /*  Set the IDAC configuration for fast Cmod precharge to Vref */ 
    CY_SET_REG32(capsense_IDAC_PTR, capsense_CSD_IDAC_PRECHARGE_CONFIG);

    #if (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG)
        CY_SET_REG32(capsense_CONFIG_PTR, CY_GET_REG32(capsense_CONFIG_PTR) & ~capsense_CONFIG_POLARITY_MASK);
    #endif /* (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG) */
    
    /*  Init Watchdog Counter to prevent a hang */ 
    watchdogCounter = capsense_SOFTWARE_WDT_CYCLES_NUM;

    /*  Wait until Cmod reaches to Vref */ 
    while((0u == (CY_GET_REG32(capsense_STATUS_PTR) & capsense_STATUS_SAMPLE)) && (0u != watchdogCounter))
    {
        watchdogCounter--;
    } 
    
    #if (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG)
        CY_SET_REG32(capsense_CONFIG_PTR, CY_GET_REG32(capsense_CONFIG_PTR) | capsense_CONFIG_POLARITY_MASK);
    #endif /* (capsense_IDAC_SINKING == capsense_CSD_IDAC_CONFIG) */

    /*  Restore the current IDAC configuration */ 
    CY_SET_REG32(capsense_IDAC_PTR, newRegValue);

    /*  Enable the sense modulator output */ 
    CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CTANK_PRECHARGE_CONFIG_CSD_EN | capsense_CONFIG_SENSE_EN_MASK);

    CyIntEnable(capsense_ISR_NUMBER);
}
#endif /* (capsense_DISABLE == capsense_CSDV2) */

#endif  /* (capsense_ENABLE == capsense_CSD_EN) */


/* [] END OF FILE */
