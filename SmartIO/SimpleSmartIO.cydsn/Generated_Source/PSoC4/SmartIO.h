/*******************************************************************************
* \file SmartIO.h
* \version 1.0
*
* \brief
*  This file provides constants and parameter values for the SmartIO Component.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SMARTIO_SmartIO_H)
#define CY_SMARTIO_SmartIO_H

#include <cydevice_trm.h>
#include <cyfitter.h>
#include <cytypes.h>


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_functions
* @{
*/
void SmartIO_Init(void);
void SmartIO_Start(void);
void SmartIO_Stop(void);
void SmartIO_SetBypass(uint32 portChannel);
void SmartIO_ClockSelect(uint32 clockSrc);
void SmartIO_HoldOverride(uint32 ovCtrl);
void SmartIO_IoSyncMode(uint32 portTerm);
void SmartIO_ChipSyncMode(uint32 portTerm);
cystatus SmartIO_LUT_SelectInputs(uint32 lutNum, uint32 inputNum, uint32 inputSrc);
void SmartIO_LUT_ConfigureMode(uint32 lutNum, uint32 mapping, uint32 mode);
/** @} functions */


/***************************************
*        API Constants
***************************************/
/**
* \addtogroup group_constants
* @{
*/
/** \addtogroup channelConst Port channel selection constants
 * \brief Constants to be passed as "portChannel" parameter in SmartIO_SetBypass() function.
 *  @{
 */
#define SmartIO_CHANNEL_NONE    0x00UL /**< \brief Do not bypass any channels  */
#define SmartIO_CHANNEL0        0x01UL /**< \brief Channel 0 (data0 <-> gpio0) */
#define SmartIO_CHANNEL1        0x02UL /**< \brief Channel 1 (data1 <-> gpio1) */
#define SmartIO_CHANNEL2        0x04UL /**< \brief Channel 2 (data2 <-> gpio2) */
#define SmartIO_CHANNEL3        0x08UL /**< \brief Channel 3 (data3 <-> gpio3) */
#define SmartIO_CHANNEL4        0x10UL /**< \brief Channel 4 (data4 <-> gpio4) */
#define SmartIO_CHANNEL5        0x20UL /**< \brief Channel 5 (data5 <-> gpio5) */
#define SmartIO_CHANNEL6        0x40UL /**< \brief Channel 6 (data6 <-> gpio6) */
#define SmartIO_CHANNEL7        0x80UL /**< \brief Channel 7 (data7 <-> gpio7) */
#define SmartIO_CHANNEL_ALL     0xffUL /**< \brief Bypass all channels         */
/** @} channelConst */

/** \addtogroup clockConst Component clock selection constants
 * \brief Constants to be passed as "clockSrc" parameter in SmartIO_ClockSelect() function.
 *  @{
 */
#define SmartIO_CLK_GPIO0       0UL  /**< \brief Clock sourced from signal on gpio0 */
#define SmartIO_CLK_GPIO1       1UL  /**< \brief Clock sourced from signal on gpio1 */
#define SmartIO_CLK_GPIO2       2UL  /**< \brief Clock sourced from signal on gpio2 */
#define SmartIO_CLK_GPIO3       3UL  /**< \brief Clock sourced from signal on gpio3 */
#define SmartIO_CLK_GPIO4       4UL  /**< \brief Clock sourced from signal on gpio4 */
#define SmartIO_CLK_GPIO5       5UL  /**< \brief Clock sourced from signal on gpio5 */
#define SmartIO_CLK_GPIO6       6UL  /**< \brief Clock sourced from signal on gpio6 */
#define SmartIO_CLK_GPIO7       7UL  /**< \brief Clock sourced from signal on gpio7 */
#define SmartIO_CLK_DATA0       8UL  /**< \brief Clock sourced from signal on data0 */
#define SmartIO_CLK_DATA1       9UL  /**< \brief Clock sourced from signal on data1 */
#define SmartIO_CLK_DATA2       10UL /**< \brief Clock sourced from signal on data2 */
#define SmartIO_CLK_DATA3       11UL /**< \brief Clock sourced from signal on data3 */
#define SmartIO_CLK_DATA4       12UL /**< \brief Clock sourced from signal on data4 */
#define SmartIO_CLK_DATA5       13UL /**< \brief Clock sourced from signal on data5 */
#define SmartIO_CLK_DATA6       14UL /**< \brief Clock sourced from signal on data6 */
#define SmartIO_CLK_DATA7       15UL /**< \brief Clock sourced from signal on data7 */
#define SmartIO_DIV_CLK_ACT     16UL /**< \brief Clock sourced from a divided clock (Active) */
#define SmartIO_DIV_CLK_DS      17UL /**< \brief Clock sourced from a divided clock (Deep-Sleep) */
#define SmartIO_DIV_CLK_HIB     18UL /**< \brief Clock sourced from a divided clock (Hibernate) */
#define SmartIO_LFCLK           19UL /**< \brief Clock sourced from LFCLK */
#define SmartIO_CLK_GATED       20UL /**< \brief Disables the clock connection. Used when turning off the block */
#define SmartIO_ASYNC           31UL /**< \brief Asynchronous operation */
/** @} clockConst */

