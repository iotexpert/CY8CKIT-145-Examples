/***************************************************************************//**
* \file capsense_Sensing.c
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

#include "capsense_Sensing.h"
#include "cyfitter.h"

/***************************************
* API Constants
***************************************/

#define capsense_WIDGET_NUM_32                          (32u)
#define capsense_WIDGET_NUM_32_DIV_SHIFT                (5u)
#define capsense_WIDGET_NUM_32_MASK                     (0x0000001FLu)
#define capsense_CALIBRATION_RESOLUTION                 (12u)
#define capsense_COARSE_TRIM_THRESHOLD_1                (40u)
#define capsense_COARSE_TRIM_THRESHOLD_2                (215u)
#define capsense_FREQUENCY_OFFSET_5                     (20u)
#define capsense_FREQUENCY_OFFSET_10                    (40u)
#define capsense_CALIBRATION_FREQ_KHZ                   (1500u)
#define capsense_CALIBRATION_MD                         (2u)
#define capsense_MIN_IMO_FREQ_KHZ                       (6000u)
#define capsense_CSD_AUTOTUNE_CAL_LEVEL                 (85u)
#define capsense_CP_MIN                                 (0u)
#define capsense_CP_MAX                                 (65u)
#define capsense_CP_ERROR                               (4u)

#if (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE)
    #define capsense_PRS_FACTOR_DIV                     (1u)
#else
    #define capsense_PRS_FACTOR_DIV                     (0u)
#endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */

#define capsense_FLIP_FLOP_DIV                          (1u)

#define capsense_MOD_CSD_CLK_12000KHZ                   (12000uL)
#define capsense_MOD_CSD_CLK_24000KHZ                   (24000uL)
#define capsense_MOD_CSD_CLK_48000KHZ                   (48000uL)

#if ((capsense_CLK_SOURCE_PRS8 == capsense_CSD_SNS_CLK_SOURCE) || \
    (capsense_CLK_SOURCE_PRS12 == capsense_CSD_SNS_CLK_SOURCE) || \
    (capsense_CLK_SOURCE_PRSAUTO == capsense_CSD_SNS_CLK_SOURCE))
    #define capsense_FACTOR_FILTER_DELAY_12MHZ          (2u)
#else
    #define capsense_FACTOR_FILTER_DELAY_12MHZ          (4u)
#endif /* (capsense_CLK_SOURCE_DIRECT != capsense_CSD_SNS_CLK_SOURCE) */

#define capsense_FACTOR_MOD_SNS                         (8u)

/*****************************************************************************/
/* Enumeration types definition                                               */
/*****************************************************************************/

typedef enum
{
    capsense_RES_PULLUP_E   = 0x02u,
    capsense_RES_PULLDOWN_E = 0x03u
} capsense_PORT_TEST_DM;

typedef enum
{
    capsense_STS_RESET      = 0x01u,
    capsense_STS_NO_RESET   = 0x02u
} capsense_TEST_TYPE;


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

#if ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
     (capsense_ENABLE == capsense_ADC_EN))
    capsense_SENSE_METHOD_ENUM capsense_currentSenseMethod = capsense_UNDEFINED_E;
#endif /* ((capsense_ENABLE == capsense_CSD_CSX_EN) || \
           (capsense_ENABLE == capsense_ADC_EN))) */

#if(capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
    /*  Module variable keep track of frequency hopping channel index   */
    uint8 capsense_scanFreqIndex = 0u;
    /*  Variable keep frequency offsets */
    uint8 capsense_immunity[capsense_NUM_SCAN_FREQS] = {0u, 0u, 0u};
#else
    /* const allows C-compiler to do optimization */
    const uint8 capsense_scanFreqIndex = 0u;
#endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

/* Global software variables */
volatile uint8 capsense_widgetIndex = 0u;    /* Index of the scanning widget */
volatile uint8 capsense_sensorIndex = 0u;    /* Index of the scanning sensor */
volatile uint32 capsense_curWidgetResolution = 0u;
uint8 capsense_requestScanAllWidget = 0u;
#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
    uint8 capsense_prescalersTuningDone = 0u;
#endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */

/* Pointer to RAM_SNS_STRUCT structure  */
capsense_RAM_SNS_STRUCT *capsense_curRamSnsPtr;

#if ((capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) || \
     (capsense_ENABLE == capsense_CSX_EN))
    /*  Pointer to Flash structure holding configuration of widget to be scanned  */
    capsense_FLASH_WD_STRUCT const *capsense_curFlashWdgtPtr = 0u;
#endif /* ((capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) || \
           (capsense_ENABLE == capsense_CSX_EN))  */
#if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) 
    /*  Pointer to Flash structure holding info of sensor to be scanned  */
    capsense_FLASH_SNS_STRUCT const *capsense_curFlashSnsPtr = 0u;
#endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */

/*******************************************************************************
* Function Name: capsense_IsBusy
****************************************************************************//**
*
* \brief
*   Returns the current status of the component (scan completed or scan in
*   progress).
*
* \details
*   This API checks the status flag in the capsense_dsRam structure
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
uint32 capsense_IsBusy(void)
{
    return ((*(volatile uint8 *)&capsense_dsRam.status) & capsense_SW_STS_BUSY);
}

/*******************************************************************************
* Function Name: capsense_SetupWidget
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
*   calling capsense_Scan() API to start the sensor scan.
* 
*   Note in the CSD2x mode, each widget should be set up individually with two calls to the
*   capsense_SetupWidget() API.
*   This API disconnects the electrode if it was previously connected by 
*   capsense_CSDSetupWidgetExt() or capsense_CSXSetupWidgetExt() 
*   APIs. This API does not disconnect the electrode if it was connected manually or
*   using capsense_CSDConnectSns() API.
*   The last widget will be configured if this API is called twice for different
*   widgets. API does not break the functionality if it is called twice for the 
*   same widget.
*
* \param wdgtIndex
*   Specifies the ID of the widget for which the hardware tuning parameters should be
*   initialized in the capsense HW block.
*
* \return 
*   Returns the status of operation:
*      - Zero     - The capsense HW block is initialized.
*      - Non-zero - The capsense HW block is busy and cannot be initialized.
*
**********************************************************************************/
cystatus capsense_SetupWidget(uint32 wdgtIndex)
{
    cystatus widgetStatus;

    if (capsense_WDGT_SW_STS_BUSY == (capsense_dsRam.status & capsense_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        widgetStatus = CYRET_INVALID_STATE;
    }
    /*  
     *  Check if widget id is valid, specified widget is enabled and widget did not
     *  detect any fault conditions if BIST is enabled. If all conditions are met,
     *  set widgetStatus as good, if not, set widgetStatus as bad.   
     */
        else if ((capsense_TOTAL_WIDGETS > wdgtIndex) &&
            (((uint32)capsense_dsRam.wdgtEnable[(uint8)(wdgtIndex >> capsense_WIDGET_NUM_32_DIV_SHIFT)] & 
                                                (0x00000001Lu << (wdgtIndex & capsense_WIDGET_NUM_32_MASK))) != capsense_DISABLE))
    
        {
            #if (capsense_ENABLE == capsense_SELF_TEST_EN)
                if (capsense_DISABLE != ((uint32)capsense_dsRam.wdgtWorking[(uint8)(wdgtIndex >> capsense_WIDGET_NUM_32_DIV_SHIFT)] & 
                                                 (0x00000001Lu << (wdgtIndex & capsense_WIDGET_NUM_32_MASK))))    
                {
                    widgetStatus = CYRET_SUCCESS;
                }
                else    
                {
                    widgetStatus = CYRET_INVALID_STATE;
                }
            #else
                widgetStatus = CYRET_SUCCESS;
            #endif  /* (capsense_ENABLE == capsense_SELF_TEST_EN)) */
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
        #if (capsense_ENABLE == capsense_CSD2X_EN)
            /******************************
            * This is the place where capsense_SetupWidget2x(uint32 slotIndex) 
            * API should be implemented.
            * CSD2X will be implemented
            * in the next component version.
            *********************************/
            widgetStatus = CYRET_UNKNOWN;
            #error "CSD2x not implemented in this version of component"
            
        #elif (capsense_ENABLE == capsense_CSD_CSX_EN)
            #if (capsense_ENABLE == capsense_ADC_EN)
                if (capsense_UNDEFINED_E == capsense_currentSenseMethod) 
                {
                    /* Release ADC resources */
                    widgetStatus = capsense_AdcReleaseResources();
                }
            #endif /* (capsense_ENABLE == capsense_ADC_EN) */
            
            /*  Check widget sensing method is CSX and call CSX APIs    */
            if (capsense_SENSE_METHOD_CSX_E == 
                (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))
            {
                /*  
                 *  Check if CSD block is  already initialized for CSX operation
                 *  If not, initialize CSD hardware block for CSX measurement   
                 */
                if (capsense_SENSE_METHOD_CSX_E != capsense_currentSenseMethod) 
                {
                    capsense_SwitchSensingMode(capsense_SENSE_METHOD_CSX_E);
                }

                #if (capsense_ENABLE == capsense_ADC_EN)
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /*  Set up widget for CSX scan  */
                        capsense_CSXSetupWidget(wdgtIndex);
                    }
                #else
                    /*  Set up widget for CSX scan  */
                    capsense_CSXSetupWidget(wdgtIndex);
                #endif /* (capsense_ENABLE == capsense_ADC_EN) */
            }
            /*  Check widget sensing method is CSD and call appropriate API */
            else if (capsense_SENSE_METHOD_CSD_E ==
                     (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))
            {
                /*  
                 * Check if CSD block is  already initialized for CSD operation
                 * If not, initialize CSD hardware block for CSD measurement   
                 */
                if (capsense_SENSE_METHOD_CSD_E != capsense_currentSenseMethod)
                {
                    capsense_SwitchSensingMode(capsense_SENSE_METHOD_CSD_E);
                }

                #if (capsense_ENABLE == capsense_ADC_EN)
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /*  Set up widget for CSD scan  */
                        capsense_CSDSetupWidget(wdgtIndex);
                    }
                #else
                    /*  Set up widget for CSD scan  */
                    capsense_CSDSetupWidget(wdgtIndex);
                #endif /* (capsense_ENABLE == capsense_ADC_EN) */
            }
            else
            {
                /*  Sensing method is invalid, return error to caller  */
                widgetStatus = CYRET_UNKNOWN;
            }
        #elif (capsense_ENABLE == capsense_CSD_EN)
            #if (capsense_ENABLE == capsense_ADC_EN)
                /*  
                 * Check if CSD block is  already initialized for CSD operation
                 * If not, initialize CSD hardware block for CSD measurement   
                 */
                if (capsense_SENSE_METHOD_CSD_E != capsense_currentSenseMethod)
                {
                    if (capsense_UNDEFINED_E == capsense_currentSenseMethod) 
                    {
                        /* Release ADC resources */
                        widgetStatus = capsense_AdcReleaseResources();
                    }

                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /* Initialize CSD mode to guarantee configured CSD mode after CSX calibration */
                        capsense_CSDInitialize();
                        capsense_currentSenseMethod = capsense_SENSE_METHOD_CSD_E;
                    }
                }
                
                if (CYRET_SUCCESS == widgetStatus)
                {
                    /*  Set up widget for scan */
                    capsense_CSDSetupWidget(wdgtIndex);  
                }
            #else    
                /*  Set up widget for scan */
                capsense_CSDSetupWidget(wdgtIndex);   
            #endif /* (capsense_ENABLE == capsense_ADC_EN) */
        #elif (capsense_ENABLE == capsense_CSX_EN)
            #if (capsense_ENABLE == capsense_ADC_EN)
                /*  
                 *  Check if CSD block is  already initialized for CSX operation
                 *  If not, initialize CSD hardware block for CSX measurement   
                 */
                if (capsense_SENSE_METHOD_CSX_E != capsense_currentSenseMethod) 
                {
                    if (capsense_UNDEFINED_E == capsense_currentSenseMethod) 
                    {
                        /* Release ADC resources */
                        widgetStatus = capsense_AdcReleaseResources();
                    }
                    
                    if (CYRET_SUCCESS == widgetStatus)
                    {
                        /* Initialize CSD hardware block for CSX measurement */
                        capsense_CSXInitialize();
                        capsense_currentSenseMethod = capsense_SENSE_METHOD_CSX_E;
                    }
                }
                
                if (CYRET_SUCCESS == widgetStatus)
                {
                    /*  Set up widgets for scan     */
                    capsense_CSXSetupWidget(wdgtIndex);
                }
            #else
                /*  Set up widgets for scan     */
                capsense_CSXSetupWidget(wdgtIndex);
            #endif /* (capsense_ENABLE == capsense_ADC_EN) */
        #else
            widgetStatus = CYRET_UNKNOWN;
            #error "No sensing method enabled, component cannot work in this mode"
        #endif  /* (capsense_ENABLE == capsense_CSD2X_EN) */
    }

    return (widgetStatus);
}


