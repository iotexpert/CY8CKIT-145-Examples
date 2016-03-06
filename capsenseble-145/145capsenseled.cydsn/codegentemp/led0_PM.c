/*******************************************************************************
* File Name: led0.c  
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
#include "led0.h"

static led0_BACKUP_STRUCT  led0_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: led0_Sleep
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
*  \snippet led0_SUT.c usage_led0_Sleep_Wakeup
*******************************************************************************/
void led0_Sleep(void)
{
    #if defined(led0__PC)
        led0_backup.pcState = led0_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            led0_backup.usbState = led0_CR1_REG;
            led0_USB_POWER_REG |= led0_USBIO_ENTER_SLEEP;
            led0_CR1_REG &= led0_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(led0__SIO)
        led0_backup.sioState = led0_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        led0_SIO_REG &= (uint32)(~led0_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: led0_Wakeup
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
*  Refer to led0_Sleep() for an example usage.
*******************************************************************************/
void led0_Wakeup(void)
{
    #if defined(led0__PC)
        led0_PC = led0_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            led0_USB_POWER_REG &= led0_USBIO_EXIT_SLEEP_PH1;
            led0_CR1_REG = led0_backup.usbState;
            led0_USB_POWER_REG &= led0_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(led0__SIO)
        led0_SIO_REG = led0_backup.sioState;
    #endif
}


/* [] END OF FILE */
