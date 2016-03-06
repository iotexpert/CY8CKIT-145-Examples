/***************************************************************************//**
* \file capsense_Sensing.h
* \version 3.0
*
* \brief
* This file provides the headers of APIs specific to implementation of the
* sensing FW block.
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

#if !defined(CY_CAPSENSE_capsense_SENSING_H)
#define CY_CAPSENSE_capsense_SENSING_H

#include <CyLib.h>
#include <cyfitter.h>
#include "cytypes.h"
#include "capsense_Configuration.h"
#include "capsense_Structure.h"
#if (capsense_DISABLE == capsense_CSD2X_EN)
    #include "capsense_ISR.h"
#endif /* (capsense_DISABLE == capsense_CSD2X_EN) */

#if (capsense_ENABLE == capsense_CSX_EN)
    #include "capsense_SensingCSX_LL.h"
#endif /* (capsense_ENABLE == capsense_CSX_EN) */
#if (capsense_ENABLE == capsense_CSD_EN)
    #include "capsense_SensingCSD_LL.h"
#endif /* (capsense_ENABLE == capsense_CSD_EN) */
#if (capsense_ENABLE == capsense_CSD2X_EN)
    #include "capsense_SensingCSD2X_LL.h"
#endif /* (capsense_ENABLE == capsense_CSD2X_EN) */
#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
    #include "capsense_SmartSense_LL.h"
#endif  /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
#if (capsense_ENABLE == capsense_ADC_EN)
    #include "capsense_Adc.h"
#endif /* (capsense_ENABLE == capsense_ADC_EN) */

/*******************************************************************************
* Enumeration types definition
*******************************************************************************/

/* Pin states */
#define capsense_GROUND                         (0u)
#define capsense_HIGHZ                          (1u)
#define capsense_SHIELD                         (2u)
#define capsense_SENSOR                         (3u)
#define capsense_TX_PIN                         (4u)
#define capsense_RX_PIN                         (5u)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
* \if SECTION_CAPSENSE_HIGH_LEVEL
* \addtogroup group_capsense_high_level
* \{
*/

cystatus capsense_SetupWidget(uint32 wdgtIndex);
cystatus capsense_Scan(void);
cystatus capsense_ScanAllWidgets(void);
uint32 capsense_IsBusy(void);

/** \}
* \endif */

/**
* \if SECTION_CAPSENSE_LOW_LEVEL
* \addtogroup group_capsense_low_level
* \{
*/

void capsense_SetPinState(uint32 wdgtIndex, uint32 snsIndex, uint32 state);
#if ((capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN) || \
     (capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN))
cystatus capsense_CalibrateWidget(uint32 wdgtIndex);
cystatus capsense_CalibrateAllWidgets(void);
#endif /* ((capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN) || \
           (capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN))  */

/** \}
* \endif */

/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

cystatus capsense_SsInitialize(void);
void capsense_ConfigureModClkClock(uint32 modClk);
void capsense_ConfigureSnsClock(uint32 snsClk);
void capsense_ConfigureClocks(uint32 snsClk, uint32 modClk);
void capsense_IsrInitialize(cyisraddress address);
void capsense_PostAllWidgetsScan(void);
#if (capsense_ENABLE == capsense_CSD_CSX_EN)
    void capsense_SwitchSensingMode(capsense_SENSE_METHOD_ENUM mode);
#endif  /* (capsense_ENABLE == capsense_CSD_CSX_EN)) */
#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
    cystatus capsense_AutoTune(void);
#endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
#if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
    void capsense_ChangeImoFreq(uint32 value);
#endif  /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
#if (capsense_ENABLE == capsense_ADC_EN)
cystatus capsense_SsReleaseResources(void);
#endif  /* (capsense_ENABLE == capsense_ADC_EN) */

/** \}
* \endif */


/****************************************************************************
* m0s8csd, m0s8peri, hsiom, and IO hardware-related registers address
****************************************************************************/

#define capsense_DR_MASK                                (0x00000001Lu)

#if (capsense_ENABLE == capsense_CSDV2)

    /* CSDv2 block registers   */
    #define capsense_CONFIG_REG                         (*(reg32 *) CYREG_CSD_CONFIG)
    #define capsense_CONFIG_PTR                         ( (reg32 *) CYREG_CSD_CONFIG)
    
    #define capsense_STAT_SEQ_REG                       (*(reg32 *) CYREG_CSD_CONFIG)
    #define capsense_STAT_SEQ_PTR                       ( (reg32 *) CYREG_CSD_CONFIG)

    #define capsense_IDAC_MOD_REG                       (*(reg32 *) CYREG_CSD_IDACA)
    #define capsense_IDAC_MOD_PTR                       ( (reg32 *) CYREG_CSD_IDACA)

    #define capsense_IDAC_COMP_REG                      (*(reg32 *) CYREG_CSD_IDACB)
    #define capsense_IDAC_COMP_PTR                      ( (reg32 *) CYREG_CSD_IDACB)

    #define capsense_SENSE_PERIOD_REG                   (*(reg32 *) CYREG_CSD_SENSE_PERIOD)
    #define capsense_SENSE_PERIOD_PTR                   ( (reg32 *) CYREG_CSD_SENSE_PERIOD)

    #define capsense_RESULT_VAL1_REG                    (*(reg32 *) CYREG_CSD_RESULT_VAL1)
    #define capsense_RESULT_VAL1_PTR                    ( (reg32 *) CYREG_CSD_RESULT_VAL1)

    #define capsense_RESULT_VAL2_REG                    (*(reg32 *) CYREG_CSD_RESULT_VAL2)
    #define capsense_RESULT_VAL2_PTR                    ( (reg32 *) CYREG_CSD_RESULT_VAL2)

    #define capsense_INTR_REG                           (*(reg32 *) CYREG_CSD_INTR)
    #define capsense_INTR_PTR                           ( (reg32 *) CYREG_CSD_INTR)

    #define capsense_INTR_SET_REG                       (*(reg32 *) CYREG_CSD_INTR_SET)
    #define capsense_INTR_SET_PTR                       ( (reg32 *) CYREG_CSD_INTR_SET)

    #define capsense_INTR_MASK_REG                      (*(reg32 *) CYREG_CSD_INTR_MASK)
    #define capsense_INTR_MASK_PTR                      ( (reg32 *) CYREG_CSD_INTR_MASK)

    #define capsense_COUNTER_REG                        (capsense_RESULT_VAL1_REG)
    #define capsense_COUNTER_PTR                        (capsense_RESULT_VAL1_PTR)

    #define capsense_REFGEN_REG                         (*(reg32 *) CYREG_CSD_REFGEN)
    #define capsense_REFGEN_PTR                         ( (reg32 *) CYREG_CSD_REFGEN)

    #define capsense_SEQ_TIME_REG                       (*(reg32 *) CYREG_CSD_SEQ_TIME)
    #define capsense_SEQ_TIME_PTR                       ( (reg32 *) CYREG_CSD_SEQ_TIME)

    #define capsense_SEQ_INIT_CNT_REG                   (*(reg32 *) CYREG_CSD_SEQ_INIT_CNT)
    #define capsense_SEQ_INIT_CNT_PTR                   ( (reg32 *) CYREG_CSD_SEQ_INIT_CNT)

    #define capsense_SEQ_NORM_CNT_REG                   (*(reg32 *) CYREG_CSD_SEQ_NORM_CNT)
    #define capsense_SEQ_NORM_CNT_PTR                   ( (reg32 *) CYREG_CSD_SEQ_NORM_CNT)

    #define capsense_SEQ_START_REG                      (*(reg32 *) CYREG_CSD_SEQ_START)
    #define capsense_SEQ_START_PTR                      ( (reg32 *) CYREG_CSD_SEQ_START)

    #define capsense_CSDCMP_REG                         (*(reg32 *) CYREG_CSD_CSDCMP)
    #define capsense_CSDCMP_PTR                         ( (reg32 *) CYREG_CSD_CSDCMP)

    #define capsense_HSCMP_REG                          (*(reg32 *) CYREG_CSD_HSCMP)
    #define capsense_HSCMP_PTR                          ( (reg32 *) CYREG_CSD_HSCMP)

    #define capsense_SENSE_DUTY_REG                     (*(reg32 *) CYREG_CSD_SENSE_DUTY)
    #define capsense_SENSE_DUTY_PTR                     ( (reg32 *) CYREG_CSD_SENSE_DUTY)

    #define capsense_AMBUF_REG                          (*(reg32 *) CYREG_CSD_AMBUF)
    #define capsense_AMBUF_PTR                          ( (reg32 *) CYREG_CSD_AMBUF)

    #define capsense_SW_BYP_SEL_REG                     (*(reg32 *) CYREG_CSD_SW_BYP_SEL)
    #define capsense_SW_BYP_SEL_PTR                     ( (reg32 *) CYREG_CSD_SW_BYP_SEL)

    #define capsense_SW_CMP_P_SEL_REG                   (*(reg32 *) CYREG_CSD_SW_CMP_P_SEL)
    #define capsense_SW_CMP_P_SEL_PTR                   ( (reg32 *) CYREG_CSD_SW_CMP_P_SEL)

    #define capsense_SW_REFGEN_SEL_REG                  (*(reg32 *) CYREG_CSD_SW_REFGEN_SEL)
    #define capsense_SW_REFGEN_SEL_PTR                  ( (reg32 *) CYREG_CSD_SW_REFGEN_SEL)

    #define capsense_SW_CMP_N_SEL_REG                   (*(reg32 *) CYREG_CSD_SW_CMP_N_SEL)
    #define capsense_SW_CMP_N_SEL_PTR                   ( (reg32 *) CYREG_CSD_SW_CMP_N_SEL)

    #define capsense_SW_RES_REG                         (*(reg32 *) CYREG_CSD_SW_RES)
    #define capsense_SW_RES_PTR                         ( (reg32 *) CYREG_CSD_SW_RES)

    #define capsense_SW_HS_P_SEL_REG                    (*(reg32 *) CYREG_CSD_SW_HS_P_SEL)
    #define capsense_SW_HS_P_SEL_PTR                    ( (reg32 *) CYREG_CSD_SW_HS_P_SEL)

    #define capsense_SW_SHIELD_SEL_REG                  (*(reg32 *) CYREG_CSD_SW_SHIELD_SEL)
    #define capsense_SW_SHIELD_SEL_PTR                  ( (reg32 *) CYREG_CSD_SW_SHIELD_SEL)

    #define capsense_SW_AMUXBUF_SEL_REG                 (*(reg32 *) CYREG_CSD_SW_AMUXBUF_SEL)
    #define capsense_SW_AMUXBUF_SEL_PTR                 ( (reg32 *) CYREG_CSD_SW_AMUXBUF_SEL)

    #define capsense_SW_HS_N_SEL_REG                    (*(reg32 *) CYREG_CSD_SW_HS_N_SEL)
    #define capsense_SW_HS_N_SEL_PTR                    ( (reg32 *) CYREG_CSD_SW_HS_N_SEL)

    #define capsense_SW_FW_MOD_SEL_REG                  (*(reg32 *) CYREG_CSD_SW_FW_MOD_SEL)
    #define capsense_SW_FW_MOD_SEL_PTR                  ( (reg32 *) CYREG_CSD_SW_FW_MOD_SEL)

    #define capsense_SW_FW_TANK_SEL_REG                 (*(reg32 *) CYREG_CSD_SW_FW_TANK_SEL)
    #define capsense_SW_FW_TANK_SEL_PTR                 ( (reg32 *) CYREG_CSD_SW_FW_TANK_SEL)

    #define capsense_SW_DSI_SEL_REG                     (*(reg32 *) CYREG_CSD_SW_DSI_SEL)
    #define capsense_SW_DSI_SEL_PTR                     ( (reg32 *) CYREG_CSD_SW_DSI_SEL)

    #define capsense_ADC_CTL_REG                        (*(reg32 *) CYREG_CSD_ADC_CTL)
    #define capsense_ADC_CTL_PTR                        ( (reg32 *) CYREG_CSD_ADC_CTL)
