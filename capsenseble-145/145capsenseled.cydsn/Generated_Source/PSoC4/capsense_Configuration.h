/*******************************************************************************
* \file       capsense_Configuration.h
* \version    3.0
*
* \brief
*   This file provides the customizer parameters definitions.
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

#if !defined(CY_CAPSENSE_capsense_CONFIGURATION_H)
#define CY_CAPSENSE_capsense_CONFIGURATION_H

#include <cytypes.h>

/*******************************************************************************
* Customizer-generated defines
*******************************************************************************/
#define capsense_ENABLE                   (1u)
#define capsense_DISABLE                  (0u)

/*******************************************************************************
* Creator-global defines
*******************************************************************************/
#ifdef CYIPBLOCK_m0s8csdv2_VERSION
    #define capsense_CSDV2                              (1u)
#else
    #define capsense_CSDV2                              (0u)
#endif  /* CYIPBLOCK_m0s8csdv2_VERSION */

#if (capsense_ENABLE == capsense_CSDV2)
    #define capsense_2000_MV                            (2000u)

    #ifdef CYDEV_VDDA_MV
        #define capsense_CYDEV_VDDA_MV                  (CYDEV_VDDA_MV)
    #else
        #define capsense_CYDEV_VDDA_MV                  (CYDEV_VDD_MV)
    #endif  /* CYIPBLOCK_m0s8csdv2_VERSION */

    #define capsense_BAD_CONVERSIONS_NUM                (1u)
    #define capsense_RESAMPLING_CYCLES_MAX_NUMBER       (1u)
#endif /* (capsense_ENABLE == capsense_CSDV2) */

/*******************************************************************************
* Enabled Scan Methods
*******************************************************************************/
#define capsense_CSD_EN                   (1u)
#define capsense_CSX_EN                   (0u)
#define capsense_CSD_CSX_EN               (capsense_CSD_EN & capsense_CSX_EN)
#define capsense_CSD2X_EN                 (0u)
#define capsense_CSX2X_EN                 (0u)

/*******************************************************************************
* Definitions for number of widgets and sensors
*******************************************************************************/
#define capsense_TOTAL_WIDGETS            (1u)
#define capsense_TOTAL_CSD_WIDGETS        (1u)
#define capsense_TOTAL_CSD_SENSORS        (5u)
#define capsense_TOTAL_CSX_WIDGETS        (0u)
#define capsense_TOTAL_CSX_NODES          (0u)

/*******************************************************************************
* Total number of CSD sensors + CSX nodes
*******************************************************************************/
#define capsense_TOTAL_SENSORS            (capsense_TOTAL_CSD_SENSORS + \
                                                   capsense_TOTAL_CSX_NODES)

/*******************************************************************************
* Define widget IDs
*******************************************************************************/
#define capsense_LINEARSLIDER0_WDGT_ID          (0u)

/*******************************************************************************
* Define sensor IDs
*******************************************************************************/

/* LinearSlider0 */
#define capsense_LINEARSLIDER0_SNS0_ID          (0u)
#define capsense_LINEARSLIDER0_SNS1_ID          (1u)
#define capsense_LINEARSLIDER0_SNS2_ID          (2u)
#define capsense_LINEARSLIDER0_SNS3_ID          (3u)
#define capsense_LINEARSLIDER0_SNS4_ID          (4u)

/*******************************************************************************
* Enabled widget types
*******************************************************************************/
#define capsense_BUTTON_WIDGET_EN         (0u)
#define capsense_SLIDER_WIDGET_EN         (1u)
#define capsense_MATRIX_WIDGET_EN         (0u)
#define capsense_PROXIMITY_WIDGET_EN      (0u)
#define capsense_TOUCHPAD_WIDGET_EN       (0u)

#define capsense_CSD_MATRIX_WIDGET_EN     (0u)
#define capsense_CSD_TOUCHPAD_WIDGET_EN   (0u)

#define capsense_CSX_MATRIX_WIDGET_EN     (0u)
#define capsense_CSX_TOUCHPAD_WIDGET_EN   (0u)