/*******************************************************************************
* Function Name: capsense_Scan
****************************************************************************//**
*
* \brief
*   Initiates scan for all sensors in the widget which is initialized by 
*   capsense_SetupWidget(), if the no scan is in progress
*
* \details
*   In the CSD mode, this API initializes the widget scanning in the descending order.
*   In the CSX mode, this API initializes the widget scanning in the ascending order. 
*   It connects the sensor in the widget, sets up a callback for the ISR
*   and starts scan of all the sensors in the widget which was set up in
*   the capsense_SetupWidget() API.
*   This API returns to the caller after scanning of the sensor in the 
*   widget is started, and scanning of the remaining sensors is started in the ISR. 
*   The status of scanning must be checked using the capsense_IsBusy() API  
*   prior to starting next scan or setting up a widget.
*
*   Note that the capsense HW block should be configured using  
*   the capsense_SetupWidget() API prior to calling this API.
*
* \return 
*   Returns the status of operation:
*      - Zero     - The capsense scan is started.
*      - Non-zero - The capsense HW block is busy and scan has not started.
*
**********************************************************************************/
cystatus capsense_Scan(void)
{
    cystatus scanStatus = CYRET_SUCCESS;

    if (capsense_WDGT_SW_STS_BUSY == (capsense_dsRam.status & capsense_WDGT_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        scanStatus = CYRET_INVALID_STATE;
    }
    else
    {
        /*  If CSD2X is enabled, call CSD2X scan    */
    #if (capsense_ENABLE == capsense_CSD2X_EN)
        scanStatus = capsense_Scan2x();

    /*  If both CSD and CSX are enabled, call scan API based on widget sensing method    */
    #elif (capsense_ENABLE == capsense_CSD_CSX_EN)
        /*  Check widget sensing method and call appropriate APIs   */
        switch (capsense_currentSenseMethod)
        {  
            case capsense_SENSE_METHOD_CSX_E:
                capsense_CSXScan();
                break;

            case capsense_SENSE_METHOD_CSD_E:
                 capsense_CSDScan();
                break;

            default:
                scanStatus = CYRET_UNKNOWN;
                break;
        }
        
    /*  If only CSD is enabled, call CSD scan   */
    #elif (capsense_ENABLE == capsense_CSD_EN)
        capsense_CSDScan();

    /*  If only CSX is enabled, call CSX scan   */
    #elif (capsense_ENABLE == capsense_CSX_EN)
        capsense_CSXScan();

    #else
        scanStatus = CYRET_UNKNOWN;
        #error "No sensing method enabled, component cannot work in this mode"
    #endif  /* (capsense_ENABLE == capsense_CSD2X_EN) */
    }

    return (scanStatus);
}


/*******************************************************************************
* Function Name: capsense_ScanAllWidgets
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
*   This function does not require the capsense_Scan() or 
*   capsense_SetupWidget(uint32 wdgtIndex) APIs to be called to start
*    scanning.
*
* \return 
*   Returns the status of operation:
*   - Zero        - Successfully initiated scan.
*   - Non-zero    - One or more errors occurred in the initialization process.
*
*******************************************************************************/
cystatus capsense_ScanAllWidgets(void)
{
    cystatus scanStatus = CYRET_UNKNOWN;
    
    #if (capsense_DISABLE == capsense_CSD2X_EN)
        if (capsense_SW_STS_BUSY == (capsense_dsRam.status & capsense_SW_STS_BUSY))
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
            for (capsense_widgetIndex = 0u; 
                 capsense_widgetIndex < capsense_TOTAL_WIDGETS; 
                 capsense_widgetIndex++)
            {

                scanStatus = capsense_SetupWidget((uint32)capsense_widgetIndex);

                if (CYRET_SUCCESS == scanStatus)
                {
                    #if (0u != (capsense_TOTAL_WIDGETS - 1u))
                        /* If there are more than one widget to be scanned, request callback to scan next widget */
                        if ((capsense_TOTAL_WIDGETS - 1u) > capsense_widgetIndex)
                        {
                             /* Request callback to scan next widget in ISR */
                            capsense_requestScanAllWidget = capsense_ENABLE;
                        }
                        else
                        {
                            /* Request to exit in ISR (Do not scan the next widgets) */
                            capsense_requestScanAllWidget = capsense_DISABLE;
                        }
                    #else 
                        {
                            /* Request to exit in ISR (We have only one widget) */
                            capsense_requestScanAllWidget = capsense_DISABLE;
                        }
                    #endif  /* (0u != (capsense_TOTAL_WIDGETS - 1u)) */
                    
                    /*  Initiate scan and quit loop */
                    scanStatus = capsense_Scan();

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
    #endif /* (capsense_DISABLE == capsense_CSD2X_EN) */

    return (scanStatus);
}


/*******************************************************************************
* Function Name: capsense_SsInitialize
****************************************************************************//**
*
* \brief
*   Performs hardware and firmware initialization required for proper operation
*   of the capsense component. This function is called from 
*   the CapSense_Start() API prior to calling any other APIs of the component.
*
* \details
*   Performs hardware and firmware initialization required for proper operation
*   of the capsense component. This function is called from 
*   the CapSense_Start() API prior to calling any other APIs of the component.
*   1. The function initialises immunity offsets when the frequency hopping is 
*      enabled.
*   2. Depending on the configuration, the function initialises the CSD block 
*      for the CSD2X, CSD, CSX, or CSD+CSX modes.
*   3. The function updates the dsRam.wdgtWorking variable with 1 when Self Test
*      is enabled.
*
*   Calling the capsense_Start API is the recommended method to initialize
*   the capsense component at power-up. The capsense_SsInitialize() 
*   API should not be used for initialization, resume, or wake-up operations.
*   The dsRam.wdgtWorking variable is updated.
*
* \return status
*   Returns status of operation:
*   - Zero        - Indicates successful initialization.
*   - Non-zero    - One or more errors occurred in the initialization process.
*
*******************************************************************************/
cystatus capsense_SsInitialize(void)
{
    cystatus initStatus = CYRET_SUCCESS;

    #if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
        capsense_immunity[capsense_FREQ_CHANNEL_0] = (uint8)CY_GET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR);
        if (capsense_COARSE_TRIM_THRESHOLD_1 > capsense_immunity[capsense_FREQ_CHANNEL_0])
        {
            capsense_immunity[capsense_FREQ_CHANNEL_1] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] + capsense_FREQUENCY_OFFSET_5;
            capsense_immunity[capsense_FREQ_CHANNEL_2] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] + capsense_FREQUENCY_OFFSET_10;
        }
        else if (capsense_COARSE_TRIM_THRESHOLD_2 > capsense_immunity[capsense_FREQ_CHANNEL_0])
        {
            capsense_immunity[capsense_FREQ_CHANNEL_1] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] - capsense_FREQUENCY_OFFSET_5;
            capsense_immunity[capsense_FREQ_CHANNEL_2] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] + capsense_FREQUENCY_OFFSET_5;
        }
        else
        {
            capsense_immunity[capsense_FREQ_CHANNEL_1] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] - capsense_FREQUENCY_OFFSET_5;
            capsense_immunity[capsense_FREQ_CHANNEL_2] = 
            capsense_immunity[capsense_FREQ_CHANNEL_0] - capsense_FREQUENCY_OFFSET_10;
        }
    #endif /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */

    /*  
     *  Check if CSD2X is enabled, if yes, initialize capsense component
     *  for CSD2x operation 
     */
    #if (capsense_ENABLE == capsense_CSD2X_EN)
        /*  
         *  Check if CSX or CSD enable along with CSD2X, if yes, provide the error
         *  message as CSD2X can not coexist with CSD or CSX    
         */
        #if (capsense_ENABLE == capsense_CSX_EN)
            #error "CSD2X and CSX can not be enabled together"
            initStatus = CYRET_UNKNOWN;
        #elif (capsense_ENABLE == capsense_CSD_EN)
            #error "CSD2X and CSD can not be enabled together"
            initStatus = CYRET_UNKNOWN;
        #endif /* (capsense_CSX_EN == capsense_ENABLE) */

        /* Initialize both CSD blocks for CSD2X scanning    */
        capsense_CSD2XInitialize();

    #elif (capsense_ENABLE == capsense_CSD_CSX_EN)
        /* 
         * Do nothing, CSD hardware shall be initialized in the Setup Widget based
         * on widget sensing method    
         */         
        capsense_currentSenseMethod = capsense_UNDEFINED_E;    

    #elif (capsense_ENABLE == capsense_CSD_EN)
        /*  Initialize CSD block for CSD scanning   */
        capsense_CSDInitialize();

    #elif (capsense_ENABLE == capsense_CSX_EN)
        /*  Initialize CSD block for CSX scanning   */
        capsense_CSXInitialize();
        
    #else
        #error "No sensing method enabled, component cannot work in this mode"
        initStatus = CYRET_UNKNOWN;
    #endif  /* (capsense_ENABLE == capsense_CSD2X_EN) */

    return (initStatus);
}