/** \addtogroup hldOvrConst Component hold override selection constants
 * \brief Constants to be passed as "ovCtrl" parameter in SmartIO_HoldOverride() function.
 *  @{
 */
#define SmartIO_OVCTRL_DISABLE  0UL /**< \brief Controlled by HSIOM  */
#define SmartIO_OVCTRL_ENABLE   1UL /**< \brief Controlled by SmartIO */
/** @} hldOvrConst */

/** \addtogroup termConst Terminal selection constants
 * \brief Constants to be passed as "portTerm" parameter in SmartIO_IoSyncMode() and 
 *  SmartIO_ChipSyncMode() functions.
 *  @{
 */
#define SmartIO_TERM_NONE       0x00UL /**< \brief No synchronization for all data/gpio     */
#define SmartIO_TERM0           0x01UL /**< \brief Enable synchronization for data0/gpio0   */
#define SmartIO_TERM1           0x02UL /**< \brief Enable synchronization for data1/gpio1   */
#define SmartIO_TERM2           0x04UL /**< \brief Enable synchronization for data2/gpio2   */
#define SmartIO_TERM3           0x08UL /**< \brief Enable synchronization for data3/gpio3   */
#define SmartIO_TERM4           0x10UL /**< \brief Enable synchronization for data4/gpio4   */
#define SmartIO_TERM5           0x20UL /**< \brief Enable synchronization for data5/gpio5   */
#define SmartIO_TERM6           0x40UL /**< \brief Enable synchronization for data6/gpio6   */
#define SmartIO_TERM7           0x80UL /**< \brief Enable synchronization for data7/gpio7   */
#define SmartIO_TERM_ALL        0xffUL /**< \brief Enable sycnhronization for all data/gpio */
/** @} termConst */

/** \addtogroup lutNumConst Look-up table number constants
 * \brief Constants to be passed as "lutNum" parameter in SmartIO_LUT_SelectInputs() and SmartIO_LUT_ConfigureMode() functions.
 *  @{
 */
#define SmartIO_LUT0            0UL /**< \brief LUT number 0  */
#define SmartIO_LUT1            1UL /**< \brief LUT number 1  */
#define SmartIO_LUT2            2UL /**< \brief LUT number 2  */
#define SmartIO_LUT3            3UL /**< \brief LUT number 3  */
#define SmartIO_LUT4            4UL /**< \brief LUT number 4  */
#define SmartIO_LUT5            5UL /**< \brief LUT number 5  */
#define SmartIO_LUT6            6UL /**< \brief LUT number 6  */
#define SmartIO_LUT7            7UL /**< \brief LUT number 7  */
/** @} lutNumConst */

/** \addtogroup inputNumConst LUT input number constants
 * \brief Constants to be passed as "inputNum" parameter in SmartIO_LUT_SelectInputs() function.
 *  @{
 */