/*******************************************************************************
* Centroid APIs
*******************************************************************************/
#define capsense_CENTROID_EN              (1u)
#define capsense_TOTAL_DIPLEXED_SLIDERS   (0u)
#define capsense_TOTAL_LINEAR_SLIDERS     (1u)
#define capsense_TOTAL_RADIAL_SLIDERS     (0u)
#define capsense_TOTAL_TOUCHPADS          (0u)
#define capsense_MAX_CENTROID_LENGTH      (5u)

/*******************************************************************************
* Enabled sensor types
*******************************************************************************/
#define capsense_REGULAR_SENSOR_EN        (1u)
#define capsense_PROXIMITY_SENSOR_EN      (0u)

/*******************************************************************************
* Sensor ganging
*******************************************************************************/
#define capsense_CSD_GANGED_SNS_EN        (0u)
#define capsense_CSX_GANGED_SNS_EN        (0u)

/*******************************************************************************
* Max number of sensors used among all widgets
*******************************************************************************/
#define capsense_MAX_SENSORS_PER_WIDGET   (5u)

/*******************************************************************************
* Total number of all used electrodes (NOT unique)
*******************************************************************************/
#define capsense_TOTAL_ELECTRODES         (5u)
/* Obsolete */
#define capsense_TOTAL_SENSOR_IOS         capsense_TOTAL_ELECTRODES

/*******************************************************************************
* Total number of used physical IOs (unique)
*******************************************************************************/
#define capsense_TOTAL_IO_CNT             (5u)

/*******************************************************************************
* Array length for widget status registers
*******************************************************************************/
#define capsense_WDGT_STATUS_WORDS        \
                        (((uint8)((capsense_TOTAL_WIDGETS - 1u) / 32u)) + 1u)


/*******************************************************************************
* Auto-tuning mode selection
*******************************************************************************/
#define capsense_CSD_SS_DIS         (0x00ul)
#define capsense_CSD_SS_HW_EN       (0x01ul)
#define capsense_CSD_SS_TH_EN       (0x02ul)
#define capsense_CSD_SS_HWTH_EN     (capsense_CSD_SS_HW_EN | \
                                             capsense_CSD_SS_TH_EN)

#define capsense_CSD_AUTOTUNE       capsense_CSD_SS_HWTH_EN


/*******************************************************************************
* General settings
*******************************************************************************/

#define capsense_AUTO_RESET_METHOD_LEGACY (0u)
#define capsense_AUTO_RESET_METHOD_SAMPLE (1u)

#define capsense_SELF_TEST_EN             (0u)
#define capsense_MULTI_FREQ_SCAN_EN       (0u)
#define capsense_SENSOR_AUTO_RESET_EN     (0u)
#define capsense_SENSOR_AUTO_RESET_METHOD (0u)
#define capsense_NUM_CENTROIDS            (1u)
#define capsense_OFF_DEBOUNCE_EN          (0u)

/* Define power status of HW IP block after scanning */
#define capsense_BLOCK_OFF_AFTER_SCAN_EN  (0u)

/* Define number of scan frequencies */
#if (capsense_DISABLE != capsense_MULTI_FREQ_SCAN_EN)
    #define capsense_NUM_SCAN_FREQS       (3u)
#else
    #define capsense_NUM_SCAN_FREQS       (1u)
#endif /* #if (capsense_DISABLE != capsense_MULTI_FREQ_SCAN_EN) */

/* Data size for thresholds / low baseline reset */
#define capsense_SIZE_8BITS               (8u)
#define capsense_SIZE_16BITS              (16u)

#define capsense_THRESHOLD_SIZE           capsense_SIZE_16BITS
typedef uint16 capsense_THRESHOLD_TYPE;

#if (capsense_AUTO_RESET_METHOD_LEGACY == capsense_SENSOR_AUTO_RESET_METHOD)
    #define capsense_LOW_BSLN_RST_SIZE        capsense_SIZE_8BITS
    typedef uint8 capsense_LOW_BSLN_RST_TYPE;
#else
    #define capsense_LOW_BSLN_RST_SIZE    (16u)
    typedef uint16 capsense_LOW_BSLN_RST_TYPE;
