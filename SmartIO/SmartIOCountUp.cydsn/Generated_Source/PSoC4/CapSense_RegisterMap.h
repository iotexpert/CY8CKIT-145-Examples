/***************************************************************************//**
* \file CapSense_RegisterMap.h
* \version 3.0
*
* \brief
*   This file provides the definitions for the component data structure register.
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

#if !defined(CY_CAPSENSE_CapSense_REGISTER_MAP_H)
#define CY_CAPSENSE_CapSense_REGISTER_MAP_H

#include <cytypes.h>
#include "CapSense_Configuration.h"
#include "CapSense_Structure.h"

/*****************************************************************************/
/* RAM Data structure register definitions                                   */
/*****************************************************************************/
#define CapSense_CONFIG_ID_VALUE                            (CapSense_dsRam.configId)
#define CapSense_CONFIG_ID_OFFSET                           (0u)
#define CapSense_CONFIG_ID_SIZE                             (2u)
#define CapSense_CONFIG_ID_PARAM_ID                         (0x87000000u)

#define CapSense_DEVICE_ID_VALUE                            (CapSense_dsRam.deviceId)
#define CapSense_DEVICE_ID_OFFSET                           (2u)
#define CapSense_DEVICE_ID_SIZE                             (2u)
#define CapSense_DEVICE_ID_PARAM_ID                         (0x8B000002u)

#define CapSense_TUNER_CMD_VALUE                            (CapSense_dsRam.tunerCmd)
#define CapSense_TUNER_CMD_OFFSET                           (4u)
#define CapSense_TUNER_CMD_SIZE                             (2u)
#define CapSense_TUNER_CMD_PARAM_ID                         (0xAD000004u)

#define CapSense_CSD_SCAN_CNT_VALUE                         (CapSense_dsRam.scanCounter)
#define CapSense_CSD_SCAN_CNT_OFFSET                        (6u)
#define CapSense_CSD_SCAN_CNT_SIZE                          (2u)
#define CapSense_CSD_SCAN_CNT_PARAM_ID                      (0x8A000006u)

#define CapSense_STATUS_VALUE                               (CapSense_dsRam.status)
#define CapSense_STATUS_OFFSET                              (8u)
#define CapSense_STATUS_SIZE                                (4u)
#define CapSense_STATUS_PARAM_ID                            (0xCA000008u)

#define CapSense_WDGT_ENABLE0_VALUE                         (CapSense_dsRam.wdgtEnable[0u])
#define CapSense_WDGT_ENABLE0_OFFSET                        (12u)
#define CapSense_WDGT_ENABLE0_SIZE                          (4u)
#define CapSense_WDGT_ENABLE0_PARAM_ID                      (0xE000000Cu)

#define CapSense_WDGT_STATUS0_VALUE                         (CapSense_dsRam.wdgtStatus[0u])
#define CapSense_WDGT_STATUS0_OFFSET                        (16u)
#define CapSense_WDGT_STATUS0_SIZE                          (4u)
#define CapSense_WDGT_STATUS0_PARAM_ID                      (0xCD000010u)

#define CapSense_SNS_STATUS0_VALUE                          (CapSense_dsRam.snsStatus[0u])
#define CapSense_SNS_STATUS0_OFFSET                         (20u)
#define CapSense_SNS_STATUS0_SIZE                           (1u)
#define CapSense_SNS_STATUS0_PARAM_ID                       (0x4B000014u)

#define CapSense_SNS_STATUS1_VALUE                          (CapSense_dsRam.snsStatus[1u])
#define CapSense_SNS_STATUS1_OFFSET                         (21u)
#define CapSense_SNS_STATUS1_SIZE                           (1u)
#define CapSense_SNS_STATUS1_PARAM_ID                       (0x4D000015u)

#define CapSense_SNS_STATUS2_VALUE                          (CapSense_dsRam.snsStatus[2u])
#define CapSense_SNS_STATUS2_OFFSET                         (22u)
#define CapSense_SNS_STATUS2_SIZE                           (1u)
#define CapSense_SNS_STATUS2_PARAM_ID                       (0x47000016u)

#define CapSense_CSD0_CONFIG_VALUE                          (CapSense_dsRam.csd0Config)
#define CapSense_CSD0_CONFIG_OFFSET                         (24u)
#define CapSense_CSD0_CONFIG_SIZE                           (2u)
#define CapSense_CSD0_CONFIG_PARAM_ID                       (0xAB000018u)