#else

    #if (capsense_ENABLE == capsense_CSD2X_EN)
        /* CSD0 block registers   */
        #define capsense_CONFIG_REG                     (*(reg32 *) capsense_CSD0__CSD_CONFIG)
        #define capsense_CONFIG_PTR                     ( (reg32 *) capsense_CSD0__CSD_CONFIG)
        #define capsense_IDAC_CONTR_REG                 (*(reg32 *) capsense_IDACMod0_cy_psoc4_idac__CONTROL)
        #define capsense_IDAC_CONTR_PTR                 ( (reg32 *) capsense_IDACMod0_cy_psoc4_idac__CONTROL)
        #define capsense_IDAC_REG                       (*(reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_IDAC)
        #define capsense_IDAC_PTR                       ( (reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_IDAC)
        #define capsense_COUNTER_REG                    (*(reg32 *) capsense_CSD0__CSD_COUNTER)
        #define capsense_COUNTER_PTR                    ( (reg32 *) capsense_CSD0__CSD_COUNTER)
        #define capsense_STATUS_REG                     (*(reg32 *) capsense_CSD__CSD_STATUS)
        #define capsense_STATUS_PTR                     ( (reg32 *) capsense_CSD__CSD_STATUS)
        #define capsense_INTR_REG                       (*(reg32 *) capsense_CSD0__CSD_INTR)
        #define capsense_INTR_PTR                       ( (reg32 *) capsense_CSD0__CSD_INTR)
        #define capsense_INTR_SET_REG                   (*(reg32 *) CYREG_CSD_INTR_SET)
        #define capsense_INTR_SET_PTR                   ( (reg32 *) CYREG_CSD_INTR_SET)
        #define capsense_PWM_REG                        (*(reg32 *) CYREG_CSD_PWM)
        #define capsense_PWM_PTR                        ( (reg32 *) CYREG_CSD_PWM)
        #define capsense_TRIM1_REG                      (*(reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_TRIM1)
        #define capsense_TRIM1_PTR                      ( (reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_TRIM1)
        #define capsense_TRIM2_REG                      (*(reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_TRIM2)
        #define capsense_TRIM2_PTR                      ( (reg32 *) capsense_IDACMod0_cy_psoc4_idac__CSD_TRIM2)
    #else
        /* CSD block registers   */
        #define capsense_CONFIG_REG                     (*(reg32 *) capsense_CSD__CSD_CONFIG)
        #define capsense_CONFIG_PTR                     ( (reg32 *) capsense_CSD__CSD_CONFIG)
        #define capsense_IDAC_CONTR_REG                 (*(reg32 *) capsense_IDACMod_cy_psoc4_idac__CONTROL)
        #define capsense_IDAC_CONTR_PTR                 ( (reg32 *) capsense_IDACMod_cy_psoc4_idac__CONTROL)
        #define capsense_IDAC_REG                       (*(reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_IDAC)
        #define capsense_IDAC_PTR                       ( (reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_IDAC)
        #define capsense_COUNTER_REG                    (*(reg32 *) capsense_CSD__CSD_COUNTER)
        #define capsense_COUNTER_PTR                    ( (reg32 *) capsense_CSD__CSD_COUNTER)
        #define capsense_STATUS_REG                     (*(reg32 *) capsense_CSD__CSD_STATUS)
        #define capsense_STATUS_PTR                     ( (reg32 *) capsense_CSD__CSD_STATUS)
        #define capsense_INTR_REG                       (*(reg32 *) capsense_CSD__CSD_INTR)
        #define capsense_INTR_PTR                       ( (reg32 *) capsense_CSD__CSD_INTR)
        #define capsense_INTR_SET_REG                   (*(reg32 *) CYREG_CSD_INTR_SET)
        #define capsense_INTR_SET_PTR                   ( (reg32 *) CYREG_CSD_INTR_SET)
        #define capsense_PWM_REG                        (*(reg32 *) CYREG_CSD_PWM)
        #define capsense_PWM_PTR                        ( (reg32 *) CYREG_CSD_PWM)
        #define capsense_TRIM1_REG                      (*(reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_TRIM1)
        #define capsense_TRIM1_PTR                      ( (reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_TRIM1)
        #define capsense_TRIM2_REG                      (*(reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_TRIM2)
        #define capsense_TRIM2_PTR                      ( (reg32 *) capsense_IDACMod_cy_psoc4_idac__CSD_TRIM2)
    #endif /* (capsense_ENABLE == capsense_CSD2X_EN) */

    #if (CY_PSOC4_4100M || CY_PSOC4_4200M)
        #if (1u != capsense_CSD__CSD_NUMBER)
            #define capsense_SFLASH_TRIM1_REG           (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
            #define capsense_SFLASH_TRIM1_PTR           ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

            #define capsense_SFLASH_TRIM2_REG           (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
            #define capsense_SFLASH_TRIM2_PTR           ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
        #else
            #define capsense_SFLASH_TRIM1_REG           (*(reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)
            #define capsense_SFLASH_TRIM1_PTR           ( (reg8 *) CYREG_SFLASH_CSD1_TRIM1_CSD)

            #define capsense_SFLASH_TRIM2_REG           (*(reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
            #define capsense_SFLASH_TRIM2_PTR           ( (reg8 *) CYREG_SFLASH_CSD1_TRIM2_CSD)
        #endif /* (1u != capsense_CSD__CSD_NUMBER) */
    #else
        #define capsense_SFLASH_TRIM1_REG               (*(reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)
        #define capsense_SFLASH_TRIM1_PTR               ( (reg8 *) CYREG_SFLASH_CSD_TRIM1_CSD)

        #define capsense_SFLASH_TRIM2_REG               (*(reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
        #define capsense_SFLASH_TRIM2_PTR               ( (reg8 *) CYREG_SFLASH_CSD_TRIM2_CSD)
    #endif /* (CY_PSOC4_4100M || CY_PSOC4_4200M) */

#endif /* (capsense_ENABLE == capsense_CSDV2) */

/* CSDv1 and CSDv2 PERI CLOCK registers */
#if defined(CYIPBLOCK_m0s8peri_VERSION)
    #define capsense_IS_M0S8PERI_BLOCK                  (1u)
    #define capsense_M0S8PERI_BLOCK_VER                 ((CYIPBLOCK_m0s8peri_VERSION))
#else
    #define capsense_IS_M0S8PERI_BLOCK                  (0u)
    #define capsense_M0S8PERI_BLOCK_VER                 (0u)
#endif /* (CYIPBLOCK_m0s8peri_VERSION) */

#if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
    #define capsense_MODCLK_DIV_REG                     (*(reg32 *)capsense_ModClk__DIV_REGISTER)
    #define capsense_MODCLK_DIV_PTR                     ( (reg32 *)capsense_ModClk__DIV_REGISTER)
    #define capsense_MODCLK_CMD_REG                     (*(reg32 *)CYREG_PERI_DIV_CMD)
    #define capsense_MODCLK_CMD_PTR                     ( (reg32 *)CYREG_PERI_DIV_CMD)
    #if (capsense_DISABLE == capsense_CSDV2)
        #define capsense_SNSCLK_DIV_REG                 (*(reg32 *)capsense_SnsClk__DIV_REGISTER)
        #define capsense_SNSCLK_DIV_PTR                 ( (reg32 *)capsense_SnsClk__DIV_REGISTER)
        #define capsense_SNSCLK_CMD_REG                 (*(reg32 *)CYREG_PERI_DIV_CMD)
        #define capsense_SNSCLK_CMD_PTR                 ( (reg32 *)CYREG_PERI_DIV_CMD)
    #endif /* (capsense_DISABLE == capsense_CSDV2) */
#else
    #define capsense_MODCLK_DIV_REG                     (*(reg32 *)capsense_ModClk__REGISTER)
    #define capsense_MODCLK_DIV_PTR                     ( (reg32 *)capsense_ModClk__REGISTER)
    #define capsense_SNSCLK_DIV_REG                     (*(reg32 *)capsense_SnsClk__REGISTER)
    #define capsense_SNSCLK_DIV_PTR                     ( (reg32 *)capsense_SnsClk__REGISTER)
    #define capsense_MODCLK_CMD_REG                     (capsense_MODCLK_DIV_REG)
    #define capsense_MODCLK_CMD_PTR                     (capsense_MODCLK_DIV_PTR)
    #define capsense_SNSCLK_CMD_REG                     (capsense_SNSCLK_DIV_REG)
    #define capsense_SNSCLK_CMD_PTR                     (capsense_SNSCLK_DIV_PTR)
#endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */

/****************************************************************************
* m0s8csd, m0s8peri, hsiom, and IO hardware-related registers masks
****************************************************************************/
#if (capsense_ENABLE == capsense_CSDV2)

    /* CSDv2 masks for Cmod, Cshield and Csh pads */
    #if (capsense_ENABLE == capsense_CSD_CSX_EN)
        #define capsense_CMOD_PAD                       (1u)
        #define capsense_CSHIELD_PAD                    (4u)
        #define capsense_CSH_TANK_PAD                   (2u)
        #define capsense_CMOD_CONNECTION                (capsense_CSHIELD_PAD)
    #elif (capsense_ENABLE == capsense_CSD_EN)
        #define capsense_CMOD_PAD                       (capsense_CSD__CMOD_PAD)
        #define capsense_CSHIELD_PAD                    (capsense_CSD__CSHIELD_PAD)
        #define capsense_CSH_TANK_PAD                   (capsense_CSD__CSH_TANK_PAD)
        #define capsense_CMOD_CONNECTION                (capsense_CSD__DEDICATED_IO)
    #else
        /* Not supported mode */
    #endif  /* (capsense_ENABLE == capsense_CSD_CSX_EN) */
    
    #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
         (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN))
        #if((4u == capsense_Csh__PORT) && (3u == capsense_Csh__SHIFT))
            #define capsense_CTANK_CONNECTION           (capsense_CSH_TANK_PAD)
        #else
            #define capsense_CTANK_CONNECTION           (0u)
        #endif /* ((4u == capsense_Csh__PORT) || (2u == capsense_Csh__SHIFT)) */
    #else
        #define capsense_CTANK_CONNECTION               (0u) 
    #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
               (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN)) */

    /* CSDv2 masks for the resistance or low EMI (slow ramp)  */
    #define capsense_RES_LOW                            (0u)
    #define capsense_RES_MED                            (1u)
    #define capsense_RES_HIGH                           (2u)
    #define capsense_RES_LOWEMI                         (3u)

    /* CSDv2 masks for the waveforms for corresponding switch  */
    #define capsense_STATIC_OPEN                        (0x00000000Lu)
    #define capsense_STATIC_CLOSE                       (0x00000001Lu)
    #define capsense_PHI1                               (0x00000002Lu)
    #define capsense_PHI2                               (0x00000003Lu)
    #define capsense_PHI1_HSCMP                         (0x00000004Lu)
    #define capsense_PHI2_HSCMP                         (0x00000005Lu)
    #define capsense_HSCMP                              (0x00000006Lu)

    /* CSDv2 CSD_CONFIG register masks    */
    #define capsense_CONFIG_DUAL_CAP_EN_MASK            (((0x00000001Lu << CYFLD_CSD_DUAL_CAP_EN__SIZE) - 1u) << CYFLD_CSD_DUAL_CAP_EN__OFFSET)
    #define capsense_CONFIG_FILTER_DELAY_MASK           (((0x00000001Lu << CYFLD_CSD_FILTER_DELAY__SIZE) - 1u) << CYFLD_CSD_FILTER_DELAY__OFFSET)
    #define capsense_CONFIG_FILTER_DELAY_2_CYCLES       (0x00000002Lu)
    #define capsense_CONFIG_FILTER_DELAY_3_CYCLES       (0x00000003Lu)
    #define capsense_CONFIG_FILTER_DELAY_4_CYCLES       (0x00000004Lu)
    #define capsense_CONFIG_FILTER_DELAY_12MHZ          (capsense_CONFIG_FILTER_DELAY_2_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)
    #define capsense_CONFIG_FILTER_DELAY_24MHZ          (capsense_CONFIG_FILTER_DELAY_3_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)
    #define capsense_CONFIG_FILTER_DELAY_48MHZ          (capsense_CONFIG_FILTER_DELAY_4_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)
    #define capsense_CONFIG_SHIELD_DELAY_MASK           (((0x00000001Lu << CYFLD_CSD_SHIELD_DELAY__SIZE) - 1u) << CYFLD_CSD_SHIELD_DELAY__OFFSET)
    #define capsense_CONFIG_SENSE_EN_MASK               (((0x00000001Lu << CYFLD_CSD_SENSE_EN__SIZE) - 1u) << CYFLD_CSD_SENSE_EN__OFFSET)
    #define capsense_CONFIG_CHARGE_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_CHARGE_MODE__SIZE) - 1u) << CYFLD_CSD_CHARGE_MODE__OFFSET)
    #define capsense_CONFIG_MUTUAL_CAP_MASK             (((0x00000001Lu << CYFLD_CSD_MUTUAL_CAP__SIZE) - 1u) << CYFLD_CSD_MUTUAL_CAP__OFFSET)
    #define capsense_CONFIG_CSX_DUAL_CNT_MASK           (((0x00000001Lu << CYFLD_CSD_CSX_DUAL_CNT__SIZE) - 1u) << CYFLD_CSD_CSX_DUAL_CNT__OFFSET)
    #define capsense_CONFIG_DSI_COUNT_SEL_MASK          (((0x00000001Lu << CYFLD_CSD_DSI_COUNT_SEL__SIZE) - 1u) << CYFLD_CSD_DSI_COUNT_SEL__OFFSET)
    #define capsense_CONFIG_DSI_SAMPLE_EN_MASK          (((0x00000001Lu << CYFLD_CSD_DSI_SAMPLE_EN__SIZE) - 1u) << CYFLD_CSD_DSI_SAMPLE_EN__OFFSET)
    #define capsense_CONFIG_SAMPLE_SYNC_MASK            (((0x00000001Lu << CYFLD_CSD_SAMPLE_SYNC__SIZE) - 1u) << CYFLD_CSD_SAMPLE_SYNC__OFFSET)
    #define capsense_CONFIG_DSI_SENSE_EN_MASK           (((0x00000001Lu << CYFLD_CSD_DSI_SENSE_EN__SIZE) - 1u) << CYFLD_CSD_DSI_SENSE_EN__OFFSET)
    #define capsense_CONFIG_LP_MODE_MASK                (((0x00000001Lu << CYFLD_CSD_LP_MODE__SIZE) - 1u) << CYFLD_CSD_LP_MODE__OFFSET)
    #define capsense_CONFIG_ENABLE_MASK                 (((0x00000001Lu << CYFLD_CSD_ENABLE__SIZE) - 1u) << CYFLD_CSD_ENABLE__OFFSET)

    /* CSDv2 CSD_STATUS register masks    */
    #define capsense_STATUS_CSD_CHARGE_MASK             (((0x00000001Lu << CYFLD_CSD_CSD_CHARGE__SIZE) - 1u) << CYFLD_CSD_CSD_CHARGE__OFFSET)
    #define capsense_STATUS_CSD_SENSE_MASK              (((0x00000001Lu << CYFLD_CSD_CSD_SENSE__SIZE) - 1u) << CYFLD_CSD_CSD_SENSE__OFFSET)
    #define capsense_STATUS_HSCMP_OUT_MASK              (((0x00000001Lu << CYFLD_CSD_HSCMP_OUT__SIZE) - 1u) << CYFLD_CSD_HSCMP_OUT__OFFSET)
    #define capsense_STATUS_CSDCMP_OUT_MASK             (((0x00000001Lu << CYFLD_CSD_CSDCMP_OUT__SIZE) - 1u) << CYFLD_CSD_CSDCMP_OUT__OFFSET)

    /* CSDv2 STAT_SEQ register masks    */
    #define capsense_STAT_SEQ_SEQ_STATE_MASK            (((0x00000001Lu << CYFLD_CSD_SEQ_STATE__SIZE) - 1u) << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_ADC_STATE_MASK            (((0x00000001Lu << CYFLD_CSD_ADC_STATE__SIZE) - 1u) << CYFLD_CSD_ADC_STATE__OFFSET)
    
    /* CSDv2 sequencer state codes */
    #define capsense_STAT_SEQ_IDLE                      (0x00000000Lu << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_INIT_COARSE               (0x00000001Lu << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_AUTO_ZERO_0               (0x00000002Lu << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_SAMPLE_INIT               (0x00000003Lu << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_AUTO_ZERO_1               (0x00000004Lu << CYFLD_CSD_SEQ_STATE__OFFSET)
    #define capsense_STAT_SEQ_SAMPLE_NORM               (0x00000005Lu << CYFLD_CSD_SEQ_STATE__OFFSET)


    /* CSDv2 STAT_CNTS register masks    */
    #define capsense_STAT_CNTS_NUM_CONV_MASK            (((0x00000001Lu << CYFLD_CSD_NUM_CONV__SIZE) - 1u) << CYFLD_CSD_NUM_CONV__OFFSET)

    /* CSDv2 RESULT_VAL1 register masks    */
    #define capsense_RESULT_VAL1_VALUE_MASK             (((0x00000001Lu << CYFLD_CSD_VALUE__SIZE) - 1u) << CYFLD_CSD_VALUE__OFFSET)
    #define capsense_RESULT_VAL1_BAD_CONVS_MASK         (((0x00000001Lu << CYFLD_CSD_BAD_CONVS__SIZE) - 1u) << CYFLD_CSD_BAD_CONVS__OFFSET)

    /* CSDv2 RESULT_VAL2 register masks    */
    #define capsense_RESULT_VAL2_VALUE_MASK             (0x0000FFFFLu)

    /* CSDv2 INTR register masks    */
    #define capsense_INTR_SAMPLE_MASK                   (((0x00000001Lu << CYFLD_CSD_SAMPLE__SIZE) - 1u) << CYFLD_CSD_SAMPLE__OFFSET)
    #define capsense_INTR_INIT_MASK                     (((0x00000001Lu << CYFLD_CSD_INIT__SIZE) - 1u) << CYFLD_CSD_INIT__OFFSET)
    #define capsense_INTR_ADC_RES_MASK                  (((0x00000001Lu << CYFLD_CSD_ADC_RES__SIZE) - 1u) << CYFLD_CSD_ADC_RES__OFFSET)

    /* CSDv2 INTR_SET register masks    */
    #define capsense_INTR_SET_SAMPLE_MASK               (capsense_INTR_SAMPLE_MASK)
    #define capsense_INTR_SET_INIT_MASK                 (capsense_INTR_INIT_MASK)
    #define capsense_INTR_SET_ADC_RES_MASK              (capsense_INTR_ADC_RES_MASK)
    #define capsense_INTR_SET_MASK                      (capsense_INTR_SET_SAMPLE_MASK | \
                                                                 capsense_INTR_SET_INIT_MASK | \
                                                                 capsense_INTR_SET_ADC_RES_MASK)

    /* CSDv2 INTR_MASK register masks    */
    #define capsense_INTR_MASK_SAMPLE_MASK              (capsense_INTR_SAMPLE_MASK)
    #define capsense_INTR_MASK_INIT_MASK                (capsense_INTR_INIT_MASK)
    #define capsense_INTR_MASK_ADC_RES_MASK             (capsense_INTR_ADC_RES_MASK)


    /* CSDv2 HSCMP v2 register masks    */
    #define capsense_HSCMP_EN_MASK                      (((0x00000001Lu << CYFLD_CSD_HSCMP_EN__SIZE) - 1u) << CYFLD_CSD_HSCMP_EN__OFFSET)
    #define capsense_HSCMP_INVERT_MASK                  (((0x00000001Lu << CYFLD_CSD_HSCMP_INVERT__SIZE) - 1u) << CYFLD_CSD_HSCMP_INVERT__OFFSET)
    #define capsense_CSD_AZ_EN_MASK                     (((0x00000001Lu << CYFLD_CSD_AZ_EN__SIZE) - 1u) << CYFLD_CSD_AZ_EN__OFFSET)

    /* CSDv2 AMBUF v2 register masks    */
    #define capsense_AMBUF_AMBUF_EN_MASK                ((0x00000001Lu << CYFLD_CSD_PWR_MODE__SIZE) - 1u)
    #define capsense_AMBUF_PWR_MODE_OFF                 (CYVAL_CSD_PWR_MODE_OFF)
    #define capsense_AMBUF_PWR_MODE_NORM                (CYVAL_CSD_PWR_MODE_NORM)
    #define capsense_AMBUF_PWR_MODE_HI                  (CYVAL_CSD_PWR_MODE_HI)

    /* CSDv2 REFGEN v2 register masks    */
    #define capsense_REFGEN_REFGEN_EN_MASK              ((0x00000001Lu << CYFLD_CSD_REFGEN_EN__SIZE) - 1u)
    #define capsense_REFGEN_BYPASS_MASK                 (((0x00000001Lu << CYFLD_CSD_BYPASS__SIZE) - 1u) << CYFLD_CSD_BYPASS__OFFSET)
    #define capsense_REFGEN_VDDA_EN_MASK                (((0x00000001Lu << CYFLD_CSD_VDDA_EN__SIZE) - 1u) << CYFLD_CSD_VDDA_EN__OFFSET)
    #define capsense_REFGEN_RES_EN_MASK                 (((0x00000001Lu << CYFLD_CSD_RES_EN__SIZE) - 1u) << CYFLD_CSD_RES_EN__OFFSET)
    #define capsense_REFGEN_GAIN_MASK                   (((0x00000001Lu << CYFLD_CSD_GAIN__SIZE) - 1u) << CYFLD_CSD_GAIN__OFFSET)
    #define capsense_REFGEN_VREFLO_SEL_MASK             (((0x00000001Lu << CYFLD_CSD_VREFLO_SEL__SIZE) - 1u) << CYFLD_CSD_VREFLO_SEL__OFFSET)
    #define capsense_REFGEN_VREFLO_INT_MASK             (((0x00000001Lu << CYFLD_CSD_VREFLO_INT__SIZE) - 1u) << CYFLD_CSD_VREFLO_INT__OFFSET)

    /* CSDv2 IDACA v2 register masks  */
    #define capsense_IDAC_MOD_VAL_MASK                  (((0x00000001Lu << CYFLD_CSD_VAL__SIZE) - 1u) << CYFLD_CSD_VAL__OFFSET)
    #define capsense_IDAC_MOD_POLARITY_MASK             (((0x00000001Lu << CYFLD_CSD_POLARITY__SIZE) - 1u) << CYFLD_CSD_POLARITY__OFFSET)
    #define capsense_IDAC_MOD_POL_DYN_MASK              (((0x00000001Lu << CYFLD_CSD_POL_DYN__SIZE) - 1u) << CYFLD_CSD_POL_DYN__OFFSET)
    #define capsense_IDAC_MOD_LEG1_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_LEG1_MODE__SIZE) - 1u) << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG1_MODE_GP_STATIC_MASK  (CYVAL_CSD_LEG1_MODE_GP_STATIC)
    #define capsense_IDAC_MOD_LEG1_MODE_GP_MASK         (CYVAL_CSD_LEG1_MODE_GP << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG1_MODE_CSD_STATIC_MASK (CYVAL_CSD_LEG1_MODE_CSD_STATIC << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG1_MODE_CSD_MASK        (0x00000003Lu << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG2_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_LEG2_MODE__SIZE) - 1u) << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG2_MODE_GP_STATIC_MASK  (CYVAL_CSD_LEG2_MODE_GP_STATIC)
    #define capsense_IDAC_MOD_LEG2_MODE_GP_MASK         (CYVAL_CSD_LEG2_MODE_GP << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG2_MODE_CSD_STATIC_MASK (0x00000002Lu << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_MOD_LEG2_MODE_CSD_MASK        (CYVAL_CSD_LEG2_MODE_CSD << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_MOD_DSI_CTRL_EN_MASK          (0x00200000Lu)
    #define capsense_IDAC_MOD_RANGE_MASK                (0x00C00000Lu)
    #define capsense_IDAC_MOD_RANGE_IDAC_LO_MASK        (0x00000000Lu)
    #define capsense_IDAC_MOD_RANGE_IDAC_MED_MASK       (0x00400000Lu)
    #define capsense_IDAC_MOD_RANGE_IDAC_HI_MASK        (0x00800000Lu)
    #define capsense_IDAC_MOD_LEG1_EN_MASK              (0x01000000Lu)
    #define capsense_IDAC_MOD_LEG2_EN_MASK              (0x02000000Lu)

    /* CSDv2 IDACB v2 register masks  */
    #define capsense_IDAC_COMP_VAL_MASK                  (((0x00000001Lu << CYFLD_CSD_VAL__SIZE) - 1u) << CYFLD_CSD_VAL__OFFSET)
    #define capsense_IDAC_COMP_POLARITY_MASK             (((0x00000001Lu << CYFLD_CSD_POLARITY__SIZE) - 1u) << CYFLD_CSD_POLARITY__OFFSET)
    #define capsense_IDAC_COMP_POL_DYN_MASK              (((0x00000001Lu << CYFLD_CSD_POL_DYN__SIZE) - 1u) << CYFLD_CSD_POL_DYN__OFFSET)
    #define capsense_IDAC_COMP_LEG1_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_LEG1_MODE__SIZE) - 1u) << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG1_MODE_GP_STATIC_MASK  (CYVAL_CSD_LEG1_MODE_GP_STATIC)
    #define capsense_IDAC_COMP_LEG1_MODE_GP_MASK         (CYVAL_CSD_LEG1_MODE_GP << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG1_MODE_CSD_STATIC_MASK (CYVAL_CSD_LEG1_MODE_CSD_STATIC << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG1_MODE_CSD_MASK        (0x00000003Lu << CYFLD_CSD_LEG1_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG2_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_LEG2_MODE__SIZE) - 1u) << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG2_MODE_GP_STATIC_MASK  (CYVAL_CSD_LEG2_MODE_GP_STATIC)
    #define capsense_IDAC_COMP_LEG2_MODE_GP_MASK         (CYVAL_CSD_LEG2_MODE_GP << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG2_MODE_CSD_STATIC_MASK (0x00000002Lu << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_COMP_LEG2_MODE_CSD_MASK        (CYVAL_CSD_LEG2_MODE_CSD << CYFLD_CSD_LEG2_MODE__OFFSET)
    #define capsense_IDAC_COMP_DSI_CTRL_EN_MASK          (0x00200000Lu)
    #define capsense_IDAC_COMP_RANGE_MASK                (0x00C00000Lu)
    #define capsense_IDAC_COMP_RANGE_IDAC_LO_MASK        (0x00000000Lu)
    #define capsense_IDAC_COMP_RANGE_IDAC_MED_MASK       (0x00400000Lu)
    #define capsense_IDAC_COMP_RANGE_IDAC_HI_MASK        (0x00800000Lu)
    #define capsense_IDAC_COMP_LEG1_EN_MASK              (0x01000000Lu)
    #define capsense_IDAC_COMP_LEG2_EN_MASK              (0x02000000Lu)

    /* CSDv2 SENSE_PERIOD register masks  */
    #define capsense_SENSE_PERIOD_SENSE_DIV_MASK        (((0x00000001Lu << CYFLD_CSD_SENSE_DIV__SIZE) - 1u) << CYFLD_CSD_SENSE_DIV__OFFSET)
    #define capsense_SENSE_PERIOD_LFSR_SIZE_MASK        (((0x00000001Lu << CYFLD_CSD_LFSR_SIZE__SIZE) - 1u) << CYFLD_CSD_LFSR_SIZE__OFFSET)
    #define capsense_SENSE_PERIOD_LFSR_SCALE_MASK       (((0x00000001Lu << CYFLD_CSD_LFSR_SCALE__SIZE) - 1u) << CYFLD_CSD_LFSR_SCALE__OFFSET)
    #define capsense_SENSE_PERIOD_LFSR_CLEAR_MASK       (((0x00000001Lu << CYFLD_CSD_LFSR_CLEAR__SIZE) - 1u) << CYFLD_CSD_LFSR_CLEAR__OFFSET)
    #define capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK     (((0x00000001Lu << CYFLD_CSD_SEL_LFSR_MSB__SIZE) - 1u) << CYFLD_CSD_SEL_LFSR_MSB__OFFSET)
    #define capsense_SENSE_6MHZ                         (6u)
    #define capsense_SENSE_PERIOD_SENSE_DIV_6MHZ        (CYDEV_BCLK__HFCLK__KHZ/capsense_CSD_SCANSPEED_DIVIDER/capsense_SENSE_6MHZ)

    /* CSDv2 SW_BYP_SEL register masks  */
    #define capsense_SW_BYP_SEL_SW_BYA_MASK             (((0x00000001Lu << CYFLD_CSD_SW_BYA__SIZE) - 1u) << CYFLD_CSD_SW_BYA__OFFSET)
    #define capsense_SW_BYP_SEL_SW_BYB_MASK             (((0x00000001Lu << CYFLD_CSD_SW_BYB__SIZE) - 1u) << CYFLD_CSD_SW_BYB__OFFSET)
    #define capsense_SW_BYP_SEL_SW_CBCC_MASK            (((0x00000001Lu << CYFLD_CSD_SW_CBCC__SIZE) - 1u) << CYFLD_CSD_SW_CBCC__OFFSET)

    /* CSDv2 SW_REFGEN_SEL register masks  */
    #define capsense_SW_REFGEN_SEL_SW_IAIB_MASK         (((0x00000001Lu << CYFLD_CSD_SW_IAIB__SIZE) - 1u) << CYFLD_CSD_SW_IAIB__OFFSET)
    #define capsense_SW_REFGEN_SEL_SW_IBCB_MASK         (((0x00000001Lu << CYFLD_CSD_SW_IBCB__SIZE) - 1u) << CYFLD_CSD_SW_IBCB__OFFSET)
    #define capsense_SW_REFGEN_SEL_SW_SGMB_MASK         (((0x00000001Lu << CYFLD_CSD_SW_SGMB__SIZE) - 1u) << CYFLD_CSD_SW_SGMB__OFFSET)
    #define capsense_SW_REFGEN_SEL_SW_SGRE_MASK         (((0x00000001Lu << CYFLD_CSD_SW_SGRE__SIZE) - 1u) << CYFLD_CSD_SW_SGRE__OFFSET)
    #define capsense_SW_REFGEN_SEL_SW_SGR_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SGR__SIZE) - 1u) << CYFLD_CSD_SW_SGR__OFFSET)

    /* CSDv2 SW_CMP_N_SEL register masks  */
    #define capsense_SW_CMP_N_SEL_SW_SCRH_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SCRH__SIZE) - 1u) << CYFLD_CSD_SW_SCRH__OFFSET)
    #define capsense_SW_CMP_N_SEL_SW_SCRH_STATIC_OPEN   (capsense_STATIC_OPEN << CYFLD_CSD_SW_SCRH__OFFSET)
    #define capsense_SW_CMP_N_SEL_SW_SCRH_STATIC_CLOSE  (capsense_STATIC_CLOSE << CYFLD_CSD_SW_SCRH__OFFSET)
    #define capsense_SW_CMP_N_SEL_SW_SCRL_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SCRL__SIZE) - 1u) << CYFLD_CSD_SW_SCRL__OFFSET)
    #define capsense_SW_CMP_N_SEL_SW_SCRL_STATIC_OPEN   (capsense_STATIC_OPEN << CYFLD_CSD_SW_SCRL__OFFSET)
    #define capsense_SW_CMP_N_SEL_SW_SCRL_STATIC_CLOSE  (capsense_STATIC_CLOSE << CYFLD_CSD_SW_SCRL__OFFSET)

    /* CSDv2 SEQ_TIME register masks  */
    #define capsense_SEQ_TIME_AZ_TIME_MASK              (((0x00000001Lu << CYFLD_CSD_AZ_TIME__SIZE) - 1u) << CYFLD_CSD_AZ_TIME__OFFSET)

    /* CSDv2 SEQ_INIT_CNT register masks  */
    #define capsense_SEQ_INIT_CNT_CONV_CNT_MASK         (((0x00000001Lu << CYFLD_CSD_CONV_CNT__SIZE) - 1u) << CYFLD_CSD_CONV_CNT__OFFSET)
    #define capsense_SEQ_INIT_CNT_CONV_CNT_FINE_INIT    (0x00000004Lu)

    /* CSDv2 SEQ_NORM_CNT register masks  */
    #define capsense_SEQ_NORM_CNT_CONV_CNT_MASK         (((0x00000001Lu << CYFLD_CSD_CONV_CNT__SIZE) - 1u) << CYFLD_CSD_CONV_CNT__OFFSET)

    /* CSDv2 SW_RES register masks  */
    #define capsense_SW_RES_RES_HCAV_MASK               (((0x00000001Lu << CYFLD_CSD_RES_HCAV__SIZE) - 1u) << CYFLD_CSD_RES_HCAV__OFFSET)
    #define capsense_SW_RES_RES_HCAG_MASK               (((0x00000001Lu << CYFLD_CSD_RES_HCAG__SIZE) - 1u) << CYFLD_CSD_RES_HCAG__OFFSET)
    #define capsense_SW_RES_RES_HCBV_MASK               (((0x00000001Lu << CYFLD_CSD_RES_HCBV__SIZE) - 1u) << CYFLD_CSD_RES_HCBV__OFFSET)
    #define capsense_SW_RES_RES_HCBG_MASK               (((0x00000001Lu << CYFLD_CSD_RES_HCBG__SIZE) - 1u) << CYFLD_CSD_RES_HCBG__OFFSET)
    #define capsense_SW_RES_RES_F1PM_MASK               (((0x00000001Lu << CYFLD_CSD_RES_F1PM__SIZE) - 1u) << CYFLD_CSD_RES_F1PM__OFFSET)
    #define capsense_SW_RES_RES_F2PT_MASK               (((0x00000001Lu << CYFLD_CSD_RES_F2PT__SIZE) - 1u) << CYFLD_CSD_RES_F2PT__OFFSET)

    #define capsense_SW_RES_RES_DEFAULT                 ((capsense_CSD_INIT_SWITCH_RES << CYFLD_CSD_RES_HCAV__OFFSET) | \
                                                                 (capsense_CSD_SHIELD_SWITCH_RES << CYFLD_CSD_RES_HCBV__OFFSET))

    /* CSDv2 SW_HS_P_SEL register masks  */
    #define capsense_SW_HS_P_SEL_SW_HMPM_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMPM__SIZE) - 1u) << CYFLD_CSD_SW_HMPM__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPM_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HMPM__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPM_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HMPM__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPT_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMPT__SIZE) - 1u) << CYFLD_CSD_SW_HMPT__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPT_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HMPT__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPT_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HMPT__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPS_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMPS__SIZE) - 1u) << CYFLD_CSD_SW_HMPS__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPS_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HMPS__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMPS_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HMPS__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMA_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMMA__SIZE) - 1u) << CYFLD_CSD_SW_HMMA__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMA_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HMMA__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMA_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HMMA__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMB_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMMB__SIZE) - 1u) << CYFLD_CSD_SW_HMMB__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMB_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HMMB__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMMB_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HMMB__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMCA_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMCA__SIZE) - 1u) << CYFLD_CSD_SW_HMCA__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMCB_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMCB__SIZE) - 1u) << CYFLD_CSD_SW_HMCB__OFFSET)
    #define capsense_SW_HS_P_SEL_SW_HMRH_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HMRH__SIZE) - 1u) << CYFLD_CSD_SW_HMRH__OFFSET)

    /* CSDv2 SW_SHIELD_SEL register masks  */
    #define capsense_SW_SHIELD_SEL_SW_HCAV_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCAV__SIZE) - 1u) << CYFLD_CSD_SW_HCAV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCAV_STATIC_OPEN  (capsense_STATIC_OPEN)
    #define capsense_SW_SHIELD_SEL_SW_HCAV_HSCMP        (capsense_HSCMP)
    #define capsense_SW_SHIELD_SEL_SW_HCAG_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCAG__SIZE) - 1u) << CYFLD_CSD_SW_HCAG__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBV_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCBV__SIZE) - 1u) << CYFLD_CSD_SW_HCBV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBV_HSCMP        (capsense_HSCMP << CYFLD_CSD_SW_HCBV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBV_PHI2_HSCMP   (capsense_PHI2_HSCMP << CYFLD_CSD_SW_HCBV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBV_STATIC_OPEN  (capsense_STATIC_OPEN << CYFLD_CSD_SW_HCBV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBG_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCBG__SIZE) - 1u) << CYFLD_CSD_SW_HCBG__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBG_PHI1         (capsense_PHI1 << CYFLD_CSD_SW_HCBG__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBG_STATIC_OPEN  (capsense_STATIC_OPEN << CYFLD_CSD_SW_HCBG__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCCV_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCCV__SIZE) - 1u) << CYFLD_CSD_SW_HCCV__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCCG_MASK         (((0x00000001Lu << CYFLD_CSD_SW_HCCG__SIZE) - 1u) << CYFLD_CSD_SW_HCCG__OFFSET)

    /* CSDv2 SEQ_START register masks  */
    #define capsense_SEQ_START_START_MASK               (((0x00000001Lu << CYFLD_CSD_START__SIZE) - 1u) << CYFLD_CSD_START__OFFSET)
    #define capsense_SEQ_START_SEQ_MODE_MASK            (((0x00000001Lu << CYFLD_CSD_SEQ_MODE__SIZE) - 1u) << CYFLD_CSD_SEQ_MODE__OFFSET)
    #define capsense_SEQ_START_ABORT_MASK               (((0x00000001Lu << CYFLD_CSD_ABORT__SIZE) - 1u) << CYFLD_CSD_ABORT__OFFSET)
    #define capsense_SEQ_START_DSI_START_EN_MASK        (((0x00000001Lu << CYFLD_CSD_DSI_START_EN__SIZE) - 1u) << CYFLD_CSD_DSI_START_EN__OFFSET)
    #define capsense_SEQ_START_AZ0_SKIP_MASK            (((0x00000001Lu << CYFLD_CSD_AZ0_SKIP__SIZE) - 1u) << CYFLD_CSD_AZ0_SKIP__OFFSET)
    #define capsense_SEQ_START_AZ1_SKIP_MASK            (((0x00000001Lu << CYFLD_CSD_AZ1_SKIP__SIZE) - 1u) << CYFLD_CSD_AZ1_SKIP__OFFSET)

    /* CSDv2 SW_CMP_P_SEL register masks  */
    #define capsense_SW_CMP_P_SEL_SW_SFPM_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFPM__SIZE) - 1u) << CYFLD_CSD_SW_SFPM__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPM_STATIC_OPEN   (capsense_STATIC_OPEN << CYFLD_CSD_SW_SFPM__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPM_STATIC_CLOSE  (capsense_STATIC_CLOSE << CYFLD_CSD_SW_SFPM__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPT_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFPT__SIZE) - 1u) << CYFLD_CSD_SW_SFPT__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPT_STATIC_OPEN   (capsense_STATIC_OPEN << CYFLD_CSD_SW_SFPT__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPT_STATIC_CLOSE  (capsense_STATIC_CLOSE << CYFLD_CSD_SW_SFPT__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPS_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFPS__SIZE) - 1u) << CYFLD_CSD_SW_SFPS__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPS_STATIC_OPEN   (capsense_STATIC_OPEN << CYFLD_CSD_SW_SFPS__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFPS_STATIC_CLOSE  (capsense_STATIC_CLOSE << CYFLD_CSD_SW_SFPS__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFMA_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFMA__SIZE) - 1u) << CYFLD_CSD_SW_SFMA__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFMB_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFMB__SIZE) - 1u) << CYFLD_CSD_SW_SFMB__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFCA_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFCA__SIZE) - 1u) << CYFLD_CSD_SW_SFCA__OFFSET)
    #define capsense_SW_CMP_P_SEL_SW_SFCB_MASK          (((0x00000001Lu << CYFLD_CSD_SW_SFCB__SIZE) - 1u) << CYFLD_CSD_SW_SFCB__OFFSET)

    /* CSDv2 CSDCMP register masks    */
    #define capsense_CSDCMP_CSDCMP_EN_MASK              (((0x00000001Lu << CYFLD_CSD_CSDCMP_EN__SIZE) - 1u) << CYFLD_CSD_CSDCMP_EN__OFFSET)
    #define capsense_CSDCMP_POLARITY_SEL_MASK           (((0x00000001Lu << CYFLD_CSD_POLARITY_SEL__SIZE) - 1u) << CYFLD_CSD_POLARITY_SEL__OFFSET)
    #define capsense_CSDCMP_FEEDBACK_MODE_MASK          (((0x00000001Lu << CYFLD_CSD_FEEDBACK_MODE__SIZE) - 1u) << CYFLD_CSD_FEEDBACK_MODE__OFFSET)
    #define capsense_CSDCMP_AZ_EN_MASK                  (((0x00000001Lu << CYFLD_CSD_AZ_EN__SIZE) - 1u) << CYFLD_CSD_AZ_EN__OFFSET)

    /* CSDv2 SENSE_DUTY register masks  */
    #define capsense_SENSE_DUTY_SENSE_WIDTH_MASK        (((0x00000001Lu << CYFLD_CSD_SENSE_WIDTH__SIZE) - 1u) << CYFLD_CSD_SENSE_WIDTH__OFFSET)
    #define capsense_SENSE_DUTY_SENSE_POL_MASK          (((0x00000001Lu << CYFLD_CSD_SENSE_POL__SIZE) - 1u) << CYFLD_CSD_SENSE_POL__OFFSET)
    #define capsense_SENSE_DUTY_OVERLAP_PHI1_MASK       (((0x00000001Lu << CYFLD_CSD_OVERLAP_PHI1__SIZE) - 1u) << CYFLD_CSD_OVERLAP_PHI1__OFFSET)
    #define capsense_SENSE_DUTY_OVERLAP_PHI2_MASK       (((0x00000001Lu << CYFLD_CSD_OVERLAP_PHI2__SIZE) - 1u) << CYFLD_CSD_OVERLAP_PHI2__OFFSET)

    /* CSDv2 SW_AMUXBUF_SEL register masks  */
    #define capsense_SW_AMUXBUF_SEL_SW_IRBY_MASK        (((0x00000001Lu << CYFLD_CSD_SW_IRBY__SIZE) - 1u) << CYFLD_CSD_SW_IRBY__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRLB_MASK        (((0x00000001Lu << CYFLD_CSD_SW_IRLB__SIZE) - 1u) << CYFLD_CSD_SW_IRLB__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRLB_STATIC_CLOSE (capsense_STATIC_CLOSE << CYFLD_CSD_SW_IRLB__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_ICA_MASK         (((0x00000001Lu << CYFLD_CSD_SW_ICA__SIZE) - 1u) << CYFLD_CSD_SW_ICA__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_ICB_MASK         (((0x00000001Lu << CYFLD_CSD_SW_ICB__SIZE) - 1u) << CYFLD_CSD_SW_ICB__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_ICB_PHI2_HSCMP   (capsense_PHI2_HSCMP << CYFLD_CSD_SW_ICB__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRLI_MASK        (((0x00000001Lu << CYFLD_CSD_SW_IRLI__SIZE) - 1u) << CYFLD_CSD_SW_IRLI__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRH_MASK         (((0x00000001Lu << CYFLD_CSD_SW_IRH__SIZE) - 1u) << CYFLD_CSD_SW_IRH__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE (capsense_STATIC_CLOSE << CYFLD_CSD_SW_IRH__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRL_MASK         (((0x00000001Lu << CYFLD_CSD_SW_IRL__SIZE) - 1u) << CYFLD_CSD_SW_IRL__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_IRL_MASK         (((0x00000001Lu << CYFLD_CSD_SW_IRL__SIZE) - 1u) << CYFLD_CSD_SW_IRL__OFFSET)
    #define capsense_SW_AMUXBUF_SEL_SW_DEFAULT          (0x01030000Lu)
    #define capsense_SW_AMUXBUF_SEL_SW_ICB_PHI2         (0x00030000uL)

    /* CSDv2 SW_HS_N_SEL register masks  */
    #define capsense_SW_HS_N_SEL_SW_HCCC_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HCCC__SIZE) - 1u) << CYFLD_CSD_SW_HCCC__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCCD_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HCCD__SIZE) - 1u) << CYFLD_CSD_SW_HCCD__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRH_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HCRH__SIZE) - 1u) << CYFLD_CSD_SW_HCRH__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRH_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HCRH__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HCRH__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRL_MASK           (((0x00000001Lu << CYFLD_CSD_SW_HCRL__SIZE) - 1u) << CYFLD_CSD_SW_HCRL__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRL_STATIC_OPEN    (capsense_STATIC_OPEN << CYFLD_CSD_SW_HCRL__OFFSET)
    #define capsense_SW_HS_N_SEL_SW_HCRL_STATIC_CLOSE   (capsense_STATIC_CLOSE << CYFLD_CSD_SW_HCRL__OFFSET)

    /* Defining default CSDv2 configuration according to settings in customizer. */
    #define capsense_DEFAULT_CSD_CONFIG                 (capsense_CONFIG_FILTER_DELAY_2_CYCLES | capsense_CONFIG_SAMPLE_SYNC_MASK)

    /* CSDv2 CTANK masks    */
    #define capsense_CTANK_DR_VDDIO                     (0x0u)
    #define capsense_CTANK_DR_VSSIO                     (capsense_DR_MASK << capsense_CSH_SHIFT)

    /* CSDv2 IDACs' masks */
    #if(capsense_IDAC_SOURCING != capsense_CSD_IDAC_CONFIG)
        #define capsense_DEFAULT_IDAC_MOD_POLARITY      (capsense_IDAC_MOD_POLARITY_MASK)
        #define capsense_DEFAULT_IDAC_COMP_POLARITY     (capsense_IDAC_COMP_POLARITY_MASK)
        #define capsense_CSH_DR_CONFIG                  (capsense_CTANK_DR_VSSIO)
    #else
        #define capsense_DEFAULT_IDAC_MOD_POLARITY      (0x00000000Lu)
        #define capsense_DEFAULT_IDAC_COMP_POLARITY     (0x00000000Lu)
        #define capsense_CSH_DR_CONFIG                  (capsense_CTANK_DR_VDDIO)
    #endif /*  (capsense_CSD_IDAC_CONFIG == capsense_IDAC_SINK) */

    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        #if (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)
            #if (0u != capsense_TOTAL_CSD_WIDGETS)
                #define capsense_IDACB_USED             (1u)
            #else
                #define capsense_IDACB_USED             (0u)
            #endif /* (0u != capsense_TOTAL_CSD_WIDGETS) */
            #define capsense_DEFAULT_IDAC_MOD_LEG2_MODE (capsense_IDAC_MOD_LEG2_MODE_GP_STATIC_MASK)
            #define capsense_DEFAULT_IDAC_COMP_LEG1_MODE (capsense_IDAC_COMP_LEG1_EN_MASK |\
                                                                capsense_IDAC_COMP_LEG1_MODE_CSD_STATIC_MASK)
            #define capsense_DEFAULT_SW_REFGEN_SEL      (capsense_SW_REFGEN_SEL_SW_SGR_MASK |\
                                                                capsense_SW_REFGEN_SEL_SW_IAIB_MASK)
        #else
            #define capsense_IDACB_USED                 (0u)
            #define capsense_DEFAULT_IDAC_MOD_LEG2_MODE (capsense_IDAC_MOD_LEG2_EN_MASK |\
                                                                capsense_IDAC_MOD_LEG2_MODE_CSD_STATIC_MASK)
            #define capsense_DEFAULT_IDAC_COMP_LEG1_MODE (capsense_IDAC_COMP_LEG1_MODE_GP_STATIC_MASK)
            #define capsense_DEFAULT_SW_REFGEN_SEL      (capsense_SW_REFGEN_SEL_SW_SGR_MASK)
        #endif /* (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN)) */
    #else
        #define capsense_IDACB_USED                     (0u)
        #define capsense_DEFAULT_IDAC_MOD_LEG2_MODE     (capsense_IDAC_MOD_LEG2_MODE_GP_STATIC_MASK)
        #define capsense_DEFAULT_IDAC_COMP_LEG1_MODE    (capsense_IDAC_COMP_LEG1_MODE_GP_STATIC_MASK)
        #define capsense_DEFAULT_SW_REFGEN_SEL          (capsense_SW_REFGEN_SEL_SW_SGR_MASK)
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */

    #define capsense_DEFAULT_IDAC_MOD_LEG1_MODE         (capsense_IDAC_MOD_LEG1_EN_MASK |\
                                                                capsense_IDAC_MOD_LEG1_MODE_CSD_MASK)
    #define capsense_DEFAULT_IDAC_COMP_LEG2_MODE        (capsense_IDAC_COMP_LEG2_MODE_GP_STATIC_MASK)

    /* IDACs ranges */
    #if (capsense_IDAC_GAIN_LOW == capsense_CSD_IDAC_GAIN)
        #define capsense_DEFAULT_IDAC_MOD_RANGE         (capsense_IDAC_MOD_RANGE_IDAC_LO_MASK)
        #define capsense_DEFAULT_IDAC_COMP_RANGE        (capsense_IDAC_MOD_RANGE_IDAC_LO_MASK)
    #elif (capsense_IDAC_GAIN_MEDIUM == capsense_CSD_IDAC_GAIN)
        #define capsense_DEFAULT_IDAC_MOD_RANGE         (capsense_IDAC_MOD_RANGE_IDAC_MED_MASK)
        #define capsense_DEFAULT_IDAC_COMP_RANGE        (capsense_IDAC_MOD_RANGE_IDAC_MED_MASK)
    #else
        #define capsense_DEFAULT_IDAC_MOD_RANGE         (capsense_IDAC_MOD_RANGE_IDAC_HI_MASK)
        #define capsense_DEFAULT_IDAC_COMP_RANGE        (capsense_IDAC_MOD_RANGE_IDAC_HI_MASK)
    #endif /* (capsense_CSD_IDAC_GAIN == capsense_IDAC_RANGE_8X) */

    #define capsense_DEFAULT_IDAC_MOD                   (capsense_DEFAULT_IDAC_MOD_RANGE |\
                                                                capsense_DEFAULT_IDAC_MOD_POLARITY |\
                                                                capsense_DEFAULT_IDAC_MOD_LEG1_MODE |\
                                                                capsense_DEFAULT_IDAC_MOD_LEG2_MODE)

    #define capsense_DEFAULT_IDAC_COMP                  (capsense_DEFAULT_IDAC_COMP_RANGE |\
                                                                capsense_DEFAULT_IDAC_COMP_POLARITY |\
                                                                capsense_DEFAULT_IDAC_COMP_LEG1_MODE |\
                                                                capsense_DEFAULT_IDAC_COMP_LEG2_MODE)

    #define capsense_CALIBRATE_IDAC_MOD                 (capsense_DEFAULT_IDAC_MOD_RANGE |\
                                                                capsense_DEFAULT_IDAC_MOD_POLARITY |\
                                                                capsense_DEFAULT_IDAC_MOD_LEG1_MODE |\
                                                                capsense_IDAC_MOD_LEG2_MODE_GP_STATIC_MASK)

    #define capsense_CALIBRATE_IDAC_COMP                (capsense_DEFAULT_IDAC_COMP_RANGE |\
                                                                capsense_DEFAULT_IDAC_COMP_POLARITY |\
                                                                capsense_IDAC_COMP_LEG1_MODE_GP_STATIC_MASK |\
                                                                capsense_IDAC_COMP_LEG2_MODE_GP_STATIC_MASK)

    #define capsense_CALIBRATE_SW_REFGEN_SEL            (capsense_SW_REFGEN_SEL_SW_SGR_MASK)

    #define capsense_DEFAULT_REFGEN_GAIN                (capsense_CSD_GAIN << CYFLD_CSD_GAIN__OFFSET)
    #define capsense_VREF_HI_OVERSHOOT_CORRECTION       (0x00000001Lu)
    #define capsense_VREF_LOW_MAX_VALUE                 (31Lu)
    #define capsense_DEFAULT_REFGEN_VREFLO_SEL          ((capsense_VREF_LOW_MAX_VALUE - capsense_VREF_HI_OVERSHOOT_CORRECTION) \
                                                                  <<  CYFLD_CSD_VREFLO_SEL__OFFSET)

    #define capsense_REFGEN_WITH_SHIELD                 (capsense_REFGEN_REFGEN_EN_MASK |\
                                                                capsense_REFGEN_RES_EN_MASK |\
                                                                capsense_DEFAULT_REFGEN_GAIN |\
                                                                capsense_DEFAULT_REFGEN_VREFLO_SEL)

    #define capsense_REFGEN_NO_SHIELD                  (capsense_REFGEN_REFGEN_EN_MASK |\
                                                                capsense_REFGEN_RES_EN_MASK |\
                                                                capsense_DEFAULT_REFGEN_GAIN)

    #define capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT       (CYFLD_CSD_LFSR_SIZE__OFFSET)

    /* Initial PRS mode */
    #if (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_SENSE_PERIOD           (capsense_CLK_SOURCE_DIRECT)
    #elif (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_SENSE_PERIOD           (capsense_CLK_SOURCE_SSC2 << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT)
    #elif ((capsense_CLK_SOURCE_PRS8) == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_SENSE_PERIOD           ((capsense_CSD_SNS_CLK_SOURCE << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT) |\
                                                                 (capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK))
    #elif ((capsense_CLK_SOURCE_PRS12) == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_SENSE_PERIOD           ((capsense_CSD_SNS_CLK_SOURCE << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT) |\
                                                                 (capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK))
    #else
        #define capsense_DEFAULT_SENSE_PERIOD           (capsense_CSD_SNS_CLK_SOURCE << capsense_SENSE_PERIOD_LFSR_SIZE_SHIFT)
    #endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */

    /* SW_HS_P_SEL switches state for Coarse initialization of CMOD (sense path) */
    #if ((capsense_ENABLE == capsense_CSD_CSX_EN) || (capsense_ENABLE == capsense_CSD_EN))
        #if (capsense_CMOD_PAD == capsense_CMOD_CONNECTION)
            #define capsense_HS_P_SEL_COARSE_CMOD             (capsense_SW_HS_P_SEL_SW_HMPM_STATIC_CLOSE)
        #elif (capsense_CSHIELD_PAD == capsense_CMOD_CONNECTION)
            #define capsense_HS_P_SEL_COARSE_CMOD             (capsense_SW_HS_P_SEL_SW_HMPS_STATIC_CLOSE)
        #else
            #define capsense_HS_P_SEL_COARSE_CMOD             (capsense_SW_HS_P_SEL_SW_HMPT_STATIC_CLOSE)
        #endif /* (capsense_CMOD_PAD == capsense_CMOD_CONNECTION) */
    #else
        #define capsense_HS_P_SEL_COARSE_CMOD                 (0x00000000uL)
    #endif /* ((capsense_ENABLE == capsense_CSD_CSX_EN) && (capsense_ENABLE == capsense_CSD_EN)) */
    
    #if((0u != capsense_CSD_SHIELD_TANK_EN) && (capsense_ENABLE == capsense_CSD_SHIELD_EN))
        /* SW_HS_P_SEL switches state for Coarse initialization of CTANK (sense path) */
        #if (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION)
            #define capsense_HS_P_SEL_COARSE_TANK             (capsense_SW_HS_P_SEL_SW_HMPT_STATIC_CLOSE)
            #define capsense_HS_P_SEL_SCAN_TANK               (capsense_SW_HS_P_SEL_SW_HMPT_STATIC_CLOSE)
        #else
            #define capsense_HS_P_SEL_COARSE_TANK             (capsense_SW_HS_P_SEL_SW_HMMA_STATIC_CLOSE)
            #define capsense_HS_P_SEL_SCAN_TANK               (capsense_SW_HS_P_SEL_SW_HMMB_STATIC_CLOSE)
        #endif /* (capsense_CSH_TANK_PAD == capsense_CTANK_CONNECTION) */
        #define capsense_SW_HS_P_SEL_SCAN                     (capsense_HS_P_SEL_SCAN_TANK)
    #else
        #define capsense_HS_P_SEL_COARSE_TANK                 (0x00000000uL)
        #define capsense_SW_HS_P_SEL_SCAN                     (capsense_SW_HS_P_SEL_SW_HMMB_STATIC_CLOSE)
    #endif /* ((0u != capsense_CSD_CSH_TANK_ENABLE) && (capsense_ENABLE == capsense_CSD_SHIELD_EN)) */
    
    #define capsense_SW_HS_P_SEL_COARSE                 (capsense_HS_P_SEL_COARSE_CMOD | capsense_HS_P_SEL_COARSE_TANK)

    /***************************************
    * CSDv2 Registers shifts
    ***************************************/
    #define capsense_SHIELD_DELAY_SHIFT                 (CYFLD_CSD_SHIELD_DELAY__OFFSET)
    #define capsense_LFSR_SIZE_SHIFT                    (CYFLD_CSD_LFSR_SIZE__OFFSET)
    #define capsense_RESULT_VAL1_BAD_CONVS_SHIFT        (CYFLD_CSD_BAD_CONVS__OFFSET)
    #define capsense_SW_SHIELD_SEL_SW_HCBV_SHIFT        (CYFLD_CSD_SW_HCBV__OFFSET)

    /***************************************
    * LFSR Register masks
    ***************************************/
    #define capsense_RESOLUTION_OFFSET                  (0u)
    #define capsense_LFSR_TABLE_SIZE                    (4u)
    #define capsense_PRS_LENGTH_2_BITS                  (0x00000003Lu)
    #define capsense_PRS_LENGTH_3_BITS                  (0x00000007Lu)
    #define capsense_PRS_LENGTH_4_BITS                  (0x0000000FLu)
    #define capsense_PRS_LENGTH_5_BITS                  (0x0000001FLu)
    #define capsense_PRS_LENGTH_8_BITS                  (0x000000FFLu)
    #define capsense_PRS_LENGTH_12_BITS                 (0x00000FFFLu)

    #define capsense_SNSCLK_SSC2_THRESHOLD              (10u)
    #define capsense_SNSCLK_SSC3_THRESHOLD              (30u)
    #define capsense_SNSCLK_SSC4_THRESHOLD              (70u)
    #define capsense_SNSCLK_SSC5_THRESHOLD              (150u)

    #define capsense_SNSCLK_SSC2_LENGTH                 ((0x00000001Lu << 2u) - 1u)
    #define capsense_SNSCLK_SSC3_LENGTH                 ((0x00000001Lu << 3u) - 1u)
    #define capsense_SNSCLK_SSC4_LENGTH                 ((0x00000001Lu << 4u) - 1u)
    #define capsense_SNSCLK_SSC5_LENGTH                 ((0x00000001Lu << 5u) - 1u)
    
    #define capsense_HFCLK_SNSCLK_FACTOR                (160u)
    
    #define capsense_SKIP_INIT_CYCLES                   (4u)

