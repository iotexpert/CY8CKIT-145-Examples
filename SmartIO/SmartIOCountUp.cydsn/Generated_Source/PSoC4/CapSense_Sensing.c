/***************************************************************************//**
* \file CapSense_Sensing.c
* \version 3.0
*
* \brief
*   This file contains the source of functions common for
*   different sensing methods. 
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

#include "CapSense_Sensing.h"
#include "cyfitter.h"

/***************************************
* API Constants
***************************************/

#define CapSense_WIDGET_NUM_32                          (32u)
#define CapSense_WIDGET_NUM_32_DIV_SHIFT                (5u)
#define CapSense_WIDGET_NUM_32_MASK                     (0x0000001FLu)
#define CapSense_CALIBRATION_RESOLUTION                 (12u)
#define CapSense_COARSE_TRIM_THRESHOLD_1                (40u)
#define CapSense_COARSE_TRIM_THRESHOLD_2                (215u)
#define CapSense_FREQUENCY_OFFSET_5                     (20u)
#define CapSense_FREQUENCY_OFFSET_10                    (40u)
#define CapSense_CALIBRATION_FREQ_KHZ                   (1500u)
#define CapSense_CALIBRATION_MD                         (2u)
#define CapSense_MIN_IMO_FREQ_KHZ                       (6000u)
#define CapSense_CSD_AUTOTUNE_CAL_LEVEL                 (CapSense_CSD_RAWCOUNT_CAL_LEVEL)
#define CapSense_CP_MIN                                 (0u)
#define CapSense_CP_MAX                                 (65000u)
#define CapSense_CP_ERROR                               (4000u)

#if (CapSense_CLK_SOURCE_DIRECT != CapSense_CSD_SNS_CLK_SOURCE)
    #if (CapSense_ENABLE == CapSense_CSDV2)
        #define CapSense_PRS_FACTOR_DIV                 (2u)
    #else  
        #define CapSense_PRS_FACTOR_DIV                 (1u)
    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
#else
    #define CapSense_PRS_FACTOR_DIV                     (0u)
#endif /* (CapSense_CLK_SOURCE_DIRECT != CapSense_CSD_SNS_CLK_SOURCE) */

#define CapSense_FLIP_FLOP_DIV                          (1u)

#define CapSense_MOD_CSD_CLK_12000KHZ                   (12000uL)
#define CapSense_MOD_CSD_CLK_24000KHZ                   (24000uL)
#define CapSense_MOD_CSD_CLK_48000KHZ                   (48000uL)

#if ((CapSense_CLK_SOURCE_PRS8 == CapSense_CSD_SNS_CLK_SOURCE) || \
    (CapSense_CLK_SOURCE_PRS12 == CapSense_CSD_SNS_CLK_SOURCE) || \
    (CapSense_CLK_SOURCE_PRSAUTO == CapSense_CSD_SNS_CLK_SOURCE))
    #define CapSense_FACTOR_FILTER_DELAY_12MHZ          (2u)
#else
    #define CapSense_FACTOR_FILTER_DELAY_12MHZ          (4u)
#endif /* (CapSense_CLK_SOURCE_DIRECT != CapSense_CSD_SNS_CLK_SOURCE) */

#define CapSense_FACTOR_MOD_SNS                         (8u)

/*****************************************************************************/
/* Enumeration types definition                                               */
/*****************************************************************************/

typedef enum
{
    CapSense_RES_PULLUP_E   = 0x02u,
    CapSense_RES_PULLDOWN_E = 0x03u
} CapSense_PORT_TEST_DM;

typedef enum
{
    CapSense_STS_RESET      = 0x01u,
    CapSense_STS_NO_RESET   = 0x02u
} CapSense_TEST_TYPE;


/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/


/** \}
* \endif */

/*******************************************************************************
* Define module variables
*******************************************************************************/

#if ((CapSense_ENABLE == CapSense_CSD_CSX_EN) || \
     (CapSense_ENABLE == CapSense_ADC_EN))
    CapSense_SENSE_METHOD_ENUM CapSense_currentSenseMethod = CapSense_UNDEFINED_E;
#endif /* ((CapSense_ENABLE == CapSense_CSD_CSX_EN) || \
           (CapSense_ENABLE == CapSense_ADC_EN))) */

#if(CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN)
    /*  Module variable keep track of frequency hopping channel index   */
    uint8 CapSense_scanFreqIndex = 0u;
    /*  Variable keep frequency offsets */
    uint8 CapSense_immunity[CapSense_NUM_SCAN_FREQS] = {0u, 0u, 0u};
#else
    /* const allows C-compiler to do optimization */
    const uint8 CapSense_scanFreqIndex = 0u;
#endif /* (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN) */

/* Global software variables */
volatile uint8 CapSense_widgetIndex = 0u;    /* Index of the scanning widget */
volatile uint8 CapSense_sensorIndex = 0u;    /* Index of the scanning sensor */
volatile uint32 CapSense_curWidgetResolution = 0u;
uint8 CapSense_requestScanAllWidget = 0u;
#if (CapSense_CSD_SS_DIS != CapSense_CSD_AUTOTUNE)
    uint8 CapSense_prescalersTuningDone = 0u;
#endif /* (CapSense_CSD_SS_DIS != CapSense_CSD_AUTOTUNE) */

/* Pointer to RAM_SNS_STRUCT structure  */
CapSense_RAM_SNS_STRUCT *CapSense_curRamSnsPtr;

#if ((CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) || \
     (CapSense_ENABLE == CapSense_CSX_EN))
    /*  Pointer to Flash structure holding configuration of widget to be scanned  */
    CapSense_FLASH_WD_STRUCT const *CapSense_curFlashWdgtPtr = 0u;
#endif /* ((CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) || \
           (CapSense_ENABLE == CapSense_CSX_EN))  */
#if (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) 
    /*  Pointer to Flash structure holding info of sensor to be scanned  */
    CapSense_FLASH_SNS_STRUCT const *CapSense_curFlashSnsPtr = 0u;
#endif /* (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) */

/*******************************************************************************
* Function Name: CapSense_IsBusy
****************************************************************************//**
*
* \brief
*   Returns the current status of the component (scan completed or scan in
*   progress).
*
* \details
*   This API checks the status flag in the CapSense_dsRam structure
*   which is set when the scanning starts. The API returns a zero value when all the
*   sensors in the widget have been scanned.
*   In the CSD2X mode the API returns a zero value when all the sensors in both slots
*   have been scanned.
*   A new scan or the widget setup change must not be made while
*   the component is busy.
*
* \return
*   Returns the current status of the component (scan completed or scan in
*   progress):
*   Zero     - Free, no scan in progress, a next scan can be initiated.
*   Non-zero - Busy, scan in progress, wait prior to initiating a next scan.
*
*******************************************************************************/
uint32 CapSense_IsBusy(void)
{
    return ((*(volatile uint8 *)&CapSense_dsRam.status) & CapSense_SW_STS_BUSY);
}

