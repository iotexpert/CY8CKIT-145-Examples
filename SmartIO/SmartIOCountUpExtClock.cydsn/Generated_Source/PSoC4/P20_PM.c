/*******************************************************************************
* File Name: P20.c  
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
#include "P20.h"

static P20_BACKUP_STRUCT  P20_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: P20_Sleep
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
*  \snippet P20_SUT.c usage_P20_Sleep_Wakeup
*******************************************************************************/
void P20_Sleep(void)
{
    #if defined(P20__PC)
        P20_backup.pcState = P20_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            P20_backup.usbState = P20_CR1_REG;
            P20_USB_POWER_REG |= P20_USBIO_ENTER_SLEEP;
            P20_CR1_REG &= P20_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P20__SIO)
        P20_backup.sioState = P20_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        P20_SIO_REG &= (uint32)(~P20_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: P20_Wakeup
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
*  Refer to P20_Sleep() for an example usage.
*******************************************************************************/
void P20_Wakeup(void)
{
    #if defined(P20__PC)
        P20_PC = P20_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            P20_USB_POWER_REG &= P20_USBIO_EXIT_SLEEP_PH1;
            P20_CR1_REG = P20_backup.usbState;
            P20_USB_POWER_REG &= P20_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P20__SIO)
        P20_SIO_REG = P20_backup.sioState;
    #endif
}


/* [] END OF FILE */