#define SmartIO_LUT_INPUT0      0x01UL /**< \brief LUT input terminal 0    */
#define SmartIO_LUT_INPUT1      0x02UL /**< \brief LUT input terminal 1    */
#define SmartIO_LUT_INPUT2      0x04UL /**< \brief LUT input terminal 2    */
#define SmartIO_LUT_INPUT_ALL   0x07UL /**< \brief All LUT input terminals */
/** @} inputNumConst */

/** \addtogroup inputSrcConst LUT input source constants
 * \brief Constants to be passed as "inputSrc" parameter in SmartIO_LUT_SelectInputs() function.
 *  @{
 */
#define SmartIO_SRC_LUT0        0UL  /**< \brief Source is LUT0 output    */
#define SmartIO_SRC_LUT1        1UL  /**< \brief Source is LUT1 output    */
#define SmartIO_SRC_LUT2        2UL  /**< \brief Source is LUT2 output    */
#define SmartIO_SRC_LUT3        3UL  /**< \brief Source is LUT3 output    */
#define SmartIO_SRC_LUT4        4UL  /**< \brief Source is LUT4 output    */
#define SmartIO_SRC_LUT5        5UL  /**< \brief Source is LUT5 output    */
#define SmartIO_SRC_LUT6        6UL  /**< \brief Source is LUT6 output    */
#define SmartIO_SRC_LUT7        7UL  /**< \brief Source is LUT7 output    */
#define SmartIO_SRC_DATA_04     8UL  /**< \brief Source is data0/data4    */
#define SmartIO_SRC_DATA_15     9UL  /**< \brief Source is data1/data5    */
#define SmartIO_SRC_DATA_26     10UL /**< \brief Source is data2/data6    */
#define SmartIO_SRC_DATA_37     11UL /**< \brief Source is data3/data7    */
#define SmartIO_SRC_GPIO_04     12UL /**< \brief Source is gpio0/gpio4    */
#define SmartIO_SRC_GPIO_15     13UL /**< \brief Source is gpio1/gpio5    */
#define SmartIO_SRC_GPIO_26     14UL /**< \brief Source is gpio2/gpio6    */
#define SmartIO_SRC_GPIO_37     15UL /**< \brief Source is gpio3/gpio7    */
/** @} inputSrcConst */

/** \addtogroup modeConst LUT mode constants
 * \brief Constants to be passed as "mode" parameter in SmartIO_LUT_ConfigureMode() function.
 *  @{
 */
#define SmartIO_MODE_COMB       0UL /**< \brief Combinatorial mode      */
#define SmartIO_MODE_REGIN      1UL /**< \brief Registered input mode   */
#define SmartIO_MODE_REGOUT     2UL /**< \brief Registered output mode  */
#define SmartIO_MODE_SRFF       3UL /**< \brief S/R Flip-Flop mode      */
/** @} modeConst */

/** @} group_constants */


/***************************************
*       Global Variables
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8 SmartIO_initVar; /**< Initialization state variable */
/** @} globals */


/***************************************
*    Enumerated Types and Parameters
***************************************/

/* Number of channels */
#define SmartIO_CHANNELS       (8u)

/**
* \addtogroup group_structs
* @{
*/
typedef struct
{
    uint32 lutSel[SmartIO_CHANNELS]; /**< LUT input selection configuration */
	uint32 lutCtl[SmartIO_CHANNELS]; /**< LUT mode and mapping configuration */
} SmartIO_lut_config_struct;
/** @} structs */


/***************************************
*    Initial Parameter Constants
***************************************/
    
/* Global control */
#define SmartIO_BYPASS0         1UL
#define SmartIO_BYPASS1         1UL
#define SmartIO_BYPASS2         1UL
#define SmartIO_BYPASS3         1UL
#define SmartIO_BYPASS4         0UL
#define SmartIO_BYPASS5         0UL
#define SmartIO_BYPASS6         1UL
#define SmartIO_BYPASS7         1UL
#define SmartIO_CLOCK_SELECT    31UL
#define SmartIO_HLD_OVR         0UL