/*******************************************************************************
* Function Name: CapSense_SetupWidget
****************************************************************************//**
*
* \brief
*   Performs the initialization required to scan a particular widget.
*
* \details
*   This API re-initializes the component hardware if the sensing methods used by
*   the widget are different from the sensing methods used by the previous widget, 
*   initializes the hardware with configurations specific to the widget. 
*   This API does not start sensor scanning. This API must be called prior 
*   calling CapSense_Scan() API to start the sensor scan.
* 
*   Note in the CSD2x mode, each widget should be set up individually with two calls to the
*   CapSense_SetupWidget() API.
*   This API disconnects the electrode if it was previously connected by 
*   CapSense_CSDSetupWidgetExt() or CapSense_CSXSetupWidgetExt() 
*   APIs. This API does not disconnect the electrode if it was connected manually or
*   using CapSense_CSDConnectSns() API.
*   The last widget will be configured if this API is called twice for different
*   widgets. API does not break the functionality if it is called twice for the 
*   same widget.
*
* \param wdgtIndex
*   Specifies the ID of the widget for which the hardware tuning parameters should be
*   initialized in the CapSense HW block.
*
* \return 
*   Returns the status of operation:
*      - Zero     - The CapSense HW block is initialized.
*      - Non-zero - The CapSense HW block is busy and cannot be initialized.
*
**********************************************************************************/
cystatus CapSense_SetupWidget(uint32 wdgtIndex)
{
    cystatus widgetStatus;

    if (CapSense_WDGT_SW_STS_BUSY == (CapSense_dsRam.status & CapSense_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        widgetStatus = CYRET_INVALID_STATE;
    }
    /*  
     *  Check if widget id is valid, specified widget is enabled and widget did not
     *  detect any fault conditions if BIST is enabled. If all conditions are met,
     *  set widgetStatus as good, if not, set widgetStatus as bad.   
     */
        else if ((CapSense_TOTAL_WIDGETS > wdgtIndex) &&
            (((uint32)CapSense_dsRam.wdgtEnable[(uint8)(wdgtIndex >> CapSense_WIDGET_NUM_32_DIV_SHIFT)] & 
                                                (0x00000001Lu << (wdgtIndex & CapSense_WIDGET_NUM_32_MASK))) != CapSense_DISABLE))
    
        {
            #if (CapSense_ENABLE == CapSense_SELF_TEST_EN)
                if (CapSense_DISABLE != ((uint32)CapSense_dsRam.wdgtWorking[(uint8)(wdgtIndex >> CapSense_WIDGET_NUM_32_DIV_SHIFT)] & 
                                                 (0x00000001Lu << (wdgtIndex & CapSense_WIDGET_NUM_32_MASK))))    
                {
                    widgetStatus = CYRET_SUCCESS;
                }
                else    
                {
                    widgetStatus = CYRET_INVALID_STATE;
                }
            #else
                widgetStatus = CYRET_SUCCESS;
            #endif  /* (CapSense_ENABLE == CapSense_SELF_TEST_EN)) */
        }
        else    
        {
            widgetStatus = CYRET_BAD_PARAM;
        }
    
    /*  
     * Check widgetStatus flag that is set earlier, if flag is good, then set up only
     * widget  
     */
    if (CYRET_SUCCESS == widgetStatus)    
    {
        /*  Check if CSD2X is enabled   */
        #if (CapSense_ENABLE == CapSense_CSD2X_EN)
            /******************************
            * This is the place where CapSense_SetupWidget2x(uint32 slotIndex) 
            * API should be implemented.
            * CSD2X will be implemented
            * in the next component version.
            *********************************/
            widgetStatus = CYRET_UNKNOWN;
            #error "CSD2x not implemented in this version of component"
            
        #elif (CapSense_ENABLE == CapSense_CSD_CSX_EN)
            #if (CapSense_ENABLE == CapSense_ADC_EN)
                if (CapSense_UNDEFINED_E == CapSense_currentSenseMethod) 
                {
                    /* Release ADC resources */
                    widgetStatus = CapSense_AdcReleaseResources();
                }
            #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */
            
            /*  Check widget sensing method is CSX and call CSX APIs    */
            if (CapSense_SENSE_METHOD_CSX_E == 
                (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))
            {
                /*  
                 *  Check if CSD block is  already initialized for CSX operation
                 *  If not, initialize CSD hardware block for CSX measurement   
                 */
                if (CapSense_SENSE_METHOD_CSX_E != CapSense_currentSenseMethod) 
                {
                    CapSense_SwitchSensingMode(CapSense_SENSE_METHOD_CSX_E);
                }

                #if (CapSense_ENABLE == CapSense_ADC_EN)
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /*  Set up widget for CSX scan  */
                        CapSense_CSXSetupWidget(wdgtIndex);
                    }
                #else
                    /*  Set up widget for CSX scan  */
                    CapSense_CSXSetupWidget(wdgtIndex);
                #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */
            }
            /*  Check widget sensing method is CSD and call appropriate API */
            else if (CapSense_SENSE_METHOD_CSD_E ==
                     (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))
            {
                /*  
                 * Check if CSD block is  already initialized for CSD operation
                 * If not, initialize CSD hardware block for CSD measurement   
                 */
                if (CapSense_SENSE_METHOD_CSD_E != CapSense_currentSenseMethod)
                {
                    CapSense_SwitchSensingMode(CapSense_SENSE_METHOD_CSD_E);
                }

                #if (CapSense_ENABLE == CapSense_ADC_EN)
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /*  Set up widget for CSD scan  */
                        CapSense_CSDSetupWidget(wdgtIndex);
                    }
                #else
                    /*  Set up widget for CSD scan  */
                    CapSense_CSDSetupWidget(wdgtIndex);
                #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */
            }
            else
            {
                /*  Sensing method is invalid, return error to caller  */
                widgetStatus = CYRET_UNKNOWN;
            }
        #elif (CapSense_ENABLE == CapSense_CSD_EN)
            #if (CapSense_ENABLE == CapSense_ADC_EN)
                /*  
                 * Check if CSD block is  already initialized for CSD operation
                 * If not, initialize CSD hardware block for CSD measurement   
                 */
                if (CapSense_SENSE_METHOD_CSD_E != CapSense_currentSenseMethod)
                {
                    if (CapSense_UNDEFINED_E == CapSense_currentSenseMethod) 
                    {
                        /* Release ADC resources */
                        widgetStatus = CapSense_AdcReleaseResources();
                    }

                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /* Initialize CSD mode to guarantee configured CSD mode after CSX calibration */
                        CapSense_CSDInitialize();
                        CapSense_currentSenseMethod = CapSense_SENSE_METHOD_CSD_E;
                    }
                }
                
                if (CYRET_SUCCESS == widgetStatus)
                {
                    /*  Set up widget for scan */
                    CapSense_CSDSetupWidget(wdgtIndex);  
                }
            #else    
                /*  Set up widget for scan */
                CapSense_CSDSetupWidget(wdgtIndex);   
            #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */
        #elif (CapSense_ENABLE == CapSense_CSX_EN)
            #if (CapSense_ENABLE == CapSense_ADC_EN)
                /*  
                 *  Check if CSD block is  already initialized for CSX operation
                 *  If not, initialize CSD hardware block for CSX measurement   
                 */
                if (CapSense_SENSE_METHOD_CSX_E != CapSense_currentSenseMethod) 
                {
                    if (CapSense_UNDEFINED_E == CapSense_currentSenseMethod) 
                    {
                        /* Release ADC resources */
                        widgetStatus = CapSense_AdcReleaseResources();
                    }
                    
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /* Initialize CSD hardware block for CSX measurement */
                        CapSense_CSXInitialize();
                        CapSense_currentSenseMethod = CapSense_SENSE_METHOD_CSX_E;
                    }
                }
                
                if (CYRET_SUCCESS == widgetStatus)
                {
                    /*  Set up widgets for scan     */
                    CapSense_CSXSetupWidget(wdgtIndex);
                }
            #else
                /*  Set up widgets for scan     */
                CapSense_CSXSetupWidget(wdgtIndex);
            #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */
        #else
            widgetStatus = CYRET_UNKNOWN;
            #error "No sensing method enabled, component cannot work in this mode"
        #endif  /* (CapSense_ENABLE == CapSense_CSD2X_EN) */
    }

    return (widgetStatus);
}


