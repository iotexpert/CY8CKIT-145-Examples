/***************************************************************************//**
* \file CapSense_Structure.c
* \version 3.0
*
* \brief
*   This file defines the data structure global variables and provides implementation
*   for the high-level and low-level APIs of the Data Structure module.
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

#include <cytypes.h>
#include "CyLib.h"
#include "CapSense_Structure.h"

#if (0u != CapSense_ADC_EN)
    #include "CapSense_Adc.h"
#endif /* (0u != CapSense_ADC_EN) */

/*******************************************************************************
* Define the RAM Data Structure variables and their init data in flash
*******************************************************************************/
/**
* \if SECTION_GLOBAL_VARIABLES
* \addtogroup group_global_variables
* \{
*/

/**
* Variable that contains CapSense configuration, settings and scanning results.
* CapSense_dsRam represents RAM Data Structure.
*/
CapSense_RAM_STRUCT CapSense_dsRam;
/** \}
* \endif */

/*******************************************************************************
* Declare Widget's De-bounce Counters
*******************************************************************************/
static uint8 CapSense_debounceButton0[CapSense_BUTTON0_NUM_SENSORS];
static uint8 CapSense_debounceButton1[CapSense_BUTTON1_NUM_SENSORS];
static uint8 CapSense_debounceButton2[CapSense_BUTTON2_NUM_SENSORS];


/*******************************************************************************
* Define and initialize the Flash Data Structure
*******************************************************************************/

/**
* \if SECTION_API_CONSTANTS
* \addtogroup group_api_constants
* \{
*/

/**
* Constant for the FLASH Data Structure
*/
const CapSense_FLASH_STRUCT CapSense_dsFlash =
{
    /* Flash Widget Initialization Data */
    {
        { /* Button0 */
            &CapSense_ioList[0u],
            (void *)&CapSense_dsRam.wdgtList.button0,
            CapSense_dsRam.snsList.button0,
            (void *)0u,
            CapSense_debounceButton0,
            CapSense_BUTTON0_STATIC_CONFIG,
            CapSense_BUTTON0_NUM_SENSORS,
            (uint8)CapSense_WD_BUTTON_E,
            CapSense_BUTTON0_NUM_SENSORS,
            1u,
        },
        { /* Button1 */
            &CapSense_ioList[2u],
            (void *)&CapSense_dsRam.wdgtList.button1,
            CapSense_dsRam.snsList.button1,
            (void *)0u,
            CapSense_debounceButton1,
            CapSense_BUTTON1_STATIC_CONFIG,
            CapSense_BUTTON1_NUM_SENSORS,
            (uint8)CapSense_WD_BUTTON_E,
            CapSense_BUTTON1_NUM_SENSORS,
            1u,
        },
        { /* Button2 */
            &CapSense_ioList[4u],
            (void *)&CapSense_dsRam.wdgtList.button2,
            CapSense_dsRam.snsList.button2,
            (void *)0u,
            CapSense_debounceButton2,
            CapSense_BUTTON2_STATIC_CONFIG,
            CapSense_BUTTON2_NUM_SENSORS,
            (uint8)CapSense_WD_BUTTON_E,
            CapSense_BUTTON2_NUM_SENSORS,
            1u,
        },
    },
};


/**
* Array of pointers to the electrode specific register
*/
const CapSense_FLASH_IO_STRUCT CapSense_ioList[CapSense_TOTAL_ELECTRODES] =
{
    { /* 0: Button0_Rx0 */
        (reg32 *)CapSense_Rx__0__HSIOM,
        (reg32 *)CapSense_Rx__0__PC,
        (reg32 *)CapSense_Rx__0__DR,
        (reg32 *)CapSense_Rx__0__PS,
        CapSense_Rx__0__HSIOM_MASK,
        CapSense_Rx__0__MASK,
        CapSense_Rx__0__HSIOM_SHIFT,
        (uint8)CapSense_Rx__0__SHIFT,
        (uint8)CapSense_Rx__0__SHIFT * 3u,
    },
    { /* 1: Button0_Tx */
        (reg32 *)CapSense_Tx__0__HSIOM,
        (reg32 *)CapSense_Tx__0__PC,
        (reg32 *)CapSense_Tx__0__DR,
        (reg32 *)CapSense_Tx__0__PS,
        CapSense_Tx__0__HSIOM_MASK,
        CapSense_Tx__0__MASK,
        CapSense_Tx__0__HSIOM_SHIFT,
        (uint8)CapSense_Tx__0__SHIFT,
        (uint8)CapSense_Tx__0__SHIFT * 3u,
    },
    { /* 2: Button1_Rx0 */
        (reg32 *)CapSense_Rx__1__HSIOM,
        (reg32 *)CapSense_Rx__1__PC,
        (reg32 *)CapSense_Rx__1__DR,
        (reg32 *)CapSense_Rx__1__PS,
        CapSense_Rx__1__HSIOM_MASK,
        CapSense_Rx__1__MASK,
        CapSense_Rx__1__HSIOM_SHIFT,
        (uint8)CapSense_Rx__1__SHIFT,
        (uint8)CapSense_Rx__1__SHIFT * 3u,
    },
    { /* 3: Button0_Tx ganged to Button1_Tx */
        (reg32 *)CapSense_Tx__0__HSIOM,
        (reg32 *)CapSense_Tx__0__PC,
        (reg32 *)CapSense_Tx__0__DR,
        (reg32 *)CapSense_Tx__0__PS,
        CapSense_Tx__0__HSIOM_MASK,
        CapSense_Tx__0__MASK,
        CapSense_Tx__0__HSIOM_SHIFT,
        (uint8)CapSense_Tx__0__SHIFT,
        (uint8)CapSense_Tx__0__SHIFT * 3u,
    },
    { /* 4: Button2_Rx0 */
        (reg32 *)CapSense_Rx__2__HSIOM,
        (reg32 *)CapSense_Rx__2__PC,
        (reg32 *)CapSense_Rx__2__DR,
        (reg32 *)CapSense_Rx__2__PS,
        CapSense_Rx__2__HSIOM_MASK,
        CapSense_Rx__2__MASK,
        CapSense_Rx__2__HSIOM_SHIFT,
        (uint8)CapSense_Rx__2__SHIFT,
        (uint8)CapSense_Rx__2__SHIFT * 3u,
    },
    { /* 5: Button0_Tx ganged to Button2_Tx */
        (reg32 *)CapSense_Tx__0__HSIOM,
        (reg32 *)CapSense_Tx__0__PC,
        (reg32 *)CapSense_Tx__0__DR,
        (reg32 *)CapSense_Tx__0__PS,
        CapSense_Tx__0__HSIOM_MASK,
        CapSense_Tx__0__MASK,
        CapSense_Tx__0__HSIOM_SHIFT,
        (uint8)CapSense_Tx__0__SHIFT,
        (uint8)CapSense_Tx__0__SHIFT * 3u,
    },
};