/* IO Synchronization control */
#define SmartIO_IO_SYNC0        0UL
#define SmartIO_IO_SYNC1        0UL
#define SmartIO_IO_SYNC2        0UL
#define SmartIO_IO_SYNC3        0UL
#define SmartIO_IO_SYNC4        0UL
#define SmartIO_IO_SYNC5        0UL
#define SmartIO_IO_SYNC6        0UL
#define SmartIO_IO_SYNC7        0UL

/* Chip Data Synchronization control */
#define SmartIO_DATA_SYNC0      0UL
#define SmartIO_DATA_SYNC1      0UL
#define SmartIO_DATA_SYNC2      0UL
#define SmartIO_DATA_SYNC3      0UL
#define SmartIO_DATA_SYNC4      0UL
#define SmartIO_DATA_SYNC5      0UL
#define SmartIO_DATA_SYNC6      0UL
#define SmartIO_DATA_SYNC7      0UL

/* LUT input selection */
#define SmartIO_LUT0_TR0        17UL
#define SmartIO_LUT0_TR1        17UL
#define SmartIO_LUT0_TR2        17UL
#define SmartIO_LUT1_TR0        17UL
#define SmartIO_LUT1_TR1        17UL
#define SmartIO_LUT1_TR2        17UL
#define SmartIO_LUT2_TR0        17UL
#define SmartIO_LUT2_TR1        17UL
#define SmartIO_LUT2_TR2        17UL
#define SmartIO_LUT3_TR0        17UL
#define SmartIO_LUT3_TR1        17UL
#define SmartIO_LUT3_TR2        17UL
#define SmartIO_LUT4_TR0        17UL
#define SmartIO_LUT4_TR1        17UL
#define SmartIO_LUT4_TR2        17UL
#define SmartIO_LUT5_TR0        8UL
#define SmartIO_LUT5_TR1        8UL
#define SmartIO_LUT5_TR2        8UL
#define SmartIO_LUT6_TR0        17UL
#define SmartIO_LUT6_TR1        17UL
#define SmartIO_LUT6_TR2        17UL
#define SmartIO_LUT7_TR0        17UL
#define SmartIO_LUT7_TR1        17UL
#define SmartIO_LUT7_TR2        17UL

/* LUT truth table */
#define SmartIO_LUT0_MAP        0UL
#define SmartIO_LUT1_MAP        0UL
#define SmartIO_LUT2_MAP        0UL
#define SmartIO_LUT3_MAP        0UL
#define SmartIO_LUT4_MAP        0UL
#define SmartIO_LUT5_MAP        128UL
#define SmartIO_LUT6_MAP        0UL
#define SmartIO_LUT7_MAP        0UL

/* LUT mode */
#define SmartIO_LUT0_MODE       0UL
#define SmartIO_LUT1_MODE       0UL
#define SmartIO_LUT2_MODE       0UL
#define SmartIO_LUT3_MODE       0UL
#define SmartIO_LUT4_MODE       0UL
#define SmartIO_LUT5_MODE       0UL
#define SmartIO_LUT6_MODE       0UL
#define SmartIO_LUT7_MODE       0UL


/***************************************
*        Registers
***************************************/

/* PRGIO global control register */
#define SmartIO_CTL                (* (reg32 *) SmartIO_cy_m0s8_prgio__CTL)

/* IO and chip data synchronization control */
#define SmartIO_SYNC_CTL           (* (reg32 *) SmartIO_cy_m0s8_prgio__SYNC_CTL)

/* LUT input selection and control registers */
#define SmartIO_LUT_CONGIG_BASE    (*(volatile SmartIO_lut_config_struct *) SmartIO_cy_m0s8_prgio__LUT_SEL0)

/* Data unit registers */
#define SmartIO_DU_SEL             (* (reg32 *) SmartIO_cy_m0s8_prgio__DU_SEL)
#define SmartIO_DU_CTL             (* (reg32 *) SmartIO_cy_m0s8_prgio__DU_CTL)
#define SmartIO_DATA               (* (reg32 *) SmartIO_cy_m0s8_prgio__DATA)


/***************************************
*        Register Constants
***************************************/

