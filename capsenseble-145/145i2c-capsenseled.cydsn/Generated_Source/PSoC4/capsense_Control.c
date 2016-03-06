/***************************************************************************//**
* \file capsense_Control.c
* \version 3.0
*
* \brief
* This file provides the source code to the Control module API of the Component.
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
#include "CyLib.h"
#include "cyPm.h"
#include "capsense_Configuration.h"
#include "capsense_Structure.h"
#include "capsense_Control.h"
#include "capsense_Processing.h"
#include "capsense_Filter.h"
#include "capsense_Sensing.h"
#include "capsense_Tuner.h"

#if (0u != capsense_ADC_EN)
    #include "capsense_Adc.h"
#endif /* (0u != capsense_ADC_EN) */
/***********************************************************************************************************************
* Local definition
***********************************************************************************************************************/
#define capsense_INIT_DONE   (1u)
#define capsense_INIT_NEEDED (0u)

/***********************************************************************************************************************
* Local variables
***********************************************************************************************************************/
static uint8 capsense_InitVar = capsense_INIT_NEEDED;

/*******************************************************************************
* Function Name: capsense_Start
****************************************************************************//**
*
* \brief
*  This function initialize component hardware and firmware modules. This API should be called
*   by application program prior to calling any other API of the component.
*
* \details
*   When called, following tasks are executed as part of initialization process:
*   - Initialize values of parameters in capsense_dsRam data structure based on user selection
*     in component configuration wizard.
*   - Configure the hardware to perform capacitive sensing.
*   - Calibrate the sensors and find optimal values for IDACs of each Widgets/sensors, if
*     Enable IDAC auto-calibration is enabled in the CSD Setting or CSX Setting tabs.
*   - If SmartSense Auto-tuning is selected for CSD Tuning mode in Basic tab, auto-tuning algorithm
*     is executed to set optimal values for hardware parameters of widgets/sensors.
*   - Perform a scan for all sensors and initialize the baseline history. During the scan CPU being
*     in the sleep mode to save power.
*   - If firmware filters are enabled in Advanced General tab, the filter histories are also initialized.
*
* \return
*  Returns the status of initialization process. If CYRET_SUCCESS is not received, some of the
*  initialized failed and component may not operate as expected.
*
*******************************************************************************/
cystatus capsense_Start(void)
{
    cystatus result;
    uint8 interruptState;

    /* Initialize CapSense modules */
    result = capsense_Initialize();

    #if (0u != capsense_SELF_TEST_EN)
        if (CYRET_SUCCESS == result)
        {
            result = capsense_RunSelfTest(capsense_TST_SHORTS);

            /* Verify if test succeed */
            if (0u != result)
            {
                result = CYRET_UNKNOWN;
            }
        }
    #endif /* #if (0u != capsense_SELF_TEST_EN) */

    #if (capsense_CSD_AUTOTUNE != capsense_CSD_SS_DIS)
        if (CYRET_SUCCESS == result)
        {
            result = capsense_AutoTune();
        }
    #endif /* #if (capsense_CSD_AUTOTUNE != capsense_CSD_SS_DIS) */

    #if ((0u != capsense_CSX_IDAC_AUTOCAL_EN) || \
        ((capsense_CSD_AUTOTUNE == capsense_CSD_SS_DIS) && (0u != capsense_CSD_IDAC_AUTOCAL_EN)))
        if (CYRET_SUCCESS == result)
        {
            result = capsense_CalibrateAllWidgets();
        }
    #endif

    if (CYRET_SUCCESS == result)
    {
        result = capsense_ScanAllWidgets();
    }

    if (CYRET_SUCCESS == result)
    {
        /* Put CPU into sleep mode while scanning */
        interruptState = CyEnterCriticalSection();
        while(capsense_NOT_BUSY != capsense_IsBusy())
        {
            CySysPmSleep();

            /* Exit from the critical section to unblock processing pending interruprs. */
            CyExitCriticalSection(interruptState);
            interruptState = CyEnterCriticalSection();
        }
        CyExitCriticalSection(interruptState);

        capsense_FtInitialize();
        capsense_InitializeAllBaselines();
    }

    #if (0u != capsense_ADC_EN)
        capsense_AdcInitialize();
    #endif /* (0u != capsense_ADC_EN) */

    return result;
}

