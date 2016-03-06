/***************************************************************************//**
* \file     capsense_Filter.c
* \version  3.0
*
* \brief    This file contains the implementation source code to implement all 
*           firmware filters.
*
* \note:
*   None.
*
* Firmware Dependency:
*   1. capsense_Customizer.h
*   2. capsense_DataStructure.h
*
* Hardware Dependency:
*   None.
*
*   \see    1. 001-64846 Getting Started With CapSense
*   \see    2. 001-85951 PSoC 4 CapSense Design Guide
*   \see    3. CapSense Component Datasheet
*   \see    4. PSoC 4 Device Technical Reference Manual
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

#include "capsense_Filter.h"

#if (0 != (capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN))
    #include "capsense_SmartSense_LL.h"
#endif /* (0 != (capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN)) */

/*******************************************************************************
* Function Name: capsense_FtInitialize
****************************************************************************//**
*
* \brief
*    Initializes all the firmware filter history, except the baseline filter.
*
* \details
*    Initializes all the firmware filter history, except the baseline filter.
*
*******************************************************************************/
void capsense_FtInitialize(void)
{
    uint32 widgetId;
    uint32 sensorId;
    uint32 sensorsNumber;

    capsense_FLASH_WD_STRUCT const *ptrWidget;
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj;
    
    #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN
        capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam;
        SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope = NULL;
    #endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */
    
    for (widgetId = capsense_TOTAL_WIDGETS; widgetId-- > 0u;)
    {
        ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];
        
        #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN
            ptrWidgetRam = ptrWidget->ptr2WdgtRam;
        #endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */

        /* Find total amount of sensors in specified widget */   
        sensorsNumber = capsense_GET_SNS_CNT_BY_PTR(ptrWidget);    
    
        for (sensorId = sensorsNumber; sensorId-- > 0u;) 
        {
            /* Find pointer to specified sensor object */
            ptrSensor = ptrWidget->ptr2SnsRam;
            ptrSensor = &ptrSensor[sensorId];
                        
            /* Find pointer to to specified filter sensor object */
            ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;

            #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN
                /* Find pointer to specified noise envelope sensor object */
                ptrNoiseEnvelope = ptrWidget->ptr2NoiseEnvlp;
                ptrNoiseEnvelope = &ptrNoiseEnvelope[sensorId];    
            #endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */
            
            if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
            {
                #if (capsense_REGULAR_SENSOR_EN)             
                    ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
                #endif /* (capsense_REGULAR_SENSOR_EN) */
            }
            else
            {
                #if (capsense_PROXIMITY_SENSOR_EN)             
                    ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
                #endif /* (capsense_PROXIMITY_SENSOR_EN) */
            }
            
            #if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN)
                capsense_InitializeIIRInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
            #endif /* (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN) */                

            #if (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
                capsense_InitializeMedianInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
            #endif /* (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN) */

            #if (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN)
                capsense_InitializeAverageInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
            #endif /* (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN) */
            
            #if (capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN)
                if (capsense_SENSE_METHOD_CSD_E ==
                    (capsense_SENSE_METHOD_ENUM)capsense_GET_SENSE_METHOD(&capsense_dsFlash.wdgtArray[widgetId]))
                {
                    SmartSense_InitializeNoiseEnvelope(ptrSensor->raw[0u], ptrWidgetRam->sigPPf, ptrNoiseEnvelope);
                }
            #endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */
        }
    }   
}


/*******************************************************************************
* Function Name: capsense_UpdateAllBaselines
****************************************************************************//**
*
* \brief    Updates the baseline for all sensors in all widgets.
*
* \details
*  Updates the baseline for all sensors in all widgets.
*
* \return   Returns a status indicating whether all relevant sensor 
*           baselines have been updated.
*
*******************************************************************************/
cystatus capsense_UpdateAllBaselines(void)
{
    uint32 widgetId; 
    cystatus bslnStatus = CYRET_SUCCESS;
    
    for(widgetId = capsense_TOTAL_WIDGETS; widgetId-- > 0u;)
    {            
        bslnStatus |= capsense_UpdateWidgetBaseline(widgetId);       
    }      
    
    return(bslnStatus);
}
  