#endif /* #if (capsense_AUTO_RESET_METHOD_LEGACY == capsense_SENSOR_AUTO_RESET_METHOD) */


/*******************************************************************************
* General Filter Constants
*******************************************************************************/

/* Baseline algorithm options */
#define capsense_IIR_BASELINE                 (0u)
#define capsense_BUCKET_BASELINE              (1u)

#define capsense_BASELINE_TYPE                capsense_IIR_BASELINE

/* IIR baseline filter algorithm for regular sensors*/
#define capsense_REGULAR_IIR_BL_TYPE          capsense_IIR_FILTER_PERFORMANCE

/* IIR baseline coefficients for regular sensors */
#define capsense_REGULAR_IIR_BL_N             (1u)
#define capsense_REGULAR_IIR_BL_SHIFT         (8u)

/* IIR baseline filter algorithm for proximity sensors*/
#define capsense_PROX_IIR_BL_TYPE             capsense_IIR_FILTER_PERFORMANCE

/* IIR baseline coefficients for proximity sensors */
#define capsense_PROX_IIR_BL_N                (1u)
#define capsense_PROX_IIR_BL_SHIFT            (8u)


/* IIR filter constants */
#define capsense_IIR_COEFFICIENT_K            (256u)

/* IIR filter type */
#define capsense_IIR_FILTER_STANDARD          (1u)
#define capsense_IIR_FILTER_PERFORMANCE       (2u)
#define capsense_IIR_FILTER_MEMORY            (3u)

/* Regular sensor raw-count filters */
#define capsense_REGULAR_RC_FILTER_EN         (0u)
#define capsense_REGULAR_RC_IIR_FILTER_EN     (0u)
#define capsense_REGULAR_RC_MEDIAN_FILTER_EN  (0u)
#define capsense_REGULAR_RC_AVERAGE_FILTER_EN (0u)
#define capsense_REGULAR_RC_CUSTOM_FILTER_EN  (0u)

/* Proximity sensor raw-count filters */
#define capsense_PROX_RC_FILTER_EN            (0u)
#define capsense_PROX_RC_IIR_FILTER_EN        (0u)
#define capsense_PROX_RC_MEDIAN_FILTER_EN     (0u)
#define capsense_PROX_RC_AVERAGE_FILTER_EN    (0u)
#define capsense_PROX_RC_CUSTOM_FILTER_EN     (0u)

/* IIR raw-count filter algorithm for regular sensors */
#define capsense_REGULAR_IIR_RC_TYPE          (capsense_IIR_FILTER_STANDARD)

/* IIR raw-count filter coefficients for regular sensors */
#define capsense_REGULAR_IIR_RC_N             (128u)
#define capsense_REGULAR_IIR_RC_SHIFT         (0u)

/* IIR raw-count filter algorithm for proximity sensors*/
#define capsense_PROX_IIR_RC_TYPE             (capsense_IIR_FILTER_STANDARD)

/* IIR raw-count filter coefficients for proximity sensors */
#define capsense_PROX_IIR_RC_N                (128u)
#define capsense_PROX_IIR_RC_SHIFT            (0u)

/* Median filter constants */

/* Order of regular sensor median filter */
#define capsense_REGULAR_MEDIAN_LEN           (2u)

/* Order of proximity sensor median filter */
#define capsense_PROX_MEDIAN_LEN              (2u)

/* Average filter constants*/
#define capsense_AVERAGE_FILTER_LEN_2         (2u)
#define capsense_AVERAGE_FILTER_LEN_4         (4u)

/* Order of regular sensor average filter */
#define capsense_REGULAR_AVERAGE_LEN          (capsense_AVERAGE_FILTER_LEN_4)

/* Order of proximity sensor average filter */
#define capsense_PROX_AVERAGE_LEN             (capsense_AVERAGE_FILTER_LEN_4)


/* Centroid position filters */
#define capsense_POS_IIR_FILTER_EN            (0x00u)
#define capsense_POS_MEDIAN_FILTER_EN         (0x00u)
#define capsense_POS_AVERAGE_FILTER_EN        (0x00u)
#define capsense_POS_JITTER_FILTER_EN         (0x00u)
#define capsense_CSX_TOUCHPAD_POS_MEDIAN_FILTER_EN (0x00u)