/*******************************************************************************
* Function Name: capsense_Initialize
****************************************************************************//**
*
* \brief
*  This function initializes the capsense component.
*
* \details
*  This API initializes all sub-modules of the capsense component:
*   - Data Structure - set the default component parameters defined in the Customizer.
*   - Data Processing - resets all widget statuses.
*   - Tuner - resets tuning state.
*   - Sensing - prepares CSD HW for operation.
*
*  Note that Data Structure module is initialized only once after the reset or
*  capsense_Stop() API is called. The repeated calls of Initialize API
*  will not re-initialize Data Structure. This is done to preserve component
*  parameters that user may set in runtime.
*
*  If default component parameters are needed the capsense_DsInitialize()
*  API must be called directly from the application code.
*
* \return
*  Return CYRET_SUCCESS if the initialization was successful.
*
*******************************************************************************/
cystatus capsense_Initialize(void)
{
    cystatus result;

    /* The Data Structure and Tuner are initialized only once */
    if (capsense_INIT_NEEDED == capsense_InitVar)
    {
        capsense_DsInitialize();
        capsense_TuInitialize();
        capsense_InitVar = capsense_INIT_DONE;
    }

    capsense_DpInitialize();

    result = capsense_SsInitialize();

    return result;
}

/*******************************************************************************
* Function Name: capsense_Stop
****************************************************************************//**
*
* \brief
*  This function stops the component operation.
*
* \details
*  The hardware block reconfigured by application
*  program for any special usage. Once the component is stopped, the operation can be resumed
*  by calling capsense_Resume() function or component can be reset by calling
*  capsense_Start() function. No sensor scanning or self-tests can be executed
*  when component is stopped.
*
* \return
*  Returns if CYRET_SUCCESS stop process is successfully executed.
*
*******************************************************************************/
cystatus capsense_Stop(void)
{
    cystatus result = CYRET_SUCCESS;

    #if (capsense_ENABLE == capsense_ADC_EN)
        /* Release CSD resources */
        result = capsense_SsReleaseResources();

        /* Release ADC resources */
        capsense_AdcStop();
    #endif /* (capsense_ENABLE == capsense_ADC_EN) */

    capsense_InitVar = capsense_INIT_NEEDED;

    return result;
}

/*******************************************************************************
* Function Name: capsense_Resume
****************************************************************************//**
*
* \brief
*  This function resumes the component operation if capsense_Stop()
*  function was called previously.
*
* \details
*   Following tasks are executed as part of operation resume process:
*   - Configure the hardware to perform capacitive sensing.
*   - Reset all Widgets/Sensors status.
*
* \return
*  Returns the status of resume process. If CYRET_SUCCESS is not received,
*  resume process failed and component may not operate as expected.
*
*******************************************************************************/
cystatus capsense_Resume(void)
{
    cystatus result;

    capsense_DpInitialize();
    capsense_TuInitialize();

    result = capsense_SsInitialize();

    capsense_InitVar = capsense_INIT_DONE;

    return result;
}