/*******************************************************************************
* Function Name: CapSense_Scan
****************************************************************************//**
*
* \brief
*   Initiates scan for all sensors in the widget which is initialized by 
*   CapSense_SetupWidget(), if the no scan is in progress
*
* \details
*   In the CSD mode, this API initializes the widget scanning in the descending order.
*   In the CSX mode, this API initializes the widget scanning in the ascending order. 
*   It connects the sensor in the widget, sets up a callback for the ISR
*   and starts scan of all the sensors in the widget which was set up in
*   the CapSense_SetupWidget() API.
*   This API returns to the caller after scanning of the sensor in the 
*   widget is started, and scanning of the remaining sensors is started in the ISR. 
*   The status of scanning must be checked using the CapSense_IsBusy() API  
*   prior to starting next scan or setting up a widget.
*
*   Note that the CapSense HW block should be configured using  
*   the CapSense_SetupWidget() API prior to calling this API.
*
* \return 
*   Returns the status of operation:
*      - Zero     - The CapSense scan is started.
*      - Non-zero - The CapSense HW block is busy and scan has not started.
*
**********************************************************************************/
cystatus CapSense_Scan(void)
{
    cystatus scanStatus = CYRET_SUCCESS;

    if (CapSense_WDGT_SW_STS_BUSY == (CapSense_dsRam.status & CapSense_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        scanStatus = CYRET_INVALID_STATE;
    }
    else
    {
        /*  If CSD2X is enabled, call CSD2X scan    */
    #if (CapSense_ENABLE == CapSense_CSD2X_EN)
        scanStatus = CapSense_Scan2x();

    /*  If both CSD and CSX are enabled, call scan API based on widget sensing method    */
    #elif (CapSense_ENABLE == CapSense_CSD_CSX_EN)
        /*  Check widget sensing method and call appropriate APIs   */
        switch (CapSense_currentSenseMethod)
        {  
            case CapSense_SENSE_METHOD_CSX_E:
                CapSense_CSXScan();
                break;

            case CapSense_SENSE_METHOD_CSD_E:
                 CapSense_CSDScan();
                break;

            default:
                scanStatus = CYRET_UNKNOWN;
                break;
        }
        
    /*  If only CSD is enabled, call CSD scan   */
    #elif (CapSense_ENABLE == CapSense_CSD_EN)
        CapSense_CSDScan();

    /*  If only CSX is enabled, call CSX scan   */
    #elif (CapSense_ENABLE == CapSense_CSX_EN)
        CapSense_CSXScan();

    #else
        scanStatus = CYRET_UNKNOWN;
        #error "No sensing method enabled, component cannot work in this mode"
    #endif  /* (CapSense_ENABLE == CapSense_CSD2X_EN) */
    }

    return (scanStatus);
}


/*******************************************************************************
* Function Name: CapSense_ScanAllWidgets
****************************************************************************//**
*
* \brief
*   Initializes a widget and scans all the sensors in the widget, then the same 
*   process is repeated for all widgets in the component. i.e. scan all the 
*   widgets in the component.
*
* \details
*   Initializes a widget and then initiates scan for the sensor in the
*   widget and returns to the caller. At the end of the sensor scan, the ISR is 
*   triggered and the following sensor scan is initiated in the ISR. Once all the 
*   sensors are scanned, the next widget in the component is initialized and the  
*   process is repeated  until all the sensors in all widgets in the component 
*   are scanned.
*   This function does not require the CapSense_Scan() or 
*   CapSense_SetupWidget(uint32 wdgtIndex) APIs to be called to start
*    scanning.
*
* \return 
*   Returns the status of operation:
*   - Zero        - Successfully initiated scan.
*   - Non-zero    - One or more errors occurred in the initialization process.
*
*******************************************************************************/
cystatus CapSense_ScanAllWidgets(void)
{
    cystatus scanStatus = CYRET_UNKNOWN;
    
    #if (CapSense_DISABLE == CapSense_CSD2X_EN)
        if (CapSense_SW_STS_BUSY == (CapSense_dsRam.status & CapSense_SW_STS_BUSY))
        {
            /* Previous widget is being scanned. Return error. */
            scanStatus = CYRET_INVALID_STATE;
        }
        else
        {
            /*  
             *  Set up widget first widget.
             *  If widget returned error, set up next, continue same until widget does not return error.    
             */
            for (CapSense_widgetIndex = 0u; 
                 CapSense_widgetIndex < CapSense_TOTAL_WIDGETS; 
                 CapSense_widgetIndex++)
            {

                scanStatus = CapSense_SetupWidget((uint32)CapSense_widgetIndex);

                if (CYRET_SUCCESS == scanStatus)
                {
                    #if (0u != (CapSense_TOTAL_WIDGETS - 1u))
                        /* If there are more than one widget to be scanned, request callback to scan next widget */
                        if ((CapSense_TOTAL_WIDGETS - 1u) > CapSense_widgetIndex)
                        {
                             /* Request callback to scan next widget in ISR */
                            CapSense_requestScanAllWidget = CapSense_ENABLE;
                        }
                        else
                        {
                            /* Request to exit in ISR (Do not scan the next widgets) */
                            CapSense_requestScanAllWidget = CapSense_DISABLE;
                        }
                    #else 
                        {
                            /* Request to exit in ISR (We have only one widget) */
                            CapSense_requestScanAllWidget = CapSense_DISABLE;
                        }
                    #endif  /* (0u != (CapSense_TOTAL_WIDGETS - 1u)) */
                    
                    /*  Initiate scan and quit loop */
                    scanStatus = CapSense_Scan();

                    break;
                } 
            } 
        }        
    #else
        /******************************
        * This is the place where the Scan2x 
        * setup should be implemented.
        * Scan2x will be implemented
        * in the next component version.
        *********************************/
    #endif /* (CapSense_DISABLE == CapSense_CSD2X_EN) */

    return (scanStatus);
}


/*******************************************************************************
* Function Name: CapSense_SsInitialize
****************************************************************************//**
*
* \brief
*   Performs hardware and firmware initialization required for proper operation
*   of the CapSense component. This function is called from 
*   the CapSense_Start() API prior to calling any other APIs of the component.
*
* \details
*   Performs hardware and firmware initialization required for proper operation
*   of the CapSense component. This function is called from 
*   the CapSense_Start() API prior to calling any other APIs of the component.
*   1. The function initialises immunity offsets when the frequency hopping is 
*      enabled.
*   2. Depending on the configuration, the function initialises the CSD block 
*      for the CSD2X, CSD, CSX, or CSD+CSX modes.
*   3. The function updates the dsRam.wdgtWorking variable with 1 when Self Test
*      is enabled.
*
*   Calling the CapSense_Start API is the recommended method to initialize
*   the CapSense component at power-up. The CapSense_SsInitialize() 
*   API should not be used for initialization, resume, or wake-up operations.
*   The dsRam.wdgtWorking variable is updated.
*
* \return status
*   Returns status of operation:
*   - Zero        - Indicates successful initialization.
*   - Non-zero    - One or more errors occurred in the initialization process.
*
*******************************************************************************/
cystatus CapSense_SsInitialize(void)
{
    cystatus initStatus = CYRET_SUCCESS;

    #if (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN)
        CapSense_immunity[CapSense_FREQ_CHANNEL_0] = (uint8)CY_GET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR);
        if (CapSense_COARSE_TRIM_THRESHOLD_1 > CapSense_immunity[CapSense_FREQ_CHANNEL_0])
        {
            CapSense_immunity[CapSense_FREQ_CHANNEL_1] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] + CapSense_FREQUENCY_OFFSET_5;
            CapSense_immunity[CapSense_FREQ_CHANNEL_2] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] + CapSense_FREQUENCY_OFFSET_10;
        }
        else if (CapSense_COARSE_TRIM_THRESHOLD_2 > CapSense_immunity[CapSense_FREQ_CHANNEL_0])
        {
            CapSense_immunity[CapSense_FREQ_CHANNEL_1] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] - CapSense_FREQUENCY_OFFSET_5;
            CapSense_immunity[CapSense_FREQ_CHANNEL_2] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] + CapSense_FREQUENCY_OFFSET_5;
        }
        else
        {
            CapSense_immunity[CapSense_FREQ_CHANNEL_1] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] - CapSense_FREQUENCY_OFFSET_5;
            CapSense_immunity[CapSense_FREQ_CHANNEL_2] = 
            CapSense_immunity[CapSense_FREQ_CHANNEL_0] - CapSense_FREQUENCY_OFFSET_10;
        }
    #endif /* (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN) */

    /*  
     *  Check if CSD2X is enabled, if yes, initialize CapSense component
     *  for CSD2x operation 
     */
    #if (CapSense_ENABLE == CapSense_CSD2X_EN)
        /*  
         *  Check if CSX or CSD enable along with CSD2X, if yes, provide the error
         *  message as CSD2X can not coexist with CSD or CSX    
         */
        #if (CapSense_ENABLE == CapSense_CSX_EN)
            #error "CSD2X and CSX can not be enabled together"
            initStatus = CYRET_UNKNOWN;
        #elif (CapSense_ENABLE == CapSense_CSD_EN)
            #error "CSD2X and CSD can not be enabled together"
            initStatus = CYRET_UNKNOWN;
        #endif /* (CapSense_CSX_EN == CapSense_ENABLE) */

        /* Initialize both CSD blocks for CSD2X scanning    */
        CapSense_CSD2XInitialize();

    #elif (CapSense_ENABLE == CapSense_CSD_CSX_EN)
        /* 
         * Do nothing, CSD hardware shall be initialized in the Setup Widget based
         * on widget sensing method    
         */         
        CapSense_currentSenseMethod = CapSense_UNDEFINED_E;    

    #elif (CapSense_ENABLE == CapSense_CSD_EN)
        /*  Initialize CSD block for CSD scanning   */
        CapSense_CSDInitialize();

    #elif (CapSense_ENABLE == CapSense_CSX_EN)
        /*  Initialize CSD block for CSX scanning   */
        CapSense_CSXInitialize();
        
    #else
        #error "No sensing method enabled, component cannot work in this mode"
        initStatus = CYRET_UNKNOWN;
    #endif  /* (CapSense_ENABLE == CapSense_CSD2X_EN) */

    return (initStatus);
}