/** \}
* \endif */

/* Initialization data for RAM widget list */
static const CapSense_RAM_WD_LIST_STRUCT CapSense_ramWidgetInit =
{
    { /* Button0 */
        CapSense_BUTTON0_RESOLUTION,
        CapSense_BUTTON0_FINGER_TH,
        CapSense_BUTTON0_NOISE_TH,
        CapSense_BUTTON0_NNOISE_TH,
        CapSense_BUTTON0_HYSTERESIS,
        CapSense_BUTTON0_ON_DEBOUNCE,
        CapSense_BUTTON0_LOW_BSLN_RST,
        CapSense_BUTTON0_BSLN_COEFF,
        CapSense_BUTTON0_SNS_CLK,
    },
    { /* Button1 */
        CapSense_BUTTON1_RESOLUTION,
        CapSense_BUTTON1_FINGER_TH,
        CapSense_BUTTON1_NOISE_TH,
        CapSense_BUTTON1_NNOISE_TH,
        CapSense_BUTTON1_HYSTERESIS,
        CapSense_BUTTON1_ON_DEBOUNCE,
        CapSense_BUTTON1_LOW_BSLN_RST,
        CapSense_BUTTON1_BSLN_COEFF,
        CapSense_BUTTON1_SNS_CLK,
    },
    { /* Button2 */
        CapSense_BUTTON2_RESOLUTION,
        CapSense_BUTTON2_FINGER_TH,
        CapSense_BUTTON2_NOISE_TH,
        CapSense_BUTTON2_NNOISE_TH,
        CapSense_BUTTON2_HYSTERESIS,
        CapSense_BUTTON2_ON_DEBOUNCE,
        CapSense_BUTTON2_LOW_BSLN_RST,
        CapSense_BUTTON2_BSLN_COEFF,
        CapSense_BUTTON2_SNS_CLK,
    },
};


/* IDAC Initialization Data */
static const uint8 CapSense_ramIdacInit[CapSense_TOTAL_SENSORS] =
{
    /* Button0 */
    CapSense_BUTTON0_RX0_IDAC_COMP0,

    /* Button1 */
    CapSense_BUTTON1_RX0_IDAC_COMP0,

    /* Button2 */
    CapSense_BUTTON2_RX0_IDAC_COMP0,
};


/*******************************************************************************
* Define internal data types and statements
*******************************************************************************/
#define PARAM_TYPE_OFFSET       (30u)
#define PARAM_TYPE_MASK         (3Lu << PARAM_TYPE_OFFSET)
#define PARAM_RWBIT_OFFSET      (29u)
#define PARAM_RWBIT_MASK        (1Lu << PARAM_RWBIT_OFFSET)
#define PARAM_FBIT_OFFSET       (28u)
#define PARAM_FBIT_MASK         (1Lu << PARAM_FBIT_OFFSET)
#define PARAM_UBIT_OFFSET       (23u)
#define PARAM_UBIT_MASK         (1Lu << PARAM_UBIT_OFFSET)
#define PARAM_WDID_OFFSET       (16u)
#define PARAM_WDID_MASK         (0x7FLu << PARAM_WDID_OFFSET)
#define PARAM_CRC_OFFSET        (24u)
#define PARAM_CRC_MASK          (0x0FLu << PARAM_CRC_OFFSET)

/* Define align masks for uint16 and uint32 */
#define UINT16_ALIGN_MASK       (1Lu)
#define UINT32_ALIGN_MASK       (3Lu)

/* Define supported parameter types */
#define DS_PARAM_TYPE_UINT8     (0x01u)
#define DS_PARAM_TYPE_UINT16    (0x02u)
#define DS_PARAM_TYPE_UINT32    (0x03u)


/* Define PARAM_ID structure */
typedef struct
{
    uint16 offset;      /* parameter offset                     */
    uint8  widgetId;    /* widget Id parameter belongs to       */
    uint8  affectsCrc;  /* parameter affects widget CRC         */
    uint8  crc;         /* parameter CRC                        */
    uint8  flash;       /* parameter located in FLASH flag      */
    uint8  rw;          /* parameter is read/write flag         */
    uint8  type;        /* parameter type: uint8/uint16/uint32  */
} PARAM_ID_STRUCT;