#define CapSense_MOD_CSX_CLK_VALUE                          (CapSense_dsRam.modCsxClk)
#define CapSense_MOD_CSX_CLK_OFFSET                         (26u)
#define CapSense_MOD_CSX_CLK_SIZE                           (1u)
#define CapSense_MOD_CSX_CLK_PARAM_ID                       (0x6F00001Au)

#define CapSense_BUTTON0_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button0.resolution)
#define CapSense_BUTTON0_RESOLUTION_OFFSET                  (28u)
#define CapSense_BUTTON0_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON0_RESOLUTION_PARAM_ID                (0xA780001Cu)

#define CapSense_BUTTON0_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button0.fingerTh)
#define CapSense_BUTTON0_FINGER_TH_OFFSET                   (30u)
#define CapSense_BUTTON0_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON0_FINGER_TH_PARAM_ID                 (0xAB80001Eu)

#define CapSense_BUTTON0_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button0.noiseTh)
#define CapSense_BUTTON0_NOISE_TH_OFFSET                    (32u)
#define CapSense_BUTTON0_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON0_NOISE_TH_PARAM_ID                  (0x63800020u)

#define CapSense_BUTTON0_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button0.nNoiseTh)
#define CapSense_BUTTON0_NNOISE_TH_OFFSET                   (33u)
#define CapSense_BUTTON0_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON0_NNOISE_TH_PARAM_ID                 (0x65800021u)

#define CapSense_BUTTON0_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button0.hysteresis)
#define CapSense_BUTTON0_HYSTERESIS_OFFSET                  (34u)
#define CapSense_BUTTON0_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON0_HYSTERESIS_PARAM_ID                (0x6F800022u)

#define CapSense_BUTTON0_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button0.onDebounce)
#define CapSense_BUTTON0_ON_DEBOUNCE_OFFSET                 (35u)
#define CapSense_BUTTON0_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON0_ON_DEBOUNCE_PARAM_ID               (0x69800023u)

#define CapSense_BUTTON0_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button0.lowBslnRst)
#define CapSense_BUTTON0_LOW_BSLN_RST_OFFSET                (36u)
#define CapSense_BUTTON0_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON0_LOW_BSLN_RST_PARAM_ID              (0x62800024u)

#define CapSense_BUTTON0_BSLN_COEFF_VALUE                   (CapSense_dsRam.wdgtList.button0.bslnCoeff)
#define CapSense_BUTTON0_BSLN_COEFF_OFFSET                  (37u)
#define CapSense_BUTTON0_BSLN_COEFF_SIZE                    (1u)
#define CapSense_BUTTON0_BSLN_COEFF_PARAM_ID                (0x64800025u)

#define CapSense_BUTTON0_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button0.snsClk)
#define CapSense_BUTTON0_SNS_CLK_OFFSET                     (38u)
#define CapSense_BUTTON0_SNS_CLK_SIZE                       (1u)
#define CapSense_BUTTON0_SNS_CLK_PARAM_ID                   (0x6E800026u)

#define CapSense_BUTTON1_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button1.resolution)
#define CapSense_BUTTON1_RESOLUTION_OFFSET                  (40u)
#define CapSense_BUTTON1_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON1_RESOLUTION_PARAM_ID                (0xAA810028u)

#define CapSense_BUTTON1_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button1.fingerTh)
#define CapSense_BUTTON1_FINGER_TH_OFFSET                   (42u)
#define CapSense_BUTTON1_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON1_FINGER_TH_PARAM_ID                 (0xA681002Au)

#define CapSense_BUTTON1_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button1.noiseTh)
#define CapSense_BUTTON1_NOISE_TH_OFFSET                    (44u)
#define CapSense_BUTTON1_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON1_NOISE_TH_PARAM_ID                  (0x6381002Cu)

#define CapSense_BUTTON1_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button1.nNoiseTh)
#define CapSense_BUTTON1_NNOISE_TH_OFFSET                   (45u)
#define CapSense_BUTTON1_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON1_NNOISE_TH_PARAM_ID                 (0x6581002Du)

#define CapSense_BUTTON1_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button1.hysteresis)
#define CapSense_BUTTON1_HYSTERESIS_OFFSET                  (46u)
#define CapSense_BUTTON1_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON1_HYSTERESIS_PARAM_ID                (0x6F81002Eu)

#define CapSense_BUTTON1_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button1.onDebounce)
#define CapSense_BUTTON1_ON_DEBOUNCE_OFFSET                 (47u)
#define CapSense_BUTTON1_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON1_ON_DEBOUNCE_PARAM_ID               (0x6981002Fu)