/*******************************************************************************
* Function Name: CapSense_SetPinState
****************************************************************************//**
*
* \brief
*   Sets the status (drive mode and output state) of a sensor to six states
*   relevant (GND, Shield,  HIgh-Z, Tx or Rx, sensor) to the CapSense operation.
*   If the sensor is configured as a ganged sensor, then the API applies to the  
*   dedicated pin and all ganged electrodes.
*
* \details
*   This API disconnects port pins from AMUXBUS and sets the drive mode of a port pin
*   (Rx, Tx or Sns electrode) based on an input parameter. The mode of the pin
*   is set to HIgh-Z in the GPIO_PRT_PCx register.
*
*   Scanning should be completed before calling this API. 
*   CapSense_SHIELD is not allowed if Enable shield electrode parameter
*   is disabled.
*   CapSense_TX_PIN and CapSense_RX_PIN are not allowed if there 
*    are no other CSX widgets configured in the project.
*
*  \param wdgtIndex Specifies the ID of the widget.
*   
*  \param snsIndex Specifies the ID of the snsIndex.
*   
*  \param state Specifies the state to be set for a specified pin:
*          -   (0) CapSense_GROUND
*          -   (1) CapSense_HIGHZ
*          -   (2) CapSense_SHIELD
*          -   (3) CapSense_SENSOR
*          -   (4) CapSense_TX_PIN
*          -   (5) CapSense_RX_PIN
*
*******************************************************************************/
void CapSense_SetPinState(uint32 wdgtIndex, uint32 snsIndex, uint32 state)
{
    CapSense_FLASH_IO_STRUCT const *curSnsIOPtr;
    #if (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN)
        CapSense_FLASH_SNS_STRUCT const *curFlashSnsPtr;
        uint32 tempVal;
    #endif /* (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) */
    uint32 newRegisterValue;
    uint8  interruptState;
    uint32 pinHSIOMShift;
    uint32 pinModeShift;
    
    #if (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN)
        /* Check ganged sns flag  */
        if (CapSense_GANGED_SNS_MASK == (CapSense_dsFlash.wdgtArray[wdgtIndex].staticConfig & 
            CapSense_GANGED_SNS_MASK))
        {             
            curFlashSnsPtr = CapSense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash;
            curFlashSnsPtr += snsIndex;
            curSnsIOPtr = &CapSense_ioList[curFlashSnsPtr->firstPinId];
            
            /* Get number of ganged pins  */
            tempVal = curFlashSnsPtr->numPins;
        }
        else
        {
            curSnsIOPtr = (CapSense_FLASH_IO_STRUCT const *)
                                    CapSense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash + snsIndex;
            /* There are no ganged pins */
            tempVal = 1u;                                
        }
    #else
        curSnsIOPtr = (CapSense_FLASH_IO_STRUCT const *)
                                    CapSense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash + snsIndex;
    #endif  /* (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) */
    
    #if (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN)
        /* Reconfigure all ganged sensors  */
        do
        {
    #endif  /* (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) */

            /* Get HSIOM and port mode shifts */
            pinHSIOMShift = (uint32)curSnsIOPtr->hsiomShift;
            pinModeShift = (uint32)curSnsIOPtr->shift;
            
            /* Reset HSIOM register */
            CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) & ~(CapSense_HSIOM_SEL_MASK << pinHSIOMShift));
           
            switch (state)
            {  
            case CapSense_GROUND:
                interruptState = CyEnterCriticalSection();

                /* Update port configuration register (drive mode) for sensor */
                newRegisterValue = CY_GET_REG32(curSnsIOPtr->pcPtr);
                newRegisterValue &= ~(CapSense_GPIO_PC_MASK << pinModeShift);
                newRegisterValue |= (CapSense_SNS_GROUND_CONNECT << pinModeShift);
                CY_SET_REG32(curSnsIOPtr->pcPtr, newRegisterValue);

                CyExitCriticalSection(interruptState);

                /* Set logic 0 to port data register */
                CY_SET_REG32(curSnsIOPtr->drPtr, CY_GET_REG32(curSnsIOPtr->drPtr) & (uint32)~(uint32)((uint32)1u << curSnsIOPtr->drShift));
                break;
                
            case CapSense_HIGHZ:
                interruptState = CyEnterCriticalSection();

                /* Update port configuration register (drive mode) for sensor */
                newRegisterValue = CY_GET_REG32(curSnsIOPtr->pcPtr);
                newRegisterValue &= ~(CapSense_GPIO_PC_MASK << pinModeShift);
                CY_SET_REG32(curSnsIOPtr->pcPtr, newRegisterValue);

                CyExitCriticalSection(interruptState);

                /* Set logic 0 to port data register */
                CY_SET_REG32(curSnsIOPtr->drPtr, CY_GET_REG32(curSnsIOPtr->drPtr) & (uint32)~(uint32)((uint32)1u << curSnsIOPtr->drShift));
                break;

            #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                case CapSense_SHIELD:
                    /* Set drive mode to Analog */
                    CY_SET_REG32(curSnsIOPtr->pcPtr, CY_GET_REG32(curSnsIOPtr->pcPtr) &
                               ~(CapSense_GPIO_PC_MASK <<(curSnsIOPtr->shift)));

                    /* Set appropriate bit in HSIOM register to configure pin to shield mode */
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | 
                                                       (CapSense_HSIOM_SEL_CSD_SHIELD << pinHSIOMShift));
                    break;
            #endif  /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
            
            #if ((CapSense_ENABLE == CapSense_CSD_EN) || \
                 (CapSense_ENABLE == CapSense_CSD_CSX_EN))
                case CapSense_SENSOR:
                    /* Enable sensor */
                    CapSense_CSDConnectSns(curSnsIOPtr);
                    break;
            #endif  /* ((CapSense_ENABLE == CapSense_CSD_EN) || \
                        (CapSense_ENABLE == CapSense_CSD_CSX_EN)) */
             
            #if ((CapSense_ENABLE == CapSense_CSX_EN) || \
                 (CapSense_ENABLE == CapSense_CSD_CSX_EN))
                case CapSense_TX_PIN:
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | (CapSense_HSIOM_SEL_CSD_SENSE << pinHSIOMShift));
                    break;
                    
                case CapSense_RX_PIN:
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | (CapSense_HSIOM_SEL_AMUXA << pinHSIOMShift));
                    CY_SET_REG32(curSnsIOPtr->pcPtr, CY_GET_REG32(curSnsIOPtr->pcPtr) & ~(CapSense_GPIO_PC_MASK << pinModeShift));
                    break;
            #endif  /* ((CapSense_ENABLE == CapSense_CSX_EN) || \
                        (CapSense_ENABLE == CapSense_CSD_CSX_EN)) */
             
            default:
                /* Wrong input */
                break;
            }

    #if (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN)
            curSnsIOPtr++;
            tempVal--;
        } while (0u != tempVal);
    #endif  /* (CapSense_ENABLE == CapSense_CSD_GANGED_SNS_EN) */
}


