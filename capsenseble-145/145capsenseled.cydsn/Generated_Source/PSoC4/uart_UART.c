/*******************************************************************************
* File Name: uart_UART.c
* Version 3.20
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "uart_PVT.h"
#include "uart_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if(uart_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const uart_UART_INIT_STRUCT uart_configUart =
    {
        uart_UART_SUB_MODE,
        uart_UART_DIRECTION,
        uart_UART_DATA_BITS_NUM,
        uart_UART_PARITY_TYPE,
        uart_UART_STOP_BITS_NUM,
        uart_UART_OVS_FACTOR,
        uart_UART_IRDA_LOW_POWER,
        uart_UART_MEDIAN_FILTER_ENABLE,
        uart_UART_RETRY_ON_NACK,
        uart_UART_IRDA_POLARITY,
        uart_UART_DROP_ON_PARITY_ERR,
        uart_UART_DROP_ON_FRAME_ERR,
        uart_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        uart_UART_MP_MODE_ENABLE,
        uart_UART_MP_ACCEPT_ADDRESS,
        uart_UART_MP_RX_ADDRESS,
        uart_UART_MP_RX_ADDRESS_MASK,
        (uint32) uart_SCB_IRQ_INTERNAL,
        uart_UART_INTR_RX_MASK,
        uart_UART_RX_TRIGGER_LEVEL,
        uart_UART_INTR_TX_MASK,
        uart_UART_TX_TRIGGER_LEVEL,
        (uint8) uart_UART_BYTE_MODE_ENABLE,
        (uint8) uart_UART_CTS_ENABLE,
        (uint8) uart_UART_CTS_POLARITY,
        (uint8) uart_UART_RTS_POLARITY,
        (uint8) uart_UART_RTS_FIFO_LEVEL
    };


    /*******************************************************************************
    * Function Name: uart_UartInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the UART operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartInit(const uart_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (uart_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (uart_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

            /* Configure pins */
            uart_SetPins(uart_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            uart_scbMode       = (uint8) uart_SCB_MODE_UART;
            uart_scbEnableWake = (uint8) config->enableWake;
            uart_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            uart_rxBuffer      =         config->rxBuffer;
            uart_rxDataBits    = (uint8) config->dataBits;
            uart_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            uart_txBuffer      =         config->txBuffer;
            uart_txDataBits    = (uint8) config->dataBits;
            uart_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure UART interface */
            if(uart_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                uart_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (uart_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (uart_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                uart_CTRL_REG  = uart_GET_CTRL_OVS(config->oversample);
            }

            uart_CTRL_REG     |= uart_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             uart_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             uart_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            uart_UART_CTRL_REG = uart_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            uart_UART_RX_CTRL_REG = uart_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        uart_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        uart_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        uart_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        uart_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr);

            if(uart_UART_PARITY_NONE != config->parity)
            {
               uart_UART_RX_CTRL_REG |= uart_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    uart_UART_RX_CTRL_PARITY_ENABLED;
            }

            uart_RX_CTRL_REG      = uart_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                uart_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                uart_GET_UART_RX_CTRL_ENABLED(config->direction);

            uart_RX_FIFO_CTRL_REG = uart_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            uart_RX_MATCH_REG     = uart_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                uart_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            uart_UART_TX_CTRL_REG = uart_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                uart_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(uart_UART_PARITY_NONE != config->parity)
            {
               uart_UART_TX_CTRL_REG |= uart_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    uart_UART_TX_CTRL_PARITY_ENABLED;
            }

            uart_TX_CTRL_REG      = uart_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                uart_GET_UART_TX_CTRL_ENABLED(config->direction);

            uart_TX_FIFO_CTRL_REG = uart_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
            uart_UART_FLOW_CTRL_REG = uart_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            uart_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            uart_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            uart_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (uart_ISR_NUMBER);
            CyIntSetPriority(uart_ISR_NUMBER, uart_ISR_PRIORITY);
            (void) CyIntSetVector(uart_ISR_NUMBER, &uart_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(uart_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (uart_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(uart_RX_WAKE_ISR_NUMBER, uart_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(uart_RX_WAKE_ISR_NUMBER, &uart_UART_WAKEUP_ISR);
        #endif /* (uart_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            uart_INTR_I2C_EC_MASK_REG = uart_NO_INTR_SOURCES;
            uart_INTR_SPI_EC_MASK_REG = uart_NO_INTR_SOURCES;
            uart_INTR_SLAVE_MASK_REG  = uart_NO_INTR_SOURCES;
            uart_INTR_MASTER_MASK_REG = uart_NO_INTR_SOURCES;
            uart_INTR_RX_MASK_REG     = config->rxInterruptMask;
            uart_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Clear RX buffer indexes */
            uart_rxBufferHead     = 0u;
            uart_rxBufferTail     = 0u;
            uart_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            uart_txBufferHead = 0u;
            uart_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: uart_UartInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the UART operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartInit(void)
    {
        /* Configure UART interface */
        uart_CTRL_REG = uart_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        uart_UART_CTRL_REG = uart_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        uart_UART_RX_CTRL_REG = uart_UART_DEFAULT_UART_RX_CTRL;
        uart_RX_CTRL_REG      = uart_UART_DEFAULT_RX_CTRL;
        uart_RX_FIFO_CTRL_REG = uart_UART_DEFAULT_RX_FIFO_CTRL;
        uart_RX_MATCH_REG     = uart_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        uart_UART_TX_CTRL_REG = uart_UART_DEFAULT_UART_TX_CTRL;
        uart_TX_CTRL_REG      = uart_UART_DEFAULT_TX_CTRL;
        uart_TX_FIFO_CTRL_REG = uart_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
        uart_UART_FLOW_CTRL_REG = uart_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(uart_SCB_IRQ_INTERNAL)
        CyIntDisable    (uart_ISR_NUMBER);
        CyIntSetPriority(uart_ISR_NUMBER, uart_ISR_PRIORITY);
        (void) CyIntSetVector(uart_ISR_NUMBER, &uart_SPI_UART_ISR);
    #endif /* (uart_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(uart_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (uart_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(uart_RX_WAKE_ISR_NUMBER, uart_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(uart_RX_WAKE_ISR_NUMBER, &uart_UART_WAKEUP_ISR);
    #endif /* (uart_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        uart_INTR_I2C_EC_MASK_REG = uart_UART_DEFAULT_INTR_I2C_EC_MASK;
        uart_INTR_SPI_EC_MASK_REG = uart_UART_DEFAULT_INTR_SPI_EC_MASK;
        uart_INTR_SLAVE_MASK_REG  = uart_UART_DEFAULT_INTR_SLAVE_MASK;
        uart_INTR_MASTER_MASK_REG = uart_UART_DEFAULT_INTR_MASTER_MASK;
        uart_INTR_RX_MASK_REG     = uart_UART_DEFAULT_INTR_RX_MASK;
        uart_INTR_TX_MASK_REG     = uart_UART_DEFAULT_INTR_TX_MASK;

    #if(uart_INTERNAL_RX_SW_BUFFER_CONST)
        uart_rxBufferHead     = 0u;
        uart_rxBufferTail     = 0u;
        uart_rxBufferOverflow = 0u;
    #endif /* (uart_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(uart_INTERNAL_TX_SW_BUFFER_CONST)
        uart_txBufferHead = 0u;
        uart_txBufferTail = 0u;
    #endif /* (uart_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (uart_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: uart_UartPostEnable
********************************************************************************
*
* Summary:
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be 
*  controlled by the SCB UART.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uart_UartPostEnable(void)
{
#if (uart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (uart_TX_SDA_MISO_PIN)
        if (uart_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            uart_SET_HSIOM_SEL(uart_TX_SDA_MISO_HSIOM_REG, uart_TX_SDA_MISO_HSIOM_MASK,
                                           uart_TX_SDA_MISO_HSIOM_POS, uart_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (uart_TX_SDA_MISO_PIN_PIN) */

    #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
        #if (uart_RTS_SS0_PIN)
            if (uart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                uart_SET_HSIOM_SEL(uart_RTS_SS0_HSIOM_REG, uart_RTS_SS0_HSIOM_MASK,
                                               uart_RTS_SS0_HSIOM_POS, uart_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (uart_RTS_SS0_PIN) */
    #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */
    
#else    
    #if (uart_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        uart_SET_HSIOM_SEL(uart_TX_HSIOM_REG, uart_TX_HSIOM_MASK,
                                       uart_TX_HSIOM_POS, uart_TX_HSIOM_SEL_UART);
    #endif /* (uart_UART_TX_PIN) */

    #if (uart_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        uart_SET_HSIOM_SEL(uart_RTS_HSIOM_REG, uart_RTS_HSIOM_MASK,
                                       uart_RTS_HSIOM_POS, uart_RTS_HSIOM_SEL_UART);
    #endif /* (uart_UART_RTS_PIN) */

#endif /* (uart_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: uart_UartStop
********************************************************************************
*
* Summary:
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature to
*  not cause it to trigger after the component is enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uart_UartStop(void)
{
#if(uart_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (uart_TX_SDA_MISO_PIN)
        if (uart_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            uart_SET_HSIOM_SEL(uart_TX_SDA_MISO_HSIOM_REG, uart_TX_SDA_MISO_HSIOM_MASK,
                                           uart_TX_SDA_MISO_HSIOM_POS, uart_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (uart_TX_SDA_MISO_PIN_PIN) */

    #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
        #if (uart_RTS_SS0_PIN)
            if (uart_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                uart_uart_rts_spi_ss0_Write(uart_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                uart_SET_HSIOM_SEL(uart_RTS_SS0_HSIOM_REG, uart_RTS_SS0_HSIOM_MASK,
                                               uart_RTS_SS0_HSIOM_POS, uart_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (uart_RTS_SS0_PIN) */
    #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

#else
    #if (uart_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        uart_SET_HSIOM_SEL(uart_TX_HSIOM_REG, uart_TX_HSIOM_MASK,
                                       uart_TX_HSIOM_POS, uart_TX_HSIOM_SEL_GPIO);
    #endif /* (uart_UART_TX_PIN) */

    #if (uart_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        uart_rts_Write(uart_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        uart_SET_HSIOM_SEL(uart_RTS_HSIOM_REG, uart_RTS_HSIOM_MASK,
                                       uart_RTS_HSIOM_POS, uart_RTS_HSIOM_SEL_GPIO);
    #endif /* (uart_UART_RTS_PIN) */

#endif /* (uart_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (uart_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    uart_UART_RX_CTRL_REG &= (uint32) ~uart_UART_RX_CTRL_SKIP_START;
#endif /* (uart_UART_WAKE_ENABLE_CONST) */
}


/*******************************************************************************
* Function Name: uart_UartSetRxAddress
********************************************************************************
*
* Summary:
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
* Parameters:
*  address: Address for hardware address detection.
*
* Return:
*  None
*
*******************************************************************************/
void uart_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = uart_RX_MATCH_REG;

    matchReg &= ((uint32) ~uart_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & uart_RX_MATCH_ADDR_MASK)); /* Set address  */

    uart_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: uart_UartSetRxAddressMask
********************************************************************************
*
* Summary:
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
* Parameters:
*  addressMask: Address mask.
*   0 - address bit does not care while comparison.
*   1 - address bit is significant while comparison.
*
* Return:
*  None
*
*******************************************************************************/
void uart_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = uart_RX_MATCH_REG;

    matchReg &= ((uint32) ~uart_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << uart_RX_MATCH_MASK_POS));

    uart_RX_MATCH_REG = matchReg;
}


#if(uart_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_UartGetChar
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer.
    *  This function is designed for ASCII characters and returns a char
    *  where 1 to 255 are valid characters and 0 indicates an error occurred or
    *  no data present.
    *  - The RX software buffer is disabled: returns the data element
    *    retrieved from the RX FIFO.
    *    Undefined data will be returned if the RX FIFO is empty.
    *  - The RX software buffer is enabled: returns the data element from
    *    the software receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The next data element from the receive buffer.
    *  ASCII character values from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Side Effects:
    *  The errors bits may not correspond with reading characters due to RX FIFO
    *  and software buffer usage.
    *  RX software buffer is enabled: The internal software buffer overflow
    *  does not treat as an error condition.
    *  Check SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 uart_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != uart_SpiUartGetRxBufferSize())
        {
            rxData = uart_SpiUartReadRxData();
        }

        if (uart_CHECK_INTR_RX(uart_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            uart_ClearRxInterruptSource(uart_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: uart_UartGetByte
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Bits 7-0 contain the next data element from the receive buffer and
    *  other bits contain the error condition.
    *
    * Side Effects:
    *  The errors bits may not correspond with reading characters due to RX FIFO
    *  and software buffer usage.
    *  RX software buffer is disabled: The internal software buffer overflow
    *  is not returned as status by this function.
    *  Check SCB_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 uart_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (uart_CHECK_RX_SW_BUFFER)
        {
            uart_DisableInt();
        }
        #endif

        if (0u != uart_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (uart_CHECK_RX_SW_BUFFER)
            {            
                uart_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = uart_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the case
            * of empty. Otherwise the first received byte will be read.
            */
            rxData = uart_RX_FIFO_RD_REG;

            /* Enables interrupt to receive more bytes.
            * The RX_NOT_EMPTY interrupt is cleared by the interrupt routine
            * in case the byte was received and read by code above.
            */
            #if (uart_CHECK_RX_SW_BUFFER)
            {
                uart_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (uart_GetRxInterruptSource() & uart_INTR_RX_ERR);
        uart_ClearRxInterruptSource(uart_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: uart_UartSetRtsPolarity
        ********************************************************************************
        *
        * Summary:
        *  Sets active polarity of RTS output signal.
        *
        * Parameters:
        *  polarity: Active polarity of RTS output signal.
        *   uart_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   uart_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void uart_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                uart_UART_FLOW_CTRL_REG |= (uint32)  uart_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                uart_UART_FLOW_CTRL_REG &= (uint32) ~uart_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: uart_UartSetRtsFifoLevel
        ********************************************************************************
        *
        * Summary:
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *
        * Parameters:
        *  level: Level in the RX FIFO for RTS signal activation.
        *         The range of valid level values is between 0 and RX FIFO depth - 1.
        *         Setting level value to 0 disables RTS signal activation.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void uart_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = uart_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~uart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (uart_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            uart_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

#endif /* (uart_UART_RX_DIRECTION) */


#if(uart_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: uart_UartPutString
    ********************************************************************************
    *
    * Summary:
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is space available to put
    *  all the requested data into the  transmit buffer.
    *
    * Parameters:
    *  string: pointer to the null terminated string array to be placed in the
    *          transmit buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            uart_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: uart_UartPutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data followed by a carriage return (0x0D) and
    *  line feed (0x0A) into the transmit buffer.
    *  This function is blocking and waits until there is space available to put
    *  all the requested data into the  transmit buffer.
    *
    * Parameters:
    *  txDataByte : the data to be transmitted.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartPutCRLF(uint32 txDataByte)
    {
        uart_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        uart_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        uart_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: uartSCB_UartEnableCts
        ********************************************************************************
        *
        * Summary:
        *  Enables usage of CTS input signal by the UART transmitter.
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void uart_UartEnableCts(void)
        {
            uart_UART_FLOW_CTRL_REG |= (uint32)  uart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: uart_UartDisableCts
        ********************************************************************************
        *
        * Summary:
        *  Disables usage of CTS input signal by the UART transmitter.
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void uart_UartDisableCts(void)
        {
            uart_UART_FLOW_CTRL_REG &= (uint32) ~uart_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: uart_UartSetCtsPolarity
        ********************************************************************************
        *
        * Summary:
        *  Sets active polarity of CTS input signal.
        *
        * Parameters:
        *  polarity: Active polarity of CTS output signal.
        *   uart_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   uart_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void uart_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                uart_UART_FLOW_CTRL_REG |= (uint32)  uart_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                uart_UART_FLOW_CTRL_REG &= (uint32) ~uart_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(uart_CY_SCBIP_V0 || uart_CY_SCBIP_V1) */

#endif /* (uart_UART_TX_DIRECTION) */


#if(uart_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: uart_UartSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Clears and enables interrupt on a falling edge of the Rx input. The GPIO
    *  event wakes up the device and SKIP_START feature allows the UART continue
    *  receiving data bytes properly. The GPIO interrupt does not track in the
    *  active mode therefore requires to be cleared by this API.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartSaveConfig(void)
    {
        /* Clear interrupt activity:
        *  - set skip start and disable RX. At GPIO wakeup RX will be enabled.
        *  - clear rx_wake interrupt source as it triggers during normal operation.
        *  - clear wake interrupt pending state as it becomes pending in active mode.
        */

        uart_UART_RX_CTRL_REG |= uart_UART_RX_CTRL_SKIP_START;

    #if(uart_SCB_MODE_UNCONFIG_CONST_CFG)
        #if(uart_MOSI_SCL_RX_WAKE_PIN)
            (void) uart_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt();
        #endif /* (uart_MOSI_SCL_RX_WAKE_PIN) */
    #else
        #if(uart_UART_RX_WAKE_PIN)
            (void) uart_rx_wake_ClearInterrupt();
        #endif /* (uart_UART_RX_WAKE_PIN) */
    #endif /* (uart_SCB_MODE_UNCONFIG_CONST_CFG) */

    #if(uart_UART_RX_WAKEUP_IRQ)
        uart_RxWakeClearPendingInt();
        uart_RxWakeEnableInt();
    #endif /* (uart_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: uart_UartRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void uart_UartRestoreConfig(void)
    {
    /* Disable RX GPIO interrupt: no more triggers in active mode */
    #if(uart_UART_RX_WAKEUP_IRQ)
        uart_RxWakeDisableInt();
    #endif /* (uart_UART_RX_WAKEUP_IRQ) */
    }
#endif /* (uart_UART_WAKE_ENABLE_CONST) */


#if(uart_UART_RX_WAKEUP_IRQ)
    /*******************************************************************************
    * Function Name: uart_UART_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
    *  event. This event is configured to trigger on a falling edge of the RX line.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    CY_ISR(uart_UART_WAKEUP_ISR)
    {
    #ifdef uart_UART_WAKEUP_ISR_ENTRY_CALLBACK
        uart_UART_WAKEUP_ISR_EntryCallback();
    #endif /* uart_UART_WAKEUP_ISR_ENTRY_CALLBACK */

        /* Clear interrupt source: the event becomes multi triggered and is
        * only disabled by uart_UartRestoreConfig() call.
        */
    #if(uart_SCB_MODE_UNCONFIG_CONST_CFG)
        #if(uart_MOSI_SCL_RX_WAKE_PIN)
            (void) uart_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt();
        #endif /* (uart_MOSI_SCL_RX_WAKE_PIN) */
    #else
        #if(uart_UART_RX_WAKE_PIN)
            (void) uart_rx_wake_ClearInterrupt();
        #endif /* (uart_UART_RX_WAKE_PIN) */
    #endif /* (uart_SCB_MODE_UNCONFIG_CONST_CFG) */

    #ifdef uart_UART_WAKEUP_ISR_EXIT_CALLBACK
        uart_UART_WAKEUP_ISR_ExitCallback();
    #endif /* uart_UART_WAKEUP_ISR_EXIT_CALLBACK */
    }
#endif /* (uart_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