#else

    /* CSDv1 CSD_CONFIG register masks    */
    #define capsense_CONFIG_BYPASS_SEL_MASK         (0x00000004Lu)
    #define capsense_CONFIG_FILTER_EN_MASK          (0x00000008Lu)
    #define capsense_CONFIG_DUAL_CAP_EN_MASK        (0x00000010Lu)
    #define capsense_CONFIG_PRS_CLEAR_MASK          (0x00000020Lu)
    #define capsense_CONFIG_PRS_SELECT_MASK         (0x00000040Lu)
    #define capsense_CONFIG_PRS_12_8_MASK           (0x00000080Lu)
    #define capsense_CONFIG_SHIELD_DELAY_MASK       (0x00000600Lu)
    #define capsense_CONFIG_SENSE_COMP_BW_MASK      (0x00000800Lu)
    #define capsense_CONFIG_SENSE_EN_MASK           (0x00001000Lu)
    #define capsense_CONFIG_REFBUF_EN_MASK          (0x00002000Lu)
    #define capsense_CONFIG_COMP_MODE_MASK          (0x00004000Lu)
    #define capsense_CONFIG_COMP_PIN_MASK           (0x00008000Lu)
    #define capsense_CONFIG_POLARITY_MASK           (0x00010000Lu)
    #define capsense_CONFIG_POLARITY2_MASK          (0x00020000Lu)
    #define capsense_CONFIG_MUTUALCAP_EN_MASK       (0x00040000Lu)
    #define capsense_CONFIG_SENSE_COMP_EN_MASK      (0x00080000Lu)
    #define capsense_CONFIG_REFBUF_OUTSEL_MASK      (0x00200000Lu)
    #define capsense_CONFIG_SENSE_INSEL_MASK        (0x00400000Lu)
    #define capsense_CONFIG_REFBUF_DRV_MASK         (0x01800000Lu)
    #define capsense_CONFIG_DDFTSEL_MASK            (0x1C000000Lu)
    #define capsense_CONFIG_ADFTEN_MASK             (0x20000000Lu)
    #define capsense_CONFIG_DDFT_COMP_MASK          (0x40000000Lu)
    #define capsense_CONFIG_ENABLE_MASK             (0x80000000Lu)

    /* CSDv1 CSD_IDAC register masks  */
    #define capsense_IDAC_MOD_MASK                  (0x000000FFLu)
    #define capsense_IDAC_MOD_MODE_MASK             (0x00000300Lu)
    #define capsense_IDAC_MOD_RANGE_MASK            (0x00000400Lu)
    #define capsense_IDAC_POLARITY1_MIR_MASK        (0x00001000Lu)
    #define capsense_IDAC_COMP_MASK                 (0x007F0000Lu)
    #define capsense_IDAC_COMP_MODE_MASK            (0x03000000Lu)
    #define capsense_IDAC_COMP_RANGE_MASK           (0x04000000Lu)
    #define capsense_IDAC_POLARITY2_MIR_MASK        (0x10000000Lu)
    #define capsense_IDAC_FEEDBACK_MODE_MASK        (0x40000000Lu)
    #define capsense_IDAC_MOD_MODE_OFF              (0x00000000Lu)
    #define capsense_IDAC_MOD_MODE_FIXED            (0x00000100Lu)
    #define capsense_IDAC_MOD_MODE_VARIABLE         (0x00000200Lu)
    #define capsense_IDAC_MOD_MODE_DSI              (0x00000300Lu)
    #define capsense_IDAC_COMP_MODE_OFF             (0x00000000Lu)
    #define capsense_IDAC_COMP_MODE_FIXED           (0x01000000Lu)
    #define capsense_IDAC_COMP_MODE_VARIABLE        (0x02000000Lu)
    #define capsense_IDAC_COMP_MODE_DSI             (0x03000000Lu)
    #define capsense_IDAC_MOD_VALUE_SHIFT           (0u)

    /* CSDv1 CSD_COUNTER register masks   */
    #define capsense_COUNTER_COUNTER_MASK           (0x0000FFFFLu)
    #define capsense_COUNTER_PERIOD_MASK            (0xFFFF0000Lu)

    /* CSDv1 CSD_STATUS register masks    */
    #define capsense_STATUS_CSD_CHARGE_MASK         (0x00000001Lu)
    #define capsense_STATUS_CSD_SENSE_MASK          (0x00000002Lu)
    #define capsense_STATUS_COMP_OUT_MASK           (0x00000004Lu)
    #define capsense_STATUS_SAMPLE                  (0x00000008Lu)

    /* CSDv1 CSD_INTR, CSD_INTR_SET register masks  */
    #define capsense_INTR_CSD_MASK                  (0x00000001Lu)
    #define capsense_INTR_SET_CSD_MASK              (0x00000001Lu)
    #define capsense_INTR_SET_MASK                  (capsense_INTR_SET_CSD_MASK)

    /* CSDv1 CSD_PWM register masks   */
    #define capsense_PWM_COUNT_MASK                 (0x0000000FLu)
    #define capsense_PWM_SEL_MASK                   (0x00000030Lu)

    /* CSDv1 CSD_TRIM1/2 (for IDAC) register masks    */
    #define capsense_IDAC_TRIM2_MOD_SNK_MASK        (0x0000000FLu)
    #define capsense_IDAC_TRIM2_COMP_SNK_MASK       (0x000000F0Lu)
    #define capsense_IDAC_TRIM1_MOD_SRC_MASK        (0x0000000FLu)
    #define capsense_IDAC_TRIM1_COMP_SRC_MASK       (0x000000F0Lu)

    /* CSDv1 CSD_TRIM FLASH register masks    */
    #define capsense_SFLASH_TRIM_IDAC_MOD_MASK      (0x0Fu)
    #define capsense_SFLASH_TRIM_IDAC_COMP_MASK     (0xF0u)

    /* CSDv1 Clock register masks    */
    #define capsense_SNSCLK_CMD_DIV_SHIFT           (0u)
    #define capsense_SNSCLK_CMD_PA_DIV_SHIFT        (8u)
    #define capsense_SNSCLK_CMD_DISABLE_SHIFT       (30u)
    #define capsense_SNSCLK_CMD_ENABLE_SHIFT        (31u)
    #define capsense_SNSCLK_CMD_DISABLE_MASK        ((uint32)((uint32)1u << capsense_SNSCLK_CMD_DISABLE_SHIFT))
    #define capsense_SNSCLK_CMD_ENABLE_MASK         ((uint32)((uint32)1u << capsense_SNSCLK_CMD_ENABLE_SHIFT))

    /* CSDv1 CTANK masks    */
    #define capsense_CTANK_DR_VDDIO                 (0x0u)
    #define capsense_CTANK_DR_VSSIO                 (capsense_DR_MASK << capsense_CSH_SHIFT)

    /* CSDv1 Set IDAC polarity */
    #if(capsense_CSD_IDAC_CONFIG == capsense_IDAC_SINKING)
        #define capsense_DEFAULT_IDAC_POLARITY      capsense_CONFIG_POLARITY_MASK
        #define capsense_CSH_DR_CONFIG              capsense_CTANK_DR_VDDIO
    #else
        #define capsense_DEFAULT_IDAC_POLARITY      (0u)
        #define capsense_CSH_DR_CONFIG              capsense_CTANK_DR_VSSIO
    #endif /* (capsense_CSD_IDAC_CONFIG == capsense_IDAC_SINK) */

    /* CSDv1 Initial PRS mode */
    #if (capsense_CLK_SOURCE_PRS8 == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_MODULATION_MODE    capsense_CONFIG_PRS_SELECT_MASK

    #elif (capsense_CLK_SOURCE_PRS12 == capsense_CSD_SNS_CLK_SOURCE)
        #define capsense_DEFAULT_MODULATION_MODE    (capsense_CONFIG_PRS_12_8_MASK |\
                                                            capsense_CONFIG_PRS_SELECT_MASK)
    #else
        #define capsense_DEFAULT_MODULATION_MODE    (0u)
    #endif /* (capsense_CSD_SNS_CLK_SOURCE == capsense_PRS_8BITS) */

    /* Defining default CSD configuration according to settings in customizer. */
    #define capsense_DEFAULT_CSD_CONFIG             (capsense_DEFAULT_MODULATION_MODE |\
                                                             capsense_CONFIG_SENSE_COMP_BW_MASK |\
                                                             capsense_DEFAULT_IDAC_POLARITY |\
                                                             capsense_CONFIG_SENSE_INSEL_MASK |\
                                                             capsense_CONFIG_REFBUF_DRV_MASK)

    #define capsense_CSD_ENABLE_MASK                (capsense_CONFIG_ENABLE_MASK |\
                                                             capsense_CONFIG_SENSE_EN_MASK |\
                                                             capsense_CONFIG_SENSE_COMP_EN_MASK)

    /* CSDv1 Defining mask intended for clearing bits related to pre-charging options. */
    #define capsense_PRECHARGE_CONFIG_MASK          (capsense_CONFIG_REFBUF_EN_MASK |\
                                                             capsense_CONFIG_COMP_MODE_MASK |\
                                                             capsense_CONFIG_COMP_PIN_MASK  |\
                                                             capsense_CONFIG_REFBUF_OUTSEL_MASK)

    #define capsense_CMOD_PRECHARGE_CONFIG          (capsense_DEFAULT_CSD_CONFIG |\
                                                             capsense_CONFIG_REFBUF_EN_MASK |\
                                                             capsense_CONFIG_COMP_PIN_MASK)

    #define capsense_CMOD_PRECHARGE_CONFIG_CSD_EN   (capsense_DEFAULT_CSD_CONFIG |\
                                                             capsense_CSD_ENABLE_MASK |\
                                                             capsense_CONFIG_REFBUF_EN_MASK |\
                                                             capsense_CONFIG_COMP_PIN_MASK)


    /* CSDv1 Ctank pre-charge mode configuration */
    #if(capsense_CSD_CSH_PRECHARGE_SRC == capsense_CSH_PRECHARGE_VREF)
        #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
            #define  capsense_CTANK_PRECHARGE_CONFIG    (capsense_DEFAULT_CSD_CONFIG |\
                                                                 capsense_CONFIG_REFBUF_EN_MASK |\
                                                                 capsense_CONFIG_PRS_CLEAR_MASK |\
                                                                 capsense_CONFIG_REFBUF_OUTSEL_MASK)
        #else
            #define  capsense_CTANK_PRECHARGE_CONFIG    (capsense_DEFAULT_CSD_CONFIG |\
                                                                 capsense_CONFIG_REFBUF_OUTSEL_MASK |\
                                                                 capsense_CONFIG_PRS_CLEAR_MASK)
        #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
    #else
        #define  capsense_CTANK_PRECHARGE_CONFIG        (capsense_DEFAULT_CSD_CONFIG |\
                                                                capsense_CONFIG_REFBUF_OUTSEL_MASK |\
                                                                capsense_CONFIG_REFBUF_EN_MASK |\
                                                                capsense_CONFIG_COMP_MODE_MASK |\
                                                                capsense_CONFIG_PRS_CLEAR_MASK |\
                                                                capsense_CONFIG_COMP_PIN_MASK)
    #endif /* (capsense_CSD_CSH_PRECHARGE_SRC == capsense__CSH_PRECHARGE_IO_BUF) */

    #define  capsense_CTANK_PRECHARGE_CONFIG_CSD_EN     (capsense_CTANK_PRECHARGE_CONFIG |\
                                                                 capsense_CONFIG_ENABLE_MASK |\
                                                                 capsense_CONFIG_SENSE_COMP_EN_MASK)

    #define capsense_RESOLUTION_OFFSET                  (16u)
    #define capsense_PRS_LENGTH_12_BITS                 (0x00000FFFLu)
    #define capsense_CSD_PRS_12_BIT                     (capsense_CONFIG_PRS_12_8_MASK)
    #define capsense_PRS_MODE_MASK                      (capsense_CONFIG_PRS_12_8_MASK)

    /* Set IDAC ranges */
    #if (capsense_CSD_IDAC_GAIN == capsense_IDAC_GAIN_8X)
        #define capsense_DEFAULT_IDAC_MOD_RANGE         (capsense_IDAC_MOD_RANGE_MASK)
        #define capsense_DEFAULT_IDAC_COMP_RANGE        (capsense_IDAC_COMP_RANGE_MASK)
    #else
        #define capsense_DEFAULT_IDAC_MOD_RANGE         (0u)
        #define capsense_DEFAULT_IDAC_COMP_RANGE        (0u)
    #endif /* (capsense_CSD_IDAC_GAIN == capsense_IDAC_RANGE_8X) */
    #define capsense_IDAC_COMP_DATA_OFFSET              (16u)

    /* Defining default IDACs configuration according to settings in customizer. */
    #if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
        #define capsense_DEFAULT_CSD_IDAC_CONFIG        (capsense_IDAC_MOD_MODE_VARIABLE |\
                                                                 capsense_IDAC_COMP_MODE_FIXED |\
                                                                 capsense_DEFAULT_IDAC_MOD_RANGE |\
                                                                 capsense_DEFAULT_IDAC_COMP_RANGE)
    #else
        #define capsense_DEFAULT_CSD_IDAC_CONFIG        (capsense_IDAC_MOD_MODE_VARIABLE |\
                                                                 capsense_DEFAULT_IDAC_MOD_RANGE)
    #endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */

    /***************************************
    * CSDv1 Registers shifts
    ***************************************/
    #define capsense_SHIELD_DELAY_SHIFT                 (0x09u)