/*******************************************************************************
* Function Name: capsense_ProcessAllWidgets
****************************************************************************//**
*
* \brief
*   Performs full data processing on all enabled widgets.
*
* \details
*   The following tasks are executed as part of processing all widgets:
*   1. Apply filter to Raw Signal
*   2. Update thresholds if FW Auto Tuning is enabled
*   3. Update Baselines and differences
*   4. Update Status (on/off, centroid, X/Y position)
*
* \return
*   Returns CYRET_SUCCESS.
*
*******************************************************************************/
cystatus capsense_ProcessAllWidgets(void)
{
    uint32 wdgtId;
    cystatus result = CYRET_SUCCESS;
    capsense_FLASH_WD_STRUCT const *ptrFlashWdgt;

    ptrFlashWdgt = capsense_dsFlash.wdgtArray;

    for (wdgtId = 0u; wdgtId < capsense_TOTAL_WIDGETS; wdgtId++)
    {
        if (0u != capsense_GET_WIDGET_EN_STATUS(wdgtId))
        {
            switch(capsense_GET_SENSE_METHOD(ptrFlashWdgt))
            {
            #if (0u != capsense_TOTAL_CSD_WIDGETS)
                case capsense_SENSE_METHOD_CSD_E:
                    capsense_DpProcessCsdWidgetRawCounts(ptrFlashWdgt);
                    capsense_DpProcessCsdWidgetStatus(wdgtId, ptrFlashWdgt);
                    break;
            #endif /* #if (0u != capsense_TOTAL_CSD_WIDGETS) */

            #if (0u != capsense_TOTAL_CSX_WIDGETS)
                case capsense_SENSE_METHOD_CSX_E:
                    capsense_DpProcessCsxWidgetRawCounts(ptrFlashWdgt);
                    capsense_DpProcessCsxWidgetStatus(wdgtId, ptrFlashWdgt);
                    break;
            #endif /* #if (0u != capsense_TOTAL_CSX_WIDGETS) */

            default:
                CYASSERT(0u);
                break;
            }
        }

        ptrFlashWdgt++;
    }

    return result;
}


/*******************************************************************************
* Function Name: capsense_ProcessWidget
****************************************************************************//**
*
* \brief
*   Performs full data processing of the selected widget if it is enabled.
*
* \details
*   The following tasks are executed as part of process a widget:
*   1. Apply filter to Raw signal
*   2. Update thresholds if FW Auto Tuning is enabled
*   3. Update Baselines and differences
*   4. Update Status (on/off, centroid, X/Y position)
*
* \param widgetId The widget index number.
*
* \return
*   Returns CYRET_SUCCESS if operation was successfully completed.
*   Returns CYRET_BAD_PARAM if the widgetId index is wrong.
*   Returns CYRET_INVALID_STATE if the specified widget is disabled or
*   if the Self Test has detected some issues with the selected widget.
*
*******************************************************************************/
cystatus capsense_ProcessWidget(uint32 widgetId)
{
    cystatus result = CYRET_SUCCESS;
    capsense_FLASH_WD_STRUCT const *ptrFlashWdgt;

    if (widgetId >= capsense_TOTAL_WIDGETS)
    {
        result = CYRET_BAD_PARAM;
    }

    if ((CYRET_SUCCESS == result) && (0u == capsense_GET_WIDGET_EN_STATUS(widgetId)))
    {
        result = CYRET_INVALID_STATE;
    }

    if (CYRET_SUCCESS == result)
    {
        ptrFlashWdgt = &capsense_dsFlash.wdgtArray[widgetId];

        switch(capsense_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != capsense_TOTAL_CSD_WIDGETS)
            case capsense_SENSE_METHOD_CSD_E:
                capsense_DpProcessCsdWidgetRawCounts(ptrFlashWdgt);
                capsense_DpProcessCsdWidgetStatus(widgetId, ptrFlashWdgt);
                break;
        #endif /* #if (0u != capsense_TOTAL_CSD_WIDGETS) */

        #if (0u != capsense_TOTAL_CSX_WIDGETS)
            case capsense_SENSE_METHOD_CSX_E:
                capsense_DpProcessCsxWidgetRawCounts(ptrFlashWdgt);
                capsense_DpProcessCsxWidgetStatus(widgetId, ptrFlashWdgt);
                break;
        #endif /* #if (0u != capsense_TOTAL_CSX_WIDGETS) */

        default:
            CYASSERT(0u);
            break;
        }
    }

    return result;
}


