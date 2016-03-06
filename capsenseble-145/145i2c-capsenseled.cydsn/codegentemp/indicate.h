/*******************************************************************************
* File Name: indicate.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_indicate_H) /* Pins indicate_H */
#define CY_PINS_indicate_H

#include "cytypes.h"
#include "cyfitter.h"
#include "indicate_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} indicate_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   indicate_Read(void);
void    indicate_Write(uint8 value);
uint8   indicate_ReadDataReg(void);
#if defined(indicate__PC) || (CY_PSOC4_4200L) 
    void    indicate_SetDriveMode(uint8 mode);
#endif
void    indicate_SetInterruptMode(uint16 position, uint16 mode);
uint8   indicate_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void indicate_Sleep(void); 
void indicate_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(indicate__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define indicate_DRIVE_MODE_BITS        (3)
    #define indicate_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - indicate_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the indicate_SetDriveMode() function.
         *  @{
         */
        #define indicate_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define indicate_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define indicate_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define indicate_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define indicate_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define indicate_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define indicate_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define indicate_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define indicate_MASK               indicate__MASK
#define indicate_SHIFT              indicate__SHIFT
#define indicate_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in indicate_SetInterruptMode() function.
     *  @{
     */
        #define indicate_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define indicate_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define indicate_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define indicate_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(indicate__SIO)
    #define indicate_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(indicate__PC) && (CY_PSOC4_4200L)
    #define indicate_USBIO_ENABLE               ((uint32)0x80000000u)
    #define indicate_USBIO_DISABLE              ((uint32)(~indicate_USBIO_ENABLE))
    #define indicate_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define indicate_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define indicate_USBIO_ENTER_SLEEP          ((uint32)((1u << indicate_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << indicate_USBIO_SUSPEND_DEL_SHIFT)))
    #define indicate_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << indicate_USBIO_SUSPEND_SHIFT)))
    #define indicate_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << indicate_USBIO_SUSPEND_DEL_SHIFT)))
    #define indicate_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(indicate__PC)
    /* Port Configuration */
    #define indicate_PC                 (* (reg32 *) indicate__PC)
#endif
/* Pin State */
#define indicate_PS                     (* (reg32 *) indicate__PS)
/* Data Register */
#define indicate_DR                     (* (reg32 *) indicate__DR)
/* Input Buffer Disable Override */
#define indicate_INP_DIS                (* (reg32 *) indicate__PC2)

/* Interrupt configuration Registers */
#define indicate_INTCFG                 (* (reg32 *) indicate__INTCFG)
#define indicate_INTSTAT                (* (reg32 *) indicate__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define indicate_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(indicate__SIO)
    #define indicate_SIO_REG            (* (reg32 *) indicate__SIO)
#endif /* (indicate__SIO_CFG) */

/* USBIO registers */
#if !defined(indicate__PC) && (CY_PSOC4_4200L)
    #define indicate_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define indicate_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define indicate_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define indicate_DRIVE_MODE_SHIFT       (0x00u)
#define indicate_DRIVE_MODE_MASK        (0x07u << indicate_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins indicate_H */


/* [] END OF FILE */