#define CapSense_BUTTON1_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button1.lowBslnRst)
#define CapSense_BUTTON1_LOW_BSLN_RST_OFFSET                (48u)
#define CapSense_BUTTON1_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON1_LOW_BSLN_RST_PARAM_ID              (0x65810030u)

#define CapSense_BUTTON1_BSLN_COEFF_VALUE                   (CapSense_dsRam.wdgtList.button1.bslnCoeff)
#define CapSense_BUTTON1_BSLN_COEFF_OFFSET                  (49u)
#define CapSense_BUTTON1_BSLN_COEFF_SIZE                    (1u)
#define CapSense_BUTTON1_BSLN_COEFF_PARAM_ID                (0x63810031u)

#define CapSense_BUTTON1_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button1.snsClk)
#define CapSense_BUTTON1_SNS_CLK_OFFSET                     (50u)
#define CapSense_BUTTON1_SNS_CLK_SIZE                       (1u)
#define CapSense_BUTTON1_SNS_CLK_PARAM_ID                   (0x69810032u)

#define CapSense_BUTTON2_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button2.resolution)
#define CapSense_BUTTON2_RESOLUTION_OFFSET                  (52u)
#define CapSense_BUTTON2_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON2_RESOLUTION_PARAM_ID                (0xA9820034u)

#define CapSense_BUTTON2_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button2.fingerTh)
#define CapSense_BUTTON2_FINGER_TH_OFFSET                   (54u)
#define CapSense_BUTTON2_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON2_FINGER_TH_PARAM_ID                 (0xA5820036u)

#define CapSense_BUTTON2_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button2.noiseTh)
#define CapSense_BUTTON2_NOISE_TH_OFFSET                    (56u)
#define CapSense_BUTTON2_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON2_NOISE_TH_PARAM_ID                  (0x62820038u)

#define CapSense_BUTTON2_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button2.nNoiseTh)
#define CapSense_BUTTON2_NNOISE_TH_OFFSET                   (57u)
#define CapSense_BUTTON2_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON2_NNOISE_TH_PARAM_ID                 (0x64820039u)

#define CapSense_BUTTON2_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button2.hysteresis)
#define CapSense_BUTTON2_HYSTERESIS_OFFSET                  (58u)
#define CapSense_BUTTON2_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON2_HYSTERESIS_PARAM_ID                (0x6E82003Au)

#define CapSense_BUTTON2_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button2.onDebounce)
#define CapSense_BUTTON2_ON_DEBOUNCE_OFFSET                 (59u)
#define CapSense_BUTTON2_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON2_ON_DEBOUNCE_PARAM_ID               (0x6882003Bu)

#define CapSense_BUTTON2_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button2.lowBslnRst)
#define CapSense_BUTTON2_LOW_BSLN_RST_OFFSET                (60u)
#define CapSense_BUTTON2_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON2_LOW_BSLN_RST_PARAM_ID              (0x6382003Cu)

#define CapSense_BUTTON2_BSLN_COEFF_VALUE                   (CapSense_dsRam.wdgtList.button2.bslnCoeff)
#define CapSense_BUTTON2_BSLN_COEFF_OFFSET                  (61u)
#define CapSense_BUTTON2_BSLN_COEFF_SIZE                    (1u)
#define CapSense_BUTTON2_BSLN_COEFF_PARAM_ID                (0x6582003Du)

#define CapSense_BUTTON2_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button2.snsClk)
#define CapSense_BUTTON2_SNS_CLK_OFFSET                     (62u)
#define CapSense_BUTTON2_SNS_CLK_SIZE                       (1u)
#define CapSense_BUTTON2_SNS_CLK_PARAM_ID                   (0x6F82003Eu)

#define CapSense_BUTTON0_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button0[0u].raw[0u])
#define CapSense_BUTTON0_RX0_RAW0_OFFSET                    (64u)
#define CapSense_BUTTON0_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON0_RX0_RAW0_PARAM_ID                  (0x8A000040u)

#define CapSense_BUTTON0_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button0[0u].bsln[0u])
#define CapSense_BUTTON0_RX0_BSLN0_OFFSET                   (66u)
#define CapSense_BUTTON0_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON0_RX0_BSLN0_PARAM_ID                 (0x86000042u)

#define CapSense_BUTTON0_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button0[0u].bslnExt[0u])
#define CapSense_BUTTON0_RX0_BSLN_EXT0_OFFSET               (68u)
#define CapSense_BUTTON0_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON0_RX0_BSLN_EXT0_PARAM_ID             (0x43000044u)