/*******************************************************************************
* Function Name: capsense_ProcessWidgetExt
****************************************************************************//**
*
* \brief
*   Performs customized data processing on the selected widget.
*
* \details
*   The execution order of the requested operations is from LSB to MSB.
*   If a different order is needed, this API can be called multiple
*   times with the required mode parameter.
*
*   The capsense_PROCESS_CALC_NOISE and capsense_PROCESS_THRESHOLDS
*   flags are not supported by the CSX widgets.
*
*   The capsense_PROCESS_DECONVOLUTION flag is supported only by the CSX widgets
*   with the multiphase TX scan mode. For such widgets it is mandatory to execute this task
*   as the first processing step to get the correct raw counts. Otherwise any further processing
*   will make no sense.
*
*   Note that this API ignores values of the wdgtEnable and wdgtWorking registers.
*
* \param widgetId The widget index number.
* \param mode Specify the type of process that needs to be executed for the widget:
*    - Bits [31..7] - Reserved
*    - Bits [6..0] - capsense_PROCESS_ALL - Execute all tasks
*    - Bit [6] - capsense_PROCESS_DECONVOLUTION - Runs the deconvolution algorithm
*               if widget supports this task
*    - Bit [5] - capsense_PROCESS_STATUS - Update Status (on/off, position)
*    - Bit [4] - capsense_PROCESS_THRESHOLDS - Update Thresholds (only in auto-tuning mode)
*    - Bit [3] - capsense_PROCESS_CALC_NOISE - Calculate Noise (only in auto-tuning mode)
*    - Bit [2] - capsense_PROCESS_DIFFCOUNTS - Update Difference Counts
*    - Bit [1] - capsense_PROCESS_BASELINE - Update Baselines
*    - Bit [0] - capsense_PROCESS_FILTER - Run Firmware Filter
*
* \return
*   Returns CYRET_SUCCESS in case of success or CYRET_BAD_PARAM in case of wrong
*   widgetId parameter.
*
*******************************************************************************/
cystatus capsense_ProcessWidgetExt(uint32 widgetId, uint32 mode)
{
    uint32 snsCount;
    cystatus result = CYRET_BAD_PARAM;
    capsense_FLASH_WD_STRUCT const *ptrFlashWdgt;
    capsense_PTR_FILTER_VARIANT fltrHistV;
    capsense_RAM_SNS_STRUCT *ptrSns;

    #if (0u != capsense_TOTAL_CSD_WIDGETS)
        uint32 isProxWdgt;
    #endif

    if (widgetId < capsense_TOTAL_WIDGETS)
    {
        ptrFlashWdgt = &capsense_dsFlash.wdgtArray[widgetId];
        snsCount = capsense_GET_SNS_CNT_BY_PTR(ptrFlashWdgt);
        ptrSns = ptrFlashWdgt->ptr2SnsRam;
        fltrHistV.ptr = ptrFlashWdgt->ptr2FltrHistory;

        switch(capsense_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != capsense_TOTAL_CSD_WIDGETS)
            case capsense_SENSE_METHOD_CSD_E:
            {
                /* Determine if widget is type of proximity.
                 * The Proximity widgets use different filters and
                 * therefore have different filter history object structure */
                isProxWdgt =
                    (capsense_GET_WIDGET_TYPE(ptrFlashWdgt) == capsense_WD_PROXIMITY_E) ? 1Lu : 0Lu;

                /* Run the desired processing for the all CSD widget sensors */
                for (;snsCount-- > 0u;)
                {
                    capsense_DpProcessCsdSensorRawCountsExt(ptrFlashWdgt, ptrSns, fltrHistV, mode);

                    /* Move to the next sensor and filter history objects */
                    ptrSns++;
                    capsense_INC_FLTR_OBJ_VARIANT(isProxWdgt, fltrHistV);
                }

                if (0u != (mode & capsense_PROCESS_STATUS))
                {
                    capsense_DpProcessCsdWidgetStatus(widgetId, ptrFlashWdgt);
                }
                break;
            }
        #endif /* #if (0u != capsense_TOTAL_CSD_WIDGETS) */

        #if (0u != capsense_TOTAL_CSX_WIDGETS)
            case capsense_SENSE_METHOD_CSX_E:

                #if (0u != capsense_CSX_MULTIPHASE_TX_EN)
                    if ((0u != (mode & capsense_PROCESS_DECONVOLUTION)) &&
                        (0u != (ptrFlashWdgt->staticConfig & capsense_MULTIPHASE_TX_MASK)))
                    {
                        capsense_DpDeconvolution(ptrFlashWdgt);
                    }
                #endif /* #if (0u != capsense_CSX_MULTIPHASE_TX_EN) */

                /* Run the desired processing for the all CSX widget sensors */
                for (;snsCount-- > 0u;)
                {
                    capsense_DpProcessCsxSensorRawCountsExt(ptrFlashWdgt, ptrSns, fltrHistV, mode);

                    /* Move to the next sensor and filter history objects */
                    ptrSns++;
                    capsense_INC_FLTR_OBJ_VARIANT(0u, fltrHistV);
                }

                if (0u != (mode & capsense_PROCESS_STATUS))
                {
                    capsense_DpProcessCsxWidgetStatus(widgetId, ptrFlashWdgt);
                }
                break;
        #endif /* #if (0u != capsense_TOTAL_CSX_WIDGETS) */

        default:
            CYASSERT(0u);
            break;
        }

        result = CYRET_SUCCESS;
    }

    return result;
}