/*******************************************************************************
* Function Name: capsense_UpdateWidgetBaseline
****************************************************************************//**
*
* \brief    Updates the baseline for all sensors in a widget indicated by an input
*           parameter.
*
* \details  Updates the baseline for all sensors in a widget indicated by an input
*           parameter.
*
* \param   widgetId Specifies the ID number of the widget to update the baseline of
*                       the sensors in the widget.
*
* \return   Returns a status indicating whether the baselines of all
*           relevant sensors have been updated or not.
*
*******************************************************************************/
cystatus capsense_UpdateWidgetBaseline(uint32 widgetId)
{
    uint32 sensorId;       
    uint32 sensorsNumber;  

    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];    
    cystatus bslnStatus = CYRET_SUCCESS;    
    
    /* Find total amount of sensors in specified widget */   
    sensorsNumber = capsense_GET_SNS_CNT_BY_PTR(ptrWidget);    
    
    for(sensorId = sensorsNumber; sensorId-- > 0u;)
    {
        bslnStatus |= capsense_UpdateSensorBaseline(widgetId, sensorId);    
    }

    return(bslnStatus);
}


/*******************************************************************************
* Function Name: capsense_UpdateSensorBaseline
****************************************************************************//**
*
* \brief    Updates the baseline for a sensor specified by an input parameter.
*
* \details  Updates the baseline for a sensor specified by an input parameter.
*
* \param   widgetId Specifies the ID number of the widget to update the baseline.
*
* \param   sensorId Specifies the ID number of the sensor in the widget to update the baseline.
*
* \return   Returns a status indicating whether the baseline has been updated. 
*
*******************************************************************************/
cystatus capsense_UpdateSensorBaseline(uint32 widgetId, uint32 sensorId)
{    
    cystatus bslnStatus = CYRET_SUCCESS;  
    
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];    
    capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam = ptrWidget->ptr2WdgtRam;   
          
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    bslnStatus = capsense_FtUpdateBaseline(
                        ptrWidgetRam, ptrSensor, (uint32)ptrWidget->wdgtType);
   
    return(bslnStatus);
}
 

/*******************************************************************************
* Function Name: capsense_FtUpdateBaseline
****************************************************************************//**
*
* \brief    Updates the baseline for a sensor specified by an input parameter.
*
* \details  Updates the baseline for a sensor specified by an input parameter.
*
* \param    ptr2wd_ram The pointer to the widget RAM structure where all the widget 
*           parameters are stored.
*
* \param    ptrSensor The pointer to the sensor RAM structure where all the sensor
*           parameters are stored.
*
* \return   Returns a status indicating whether the baseline has been updated. 
*
*******************************************************************************/
cystatus capsense_FtUpdateBaseline(
                                capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam,
                                capsense_RAM_SNS_STRUCT *ptrSensor, 
                                uint32 wdType)
{         
    uint32 sign;
    uint32 difference;        
    uint32 channel;
    cystatus bslnStatus = CYRET_SUCCESS; 
    
    #if (capsense_TOTAL_WIDGETS)  
        uint32 history;
    #endif /* (capsense_TOTAL_WIDGETS) */    
    
    /* Apply baseline to every channel in sensor */
    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;) 
    {               
        /* Calculate signal value and its sign */
        if(ptrSensor->raw[channel] >= ptrSensor->bsln[channel])
        {
            difference = (uint32)ptrSensor->raw[channel] - (uint32)ptrSensor->bsln[channel];
            sign = 1u;
            ptrSensor->negBslnRstCnt[channel] = 0u;
        }
        else
        {
            difference = (uint32)ptrSensor->bsln[channel] - (uint32)ptrSensor->raw[channel];
            sign = 0u;
        }     
        
        /* Reset baseline if condition is met */ 
        if((sign == 0u) && (difference > (uint32) ptrWidgetRam->nNoiseTh))
        {
            if(ptrSensor->negBslnRstCnt[channel] >= ptrWidgetRam->lowBslnRst)
            {                
                capsense_FtInitializeBaselineChannel(ptrSensor, wdType, channel);             
            }
            else
            {
                ptrSensor->negBslnRstCnt[channel]++;
            }
        }
        else
        {
            #if (!capsense_SENSOR_AUTO_RESET_EN)
                /* Update baseline only if signal is in range between noiseThreshold and negativenoiseThreshold */
                if ((difference <= (uint32)ptrWidgetRam->noiseTh) ||
                    ((difference < (uint32) ptrWidgetRam->nNoiseTh) && (sign == 0u)))
                {
            #endif /* (CapSense_CSD_AUTO_RESET == CapSense_CSD_AUTO_RESET_DISABLE) */
                    #if (capsense_BASELINE_TYPE == capsense_IIR_BASELINE)  
                        
                        /* Calculate baseline value */
                        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
                        {
                            #if (capsense_REGULAR_SENSOR_EN) 
                                #if (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                                    
                                    history = (uint32) ptrSensor->bsln[channel] << 8u; 
                                    history |= ptrSensor->bslnExt[channel];
                                #else
                                    history = ptrSensor->bsln[channel];                                     
                                #endif /* (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */                            
                            
                                history =  capsense_FtIIR1stOrder(
                                                (uint32)ptrSensor->raw[channel],
                                                history, 
                                                capsense_REGULAR_IIR_BL_N, 
                                                capsense_REGULAR_IIR_BL_SHIFT);
                                                           
                                #if (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                                    
                                    ptrSensor->bsln[channel] = LO16(history >> 8u);
                                    ptrSensor->bslnExt[channel] = LO8(history);
                                #else
                                    ptrSensor->bsln[channel] = LO16(history);
                                #endif /* (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
                            #endif /* (capsense_REGULAR_SENSOR_EN) */
                        }
                        else 
                        {
                            #if (capsense_PROXIMITY_SENSOR_EN)     
                                #if (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                                
                                    history = (uint32) ptrSensor->bsln[channel] << 8u; 
                                    history |= ptrSensor->bslnExt[channel];
                                #else
                                    history = (uint32) ptrSensor->bsln[channel];                                    
                                #endif /* (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */ 
                                
                                history = capsense_FtIIR1stOrder((uint32)ptrSensor->raw[channel], history, capsense_PROX_IIR_BL_N , capsense_PROX_IIR_BL_SHIFT);
                                
                                #if (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                                
                                    ptrSensor->bsln[channel] = LO16(history >> 8u);
                                    ptrSensor->bslnExt[channel] = LO8(history);
                                #else
                                    ptrSensor->bsln[channel] = LO16(history);
                                #endif /* (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */        
                            #endif /* (capsense_PROX_SENSOR_EN)   */
                        }
                    #else
                        
                        /******************************************************************
                        * This is the place where the bucket algorithm should be implemented.
                        * The bucket method will be implemented in the next component version.
                        *******************************************************************/
                        
                    #endif /* (capsense_CSD_BASELINE_TYPE == capsense_IIR_BASELINE)  */
                    
            #if (!capsense_SENSOR_AUTO_RESET_EN)
                }
            #endif /* (!capsense_SENSOR_AUTO_RESET_EN) */
        }
    }
    
    return(bslnStatus);    
}