#define CapSense_BUTTON0_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button0[0u].idacComp[0u])
#define CapSense_BUTTON0_RX0_IDAC_COMP0_OFFSET              (69u)
#define CapSense_BUTTON0_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON0_RX0_IDAC_COMP0_PARAM_ID            (0x45000045u)

#define CapSense_BUTTON0_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button0[0u].diff)
#define CapSense_BUTTON0_RX0_DIFF_OFFSET                    (70u)
#define CapSense_BUTTON0_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON0_RX0_DIFF_PARAM_ID                  (0x87000046u)

#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button0[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_OFFSET       (72u)
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x40000048u)

#define CapSense_BUTTON1_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button1[0u].raw[0u])
#define CapSense_BUTTON1_RX0_RAW0_OFFSET                    (74u)
#define CapSense_BUTTON1_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON1_RX0_RAW0_PARAM_ID                  (0x8400004Au)

#define CapSense_BUTTON1_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button1[0u].bsln[0u])
#define CapSense_BUTTON1_RX0_BSLN0_OFFSET                   (76u)
#define CapSense_BUTTON1_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON1_RX0_BSLN0_PARAM_ID                 (0x8900004Cu)

#define CapSense_BUTTON1_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button1[0u].bslnExt[0u])
#define CapSense_BUTTON1_RX0_BSLN_EXT0_OFFSET               (78u)
#define CapSense_BUTTON1_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON1_RX0_BSLN_EXT0_PARAM_ID             (0x4D00004Eu)

#define CapSense_BUTTON1_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button1[0u].idacComp[0u])
#define CapSense_BUTTON1_RX0_IDAC_COMP0_OFFSET              (79u)
#define CapSense_BUTTON1_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON1_RX0_IDAC_COMP0_PARAM_ID            (0x4B00004Fu)

#define CapSense_BUTTON1_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button1[0u].diff)
#define CapSense_BUTTON1_RX0_DIFF_OFFSET                    (80u)
#define CapSense_BUTTON1_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON1_RX0_DIFF_PARAM_ID                  (0x8F000050u)

#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button1[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_OFFSET       (82u)
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4B000052u)

#define CapSense_BUTTON2_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button2[0u].raw[0u])
#define CapSense_BUTTON2_RX0_RAW0_OFFSET                    (84u)
#define CapSense_BUTTON2_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON2_RX0_RAW0_PARAM_ID                  (0x8E000054u)

#define CapSense_BUTTON2_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button2[0u].bsln[0u])
#define CapSense_BUTTON2_RX0_BSLN0_OFFSET                   (86u)
#define CapSense_BUTTON2_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON2_RX0_BSLN0_PARAM_ID                 (0x82000056u)

#define CapSense_BUTTON2_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button2[0u].bslnExt[0u])
#define CapSense_BUTTON2_RX0_BSLN_EXT0_OFFSET               (88u)
#define CapSense_BUTTON2_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON2_RX0_BSLN_EXT0_PARAM_ID             (0x45000058u)

#define CapSense_BUTTON2_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button2[0u].idacComp[0u])
#define CapSense_BUTTON2_RX0_IDAC_COMP0_OFFSET              (89u)
#define CapSense_BUTTON2_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON2_RX0_IDAC_COMP0_PARAM_ID            (0x43000059u)

#define CapSense_BUTTON2_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button2[0u].diff)
#define CapSense_BUTTON2_RX0_DIFF_OFFSET                    (90u)
#define CapSense_BUTTON2_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON2_RX0_DIFF_PARAM_ID                  (0x8100005Au)

#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button2[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_OFFSET       (92u)
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4400005Cu)

#define CapSense_SNR_TEST_WIDGET_ID_VALUE                   (CapSense_dsRam.snrTestWidgetId)
#define CapSense_SNR_TEST_WIDGET_ID_OFFSET                  (94u)
#define CapSense_SNR_TEST_WIDGET_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_WIDGET_ID_PARAM_ID                (0x6300005Eu)

#define CapSense_SNR_TEST_SENSOR_ID_VALUE                   (CapSense_dsRam.snrTestSensorId)
#define CapSense_SNR_TEST_SENSOR_ID_OFFSET                  (95u)
#define CapSense_SNR_TEST_SENSOR_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_SENSOR_ID_PARAM_ID                (0x6500005Fu)