/*******************************************************************************
* Function Name: capsense_ProcessSensorExt
****************************************************************************//**
*
* \brief
*   Performs customized data processing on the selected widget's sensor
*   after scanning.
*
* \details
*   The execution order of the requested operations is the same as in the
*   provided list. If a different order is needed, this API can be called multiple
*   times with the required mode parameter.
*
*   The capsense_PROCESS_CALC_NOISE and capsense_PROCESS_THRESHOLDS
*   flags are not supported by the CSX widgets.
*
*   Note that this API ignores values of the wdgtEnable and wdgtWorking registers.
*
* \param widgetId  The widget index number.
* \param sensorId  The sensor index number within the widget.
* \param mode      The bit-mask with data processing tasks to be executed.
*   The mode parameter can take the following values:
*    - capsense_PROCESS_FILTER     Run Firmware Filter
*    - capsense_PROCESS_BASELINE   Update Baselines
*    - capsense_PROCESS_DIFFCOUNTS Update Difference Counts
*    - capsense_PROCESS_CALC_NOISE Calculate Noise (only in auto-tuning mode)
*    - capsense_PROCESS_THRESHOLDS Update Thresholds (only in auto-tuning mode)
*    - capsense_PROCESS_ALL        Execute all tasks
*
* \return
*   Returns CYRET_SUCCESS in case of success or CYRET_BAD_PARAM in case of wrong
*   widgetId or sensorId parameters.
*
*******************************************************************************/
cystatus capsense_ProcessSensorExt(uint32 widgetId, uint32 sensorId, uint32 mode)
{
    cystatus result = CYRET_BAD_PARAM;
    capsense_FLASH_WD_STRUCT const *ptrFlashWdgt;
    capsense_PTR_FILTER_VARIANT fltrHistV;
    capsense_RAM_SNS_STRUCT *ptrSns;

    if ((widgetId < capsense_TOTAL_WIDGETS) && (sensorId < capsense_GET_SENSOR_COUNT(widgetId)))
    {
        ptrFlashWdgt = &capsense_dsFlash.wdgtArray[widgetId];
        fltrHistV.ptr = ptrFlashWdgt->ptr2FltrHistory;
        ptrSns = ptrFlashWdgt->ptr2SnsRam;
        ptrSns += sensorId;

        switch(capsense_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != capsense_TOTAL_CSD_WIDGETS)
            case capsense_SENSE_METHOD_CSD_E:
                if (capsense_WD_PROXIMITY_E == (capsense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType)
                {
                    #if (0u != capsense_PROX_RC_FILTER_EN)
                        fltrHistV.ptr = &fltrHistV.ptrProx[sensorId];
                    #endif /* #if (0u != capsense_PROX_RC_FILTER_EN) */
                }
                else
                {
                    #if (0u != capsense_REGULAR_RC_FILTER_EN)
                        fltrHistV.ptr = &fltrHistV.ptrRegular[sensorId];
                    #endif /* #if (0u != capsense_REGULAR_RC_FILTER_EN) */
                }

                capsense_DpProcessCsdSensorRawCountsExt(ptrFlashWdgt, ptrSns, fltrHistV, mode);
                break;
        #endif /* #if (0u != capsense_TOTAL_CSD_WIDGETS) */

        #if (0u != capsense_TOTAL_CSX_WIDGETS)
            case capsense_SENSE_METHOD_CSX_E:
                #if (0u != capsense_REGULAR_RC_FILTER_EN)
                    fltrHistV.ptr = &fltrHistV.ptrRegular[sensorId];
                #endif /* #if (0u != capsense_REGULAR_RC_FILTER_EN) */

                capsense_DpProcessCsxSensorRawCountsExt(ptrFlashWdgt, ptrSns, fltrHistV, mode);
                break;
        #endif /* #if (0u != capsense_TOTAL_CSX_WIDGETS) */

        default:
            CYASSERT(0u);
            break;
        }

        result = CYRET_SUCCESS;
    }

    return result;
}