/*******************************************************************************
* Function Name: capsense_InitializeAllBaselines
****************************************************************************//**
*
* \brief    Initializes the baseline for all sensors of all widgets. 
*           Also, this function can be used to re-initialise baselines.
*
* \details  Initializes the baseline for all sensors of all widgets. 
*           Also, this function can be used to re-initialise baselines.
*
*******************************************************************************/
void capsense_InitializeAllBaselines(void)
{
    uint32 widgetId;    
    
    for(widgetId = capsense_TOTAL_WIDGETS; widgetId-- > 0u;)
    {
        capsense_InitializeWidgetBaseline(widgetId); 
    }
}


/*******************************************************************************
* Function Name: capsense_InitializeWidgetBaseline
****************************************************************************//**
*
* \brief    Initializes the baseline for all sensors of the widgets indicated by 
*           an input parameter. 
*
* \details  Initializes the baseline for all sensors of the widgets indicated by 
*           an input parameter. 
*
* \param  widgetId Specifies the ID number of a widget to initialize the baseline 
*                       of all sensors in the widget.
*
*******************************************************************************/
void capsense_InitializeWidgetBaseline(uint32 widgetId)
{   
    uint32 sensorId;
    uint32 sensorsNumber;
  
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];

    /* Find total amount of sensors in specified widget */   
    sensorsNumber = capsense_GET_SNS_CNT_BY_PTR(ptrWidget);    
    
    for(sensorId = sensorsNumber; sensorId-- > 0u;)
    {
        capsense_InitializeSensorBaseline(widgetId, sensorId);   
    }
}


/*******************************************************************************
* Function Name: capsense_InitializeSensorBaseline
****************************************************************************//**
*
* \brief    Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \details  Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \param    widgetId Specifies the ID number of a widget to initialize the baseline 
*                       of all sensors in the widget.
* \param    sensorId Specifies the ID number of sensor to initialize the baseline.
*
*******************************************************************************/
void capsense_InitializeSensorBaseline(uint32 widgetId, uint32 sensorId)
{      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_FLASH_WD_STRUCT const *ptrWidget = NULL;
    ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];
    
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    capsense_FtInitializeBaseline(ptrSensor, (uint32)ptrWidget->wdgtType);      
}


