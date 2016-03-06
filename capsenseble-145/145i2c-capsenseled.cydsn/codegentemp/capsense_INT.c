/***************************************************************************//**
* \file capsense_INT.c
* \version 3.0
*
* \brief
*   This file contains the source code for implementation of the CapSense component
*   Interrupt Service Routine (ISR).
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
#include "cytypes.h"
#include "cyfitter.h"
#include "capsense_Sensing.h"
#include "cyapicallbacks.h"

/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

#if (((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) && \
     (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN))
    static void capsense_SsNextFrequencyScan(void);
#endif /* (((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) && \
            (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)) */

#if (((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) && \
     (capsense_ENABLE == capsense_CSDV2))
    static void capsense_SsPostScan(void);
    CY_INLINE static void capsense_SsInitNextScan(void);
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        CY_INLINE static void capsense_SsInitNextGangedScan(void);
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */  
#endif /* (((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) && \
           (capsense_ENABLE == capsense_CSDV2)) */ 
            
/** \}
* \endif */


/**
* \if SECTION_CAPSENSE_INTERRUPT
* \addtogroup group_capsense_interrupt
* \{
*/


#if ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN))

#if (capsense_ENABLE == capsense_CSDV2)
    /* CSDv2 part */

    /*******************************************************************************
    * Function Name: capsense_CSDPostSingleScan
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for single-sensor scanning implementation.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_CSDScanExt() function.
    *
    *   The following tasks are performed for CSDv1 HW IP block:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Update the Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The following tasks are performed for CSDv2 HW IP block:
    *   - Check if the raw data is not noisy
    *   - Read the Counter register and update the data structure with raw data
    *   - Configure and start the scan for the next frequency if the
    *      multi-frequency is enabled
    *   - Update the Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler changes IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostSingleScan)
    {
        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

    #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
        if ((capsense_CSD_NOISE_METRIC_TH < ((CY_GET_REG32(capsense_RESULT_VAL1_PTR) &
                                                    capsense_RESULT_VAL1_BAD_CONVS_MASK) >>
                                                    capsense_RESULT_VAL1_BAD_CONVS_SHIFT)) &&
                                                    (0u < capsense_badConversionsNum))
        {
            /* Decrement bad conversions number */
            capsense_badConversionsNum--;

            /* Start the re-scan */
            CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_AZ0_SKIP_MASK |
                                                         capsense_SEQ_START_AZ1_SKIP_MASK |
                                                         capsense_SEQ_START_START_MASK);
        }
        else
        {
    #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

            capsense_SsPostScan();

            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
                {
                    /* Scan the next channel */
                    capsense_SsNextFrequencyScan();
                }
                else
                {
                    /* All channels are scanned. Set IMO to zero channel */
                    capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                    capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);

                    #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                        /*  Disable CSDv2 block */
                        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
                    #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */    

                    /* Update Scan Counter */
                    capsense_dsRam.scanCounter++;

                    /* Sensor is totally scanned. Reset BUSY flag */
                    capsense_dsRam.status &= ~(capsense_SW_STS_BUSY | capsense_WDGT_SW_STS_BUSY);
                }   
            #else
                {
                    #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                        /*  Disable CSDv2 block */
                        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
                    #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */    

                    /* Update Scan Counter */
                    capsense_dsRam.scanCounter++;

                    /* Sensor is totally scanned. Reset BUSY flag */
                    capsense_dsRam.status &= ~(capsense_SW_STS_BUSY | capsense_WDGT_SW_STS_BUSY);
                }
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
    #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
        }
    #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }


    /*******************************************************************************
    * Function Name: capsense_CSDPostMultiScan
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for multiple sensor-scanning implementation.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_Scan() or capsense_ScanAllWidgets() APIs.
    *   The following tasks are performed:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Disable the CSD block (after the widget has been scanned)
    *   - Update Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler initializes scanning for the previous sensor when the widget has
    *   more than one sensor.
    *   The ISR handler initializes scanning for the next widget when the
    *   capsense_ScanAllWidgets() APIs are called and the project has more than
    *   one widget.
    *   The ISR handler changes the IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostMultiScan)
    {
        /*  Declare and initialise ptr to sensor IO structure to appropriate address        */
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                          capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                                          + capsense_sensorIndex;

        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

        #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
            if ((capsense_CSD_NOISE_METRIC_TH < ((CY_GET_REG32(capsense_RESULT_VAL1_PTR) &
                                                      capsense_RESULT_VAL1_BAD_CONVS_MASK) >>
                                                      capsense_RESULT_VAL1_BAD_CONVS_SHIFT)) &&
                                                      (0u < capsense_badConversionsNum))
            {
                /* Decrement bad conversions number */
                capsense_badConversionsNum--;

                /* Start the re-scan */
                CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_AZ0_SKIP_MASK |
                                                             capsense_SEQ_START_AZ1_SKIP_MASK |
                                                             capsense_SEQ_START_START_MASK);
            }
            else
            {
        #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */
            
            capsense_SsPostScan();

            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                /* Disable sensor when all frequency channels are scanned */
                if (capsense_FREQ_CHANNEL_2 == capsense_scanFreqIndex)
                {
                    /* Disable sensor */
                    capsense_CSDDisconnectSns(curSnsIOPtr);
                }
            #else
                /* Disable sensor */
                capsense_CSDDisconnectSns(curSnsIOPtr);  
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
                {
                     /* Scan the next channel */
                    capsense_SsNextFrequencyScan();
                }
                else
                {
                     /* All channels are scanned. Set IMO to zero channel */
                    capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                    capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);

                     /* Scan the next sensor */
                    capsense_SsInitNextScan();   
                }
            #else 
                /* Scan the next sensor */
                capsense_SsInitNextScan();
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
            }
        #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }


    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
    /*******************************************************************************
    * Function Name: capsense_CSDPostMultiScanGanged
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for multiple sensor scanning implementation
    *   for ganged sensors.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_Scan() API for a ganged sensor or the
    *   capsense_ScanAllWidgets() API in the project with ganged sensors.
    *
    *   The following tasks are performed:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Disable the CSD block (after the widget has been scanned)
    *   - Update Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler initializes scanning for the previous sensor when the widget has
    *   more than one sensor.
    *   The ISR handler initializes scanning for the next widget when the
    *   capsense_ScanAllWidgets() APIs are called and the project has more than
    *   one widget.
    *   The ISR handler changes the IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostMultiScanGanged)
    {
        uint32 tempVal;

        /*  Declare and initialise ptr to sensor IO structure to appropriate address       */
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr =
            (capsense_FLASH_IO_STRUCT const *)capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
            + capsense_sensorIndex;

        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

        #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
            if ((capsense_CSD_NOISE_METRIC_TH < ((CY_GET_REG32(capsense_RESULT_VAL1_PTR) &
                                                      capsense_RESULT_VAL1_BAD_CONVS_MASK) >>
                                                      capsense_RESULT_VAL1_BAD_CONVS_SHIFT)) &&
                                                      (0u < capsense_badConversionsNum))
            {
                /* Decrement bad conversions number */
                capsense_badConversionsNum--;

                /* Start the re-scan */
                CY_SET_REG32(capsense_SEQ_START_PTR, capsense_SEQ_START_AZ0_SKIP_MASK |
                                                             capsense_SEQ_START_AZ1_SKIP_MASK |
                                                             capsense_SEQ_START_START_MASK);
            }
            else
            {
        #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

            capsense_SsPostScan();

            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                if (capsense_FREQ_CHANNEL_2 == capsense_scanFreqIndex)
                {
                    /* Check ganged sns flag  */
                    if ((capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig &
                        capsense_GANGED_SNS_MASK) == capsense_GANGED_SNS_MASK)
                    {
                        /* Get sns pointer */
                        capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                           capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash +
                                                           capsense_sensorIndex;

                        /* Get number of ganged pins */
                        tempVal = capsense_curFlashSnsPtr->numPins;

                        /* Get IO pointer  */
                        curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

                        /* Disconnect all ganged sensors */
                        do
                        {
                            capsense_CSDDisconnectSns(curSnsIOPtr);
                            curSnsIOPtr++;
                            tempVal--;
                        } while (0u != tempVal);
                    }
                    else
                    {
                        /* Disconnect ganged sensor */
                        capsense_CSDDisconnectSns(curSnsIOPtr);
                    }
                }        
            #else
                /* Check ganged sns flag  */
                if ((capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig &
                    capsense_GANGED_SNS_MASK) == capsense_GANGED_SNS_MASK)
                {
                    /* Get sns pointer */
                    capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                       capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash +
                                                       capsense_sensorIndex;

                    /* Get number of ganged pins */
                    tempVal = capsense_curFlashSnsPtr->numPins;

                    /* Get IO pointer  */
                    curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

                    /* Disconnect all ganged sensors */
                    do
                    {
                        capsense_CSDDisconnectSns(curSnsIOPtr);
                        curSnsIOPtr++;
                        tempVal--;
                    } while (0u != tempVal);
                }
                else
                {
                    /* Disconnect ganged sensor */
                    capsense_CSDDisconnectSns(curSnsIOPtr);
                }
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

            #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
                {
                     /* Scan the next channel */
                    capsense_SsNextFrequencyScan();
                }
                else
                {
                    /* All channels are scanned. Set IMO to zero channel */
                    capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                    capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);

                 /* Scan the next sensor */
                    capsense_SsInitNextGangedScan();
                }
            #else    
                 /* Scan the next sensor */
                capsense_SsInitNextGangedScan();  
            #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
            }
        #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */

#else
    /* CSDv1 part */

    /*******************************************************************************
    * Function Name: capsense_CSDPostSingleScan
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for single-sensor scanning implementation.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_CSDScanExt() function.
    *
    *   The following tasks are performed for CSDv1 HW IP block:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Update the Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The following tasks are performed for CSDv2 HW IP block:
    *   - Check if the raw data is not noisy
    *   - Read the Counter register and update the data structure with raw data
    *   - Configure and start the scan for the next frequency if the
    *         multi-frequency is enabled
    *   - Update the Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler changes IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostSingleScan)
    {
        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

        /* Read SlotResult from Raw Counter */
        capsense_curRamSnsPtr->raw[capsense_scanFreqIndex] = (uint16)CY_GET_REG32(capsense_COUNTER_PTR);

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
        {
            /*  Connect Vref Buffer to AMUX bus. CSDv1 block is enabled */
            CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG_CSD_EN);

            capsense_SsNextFrequencyScan();
        }
        else
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        {
            #if(capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);
            #endif  /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

            #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                /*  Disable CSDv1 block. Connect Vref Buffer to AMUX bus */
                CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG);
            #else
                /*  Connect Vref Buffer to AMUX bus. CSDv1 block is enabled */
                CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG_CSD_EN);
            #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */

            /* Update Scan Counter */
            capsense_dsRam.scanCounter++;

            /* Sensor is totally scanned. Reset BUSY flag */
            capsense_dsRam.status &= ~(capsense_SW_STS_BUSY | capsense_WDGT_SW_STS_BUSY);
        }

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }


    /*******************************************************************************
    * Function Name: capsense_CSDPostMultiScan
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for multiple sensor-scanning implementation.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_Scan() or capsense_ScanAllWidgets() APIs.
    *   The following tasks are performed:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Disable the CSD block (after the widget has been scanned)
    *   - Update Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler initializes scanning for the previous sensor when the widget has
    *   more than one sensor.
    *   The ISR handler initializes scanning for the next widget when the
    *   capsense_ScanAllWidgets() APIs are called and the project has more than
    *   one widget.
    *   The ISR handler changes the IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostMultiScan)
    {
        /*  Declare and initialise ptr to widget and sensor structures to appropriate address        */
        #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
            (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
            capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */

        /*  Declare and initialise ptr to sensor IO structure to appropriate address        */
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                          capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                                          + capsense_sensorIndex;

        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

        /* Read SlotResult from Raw Counter */
        capsense_curRamSnsPtr->raw[capsense_scanFreqIndex] = (uint16)CY_GET_REG32(capsense_COUNTER_PTR);

        /*  Connect Vref Buffer to AMUX bus */
        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG_CSD_EN);

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        /* Disable sensor when all frequency channels are scanned */
        if (capsense_FREQ_CHANNEL_2 == capsense_scanFreqIndex)
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        {
            /* Disable sensor */
            capsense_CSDDisconnectSns(curSnsIOPtr);
        }

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
            {
                capsense_SsNextFrequencyScan();
            }
            else
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
            {
                #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                    capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                    capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);
                #endif  /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

                /* Check if all sensors are scanned in widget */
                if (0u < capsense_sensorIndex)
                {
                    /*  Decrement snsIndex to configure next sensor in widget */
                    capsense_sensorIndex--;

                    /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
                    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                                              capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                                              + capsense_sensorIndex;

                    /*  Initialise ptr to sensor IO structure to appropriate address        */
                    curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                              capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                                              + capsense_sensorIndex;

                    /* Configure PRS mode */
                    #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                         (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE))
                        capsense_CSDConfigurePrsMode();
                    #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                               (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)) */

                    /* Setup Compensation IDAC for next sensor in widget */
                    #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                         (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
                        capsense_SetUpIdacs(ptrWdgt);
                    #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                         (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */

                    /* Enable sensor */
                    capsense_CSDConnectSns(curSnsIOPtr);

                    /* Proceed scanning */
                    capsense_CSDStartSample();
                }
                /*    Call scan next widget API if requested, if not, complete the scan  */
                else
                {
                    /* Current widget is totally scanned. Reset WIDGET BUSY flag */
                    capsense_dsRam.status &= ~capsense_WDGT_SW_STS_BUSY;

                    /* Check if all widgets have been scanned */
                    if (capsense_ENABLE == capsense_requestScanAllWidget)
                    {
                        /* Configure and begin scanning next widget */
                        capsense_PostAllWidgetsScan();
                    }
                    else
                    {
                        #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                            /*  Disable the CSD block. Connect Vref Buffer to AMUX bus */
                            CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG);
                        #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)  */

                        /* All widgets are totally scanned. Reset BUSY flag */
                        capsense_dsRam.status &= ~capsense_SW_STS_BUSY;

                        /* Update scan Counter */
                        capsense_dsRam.scanCounter++;
                    }
                }
            }

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }


    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
    /*******************************************************************************
    * Function Name: capsense_CSDPostMultiScanGanged
    ****************************************************************************//**
    *
    * \brief
    *   This is an internal ISR function for multiple sensor scanning implementation
    *   for ganged sensors.
    *
    * \details
    *   This ISR handler is triggered when the user calls the
    *   capsense_Scan() API for a ganged sensor or the
    *   capsense_ScanAllWidgets() API in the project with ganged sensors.
    *
    *   The following tasks are performed:
    *   - Disable the CSD interrupt
    *   - Read the Counter register and update the data structure with raw data
    *   - Connect the Vref buffer to the AMUX bus
    *   - Disable the CSD block (after the widget has been scanned)
    *   - Update Scan Counter
    *   - Reset the BUSY flag
    *   - Enable the CSD interrupt.
    *
    *   The ISR handler initializes scanning for the previous sensor when the widget has
    *   more than one sensor.
    *   The ISR handler initializes scanning for the next widget when the
    *   capsense_ScanAllWidgets() APIs are called and the project has more than
    *   one widget.
    *   The ISR handler changes the IMO and initializes scanning for the next frequency
    *   channels when multi-frequency scanning is enabled.
    *
    *   This function has two Macro Callbacks that allow to call user code from macros
    *   specified in a component's generated code. Refer to Macro Callbacks section
    *   of PSoC Creator™ User Guide for details.
    *
    *******************************************************************************/
    CY_ISR(capsense_CSDPostMultiScanGanged)
    {
        uint32 tempVal;

        /*  Declare and initialise ptr to widget and sensor structures to appropriate address        */
        #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
            (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
            capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */

        /*  Declare and initialise ptr to sensor IO structure to appropriate address       */
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr =
            (capsense_FLASH_IO_STRUCT const *)capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
            + capsense_sensorIndex;

        #ifdef capsense_ENTRY_CALLBACK
            capsense_EntryCallback();
        #endif /* capsense_ENTRY_CALLBACK */

        /* Clear pending interrupt */
        CY_SET_REG32(capsense_INTR_PTR, capsense_INTR_SET_MASK);

        /* Read SlotResult from Raw Counter */
       capsense_curRamSnsPtr->raw[capsense_scanFreqIndex] = (uint16)CY_GET_REG32(capsense_COUNTER_PTR);

        /*  Connect Vref Buffer to AMUX bus */
        CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG_CSD_EN);

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        if (capsense_FREQ_CHANNEL_2 == capsense_scanFreqIndex)
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
        {
            /* Check ganged sns flag  */
            if ((capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig &
                capsense_GANGED_SNS_MASK) == capsense_GANGED_SNS_MASK)
            {
                /* Get sns pointer */
                capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                   capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash +
                                                   capsense_sensorIndex;

                /* Get number of ganged pins */
                tempVal = capsense_curFlashSnsPtr->numPins;

                /* Get IO pointer  */
                curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

                /* Disconnect all ganged sensors */
                do
                {
                    capsense_CSDDisconnectSns(curSnsIOPtr);
                    curSnsIOPtr++;
                    tempVal--;
                } while (0u != tempVal);
            }
            else
            {
                /* Disconnect ganged sensor */
                capsense_CSDDisconnectSns(curSnsIOPtr);
            }
        }

        #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
            if (capsense_FREQ_CHANNEL_2 > capsense_scanFreqIndex)
            {
                capsense_SsNextFrequencyScan();
            }
            else
        #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
            {
                #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
                    capsense_scanFreqIndex = capsense_FREQ_CHANNEL_0;
                    capsense_ChangeImoFreq(capsense_FREQ_CHANNEL_0);
                #endif  /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

                /* Check if all sensors are scanned in the widget */
                if (0u < capsense_sensorIndex)
                {
                    /*  Decrement snsIndex to configure next sensor in widget */
                    capsense_sensorIndex--;

                    /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
                    capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                                              capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                                              + capsense_sensorIndex;

                    /* Configure PRS mode */
                    #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                         (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE))
                        capsense_CSDConfigurePrsMode();
                    #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                               (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)) */

                    /* Set up Compensation IDAC for next sensor in widget */
                    #if ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                         (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
                        capsense_SetUpIdacs(ptrWdgt);
                    #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) || \
                         (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) */

                    /* Check ganged sns flag  */
                    if (capsense_GANGED_SNS_MASK ==
                       (capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig &
                        capsense_GANGED_SNS_MASK))
                    {
                        /* Get sns pointer */
                        capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                           capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash +
                                                           capsense_sensorIndex;

                        /* Get number of ganged pins */
                        tempVal = capsense_curFlashSnsPtr->numPins;

                        /* Get IO pointer  */
                        curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

                        /* Connect all ganged sensors  */
                        do
                        {
                            capsense_CSDConnectSns(curSnsIOPtr);
                            curSnsIOPtr++;
                            tempVal--;
                        } while (0u != tempVal);
                    }
                    else
                    {
                        /*  Initialize ptr to sensor IO structure to appropriate address       */
                        curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                                       + capsense_sensorIndex;

                        /* Enable sensor */
                        capsense_CSDConnectSns(curSnsIOPtr);
                    }

                    /* Proceed scanning */
                    capsense_CSDStartSample();
                }
                /*    Call scan next widget API if requested, if not, complete scan  */
                else
                {
                    /* Current widget is totally scanned. Reset WIDGET BUSY flag */
                    capsense_dsRam.status &= ~capsense_WDGT_SW_STS_BUSY;

                    /* Check if all widgets have been scanned */
                    if (capsense_ENABLE == capsense_requestScanAllWidget)
                    {
                        /* Configure and begin scanning next widget */
                        capsense_PostAllWidgetsScan();
                    }
                    else
                    {
                        #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                            /*  Disable the CSD block. Connect Vref Buffer to AMUX bus */
                            CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd | capsense_CMOD_PRECHARGE_CONFIG);
                        #endif /* (capsense_ENABLE == capsense_CSDV2) */

                        /* All widgets are totally scanned. Reset BUSY flag */
                        capsense_dsRam.status &= ~capsense_SW_STS_BUSY;

                        /* Update Scan Counter */
                        capsense_dsRam.scanCounter++;
                    }
                }
            }

        #ifdef capsense_EXIT_CALLBACK
            capsense_ExitCallback();
        #endif /* capsense_EXIT_CALLBACK */
    }
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */

#endif /* (capsense_ENABLE == capsense_CSDV2) */
#endif /* ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) */

/** \}
 * \endif */

#if ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN))

#if (capsense_ENABLE == capsense_CSDV2)
    /*******************************************************************************
    * Function Name: capsense_SsPostScan
    ****************************************************************************//**
    *
    * \brief
    *   This function reads rawdata and releases required HW resoures after scan.
    *
    * \details
    *   This function performs following tasks after scan:
    *   - Reads SlotResult from Raw Counter;
    *   - Inits bad Conversions number;
    *   - Disconnects Vrefhi from AMUBUF positive input;
    *   - Disconnects AMUBUF output from CSDBUSB with sych PHI2+HSCMP;
    *   - Opens HCBV and HCBG switches.
    * 
    *******************************************************************************/
    static void capsense_SsPostScan(void)
    {
        uint32 tmpRawData;
        uint32 tmpMaxCount;
        
        /* Read SlotResult from Raw Counter */
        tmpMaxCount = ((1uL << capsense_curWidgetResolution) - 1uL);
        tmpRawData = capsense_RESULT_VAL1_VALUE_MASK & CY_GET_REG32(capsense_COUNTER_PTR);
        
        if(tmpRawData < tmpMaxCount)
        {
            capsense_curRamSnsPtr->raw[capsense_scanFreqIndex] = LO16(tmpRawData);
        }
        else
        {
            capsense_curRamSnsPtr->raw[capsense_scanFreqIndex] = LO16(tmpMaxCount);
        }

        #if (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN)
            /*  Init bad Conversions number */
            capsense_badConversionsNum = capsense_BAD_CONVERSIONS_NUM;
        #endif /* (capsense_ENABLE == capsense_CSD_NOISE_METRIC_EN) */

        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (capsense_2000_MV <= capsense_CYDEV_VDDA_MV))
            /*  Disconnect Vrefhi from AMUBUF positive input. Disconnect AMUBUF output from CSDBUSB with sych PHI2+HSCMP  */
            CY_SET_REG32(capsense_SW_AMUXBUF_SEL_PTR, capsense_SW_AMUXBUF_SEL_SW_DEFAULT);
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (capsense_2000_MV <= capsense_CYDEV_VDDA_MV)) */

        #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
            /* Open HCBV and HCBG switches */
            CY_SET_REG32(capsense_SW_SHIELD_SEL_PTR, capsense_SW_SHIELD_SEL_SW_HCBV_STATIC_OPEN |
                                                             capsense_SW_SHIELD_SEL_SW_HCBG_STATIC_OPEN);
        #endif /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
    }


    /*******************************************************************************
    * Function Name: capsense_SsInitNextScan
    ****************************************************************************//**
    *
    * \brief
    *   This function initializes the next sensor scan.
    *
    * \details
    *   The function decrements the sensor index, updates sense clock for matrix 
    *   or touchpad widgets only, sets up Compensation IDAC, enables the sensor and
    *   scans it. When all sensors are scanned it continues to set up the next widget
    *   until all widgets are scanned. The CSD block is disabled when all widgets are 
    *   scanned.
    * 
    *******************************************************************************/
    CY_INLINE static void capsense_SsInitNextScan(void)
    {
        /*  Declare and initialise ptr to widget and sensor structures to appropriate address */
        #if ((((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
              (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
              (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) || \
             (((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
              (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) && \
              (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)))
            capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                   (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) || \
                   (((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) && \
                   (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE))) */

        /*  Declare and initialise ptr to sensor IO structure to appropriate address        */  
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                          capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash 
                                                          + capsense_sensorIndex;  
                   
        /* Check if all sensors are scanned in widget */
        if (0u < capsense_sensorIndex)
        {
            /*  Decrement snsIndex to configure next sensor in widget */
            capsense_sensorIndex--;

            /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
            capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                                      capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                                      + capsense_sensorIndex; 
            
            /*  Initialise ptr to sensor IO structure to appropriate address        */
            curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                      capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash 
                                                      + capsense_sensorIndex;

            #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
                #if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
                    /* Configure PRS mode */
                    capsense_CSDConfigurePrsMode();
                #else
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
                #endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */      
            #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))  */

            /* Setup Compensation IDAC for next sensor in widget */
            #if (((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                 (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))))
                capsense_SetUpIdacs(ptrWdgt);
            #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                 (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) */
            
            /* Enable sensor */
            capsense_CSDConnectSns(curSnsIOPtr);

            /* Proceed scanning */
            capsense_CSDStartSample();
        }
        /*    Call scan next widget API if requested, if not, complete the scan  */
        else 
        {
            #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                 (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
                /*	Disable shield pins */
                capsense_DisableShieldElectrodes();
            #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                       (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */
            
            /* Current widget is totally scanned. Reset WIDGET BUSY flag */
            capsense_dsRam.status &= ~capsense_WDGT_SW_STS_BUSY;

            /* Check if all widgets have been scanned */
            if (capsense_ENABLE == capsense_requestScanAllWidget)  
            {
                /* Configure and begin scanning next widget */
                capsense_PostAllWidgetsScan();
            }
            else
            {
                #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                    /*  Disable the CSD block */
                    CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
                #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */

                /* All widgets are totally scanned. Reset BUSY flag */
                capsense_dsRam.status &= ~capsense_SW_STS_BUSY;
                
                /* Update scan Counter */
                capsense_dsRam.scanCounter++;
            }
        }
    
    }

    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
    /*******************************************************************************
    * Function Name: capsense_SsInitNextGangedScan
    ****************************************************************************//**
    *
    * \brief
    *   This function initializes the next ganged sensor scan.
    *
    * \details
    *   The function decrements the sensor index, updates sense clock for matrix 
    *   or touchpad widgets only, sets up Compensation IDAC, enables the ganged sensor
    *   and scans it. When all sensors are scanned it continues to set up the next 
    *   widget until all widgets are scanned. The CSD block is disabled when all 
    *   widgets are scanned.
    * 
    *******************************************************************************/
    CY_INLINE static void capsense_SsInitNextGangedScan(void)
    {
        uint32 tempVal;

        /*  Declare and initialise ptr to widget and sensor structures to appropriate address */
        #if ((((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
              (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
              (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) || \
             (((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
              (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) && \
              (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE)))
            capsense_RAM_WD_BASE_STRUCT *ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                                                            capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2WdgtRam;
        #endif /* ((((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                   (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) || \
                   (((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                   (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))) && \
                   (capsense_CLK_SOURCE_DIRECT == capsense_CSD_SNS_CLK_SOURCE))) */

        /*  Declare and initialise ptr to sensor IO structure to appropriate address        */  
        capsense_FLASH_IO_STRUCT const *curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                                          capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash 
                                                          + capsense_sensorIndex;

        /* Check if all sensors are scanned in the widget */
        if (0u < capsense_sensorIndex)
        {
            /*  Decrement snsIndex to configure next sensor in widget */
            capsense_sensorIndex--;

            /*  Update global pointer to capsense_RAM_SNS_STRUCT to current sensor  */
            capsense_curRamSnsPtr = (capsense_RAM_SNS_STRUCT *)
                                                      capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsRam
                                                      + capsense_sensorIndex;

            #if ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))
                #if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
                    /* Configure PRS mode */
                    capsense_CSDConfigurePrsMode();
                #else
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
                #endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */
            #endif /* ((capsense_DISABLE == capsense_CSD_COMMON_SNS_CLK_EN) && \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))  */

            /* Set up Compensation IDAC for next sensor in widget */
            #if (((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                 (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN))))
                capsense_SetUpIdacs(ptrWdgt);
            #endif /* ((capsense_ENABLE == capsense_CSD_IDAC_COMP_EN) && \
                 (capsense_ENABLE == capsense_CSD_DEDICATED_IDAC_COMP_EN) || \
                 (capsense_ENABLE == (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)))) */

            /* Check ganged sns flag  */
            if (capsense_GANGED_SNS_MASK ==
               (capsense_dsFlash.wdgtArray[capsense_widgetIndex].staticConfig &
                capsense_GANGED_SNS_MASK))
            {
                /* Get sns pointer */
                capsense_curFlashSnsPtr = (capsense_FLASH_SNS_STRUCT const *)
                                                   capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash +
                                                   capsense_sensorIndex;

                /* Get number of ganged pins */
                tempVal = capsense_curFlashSnsPtr->numPins;

                /* Get IO pointer  */
                curSnsIOPtr = &capsense_ioList[capsense_curFlashSnsPtr->firstPinId];

                /* Connect all ganged sensors  */
                do
                {
                    capsense_CSDConnectSns(curSnsIOPtr);
                    curSnsIOPtr++;
                    tempVal--;
                } while (0u != tempVal);
            }
            else
            {
                /*  Initialize ptr to sensor IO structure to appropriate address       */
                curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)capsense_dsFlash.wdgtArray[capsense_widgetIndex].ptr2SnsFlash
                               + capsense_sensorIndex;

                /* Enable sensor */
                capsense_CSDConnectSns(curSnsIOPtr);
            }

            /* Proceed scanning */
            capsense_CSDStartSample();
        }
        /*    Call scan next widget API if requested, if not, complete scan  */
        else
        {
            #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                 (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
                /*  Disable shield pins */
                capsense_DisableShieldElectrodes();
            #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                       (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */

            /* Current widget is totally scanned. Reset WIDGET BUSY flag */
            capsense_dsRam.status &= ~capsense_WDGT_SW_STS_BUSY;

            /* Check if all widgets have been scanned */
            if (capsense_ENABLE == capsense_requestScanAllWidget)
            {
                /* Configure and begin scanning next widget */
                capsense_PostAllWidgetsScan();
            }
            else
            {
                #if (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN)
                    /*  Disable the CSD block */
                    CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
                #endif /* (capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) */

                /* All widgets are totally scanned. Reset BUSY flag */
                capsense_dsRam.status &= ~capsense_SW_STS_BUSY;

                /* Update Scan Counter */
                capsense_dsRam.scanCounter++;
            }
        }
    }
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
#endif /* (capsense_ENABLE == capsense_CSDV2) */ 
    

#if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
/*******************************************************************************
* Function Name: capsense_SsNextFrequencyScan
****************************************************************************//**
*
* \brief
*   This function scans the sensor on the next frequency channel.
*
* \details
*   The function increments the frequency channel, changes IMO and initializes 
*   the scanning process of the same sensor.
* 
*******************************************************************************/
static void capsense_SsNextFrequencyScan(void)
{
    capsense_scanFreqIndex++;

    /* Set Immunity */
    capsense_ChangeImoFreq((uint32)capsense_scanFreqIndex);
    
    /* Proceed scanning */
    capsense_CSDStartSample();       
}
#endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */    
#endif /* ((capsense_ENABLE == capsense_CSD_EN) || (capsense_ENABLE == capsense_CSD_CSX_EN)) */    


/* [] END OF FILE */