#endif /* (capsense_ENABLE == capsense_CSDV2) */

    /* CSDv1 and CSDv2 Clock register masks    */
    #define capsense_MODCLK_CMD_DIV_SHIFT               (0u)
    #define capsense_MODCLK_CMD_PA_DIV_SHIFT            (8u)
    #define capsense_MODCLK_CMD_DISABLE_SHIFT           (30u)
    #define capsense_MODCLK_CMD_ENABLE_SHIFT            (31u)
    #define capsense_MODCLK_CMD_DISABLE_MASK            ((uint32)((uint32)1u << capsense_MODCLK_CMD_DISABLE_SHIFT))
    #define capsense_MODCLK_CMD_ENABLE_MASK             ((uint32)((uint32)1u << capsense_MODCLK_CMD_ENABLE_SHIFT))

/* CintB and CintA pin registers  */
#if ((capsense_ENABLE == capsense_CSX_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN))

    #define capsense_CintA_PC_REG                       (* (reg32 *) capsense_CintA__0__PC)
    #define capsense_CintA_DR_REG                       (* (reg32 *) capsense_CintA__0__DR)
    #define capsense_CintA_HSIOM_REG                    (* (reg32 *) capsense_CintA__0__HSIOM)
    #define capsense_CintB_PC_REG                       (* (reg32 *) capsense_CintB__0__PC)
    #define capsense_CintB_DR_REG                       (* (reg32 *) capsense_CintB__0__DR)
    #define capsense_CintB_HSIOM_REG                    (* (reg32 *) capsense_CintB__0__HSIOM)

    #define capsense_CintA_PC_PTR                       ((reg32 *) capsense_CintA__0__PC)
    #define capsense_CintA_DR_PTR                       ((reg32 *) capsense_CintA__0__DR)
    #define capsense_CintA_HSIOM_PTR                    ((reg32 *) capsense_CintA__0__HSIOM)
    #define capsense_CintB_PC_PTR                       ((reg32 *) capsense_CintB__0__PC)
    #define capsense_CintB_DR_PTR                       ((reg32 *) capsense_CintB__0__DR)
    #define capsense_CintB_HSIOM_PTR                    ((reg32 *) capsense_CintB__0__HSIOM)

    /* CintB and CintA pin masks                                                */
    #define capsense_CintA_HSIOM_MASK                   ((uint32)capsense_CintA__0__HSIOM_MASK)
    #define capsense_CintA_HSIOM_SHIFT                  ((uint32)capsense_CintA__0__HSIOM_SHIFT)
    #define capsense_CintA_SHIFT                        ((uint32)capsense_CintA__0__SHIFT)

    #define capsense_CintB_HSIOM_MASK                   ((uint32)capsense_CintB__0__HSIOM_MASK)
    #define capsense_CintB_HSIOM_SHIFT                  ((uint32)capsense_CintB__0__HSIOM_SHIFT)
    #define capsense_CintB_SHIFT                        ((uint32)capsense_CintB__0__SHIFT)