#define capsense_POS_IIR_COEFF                (128u)
#define capsense_CSX_TOUCHPAD_UNDEFINED       (40u)


/* IDAC options */

/* CSDv1 IDAC gain */
#define capsense_IDAC_GAIN_4X                 (4u)
#define capsense_IDAC_GAIN_8X                 (8u)

/* CSDv2 IDAC gain */
#define capsense_IDAC_GAIN_LOW                (0uL)
#define capsense_IDAC_GAIN_MEDIUM             (1uL)
#define capsense_IDAC_GAIN_HIGH               (2uL)

#define capsense_IDAC_SOURCING                (0u)
#define capsense_IDAC_SINKING                 (1u)

/* Shield tank capacitor precharge source */
#define capsense_CSH_PRECHARGE_VREF           (0u)
#define capsense_CSH_PRECHARGE_IO_BUF         (1u)

/* Shield electrode delay */
#define capsense_NO_DELAY                     (0u)
/* CSDv1 */
#define capsense_SH_DELAY_50NS                (1u)
#define capsense_SH_DELAY_100NS               (2u)
/* CSDv2 */
#define capsense_SH_DELAY_5NS                 (1u)
#define capsense_SH_DELAY_10NS                (2u)
#define capsense_SH_DELAY_20NS                (3u)

/* Inactive sensor connection options */
#define capsense_SNS_CONNECTION_GROUND        (0x00000006Lu)
#define capsense_SNS_CONNECTION_HIGHZ         (0x00000000Lu)
#define capsense_SNS_CONNECTION_SHIELD        (0x00000002Lu)

/* Sense clock selection options */
#define capsense_CLK_SOURCE_DIRECT            (0x00000000Lu)
#define capsense_CLK_SOURCE_PRSAUTO           (0x000000FFLu)
#define capsense_CLK_SOURCE_SSC2              (0x00000001Lu)
#define capsense_CLK_SOURCE_SSC3              (0x00000002Lu)
#define capsense_CLK_SOURCE_SSC4              (0x00000003Lu)
#define capsense_CLK_SOURCE_SSC5              (0x00000004Lu)
#define capsense_CLK_SOURCE_PRS8              (0x00000005Lu)
#define capsense_CLK_SOURCE_PRS12             (0x00000006Lu)

/* Defines scan resolutions */
#define capsense_RES6BIT                      (6u)
#define capsense_RES7BIT                      (7u)
#define capsense_RES8BIT                      (8u)
#define capsense_RES9BIT                      (9u)
#define capsense_RES10BIT                     (10u)
#define capsense_RES11BIT                     (11u)
#define capsense_RES12BIT                     (12u)
#define capsense_RES13BIT                     (13u)
#define capsense_RES14BIT                     (14u)
#define capsense_RES15BIT                     (15u)
#define capsense_RES16BIT                     (16u)

/* CSDv2: Initialization switch resistance */
#define capsense_INIT_SW_RES_LOW              (0x00000000Lu)
#define capsense_INIT_SW_RES_MEDIUM           (0x00000001Lu)
#define capsense_INIT_SW_RES_HIGH             (0x00000002Lu)

/* CSDv2: CSD shield switch resistance */
#define capsense_SHIELD_SW_RES_LOW            (0x00000000Lu)
#define capsense_SHIELD_SW_RES_MEDIUM         (0x00000001Lu)
#define capsense_SHIELD_SW_RES_HIGH           (0x00000002Lu)
#define capsense_SHIELD_SW_RES_LOW_EMI        (0x00000003Lu)

/*******************************************************************************
* CSD Specific settings
*******************************************************************************/