/*******************************************************************************
* Function Name: capsense_SetPinState
****************************************************************************//**
*
* \brief
*   Sets the status (drive mode and output state) of a sensor to six states
*   relevant (GND, Shield,  HIgh-Z, Tx or Rx, sensor) to the capsense operation.
*   If the sensor is configured as a ganged sensor, then the API applies to the  
*   dedicated pin and all ganged electrodes.
*
* \details
*   This API disconnects port pins from AMUXBUS and sets the drive mode of a port pin
*   (Rx, Tx or Sns electrode) based on an input parameter. The mode of the pin
*   is set to HIgh-Z in the GPIO_PRT_PCx register.
*
*   Scanning should be completed before calling this API. 
*   capsense_SHIELD is not allowed if Enable shield electrode parameter
*   is disabled.
*   capsense_TX_PIN and capsense_RX_PIN are not allowed if there 
*    are no other CSX widgets configured in the project.
*
*  \param wdgtIndex Specifies the ID of the widget.
*   
*  \param snsIndex Specifies the ID of the snsIndex.
*   
*  \param state Specifies the state to be set for a specified pin:
*          -   (0) capsense_GROUND
*          -   (1) capsense_HIGHZ
*          -   (2) capsense_SHIELD
*          -   (3) capsense_SENSOR
*          -   (4) capsense_TX_PIN
*          -   (5) capsense_RX_PIN
*
*******************************************************************************/
void capsense_SetPinState(uint32 wdgtIndex, uint32 snsIndex, uint32 state)
{
    capsense_FLASH_IO_STRUCT const *curSnsIOPtr;
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        capsense_FLASH_SNS_STRUCT const *curFlashSnsPtr;
        uint32 tempVal;
    #endif /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
    uint32 newRegisterValue;
    uint8  interruptState;
    uint32 pinHSIOMShift;
    uint32 pinModeShift;
    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        /* Check ganged sns flag  */
        if (capsense_GANGED_SNS_MASK == (capsense_dsFlash.wdgtArray[wdgtIndex].staticConfig & 
            capsense_GANGED_SNS_MASK))
        {             
            curFlashSnsPtr = capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash;
            curFlashSnsPtr += snsIndex;
            curSnsIOPtr = &capsense_ioList[curFlashSnsPtr->firstPinId];
            
            /* Get number of ganged pins  */
            tempVal = curFlashSnsPtr->numPins;
        }
        else
        {
            curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                    capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash + snsIndex;
            /* There are no ganged pins */
            tempVal = 1u;                                
        }
    #else
        curSnsIOPtr = (capsense_FLASH_IO_STRUCT const *)
                                    capsense_dsFlash.wdgtArray[wdgtIndex].ptr2SnsFlash + snsIndex;
    #endif  /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
    
    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
        /* Reconfigure all ganged sensors  */
        do
        {
    #endif  /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */

            /* Get HSIOM and port mode shifts */
            pinHSIOMShift = (uint32)curSnsIOPtr->hsiomShift;
            pinModeShift = (uint32)curSnsIOPtr->shift;
            
            /* Reset HSIOM register */
            CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) & ~(capsense_HSIOM_SEL_MASK << pinHSIOMShift));
           
            switch (state)
            {  
            case capsense_GROUND:
                interruptState = CyEnterCriticalSection();

                /* Update port configuration register (drive mode) for sensor */
                newRegisterValue = CY_GET_REG32(curSnsIOPtr->pcPtr);
                newRegisterValue &= ~(capsense_GPIO_PC_MASK << pinModeShift);
                newRegisterValue |= (capsense_SNS_GROUND_CONNECT << pinModeShift);
                CY_SET_REG32(curSnsIOPtr->pcPtr, newRegisterValue);

                CyExitCriticalSection(interruptState);

                /* Set logic 0 to port data register */
                CY_SET_REG32(curSnsIOPtr->drPtr, CY_GET_REG32(curSnsIOPtr->drPtr) & (uint32)~(uint32)((uint32)1u << curSnsIOPtr->drShift));
                break;
                
            case capsense_HIGHZ:
                interruptState = CyEnterCriticalSection();

                /* Update port configuration register (drive mode) for sensor */
                newRegisterValue = CY_GET_REG32(curSnsIOPtr->pcPtr);
                newRegisterValue &= ~(capsense_GPIO_PC_MASK << pinModeShift);
                CY_SET_REG32(curSnsIOPtr->pcPtr, newRegisterValue);

                CyExitCriticalSection(interruptState);

                /* Set logic 0 to port data register */
                CY_SET_REG32(curSnsIOPtr->drPtr, CY_GET_REG32(curSnsIOPtr->drPtr) & (uint32)~(uint32)((uint32)1u << curSnsIOPtr->drShift));
                break;

            #if (capsense_ENABLE == capsense_CSD_SHIELD_EN)
                case capsense_SHIELD:
                    /* Set drive mode to Analog */
                    CY_SET_REG32(curSnsIOPtr->pcPtr, CY_GET_REG32(curSnsIOPtr->pcPtr) &
                               ~(capsense_GPIO_PC_MASK <<(curSnsIOPtr->shift)));

                    /* Set appropriate bit in HSIOM register to configure pin to shield mode */
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | 
                                                       (capsense_HSIOM_SEL_CSD_SHIELD << pinHSIOMShift));
                    break;
            #endif  /* (capsense_ENABLE == capsense_CSD_SHIELD_EN) */
            
            #if ((capsense_ENABLE == capsense_CSD_EN) || \
                 (capsense_ENABLE == capsense_CSD_CSX_EN))
                case capsense_SENSOR:
                    /* Enable sensor */
                    capsense_CSDConnectSns(curSnsIOPtr);
                    break;
            #endif  /* ((capsense_ENABLE == capsense_CSD_EN) || \
                        (capsense_ENABLE == capsense_CSD_CSX_EN)) */
             
            #if ((capsense_ENABLE == capsense_CSX_EN) || \
                 (capsense_ENABLE == capsense_CSD_CSX_EN))
                case capsense_TX_PIN:
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | (capsense_HSIOM_SEL_CSD_SENSE << pinHSIOMShift));
                    break;
                    
                case capsense_RX_PIN:
                    CY_SET_REG32(curSnsIOPtr->hsiomPtr, CY_GET_REG32(curSnsIOPtr->hsiomPtr) | (capsense_HSIOM_SEL_AMUXA << pinHSIOMShift));
                    CY_SET_REG32(curSnsIOPtr->pcPtr, CY_GET_REG32(curSnsIOPtr->pcPtr) & ~(capsense_GPIO_PC_MASK << pinModeShift));
                    break;
            #endif  /* ((capsense_ENABLE == capsense_CSX_EN) || \
                        (capsense_ENABLE == capsense_CSD_CSX_EN)) */
             
            default:
                /* Wrong input */
                break;
            }

    #if (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN)
            curSnsIOPtr++;
            tempVal--;
        } while (0u != tempVal);
    #endif  /* (capsense_ENABLE == capsense_CSD_GANGED_SNS_EN) */
}


#if (capsense_ENABLE == capsense_CSD_CSX_EN)
/*******************************************************************************
* Function Name: capsense_SwitchSensingMode
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
*           -  (1) capsense_SENSE_METHOD_CSD_E
*           -  (2) capsense_SENSE_METHOD_CSX_E
*
*******************************************************************************/
void capsense_SwitchSensingMode(capsense_SENSE_METHOD_ENUM mode)
{
    uint32 newRegValue;
        
    if (capsense_SENSE_METHOD_CSD_E == mode) 
    {
        /*  Disconnect CintA from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(capsense_CintA_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)capsense_CintA_HSIOM_MASK);
        CY_SET_REG32(capsense_CintA_HSIOM_PTR, newRegValue);
        
        /*  Disconnect CintB from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(capsense_CintB_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)capsense_CintB_HSIOM_MASK);
        CY_SET_REG32(capsense_CintB_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSX electrode if it has been connected */
        capsense_CSXElectrodeCheck();
        
        /* Initialize CSD mode to guarantee configured CSD mode after CSX calibration */
        capsense_CSDInitialize();
        capsense_currentSenseMethod = capsense_SENSE_METHOD_CSD_E;
    }
    else if (capsense_SENSE_METHOD_CSX_E == mode) 
    {
        /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */ 
        newRegValue = CY_GET_REG32(capsense_CMOD_HSIOM_PTR); 
        newRegValue &= (uint32)(~(uint32)capsense_CMOD_HSIOM_MASK);
        CY_SET_REG32(capsense_CMOD_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSD electrode if it has been connected */
        capsense_CSDElectrodeCheck();

        /* Disconnect Csh from AMUXBUS-B */
        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN))
            newRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR);
            newRegValue &= (uint32)(~(uint32)(capsense_CSH_TO_AMUXBUS_B_MASK << capsense_CSH_HSIOM_SHIFT));
            CY_SET_REG32(capsense_CSH_HSIOM_PTR, newRegValue);
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN)) */
        
        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
            capsense_DisableShieldElectrodes();
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */
        
        /* Initialize CSD hardware block for CSX measurement */
        capsense_CSXInitialize();
        capsense_currentSenseMethod = capsense_SENSE_METHOD_CSX_E;
    }
    else
    {
        capsense_currentSenseMethod = capsense_UNDEFINED_E;  
    }
}
#endif  /* (capsense_ENABLE == capsense_CSD_CSX_EN)) */