#if (CapSense_ENABLE == CapSense_CSD_CSX_EN)
/*******************************************************************************
* Function Name: CapSense_SwitchSensingMode
****************************************************************************//**
*
* \brief
*  This function changes the mode for case when both CSD and CSX widgets are
*  scanned.
*
* \details
*  To switch to the CSD mode the following tasks are performed:
*  1. Disconnect CintA and CintB from AMUXBUS-A
*  2. Disconnect previous CSD electrode if it has been connected
*  3. Initialize CSD mode.
*
*  To switch to the CSX mode the following tasks are performed:
*  1. Disconnect Cmod from AMUXBUS-A
*  2. Disconnect previous CSX electrode if it has been connected
*  3. Initialize CSX mode.
*
* \param mode Specifies the scan mode:
*           -  (1) CapSense_SENSE_METHOD_CSD_E
*           -  (2) CapSense_SENSE_METHOD_CSX_E
*
*******************************************************************************/
void CapSense_SwitchSensingMode(CapSense_SENSE_METHOD_ENUM mode)
{
    uint32 newRegValue;
        
    if (CapSense_SENSE_METHOD_CSD_E == mode) 
    {
        /*  Disconnect CintA from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
        CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
        
        /*  Disconnect CintB from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
        CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSX electrode if it has been connected */
        CapSense_CSXElectrodeCheck();
        
        /* Initialize CSD mode to guarantee configured CSD mode after CSX calibration */
        CapSense_CSDInitialize();
        CapSense_currentSenseMethod = CapSense_SENSE_METHOD_CSD_E;
    }
    else if (CapSense_SENSE_METHOD_CSX_E == mode) 
    {
        /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */ 
        newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR); 
        newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
        CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSD electrode if it has been connected */
        CapSense_CSDElectrodeCheck();

        /* Disconnect Csh from AMUXBUS-B */
        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
             (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
            newRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
            newRegValue &= (uint32)(~(uint32)(CapSense_CSH_TO_AMUXBUS_B_MASK << CapSense_CSH_HSIOM_SHIFT));
            CY_SET_REG32(CapSense_CSH_HSIOM_PTR, newRegValue);
        #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                   (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */
        
        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
             (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
            CapSense_DisableShieldElectrodes();
        #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                   (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)) */
        
        /* Initialize CSD hardware block for CSX measurement */
        CapSense_CSXInitialize();
        CapSense_currentSenseMethod = CapSense_SENSE_METHOD_CSX_E;
    }
    else
    {
        CapSense_currentSenseMethod = CapSense_UNDEFINED_E;  
    }
}
#endif  /* (CapSense_ENABLE == CapSense_CSD_CSX_EN)) */


#if (CapSense_ENABLE == CapSense_ADC_EN)
/*******************************************************************************
* Function Name: CapSense_SsReleaseResources()
****************************************************************************//**
*
* \brief
*  This function sets the resources that do not belong to the CSDv2 HW block to 
*  default state.
*
* \details
*  The function performs following tasks:
*  1. Checks if CSD block busy and returns error if it is busy
*  2. Disconnects integration capacitors (CintA, CintB for CSX mode and 
*     Cmod for CSD mode)
*  3. Disconnect electroded if they have been connected.
*
* \return 
*   Returns the status of operation:
*   - Zero        - Resources released successfully.
*   - Non-zero    - One or more errors occurred in releasing process.
*
*******************************************************************************/
cystatus CapSense_SsReleaseResources(void)
{
    cystatus busyStatus = CYRET_SUCCESS;
    uint32 newRegValue;

    if (CapSense_SW_STS_BUSY == (CapSense_dsRam.status & CapSense_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        busyStatus = CYRET_INVALID_STATE;
    }
    else
    {
    #if (CapSense_ENABLE == CapSense_CSX_EN)
        /*  Disconnect CintA from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
        CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
        
        /*  Disconnect CintB from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
        CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSX electrode if it has been connected */
        CapSense_CSXElectrodeCheck();
    #endif /* (CapSense_ENABLE == CapSense_CSX_EN) */
    
    #if (CapSense_ENABLE == CapSense_CSD_EN)
        /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */ 
        newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR); 
        newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
        CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSD electrode if it has been connected */
        CapSense_CSDElectrodeCheck();
        
        /* Disconnect Csh from AMUXBUS-B */
        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
             (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
            newRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
            newRegValue &= (uint32)(~(uint32)(CapSense_CSH_TO_AMUXBUS_B_MASK << CapSense_CSH_HSIOM_SHIFT));
            CY_SET_REG32(CapSense_CSH_HSIOM_PTR, newRegValue);
        #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                   (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */
        
        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
             (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
            CapSense_DisableShieldElectrodes();
        #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                   (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)) */
    #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */

        /* 
         * Reset of the currentSenseMethod variable to make sure that the next
         * call of SetupWidget() API setups the correct widget mode
         */
        CapSense_currentSenseMethod = CapSense_UNDEFINED_E;
    }

    return busyStatus;
}
#endif /* (CapSense_ENABLE == CapSense_ADC_EN) */


/*******************************************************************************
* Function Name: CapSense_PostAllWidgetsScan
****************************************************************************//**
*
* \brief
*   The ISR function for multiple widget scanning implementation.
*
* \details
*   This is the function used by the CapSense ISR to implement multiple widget
*   scanning.
*   Should not be used by the application layer.
*
*******************************************************************************/
void CapSense_PostAllWidgetsScan(void)
{
    #if (CapSense_DISABLE == CapSense_CSD2X_EN)
        /*  
        *   1. Increment widget index
        *   2. Check if all widgets are scanned
        *   3. If all widgets are not scanned, set up and scan next widget  
        */
        #if (1u != CapSense_TOTAL_WIDGETS)  
            cystatus postScanStatus;
        
            do
            {
                CapSense_widgetIndex++;

                postScanStatus = CapSense_SetupWidget((uint32)CapSense_widgetIndex);

                if (CYRET_SUCCESS == postScanStatus)
                {
                    if((CapSense_TOTAL_WIDGETS - 1u) == CapSense_widgetIndex)
                    {
                        /* The last widget will be scanned. Reset flag to skip configuring the next widget setup in ISR. */
                        CapSense_requestScanAllWidget = CapSense_DISABLE;
                    }
                    (void)CapSense_Scan();
                }
                else if((CapSense_TOTAL_WIDGETS - 1u) == CapSense_widgetIndex)
                {
                    #if ((CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) && \
                         (CapSense_ENABLE == CapSense_CSD_EN))
                        if (CapSense_SENSE_METHOD_CSD_E ==
                             (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[CapSense_widgetIndex]))
                        {
                            /*  Disable the CSD block */
                            CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_configCsd);
                        }
                    #endif /* ((CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) && \
                               (CapSense_ENABLE == CapSense_CSD_EN)) */
                    
                    /* All widgets are totally processed. Reset BUSY flag */
                    CapSense_dsRam.status &= ~CapSense_SW_STS_BUSY;
                    
                    /* Update status with with the failure */
                    CapSense_dsRam.status &= ~CapSense_STATUS_ERR_MASK;                
                    CapSense_dsRam.status |= ((postScanStatus & CapSense_STATUS_ERR_SIZE) << CapSense_STATUS_ERR_SHIFT);
                    
                    /* Set postScanStatus to exit the while loop */
                    postScanStatus = CYRET_SUCCESS;
                }
                else
                {
                    /* Update status with with the failure. Configure the next widget in while() loop */
                    CapSense_dsRam.status &= ~CapSense_STATUS_ERR_MASK;                
                    CapSense_dsRam.status |= ((postScanStatus & CapSense_STATUS_ERR_SIZE) << CapSense_STATUS_ERR_SHIFT);
                }
            } while (CYRET_SUCCESS != postScanStatus);
        #endif /* (1u != CapSense_TOTAL_WIDGETS) */
    #else
        /******************************
        * This is the place where the CSD2X 
        * postscan for 2 widgets should be implemented.
        * The CSD2X postscan will be implemented
        * in the next component version.
        *********************************/
    #endif /* (CapSense_CSD2X_EN == CapSense_DISABLE) */
}


/*******************************************************************************
* Function Name: CapSense_IsrInitialize
****************************************************************************//**
*
* \brief
*   Enables and initializes for the function pointer for a callback for the ISR.
*
* \details
*   The  "address" is a special type cyisraddress defined by Cylib. This function
*   is used by component APIs and should not be used by an application program for
*   proper working of the component.
*
* \param  address The address of the function to be called when interrupt is fired.
*
*******************************************************************************/
void CapSense_IsrInitialize(cyisraddress address)
{
#if (CapSense_ENABLE == CapSense_CSD2X_EN)
    /******************************
    * This is the place where the CSD2X ISR 
    * Setup should be implemented.
    * The CSD2X ISR Setup will be implemented
    * in the next component version.
    *********************************/
#else
    CapSense_ISR_StartEx(address);
#endif /* (CapSense_ENABLE == CapSense_CSD2X_EN) */
}