/* Channel number */
#define SmartIO_CH0                       (0u)
#define SmartIO_CH1                       (1u)
#define SmartIO_CH2                       (2u)
#define SmartIO_CH3                       (3u)
#define SmartIO_CH4                       (4u)
#define SmartIO_CH5                       (5u)
#define SmartIO_CH6                       (6u)
#define SmartIO_CH7                       (7u)

/* Register set to all zeros/ones */
#define SmartIO_REG_ZEROS                 ((uint32)0x00000000u)
#define SmartIO_REG_ONES                  ((uint32)0xffffffffu)

/* SmartIO Fabric Enable/Disable */
#define SmartIO_FABRIC_ENABLE             ((uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_ENABLED__OFFSET))
#define SmartIO_FABRIC_DISABLE            ((uint32)(~(uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_ENABLED__OFFSET)))
#define SmartIO_FABRIC_MASK               ((uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_ENABLED__OFFSET))

/* Bypass Mask */
#define SmartIO_BYPASS_MASK               ((uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_BYPASS__OFFSET))
#define SmartIO_BYPASS_CLEAR              ((uint32)(~(uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_BYPASS__OFFSET)))

/* Clock Source Const 0, clear, mask */
#define SmartIO_CLOCK_CONST_ZERO          ((uint32)((uint32)SmartIO_CLK_GATED << CYFLD_PRGIO_PRT_CLOCK_SRC__OFFSET))
#define SmartIO_CLOCK_CLEAR               ((uint32)(~(uint32)((uint32)0x1fu << CYFLD_PRGIO_PRT_CLOCK_SRC__OFFSET)))
#define SmartIO_CLOCK_MASK                ((uint32)((uint32)0x1fu << CYFLD_PRGIO_PRT_CLOCK_SRC__OFFSET))

/* Hold override HSIOM/SmartIO */
#define SmartIO_HLD_OVR_SMARTIO           ((uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_HLD_OVR__OFFSET))
#define SmartIO_HLD_OVR_HSIOM             ((uint32)(~(uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_HLD_OVR__OFFSET)))

/* Pipeline Enable/Disable */
#define SmartIO_PIPELINE_ENABLE           ((uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_PIPELINE_EN__OFFSET))
#define SmartIO_PIPELINE_DISABLE          ((uint32)(~(uint32)((uint32)0x01u << CYFLD_PRGIO_PRT_PIPELINE_EN__OFFSET)))

/* Sync control clear */
#define SmartIO_IO_SYNC_MASK              ((uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_IO_SYNC_EN__OFFSET))
#define SmartIO_DATA_SYNC_MASK            ((uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_CHIP_SYNC_EN__OFFSET))
#define SmartIO_IO_SYNC_CLEAR             ((uint32)(~(uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_IO_SYNC_EN__OFFSET)))
#define SmartIO_DATA_SYNC_CLEAR           ((uint32)(~(uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_CHIP_SYNC_EN__OFFSET)))

/* LUT inputs clear */
#define SmartIO_LUT_TR0_CLEAR             ((uint32)(~(uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET)))
#define SmartIO_LUT_TR1_CLEAR             ((uint32)(~(uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET)))
#define SmartIO_LUT_TR2_CLEAR             ((uint32)(~(uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET)))
#define SmartIO_LUT_TR_CLEAR_ALL          (SmartIO_LUT_TR0_CLEAR & SmartIO_LUT_TR1_CLEAR & SmartIO_LUT_TR2_CLEAR)
#define SmartIO_LUT_TR0_MASK              ((uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_LUT_TR1_MASK              ((uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_LUT_TR2_MASK              ((uint32)((uint32)0x0fu << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))

/* LUT config clear */
#define SmartIO_LUT_MAP_CLEAR             ((uint32)(~(uint32)((uint32)0xffu << CYFLD_PRGIO_PRT_LUT__OFFSET)))
#define SmartIO_LUT_OPC_CLEAR             ((uint32)(~(uint32)((uint32)0x03u << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET)))
#define SmartIO_LUT_CTL_CLEAR             ((uint32)(SmartIO_LUT_MAP_CLEAR & SmartIO_LUT_OPC_CLEAR))
#define SmartIO_LUT_MAP_MASK              (((uint32)0xffu << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_LUT_OPC_MASK              (((uint32)0x03u << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))