#if (capsense_ENABLE == capsense_ADC_EN)
/*******************************************************************************
* Function Name: capsense_SsReleaseResources()
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
cystatus capsense_SsReleaseResources(void)
{
    cystatus busyStatus = CYRET_SUCCESS;
    uint32 newRegValue;

    if (capsense_SW_STS_BUSY == (capsense_dsRam.status & capsense_SW_STS_BUSY))
    {
        /* Previous widget is being scanned. Return error. */
        busyStatus = CYRET_INVALID_STATE;
    }
    else
    {
    #if (capsense_ENABLE == capsense_CSX_EN)
        /*  Disconnect CintA from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(capsense_CintA_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)capsense_CintA_HSIOM_MASK);
        CY_SET_REG32(capsense_CintA_HSIOM_PTR, newRegValue);
        
        /*  Disconnect CintB from AMUXBUS-A using HSIOM registers.   */
        newRegValue = CY_GET_REG32(capsense_CintB_HSIOM_PTR);
        newRegValue &= (uint32)(~(uint32)capsense_CintB_HSIOM_MASK);
        CY_SET_REG32(capsense_CintB_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSX electrode if it has been connected */
        capsense_CSXElectrodeCheck();
    #endif /* (capsense_ENABLE == capsense_CSX_EN) */
    
    #if (capsense_ENABLE == capsense_CSD_EN)
        /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */ 
        newRegValue = CY_GET_REG32(capsense_CMOD_HSIOM_PTR); 
        newRegValue &= (uint32)(~(uint32)capsense_CMOD_HSIOM_MASK);
        CY_SET_REG32(capsense_CMOD_HSIOM_PTR, newRegValue);

        /* Disconnect previous CSD electrode if it has been connected */
        capsense_CSDElectrodeCheck();
        
        /* Disconnect Csh from AMUXBUS-B */
        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN))
            newRegValue = CY_GET_REG32(capsense_CSH_HSIOM_PTR);
            newRegValue &= (uint32)(~(uint32)(capsense_CSH_TO_AMUXBUS_B_MASK << capsense_CSH_HSIOM_SHIFT));
            CY_SET_REG32(capsense_CSH_HSIOM_PTR, newRegValue);
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (capsense_ENABLE == capsense_CSD_SHIELD_TANK_EN)) */
        
        #if ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
             (0u != capsense_CSD_TOTAL_SHIELD_COUNT))
            capsense_DisableShieldElectrodes();
        #endif /* ((capsense_ENABLE == capsense_CSD_SHIELD_EN) && \
                   (0u != capsense_CSD_TOTAL_SHIELD_COUNT)) */
    #endif /* (capsense_ENABLE == capsense_CSD_EN) */

        /* 
         * Reset of the currentSenseMethod variable to make sure that the next
         * call of SetupWidget() API setups the correct widget mode
         */
        capsense_currentSenseMethod = capsense_UNDEFINED_E;
    }

    return busyStatus;
}
#endif /* (capsense_ENABLE == capsense_ADC_EN) */


/*******************************************************************************
* Function Name: capsense_PostAllWidgetsScan
****************************************************************************//**
*
* \brief
*   The ISR function for multiple widget scanning implementation.
*
* \details
*   This is the function used by the capsense ISR to implement multiple widget
*   scanning.
*   Should not be used by the application layer.
*
*******************************************************************************/
void capsense_PostAllWidgetsScan(void)
{
    #if (capsense_DISABLE == capsense_CSD2X_EN)
        /*  
        *   1. Increment widget index
        *   2. Check if all widgets are scanned
        *   3. If all widgets are not scanned, set up and scan next widget  
        */
        #if (1u != capsense_TOTAL_WIDGETS)  
            cystatus postScanStatus;
        
            do
            {
                capsense_widgetIndex++;

                postScanStatus = capsense_SetupWidget((uint32)capsense_widgetIndex);

                if (CYRET_SUCCESS == postScanStatus)
                {
                    if((capsense_TOTAL_WIDGETS - 1u) == capsense_widgetIndex)
                    {
                        /* The last widget will be scanned. Reset flag to skip configuring the next widget setup in ISR. */
                        capsense_requestScanAllWidget = capsense_DISABLE;
                    }
                    (void)capsense_Scan();
                }
                else if((capsense_TOTAL_WIDGETS - 1u) == capsense_widgetIndex)
                {
                    #if ((capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) && \
                         (capsense_ENABLE == capsense_CSD_EN))
                        if (capsense_SENSE_METHOD_CSD_E ==
                             (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[capsense_widgetIndex]))
                        {
                            /*  Disable the CSD block */
                            CY_SET_REG32(capsense_CONFIG_PTR, capsense_configCsd);
                        }
                    #endif /* ((capsense_ENABLE == capsense_BLOCK_OFF_AFTER_SCAN_EN) && \
                               (capsense_ENABLE == capsense_CSD_EN)) */
                    
                    /* All widgets are totally processed. Reset BUSY flag */
                    capsense_dsRam.status &= ~capsense_SW_STS_BUSY;
                    
                    /* Update status with with the failure */
                    capsense_dsRam.status &= ~capsense_STATUS_ERR_MASK;                
                    capsense_dsRam.status |= ((postScanStatus & capsense_STATUS_ERR_SIZE) << capsense_STATUS_ERR_SHIFT);
                    
                    /* Set postScanStatus to exit the while loop */
                    postScanStatus = CYRET_SUCCESS;
                }
                else
                {
                    /* Update status with with the failure. Configure the next widget in while() loop */
                    capsense_dsRam.status &= ~capsense_STATUS_ERR_MASK;                
                    capsense_dsRam.status |= ((postScanStatus & capsense_STATUS_ERR_SIZE) << capsense_STATUS_ERR_SHIFT);
                }
            } while (CYRET_SUCCESS != postScanStatus);
        #endif /* (1u != capsense_TOTAL_WIDGETS) */
    #else
        /******************************
        * This is the place where the CSD2X 
        * postscan for 2 widgets should be implemented.
        * The CSD2X postscan will be implemented
        * in the next component version.
        *********************************/
    #endif /* (capsense_CSD2X_EN == capsense_DISABLE) */
}


/*******************************************************************************
* Function Name: capsense_IsrInitialize
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
void capsense_IsrInitialize(cyisraddress address)
{
#if (capsense_ENABLE == capsense_CSD2X_EN)
    /******************************
    * This is the place where the CSD2X ISR 
    * Setup should be implemented.
    * The CSD2X ISR Setup will be implemented
    * in the next component version.
    *********************************/
#else
    capsense_ISR_StartEx(address);
