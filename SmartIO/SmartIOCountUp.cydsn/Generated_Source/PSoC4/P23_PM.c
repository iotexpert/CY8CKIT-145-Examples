/*******************************************************************************
* File Name: P23.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "P23.h"

static P23_BACKUP_STRUCT  P23_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: P23_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet P23_SUT.c usage_P23_Sleep_Wakeup
*******************************************************************************/
void P23_Sleep(void)
{
    #if defined(P23__PC)
        P23_backup.pcState = P23_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            P23_backup.usbState = P23_CR1_REG;
            P23_USB_POWER_REG |= P23_USBIO_ENTER_SLEEP;
            P23_CR1_REG &= P23_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P23__SIO)
        P23_backup.sioState = P23_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        P23_SIO_REG &= (uint32)(~P23_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: P23_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to P23_Sleep() for an example usage.
*******************************************************************************/
void P23_Wakeup(void)
{
    #if defined(P23__PC)
        P23_PC = P23_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            P23_USB_POWER_REG &= P23_USBIO_EXIT_SLEEP_PH1;
            P23_CR1_REG = P23_backup.usbState;
            P23_USB_POWER_REG &= P23_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P23__SIO)
        P23_SIO_REG = P23_backup.sioState;
    #endif
}


/* [] END OF FILE */