/*******************************************************************************
* Function Name: CapSense_ConfigureModClkClock
****************************************************************************//**
*
* \brief
*   Sets the divider values for the modulator clock and then starts
*   the modulator clock.
*
* \details
*   It is not recommended to call this function directly by the application layer.
*   It is used by initialization, widget APIs or wakeup functions to
*   enable the clocks.
*
* \param
*   modClk The divider value for the modulator clock.
*
*******************************************************************************/
void CapSense_ConfigureModClkClock(uint32 modClk)
{
#if (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK)
    /*  Stop modulator clock   */
    CY_SET_REG32(CapSense_MODCLK_CMD_PTR, 
                 ((uint32)CapSense_ModClk__DIV_ID <<
                 CapSense_MODCLK_CMD_DIV_SHIFT)|
                 ((uint32)CapSense_MODCLK_CMD_DISABLE_MASK));

    /*  
     * Set divider value for sense modClk.
     * 1u is subtracted from modClk because Divider register value 0 corresponds
     * to dividing by 1.  
     */
    CY_SET_REG32(CapSense_MODCLK_DIV_PTR, ((modClk - 1u) << 8u));  

    /*  Check whether previous modulator clock start command has finished. */
    while(0u != (CY_GET_REG32(CapSense_MODCLK_CMD_PTR) & CapSense_MODCLK_CMD_ENABLE_MASK))
    {
        /*  Wait until previous modulator clock start command has finished. */  
    }
    
    /*  Start modulator clock, aligned to HFCLK */
    CY_SET_REG32(CapSense_MODCLK_CMD_PTR, 
                 (uint32)(((uint32)CapSense_ModClk__DIV_ID << CapSense_MODCLK_CMD_DIV_SHIFT) |
                  ((uint32)CapSense_ModClk__PA_DIV_ID << CapSense_MODCLK_CMD_PA_DIV_SHIFT) |
                  CapSense_MODCLK_CMD_ENABLE_MASK));
#else
    uint32 newRegValue;

    /* Clear bit to disable ModClk clock. */
    CY_SET_REG32(CapSense_MODCLK_CMD_PTR, 
                 CY_GET_REG32(CapSense_MODCLK_CMD_PTR) & 
                 (uint32)(~CapSense_ModClk__ENABLE_MASK));
        
    /* 
     * Update ModClk clock divider.
     * 1u is subtracted from modClk because Divider register value has range (0-65535).  
     */
    newRegValue = CY_GET_REG32(CapSense_MODCLK_DIV_PTR) & (uint32)(~(uint32)CapSense_ModClk__DIVIDER_MASK);
    newRegValue |= (modClk - 1u);
    CY_SET_REG32(CapSense_MODCLK_DIV_PTR, newRegValue);
    
    /* Set bit to enable ModClk clock. */
    CY_SET_REG32(CapSense_MODCLK_CMD_PTR, 
                 CY_GET_REG32(CapSense_MODCLK_CMD_PTR) | 
                CapSense_ModClk__ENABLE_MASK);
#endif /* (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK) */
}


/*******************************************************************************
* Function Name: CapSense_ConfigureSnsClock
****************************************************************************//**
*
* \brief
*   Sets the divider values for the sense clock and then starts
*   the sense clock.
*
* \details
*   It is not recommended to call this function directly by the application layer.
*   It is used by initialization, widget APIs or wakeup functions to
*   enable the clocks.
*
* \param
*   snsClk The divider value for the sense clock.
*
*******************************************************************************/
void CapSense_ConfigureSnsClock(uint32 snsClk)
{
#if (CapSense_ENABLE == CapSense_CSDV2)
    uint32 newRegValue;

    /*  
     * Set divider value for sense clock.
     * 1u is subtracted from snsClk because SENSE_DIV value 0 corresponds
     * to dividing by 1.  
     */
    newRegValue = CY_GET_REG32(CapSense_SENSE_PERIOD_PTR); 
    newRegValue &= (uint32)(~CapSense_SENSE_PERIOD_SENSE_DIV_MASK);
    newRegValue |= snsClk - 1u;
    CY_SET_REG32(CapSense_SENSE_PERIOD_PTR, newRegValue);
#else
    #if (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK)
        /*  Stop sense clock clock   */
        CY_SET_REG32(CapSense_SNSCLK_CMD_PTR, 
                     ((uint32)CapSense_SnsClk__DIV_ID <<
                     CapSense_SNSCLK_CMD_DIV_SHIFT)|
                     ((uint32)CapSense_SNSCLK_CMD_DISABLE_MASK));

        /*  
         * Set divider value for sense clock.
         * 1u is subtracted from snsClk because Divider register value 0 corresponds
         * to dividing by 1.  
         */
        CY_SET_REG32(CapSense_SNSCLK_DIV_PTR, ((snsClk - 1u) << 8u));
        
        /*  Check whether previous sense clock start command has finished. */
        while(0u != (CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) & CapSense_SNSCLK_CMD_ENABLE_MASK))
        {
            /*  Wait until previous sense clock start command has finished. */  
        } 
        
        /* Start sense clock aligned to previously started modulator clock. */
        CY_SET_REG32(CapSense_SNSCLK_CMD_PTR, 
                     (uint32)(((uint32)CapSense_SnsClk__DIV_ID << CapSense_SNSCLK_CMD_DIV_SHIFT) |
                      ((uint32)CapSense_ModClk__PA_DIV_ID << CapSense_SNSCLK_CMD_PA_DIV_SHIFT) |
                      CapSense_SNSCLK_CMD_ENABLE_MASK));
    #else
        uint32 newRegValue;

        /* Clear bit to disable SnsClk clock. */
        CY_SET_REG32(CapSense_SNSCLK_CMD_PTR, 
                     CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) & 
                     (uint32)(~(uint32)CapSense_SnsClk__ENABLE_MASK));
        
        /* 
         * Update snsClk clock divider.
         * 1u is subtracted from snsClk because Divider register value has range (0-65535).  
         */
        newRegValue = CY_GET_REG32(CapSense_SNSCLK_DIV_PTR) & (uint32)(~(uint32)CapSense_SnsClk__DIVIDER_MASK);
        newRegValue |= (snsClk - 1u);
        CY_SET_REG32(CapSense_SNSCLK_DIV_PTR, newRegValue);

        /* Set bit to enable SnsClk clock. */
        CY_SET_REG32(CapSense_SNSCLK_CMD_PTR, 
                     CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) | 
                    CapSense_SnsClk__ENABLE_MASK);
    #endif /* (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK) */
#endif /* (CapSense_ENABLE == CapSense_CSDV2) */
}


/*******************************************************************************
* Function Name: CapSense_ConfigureClocks
****************************************************************************//**
*
* \brief
*   Sets the divider values for sense and modulator clocks and then starts
*   a modulator clock-phase aligned to HFCLK and sense clock-phase aligned to
*   the modulator clock.
*
* \details
*   It is not recommended to call this function directly by the application layer.
*   It is used by initialization, widget APIs or wakeup functions to
*   enable the clocks.
*
* \param
*   snsClk The divider value for the sense clock.
*   modClk The divider value for the modulator clock.
*
*******************************************************************************/
void CapSense_ConfigureClocks(uint32 snsClk, uint32 modClk)
{
    /* Configure Mod clock */
    CapSense_ConfigureModClkClock(modClk);

    /* Configure Sns clock */
    CapSense_ConfigureSnsClock(snsClk);
}


#if ((CapSense_ENABLE == CapSense_CSD_IDAC_AUTOCAL_EN) || \
     (CapSense_ENABLE == CapSense_CSX_IDAC_AUTOCAL_EN))