#endif /* (capsense_ENABLE == capsense_CSD2X_EN) */
}


/*******************************************************************************
* Function Name: capsense_ConfigureModClkClock
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
void capsense_ConfigureModClkClock(uint32 modClk)
{
#if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
    /*  Stop modulator clock   */
    CY_SET_REG32(capsense_MODCLK_CMD_PTR, 
                 ((uint32)capsense_ModClk__DIV_ID <<
                 capsense_MODCLK_CMD_DIV_SHIFT)|
                 ((uint32)capsense_MODCLK_CMD_DISABLE_MASK));

    /*  
     * Set divider value for sense modClk.
     * 1u is subtracted from modClk because Divider register value 0 corresponds
     * to dividing by 1.  
     */
    CY_SET_REG32(capsense_MODCLK_DIV_PTR, ((modClk - 1u) << 8u));  

    /*  Check whether previous modulator clock start command has finished. */
    while(0u != (CY_GET_REG32(capsense_MODCLK_CMD_PTR) & capsense_MODCLK_CMD_ENABLE_MASK))
    {
        /*  Wait until previous modulator clock start command has finished. */  
    }
    
    /*  Start modulator clock, aligned to HFCLK */
    CY_SET_REG32(capsense_MODCLK_CMD_PTR, 
                 (uint32)(((uint32)capsense_ModClk__DIV_ID << capsense_MODCLK_CMD_DIV_SHIFT) |
                  ((uint32)capsense_ModClk__PA_DIV_ID << capsense_MODCLK_CMD_PA_DIV_SHIFT) |
                  capsense_MODCLK_CMD_ENABLE_MASK));
#else
    uint32 newRegValue;

    /* Clear bit to disable ModClk clock. */
    CY_SET_REG32(capsense_MODCLK_CMD_PTR, 
                 CY_GET_REG32(capsense_MODCLK_CMD_PTR) & 
                 (uint32)(~capsense_ModClk__ENABLE_MASK));
        
    /* 
     * Update ModClk clock divider.
     * 1u is subtracted from modClk because Divider register value has range (0-65535).  
     */
    newRegValue = CY_GET_REG32(capsense_MODCLK_DIV_PTR) & (uint32)(~(uint32)capsense_ModClk__DIVIDER_MASK);
    newRegValue |= (modClk - 1u);
    CY_SET_REG32(capsense_MODCLK_DIV_PTR, newRegValue);
    
    /* Set bit to enable ModClk clock. */
    CY_SET_REG32(capsense_MODCLK_CMD_PTR, 
                 CY_GET_REG32(capsense_MODCLK_CMD_PTR) | 
                capsense_ModClk__ENABLE_MASK);
#endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */
}


/*******************************************************************************
* Function Name: capsense_ConfigureSnsClock
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
void capsense_ConfigureSnsClock(uint32 snsClk)
{
#if (capsense_ENABLE == capsense_CSDV2)
    uint32 newRegValue;

    /*  
     * Set divider value for sense clock.
     * 1u is subtracted from snsClk because SENSE_DIV value 0 corresponds
     * to dividing by 1.  
     */
    newRegValue = CY_GET_REG32(capsense_SENSE_PERIOD_PTR); 
    newRegValue &= (uint32)(~capsense_SENSE_PERIOD_SENSE_DIV_MASK);
    newRegValue |= snsClk - 1u;
    CY_SET_REG32(capsense_SENSE_PERIOD_PTR, newRegValue);
#else
    #if (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
        /*  Stop sense clock clock   */
        CY_SET_REG32(capsense_SNSCLK_CMD_PTR, 
                     ((uint32)capsense_SnsClk__DIV_ID <<
                     capsense_SNSCLK_CMD_DIV_SHIFT)|
                     ((uint32)capsense_SNSCLK_CMD_DISABLE_MASK));

        /*  
         * Set divider value for sense clock.
         * 1u is subtracted from snsClk because Divider register value 0 corresponds
         * to dividing by 1.  
         */
        CY_SET_REG32(capsense_SNSCLK_DIV_PTR, ((snsClk - 1u) << 8u));
        
        /*  Check whether previous sense clock start command has finished. */
        while(0u != (CY_GET_REG32(capsense_SNSCLK_CMD_PTR) & capsense_SNSCLK_CMD_ENABLE_MASK))
        {
            /*  Wait until previous sense clock start command has finished. */  
        } 
        
        /* Start sense clock aligned to previously started modulator clock. */
        CY_SET_REG32(capsense_SNSCLK_CMD_PTR, 
                     (uint32)(((uint32)capsense_SnsClk__DIV_ID << capsense_SNSCLK_CMD_DIV_SHIFT) |
                      ((uint32)capsense_ModClk__PA_DIV_ID << capsense_SNSCLK_CMD_PA_DIV_SHIFT) |
                      capsense_SNSCLK_CMD_ENABLE_MASK));
    #else
        uint32 newRegValue;

        /* Clear bit to disable SnsClk clock. */
        CY_SET_REG32(capsense_SNSCLK_CMD_PTR, 
                     CY_GET_REG32(capsense_SNSCLK_CMD_PTR) & 
                     (uint32)(~(uint32)capsense_SnsClk__ENABLE_MASK));
        
        /* 
         * Update snsClk clock divider.
         * 1u is subtracted from snsClk because Divider register value has range (0-65535).  
         */
        newRegValue = CY_GET_REG32(capsense_SNSCLK_DIV_PTR) & (uint32)(~(uint32)capsense_SnsClk__DIVIDER_MASK);
        newRegValue |= (snsClk - 1u);
        CY_SET_REG32(capsense_SNSCLK_DIV_PTR, newRegValue);

        /* Set bit to enable SnsClk clock. */
        CY_SET_REG32(capsense_SNSCLK_CMD_PTR, 
                     CY_GET_REG32(capsense_SNSCLK_CMD_PTR) | 
                    capsense_SnsClk__ENABLE_MASK);
    #endif /* (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) */
#endif /* (capsense_ENABLE == capsense_CSDV2) */
}


/*******************************************************************************
* Function Name: capsense_ConfigureClocks
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
void capsense_ConfigureClocks(uint32 snsClk, uint32 modClk)
{
    /* Configure Mod clock */
    capsense_ConfigureModClkClock(modClk);

    /* Configure Sns clock */
    capsense_ConfigureSnsClock(snsClk);
}


#if ((capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN) || \
     (capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN))