/*******************************************************************************
* Static Function Prototypes
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

static cystatus DsParseParamId(uint32 paramId, PARAM_ID_STRUCT *pData);

/** \}
* \endif */


/*******************************************************************************
* Function Name: CapSense_DsInitialize
****************************************************************************//**
*
* \brief
*   This function initializes the Data Structure storage.
*
* \details
*   This function copies the default widget configuration parameters 
*   from the Flash storage into the RAM Data Structure.
*
*******************************************************************************/
void CapSense_DsInitialize(void)
{
    #if (0u != CapSense_CSX_EN) || (0 != CapSense_CSD_IDAC_COMP_EN)
        uint32 snsId;
        uint32 wdgtId;
    
        CapSense_RAM_SNS_STRUCT * ptrSns;
        CapSense_FLASH_WD_STRUCT const * ptrFlashWdgt;
        uint8 const * ptrIdacInit = CapSense_ramIdacInit;
    #endif        
    
    /* Reset RAM data structure content */
    (void)memset(&CapSense_dsRam, 0, sizeof(CapSense_dsRam));
    
    /* Initialize configId and deviceId registers */
    CapSense_dsRam.configId = CapSense_CONFIG_ID;
    CapSense_dsRam.deviceId = CapSense_DEVICE_ID;
    
    /* Initialize global RAM data */
    CapSense_dsRam.csd0Config = CapSense_CSD0_CONFIG;

    #if ((0u != CapSense_CSD2X_EN) || (0u != CapSense_CSX2X_EN))
        CapSense_dsRam.csd1Config = CapSense_CSD1_CONFIG;
    #endif /* #if ((0u != CapSense_CSD2X_EN) || (0u != CapSense_CSX2X_EN)) */
    
    #if (0u != CapSense_CSD_EN)
        CapSense_dsRam.modCsdClk = CapSense_CSD_SCANSPEED_DIVIDER;
        
        #if (0u != CapSense_CSD_COMMON_SNS_CLK_EN)
            CapSense_dsRam.snsCsdClk = CapSense_CSD_SNS_CLK_DIVIDER;
        #endif /* #if (0u != CapSense_CSD_COMMON_SNS_CLK_EN) */
    #endif /* #if (0u != CapSense_CSD_EN) */
    
    #if (0u != CapSense_CSX_EN)
        CapSense_dsRam.modCsxClk = CapSense_CSX_SCANSPEED_DIVIDER;
        
        #if (0u != CapSense_CSX_COMMON_TX_CLK_EN)
            CapSense_dsRam.snsCsxClk = CapSense_CSX_TX_CLK_DIVIDER;
        #endif /* #if (0u != CapSense_CSX_COMMON_TX_CLK_EN) */
    #endif /* #if (0u != CapSense_CSX_EN) */
    
    #if (0u != CapSense_SELF_TEST_EN)
        CapSense_dsRam.glbCrc = CapSense_GLB_CRC;
    #endif /* #if (0u != CapSense_SELF_TEST_EN) */
   
    /* Initialize RAM widget data */
    CapSense_dsRam.wdgtList = CapSense_ramWidgetInit;
    
    #if (0u != CapSense_CSX_EN) || (0 != CapSense_CSD_IDAC_COMP_EN)
        /* Initialize IDAC data */
        ptrFlashWdgt = CapSense_dsFlash.wdgtArray;
        
        for (wdgtId = CapSense_TOTAL_WIDGETS; wdgtId-- > 0u; )
        {
            ptrSns = ptrFlashWdgt->ptr2SnsRam;
                
            for (snsId = CapSense_GET_SNS_CNT_BY_PTR(ptrFlashWdgt); snsId-- > 0u;)
            {
                ptrSns->idacComp[0u] = *ptrIdacInit;
                ptrIdacInit++;
                
                #if (0u != CapSense_MULTI_FREQ_SCAN_EN)
                    ptrSns->idacComp[1u] = *ptrIdacInit;
                    ptrIdacInit++;
                    
                    ptrSns->idacComp[2u] = *ptrIdacInit;
                    ptrIdacInit++;
                #endif /* #if (0u != CapSense_MULTI_FREQ_SCAN_EN) */
                
                ptrSns++;
            }
            
            ptrFlashWdgt++; /* Move to next widget */
        }
    #endif /*(0u != CapSense_CSX_EN) || (0 != CapSense_CSD_IDAC_COMP_EN)*/
    
    #if (0u != CapSense_ADC_EN)
        CapSense_AdcDsInitialize();
    #endif /* (0u != CapSense_ADC_EN) */
}


#if (0u != CapSense_ADC_EN)
    /*******************************************************************************
    * Function Name: CapSense_AdcDsInitialize
    ****************************************************************************//**
    *
    * \brief
    *   Configures the initial Adc datastructure members.
    *
    *******************************************************************************/
    void CapSense_AdcDsInitialize(void)
    {
        CapSense_dsRam.adcResolution = CapSense_ADC_RESOLUTION;
        CapSense_dsRam.adcIdac = CapSense_ADC_IDACVAL;
    }
#endif /* (0u != CapSense_ADC_EN) */