#define CapSense_SNR_TEST_SCAN_COUNTER_VALUE                (CapSense_dsRam.snrTestScanCounter)
#define CapSense_SNR_TEST_SCAN_COUNTER_OFFSET               (96u)
#define CapSense_SNR_TEST_SCAN_COUNTER_SIZE                 (2u)
#define CapSense_SNR_TEST_SCAN_COUNTER_PARAM_ID             (0x80000060u)

#define CapSense_SNR_TEST_RAW_COUNT0_VALUE                  (CapSense_dsRam.snrTestRawCount[0u])
#define CapSense_SNR_TEST_RAW_COUNT0_OFFSET                 (98u)
#define CapSense_SNR_TEST_RAW_COUNT0_SIZE                   (2u)
#define CapSense_SNR_TEST_RAW_COUNT0_PARAM_ID               (0x8C000062u)


/*****************************************************************************/
/* Flash Data structure register definitions                                 */
/*****************************************************************************/
#define CapSense_BUTTON0_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[0].ptr2SnsFlash)
#define CapSense_BUTTON0_PTR2SNS_FLASH_OFFSET               (0u)
#define CapSense_BUTTON0_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON0_PTR2SNS_FLASH_PARAM_ID             (0xD1000000u)

#define CapSense_BUTTON0_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[0].ptr2WdgtRam)
#define CapSense_BUTTON0_PTR2WD_RAM_OFFSET                  (4u)
#define CapSense_BUTTON0_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON0_PTR2WD_RAM_PARAM_ID                (0xD0000004u)

#define CapSense_BUTTON0_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[0].ptr2SnsRam)
#define CapSense_BUTTON0_PTR2SNS_RAM_OFFSET                 (8u)
#define CapSense_BUTTON0_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON0_PTR2SNS_RAM_PARAM_ID               (0xD3000008u)

#define CapSense_BUTTON0_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[0].ptr2FltrHistory)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_OFFSET            (12u)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_PARAM_ID          (0xD200000Cu)

#define CapSense_BUTTON0_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[0].ptr2DebounceArr)
#define CapSense_BUTTON0_PTR2DEBOUNCE_OFFSET                (16u)
#define CapSense_BUTTON0_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON0_PTR2DEBOUNCE_PARAM_ID              (0xD4000010u)

#define CapSense_BUTTON0_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[0].staticConfig)
#define CapSense_BUTTON0_STATIC_CONFIG_OFFSET               (20u)
#define CapSense_BUTTON0_STATIC_CONFIG_SIZE                 (2u)
#define CapSense_BUTTON0_STATIC_CONFIG_PARAM_ID             (0x9A000014u)

#define CapSense_BUTTON0_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[0].totalNumSns)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_OFFSET               (22u)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_PARAM_ID             (0x96000016u)

#define CapSense_BUTTON0_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[0].wdgtType)
#define CapSense_BUTTON0_TYPE_OFFSET                        (24u)
#define CapSense_BUTTON0_TYPE_SIZE                          (1u)
#define CapSense_BUTTON0_TYPE_PARAM_ID                      (0x51000018u)

#define CapSense_BUTTON0_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[0].numCols)
#define CapSense_BUTTON0_NUM_COLS_OFFSET                    (25u)
#define CapSense_BUTTON0_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON0_NUM_COLS_PARAM_ID                  (0x57000019u)

#define CapSense_BUTTON0_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[0].numRows)
#define CapSense_BUTTON0_NUM_ROWS_OFFSET                    (26u)
#define CapSense_BUTTON0_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON0_NUM_ROWS_PARAM_ID                  (0x5D00001Au)

#define CapSense_BUTTON1_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[1].ptr2SnsFlash)
#define CapSense_BUTTON1_PTR2SNS_FLASH_OFFSET               (28u)
#define CapSense_BUTTON1_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON1_PTR2SNS_FLASH_PARAM_ID             (0xD401001Cu)

#define CapSense_BUTTON1_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[1].ptr2WdgtRam)
#define CapSense_BUTTON1_PTR2WD_RAM_OFFSET                  (32u)
#define CapSense_BUTTON1_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON1_PTR2WD_RAM_PARAM_ID                (0xD8010020u)

#define CapSense_BUTTON1_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[1].ptr2SnsRam)
#define CapSense_BUTTON1_PTR2SNS_RAM_OFFSET                 (36u)
#define CapSense_BUTTON1_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON1_PTR2SNS_RAM_PARAM_ID               (0xD9010024u)