#endif  /* ((capsense_ENABLE == capsense_CSX_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) */

/* CMOD and CSH capacitor port-pins registers */
#if ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN))

    #define capsense_CSH_PC_REG                         (* (reg32 *) capsense_Csh__PC)
    #define capsense_CSH_PC_PTR                         (  (reg32 *) capsense_Csh__PC)
    #define capsense_CSH_DR_REG                         (* (reg32 *) capsense_Csh__DR)
    #define capsense_CSH_DR_PTR                         (  (reg32 *) capsense_Csh__DR)
    #define capsense_CSH_HSIOM_REG                      (* (reg32 *) capsense_Csh__0__HSIOM)
    #define capsense_CSH_HSIOM_PTR                      (  (reg32 *) capsense_Csh__0__HSIOM)

    #define capsense_CMOD_PC_REG                        (* (reg32 *) capsense_Cmod__PC)
    #define capsense_CMOD_PC_PTR                        (  (reg32 *) capsense_Cmod__PC)
    #define capsense_CMOD_DR_REG                        (* (reg32 *) capsense_Cmod__DR)
    #define capsense_CMOD_DR_PTR                        (  (reg32 *) capsense_Cmod__DR)
    #define capsense_CMOD_HSIOM_REG                     (* (reg32 *) capsense_Cmod__0__HSIOM)
    #define capsense_CMOD_HSIOM_PTR                     (  (reg32 *) capsense_Cmod__0__HSIOM)

    /* Cmod and Csh pin masks */
    #define capsense_CMOD_HSIOM_MASK                    capsense_Cmod__0__HSIOM_MASK
    #define capsense_CMOD_HSIOM_SHIFT                   capsense_Cmod__0__HSIOM_SHIFT
    #define capsense_CSH_HSIOM_MASK                     capsense_Csh__0__HSIOM_MASK
    #define capsense_CSH_HSIOM_SHIFT                    capsense_Csh__0__HSIOM_SHIFT
    #define capsense_CSH_SHIFT                          capsense_Csh__0__SHIFT
    #define capsense_CSH_TO_AMUXBUS_B_MASK              (0x00000007Lu)
    #define capsense_CSH_PC_MASK                        (0x00000007Lu)
    #define capsense_CSH_PC_STRONG_MODE                 (0x00000006Lu)
    #define capsense_CSH_PC_SHIFT                       (capsense_Csh__0__SHIFT * 3u)