/*******************************************************************************
* Function Name: DsParseParamId
****************************************************************************//**
*
* \brief
*   This function parses the parameter ID and checks its correctness.
*
* \details
*   This function checks the parameter CRC if the self-test is enabled, 
*   validates the offset value and parameter type.
*
* \param paramId The parameter to parse.
* \param pData   The pointer to the structure that will hold parsed data.
*
* \return CYRET_SUCCESS     If the parameter is valid
*         CYRET_BAD_PARAM   If  the parameter is invalid
*
*******************************************************************************/
static cystatus DsParseParamId(uint32 paramId, PARAM_ID_STRUCT *pData)
{
    cystatus result = CYRET_SUCCESS;
    
    #if (0u != CapSense_SELF_TEST_EN)
        const uint8 crcTable[] =
        {
            0x00u, 0x09u, 0x0Bu, 0x02u, 0x0Fu, 0x06u, 0x04u, 0x0Du,
            0x07u, 0x0Eu, 0x0Cu, 0x05u, 0x08u, 0x01u, 0x03u, 0x0Au
        };
        
        uint32 i;
        uint32 actualCrc = 0u;
        uint32 crcIndex;
    #endif /* #if (0u != CapSense_SELF_TEST_EN) */
            
    /* Extract parameter data */
    pData->offset     = LO16(paramId);
    pData->affectsCrc = LO8((paramId & PARAM_UBIT_MASK)  >> PARAM_UBIT_OFFSET);
    pData->widgetId   = LO8((paramId & PARAM_WDID_MASK)  >> PARAM_WDID_OFFSET);
    pData->type       = LO8((paramId & PARAM_TYPE_MASK)  >> PARAM_TYPE_OFFSET);
    pData->flash      = LO8((paramId & PARAM_FBIT_MASK)  >> PARAM_FBIT_OFFSET);
    pData->rw         = LO8((paramId & PARAM_RWBIT_MASK) >> PARAM_RWBIT_OFFSET);
    pData->crc        = LO8((paramId & PARAM_CRC_MASK)   >> PARAM_CRC_OFFSET);
    
    /* Check parameter CRC if self-test is enabled */
    #if (0u != CapSense_SELF_TEST_EN)
 
        /* Calculate CRC for bits 0..24.
         * The CRC is calculated using nibbles (4-bits). 
         * So for 0..24 bits there are in total 6 nibbles. */
        for (i = 6u; i > 0u; i--)
        {
            crcIndex = actualCrc ^ (paramId & 0x0Fu);
            actualCrc = crcTable[crcIndex];
            paramId >>= 4u;
        }

        /* Add bits 28..32 */
        crcIndex = actualCrc ^ ((paramId >> 4u) & 0x0Fu);
        actualCrc = crcTable[crcIndex];

        /* Add bits 24..28 (CRC) */
        crcIndex = actualCrc ^ (paramId & 0x0Fu);
        actualCrc = crcTable[crcIndex];
        
        if (0u != actualCrc)
        {
            /* CRC mismatch */
            result = CYRET_BAD_PARAM;
        }
        
        if ((CYRET_SUCCESS == result) && 
            (0u != pData->affectsCrc) && (pData->widgetId >= CapSense_TOTAL_WIDGETS ))
        {
            /* Wrong widgetId for protected parameter */
            result = CYRET_BAD_PARAM;
        }
        
        /* Check offset value range */
        if (CYRET_SUCCESS == result)
        {
            if (0u == pData->flash)
            {
                /* Check offset for RAM Data Structure range */
                if (pData->offset >= sizeof(CapSense_dsRam))
                {
                    result = CYRET_BAD_PARAM;
                }
            }
            else
            {
                /* Check offset for Flash Data Structure range */
                if (pData->offset >= sizeof(CapSense_dsFlash))
                {
                    result = CYRET_BAD_PARAM;
                }
                
                /* Check if it is marked as Read Only */
                if (0u != pData->rw)
                {
                    result = CYRET_BAD_PARAM;
                }
            }
        }
        
        if (CYRET_SUCCESS == result)
        {
            /* Check parameter type, offset alignment */
            switch (pData->type)
            {
            case DS_PARAM_TYPE_UINT16:
                if (0u != (pData->offset & UINT16_ALIGN_MASK))
                {
                    result = CYRET_BAD_PARAM;
                }
                break;
                
            case DS_PARAM_TYPE_UINT32:
                if (0u != (pData->offset & UINT32_ALIGN_MASK))
                {
                    result = CYRET_BAD_PARAM;
                }
                break;
                
            case DS_PARAM_TYPE_UINT8:
                break;
                
            default:
                result = CYRET_BAD_PARAM;
                break;
            }
        }
    #else
        /* Check offset value range */
        if (0u == pData->flash)
        {
            /* Check offset for RAM Data Structure range */
            if (pData->offset >= sizeof(CapSense_dsRam))
            {
                result = CYRET_BAD_PARAM;
            }
        }
        else
        {
            /* Check offset for Flash Data Structure range */
            if (pData->offset >= sizeof(CapSense_dsFlash))
            {
                result = CYRET_BAD_PARAM;
            }
            
            /* Check if it is marked as Read Only */
            if (0u != pData->rw)
            {
                result = CYRET_BAD_PARAM;
            }
        }
    
        if (CYRET_SUCCESS == result)
        {
            /* Check parameter type, offset alignment */
            switch (pData->type)
            {
            case DS_PARAM_TYPE_UINT16:
                if (0u != (pData->offset & UINT16_ALIGN_MASK))
                {
                    result = CYRET_BAD_PARAM;
                }
                break;
                
            case DS_PARAM_TYPE_UINT32:
                if (0u != (pData->offset & UINT32_ALIGN_MASK))
                {
                    result = CYRET_BAD_PARAM;
                }
                break;
                
            case DS_PARAM_TYPE_UINT8:
                break;
                
            default:
                result = CYRET_BAD_PARAM;
                break;
            }
        }  
    #endif /* #if (0u != CapSense_SELF_TEST_EN) */

    return result;
}