/*******************************************************************************
* Function Name: capsense_FtInitializeBaseline
****************************************************************************//**
*
* \brief    Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \details  Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \param    *ptrSensor The pointer to the sensor RAM object.
* \param    wdType     Specifies the type of a widget.
*
*******************************************************************************/
CY_INLINE void capsense_FtInitializeBaseline(capsense_RAM_SNS_STRUCT *ptrSensor, uint32 wdType) 
{
    uint32 channel;

    /* Apply baseline initialization to every channel in sensor */    
    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;) 
    {
        capsense_FtInitializeBaselineChannel(ptrSensor, wdType, channel); 
    }
}


/*******************************************************************************
* Function Name: capsense_FtInitializeBaselineChannel
****************************************************************************//**
*
* \brief    Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \details  Initializes the baseline history for a sensor indicated by an input
*           parameter.
*
* \param    *ptrSensor The pointer to the sensor RAM object.
* \param    wdType     Specifies the type of a widget.
* \param    channel    Specifies the number of the channel to be initialized.
*
*******************************************************************************/
void capsense_FtInitializeBaselineChannel(capsense_RAM_SNS_STRUCT *ptrSensor, uint32 wdType, uint32 channel) 
{
    #if (capsense_BASELINE_TYPE == capsense_IIR_BASELINE)  
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN)
                #if (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                    
                    ptrSensor->bslnExt[channel] = 0u;                
                #endif /* (capsense_REGULAR_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_REGULAR_SENSOR_EN) */
        }
        else
        {
            #if (capsense_PROXIMITY_SENSOR_EN)     
                #if (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE)                   
                    ptrSensor->bslnExt[channel] = 0u;               
                #endif /* (capsense_PROX_IIR_BL_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_PROX_SENSOR_EN) */
        }
    #else
        /* capsense_BASELINE_TYPE == capsense_BUCKET_BASELINE */        
        ptrSensor->bslnExt[channel] = 0u;        
    #endif /* (capsense_BASELINE_TYPE == capsense_IIR_BASELINE) */      
    
    ptrSensor->bsln[channel] = ptrSensor->raw[channel];
    ptrSensor->negBslnRstCnt[channel] = 0u;
}


#if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN)

/*******************************************************************************
* Function Name: capsense_InitializeIIR
****************************************************************************//**
*
* \brief    Initialize the IIR filter history.
*
* \details  Initialize the IIR filter history.
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_InitializeIIR(uint32 widgetId, uint32 sensorId)
{   
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
        
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }    
    
    capsense_InitializeIIRInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}


/*******************************************************************************
* Function Name: capsense_RunIIR
****************************************************************************//**
*
* \brief    Executes the IIR filter algorithm on a sensor indicated by an input
*           parameter. 
*
* \details  Executes the IIR filter algorithm on a sensor indicated by an input
*           parameter. 
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_RunIIR(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
        
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }    
    
    capsense_RunIIRInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}
    
/*******************************************************************************
* Function Name: capsense_InitializeIIRInternal
****************************************************************************//**
*
* \brief    Initializes the IIR filter history.
*
* \details  Initializes the IIR filter history.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.                                  
*
*******************************************************************************/
void capsense_InitializeIIRInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                            capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    uint32 channel;

    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_IIR_FILTER_EN)
                #if (capsense_REGULAR_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE)
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory = ptrSensorObj->raw[channel];
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistoryLow = 0u;
                #else
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory = ptrSensorObj->raw[channel] << capsense_REGULAR_IIR_RC_SHIFT;                   
                #endif /* (capsense_REGULAR_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_REGULAR_SENSOR_EN) */    
        }
        else
        {
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_IIR_FILTER_EN)
                #if (capsense_PROX_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE)
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory = ptrSensorObj->raw[channel];
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistoryLow = 0u;    
                #else
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory = ptrSensorObj->raw[channel] << capsense_PROX_IIR_RC_SHIFT;                                   
                #endif /* (capsense_PROX_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_PROX_SENSOR_EN) */
        }    
    }    
}
 

