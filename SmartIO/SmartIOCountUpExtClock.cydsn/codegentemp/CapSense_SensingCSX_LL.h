/***************************************************************************//**
* \file CapSense_SensingCSX_LL.h
* \version 3.0
*
* \brief
*   This file provides the headers of APIs specific to CSX sensing implementation.
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

#if !defined(CapSense_SENSING_CSX_LL_H)
#define CapSense_SENSING_CSX_LL_H

#include "cytypes.h"
#include "CapSense_Structure.h"
#include "CapSense_Configuration.h"

#if (CapSense_CSX_EN == 1u)

/***************************************
* API Constants
***************************************/

/* PERI CLOCK registers */

#if defined(CYIPBLOCK_m0s8gpio2_VERSION)
    #define CapSense_IS_M0S8GPIO2_BLOCK         (1u)
    #define CapSense_M0S8GPIO2_BLOCK_VER        (CYIPBLOCK_m0s8gpio2_VERSION)
#else
    #define CapSense_IS_M0S8GPIO2_BLOCK         (0u)
    #define CapSense_M0S8GPIO2_BLOCK_VER        (0u)
#endif /* (CYIPBLOCK_m0s8gpio2_VERSION) */

#if defined(CYIPBLOCK_m0s8hsiom4a_VERSION)
    #define CapSense_IS_M0S8HSIOM4A_BLOCK       (1u)
    #define CapSense_M0S8HSIOM4A_BLOCK_VER      (CYIPBLOCK_m0s8hsiom4a_VERSION)
#else
    #define CapSense_IS_M0S8HSIOM4A_BLOCK       (0u)
    #define CapSense_M0S8HSIOM4A_BLOCK_VER      (0u)
#endif /* (CYIPBLOCK_m0s8hsiom4a_VERSION) */

#if defined(CYIPBLOCK_m0s8ioss_VERSION)
    #define CapSense_IS_M0S8IOSS_BLOCK          (1u)
    #define CapSense_M0S8IOSS_BLOCK_VER         (CYIPBLOCK_m0s8ioss_VERSION)
#else
    #define CapSense_IS_M0S8IOSS_BLOCK          (0u)
    #define CapSense_M0S8IOSS_BLOCK_VER         (0u)
#endif /* (CYIPBLOCK_m0s8ioss_VERSION) */

#if defined(CYIPBLOCK_m0s8pclk_VERSION)
    #define CapSense_IS_M0S8PCLK_BLOCK          (1u)
    #define CapSense_M0S8PCLK_BLOCK_VER         (CYIPBLOCK_m0s8pclk_VERSION)
#else
    #define CapSense_IS_M0S8PCLK_BLOCK          (0u)
    #define CapSense_M0S8PCLK_BLOCK_VER         (0u)
#endif /* (CYIPBLOCK_m0s8pclk_VERSION) */

#if defined(CYIPBLOCK_m0s8peri_VERSION)
    #define CapSense_IS_M0S8PERI_BLOCK          (1u)
    #define CapSense_M0S8PERI_BLOCK_VER         ((CYIPBLOCK_m0s8peri_VERSION))
#else
    #define CapSense_IS_M0S8PERI_BLOCK          (0u)
    #define CapSense_M0S8PERI_BLOCK_VER         (0u)
#endif /* (CYIPBLOCK_m0s8peri_VERSION) */

#if defined(CYIPBLOCK_m0s8csd_VERSION)
    #define CapSense_IS_M0S8CSD_BLOCK           (1u)
    #define CapSense_M0S8CSD_BLOCK_VER          (CYIPBLOCK_m0s8csd_VERSION)
#else
    #define CapSense_IS_M0S8CSD_BLOCK           (0u)
    #define CapSense_M0S8CSD_BLOCK_VER          (0u)
#endif /* (CYIPBLOCK_m0s8csd_VERSION) */