/*******************************************************************************
* Function Name: CapSense_CalibrateWidget 
****************************************************************************//**
*
* \brief
*  Executes the IDAC calibration for the widget.
*
* \details
*   Depending on the widget type, this function calibrates an appropriate widget: CSD or CSX.
*   Before calibration, the function checks if the widget failed during the self-test  
*   if it is enabled in the customizer.
*
* \param wdgtIndex Specifies the ID of the widget for calibration.
*   
* \return status 
*   - Zero     - The widget is calibrated successfully.
*   - Non-zero - Calibration is failed for the widget.
*
*******************************************************************************/
cystatus CapSense_CalibrateWidget(uint32 wdgtIndex)
{
    cystatus calibrateStatus = CYRET_SUCCESS;

    do
    {   
        if (CapSense_TOTAL_WIDGETS <= wdgtIndex)
        {
            calibrateStatus = CYRET_BAD_PARAM;
            break;
        }
    
        /*  
         *  Check if widget id is valid, specified widget did not
         *  detect any faults conditions if BIST is enabled.   
         */
        #if (CapSense_ENABLE == CapSense_SELF_TEST_EN)
            if (CapSense_DISABLE != ((uint32)CapSense_dsRam.wdgtWorking[(uint8)wdgtIndex >> CapSense_WIDGET_NUM_32_DIV_SHIFT] & 
                                             (0x00000001Lu << (wdgtIndex & CapSense_WIDGET_NUM_32_MASK))))    
            {
                calibrateStatus = CYRET_SUCCESS;
            }
            else    
            {
                calibrateStatus = CYRET_INVALID_STATE;
                /*  Exit because widget is not working  */
                break;
            }
        #endif  /* (CapSense_ENABLE == CapSense_SELF_TEST_EN) */
    
        #if (CapSense_ENABLE == CapSense_ADC_EN)
            if (CapSense_UNDEFINED_E == CapSense_currentSenseMethod) 
            {
                /* Release ADC resources */
                calibrateStatus = CapSense_AdcReleaseResources();

                if (CYRET_SUCCESS != calibrateStatus)
                {
                    /* Exit because widget is not working */
                    break;
                }
            }
        #endif /* (CapSense_ENABLE == CapSense_ADC_EN) */

        /*  If CSD2X is enabled, calibrate CSD sensor    */
        #if (CapSense_ENABLE == CapSense_CSD2X_EN)
            /******************************
            * This is the place where the CSD2X mode
            * API should be implemented.
            * CSD2X will be implemented
            * in the next component version.
            *********************************/

        /*  If both CSD and CSX are enabled, calibrate widget using sensing method */
        #elif (CapSense_ENABLE == CapSense_CSD_CSX_EN)

            /*  Check widget sensing method and call appropriate APIs   */
            #if ((CapSense_ENABLE == CapSense_CSX_IDAC_AUTOCAL_EN) && (CapSense_ENABLE == CapSense_CSD_IDAC_AUTOCAL_EN))
                #if (CapSense_CSD_SS_DIS == CapSense_CSD_AUTOTUNE)
                    if (CapSense_SENSE_METHOD_CSX_E ==
                        (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))   
                    {
                        /* Calibrate CSX widget (when Autotune mode is disabled) */
                       CapSense_CSXCalibrateWidget(wdgtIndex, CapSense_CSX_RAWCOUNT_CAL_LEVEL);
                    }
                    else if (CapSense_SENSE_METHOD_CSD_E ==
                        (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))
                    {
                        /*  
                         *  Check if CSD block is  already initialized for CSD operation
                         *  If not, initialize CSD hardware block for CSD measurement   
                         */
                        if (CapSense_SENSE_METHOD_CSD_E != CapSense_currentSenseMethod) 
                        {
                            CapSense_SwitchSensingMode(CapSense_SENSE_METHOD_CSD_E);
                        }
                        
                        /* Calibrate CSD widget (when Autotune mode is disabled) */
                        calibrateStatus = CapSense_CSDCalibrateWidget(wdgtIndex, CapSense_CSD_RAWCOUNT_CAL_LEVEL);
                    }
                    else    
                    {
                        calibrateStatus = CYRET_UNKNOWN;
                    }
                #else
                    if (CapSense_SENSE_METHOD_CSX_E ==
                        (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))   
                    {
                        /* Calibrate CSX widget only (when Autotune mode is enabled) */
                       CapSense_CSXCalibrateWidget(wdgtIndex, CapSense_CSX_RAWCOUNT_CAL_LEVEL);
                    }
                #endif /* (CapSense_CSD_SS_DIS == CapSense_CSD_AUTOTUNE) */
            #elif (CapSense_ENABLE == CapSense_CSX_IDAC_AUTOCAL_EN)
                if (CapSense_SENSE_METHOD_CSX_E ==
                    (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))   
                {
                    /* Calibrate CSX widget only */
                   CapSense_CSXCalibrateWidget(wdgtIndex, CapSense_CSX_RAWCOUNT_CAL_LEVEL);
                }          
            #elif ((CapSense_ENABLE == CapSense_CSD_IDAC_AUTOCAL_EN) && (CapSense_CSD_SS_DIS == CapSense_CSD_AUTOTUNE))
                if (CapSense_SENSE_METHOD_CSD_E ==
                    (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex])) 
                {
                    /* Initialize CSD mode */
                    CapSense_CSDInitialize();

                    /* Calibrate CSD widget only */
                    calibrateStatus = CapSense_CSDCalibrateWidget(wdgtIndex, CapSense_CSD_RAWCOUNT_CAL_LEVEL);
                }
            #else
                /*  No calibration  */
            #endif  /* ((CapSense_ENABLE == CapSense_CSX_IDAC_AUTOCAL_EN) && (CapSense_ENABLE == CapSense_CSD_IDAC_AUTOCAL_EN)) */
            
        /*  If only CSD is enabled, calibrate CSD sensor  */
        #elif ((CapSense_ENABLE == CapSense_CSD_EN) && (CapSense_CSD_SS_DIS == CapSense_CSD_AUTOTUNE))
            calibrateStatus = CapSense_CSDCalibrateWidget(wdgtIndex, CapSense_CSD_RAWCOUNT_CAL_LEVEL);
        
        /*  If only CSX is enabled, call CSX scan   */
        #elif (CapSense_ENABLE == CapSense_CSX_EN)
            CapSense_CSXCalibrateWidget(wdgtIndex, CapSense_CSX_RAWCOUNT_CAL_LEVEL);

        #else
            calibrateStatus = CYRET_UNKNOWN;
        #endif  /* (CapSense_ENABLE == CapSense_CSD2X_EN) */   
    
    } while (0u);
    
    return calibrateStatus;
}


/*******************************************************************************
* Function Name: CapSense_CalibrateAllWidgets 
****************************************************************************//**
*
* \brief
*  Executes the IDAC calibration for all the widgets in the component.
*
* \details
*   The function goes through all widgets and calls 
*   CapSense_CalibrateWidget() for each widget.
*   It returns a fail status if any widget has failed to calibrate or it   
*   has been failed during the self-test.
*
* \return status: 
*   - Zero     -  All the widgets are calibrated successfully.
*   - Non-zero - Calibration failed for any widget.
*
*******************************************************************************/
cystatus CapSense_CalibrateAllWidgets(void)
{
    cystatus calibrateStatus = CYRET_SUCCESS;
    uint32 wdgtIndex;
    
    for (wdgtIndex = 0u; wdgtIndex < CapSense_TOTAL_WIDGETS; wdgtIndex++)
    {
        calibrateStatus |= CapSense_CalibrateWidget(wdgtIndex); 
    }
    
    return calibrateStatus;
}

#endif /* ((CapSense_ENABLE == CapSense_CSD_IDAC_AUTOCAL_EN) || 
           (CapSense_ENABLE == CapSense_CSX_IDAC_AUTOCAL_EN))  */