/***************************************
*        Initial configuration
***************************************/

/* Bypass configuration */
#define SmartIO_INIT_BYPASS              ((uint32)(((SmartIO_BYPASS0)       | \
                                                     (SmartIO_BYPASS1 << SmartIO_CH1) | \
                                                     (SmartIO_BYPASS2 << SmartIO_CH2) | \
                                                     (SmartIO_BYPASS3 << SmartIO_CH3) | \
                                                     (SmartIO_BYPASS4 << SmartIO_CH4) | \
                                                     (SmartIO_BYPASS5 << SmartIO_CH5) | \
                                                     (SmartIO_BYPASS6 << SmartIO_CH6) | \
                                                     (SmartIO_BYPASS7 << SmartIO_CH7))  \
                                                     << CYFLD_PRGIO_PRT_BYPASS__OFFSET))
/* Clock configuration */
#define SmartIO_INIT_CLOCK_SRC           ((uint32)(SmartIO_CLOCK_SELECT << CYFLD_PRGIO_PRT_CLOCK_SRC__OFFSET))
/* Hold override configuration */
#define SmartIO_INIT_HLD_OVR             ((uint32)(SmartIO_HLD_OVR << CYFLD_PRGIO_PRT_HLD_OVR__OFFSET))
/* Disable the pipeline */
#define SmartIO_INIT_PIPELINE_EN         ((uint32)((uint32)0x00u << CYFLD_PRGIO_PRT_PIPELINE_EN__OFFSET))
/* Initial general control configuration */
#define SmartIO_INIT_CTL_CONFIG          ((uint32)(SmartIO_INIT_PIPELINE_EN | \
                                                    SmartIO_INIT_HLD_OVR     | \
                                                    SmartIO_INIT_CLOCK_SRC   | \
                                                    SmartIO_INIT_BYPASS))

/* IO Sync configuration */
#define SmartIO_INIT_IO_SYNC             ((uint32)(((SmartIO_IO_SYNC0)       | \
                                                     (SmartIO_IO_SYNC1 << SmartIO_CH1) | \
                                                     (SmartIO_IO_SYNC2 << SmartIO_CH2) | \
                                                     (SmartIO_IO_SYNC3 << SmartIO_CH3) | \
                                                     (SmartIO_IO_SYNC4 << SmartIO_CH4) | \
                                                     (SmartIO_IO_SYNC5 << SmartIO_CH5) | \
                                                     (SmartIO_IO_SYNC6 << SmartIO_CH6) | \
                                                     (SmartIO_IO_SYNC7 << SmartIO_CH7))  \
                                                     << CYFLD_PRGIO_PRT_IO_SYNC_EN__OFFSET))
/* Chip data Sync configuration */
#define SmartIO_INIT_DATA_SYNC           ((uint32)(((SmartIO_DATA_SYNC0)       | \
                                                     (SmartIO_DATA_SYNC1 << SmartIO_CH1) | \
                                                     (SmartIO_DATA_SYNC2 << SmartIO_CH2) | \
                                                     (SmartIO_DATA_SYNC3 << SmartIO_CH3) | \
                                                     (SmartIO_DATA_SYNC4 << SmartIO_CH4) | \
                                                     (SmartIO_DATA_SYNC5 << SmartIO_CH5) | \
                                                     (SmartIO_DATA_SYNC6 << SmartIO_CH6) | \
                                                     (SmartIO_DATA_SYNC7 << SmartIO_CH7))  \
                                                     << CYFLD_PRGIO_PRT_CHIP_SYNC_EN__OFFSET))
/* Initial synchronization configuration */
#define SmartIO_INIT_SYNC_CONFIG         ((uint32)(SmartIO_INIT_DATA_SYNC | \
                                                     SmartIO_INIT_IO_SYNC))