/*******************************************************************************
* Function Name: capsense_CalibrateWidget 
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
cystatus capsense_CalibrateWidget(uint32 wdgtIndex)
{
    cystatus calibrateStatus = CYRET_SUCCESS;

    do
    {   
        if (capsense_TOTAL_WIDGETS <= wdgtIndex)
        {
            calibrateStatus = CYRET_BAD_PARAM;
            break;
        }
    
        /*  
         *  Check if widget id is valid, specified widget did not
         *  detect any faults conditions if BIST is enabled.   
         */
        #if (capsense_ENABLE == capsense_SELF_TEST_EN)
            if (capsense_DISABLE != ((uint32)capsense_dsRam.wdgtWorking[(uint8)wdgtIndex >> capsense_WIDGET_NUM_32_DIV_SHIFT] & 
                                             (0x00000001Lu << (wdgtIndex & capsense_WIDGET_NUM_32_MASK))))    
            {
                calibrateStatus = CYRET_SUCCESS;
            }
            else    
            {
                calibrateStatus = CYRET_INVALID_STATE;
                /*  Exit because widget is not working  */
                break;
            }
        #endif  /* (capsense_ENABLE == capsense_SELF_TEST_EN) */
    
        #if (capsense_ENABLE == capsense_ADC_EN)
            if (capsense_UNDEFINED_E == capsense_currentSenseMethod) 
            {
                /* Release ADC resources */
                calibrateStatus = capsense_AdcReleaseResources();

                if (CYRET_SUCCESS != calibrateStatus)
                {
                    /* Exit because widget is not working */
                    break;
                }
            }
        #endif /* (capsense_ENABLE == capsense_ADC_EN) */

        /*  If CSD2X is enabled, calibrate CSD sensor    */
        #if (capsense_ENABLE == capsense_CSD2X_EN)
            /******************************
            * This is the place where the CSD2X mode
            * API should be implemented.
            * CSD2X will be implemented
            * in the next component version.
            *********************************/

        /*  If both CSD and CSX are enabled, calibrate widget using sensing method */
        #elif (capsense_ENABLE == capsense_CSD_CSX_EN)

            /*  Check widget sensing method and call appropriate APIs   */
            #if ((capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN) && (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN))
                #if (capsense_CSD_SS_DIS == capsense_CSD_AUTOTUNE)
                    if (capsense_SENSE_METHOD_CSX_E ==
                        (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))   
                    {
                        /* Calibrate CSX widget (when Autotune mode is disabled) */
                       capsense_CSXCalibrateWidget(wdgtIndex, capsense_CSX_RAWCOUNT_CAL_LEVEL);
                    }
                    else if (capsense_SENSE_METHOD_CSD_E ==
                        (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))
                    {
                        /*  
                         *  Check if CSD block is  already initialized for CSD operation
                         *  If not, initialize CSD hardware block for CSD measurement   
                         */
                        if (capsense_SENSE_METHOD_CSD_E != capsense_currentSenseMethod) 
                        {
                            capsense_SwitchSensingMode(capsense_SENSE_METHOD_CSD_E);
                        }
                        
                        /* Calibrate CSD widget (when Autotune mode is disabled) */
                        calibrateStatus = capsense_CSDCalibrateWidget(wdgtIndex, capsense_CSD_RAWCOUNT_CAL_LEVEL);
                    }
                    else    
                    {
                        calibrateStatus = CYRET_UNKNOWN;
                    }
                #else
                    if (capsense_SENSE_METHOD_CSX_E ==
                        (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))   
                    {
                        /* Calibrate CSX widget only (when Autotune mode is enabled) */
                       capsense_CSXCalibrateWidget(wdgtIndex, capsense_CSX_RAWCOUNT_CAL_LEVEL);
                    }
                #endif /* (capsense_CSD_SS_DIS == capsense_CSD_AUTOTUNE) */
            #elif (capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN)
                if (capsense_SENSE_METHOD_CSX_E ==
                    (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))   
                {
                    /* Calibrate CSX widget only */
                   capsense_CSXCalibrateWidget(wdgtIndex, capsense_CSX_RAWCOUNT_CAL_LEVEL);
                }          
            #elif ((capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN) && (capsense_CSD_SS_DIS == capsense_CSD_AUTOTUNE))
                if (capsense_SENSE_METHOD_CSD_E ==
                    (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex])) 
                {
                    /* Initialize CSD mode */
                    capsense_CSDInitialize();

                    /* Calibrate CSD widget only */
                    calibrateStatus = capsense_CSDCalibrateWidget(wdgtIndex, capsense_CSD_RAWCOUNT_CAL_LEVEL);
                }
            #else
                /*  No calibration  */
            #endif  /* ((capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN) && (capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN)) */
            
        /*  If only CSD is enabled, calibrate CSD sensor  */
        #elif ((capsense_ENABLE == capsense_CSD_EN) && (capsense_CSD_SS_DIS == capsense_CSD_AUTOTUNE))
            calibrateStatus = capsense_CSDCalibrateWidget(wdgtIndex, capsense_CSD_RAWCOUNT_CAL_LEVEL);
        
        /*  If only CSX is enabled, call CSX scan   */
        #elif (capsense_ENABLE == capsense_CSX_EN)
            capsense_CSXCalibrateWidget(wdgtIndex, capsense_CSX_RAWCOUNT_CAL_LEVEL);

        #else
            calibrateStatus = CYRET_UNKNOWN;
        #endif  /* (capsense_ENABLE == capsense_CSD2X_EN) */   
    
    } while (0u);
    
    return calibrateStatus;
}


/*******************************************************************************
* Function Name: capsense_CalibrateAllWidgets 
****************************************************************************//**
*
* \brief
*  Executes the IDAC calibration for all the widgets in the component.
*
* \details
*   The function goes through all widgets and calls 
*   capsense_CalibrateWidget() for each widget.
*   It returns a fail status if any widget has failed to calibrate or it   
*   has been failed during the self-test.
*
* \return status: 
*   - Zero     -  All the widgets are calibrated successfully.
*   - Non-zero - Calibration failed for any widget.
*
*******************************************************************************/
cystatus capsense_CalibrateAllWidgets(void)
{
    cystatus calibrateStatus = CYRET_SUCCESS;
    uint32 wdgtIndex;
    
    for (wdgtIndex = 0u; wdgtIndex < capsense_TOTAL_WIDGETS; wdgtIndex++)
    {
        calibrateStatus |= capsense_CalibrateWidget(wdgtIndex); 
    }
    
    return calibrateStatus;
}

#endif /* ((capsense_ENABLE == capsense_CSD_IDAC_AUTOCAL_EN) || 
           (capsense_ENABLE == capsense_CSX_IDAC_AUTOCAL_EN))  */