/*******************************************************************************
* Function Name: CapSense_GetParam
****************************************************************************//**
*
* \brief
*   Gets the specified parameter value from the data structure.
*
* \details
*   The paramId for each register is available in CapSense_RegisterMap header file.
*   The paramId is a special enumerated value generated by the customizer.  
*   The format of paramId is as follows:       
*   -   [  byte 3   byte 2   byte 1   byte 0 ]  
*   -   [ TTWFCCCC UIIIIIII MMMMMMMM LLLLLLLL ] 
*   -   T - encodes the parameter type:
*          - 01b - uint8
*          - 10b - uint16
*          - 11b uint32    
*   -   W - indicates whether the parameter is writeable:
*          - 0 - ReadOnly
*          - 1 - Read/Write
*   -   C - 4 bit CRC (X^3 + 1) of the whole paramId word,
*          the C bits are filled with 0s when the CRC is calculated.
*   -   U - indicates if the parameter affects the RAM Widget Object CRC.
*   -   I - specifies that the widgetID parameter belongs to
*   - M,L - the parameter offset MSB and LSB accordingly in:  
*             - Flash Data Structure if W bit is 0.
*             - RAM Data Structure if W bit is 1.
*
* \param  paramId The identifier of the Data Structure parameter.
* \param  value   The pointer to a variable that should be updated with the value.
*
* \return
*   Returns the status of operation:
*     - CYRET_SUCCESS    The operation is successful
*     - CYRET_BAD_PARAM  The parameter ID is wrong.
*
*******************************************************************************/
cystatus CapSense_GetParam(uint32 paramId, uint32 *value)
{
    PARAM_ID_STRUCT pData;
    cystatus result;
    
    union
    {
        volatile void   const * ptr;
        volatile uint8  const * ptrUint8;
        volatile uint16 const * ptrUint16;
        volatile uint32 const * ptrUint32;
    } ptrData;
    
    result = DsParseParamId(paramId, &pData);
    
    /* Parse and validate paramId */
    if (CYRET_SUCCESS == result)
    {
        /* Get base address of parameter */
        if (0u == pData.flash)
        {
            ptrData.ptr = &CapSense_dsRam;
        }
        else
        {
            ptrData.ptr = &CapSense_dsFlash;
        }
        
        /* Add offset to base address */
        ptrData.ptrUint8 += pData.offset;
        
        /* Read data */
        if (DS_PARAM_TYPE_UINT8 == pData.type)
        {
            *value = (uint32) (*ptrData.ptrUint8);
        }
        else if (DS_PARAM_TYPE_UINT16 == pData.type)
        {
            *value = (uint32) (*ptrData.ptrUint16);
        }
        else if (DS_PARAM_TYPE_UINT32 == pData.type)
        {
            *value = *ptrData.ptrUint32;
        }
        else
        {
            /* Parameter comes here already validated. */
            CYASSERT(0u);
        }
    }
    
    return result;
}


/*******************************************************************************
* Function Name: CapSense_SetParam
****************************************************************************//**
*
* \brief
*   Sets the new value for the specified parameter in the data structure.
*
* \details
*   The paramId for each register is available in CapSense_RegisterMap header file.
*   The paramId is a special enumerated value generated by the customizer.
*   The format of paramId is as follows:       
*    -  [  byte 3   byte 2   byte 1   byte 0  ]
*    -  [ TTWFCCCC UIIIIIII MMMMMMMM LLLLLLLL ]
*    -  T - encodes the parameter type:
*           - 01b - uint8
*           - 10b - uint16
*           - 11b - uint32
*    -  W - indicates whether the parameter is writeable:
*           - 0 - ReadOnly
*           - 1 - Read/Write              
*    -   C - 4 bit CRC (X^3 + 1) of the whole paramId word,
*            the C bits are filled with 0s when the CRC is calculated.
*    -   U - indicates if parameter affects RAM Widget Object CRC.
*    -   I - specifies the widgetID parameter belongs to
*    - M,L - the parameter offset MSB and LSB accordingly in:
*             - Flash Data Structure if W bit is 0.
*             - RAM Data Structure if W bit is 1.
*
* \param paramId  The identifier of the Data Structure parameter.
* \param value    The new parameter's value.
*
* \return
*   Returns the status of operation:
*       - CYRET_SUCCESS    The operation is successful.
*       - CYRET_BAD_PARAM  The parameter ID is wrong.
*
*******************************************************************************/
cystatus CapSense_SetParam(uint32 paramId, uint32 value)
{
    cystatus result;
    PARAM_ID_STRUCT pData;
    
    union
    {
        volatile void   * ptr;
        volatile uint8  * ptrUint8;
        volatile uint16 * ptrUint16;
        volatile uint32 * ptrUint32;
    } ptrData;
    
    result = DsParseParamId(paramId, &pData);
    
    /* verify if parameter is writable */
    if (CYRET_SUCCESS == result)
    {
        if (0u == pData.rw)
        {
            result = CYRET_BAD_PARAM;
        }
    }
    
    /* Parse and validate paramId */
    if (CYRET_SUCCESS == result)
    {
        /* Get base address of parameter */
        ptrData.ptr = &CapSense_dsRam;
        
        /* Add offset to base address */
        ptrData.ptrUint8 += pData.offset;
        
        /* Write data */
        if (DS_PARAM_TYPE_UINT8 == pData.type)
        {
            *ptrData.ptrUint8 = LO8(value);
        }
        else if (DS_PARAM_TYPE_UINT16 == pData.type)
        {
            *ptrData.ptrUint16 = LO16(value);
        }
        else if (DS_PARAM_TYPE_UINT32 == pData.type)
        {
            *ptrData.ptrUint32 = value;
        }
        else
        {
            /* Parameter comes here already validated. */
            CYASSERT(0u);
        }
        
        /* Update widget CRC if self-test is enabled and parameter affects it */
        #if (0u != CapSense_SELF_TEST_EN)
            if (0u != pData.affectsCrc)
            {
                CapSense_DsUpdateWidgetCrc((uint32)pData.widgetId);
            }
            else
            {
                if ((ptrData.ptrUint16 >= &CapSense_dsRam.csd0Config) &&
                    (ptrData.ptrUint16 <  &CapSense_dsRam.glbCrc))
                {
                    /* Update global data CRC */
                    CapSense_dsRam.glbCrc = 
                        CapSense_DsCalculateCrc16(
                            (void *)&CapSense_dsRam.csd0Config,
                            (uint32)&CapSense_dsRam.glbCrc - (uint32)&CapSense_dsRam.csd0Config);
                }
            }
        #endif /* #if (0u != CapSense_SELF_TEST_EN) */
    }
    
    return result;
}