/* LUTs input select TR0 */
#define SmartIO_INIT_LUT0_TR0            ((uint32)(SmartIO_LUT0_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT1_TR0            ((uint32)(SmartIO_LUT1_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT2_TR0            ((uint32)(SmartIO_LUT2_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT3_TR0            ((uint32)(SmartIO_LUT3_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT4_TR0            ((uint32)(SmartIO_LUT4_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT5_TR0            ((uint32)(SmartIO_LUT5_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT6_TR0            ((uint32)(SmartIO_LUT6_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
#define SmartIO_INIT_LUT7_TR0            ((uint32)(SmartIO_LUT7_TR0 << CYFLD_PRGIO_PRT_LUT_TR0_SEL__OFFSET))
/* LUTs input select TR1 */
#define SmartIO_INIT_LUT0_TR1            ((uint32)(SmartIO_LUT0_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT1_TR1            ((uint32)(SmartIO_LUT1_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT2_TR1            ((uint32)(SmartIO_LUT2_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT3_TR1            ((uint32)(SmartIO_LUT3_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT4_TR1            ((uint32)(SmartIO_LUT4_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT5_TR1            ((uint32)(SmartIO_LUT5_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT6_TR1            ((uint32)(SmartIO_LUT6_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
#define SmartIO_INIT_LUT7_TR1            ((uint32)(SmartIO_LUT7_TR1 << CYFLD_PRGIO_PRT_LUT_TR1_SEL__OFFSET))
/* LUTs input select TR2 */
#define SmartIO_INIT_LUT0_TR2            ((uint32)(SmartIO_LUT0_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT1_TR2            ((uint32)(SmartIO_LUT1_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT2_TR2            ((uint32)(SmartIO_LUT2_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT3_TR2            ((uint32)(SmartIO_LUT3_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT4_TR2            ((uint32)(SmartIO_LUT4_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT5_TR2            ((uint32)(SmartIO_LUT5_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT6_TR2            ((uint32)(SmartIO_LUT6_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
#define SmartIO_INIT_LUT7_TR2            ((uint32)(SmartIO_LUT7_TR2 << CYFLD_PRGIO_PRT_LUT_TR2_SEL__OFFSET))
/* Initial LUTs input selections */
#define SmartIO_INIT_LUT0_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT0_TR0 | \
                                                    SmartIO_INIT_LUT0_TR1 | \
                                                    SmartIO_INIT_LUT0_TR2))
#define SmartIO_INIT_LUT1_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT1_TR0 | \
                                                    SmartIO_INIT_LUT1_TR1 | \
                                                    SmartIO_INIT_LUT1_TR2))
#define SmartIO_INIT_LUT2_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT2_TR0 | \
                                                    SmartIO_INIT_LUT2_TR1 | \
                                                    SmartIO_INIT_LUT2_TR2))
#define SmartIO_INIT_LUT3_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT3_TR0 | \
                                                    SmartIO_INIT_LUT3_TR1 | \
                                                    SmartIO_INIT_LUT3_TR2))
#define SmartIO_INIT_LUT4_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT4_TR0 | \
                                                    SmartIO_INIT_LUT4_TR1 | \
                                                    SmartIO_INIT_LUT4_TR2))
#define SmartIO_INIT_LUT5_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT5_TR0 | \
                                                    SmartIO_INIT_LUT5_TR1 | \
                                                    SmartIO_INIT_LUT5_TR2))
#define SmartIO_INIT_LUT6_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT6_TR0 | \
                                                    SmartIO_INIT_LUT6_TR1 | \
                                                    SmartIO_INIT_LUT6_TR2))
#define SmartIO_INIT_LUT7_SEL_CONFIG     ((uint32)(SmartIO_INIT_LUT7_TR0 | \
                                                    SmartIO_INIT_LUT7_TR1 | \
                                                    SmartIO_INIT_LUT7_TR2))

