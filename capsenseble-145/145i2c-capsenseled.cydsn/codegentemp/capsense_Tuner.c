/***************************************************************************//**
* \file capsense_Tuner.c
* \version 3.0
*
* \brief
* This file provides the source code for the Tuner module API of the Component.
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
#include "capsense_Structure.h"
#include "capsense_Tuner.h"
#include "capsense_Control.h"

/* Tuner states definition */
#define TU_FSM_RUNNING      (0u)
#define TU_FSM_SUSPENDED    (1u)
#define TU_FSM_SNR_TEST     (2u)

/* Tuner state variable */
static uint32 tunerFsm = TU_FSM_RUNNING;


/*******************************************************************************
* Function Name: capsense_TuInitialize
****************************************************************************//**
*
* \brief
*   This function initializes the communication interface with the Tuner GUI.
*
* \details
*   This API sets the tuner state machine to the default state. It is called by
*   the capsense_Start() API.
*
*******************************************************************************/
void capsense_TuInitialize(void)
{
    volatile capsense_RAM_STRUCT *ptrDsRam = &capsense_dsRam;
    
    ptrDsRam->tunerCmd = (uint16)capsense_TU_CMD_NONE_E;
    tunerFsm = TU_FSM_RUNNING;
}

/*******************************************************************************
* Function Name: capsense_RunTuner
****************************************************************************//**
*
* \brief
*   This function should be called to establish synchronized communication
*   with the Tuner application.
*
* \details
*   This API should be periodically called by the application layer to service
*   the host requests in the command register.
*
*   This API blocks the CPU if the host sends the "Suspend" command. The 
*   execution is suspended until any different command is sent by the host.
*
*   If the SNR TEST mode is initiated by the host, this API copies the
*   raw count data of the widget indicated in the snrTestWidgetId and
*   snrTestSensorId registers into the snrTestRawCount register.
*
*   This API calls the capsense_Start() function every  if restart
*   command is received from a host.
*
* \return
*   Indicates if restart command was executed or not: \n
*   capsense_TU_STS_NONE restart was no performed \n
*   capsense_TU_STS_RESTART_DONE restart was done by calling 
*   capsense_Start() API
*
*******************************************************************************/
uint32 capsense_RunTuner(void)
{
    uint8  critSect;
    uint32 widgetId;
    uint32 sensorId;
    uint32 updateFlag = 0Lu;
    capsense_TU_CMD_ENUM command;
    uint32 status = capsense_TU_STS_NONE;
    
    volatile capsense_RAM_STRUCT *ptrDsRam;
    volatile capsense_RAM_SNS_STRUCT *ptrSns;
    
    ptrDsRam = &capsense_dsRam;
    
    do 
    {
        command = (capsense_TU_CMD_ENUM)ptrDsRam->tunerCmd;
        
        /* Check command register */
        switch (command)
        {
        case capsense_TU_CMD_SUSPEND_E:
            tunerFsm = TU_FSM_SUSPENDED;
            updateFlag = 1Lu;
            break;
            
        case capsense_TU_CMD_RESUME_E:    
            tunerFsm = TU_FSM_RUNNING;
            updateFlag = 1Lu;
            break;
            
        case capsense_TU_CMD_RESTART_E:
            (void)capsense_Start();
            status = capsense_TU_STS_RESTART_DONE;
            tunerFsm = TU_FSM_RUNNING;
            updateFlag = 1Lu;
            break;
            
        case capsense_TU_CMD_RUN_SNR_TEST_E:
            tunerFsm = TU_FSM_SNR_TEST;
            updateFlag = 1Lu;
            break;
            
        case capsense_TU_CMD_PING_E:
            updateFlag = 1Lu;
            break;
            
        default:
            break;
        }
        
        /* Set Complete flag in command register if needed */
        if (0Lu != updateFlag)
        {
            critSect = CyEnterCriticalSection();
            
            /* Check if command wasn't overwritten with new command */
            if (command == (capsense_TU_CMD_ENUM)ptrDsRam->tunerCmd)
            {
                ptrDsRam->tunerCmd |= capsense_TU_CMD_COMPLETE_BIT;
            }
            CyExitCriticalSection(critSect);
            
            updateFlag = 0Lu;
        }
    } while (TU_FSM_SUSPENDED == tunerFsm);

    /* Check for SNR Test mode and do appropriate actions */
    if (TU_FSM_SNR_TEST == tunerFsm)
    {
        widgetId = ptrDsRam->snrTestWidgetId;
        sensorId = ptrDsRam->snrTestSensorId;
        
        /* Check if widgetId and sensorId are in range, 
         * and data was updated in RAM (scan counter changed) 
         */
        if ((widgetId < capsense_TOTAL_WIDGETS)              && 
            (sensorId < capsense_GET_SENSOR_COUNT(widgetId)) &&
            (ptrDsRam->scanCounter != ptrDsRam->snrTestScanCounter))
        {
            /* Get access to the Sensor Object */
            ptrSns = capsense_dsFlash.wdgtArray[widgetId].ptr2SnsRam;
            ptrSns += sensorId;
            
            /* Update data in SNR Test Object */
            critSect = CyEnterCriticalSection();
            ptrDsRam->snrTestScanCounter = ptrDsRam->scanCounter;
            ptrDsRam->snrTestRawCount[0u] = ptrSns->raw[0u];
            
            #if (0u != capsense_MULTI_FREQ_SCAN_EN)
                ptrDsRam->snrTestRawCount[1u] = ptrSns->raw[1u];
                ptrDsRam->snrTestRawCount[2u] = ptrSns->raw[2u];
            #endif
            
            CyExitCriticalSection(critSect);
        } 
    }
    return status;
}


/* [] END OF FILE */