/* CSD scan method settings */
#define capsense_CSD_IDAC_AUTOCAL_EN          (1u)
#define capsense_CSD_IDAC_GAIN                (capsense_IDAC_GAIN_HIGH)
#define capsense_CSD_SHIELD_EN                (0u)
#define capsense_CSD_SHIELD_TANK_EN           (0u)
#define capsense_CSD_CSH_PRECHARGE_SRC        (capsense_CSH_PRECHARGE_VREF)
#define capsense_CSD_SHIELD_DELAY             (capsense_NO_DELAY)
#define capsense_CSD_TOTAL_SHIELD_COUNT       (0u)
#define capsense_CSD_SCANSPEED_DIVIDER        (1u)
#define capsense_CSD_COMMON_SNS_CLK_EN        (0u)
#define capsense_CSD_SNS_CLK_SOURCE           (capsense_CLK_SOURCE_PRSAUTO)
#define capsense_CSD_SNS_CLK_DIVIDER          (8u)
#define capsense_CSD_INACTIVE_SNS_CONNECTION  (capsense_SNS_CONNECTION_GROUND)
#define capsense_CSD_IDAC_COMP_EN             (1u)
#define capsense_CSD_IDAC_CONFIG              (capsense_IDAC_SOURCING)
#define capsense_CSD_RAWCOUNT_CAL_LEVEL       (85u)
#define capsense_CSD_DUALIDAC_LEVEL           (50u)
#define capsense_CSD_PRESCAN_SETTLING_TIME    (5u)
#define capsense_CSD_SNSCLK_R_CONST           (1000u)
#define capsense_CSD_VREF_MV                  (2133u)

/* CSD settings - CSDv2 */
#define capsense_CSD_DEDICATED_IDAC_COMP_EN   (1u)
#define capsense_CSD_AUTO_ZERO_EN             (0u)
#define capsense_CSD_AUTO_ZERO_TIME           (1Lu)
#define capsense_CSD_NOISE_METRIC_EN          (0u)
#define capsense_CSD_NOISE_METRIC_TH          (1Lu)
#define capsense_CSD_INIT_SWITCH_RES          (capsense_INIT_SW_RES_MEDIUM)
#define capsense_CSD_SHIELD_SWITCH_RES        (capsense_SHIELD_SW_RES_MEDIUM)
#define capsense_CSD_GAIN                     (17Lu)


/*******************************************************************************
* CSX Specific settings
*******************************************************************************/

/* CSX scan method settings */
#define capsense_CSX_SCANSPEED_DIVIDER        (1u)
#define capsense_CSX_COMMON_TX_CLK_EN         (0u)
#define capsense_CSX_TX_CLK_SOURCE            (capsense_CLK_SOURCE_PRSAUTO)
#define capsense_CSX_TX_CLK_DIVIDER           (80u)
#define capsense_CSX_MAX_FINGERS              (1u)
#define capsense_CSX_MAX_LOCAL_PEAKS          (5u)
#define capsense_CSX_IDAC_AUTOCAL_EN          (0u)
#define capsense_CSX_IDAC_BITS_USED           (8u)
#define capsense_CSX_RAWCOUNT_CAL_LEVEL       (40u)
#define capsense_CSX_IDAC_GAIN                (capsense_IDAC_GAIN_MEDIUM)
#define capsense_CSX_SKIP_OVSMPL_SPECIFIC_NODES (0u)
#define capsense_CSX_MULTIPHASE_TX_EN         (0u)
#define capsense_CSX_MAX_TX_PHASE_LENGTH      (0u)

/* CSX settings - CSDv2 */
#define capsense_CSX_AUTO_ZERO_EN             (0u)
#define capsense_CSX_AUTO_ZERO_TIME           (1u)
#define capsense_CSX_NOISE_METRIC_EN          (0u)
#define capsense_CSX_NOISE_METRIC_TH          (1u)
#define capsense_CSX_INIT_SWITCH_RES          (capsense_INIT_SW_RES_MEDIUM)


/*******************************************************************************
* Global Parameter Definitions
*******************************************************************************/

/* RAM Global Parameters Definitions */
#define capsense_CONFIG_ID                      (0x76E2u)
#define capsense_DEVICE_ID                      (0x0160u)
#define capsense_CSD0_CONFIG                    (0x0000u)