#if (0u != CapSense_SELF_TEST_EN)
/*******************************************************************************
* Function Name: DsCalculateCrc16
****************************************************************************//**
*
* \brief
*   Calculates CRC for the specified buffer and length. CRC Poly: 0xAC9A
*
* \details 
*  This API is used for the CRC protection of the RAM Widget Object.
*  The maximum size of the RAM Widget Object is 192 bits. So, the best suitable 
*  CRC polynomial is 0xAC9A. It has a Hamming distance 5 for data words up to 241 bits.
*    
*  Reference:  "P. Koopman, T. Chakravarthy, 
*  "Cyclic Redundancy Code (CRC) Polynomial Selection for Embedded Networks",
*  The International Conference on Dependable Systems and Networks, DSN-2004"
*
* \param ptrData The pointer to the data.
* \param len     The length of the data in bytes.
*
* \return A calculated CRC-16 value.
*
*******************************************************************************/
uint16 CapSense_DsCalculateCrc16(uint8 *ptrData, uint32 len)
{
    uint32 idx;
    uint32 actualCrc = 0Lu;
    const uint16 crcTable[] =
    {
        0x0000u, 0xAC9Au, 0xF5AEu, 0x5934u, 0x47C6u, 0xEB5Cu, 0xB268u, 0x1EF2u,
        0x8F8Cu, 0x2316u, 0x7A22u, 0xD6B8u, 0xC84Au, 0x64D0u, 0x3DE4u, 0x917Eu
    };
    
    for (;len-- > 0u;)
    {
        /* Process HI Nibble */
        idx = ((actualCrc >> 12u) ^ (((uint32)*ptrData) >> 4u)) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);

        /* Process LO Nibble */
        idx = ((actualCrc >> 12u) ^ (uint32)*ptrData) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);
        
        ptrData++;
    }
        
    return (uint16)actualCrc;
}

/*******************************************************************************
* Function Name: CapSense_DsUpdateWidgetCrc
****************************************************************************//**
*
* \brief
*   Recalculates the CRC for the specified widgetId and 
*   writes it to the Data Structure.
*
* \details 
*  The maximum size of the RAM Widget Object is 192 bits. So, the best suitable 
*  CRC polynomial is 0xAC9A that has hamming distance 5 for the data up to 241 bits. 
*    
*  Reference:  "P. Koopman, T. Chakravarthy, 
*  "Cyclic Redundancy Code (CRC) Polynomial Selection for Embedded Networks", 
*  The International Conference on Dependable Systems and Networks, DSN-2004"
*
* \param widgetId  The widget index number.
*
*******************************************************************************/
void CapSense_DsUpdateWidgetCrc(uint32 widgetId)
{
    uint32 len;
    uint8 *ptrData;
    CapSense_RAM_WD_BASE_STRUCT *ptrWd;
    
    if (widgetId < CapSense_TOTAL_WIDGETS)
    {
        /* Get pointer to RAM object data */
        ptrWd = CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        ptrData = CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        len = (uint32)CapSense_dsFlash.wdgtArray[widgetId].ramObjectSize - 2u;
        
        /* Skip CRC in RAM object structure for CRC calculation */
        ptrData += 2u;
      
        ptrWd->crc = CapSense_DsCalculateCrc16(ptrData, len);
    }
}
#endif /* #if (0u != CapSense_SELF_TEST_EN) */

/*******************************************************************************
* Function Name: CapSense_IsAnyWidgetActive
****************************************************************************//**
*
* \brief
*   Reports if any widget has detected a touch.
*
* \details
*   The information is extracted from the wdgtStatus registers.
*
* \return 
*   Returns the status of all widgets:
*   - Non-Zero - Indicates one or more widgets are active.
*   - Zero     - Indicates no touch is detected.
*
*******************************************************************************/
uint32 CapSense_IsAnyWidgetActive(void)
{
    uint32 result = 0Lu;
    uint32 wdWord;
    
    for (wdWord = CapSense_WDGT_STATUS_WORDS; wdWord-- > 0u;)
    {
        result |= CapSense_dsRam.wdgtStatus[wdWord];
    }
    
    return result;
}