/*******************************************************************************
* Function Name: capsense_RunIIRInternal
****************************************************************************//**
*
* \brief    Run the IIR filter.
*
* \details  Run the IIR filter.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/    
void capsense_RunIIRInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj, 
                                     capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{        
    uint32 channel;
    uint32 temp;

    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_IIR_FILTER_EN)
                #if (capsense_REGULAR_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE)
                    temp =  ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory << capsense_REGULAR_IIR_RC_SHIFT;
                    temp |= ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistoryLow;
                    temp =capsense_FtIIR1stOrder((uint32)ptrSensorObj->raw[channel],
                                                        temp,
                                                        capsense_REGULAR_IIR_RC_N,
                                                        capsense_REGULAR_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory = LO16(temp >>capsense_REGULAR_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistoryLow = LO8(temp);
                    ptrSensorObj->raw[channel] = LO16(temp >>capsense_REGULAR_IIR_RC_SHIFT);
                #else
                    temp =capsense_FtIIR1stOrder((uint32)ptrSensorObj->raw[channel],
                                                        (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory,
                                                        capsense_REGULAR_IIR_RC_N,
                                                        capsense_REGULAR_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].iirHistory = LO16(temp);
                    ptrSensorObj->raw[channel] = LO16(temp >>capsense_REGULAR_IIR_RC_SHIFT);
                #endif /* (capsense_REGULAR_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_REGULAR_SENSOR_EN) */    
        }
        else
        {
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_IIR_FILTER_EN)
                #if (capsense_PROX_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE)
                    temp =  ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory << capsense_PROX_IIR_RC_SHIFT;
                    temp |= ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistoryLow;
                    temp =capsense_FtIIR1stOrder((uint32)ptrSensorObj->raw[channel],
                                                        temp,
                                                        capsense_PROX_IIR_RC_N,
                                                        capsense_PROX_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory = LO16(temp >>capsense_PROX_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistoryLow = LO8(temp);
                    ptrSensorObj->raw[channel] = LO16(temp >>capsense_PROX_IIR_RC_SHIFT);
                #else
                    temp =capsense_FtIIR1stOrder((uint32)ptrSensorObj->raw[channel],
                                                        (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory, 
                                                        capsense_PROX_IIR_RC_N,
                                                        capsense_PROX_IIR_RC_SHIFT);
                    ptrFilterHistObj.ptrProx->proxChannel[channel].iirHistory = LO16(temp);
                    ptrSensorObj->raw[channel] = LO16(temp >>capsense_PROX_IIR_RC_SHIFT);
                #endif /* (capsense_PROX_IIR_RC_TYPE == capsense_IIR_FILTER_PERFORMANCE) */
            #endif /* (capsense_PROX_SENSOR_EN) */
        }    
    }        
}    
#endif /* #if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN) */


#if (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)

/*******************************************************************************
* Function Name: capsense_InitializeMedian
****************************************************************************//**
*
* \brief    Initializes the Median filter history.
*
* \details  Initializes the Median filter history.
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_InitializeMedian(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }
    
    capsense_InitializeMedianInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}     


/*******************************************************************************
* Function Name: capsense_RunMedian
****************************************************************************//**
*
* \brief    Executes the Median filter algorithm on a sensor indicated by an input 
*           parameter. 
*
* \details  Executes the Median filter algorithm on a sensor indicated by an input 
*           parameter. 
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_RunMedian(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }
    
    capsense_RunMedianInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}
   

/*******************************************************************************
* Function Name: capsense_InitializeMedianInternal
****************************************************************************//**
*
* \brief    Initializes the Median filter.
*
* \details  Initializes the Median filter.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/
void capsense_InitializeMedianInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                               capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    uint32 channel;

    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_MEDIAN_FILTER_EN)
                ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[0u] = ptrSensorObj->raw[channel];
                ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[1u] = ptrSensorObj->raw[channel];
            #endif /* (capsense_REGULAR_SENSOR_EN) */
        }
        else
        {
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_MEDIAN_FILTER_EN)
                ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[0u] = ptrSensorObj->raw[channel];
                ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[1u] = ptrSensorObj->raw[channel];
            #endif /* (capsense_PROXIMITY_SENSOR_EN) */               
        }
    }
}


/*******************************************************************************
* Function Name: capsense_RunMedianInternal
****************************************************************************//**
*
* \brief    Runs the Median filter.
*
* \details  Runs the Median filter.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/
void capsense_RunMedianInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,
                                        capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    uint32 channel;
    
    #if ((capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_MEDIAN_FILTER_EN) || \
         (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_MEDIAN_FILTER_EN))
        uint32 temp;
    #endif
    
    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {    
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {                                             
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_MEDIAN_FILTER_EN)
                temp = capsense_FtMedian((uint32)ptrSensorObj->raw[channel],\
                                                 (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[0u],\
                                                 (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[1u]);
                ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[1u] = \
                ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[0u];
                ptrFilterHistObj.ptrRegular->regularChannel[channel].medHistory[0u] = ptrSensorObj->raw[channel];
                ptrSensorObj->raw[channel] = LO16(temp);
            #endif /* (capsense_REGULAR_SENSOR_EN) */     
        }
        else 
        {                   
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_MEDIAN_FILTER_EN)
                temp = capsense_FtMedian((uint32)ptrSensorObj->raw[channel],\
                                                 (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[0u],\
                                                 (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[1u]);
                ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[1u] = \
                ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[0u];
                ptrFilterHistObj.ptrProx->proxChannel[channel].medHistory[0u] = ptrSensorObj->raw[channel];
                ptrSensorObj->raw[channel] = LO16(temp);
            #endif /* (capsense_PROXIMITY_SENSOR_EN) */               
        }
    }
}
#endif /* (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN) */


