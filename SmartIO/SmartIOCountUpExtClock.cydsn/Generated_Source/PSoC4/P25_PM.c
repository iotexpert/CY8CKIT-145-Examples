/*******************************************************************************
* File Name: P25.c  
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
#include "P25.h"

static P25_BACKUP_STRUCT  P25_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: P25_Sleep
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
*  \snippet P25_SUT.c usage_P25_Sleep_Wakeup
*******************************************************************************/
void P25_Sleep(void)
{
    #if defined(P25__PC)
        P25_backup.pcState = P25_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            P25_backup.usbState = P25_CR1_REG;
            P25_USB_POWER_REG |= P25_USBIO_ENTER_SLEEP;
            P25_CR1_REG &= P25_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P25__SIO)
        P25_backup.sioState = P25_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        P25_SIO_REG &= (uint32)(~P25_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: P25_Wakeup
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
*  Refer to P25_Sleep() for an example usage.
*******************************************************************************/
void P25_Wakeup(void)
{
    #if defined(P25__PC)
        P25_PC = P25_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            P25_USB_POWER_REG &= P25_USBIO_EXIT_SLEEP_PH1;
            P25_CR1_REG = P25_backup.usbState;
            P25_USB_POWER_REG &= P25_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(P25__SIO)
        P25_SIO_REG = P25_backup.sioState;
    #endif
}


/* [] END OF FILE */