#if (CapSense_CSD_SS_DIS != CapSense_CSD_AUTOTUNE)
/*******************************************************************************
* Function Name: CapSense_AutoTune
****************************************************************************//**
*
* \brief
*  This API performs the parameters auto-tuning for the optimal CapSense operation.
*
* \details
*  This API performs the following:
*  - Calibrates Modulator and Compensation IDACs.
*  - Tunes the Sense Clock optimal value to get a Sense Clock period greater than
*     2*5*R*Cp.
*  - Calculates the resolution for the optimal sensitivity.
*
* \return 
*   Returns the status of operation: 
*   - Zero     - All the widgets are auto-tuned successfully.
*   - Non-zero - Auto-tuning failed for any widget.
*
*******************************************************************************/
cystatus CapSense_AutoTune(void)
{
    cystatus autoTuneStatus = CYRET_SUCCESS;
    uint32 wdgtIndex;
    uint32 cp;
    CapSense_RAM_WD_BASE_STRUCT *ptrWdgt;
    AUTO_TUNE_CONFIG_TYPE autoTuneConfig;  
    
    /* Configure common config variables */
    autoTuneConfig.snsClkConstantR = CapSense_CSD_SNSCLK_R_CONST;
    autoTuneConfig.vRef = CapSense_CSD_VREF_MV;
    autoTuneConfig.iDacGain = CapSense_CSD_IDAC_GAIN_VALUE_NA * CapSense_CSD_DUAL_IDAC_FACTOR;

    /* Calculate snsClk Input Clock in KHz */
    #if (CapSense_ENABLE == CapSense_CSDV2)
        /*  Dividers are chained (CSDV2). Flip flop is not available */
        autoTuneConfig.snsClkInputClock = (CYDEV_BCLK__HFCLK__KHZ / CapSense_dsRam.modCsdClk);
    #elif ((CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK) && (CapSense_DISABLE == CY_PSOC4_4000))
        /*  Dividers are not chained */
        autoTuneConfig.snsClkInputClock = CYDEV_BCLK__HFCLK__KHZ >> CapSense_FLIP_FLOP_DIV;
    #elif (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK)
        /*  Dividers are not chained (PSoC 4000) */
        autoTuneConfig.snsClkInputClock = CYDEV_BCLK__HFCLK__KHZ >> CapSense_FLIP_FLOP_DIV;
    #else
        /*  Dividers are chained (PSoC 4100, PSoC 4200) */
        autoTuneConfig.snsClkInputClock = (CYDEV_BCLK__HFCLK__KHZ / CapSense_dsRam.modCsdClk) >> CapSense_FLIP_FLOP_DIV;
    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
 
    /* If both CSD and CSX are enabled, calibrate widget using sensing method */
    #if (CapSense_ENABLE == CapSense_CSD_CSX_EN)
        /* Initialize CSD mode */
        CapSense_CSDInitialize();
    #endif /* (CapSense_ENABLE == CapSense_CSD_CSX_EN) */
 
    for (wdgtIndex = 0u; wdgtIndex < CapSense_TOTAL_WIDGETS; wdgtIndex++)
    {
        if (CapSense_SENSE_METHOD_CSD_E ==
            (CapSense_SENSE_METHOD_ENUM)CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtIndex]))
        {
            ptrWdgt = (CapSense_RAM_WD_BASE_STRUCT *)
                      CapSense_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;

            /* Switch charge clock source to direct clock mode */ 
            CapSense_SET_DIRECT_CLOCK_MODE;

            /* Get sensitivity  */
            autoTuneConfig.sensitivity = ptrWdgt->sensitivity;

            /* Init pointer to sigPPf */
            autoTuneConfig.sigPPf = &ptrWdgt->sigPPf;

            /* Set calibration resolution to 12 bits */
            ptrWdgt->resolution = CapSense_CALIBRATION_RESOLUTION;

            /* Set Sense clock frequency to 1.5 MHz */
            #if (CapSense_ENABLE == CapSense_CSD_COMMON_SNS_CLK_EN)
                CapSense_dsRam.snsCsdClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                                   CapSense_CALIBRATION_FREQ_KHZ);
            #elif (CapSense_CSD_MATRIX_WIDGET_EN | CapSense_CSD_TOUCHPAD_WIDGET_EN)
                ptrWdgt->rowSnsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                     CapSense_CALIBRATION_FREQ_KHZ);    
                ptrWdgt->snsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                  CapSense_CALIBRATION_FREQ_KHZ);
            #else
                ptrWdgt->snsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                  CapSense_CALIBRATION_FREQ_KHZ);
            #endif /*  (CapSense_ENABLE == CapSense_CSD_COMMON_SNS_CLK_EN) */

            #if (CapSense_ENABLE == CapSense_CSDV2)
                CapSense_ConfigureSnsClock((uint32)ptrWdgt->snsClk);
            #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
        
            /* Set flag to calibrate in PWM mode only */
            CapSense_prescalersTuningDone = CapSense_DISABLE;

            #if (CapSense_DISABLE == CapSense_CSD2X_EN)
                /* Calibrate CSD widget to 85% */
                (void)CapSense_CSDCalibrateWidget(wdgtIndex, CapSense_CSD_AUTOTUNE_CAL_LEVEL);
            #endif /* (CapSense_DISABLE == CapSense_CSD2X_EN) */

            #if (CapSense_CSD_MATRIX_WIDGET_EN | CapSense_CSD_TOUCHPAD_WIDGET_EN)
                if((uint8)(CapSense_WD_TOUCHPAD_E | CapSense_WD_MATRIX_BUTTON_E) == CapSense_dsFlash.wdgtArray[wdgtIndex].wdgtType)
                {
                    /* Get pointer to Modulator IDAC for columns */
                    autoTuneConfig.ptrModIDAC = &ptrWdgt->rowIdacMod[0u];
                    
                    /* Get pointer to Sense Clock Divider for columns */
                    autoTuneConfig.ptrSenseClk = &ptrWdgt->rowSnsClk;

                    /* Find correct sense clock value */
                    cp = SmartSense_TunePrescalers(&autoTuneConfig);

                    if ((CapSense_CP_MAX + CapSense_CP_ERROR) <= cp)
                    {
                        autoTuneStatus = CYRET_BAD_DATA;
                    }
                    
                    #if (CapSense_ENABLE == CapSense_CSDV2)
                        /* Make sure that ModClk >= 4 * rowSnsClk for ModClk <= 12 MHz and rowSnsClk <= 6MHz */
                        if (autoTuneConfig.snsClkInputClock <= CapSense_MOD_CSD_CLK_24000KHZ)
                        {
                            if (ptrWdgt->rowSnsClk < CapSense_FACTOR_FILTER_DELAY_12MHZ)
                            {
                                ptrWdgt->rowSnsClk = CapSense_FACTOR_FILTER_DELAY_12MHZ;
                            }
                        }
                        else if (autoTuneConfig.snsClkInputClock <= CapSense_MOD_CSD_CLK_48000KHZ)
                        {
                            if (ptrWdgt->rowSnsClk < CapSense_FACTOR_MOD_SNS)
                            {
                                ptrWdgt->rowSnsClk = CapSense_FACTOR_MOD_SNS;
                            }
                        }
                        else
                        {
                            /* rowSnsClk is valid*/
                        }
                    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
                }
            #endif /* (CapSense_CSD_MATRIX_WIDGET_EN | CapSense_CSD_TOUCHPAD_WIDGET_EN) */

            /* Get pointer to Modulator IDAC  for rows */
            autoTuneConfig.ptrModIDAC = &ptrWdgt->idacMod[0u];
                
            /* Get pointer to Sense Clock Divider for columns */
            autoTuneConfig.ptrSenseClk = &ptrWdgt->snsClk;
               
            /* Find correct sense clock value */
            cp = SmartSense_TunePrescalers(&autoTuneConfig);

            if ((CapSense_CP_MAX + CapSense_CP_ERROR) <= cp)
            {
                autoTuneStatus = CYRET_BAD_DATA;
            }
            
            #if (CapSense_ENABLE == CapSense_CSDV2)
                /* Make sure that ModClk >= 4 * SnsClk for ModClk <= 12 MHz and SnsClk <= 6MHz */
                if (autoTuneConfig.snsClkInputClock <= CapSense_MOD_CSD_CLK_24000KHZ)
                {
                    if (ptrWdgt->snsClk < CapSense_FACTOR_FILTER_DELAY_12MHZ)
                    {
                        ptrWdgt->snsClk = CapSense_FACTOR_FILTER_DELAY_12MHZ;
                    }
                }
                else if (autoTuneConfig.snsClkInputClock <= CapSense_MOD_CSD_CLK_48000KHZ)
                {
                    if (ptrWdgt->snsClk < CapSense_FACTOR_MOD_SNS)
                    {
                        ptrWdgt->snsClk = CapSense_FACTOR_MOD_SNS;
                    }
                }
                else
                {
                    /* SnsClk is valid*/
                }
            #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            
            /* Set flag to indicate that calibration in PWM mode has been performed */
            CapSense_prescalersTuningDone = 1u;

            /* Multiply Clk divider to 2 for PRS mode to take into account average PRS frequency */
            autoTuneConfig.prsFactor = CapSense_PRS_FACTOR_DIV; 
 
            #if (CapSense_DISABLE == CapSense_CSD2X_EN)
                /* Calibrate CSD widget to 85% */
                autoTuneStatus |= CapSense_CSDCalibrateWidget(wdgtIndex, CapSense_CSD_AUTOTUNE_CAL_LEVEL);  
            #endif /* (CapSense_DISABLE == CapSense_CSD2X_EN) */

            /* Find resolution */
            ptrWdgt->resolution = SmartSense_TuneSensitivity(&autoTuneConfig);
        }
    }
    return autoTuneStatus;
}
#endif /* (CapSense_CSD_SS_DIS != CapSense_CSD_AUTOTUNE)) */


#if (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN)
/*******************************************************************************
* Function Name: CapSense_ChangeImoFreq
****************************************************************************//**
*
* \brief
*  This function changes the IMO frequency.
*
* \details
*   The IMO frequency can have three offsets: 0%, -5% and +5%. The frequency 
*   trims are contained in the CapSense_immunity[value] array for each 
*   frequency channel.
*
* \param value The frequency channel ID.  
*
*******************************************************************************/
    void CapSense_ChangeImoFreq(uint32 value)
    {
        CY_SET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR, CapSense_immunity[value]);
    }
#endif  /* (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN) */
 

/* [] END OF FILE */