#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
/*******************************************************************************
* Function Name: capsense_AutoTune
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
cystatus capsense_AutoTune(void)
{
    cystatus autoTuneStatus = CYRET_SUCCESS;
    uint32 wdgtIndex;
    uint32 cp;
    capsense_RAM_WD_BASE_STRUCT *ptrWdgt;
    AUTO_TUNE_CONFIG_TYPE autoTuneConfig;  
    
    /* Configure common config variables */
    autoTuneConfig.snsClkConstantR = capsense_CSD_SNSCLK_R_CONST;
    autoTuneConfig.vRef = capsense_CSD_VREF_MV;
    autoTuneConfig.iDacGain = capsense_CSD_IDAC_GAIN_VALUE_NA * capsense_CSD_DUAL_IDAC_FACTOR;

    /* Calculate snsClk Input Clock in KHz */
    #if (capsense_ENABLE == capsense_CSDV2)
        /*  Dividers are chained (CSDV2). Flip flop is not available */
        autoTuneConfig.snsClkInputClock = (CYDEV_BCLK__HFCLK__KHZ / capsense_dsRam.modCsdClk);
    #elif ((capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK) && (capsense_DISABLE == CY_PSOC4_4000))
        /*  Dividers are not chained */
        autoTuneConfig.snsClkInputClock = CYDEV_BCLK__HFCLK__KHZ >> capsense_FLIP_FLOP_DIV;
    #elif (capsense_ENABLE == capsense_IS_M0S8PERI_BLOCK)
        /*  Dividers are not chained (PSoC 4000) */
        autoTuneConfig.snsClkInputClock = CYDEV_BCLK__HFCLK__KHZ >> capsense_FLIP_FLOP_DIV;
    #else
        /*  Dividers are chained (PSoC 4100, PSoC 4200) */
        autoTuneConfig.snsClkInputClock = (CYDEV_BCLK__HFCLK__KHZ / capsense_dsRam.modCsdClk) >> capsense_FLIP_FLOP_DIV;
    #endif /* (capsense_ENABLE == capsense_CSDV2) */
 
    /* If both CSD and CSX are enabled, calibrate widget using sensing method */
    #if (capsense_ENABLE == capsense_CSD_CSX_EN)
        /* Initialize CSD mode */
        capsense_CSDInitialize();
    #endif /* (capsense_ENABLE == capsense_CSD_CSX_EN) */
 
    for (wdgtIndex = 0u; wdgtIndex < capsense_TOTAL_WIDGETS; wdgtIndex++)
    {
        if (capsense_SENSE_METHOD_CSD_E ==
            (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[wdgtIndex]))
        {
            ptrWdgt = (capsense_RAM_WD_BASE_STRUCT *)
                      capsense_dsFlash.wdgtArray[wdgtIndex].ptr2WdgtRam;

            /* Switch charge clock source to direct clock mode */ 
            capsense_SET_DIRECT_CLOCK_MODE;

            /* Get sensitivity and multiply it to 2 for PRS mode (to take into account average PRS frequency) */
            autoTuneConfig.sensitivity = ptrWdgt->sensitivity << capsense_PRS_FACTOR_DIV;

            /* Init pointer to sigPPf */
            autoTuneConfig.sigPPf = &ptrWdgt->sigPPf;

            /* Set calibration resolution to 12 bits */
            ptrWdgt->resolution = capsense_CALIBRATION_RESOLUTION;

            /* Set Sense clock frequency to 1.5 MHz */
            #if (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN)
                capsense_dsRam.snsCsdClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                                   capsense_CALIBRATION_FREQ_KHZ);
            #elif (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
                ptrWdgt->rowSnsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                     capsense_CALIBRATION_FREQ_KHZ);    
                ptrWdgt->snsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                  capsense_CALIBRATION_FREQ_KHZ);
            #else
                ptrWdgt->snsClk = (uint8)((uint32)autoTuneConfig.snsClkInputClock / 
                                  capsense_CALIBRATION_FREQ_KHZ);
            #endif /*  (capsense_ENABLE == capsense_CSD_COMMON_SNS_CLK_EN) */

            #if (capsense_ENABLE == capsense_CSDV2)
                capsense_ConfigureSnsClock((uint32)ptrWdgt->snsClk);
            #endif /* (capsense_ENABLE == capsense_CSDV2) */
        
            /* Set flag to calibrate in PWM mode only */
            capsense_prescalersTuningDone = capsense_DISABLE;

            #if (capsense_DISABLE == capsense_CSD2X_EN)
                /* Calibrate CSD widget to 85% */
                (void)capsense_CSDCalibrateWidget(wdgtIndex, capsense_CSD_AUTOTUNE_CAL_LEVEL);
            #endif /* (capsense_DISABLE == capsense_CSD2X_EN) */

            #if (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN)
                if((uint8)(capsense_WD_TOUCHPAD_E | capsense_WD_MATRIX_BUTTON_E) == capsense_dsFlash.wdgtArray[wdgtIndex].wdgtType)
                {
                    /* Get pointer to Modulator IDAC for columns */
                    autoTuneConfig.ptrModIDAC = &ptrWdgt->rowIdacMod[0u];
                    
                    /* Get pointer to Sense Clock Divider for columns */
                    autoTuneConfig.ptrSenseClk = &ptrWdgt->rowSnsClk;

                    /* Find correct sense clock value */
                    cp = SmartSense_TunePrescalers(&autoTuneConfig);

                    if ((capsense_CP_MAX + capsense_CP_ERROR) <= cp)
                    {
                        autoTuneStatus = CYRET_BAD_DATA;
                    }
                    
                    #if (capsense_ENABLE == capsense_CSDV2)
                        /* Make sure that ModClk >= 4 * rowSnsClk for ModClk <= 12 MHz and rowSnsClk <= 6MHz */
                        if (autoTuneConfig.snsClkInputClock <= capsense_MOD_CSD_CLK_24000KHZ)
                        {
                            if (ptrWdgt->rowSnsClk < capsense_FACTOR_FILTER_DELAY_12MHZ)
                            {
                                ptrWdgt->rowSnsClk = capsense_FACTOR_FILTER_DELAY_12MHZ;
                            }
                        }
                        else if (autoTuneConfig.snsClkInputClock <= capsense_MOD_CSD_CLK_48000KHZ)
                        {
                            if (ptrWdgt->rowSnsClk < capsense_FACTOR_MOD_SNS)
                            {
                                ptrWdgt->rowSnsClk = capsense_FACTOR_MOD_SNS;
                            }
                        }
                        else
                        {
                            /* rowSnsClk is valid*/
                        }
                    #endif /* (capsense_ENABLE == capsense_CSDV2) */
                }
            #endif /* (capsense_CSD_MATRIX_WIDGET_EN | capsense_CSD_TOUCHPAD_WIDGET_EN) */

            /* Get pointer to Modulator IDAC  for rows */
            autoTuneConfig.ptrModIDAC = &ptrWdgt->idacMod[0u];
                
            /* Get pointer to Sense Clock Divider for columns */
            autoTuneConfig.ptrSenseClk = &ptrWdgt->snsClk;
               
            /* Find correct sense clock value */
            cp = SmartSense_TunePrescalers(&autoTuneConfig);

            if ((capsense_CP_MAX + capsense_CP_ERROR) <= cp)
            {
                autoTuneStatus = CYRET_BAD_DATA;
            }
            
            #if (capsense_ENABLE == capsense_CSDV2)
                /* Make sure that ModClk >= 4 * SnsClk for ModClk <= 12 MHz and SnsClk <= 6MHz */
                if (autoTuneConfig.snsClkInputClock <= capsense_MOD_CSD_CLK_24000KHZ)
                {
                    if (ptrWdgt->snsClk < capsense_FACTOR_FILTER_DELAY_12MHZ)
                    {
                        ptrWdgt->snsClk = capsense_FACTOR_FILTER_DELAY_12MHZ;
                    }
                }
                else if (autoTuneConfig.snsClkInputClock <= capsense_MOD_CSD_CLK_48000KHZ)
                {
                    if (ptrWdgt->snsClk < capsense_FACTOR_MOD_SNS)
                    {
                        ptrWdgt->snsClk = capsense_FACTOR_MOD_SNS;
                    }
                }
                else
                {
                    /* SnsClk is valid*/
                }
            #endif /* (capsense_ENABLE == capsense_CSDV2) */
            
            /* Set flag to indicate that calibration in PWM mode has been performed */
            capsense_prescalersTuningDone = 1u;

            #if (capsense_DISABLE == capsense_CSD2X_EN)
                /* Calibrate CSD widget to 85% */
                autoTuneStatus |= capsense_CSDCalibrateWidget(wdgtIndex, capsense_CSD_AUTOTUNE_CAL_LEVEL);  
            #endif /* (capsense_DISABLE == capsense_CSD2X_EN) */

            /* Find resolution */
            ptrWdgt->resolution = SmartSense_TuneSensitivity(&autoTuneConfig);
        }
    }
    return autoTuneStatus;
}
#endif /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)) */


#if (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN)
/*******************************************************************************
* Function Name: capsense_ChangeImoFreq
****************************************************************************//**
*
* \brief
*  This function changes the IMO frequency.
*
* \details
*   The IMO frequency can have three offsets: 0%, -5% and +5%. The frequency 
*   trims are contained in the capsense_immunity[value] array for each 
*   frequency channel.
*
* \param value The frequency channel ID.  
*
*******************************************************************************/
    void capsense_ChangeImoFreq(uint32 value)
    {
        CY_SET_REG32(CY_SYS_CLK_IMO_TRIM1_PTR, capsense_immunity[value]);
    }
#endif  /* (capsense_ENABLE == capsense_MULTI_FREQ_SCAN_EN) */
 

/* [] END OF FILE */