#if (0u != capsense_SELF_TEST_EN)
/*******************************************************************************
* Function Name: capsense_RunSelfTest
****************************************************************************//**
*
* \brief
*  Run-self tests based on the test-enable mask.
*
* \details
*  The test bit-mask can take the following values:
*   - capsense_TST_SHORTS        - A test for the sensor shorts.
*   - capsense_TST_GLOBAL_CRC    - Verify CRC of RAM and FLASH global parameters.
*   - capsense_TST_BSL_RAW_LIMTS - Verify Baseline and Raw Counts limits.
*   - capsense_TST_WIDGET_CRC    - Verify CRC of the widget's configuration in RAM.
*   - capsense_TST_CSD_CAPS      - Measure the Cmod and Ctank capacitance.
*   - capsense_TST_CSX_CAPS      - Measure the Cint1 and Cint2 capacitance.
*   - capsense_TST_BASELINES     - Verify the baselines consistency (inverse copy).
*
*  The results of the capacitance measurements are stored into the corresponding
*  registers of the data structure.
*
* \param testEnMask The bit-mask with the requested tests to be executed.
*
* \return
*  Bitmask that contains 0 for passed tests (or not selected with the testEnMask)
*  and 1 for the failed tests.
*
*******************************************************************************/
uint32 capsense_RunSelfTest(uint32 testEnMask)
{
    /* The implementation of this API will be
     * provided in the FUTURE version of the
     * CapSense component */
    (void)testEnMask;
    return 0Lu;
}
#endif /* #if capsense_SELF_TEST_EN */


/*******************************************************************************
* Function Name: capsense_Sleep
****************************************************************************//**
*
* \brief
*  This function sets the component to sleep.
*
* \details
*  This function sets the component to sleep.
*
*******************************************************************************/
void capsense_Sleep(void)
{
}


/*******************************************************************************
* Function Name: capsense_Wakeup
****************************************************************************//**
*
* \brief
*  This function resumes the component after sleep.
*
* \details
*  This function resumes the component after sleep.
*
*******************************************************************************/
void capsense_Wakeup(void)
{
}


/*******************************************************************************
* Function Name: capsense_SaveConfig
****************************************************************************//**
*
* \brief
*   This is a dummy API for the consistency reason with the component PM architecture.
*
* \details
*   This is a dummy API for the consistency reason with the component PM architecture.
*
*******************************************************************************/
void capsense_SaveConfig(void)
{
}


/*******************************************************************************
* Function Name: capsense_RestoreConfig
****************************************************************************//**
*
* \brief
*   This is a dummy API for the consistency reason with the component PM architecture.
*
* \details
*   This is a dummy API for the consistency reason with the component PM architecture.
*
*******************************************************************************/
void capsense_RestoreConfig(void)
{
}


/* [] END OF FILE */