#if (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN)

/*******************************************************************************
* Function Name: capsense_InitializeAverage
****************************************************************************//**
*
* \brief    Initializes the average filter history.
*
* \details  Initializes the average filter history.
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_InitializeAverage(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj;

    /* Find pointer to specified sensor object */
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;

    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }

    capsense_InitializeAverageInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}


/*******************************************************************************
* Function Name: capsense_RunAverage
****************************************************************************//**
*
* \brief    Executes the average filter algorithm on a sensor indicated by an input 
*           parameter. 
*
* \details  Executes the average filter algorithm on a sensor indicated by an input 
*           parameter. 
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_RunAverage(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }
    
    capsense_RunAverageInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}


/*******************************************************************************
* Function Name: capsense_InitializeAverageInternal
****************************************************************************//**
*
* \brief    Initializes the average filter.
*
* \details  Initializes the average filter.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/
void capsense_InitializeAverageInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,\
                                                  capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    uint32 channel;

    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_AVERAGE_FILTER_EN)
                ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                #if (capsense_REGULAR_AVERAGE_LEN == capsense_AVERAGE_FILTER_LEN_4)
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[1u] = ptrSensorObj->raw[channel];
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[2u] = ptrSensorObj->raw[channel];
                #endif /* capsense_REGULAR_AVERAGE_LEN == CapSense_1_AVERAGE_FILTER_LEN_4 */
            #endif /* (capsense_REGULAR_SENSOR_EN) */      
        }
        else
        {
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_AVERAGE_FILTER_EN)
                ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                    #if (capsense_PROX_AVERAGE_LEN == capsense_AVERAGE_FILTER_LEN_4)
                        ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[1u] = ptrSensorObj->raw[channel];
                        ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[2u] = ptrSensorObj->raw[channel];
                    #endif /* capsense_REGULAR_AVERAGE_LEN == CapSense_1_AVERAGE_FILTER_LEN_4 */
            #endif /* (capsense_PROXIMITY_SENSOR_EN) */
        }
    }
}


/*******************************************************************************
* Function Name: capsense_RunAverageInternal
****************************************************************************//**
*
* \brief    Runs the average filter.
*
* \details  Runs the average filter.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/
void capsense_RunAverageInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj,\
                                           capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    uint32 channel;
    uint32 temp;

    for(channel = capsense_NUM_SCAN_FREQS; channel-- > 0u;)
    {
        if ((capsense_WD_TYPE_ENUM)wdType != capsense_WD_PROXIMITY_E)
        {
            #if (capsense_REGULAR_SENSOR_EN && capsense_REGULAR_RC_AVERAGE_FILTER_EN)
                #if (capsense_REGULAR_AVERAGE_LEN == capsense_AVERAGE_FILTER_LEN_2)
                    temp = ((uint32)ptrSensorObj->raw[channel] + 
                            (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0]) >> 1u;
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                    ptrSensorObj->raw[channel] = LO16(temp);
                #else
                    temp = ((uint32)ptrSensorObj->raw[channel] + 
                            (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0] + 
                            (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[1u] + 
                            (uint32)ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[2u]) >> 2u;

                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[2u] =
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[1u];
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[1u] =
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0];
                    ptrFilterHistObj.ptrRegular->regularChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                    ptrSensorObj->raw[channel] = LO16(temp);
                #endif /* capsense_REGULAR_AVERAGE_LEN == CapSense_1_AVERAGE_FILTER_LEN_4 */
            #endif /* (capsense_REGULAR_SENSOR_EN) */
        }
        else 
        {
            #if (capsense_PROXIMITY_SENSOR_EN && capsense_PROX_RC_AVERAGE_FILTER_EN)
                #if (capsense_PROX_AVERAGE_LEN == capsense_AVERAGE_FILTER_LEN_2)     
                    temp = ((uint32)ptrSensorObj->raw[channel] + 
                            (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0]) >> 1u;
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                    ptrSensorObj->raw[channel] = LO16(temp);
                #else
                    temp = ((uint32)ptrSensorObj->raw[channel] + 
                            (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0] +
                            (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[1u] +
                            (uint32)ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[2u]) >> 2u;
                    
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[2u] =
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[1u];
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[1u] =
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0];
                    ptrFilterHistObj.ptrProx->proxChannel[channel].avgHistory[0] = ptrSensorObj->raw[channel];
                    ptrSensorObj->raw[channel] = LO16(temp);
                #endif /* capsense_PROX_AVERAGE_LEN == CapSense_1_AVERAGE_FILTER_LEN_4 */
            #endif /* (capsense_PROXIMITY_SENSOR_EN) */
        }
    }
}
#endif /* (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN) */