#endif  /* ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) */

/* GPIO register masks */
#define capsense_HSIOM_SEL_MASK                         (0x0000000FLu)
#define capsense_HSIOM_SEL_GPIO                         (0x00000000Lu)
#define capsense_HSIOM_SEL_CSD_SENSE                    (0x00000004Lu)
#define capsense_HSIOM_SEL_CSD_SHIELD                   (0x00000005Lu)
#define capsense_HSIOM_SEL_AMUXA                        (0x00000006Lu)
#define capsense_HSIOM_SEL_AMUXB                        (0x00000007Lu)

#define capsense_GPIO_PC_MASK                           (0x00000007Lu)
#define capsense_GPIO_HIGHZ_MASK                        (0x00000007Lu)
#define capsense_GPIO_STRGDRV                           (0x00000006Lu)

#define capsense_SNS_GROUND_CONNECT                     (0x00000006Lu)

#define capsense_GPIO_PC_BIT_SIZE                       (3u)

/***************************************
* API Constants
***************************************/

#define capsense_MOD_CSD_CLK_12MHZ                      (12000000uL)
#define capsense_MOD_CSD_CLK_24MHZ                      (24000000uL)
#define capsense_MOD_CSD_CLK_48MHZ                      (48000000uL)

/* ISR Number to work with CyLib functions */
#define capsense_ISR_NUMBER                             (capsense_ISR__INTC_NUMBER)