#define CapSense_BUTTON1_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[1].ptr2FltrHistory)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_OFFSET            (40u)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_PARAM_ID          (0xDA010028u)

#define CapSense_BUTTON1_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[1].ptr2DebounceArr)
#define CapSense_BUTTON1_PTR2DEBOUNCE_OFFSET                (44u)
#define CapSense_BUTTON1_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON1_PTR2DEBOUNCE_PARAM_ID              (0xDB01002Cu)

#define CapSense_BUTTON1_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[1].staticConfig)
#define CapSense_BUTTON1_STATIC_CONFIG_OFFSET               (48u)
#define CapSense_BUTTON1_STATIC_CONFIG_SIZE                 (2u)
#define CapSense_BUTTON1_STATIC_CONFIG_PARAM_ID             (0x92010030u)

#define CapSense_BUTTON1_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[1].totalNumSns)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_OFFSET               (50u)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_PARAM_ID             (0x9E010032u)

#define CapSense_BUTTON1_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[1].wdgtType)
#define CapSense_BUTTON1_TYPE_OFFSET                        (52u)
#define CapSense_BUTTON1_TYPE_SIZE                          (1u)
#define CapSense_BUTTON1_TYPE_PARAM_ID                      (0x5B010034u)

#define CapSense_BUTTON1_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[1].numCols)
#define CapSense_BUTTON1_NUM_COLS_OFFSET                    (53u)
#define CapSense_BUTTON1_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON1_NUM_COLS_PARAM_ID                  (0x5D010035u)

#define CapSense_BUTTON1_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[1].numRows)
#define CapSense_BUTTON1_NUM_ROWS_OFFSET                    (54u)
#define CapSense_BUTTON1_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON1_NUM_ROWS_PARAM_ID                  (0x57010036u)

#define CapSense_BUTTON2_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[2].ptr2SnsFlash)
#define CapSense_BUTTON2_PTR2SNS_FLASH_OFFSET               (56u)
#define CapSense_BUTTON2_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON2_PTR2SNS_FLASH_PARAM_ID             (0xDA020038u)

#define CapSense_BUTTON2_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[2].ptr2WdgtRam)
#define CapSense_BUTTON2_PTR2WD_RAM_OFFSET                  (60u)
#define CapSense_BUTTON2_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON2_PTR2WD_RAM_PARAM_ID                (0xDB02003Cu)

#define CapSense_BUTTON2_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[2].ptr2SnsRam)
#define CapSense_BUTTON2_PTR2SNS_RAM_OFFSET                 (64u)
#define CapSense_BUTTON2_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON2_PTR2SNS_RAM_PARAM_ID               (0xDA020040u)

#define CapSense_BUTTON2_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[2].ptr2FltrHistory)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_OFFSET            (68u)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_PARAM_ID          (0xDB020044u)

#define CapSense_BUTTON2_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[2].ptr2DebounceArr)
#define CapSense_BUTTON2_PTR2DEBOUNCE_OFFSET                (72u)
#define CapSense_BUTTON2_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON2_PTR2DEBOUNCE_PARAM_ID              (0xD8020048u)

#define CapSense_BUTTON2_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[2].staticConfig)
#define CapSense_BUTTON2_STATIC_CONFIG_OFFSET               (76u)
#define CapSense_BUTTON2_STATIC_CONFIG_SIZE                 (2u)
#define CapSense_BUTTON2_STATIC_CONFIG_PARAM_ID             (0x9602004Cu)

#define CapSense_BUTTON2_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[2].totalNumSns)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_OFFSET               (78u)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_PARAM_ID             (0x9A02004Eu)

#define CapSense_BUTTON2_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[2].wdgtType)
#define CapSense_BUTTON2_TYPE_OFFSET                        (80u)
#define CapSense_BUTTON2_TYPE_SIZE                          (1u)
#define CapSense_BUTTON2_TYPE_PARAM_ID                      (0x58020050u)

#define CapSense_BUTTON2_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[2].numCols)
#define CapSense_BUTTON2_NUM_COLS_OFFSET                    (81u)
#define CapSense_BUTTON2_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON2_NUM_COLS_PARAM_ID                  (0x5E020051u)

#define CapSense_BUTTON2_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[2].numRows)
#define CapSense_BUTTON2_NUM_ROWS_OFFSET                    (82u)
#define CapSense_BUTTON2_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON2_NUM_ROWS_PARAM_ID                  (0x54020052u)


#endif /* End CY_CAPSENSE_CapSense_REGISTER_MAP_H */

/* [] END OF FILE */