/***************************************************************************//**
* LinearSlider0 initialization values for FLASH data structure
*******************************************************************************/
#define capsense_LINEARSLIDER0_STATIC_CONFIG    (1u)
#define capsense_LINEARSLIDER0_NUM_SENSORS      (5u)
#define capsense_LINEARSLIDER0_X_RESOLUTION     (100u)
#define capsense_LINEARSLIDER0_X_CENT_MULT      ((capsense_LINEARSLIDER0_X_RESOLUTION * 256u) / \
                                                 (capsense_LINEARSLIDER0_NUM_SENSORS - 1u))

/***************************************************************************//**
* LinearSlider0 initialization values for RAM data structure
*******************************************************************************/
#define capsense_LINEARSLIDER0_RESOLUTION       (capsense_RES12BIT)
#define capsense_LINEARSLIDER0_FINGER_TH        (1u)
#define capsense_LINEARSLIDER0_NOISE_TH         (1u)
#define capsense_LINEARSLIDER0_NNOISE_TH        (1u)
#define capsense_LINEARSLIDER0_HYSTERESIS       (1u)
#define capsense_LINEARSLIDER0_ON_DEBOUNCE      (3u)
#define capsense_LINEARSLIDER0_LOW_BSLN_RST     (30u)
#define capsense_LINEARSLIDER0_BSLN_COEFF       (100u)
#define capsense_LINEARSLIDER0_IDAC_MOD0        (32u)
#define capsense_LINEARSLIDER0_SNS_CLK          (8u)
#define capsense_LINEARSLIDER0_SENSITIVITY      (500u)
#define capsense_LINEARSLIDER0_SIGPPF           (0u)
#define capsense_LINEARSLIDER0_POSITION         (0xFFFFu)

/* RAM Sensor Parameters Definitions */
#define capsense_LINEARSLIDER0_SNS0_IDAC_COMP0  (32u)
#define capsense_LINEARSLIDER0_SNS1_IDAC_COMP0  (32u)
#define capsense_LINEARSLIDER0_SNS2_IDAC_COMP0  (32u)
#define capsense_LINEARSLIDER0_SNS3_IDAC_COMP0  (32u)
#define capsense_LINEARSLIDER0_SNS4_IDAC_COMP0  (32u)


/*******************************************************************************
* ADC Specific Macros
*******************************************************************************/
#define capsense_ADC_RES8BIT                  (8u)
#define capsense_ADC_RES10BIT                 (10u)

#define capsense_ADC_EN                       (0u)
#define capsense_ADC_TOTAL_CHANNELS           (1u)
#define capsense_ADC_RESOLUTION               (capsense_ADC_RES10BIT)
#define capsense_ADC_AMUXB_INPUT_EN           (0u)
#define capsense_ADC_SELECT_AMUXB_CH          (0u)
#define capsense_ADC_AZ_EN                    (0Lu)
#define capsense_ADC_AZ_TIME                  (10u)
#define capsense_ADC_VREF_MV                  (2133u)
#define capsense_ADC_GAIN                     (17u)

/* Refgen constants definitions */
#define capsense_AdcIDAC_BASELINE   (30UL)
#define capsense_Adc48MHZ_HZ        (48000000UL)
#define capsense_AdcVREFHI_BASELINE (1200UL)
    /* From a baseline of 30 for 1200mV Vref_hi at 48MHz, scale down by HFCLK, 
       up by actual Vref_hi, and exponentially down for higher resolutions. 
       Define using 'round-to-nearest' arithmetic, i.e., 
              roundQuotient = [dividend + (divisor/2)] / divisor 
    */
#define capsense_ADC_IDACVAL   ((uint8)((((capsense_Adc48MHZ_HZ / \
        CYDEV_BCLK__HFCLK__HZ) * ((capsense_AdcIDAC_BASELINE * \
        capsense_ADC_VREF_MV) / capsense_AdcVREFHI_BASELINE)) + \
        ((1UL << (capsense_ADC_RESOLUTION - capsense_ADC_RES8BIT)) / 2UL)) / \
	  	(1UL << (capsense_ADC_RESOLUTION - capsense_ADC_RES8BIT))))

#endif /* CY_CAPSENSE_capsense_CONFIGURATION_H */


/* [] END OF FILE */