/* LUTs truth tables */
#define SmartIO_INIT_LUT0_MAP            ((uint32)(SmartIO_LUT0_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT1_MAP            ((uint32)(SmartIO_LUT1_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT2_MAP            ((uint32)(SmartIO_LUT2_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT3_MAP            ((uint32)(SmartIO_LUT3_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT4_MAP            ((uint32)(SmartIO_LUT4_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT5_MAP            ((uint32)(SmartIO_LUT5_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT6_MAP            ((uint32)(SmartIO_LUT6_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
#define SmartIO_INIT_LUT7_MAP            ((uint32)(SmartIO_LUT7_MAP << CYFLD_PRGIO_PRT_LUT__OFFSET))
/* LUTs modes */
#define SmartIO_INIT_LUT0_MODE           ((uint32)(SmartIO_LUT0_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT1_MODE           ((uint32)(SmartIO_LUT1_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT2_MODE           ((uint32)(SmartIO_LUT2_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT3_MODE           ((uint32)(SmartIO_LUT3_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT4_MODE           ((uint32)(SmartIO_LUT4_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT5_MODE           ((uint32)(SmartIO_LUT5_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT6_MODE           ((uint32)(SmartIO_LUT6_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
#define SmartIO_INIT_LUT7_MODE           ((uint32)(SmartIO_LUT7_MODE << CYFLD_PRGIO_PRT_LUT_OPC__OFFSET))
/* Initial LUTs control config */
#define SmartIO_INIT_LUT0_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT0_MAP  | SmartIO_INIT_LUT0_MODE))
#define SmartIO_INIT_LUT1_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT1_MAP  | SmartIO_INIT_LUT1_MODE))
#define SmartIO_INIT_LUT2_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT2_MAP  | SmartIO_INIT_LUT2_MODE))
#define SmartIO_INIT_LUT3_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT3_MAP  | SmartIO_INIT_LUT3_MODE))
#define SmartIO_INIT_LUT4_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT4_MAP  | SmartIO_INIT_LUT4_MODE))
#define SmartIO_INIT_LUT5_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT5_MAP  | SmartIO_INIT_LUT5_MODE))
#define SmartIO_INIT_LUT6_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT6_MAP  | SmartIO_INIT_LUT6_MODE))
#define SmartIO_INIT_LUT7_CTL_CONFIG     ((uint32)(SmartIO_INIT_LUT7_MAP  | SmartIO_INIT_LUT7_MODE))


/***************************************
*        Macro Functions
***************************************/

/**
* \addtogroup group_functions
* @{
*/


/*******************************************************************************
* Function Name: SmartIO_Enable
****************************************************************************//**
*
* \brief Enables the component. 
*  
* Once enabled, it takes two component clock cycles for the fabric reset to
* deactivate and the fabric becomes operational. If the clock source is set to
* Asynchronous mode, it takes three SYSCLK cycles before becoming functional.
*
* \funcusage
*  \snippet SmartIO_SUT.c usage_SmartIO_Enable
*******************************************************************************/
#define SmartIO_Enable()           (SmartIO_CTL |= SmartIO_FABRIC_ENABLE)


/*******************************************************************************
* Function Name: SmartIO_Disable
****************************************************************************//**
*
* \brief Disables the component. 
*  
* The block is disabled, which places the channels into bypass mode and the
* sequential elements are reset based on the chosen clock selection.
*
* \funcusage
*  \snippet SmartIO_SUT.c usage_SmartIO_Disable
*******************************************************************************/
#define SmartIO_Disable()           (SmartIO_CTL &= SmartIO_FABRIC_DISABLE)


/*******************************************************************************
* Function Name: SmartIO_GetBypass
****************************************************************************//**
*
* \brief Returns the bypass configuration of the channels on a bit by bit basis. 
*
* Bypassed channels behave like they would as if the SmartIO component was
* not present for those particular channels. 
*
* \return
*  uint8 Bypass state of the channels on the port.
*
* \funcusage
*  \snippet SmartIO_SUT.c usage_SmartIO_GetBypass
*******************************************************************************/
#define SmartIO_GetBypass()        ((uint8)(SmartIO_CTL \
                                            & ((uint32)SmartIO_CHANNEL_ALL << CYFLD_PRGIO_PRT_BYPASS__OFFSET)))
/** @} functions */

#endif /* (CY_SMARTIO_SmartIO_H) */


/* [] END OF FILE */