/*******************************************************************************
* Function Name: CapSense_IsWidgetActive
****************************************************************************//**
*
* \brief
*   Reports if the specified widget detects a touch on any of its sensors.
*
* \details
*   Reports if the specified widget detects a touch on any of its sensors.
*   The information is extracted from the wdgtStatus register.
*
* \param widgetId  The widget index number.
*
* \return 
*   Returns the status of specified widget:
*   - Non-Zero - Indicates a touch is detected.
*   - Zero     - Indicates no touch is detected.
*
*******************************************************************************/
uint32 CapSense_IsWidgetActive(uint32 widgetId)
{
    uint32 result = 0Lu;
   
    if (widgetId < CapSense_TOTAL_WIDGETS)
    {
        result = CapSense_GET_WIDGET_ACTIVE_STATUS(widgetId);
    }
    return result;
}


/*******************************************************************************
* Function Name: CapSense_IsSensorActive
****************************************************************************//**
*
* \brief
*   Reports if the specified sensor in the widget detects a touch.
*
* \details 
*   Reports if the specified sensor in the widget detects a touch.
*   The information is extracted from the snsStatus register.
*
* \param widgetId  The widget index number.
* \param sensorId  The sensor index number within the widget
*
* \return 
*   Returns the status of sensors in the specified widget/sensor as below:
*   - Non-Zero - Indicates a touch is detected.
*   - Zero     - Indicates no touch is detected or a wrong widgetId/sensorId is specified.
*
*******************************************************************************/
uint32 CapSense_IsSensorActive(uint32 widgetId, uint32 sensorId)
{
    uint32 result = 0Lu;
    
    if ((widgetId < CapSense_TOTAL_WIDGETS) &&
        (sensorId < CapSense_GET_SENSOR_COUNT(widgetId)))
    {
        result = CapSense_dsRam.snsStatus[widgetId] & (1Lu << sensorId);
    }
    
    return result;
}

#if (0u != CapSense_MATRIX_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_IsMatrixButtonsActive
****************************************************************************//**
*
* \brief
*   Reports the status of the selected Matrix Buttons widget.
*
* \details 
*   Reports the status of the specified Matrix Buttons widget.
*
* \param widgetId Specify the ID number of the matrix button widget to check status 
*            of its sensors. A macro for Widget ID can be found in 
*            CapSense_RegisterMap header file defined as CapSense_<WidgetName>_ID
*
* \return 
*   Returns the status of sensors in the specified widget as below:
*       - For CSD Matrix Buttons:
*           - Bit [31] - One or more sensors in the widget detected a touch
*           - Bits [30..24] - Reserved
*           - Bits [23..16] - Indicates logic sensor number of sensor that detected a touch. 
*                             If more than one sensor detected a touch for CSX widget, 
*                             the sensor with lowest logical number is reported and in case 
*                             of CSD widget, no status is reported as more than one touch is 
*                             invalid for CSD matrix button widget.
*           - Bits [15..8]  - Indicates status of row sensors. Each bit indicates ON/OFF 
*                             status of each row sensor.
*           - Bits [7..0]   - Indicates status of column sensors. Each bit indicates ON/OFF 
*                             status of each column sensor.
*       - For CSX Matrix Buttons: each bit corresponds to the TX/RX intersection.
*
*******************************************************************************/
uint32 CapSense_IsMatrixButtonsActive(uint32 widgetId)
{
    uint32 result = 0Lu;
    cystatus state = CYRET_SUCCESS;
    CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt = 0u;
    
    #if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
        CapSense_RAM_WD_CSD_MATRIX_STRUCT *wdCsdMatrix;
    #endif
  
    if (widgetId >= CapSense_TOTAL_WIDGETS)
    {
        state = CYRET_BAD_PARAM;
    }
    else
    {
        ptrFlashWdgt = &CapSense_dsFlash.wdgtArray[widgetId];
        
        if ((CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType != CapSense_WD_MATRIX_BUTTON_E)
        {
            state = CYRET_BAD_PARAM;
        }
        else if (0u == CapSense_GET_WIDGET_ACTIVE_STATUS(widgetId))
        {
            state = CYRET_BAD_PARAM;
        }
        else
        {
            /* input parameters are OK */
        }
    }
       
    if (CYRET_SUCCESS == state)
    {
        switch(CapSense_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != CapSense_CSD_MATRIX_WIDGET_EN)
            case CapSense_SENSE_METHOD_CSD_E:
                wdCsdMatrix = ptrFlashWdgt->ptr2WdgtRam;
                
                result = CapSense_MATRIX_BUTTONS_TOUCHED | 
                         ((uint32)wdCsdMatrix->posSnsId << 16u)  |
                         ((uint32)wdCsdMatrix->posRow   << 8u)   |
                         (uint32)wdCsdMatrix->posCol;
                break;
        #endif /* #if (0u != CapSense_CSD_MATRIX_WIDGET_EN) */
        
        #if (0u != CapSense_CSX_MATRIX_WIDGET_EN)
            case CapSense_SENSE_METHOD_CSX_E:
                result = CapSense_dsRam.snsStatus[widgetId];
                break;
        #endif /* #if (0u != CapSense_CSX_MATRIX_WIDGET_EN) */
        
        default:
            CYASSERT(0u);
            break;
        }
    }
    
    return result;
}
#endif /* #if (0u != CapSense_MATRIX_WIDGET_EN) */