#if ((capsense_REGULAR_RC_FILTER_EN || capsense_PROX_RC_FILTER_EN))
/*******************************************************************************
* Function Name: capsense_FtRunEnabledFilters
****************************************************************************//**
*
* \brief    Runs all enabled filters.
*
* \details  Runs all enabled filters.
*
* \param    widgetId Specifies the ID number of a widget to update the IIR filter history.*
* \param    sensorId Specifies the ID number of a sensor in the widget to update the IIR 
*           filter history.
*
*******************************************************************************/
void capsense_FtRunEnabledFilters(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];      
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    capsense_PTR_FILTER_VARIANT ptrFilterHistObj; 
              
    /* Find pointer to specified sensor object */     
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */     
    ptrFilterHistObj.ptr = ptrWidget->ptr2FltrHistory;
    
    if ((capsense_WD_TYPE_ENUM)ptrWidget->wdgtType != capsense_WD_PROXIMITY_E)
    {
        #if (capsense_REGULAR_SENSOR_EN)
            ptrFilterHistObj.ptrRegular = &ptrFilterHistObj.ptrRegular[sensorId];
        #endif /* (capsense_REGULAR_SENSOR_EN) */
    }
    else
    {
        #if (capsense_PROXIMITY_SENSOR_EN)
            ptrFilterHistObj.ptrProx = &ptrFilterHistObj.ptrProx[sensorId];
        #endif /* (capsense_PROXIMITY_SENSOR_EN) */
    }
    
    capsense_FtRunEnabledFiltersInternal(ptrFilterHistObj, ptrSensor, (uint32)ptrWidget->wdgtType);
}


/*******************************************************************************
* Function Name: capsense_FtRunEnabledFiltersInternal
****************************************************************************//**
*
* \brief    Runs all enabled filters.
*
* \details  Runs all enabled filters.
*
* \param    ptrFilterHistObj The pointer to the filter RAM object of the sensor.
* \param    ptrSensorObj     The pointer to the sensor RAM object of the sensor.
* \param    wdType           Specifies the type of a widget.
*
*******************************************************************************/
void capsense_FtRunEnabledFiltersInternal(capsense_PTR_FILTER_VARIANT ptrFilterHistObj, 
                                                  capsense_RAM_SNS_STRUCT *ptrSensorObj, uint32 wdType)
{
    #if (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
        capsense_RunMedianInternal(ptrFilterHistObj, ptrSensorObj, wdType);
    #endif /* (capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN) */
    
    #if (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN)    
        capsense_RunIIRInternal(ptrFilterHistObj, ptrSensorObj, wdType);
    #endif /* (capsense_REGULAR_RC_IIR_FILTER_EN || capsense_PROX_RC_IIR_FILTER_EN) */ 

    #if (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN)
        capsense_RunAverageInternal(ptrFilterHistObj, ptrSensorObj, wdType);
    #endif /* (capsense_REGULAR_RC_AVERAGE_FILTER_EN || capsense_PROX_RC_AVERAGE_FILTER_EN) */            
}
#endif /* (capsense_REGULAR_RC_FILTER_EN || capsense_PROX_RC_FILTER_EN) */


#if (capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN)
/*******************************************************************************
* Function Name: capsense_RunNoiseEnvelope
****************************************************************************//**
*
* \brief    Executes the noise envelope  filter algorithm on a sensor indicated by 
*           an input parameter to measure the pk-to-pk noise in the sensor raw count.
*
* \details  Executes the noise envelope  filter algorithm on a sensor indicated by 
*           an input parameter to measure the pk-to-pk noise in the sensor raw count.
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_RunNoiseEnvelope(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];
    capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam = ptrWidget->ptr2WdgtRam;
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope = NULL;

    /* Find pointer to specified sensor object */
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */
    ptrNoiseEnvelope = ptrWidget->ptr2NoiseEnvlp;
    ptrNoiseEnvelope = &ptrNoiseEnvelope[sensorId];    

    SmartSense_RunNoiseEnvelope(ptrSensor->raw[0u], ptrWidgetRam->sigPPf, ptrNoiseEnvelope);
}