/* Multi-frequency scanning constants */
#define capsense_FREQ_CHANNEL_0                         (0u)
#define capsense_FREQ_CHANNEL_1                         (1u)
#define capsense_FREQ_CHANNEL_2                         (2u)

/* Bit-mask which says that scanning is not complete */
#define capsense_SW_STS_BUSY                            (capsense_STATUS_CSD0_MASK)
#define capsense_NOT_BUSY                               (0u)

#define capsense_WDGT_SW_STS_BUSY                       (capsense_STATUS_WDGT0_BUSY_MASK)

/* Definition of time interval that is enough for charging 100nF capacitor */
#define capsense_AVG_CYCLES_PER_LOOP                    (5u)
#define capsense_MAX_CHARGE_TIME_US                     (100u)
#define capsense_SOFTWARE_WDT_CYCLES_NUM                (((CYDEV_BCLK__SYSCLK__MHZ) * (capsense_MAX_CHARGE_TIME_US)) /\
                                                                 (capsense_AVG_CYCLES_PER_LOOP))

#define capsense_PRECHARGE_IDAC_MOD_VAL                 (0xF0u)
#define capsense_CSD_IDAC_PRECHARGE_CONFIG              (capsense_IDAC_MOD_MODE_VARIABLE |\
                                                                 capsense_IDAC_MOD_RANGE_MASK |\
                                                                 capsense_IDAC_FEEDBACK_MODE_MASK |\
                                                                 capsense_PRECHARGE_IDAC_MOD_VAL)

#define capsense_CNT_RESOLUTION_12_BITS                 (0x0FFF0000Lu)
#define capsense_ONE_CYCLE                              (0x00010000Lu)
#define capsense_RESOLUTION_16_BITS                     (0xFFFF0000Lu)
#define capsense_DISCONNECT_IO_FLAG                     (1u)
#define capsense_PRESCALERS_TBL_SIZE                    (16u)

/* Calibration constants */
#define capsense_CALIBRATION_LEVEL_85                   (85u)
#define capsense_PERCENTAGE_100                         (0x00000064Lu)
#define capsense_IDAC_MOD_MAX_CALIB_ERROR_NEG           (10u)
#define capsense_IDAC_MOD_MAX_CALIB_ERROR_POS           (10u)
#define capsense_IDAC_MOD_MAX_INT_MATH_SHIFT            (16u)
#define capsense_IDAC_MOD_MAX_CALIB_ERROR_MULT          ((((capsense_PERCENTAGE_100 + capsense_IDAC_MOD_MAX_CALIB_ERROR_POS + 1u) \
                                                                 << capsense_IDAC_MOD_MAX_INT_MATH_SHIFT) / capsense_PERCENTAGE_100))
#define capsense_IDAC_MOD_MIN_CALIB_ERROR_MULT          ((((capsense_PERCENTAGE_100 - capsense_IDAC_MOD_MAX_CALIB_ERROR_NEG - 1u) \
                                                                 << capsense_IDAC_MOD_MAX_INT_MATH_SHIFT) / capsense_PERCENTAGE_100))

#if (capsense_ENABLE == capsense_CSDV2)
    #define capsense_CSD_IDAC_BITS_USED                 (7u)
#else
    #define capsense_CSD_IDAC_BITS_USED                 (8u)
#endif /* (capsense_ENABLE == capsense_CSDV2) */
#define capsense_CAL_MIDDLE_BIT                         (1u << (capsense_CSD_IDAC_BITS_USED - 1u))
#define capsense_MAX_16_BITS_VALUE                      (0x0000FFFFLu)
#define capsense_MAX_SCAN_TIME                          (capsense_MAX_16_BITS_VALUE * capsense_CSD_SCANSPEED_DIVIDER)
#define capsense_CALIBR_WATCHDOG_CYCLES_NUM             (capsense_MAX_SCAN_TIME / capsense_AVG_CYCLES_PER_LOOP)

#if (capsense_IDAC_GAIN_8X == capsense_CSD_IDAC_GAIN)
    #define capsense_CSD_IDAC_GAIN_VALUE_NA             (2400u)
#elif (capsense_IDAC_GAIN_HIGH == capsense_CSD_IDAC_GAIN) 
    #define capsense_CSD_IDAC_GAIN_VALUE_NA             (2400u)
#elif (capsense_IDAC_GAIN_4X == capsense_CSD_IDAC_GAIN)
    #define capsense_CSD_IDAC_GAIN_VALUE_NA             (1200u)
#elif (capsense_IDAC_GAIN_MEDIUM == capsense_CSD_IDAC_GAIN)
    #define capsense_CSD_IDAC_GAIN_VALUE_NA             (300u)
#else
    #define capsense_CSD_IDAC_GAIN_VALUE_NA             (37u)
#endif /* (capsense_IDAC_GAIN_8X == capsense_CSD_IDAC_GAIN) */

#if (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN)
    #define capsense_CSD_DUAL_IDAC_FACTOR               (2u)
#else
    #define capsense_CSD_DUAL_IDAC_FACTOR               (1u)
#endif /* (capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) */
    
/***************************************
* API Macros
***************************************/

#if (capsense_ENABLE == capsense_CSDV2)
    #define capsense_SET_DIRECT_CLOCK_MODE CY_SET_REG32(capsense_SENSE_PERIOD_PTR, \
                                                   CY_GET_REG32(capsense_SENSE_PERIOD_PTR) & \
                                                   (uint32)~(capsense_SENSE_PERIOD_SEL_LFSR_MSB_MASK | \
                                                   capsense_SENSE_PERIOD_LFSR_SIZE_MASK))
#else
    #define capsense_SET_DIRECT_CLOCK_MODE capsense_configCsd = capsense_configCsd &(uint32)~(capsense_CONFIG_PRS_SELECT_MASK)
#endif /* (capsense_ENABLE == capsense_CSDV2) */

/***************************************
* Global software/external variables
***************************************/
#if ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
     (capsense_ENABLE == capsense_ADC_EN))
    extern capsense_SENSE_METHOD_ENUM capsense_currentSenseMethod;
#endif /* ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
     (capsense_ENABLE == capsense_ADC_EN)) */

#if ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
     (capsense_ENABLE == capsense_CSX_EN))
extern void (*CSXPostScanApiPtr)(void);
#endif /* ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
           (capsense_ENABLE == capsense_CSX_EN)) */

#if(capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
    extern uint8 capsense_scanFreqIndex;
    extern uint8 capsense_immunity[capsense_NUM_SCAN_FREQS];
#else
    extern const uint8 capsense_scanFreqIndex;
#endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
extern volatile uint8 capsense_widgetIndex;
extern volatile uint8 capsense_sensorIndex;
extern volatile uint32 capsense_curWidgetResolution;
extern uint8 capsense_requestScanAllWidget;
extern capsense_RAM_SNS_STRUCT *capsense_curRamSnsPtr;
#if ((capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) || \
     (capsense_ENABLE == capsense_CSX_EN))
    extern capsense_FLASH_WD_STRUCT const *capsense_curFlashWdgtPtr;
#endif /* ((capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) || \
           (capsense_ENABLE == capsense_CSX_EN))  */
#if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
    /*  Pointer to Flash structure holding info of sensor to be scanned  */
    extern capsense_FLASH_SNS_STRUCT const *capsense_curFlashSnsPtr;
#endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)  */
#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
    extern uint8 capsense_prescalersTuningDone;
    extern const uint8 capsense_prescalerTable[capsense_PRESCALERS_TBL_SIZE];
#endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */
#endif /* End CY_CAPSENSE_capsense_SENSING_H */


/* [] END OF FILE */