#if (0u != CapSense_PROXIMITY_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_IsProximityTouchActive
****************************************************************************//**
*
* \brief
*   Reports the touch status of the selected Proximity widget.
*
* \details 
*   The information is extracted from the snsStatus register.
*
* \param widgetId  The widget index number.
* \param proxId    The proximity sensor number.
*
* \return 
*  Returns the status in the specified widget:
*   - 0x01Lu  - Indicates a finger proximity is detected.
*   - 0x03Lu  - Indicates a finger touch is detected.
*   - Zero    - Indicates no touch is detected or the specified widget is not proximity.
*
*******************************************************************************/
uint32 CapSense_IsProximityTouchActive(uint32 widgetId, uint32 proxId)
{
    uint32 result = 0Lu;
    
    if ((widgetId < CapSense_TOTAL_WIDGETS) && (proxId < CapSense_GET_SENSOR_COUNT(widgetId)) &&
        (CapSense_WD_PROXIMITY_E == 
            (CapSense_WD_TYPE_ENUM)CapSense_dsFlash.wdgtArray[widgetId].wdgtType))
    {
        result = CapSense_dsRam.snsStatus[widgetId];
        result >>= CapSense_PROX_STS_OFFSET(proxId);
        result &=  CapSense_PROX_STS_MASK;
    }
    
    return result;
}
#endif /* #if (0u != CapSense_PROXIMITY_WIDGET_EN) */


#if (0u != CapSense_SLIDER_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_GetCentroidPos
****************************************************************************//**
*
* \brief
*   Reports the centroid position for the sliders.
*
* \details 
*   Reports the centroid position for the sliders.
*   The information is extracted from the Data Structure registers.
*
* \param widgetId The widget index number.
*
* \return 
*   The slider position or CapSense_SLIDER_NO_TOUCH in the case of no touch 
*   or wrong widget index number.
*
*******************************************************************************/
uint32 CapSense_GetCentroidPos(uint32 widgetId)
{
    uint32 result = CapSense_SLIDER_NO_TOUCH;
    CapSense_RAM_WD_SLIDER_STRUCT *wdSlider;
    
    if ((widgetId < CapSense_TOTAL_WIDGETS) && 
        ((CapSense_WD_LINEAR_SLIDER_E == 
            (CapSense_WD_TYPE_ENUM)CapSense_dsFlash.wdgtArray[widgetId].wdgtType) ||
         (CapSense_WD_RADIAL_SLIDER_E == 
            (CapSense_WD_TYPE_ENUM)CapSense_dsFlash.wdgtArray[widgetId].wdgtType)))
    {
        wdSlider = CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        result = (uint32)wdSlider->position[0u];
    }
    
    return result;
}
#endif /* #if (0u != CapSense_SLIDER_WIDGET_EN) */


#if (0u != CapSense_TOUCHPAD_WIDGET_EN)
/*******************************************************************************
* Function Name: CapSense_GetXYCoordinates
****************************************************************************//**
*
* \brief
*   This function reports the X/Y position detected for the selected Touchpad ID.
*
* \details 
*   This function reports the X/Y position detected for the selected Touchpad ID.
*   The information is extracted from the Data Structure registers.
*
*   In the case of the CSX Touchpad with multi-touch support, only
*   the first finger position will be reported.
*
* \param widgetId The widget index number.
*
* \return 
*   Returns X and Y coordinates of touch if present:
*   - LSB is the X Coordinate; MSB is the Y Coordinate.
*   - CapSense_TOUCHPAD_NO_TOUCH in the case of no touch or wrong widgetId.
*
*******************************************************************************/
uint32 CapSense_GetXYCoordinates(uint32 widgetId)
{
    CapSense_FLASH_WD_STRUCT const *ptrFlashWdgt = NULL;
    
    #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
        CapSense_RAM_WD_CSD_TOUCHPAD_STRUCT *wdCsdTouchpad;
    #endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */
    
    #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
        CapSense_RAM_WD_CSX_TOUCHPAD_STRUCT *wdCsxTouchpad;
    #endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */
    
    cystatus state = CYRET_SUCCESS;
    uint32 result = CapSense_TOUCHPAD_NO_TOUCH;

    if (widgetId >= CapSense_TOTAL_WIDGETS)
    {
        state = CYRET_BAD_PARAM;
    }
    else
    {
        ptrFlashWdgt = &CapSense_dsFlash.wdgtArray[widgetId];
         
        if ((CapSense_WD_TYPE_ENUM)ptrFlashWdgt->wdgtType != CapSense_WD_TOUCHPAD_E)
        {
            state = CYRET_BAD_PARAM;
        }
    }
      
    if (CYRET_SUCCESS == state)
    {
        switch(CapSense_GET_SENSE_METHOD(ptrFlashWdgt))
        {
        #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN)
            case CapSense_SENSE_METHOD_CSD_E:
                wdCsdTouchpad = ptrFlashWdgt->ptr2WdgtRam;
                
                result = ((uint32)wdCsdTouchpad->posX) |
                         ((uint32)wdCsdTouchpad->posY << 16u);
                break;
        #endif /* #if (0u != CapSense_CSD_TOUCHPAD_WIDGET_EN) */
        
        #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN)
            case CapSense_SENSE_METHOD_CSX_E:
                wdCsxTouchpad = ptrFlashWdgt->ptr2WdgtRam;
                
                result = ((uint32)wdCsxTouchpad->touch[0u].x) |
                         ((uint32)wdCsxTouchpad->touch[0u].y << 16u);
                break;
        #endif /* #if (0u != CapSense_CSX_TOUCHPAD_WIDGET_EN) */
        
        default:
            CYASSERT(0u);
            break;
        }
    }
    
    return result;
}
#endif /* #if (0u != CapSense_TOUCHPAD_WIDGET_EN) */

/* [] END OF FILE */