/*******************************************************************************
* Function Name: capsense_InitializeNoiseEnvelope
****************************************************************************//**
*
* \brief    Initializes the noise-envelope filter
*
* \details  Initializes the noise-envelope filter
*
* \param    widgetId Specifies the ID number of the widget.
* \param    sensorId Specifies the ID number of the sensor in the widget.
*
*******************************************************************************/
void capsense_InitializeNoiseEnvelope(uint32 widgetId, uint32 sensorId)
{
    capsense_FLASH_WD_STRUCT const *ptrWidget = &capsense_dsFlash.wdgtArray[widgetId];
    capsense_RAM_WD_BASE_STRUCT *ptrWidgetRam = ptrWidget->ptr2WdgtRam;
    capsense_RAM_SNS_STRUCT *ptrSensor = NULL;    
    SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT *ptrNoiseEnvelope = NULL;

    /* Find pointer to specified sensor object */
    ptrSensor = ptrWidget->ptr2SnsRam;
    ptrSensor = &ptrSensor[sensorId];
    
    /* Find pointer to specified filter sensor object */
    ptrNoiseEnvelope = ptrWidget->ptr2NoiseEnvlp;
    ptrNoiseEnvelope = &ptrNoiseEnvelope[sensorId];    

    SmartSense_InitializeNoiseEnvelope(ptrSensor->raw[0u], ptrWidgetRam->sigPPf, ptrNoiseEnvelope);
}
#endif /* #if capsense_CSD_AUTOTUNE & capsense_CSD_SS_TH_EN */


#if (capsense_POS_MEDIAN_FILTER_EN || capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN)
/*******************************************************************************
* Function Name: capsense_FtMedian
****************************************************************************//**
*
* \brief    Return the median value from the three passed arguments.
*
* \details  Return the median value from the three passed arguments.
*
* \param    x1 The first value to be compared.
* \param    x2 The second value to be compared.
* \param    x3 The third value to be compared.
*
* \return   Returns the median value of input arguments.
*
*******************************************************************************/
uint32 capsense_FtMedian(uint32 x1, uint32 x2, uint32 x3)
{
    uint32 tmp;

    if (x1 > x2)
    {
        tmp = x2;
        x2 = x1;
        x1 = tmp;
    }

    if (x2 > x3)
    {
        x2 = x3;
    }

    return ((x1 > x2) ? x1 : x2);
}
#endif /*capsense_POS_MEDIAN_FILTER_EN || capsense_REGULAR_RC_MEDIAN_FILTER_EN || capsense_PROX_RC_MEDIAN_FILTER_EN*/


/*******************************************************************************
* Function Name: capsense_FtIIR1stOrder
****************************************************************************//**
*
* \brief    Return the filtered data by the IIR 1-st order algorithm
*
* \details  Return the filtered data by the IIR 1-st order algorithm
*
* \param    input      The data to be filtered.
* \param    prevOutput The previous filtered data.
* \param    n          The IIR filter coefficient (n/256).
* \param    shift      The parameter is used to shift input data to have free LSB 
*                       bits for a fraction storage of the filter output calculation.
*
* \return   Returns the filtered data.
*
*******************************************************************************/
uint32 capsense_FtIIR1stOrder(uint32 input, uint32 prevOutput, uint32 n, uint32 shift)
{   
    uint32 output;
   
    /*
    * n - IIR filter coefficient (n/256)
    * shift - Used to shift input data to have free LSB bits
    * for fraction starage of filter output calculation
    */
    output = ((n * (input << shift)) + ((capsense_IIR_COEFFICIENT_K - n) * prevOutput)) >> 8u;
    
    /* Shift operation of output will be done in upper level API if needed */
    return output;
}


#if (capsense_POS_JITTER_FILTER_EN)
/*******************************************************************************
* Function Name: capsense_FtJitter
****************************************************************************//**
*
* \brief    Returns the filtered data by the jitter algorithm.
*
* \details  Returns the filtered data by the jitter algorithm.
*
* \param    input      The data to be filtered.
* \param    prevOutput The previous filtered data.
*
*
* \return   Returns the filtered data.
*
*******************************************************************************/
uint32 capsense_FtJitter(uint32 input, uint32 prevOutput)
{
    if (prevOutput > input)
    {
        input++;
    }
    else if (prevOutput < input)
    {
        input--;
    }
    else
    { 
        /* Nothing to do - MISRA 14.1 requirement*/
    }
    return input;
}
#endif /* capsense_POS_JITTER_FILTER_EN */


/* [] END OF FILE */