#define CapSense_GPIO_PC_BIT_SIZE               (3u)


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
* \if SECTION_CAPSENSE_LOW_LEVEL
* \addtogroup group_capsense_low_level
* \{
*/

void CapSense_CSXInitialize(void);
void CapSense_CSXSetupWidget(uint32 widgetIndex);
void CapSense_CSXSetupWidgetExt(uint32 widgetIndex, uint32 snsIndex);
void CapSense_CSXScan (void);
void CapSense_CSXScanExt (void);

#if (CapSense_CSX_IDAC_AUTOCAL_EN == CapSense_ENABLE)
    void CapSense_CSXCalibrateWidget(uint32 widgetIndex, uint16 target);
#endif /* (CapSense_CSX_IDAC_AUTOCAL_EN == CapSense_ENABLE) */

void CapSense_CSXConnectTx(CapSense_FLASH_IO_STRUCT const *txPtr);
void CapSense_CSXConnectRx(CapSense_FLASH_IO_STRUCT const *rxPtr);
void CapSense_CSXDisconnectTx(CapSense_FLASH_IO_STRUCT const *txPtr);
void CapSense_CSXDisconnectRx(CapSense_FLASH_IO_STRUCT const *rxPtr);

/** \}
* \endif */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

void CapSense_CSXElectrodeCheck(void);

/** \}
* \endif */

/*******************************************************************************
* Global variables exposed by the module
*******************************************************************************/

/*  Local variable used to store the Scan period */
extern volatile uint16 CapSense_csxScanPeriod;

/*  Local variable used to return RawCount from the post scan ISR */
extern volatile uint16 CapSense_csxRawCount;

#if(CapSense_ENABLE == CapSense_CSDV2)
    extern volatile uint32 CapSense_resamplingCyclesCnt;
#else
    /*  Local variable used by ASM function for raw count correction */
    extern volatile uint32 CapSense_csxRawGarbage;
#endif /* (CapSense_ENABLE == CapSense_CSDV2) */

/*  Pointer to function used to register callback in ISR */
extern void (* CapSense_CSXPostScanApiPtr)(void);

/*******************************************************************************
* External APIs required by the module
*******************************************************************************/
#if(CapSense_ENABLE != CapSense_CSDV2)
    /*  ASM function called from C, CSX ISR proto */
    extern void CapSense_SsCSXStartSampleExt(void);
#endif /* (CapSense_ENABLE != CapSense_CSDV2) */

/* ISR for CSX sensing module      */
CY_ISR_PROTO(CapSense_SsCSXScanISR);

#if(CapSense_ENABLE == CapSense_CSDV2)
/*******************************************************************************
* Function constants
*******************************************************************************/

/*
   +--------+---------------+-----------------------------+-----------------------------+
   |        |               | BLOCK_OFF_AFTER_SCAN=TRUE   | BLOCK_OFF_AFTER_SCAN=FALSE  |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |              MODE           |
   |--------|---------------|-----------------------------|-----------------------------|
   |  6:4   | FILTER_DELAY  | 0x02(2_SAMPLE_CYCLES)       | 0x02(2_SAMPLE_CYCLES)       |
   |  9:8   | SHIELD_DELAY  | 0x00(OFF)                   | 0x00(OFF)                   |
   |  12    | SENSE_EN      | 0x00(DISABLED)              | 0x01(ENABLED)               |
   |  14    | CHARGE_MODE   | 0x00(CHARGE_OFF)            | 0x00(CHARGE_OFF)            |
   |  18    | MUTUAL_CAP    | 0x01(MUTUALCAP)             | 0x01(MUTUALCAP)             |
   |  19    | CSX_DUAL_CNT  | 0x01(TWO)                   | 0x01(TWO)                   |
   |  24    | DSI_COUNT_SEL | 0x00(CSD_RESULT)            | 0x00(CSD_RESULT)            |
   |  25    | DSI_SAMPLE_EN | 0x00(DISABLED)              | 0x00(DISABLED)              |
   |  26    | SAMPLE_SYNC   | 0x00(DISABLED)              | 0x00(DISABLED)              |
   |  27    | DSI_SENSE_EN  | 0x00(DISABLED)              | 0x00(DISABLED)              |
   |  30    | LP_MODE       | 0x00(HIGH_POWER)            | 0x00(HIGH_POWER)            |
   |  31    | ENABLE        | 0x00(DISABLED)              | 0x01(ENABLED)               |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(0u != CapSense_BLOCK_OFF_AFTER_SCAN_EN)
    #define CapSense_DEFAULT_CSD_CONFIG_CFG        (0x00000020uL)
#else
    #define CapSense_DEFAULT_CSD_CONFIG_CFG        (0x80041020uL)
#endif /* (0u != CapSense_BLOCK_OFF_AFTER_SCAN_EN) */

/* +--------+---------------+--------------------------------------------+
   |  BITS  |   FIELD       |              MODE                          |
   |--------|---------------|--------------------------------------------|
   | 1      | SAMPLE        | 0x01(CLEAR_CSD_EOS_INTERRUPT)              |
   | 2      | INIT          | 0x01(CLEAR_END_OF_COARSE_INIT_INTERRUPT)   |
   | 8      | ADC_RES       | 0x01(CLEAR_ADC_EOS_INTERRUPT               |
   +--------+---------------+--------------------------------------------+ */
#define CapSense_DEFAULT_CSD_INTR_CFG              (0x00000106uL)

/* +--------+---------------+--------------------------------------------+
   |  BITS  |   FIELD       |              MODE                          |
   |--------|---------------|--------------------------------------------|
   | 1      | SAMPLE        | 0x00(CSD_EOS_INTERRUPT_CLEARED)            |
   | 2      | INIT          | 0x00(END_OF_COARSE_INIT_INTERRUPT_CLEARED) |
   | 8      | ADC_RES       | 0x00(ADC_EOS_INTERRUPT_CLEARED)            |
   +--------+---------------+--------------------------------------------+*/
#define CapSense_DEFAULT_CSD_INTR_SET_CFG          (0x00000000uL)

/* +--------+---------------+--------------------------------------------+
   |  BITS  |   FIELD       |              MODE                          |
   |--------|---------------|--------------------------------------------|
   | 1      | SAMPLE        | 0x00(CSD_EOS_INTERRUPT_UNMASKED)           |
   | 2      | INIT          | 0x00(END_OF_COARSE_INIT_INTERRUPT_UNMASKED)|
   | 8      | ADC_RES       | 0x00(ADC_EOS_INTERRUPT_UNMASKED)           |
   +--------+---------------+--------------------------------------------+*/
#define CapSense_DEFAULT_CSD_INTR_MASK_CFG         (0x00000002uL)

/* +--------+---------------+--------------------------------------------+
   |  BITS  |   FIELD       |              MODE                          |
   |--------|---------------|--------------------------------------------|
   |  0     |  HSCMP_EN     | 0x00(OFF)                                  |
   |  4     |  HSCMP_INVERT | 0x00(NON_INVERT)                           |
   |  31    |  AZ_EN        | 0x00(DISABLED)                             |
   +--------+---------------+--------------------------------------------+*/
#define CapSense_DEFAULT_CSD_HSCMP_CFG            (0x00000000uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |           VDDA >= 2V        |          VDDA <2V           |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  1:0   | PWR_MODE      | 0x00(OFF)                   | 0x01(NORMAL)                |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if (CapSense_CYDEV_VDDA_MV >= 2000u)
    #define CapSense_DEFAULT_CSD_AMBUF_CFG           (0x00000000uL)
#else
    #define CapSense_DEFAULT_CSD_AMBUF_CFG           (0x00000001uL)
#endif /* (CYDEV_VDDA_MV >= 2000u) */

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |           VDDA >= 2V        |          VDDA <2V           |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  0     | REFGEN_EN     | 0x01(ON)                    | 0x01(ON)                    |
   |  4     | BYPASS        | 0x00(OFF)                   | 0x01(ON)                    |
   |  5     | VDDA_EN       | 0x00(OFF)                   | 0x00(OFF)                   |
   |  6     | RES_EN        | 0x01(ENABLED)               | 0x01(ENABLED)               |
   |  12:08 | GAIN          | 0x10                        | 0x10                        |
   |  20:16 | VREFLO_SEL    | 0x00                        | 0x00                        |
   |  23    | VREFHI_SEL    | 0x00                        | 0x00                        |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if (CapSense_CYDEV_VDDA_MV >= 2000u)
    #define CapSense_DEFAULT_CSD_REFGEN_CFG          (0x001E1F41uL)
#else
    #define CapSense_DEFAULT_CSD_REFGEN_CFG          (0x001E1F41uL)
#endif /* (CYDEV_VDDA_MV >= 2000u) */

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |        AUTO_ZERO_EN=TRUE    |       AUTO_ZERO_EN=FALSE    |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  0     | CSDCMP_EN     | 0x00(OFF)                   | 0x00(OFF)                   |
   |  5:04  | POLARITY_SEL  | 0x00(IDACA_POL)             | 0x00(IDACA_POL)             |
   |  9:08  | CMP_PHASE     | 0x03(PHI1_2)                | 0x03(PHI1_2)                |
   |  28    | CMP_MODE      | 0x00(CSD)                   | 0x00(CSD)                   |
   |  29    | FEEDBACK_MODE | 0x00(FLOP)                  | 0x00(FLOP)                  |
   |  31    | AZ_EN         | 0x01(ENABLED)               | 0x00(DISABLED)              |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_DEFAULT_CSD_CSDCMP_CFG          (0x80000300uL)
#else
    #define CapSense_DEFAULT_CSD_CSDCMP_CFG          (0x00000300uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 6:0    | VAL           | 0x00                                        |
   | 7      | POL_DYN       | 0x01                                        |
   | 9:8    | POLARITY      | 0x02                                        |
   | 11:10  | BAL_MODE      | 0x03                                        |
   | 17:16  | LEG1_MODE     | 0x03                                        |
   | 19:18  | LEG2_MODE     | 0x00                                        |
   | 21     | DSI_CTRL_EN   | 0x00                                        |
   | 23:22  | RANGE         | CSX_IDAC_GAIN                               |
   | 24     | LEG1_EN       | 0x01                                        |
   | 25     | LEG2_EN       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_IDACA_CFG           ((0x01030E80uL) | ((CapSense_CSX_IDAC_GAIN << CYFLD_CSD_RANGE__OFFSET) &\
                                                                                                     CapSense_IDAC_MOD_RANGE_MASK))

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 6:0    | VAL           | 0x00                                        |
   | 7      | POL_DYN       | 0x00                                        |
   | 9:8    | POLARITY      | 0x00                                        |
   | 11:10  | BAL_MODE      | 0x00                                        |
   | 17:16  | LEG1_MODE     | 0x00                                        |
   | 19:18  | LEG2_MODE     | 0x00                                        |
   | 21     | DSI_CTRL_EN   | 0x00                                        |
   | 23:22  | RANGE         | 0x00                                        |
   | 24     | LEG1_EN       | 0x00                                        |
   | 25     | LEG2_EN       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_IDACB_CFG           (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   +--------+---------------+---------------------------------------------+
   | 1:0    | RES_HCAV      | 0x00(LOW)                                   |
   | 3:2    | RES_HCAG      | 0x00                                        |
   | 5:4    | RES_HCBV      | 0x00                                        |
   | 7:6    | RES_HCBG      | 0x00                                        |
   | 17:16  | RES_F1PM      | 0x00(LOW)                                   |
   | 19:18  | RES_F2PT      | 0x00(LOW)                                   |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_RES_CFG          (0x00000000uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |      TX_CLK_SOURCE=PRSAUTO  |     TX_CLK_SOURCE!=PRSAUTO  |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  11:00 | SENSE_DIV     | CSX_TX_CLK_DIVIDER          | CSX_TX_CLK_DIVIDER          |
   |  18:16 | LFSR_SIZE     | 0x00                        | 0x00                        |
   |  23:20 | LFSR_SCALE    | 0x00                        | CSX_TX_CLK_SOURCE           |
   |  24    | LFSR_CLEAR    | 0x00                        | 0x00                        |
   |  25    | SEL_LFSR_MSB  | 0x00                        | 0x00                        |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(CapSense_CSX_TX_CLK_SOURCE != CapSense_CLK_SOURCE_PRSAUTO)
    #define CapSense_DEFAULT_CSD_SENSE_PERIOD_CFG    ((CapSense_CSX_TX_CLK_SOURCE << CapSense_SENSE_PERIOD_LFSR_SIZE_SHIFT) |\
                                                              (CapSense_CSX_TX_CLK_DIVIDER & CapSense_SENSE_PERIOD_SENSE_DIV_MASK))
#else
    #define CapSense_DEFAULT_CSD_SENSE_PERIOD_CFG    (CapSense_CSX_TX_CLK_DIVIDER & CapSense_SENSE_PERIOD_SENSE_DIV_MASK)
#endif /* (CapSense_CSX_TX_CLK_SOURCE != CapSense_CLK_SOURCE_PRSAUTO) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 11:0   | SENSE_WIDTH   | 0x0000(0_SENSE_CYCLES)                      |
   | 16     | SENSE_POL     | 0x00(START_WITH_LOW_PHASE)                  |
   | 18     | OVERLAP_PHI1  | 0x00(NON_OVERLAP)                           |
   | 19     | OVERLAP_PHI2  | 0x00(NON_OVERLAP)                           |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SENSE_DUTY_CFG      (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | SW_HMPM       | 0x00(SWITCH_OPEN)                           |
   | 4      | SW_HMPT       | 0x00(SWITCH_OPEN)                           |
   | 8      | SW_HMPS       | 0x00(SWITCH_OPEN)                           |
   | 12     | SW_HMMA       | 0x00(SWITCH_OPEN)                           |
   | 16     | SW_HMMB       | 0x00(SWITCH_OPEN)                           |
   | 20     | SW_HMCA       | 0x00(SWITCH_OPEN)                           |
   | 24     | SW_HMCB       | 0x00(SWITCH_OPEN)                           |
   | 28     | SW_HMRH       | 0x00(SWITCH_OPEN)                           |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_HS_P_SEL_CFG     (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 16     | SW_HCCC       | 0x00(SWITH_OPEN)                            |
   | 20     | SW_HCCD       | 0x00(SWITH_OPEN)                            |
   | 26:24  | SW_HCRH       | 0x00                                        |
   | 30:28  | SW_HCRL       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_HS_N_SEL_CFG     (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 2:0    | SW_HCAV       | 0x0000(STATIC_OPEN)                         |
   | 6:4    | SW_HCAG       | 0x00                                        |
   | 10:8   | SW_HCBV       | 0x00                                        |
   | 14:12  | SW_HCBG       | 0x00                                        |
   | 16     | SW_HCCV       | 0x0(SWITCH_OPEN)                            |
   | 20     | SW_HCCG       | 0x0(SWITCH_OPEN)                            |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_SHIELD_SEL_CFG   (0x00000000uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |           VDDA >= 2V        |          VDDA < 2V          |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  4     | SW_IRBY       | 0x00                        | 0x00                        |
   |  8     | SW_IRLB       | 0x00                        | 0x01                        |
   |  12    | SW_ICA        | 0x00                        | 0x00                        |
   |  18:16 | SW_ICB        | 0x00                        | 0x00                        |
   |  20    | SW_IRLI       | 0x00                        | 0x00                        |
   |  24    | SW_IRH        | 0x00                        | 0x01                        |
   |  28    | SW_IRL        | 0x00                        | 0x00                        |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if (CapSense_CYDEV_VDDA_MV >= 2000u)
    #define CapSense_DEFAULT_CSD_SW_AMUXBUF_SEL_CFG  (0x00000000uL)
#else
    #define CapSense_DEFAULT_CSD_SW_AMUXBUF_SEL_CFG  (0x01000100uL)
#endif /* (CYDEV_VDDA_MV >= 2000u) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 12     | SW_BYA        | 0x01(SWITH_CLOSED)                          |
   | 16     | SW_BYB        | 0x00(SWITH_OPEN)                            |
   | 20     | SW_CBCC       | 0x00(SWITH_OPEN)                            |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_BYP_SEL_CFG      (0x00001000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 2:0    | SW_SFPM       | 0x02                                        |
   | 6:4    | SW_SFPT       | 0x03                                        |
   | 10:8   | SW_SFPS       | 0x00                                        |
   | 12     | SW_SFMA       | 0x00(SWITH_OPEN)                            |
   | 16     | SW_SFMB       | 0x00(SWITH_OPEN)                            |
   | 20     | SW_SFCA       | 0x00(SWITH_OPEN)                            |
   | 24     | SW_SFCB       | 0x00(SWITH_OPEN)                            |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_CMP_P_SEL_CFG    (0x00000032uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |           VDDA >= 2V        |          VDDA <2V           |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  26:24 | SW_SCRH       |  0x01                       |  0x00                       |
   |  30:28 | SW_SCRL       |  0x00                       |  0x01                       |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if (CapSense_CYDEV_VDDA_MV >= 2000u)
    #define CapSense_DEFAULT_CSD_SW_CMP_N_SEL_CFG    (0x01000000uL)
#else
    #define CapSense_DEFAULT_CSD_SW_CMP_N_SEL_CFG    (0x10000000uL)
#endif /* (CYDEV_VDDA_MV >= 2000u) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | SW_IAIB       | 0x00(SWITH_OPEN)                            |
   | 4      | SW_IBCB       | 0x00(SWITH_OPEN)                            |
   | 16     | SW_SGMB       | 0x00(SWITH_OPEN)                            |
   | 24     | SW_SGRE       | 0x00(SWITH_OPEN)                            |
   | 28     | SW_SGR        | 0x01(SWITH_CLOSED)                          |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_REFGEN_SEL_CFG   (0x10000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | SW_F1PM       | 0x00(SWITH_OPEN)                            |
   | 10:8   | SW_F1MA       | 0x00                                        |
   | 18:16  | SW_F1CA       | 0x00                                        |
   | 20     | SW_C1CC       | 0x00(SWITH_OPEN)                            |
   | 24     | SW_C1CD       | 0x00(SWITH_OPEN)                            |
   | 28     | SW_C1F1       | 0x00(SWITH_OPEN)                            |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_FW_MOD_SEL_CFG   (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 4      | SW_F2PT       | 0x00                                        |
   | 10:8   | SW_F2MA       | 0x00                                        |
   | 14:12  | SW_F2CA       | 0x00                                        |
   | 18:16  | SW_F2CB       | 0x00                                        |
   | 20     | SW_C2CC       | 0x00                                        |
   | 24     | SW_C2CD       | 0x00                                        |
   | 28     | SW_C2F2       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_FW_TANK_SEL_CFG  (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 2:0    | DSI_CSH_TANK  | 0x03                                        |
   | 6:4    | DSI_CMOD      | 0x02                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SW_DSI_SEL_CFG      (0x00000023uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 7:0    | AZ_TIME       | AUTO_ZERO_TIME                              |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_TIME_CFG        (CapSense_CSX_AUTO_ZERO_TIME &\
                                                          CapSense_SEQ_INIT_CNT_CONV_CNT_MASK)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 15:0   | CONV_CNT      | 0x0014(20_CONVERSIONS)                      |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_INIT_CNT_CFG    (0x00000014uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 15:0   | CONV_CNT      | 0x0000(0_CONVERSIONS)                       |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_SEQ_NORM_CNT_CFG    (0x00000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 7:0    | ADC_TIME      | 0x00(0_SENSE_CYCLES)                        |
   | 17:16  | ADC_MODE      | 0b00(OFF)                                   |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_DEFAULT_CSD_ADC_CTL_CFG         (0x00000000uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |      AUTO_ZERO_EN=TRUE      |     AUTO_ZERO_EN=TRUE       |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  0     | START         | 0x00(STOPPED)               | 0x00(STOPPED)               |
   |  1     | SEQ_MODE      | 0x00(REGULAR_CSD_SCAN)      | 0x00(REGULAR_CSD_SCAN)      |
   |  3     | ABORT         | 0x01(RESET_TO_IDLE_STATE)   | 0x01(RESET_TO_IDLE_STATE)   |
   |  4     | DSI_START_EN  | 0x00(DISABLE_CTRL_BY_DSI)   | 0x00(DISABLE_CTRL_BY_DSI)   |
   |  8     | AZ0_SKIP      | 0x00(DO_NOT_SKIP_AZ0)       | 0x01(SKIP_AZ0)              |
   |  9     | AZ1_SKIP      | 0x00(DO_BOT_SKIP_AZ1)       | 0x01(SKIP_AZ1)              |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_DEFAULT_CSD_SEQ_START_CFG       (0x00000008uL)
#else
    #define CapSense_DEFAULT_CSD_SEQ_START_CFG       (0x00000308uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   |  6:4   | FILTER_DELAY  | 0x02(2_SAMPLE_CYCLES)                       |
   |  9:8   | SHIELD_DELAY  | 0x00(OFF)                                   |
   |  12    | SENSE_EN      | 0x01(ENABLED)                               |
   |  14    | CHARGE_MODE   | 0x00(CHARGE_OFF)                            |
   |  18    | MUTUAL_CAP    | 0x01(MUTUALCAP)                             |
   |  19    | CSX_DUAL_CNT  | 0x01(TWO)                                   |
   |  24    | DSI_COUNT_SEL | 0x00(CSD_RESULT)                            |
   |  25    | DSI_SAMPLE_EN | 0x00(DISABLED)                              |
   |  26    | SAMPLE_SYNC   | 0x00(DISABLED)                              |
   |  27    | DSI_SENSE_EN  | 0x00(DISABLED)                              |
   |  30    | LP_MODE       | 0x00(HIGH_POWER_MODE)                       |
   |  31    | ENABLE        | 0x01(ENABLED)                               |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRESCAN_CSD_CONFIG_CFG          (0x800C1000uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |    AUTO_ZERO_EN=TRUE        |    AUTO_ZERO_EN=FALSE       |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  0     | CSDCMP_EN     | 0x01(ON)                    | 0x01(ON)                    |
   |  5:4   | POLARITY_SEL  | 0x00(IDACA_POL)             | 0x00(IDACA_POL)             |
   |  9:8   | CMP_PHASE     | 0x03(PHI1_2)                | 0x03(PHI1_2)                |
   |  28    | CMP_MODE      | 0x00(CSD)                   | 0x00(CSD)                   |
   |  29    | FEEDBACK_MODE | 0x00(FLOP)                  | 0x00(FLOP)                  |
   |  31    | AZ_EN         | 0x01(ENABLED)               | 0x00(DISABLED)              |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_PRESCAN_CSD_CSDCMP_CFG      (0x80000301uL)
#else
    #define CapSense_PRESCAN_CSD_CSDCMP_CFG      (0x00000301uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN) */

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   |  0     |  HSCMP_EN     | 0x01(ON)                                    |
   |  4     |  HSCMP_INVERT | 0x00(NON_INVERT)                            |
   |  31    |  AZ_EN        | 0x00(DISABLED)                              |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_HSCMP_CFG             (0x00000001uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | SW_HMPM       | 0x01(SWITCH_CLOSED)                         |
   | 4      | SW_HMPT       | 0x01(SWITCH_CLOSED)                         |
   | 8      | SW_HMPS       | 0x00(SWITCH_OPEN)                           |
   | 12     | SW_HMMA       | 0x00(SWITCH_OPEN)                           |
   | 16     | SW_HMMB       | 0x00(SWITCH_OPEN)                           |
   | 20     | SW_HMCA       | 0x00(SWITCH_OPEN)                           |
   | 24     | SW_HMCB       | 0x00(SWITCH_OPEN)                           |
   | 28     | SW_HMRH       | 0x00(SWITCH_OPEN)                           |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_HS_P_SEL_CFG     (0x00000011uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 16     | SW_HCCC       | 0x00                                        |
   | 20     | SW_HCCD       | 0x00                                        |
   | 26:24  | SW_HCRH       | 0x01                                        |
   | 30:28  | SW_HCRL       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_HS_N_SEL_CFG     (0x01000000uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 2:0    | DSI_CSH_TANK  | 0x01                                        |
   | 6:4    | DSI_CMOD      | 0x01                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_DSI_SEL_CFG      (0x00000011uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 2:0    | SW_HCAV       | 0x06                                        |
   | 6:4    | SW_HCAG       | 0x00                                        |
   | 10:8   | SW_HCBV       | 0x00                                        |
   | 14:12  | SW_HCBG       | 0x00                                        |
   | 16     | SW_HCCV       | 0x00                                        |
   | 20     | SW_HCCG       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_SHIELD_SEL_CFG   (0x00000006uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | SW_F1PM       | 0x00(SWITH_OPEN)                            |
   | 10:8   | SW_F1MA       | 0x00                                        |
   | 18:16  | SW_F1CA       | 0x00                                        |
   | 20     | SW_C1CC       | 0x00(SWITH_OPEN)                            |
   | 24     | SW_C1CD       | 0x00(SWITH_OPEN)                            |
   | 28     | SW_C1F1       | 0x00(SWITH_OPEN)                            |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_FW_MOD_SEL_CFG   (0x00010101uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 4      | SW_F2PT       | 0x00                                        |
   | 10:8   | SW_F2MA       | 0x00                                        |
   | 14:12  | SW_F2CA       | 0x00                                        |
   | 18:16  | SW_F2CB       | 0x00                                        |
   | 20     | SW_C2CC       | 0x00                                        |
   | 24     | SW_C2CD       | 0x00                                        |
   | 28     | SW_C2F2       | 0x00                                        |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SW_FW_TANK_SEL_CFG  (0x00001010uL)

/* +--------+---------------+---------------------------------------------+
   |  BITS  |   FIELD       |              MODE                           |
   |--------|---------------|---------------------------------------------|
   | 0      | START         | 0x01(STARTED)                               |
   | 1      | SEQ_MODE      | 0x01(INIT_COARSE)                           |
   | 3      | ABORT         | 0x00(RESET_TO_IDLE_STATE)                   |
   | 4      | DSI_START_EN  | 0x00(DISABLE_CONTROL_BY_DSI)                |
   | 8      | AZ0_SKIP      | 0x00(DO_NOT_SKIP_AZ0)                       |
   | 9      | AZ1_SKIP      | 0x00(DO_BOT_SKIP_AZ1)                       |
   +--------+---------------+---------------------------------------------+*/
#define CapSense_PRECHARGE_CSD_SEQ_START_CFG       (0x00000303uL)

 /*+--------+---------------+-----------------------------+-----------------------------+
   |        |               |      AUTO_ZERO_EN=TRUE      |     AUTO_ZERO_EN=TRUE       |
   +  BITS  +   FIELD       +-----------------------------+-----------------------------+
   |        |               |              MODE           |             MODE            |
   |--------|---------------|-----------------------------|-----------------------------|
   |  0     | START         | 0x01(STARTED)               | 0x01(STARTED)               |
   |  1     | SEQ_MODE      | 0x00(REGULAR_CSD_SCAN)      | 0x00(REGULAR_CSD_SCAN)      |
   |  3     | ABORT         | 0x00(RESET_TO_IDLE_STATE)   | 0x00(RESET_TO_IDLE_STATE)   |
   |  4     | DSI_START_EN  | 0x00(DISABLE_CTRL_BY_DSI)   | 0x00(DISABLE_CTRL_BY_DSI)   |
   |  8     | AZ0_SKIP      | 0x00(DO_NOT_SKIP_AZ0)       | 0x01(SKIP_AZ0)              |
   |  9     | AZ1_SKIP      | 0x00(DO_BOT_SKIP_AZ1)       | 0x01(SKIP_AZ1)              |
   +--------+---------------+-----------------------------+-----------------------------+ */
#if(0u != CapSense_CSX_AUTO_ZERO_EN)
    #define CapSense_SCAN_CSD_SEQ_START_CFG   (0x00000001uL)
#else
    #define CapSense_SCAN_CSD_SEQ_START_CFG   (0x00000301uL)
#endif /* (0u != CapSense_CSX_AUTO_ZERO_EN)  */

#define CapSense_CSX_DEADBAND_CYCLES_NUMBER   (2u)

#define CapSense_MOD_CSD_CLK_12MHZ                      (12000000uL)
#define CapSense_MOD_CSD_CLK_24MHZ                      (24000000uL)
#define CapSense_MOD_CSD_CLK_48MHZ                      (48000000uL)

#define CapSense_CONFIG_FILTER_DELAY_MASK           (((0x00000001Lu << CYFLD_CSD_FILTER_DELAY__SIZE) - 1u) << CYFLD_CSD_FILTER_DELAY__OFFSET)
#define CapSense_CONFIG_FILTER_DELAY_2_CYCLES       (0x00000002Lu)
#define CapSense_CONFIG_FILTER_DELAY_3_CYCLES       (0x00000003Lu)
#define CapSense_CONFIG_FILTER_DELAY_4_CYCLES       (0x00000004Lu)
#define CapSense_CONFIG_FILTER_DELAY_12MHZ          (CapSense_CONFIG_FILTER_DELAY_2_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)
#define CapSense_CONFIG_FILTER_DELAY_24MHZ          (CapSense_CONFIG_FILTER_DELAY_3_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)
#define CapSense_CONFIG_FILTER_DELAY_48MHZ          (CapSense_CONFIG_FILTER_DELAY_4_CYCLES << CYFLD_CSD_FILTER_DELAY__OFFSET)

/* Definition of time interval long enough for charging 100nF capacitor */
#define CapSense_CSX_MAX_CHARGE_TIME_US             (100u)
#define CapSense_CSX_AVG_CYCLES_PER_LOOP            (5u)
#define CapSense_CSX_PRECHARGE_WATCHDOG_CYCLES_NUM  (((CYDEV_BCLK__SYSCLK__MHZ) * (CapSense_MAX_CHARGE_TIME_US)) /\
                                                            (CapSense_AVG_CYCLES_PER_LOOP))
#endif  /* (CapSense_ENABLE == CapSense_CSDV2) */
#endif  /*  End of CapSense_CSX_EN              */
#endif  /*  End of CapSense_SENSING_CSD_LL_H    */


/* [] END OF FILE */
