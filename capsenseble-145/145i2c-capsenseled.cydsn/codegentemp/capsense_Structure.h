/***************************************************************************//**
* \file capsense_Structure.h
* \version 3.0
*
* \brief
*   This file provides the top level declarations of the component data structure.
*   Also, the file declares the high-level and low-level APIs for data access.
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

#if !defined(CY_CAPSENSE_capsense_STRUCTURE_H)
#define CY_CAPSENSE_capsense_STRUCTURE_H

#include <cytypes.h>
#include "capsense_Configuration.h"
#if (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE)
    #include "capsense_SmartSense_LL.h"
#endif  /* (capsense_CSD_SS_DIS != capsense_CSD_AUTOTUNE) */

/*******************************************************************************
* Constant Definitions
*******************************************************************************/

/* Defines size of Sensor Status Register in Data Structure */
#define capsense_SNS_STS_TYPE               uint8

/* No touch condition for slider position report */
#define capsense_SLIDER_NO_TOUCH            (0xFFFFFFFFLu)
#define capsense_TOUCHPAD_NO_TOUCH          (0xFFFFFFFFLu)

#define capsense_SLIDER_POS_NONE            (0xFFFFu)
#define capsense_TOUCHPAD_POS_NONE          (0xFFFFu)

#define capsense_MATRIX_POS_NONE            (0xFFu)
#define capsense_MATRIX_POS_MULTI           (0xFEu)

#define capsense_PROX_STS_MASK              (3Lu)
#define capsense_PROX_STS_OFFSET(proxId)    ((proxId) << 1u)
    
#define capsense_MATRIX_BUTTONS_TOUCHED     (0x80000000Lu)

/*******************************************************************************
* Enumeration types definition
*******************************************************************************/

/***************************************************************************//**
* \brief Defines widget types
*******************************************************************************/
typedef enum
{
    capsense_WD_BUTTON_E        = 0x01u,
    capsense_WD_LINEAR_SLIDER_E = 0x02u,
    capsense_WD_RADIAL_SLIDER_E = 0x03u,
    capsense_WD_MATRIX_BUTTON_E = 0x04u,
    capsense_WD_TOUCHPAD_E      = 0x05u,
    capsense_WD_PROXIMITY_E     = 0x06u
} capsense_WD_TYPE_ENUM;


/***************************************************************************//**
* \brief Defines sensing methods types
*******************************************************************************/
typedef enum
{
    capsense_UNDEFINED_E        = 0x00u,
    capsense_SENSE_METHOD_CSD_E = 0x01u,
    capsense_SENSE_METHOD_CSX_E = 0x02u
} capsense_SENSE_METHOD_ENUM;

/***************************************************************************//**
* \brief Defines electrode types
*******************************************************************************/
typedef enum
{
    capsense_ELTD_TYPE_SELF_E   = 0x01u,
    capsense_ELTD_TYPE_MUT_TX_E = 0x02u,
    capsense_ELTD_TYPE_MUT_RX_E = 0x03u
} capsense_ELTD_TYPE_ENUM;


/**
* \if SECTION_STRUCTURES
* \addtogroup group_structures
* \{
*/

/*******************************************************************************
* Declare RAM structures for all used widgets
*******************************************************************************/

/***************************************************************************//**
* \brief Declare common widget RAM parameters
*******************************************************************************/
typedef struct
{
    /**
     *  For the CSD Widgets provides scan resolution.
     *  For the CSX Widgets provides number of the Sub-conversions.
     */
    uint16 resolution;

    /**
     *  Widget Finger Threshold.
     */
    capsense_THRESHOLD_TYPE fingerTh;

    /**
     *  Widget Noise Threshold.
     */
    uint8  noiseTh;

    /**
     *  Widget Negative Noise Threshold.
     */
    uint8  nNoiseTh;

    /**
     *  Widget Hysteresis for the signal crossing finger threshold.
     */
    uint8  hysteresis;

    /**
     *  Widget Debounce for the signal above the finger threshold. OFF to ON.
     */
    uint8  onDebounce;

    /**
     *  Widget Low Baseline reset count.
     *  Specifies the number of samples the sensor has to be below the Negative Noise Threshold to trigger the baseline reset.
     */
    capsense_LOW_BSLN_RST_TYPE lowBslnRst;

    /**
     *  Widget baseline filter coefficient N (for IIR 2 to 8) or Baseline Update Threshold (for bucket method 1 to 255)
     */
    uint8  bslnCoeff;

    /**
     *  For the CSD widgets sets current of the modulation IDAC. 
     *  For the CSD Touchpad and Matrix Button widgets sets the current of the modulation IDAC for the column sensors.
     *  Not used for the CSX widgets.
     */
    uint8  idacMod [capsense_NUM_SCAN_FREQS];

    /**
     *  Specifies the sense clock divider. Is present only if individual clock dividers are enabled.
     *  For the Matrix Buttons and Touchpad widgets specifies the sense clock divider for the Column sensors.
     *  For the CSX Widgets sets TX clock divider.
     */
    uint8  snsClk;

    /**
     *  Widget Sensitivity parameter. Is available only if the HW Auto tuning is enabled.
     *  Is not used for the CSX Widgets.
     */
    uint16 sensitivity;

    /**
     *  Sensor signal per 1 pF (in raw counts)
     */
    uint16 sigPPf;
} capsense_RAM_WD_BASE_STRUCT;

/***************************************************************************//**
* \brief Declare RAM parameters for the Slider
*******************************************************************************/
typedef struct
{
    /**
     *  For the CSD Widgets provides scan resolution.
     *  For the CSX Widgets provides number of the Sub-conversions.
     */
    uint16 resolution;

    /**
     *  Widget Finger Threshold.
     */
    capsense_THRESHOLD_TYPE fingerTh;

    /**
     *  Widget Noise Threshold.
     */
    uint8  noiseTh;

    /**
     *  Widget Negative Noise Threshold.
     */
    uint8  nNoiseTh;

    /**
     *  Widget Hysteresis for the signal crossing finger threshold.
     */
    uint8  hysteresis;

    /**
     *  Widget Debounce for the signal above the finger threshold. OFF to ON.
     */
    uint8  onDebounce;

    /**
     *  Widget Low Baseline reset count.
     *  Specifies the number of samples the sensor has to be below the Negative Noise Threshold to trigger the baseline reset.
     */
    capsense_LOW_BSLN_RST_TYPE lowBslnRst;

    /**
     *  Widget baseline filter coefficient N (for IIR 2 to 8) or Baseline Update Threshold (for bucket method 1 to 255)
     */
    uint8  bslnCoeff;

    /**
     *  For the CSD widgets sets current of the modulation IDAC. 
     *  For the CSD Touchpad and Matrix Button widgets sets the current of the modulation IDAC for the column sensors.
     *  Not used for the CSX widgets.
     */
    uint8  idacMod [capsense_NUM_SCAN_FREQS];

    /**
     *  Specifies the sense clock divider. Is present only if individual clock dividers are enabled.
     *  For the Matrix Buttons and Touchpad widgets specifies the sense clock divider for the Column sensors.
     *  For the CSX Widgets sets TX clock divider.
     */
    uint8  snsClk;

    /**
     *  Widget Sensitivity parameter. Is available only if the HW Auto tuning is enabled.
     *  Is not used for the CSX Widgets.
     */
    uint16 sensitivity;

    /**
     *  Sensor signal per 1 pF (in raw counts)
     */
    uint16 sigPPf;

    /**
     *  Reports widget position.
     */
    uint16 position [capsense_NUM_CENTROIDS];
} capsense_RAM_WD_SLIDER_STRUCT;


/***************************************************************************//**
* \brief Declare RAM structure with all defined widgets
*******************************************************************************/
typedef struct
{
    /**
     *  LinearSlider0 widget RAM structure
     */
    capsense_RAM_WD_SLIDER_STRUCT linearslider0;
} capsense_RAM_WD_LIST_STRUCT;


/***************************************************************************//**
* \brief Declare RAM structure for sensors
*******************************************************************************/
typedef struct
{
    /**
     *  Sensor raw counts.
     */
    uint16 raw [capsense_NUM_SCAN_FREQS];

    /**
     *  Sensor baseline.
     */
    uint16 bsln [capsense_NUM_SCAN_FREQS];

    /**
     *  For the bucket baseline algorithm holds the bucket state,
     *  For the IIR baseline keeps LSB of the baseline value.
     */
    uint8  bslnExt [capsense_NUM_SCAN_FREQS];

    /**
     *  CSD Widgets: Compensation IDAC value.
     *  CSX Widgets: Balancing IDAC value.
     */
    uint8  idacComp [capsense_NUM_SCAN_FREQS];

    /**
     *  Sensor differences.
     */
    capsense_THRESHOLD_TYPE diff;

    /**
     *  Baseline reset counter for the low baseline reset function.
     */
    capsense_LOW_BSLN_RST_TYPE negBslnRstCnt [capsense_NUM_SCAN_FREQS];
} capsense_RAM_SNS_STRUCT;


/***************************************************************************//**
* \brief Declare RAM structure with all defined sensors
*******************************************************************************/
typedef struct
{
    /**
     *  LinearSlider0 sensors RAM structures array
     */
    capsense_RAM_SNS_STRUCT linearslider0 [capsense_LINEARSLIDER0_NUM_SENSORS];
} capsense_RAM_SNS_LIST_STRUCT;


/***************************************************************************//**
* \brief Declares the top-level RAM Data Structure
*******************************************************************************/
typedef struct
{
    /**
     *  16-bit CRC calculated by Customizer for the component configuration. 
     *  Used by the Tuner application to identify if the FW corresponds to the specific user configuration.
     */
    uint16 configId;

    /**
     *  Used by the Tuner application to identify device-specific configuration.
     */
    uint16 deviceId;

    /**
     *  Tuner Command Register. 
     *  Used for the communication between Tuner GUI and the component.
     */
    uint16 tunerCmd;

    /**
     *  This counter gets incremented after each scan.
     */
    uint16 scanCounter;

    /**
     *  Status information: Current Widget, Scan active, Error code.
     */
    uint32 status;

    /**
     *  Bitmask that sets which Widgets are enabled and scanned. 
     *  Each bit corresponds to one widget.
     */
    uint32 wdgtEnable [capsense_WDGT_STATUS_WORDS];

    /**
     *  Bitmask that reports activated Widgets (widgets that detect a touch signal above the threshold). 
     *  Each bit corresponds to one widget.
     */
    uint32 wdgtStatus [capsense_WDGT_STATUS_WORDS];

    /**
     *  For Buttons, Sliders, Matrix Buttons and CSD Touchpad each bit reports status of the individual sensor of the widget:
     *  1 - active (above the finger threshold);
     *  0 - inactive;
     *  For the CSD Touchpad and CSD Matrix Buttons the column sensors occupy the least significant bits.
     *  For the Proximity widget each sensor uses two bits with the following meaning:
     *  00 - not active;
     *  01 - proximity detected (signal above finger threshold);
     *  11 - finger touch detected (signal above the touch threshold);
     *  For the CSX Touchpad Widget this register provides number of the detected touches.
     *  The array size is equal to the total number of widgets. Size of the array element depends on the max number of sensors per widget used in the current design. It could be 1, 2 or 4 bytes.
     */
    capsense_SNS_STS_TYPE snsStatus [capsense_TOTAL_WIDGETS];

    /**
     *  Configuration register for global parameters of CSD0 block.
     */
    uint16 csd0Config;

    /**
     *  Modulator clock divider for CSD widgets.
     */
    uint8  modCsdClk;

    /**
     *  RAM Widget Objects.
     */
    capsense_RAM_WD_LIST_STRUCT wdgtList;

    /**
     *  RAM Sensor Objects.
     */
    capsense_RAM_SNS_LIST_STRUCT snsList;

    /**
     *  Selected widget ID.
     */
    uint8  snrTestWidgetId;

    /**
     *  Selected sensor ID.
     */
    uint8  snrTestSensorId;

    /**
     *  Scan counter.
     */
    uint16 snrTestScanCounter;

    /**
     *  Sensor raw counts.
     */
    uint16 snrTestRawCount [capsense_NUM_SCAN_FREQS];
} capsense_RAM_STRUCT;


/***************************************************************************//**
* \brief Declares the Flash IO object
*******************************************************************************/
typedef struct
{
    /**
     *  Pointer to the HSIOM configuration register of the IO.
     */
    reg32  * hsiomPtr;

    /**
     *  Pointer to the port configuration register of the IO.
     */
    reg32  * pcPtr;

    /**
     *  Pointer to the port data register of the IO.
     */
    reg32  * drPtr;

    /**
     *  Pointer to the pin state data register of the IO.
     */
    reg32  * psPtr;

    /**
     *  IO mask in the HSIOM configuration register.
     */
    uint32   hsiomMask;

    /**
     *  IO mask in the DR and PS registers.
     */
    uint32   mask;

    /**
     *  Position of the IO configuration bits in the HSIOM register.
     */
    uint8    hsiomShift;

    /**
     *  Position of the IO configuration bits in the DR and PS registers.
     */
    uint8    drShift;

    /**
     *  Position of the IO configuration bits in the PC register.
     */
    uint8    shift;
} capsense_FLASH_IO_STRUCT;


/***************************************************************************//**
* \brief Declares the Flash Electrode object
*******************************************************************************/
typedef struct
{
    /**
     *  Index of the first IO in the Flash IO Object Array.
     */
    uint16 firstPinId;

    /**
     *  Total number of IOs in this sensor.
     */
    uint8  numPins;

    /**
     * Sensor type:
     * - ELTD_TYPE_SELF_E - CSD sensor;
     * - ELTD_TYPE_MUT_TX_E - CSX Tx sensor;
     * - ELTD_TYPE_MUT_RX_E - CSX Rx sensor;
     */
    uint8  type;
} capsense_FLASH_SNS_STRUCT;


/***************************************************************************//**
* \brief Declares the structure with all Flash electrode objects
*******************************************************************************/
typedef struct
{
    /**
     *  No ganged sensors available
     */
    uint8 notUsed;
} capsense_FLASH_SNS_LIST_STRUCT;


/***************************************************************************//**
* \brief Declare Flash widget object
*******************************************************************************/
typedef struct
{
    /**
     *  Points to the array of the FLASH Sensor Objects or FLASH IO Objects that belong to this widget.
     *  Sensing block uses this pointer to access and configure IOs for the scanning.
     *  Bit #2 in WD_STATIC_CONFIG field indicates the type of array: 
     *  1 - Sensor Object; 
     *  0 - IO Object.
     */
    void const * ptr2SnsFlash;

    /**
     *  Points to the Widget Object in RAM. Sensing block uses it to access scan parameters.
     *  Processing uses it to access threshold and widget specific data.
     */
    void * ptr2WdgtRam;

    /**
     *  Points to the array of Sensor Objects in RAM.
     *  Sensing and Processing blocks use it to access scan data.
     */
    capsense_RAM_SNS_STRUCT * ptr2SnsRam;

    /**
     *  Points to the array of the Filter History Objects in RAM that belongs to this widget.
     */
    void * ptr2FltrHistory;

    /**
     *  Points to the array of the debounce counters.
     *  Size of debounce counter is 8bit.
     *  These arrays are not part of the data structure.
     */
    uint8 * ptr2DebounceArr;

    /**
     *  Miscellaneous configuration flags
     */
    uint16 staticConfig;

    /**
     *  Total number of sensors. 
     *  For CSD widgets: WD_NUM_ROWS + WD_NUM_COLS
     *  For CSX widgets: WD_NUM_ROWS * WD_NUM_COLS
     */
    uint16 totalNumSns;

    /**
     *  Specifies one of the following widget types: 
     *  WD_BUTTON_E,
     *  WD_LINEAR_SLIDER_E,
     *  WD_RADIAL_SLIDER_E,
     *  WD_MATRIX_BUTTON_E, 
     *  WD_TOUCHPAD_E, 
     *  WD_PROXIMITY_E
     */
    uint8  wdgtType;

    /**
     *  For CSD Button and Proximity Widgets: Number of sensors.
     *  For CSD Slider Widget: Number of segments.
     *  For CSD Touchpad and Matrix Button: Number of Column sensors.
     *  For CSX Button, Touchpad and Matrix Button: number of RX electrodes.
     */
    uint8  numCols;

    /**
     *  Sliders: Linear/Angular resolution.
     *  Touchpad: X-Axis resolution.
     */
    uint16 xResolution;

    /**
     *  The pre-calculated X resolution centroid multiplier that is used for the X axis position calculation.
     *  Calculated as follows:
     *  RADIAL: (WD_X_RESOLUTION * 256) / WD_NUM_COLS;
     *  LINEAR: (WD_X_RESOLUTION * 256) / (WD_NUM_COLS - 1);
     *  TOUCHPAD: same as LINEAR
     */
    uint16 xCentroidMultiplier;

    /**
     *  Pointer to the array with the sensor noise envelope data.
     *  Is set to the valid value only for the CSD widgets.
     *  For the CSX widgets this pointer is set to NULL.
     *  The pointed array is not part of the data structure.
     */
    SMARTSENSE_CSD_NOISE_ENVELOPE_STRUCT * ptr2NoiseEnvlp;

    /**
     *  Pointer to the RAM position history object. This parameter is used for the Sliders and CSD touchpads that have enabled median position filter.
     */
    void * ptr2PosHistory;
} capsense_FLASH_WD_STRUCT;


/***************************************************************************//**
* \brief Declares top-level Flash Data Structure
*******************************************************************************/
typedef struct
{
    /**
     *  Array of flash widget objects
     */
    capsense_FLASH_WD_STRUCT wdgtArray[capsense_TOTAL_WIDGETS];
} capsense_FLASH_STRUCT;


/** \}
* \endif */


/***************************************************************************//**
* Declares Filter module structures
*******************************************************************************/

/***************************************************************************//**
* \brief Declare filter channel structure for regular sensors
*******************************************************************************/
typedef struct
{
    uint16 notUsed;
} capsense_REGULAR_FLTR_CHANNEL_STRUCT;

/***************************************************************************//**
* \brief Declare filter structure for regular sensors
*******************************************************************************/
typedef struct
{
    /**
     *  Array of capsense_REGULAR_FLTR_CHANNEL_STRUCT for each available scan frequency
     */
    capsense_REGULAR_FLTR_CHANNEL_STRUCT regularChannel[capsense_NUM_SCAN_FREQS];
} capsense_REGULAR_FLTR_STRUCT;

/***************************************************************************//**
* \brief Declare union for filter structure variants
*******************************************************************************/
typedef union
{
    /**
     *  Pointer to void type
     */
    void *ptr;

    /**
     *  Pointer to capsense_REGULAR_FLTR_STRUCT type
     */
    capsense_REGULAR_FLTR_STRUCT *ptrRegular;
} capsense_PTR_FILTER_VARIANT;


typedef struct 
{
    uint16 posMedianZ1;
    uint16 posMedianZ2;
} capsense_SLIDER_POS_HISTORY_STRUCT;

typedef struct
{
    capsense_SLIDER_POS_HISTORY_STRUCT xPos;
    capsense_SLIDER_POS_HISTORY_STRUCT yPos;
} capsense_TOUCHPAD_POS_HISTORY_STRUCT;

/*******************************************************************************
* Defines shift/mask values for data structure register bit fields
*******************************************************************************/

/* CSD0_CONFIG bit fields */

/* Bit 0: Multi-frequency scan enable/disable at runtime. This bit is functional
 * only if Multi-frequency scan functionality has been activated
 * in general component configuration. */
#define capsense_MULTI_FREQ_EN_SIZE       (0x00000001Lu)
#define capsense_MULTI_FREQ_EN_SHIFT      (0u)
#define capsense_MULTI_FREQ_EN_MASK       (capsense_MULTI_FREQ_EN_SIZE << capsense_MULTI_FREQ_EN_SHIFT)

/* Bit 1: Sensor auto-reset is enabled */
#define capsense_SNS_ARST_SIZE            (0x00000001Lu)
#define capsense_SNS_ARST_SHIFT           (1u)
#define capsense_SNS_ARST_MASK            (capsense_SNS_ARST_SIZE << capsense_SNS_ARST_SHIFT)

/* Bit 2: IDAC range setting (4x/8x) */
#define capsense_IDAC_RANGE_SIZE          (0x00000001Lu)
#define capsense_IDAC_RANGE_SHIFT         (2u)
#define capsense_IDAC_RANGE_MASK          (capsense_IDAC_RANGE_SIZE << capsense_IDAC_RANGE_SHIFT)

/* Bits 3-7: Reserved */

/* Bit 8: Shield electrode signal enable/disable */
#define capsense_SHLD_EN_SIZE             (0x00000001Lu)
#define capsense_SHLD_EN_SHIFT            (8u)
#define capsense_SHLD_EN_MASK             (capsense_SHLD_EN_SIZE << capsense_SHLD_EN_SHIFT)

/* Bits 9-10: Selects delay by which csd_shield is delayed relative to csd_sense */
#define capsense_SHLD_DLY_SIZE            (0x00000003Lu)
#define capsense_SHLD_DLY_SHIFT           (9u)
#define capsense_SHLD_DLY_MASK            (capsense_SHLD_DLY_SIZE << capsense_SHLD_DLY_SHIFT)


/* STATUS bit fields */

/* Bits 0-6: Currently selected widget for CSD0 */
#define capsense_STATUS_WDGT0_SIZE        (0x0000007FLu)
#define capsense_STATUS_WDGT0_SHIFT       (0u)
#define capsense_STATUS_WDGT0_MASK        (capsense_STATUS_WDGT0_SIZE << capsense_STATUS_WDGT0_SHIFT)

/* Bit 7 */
#define capsense_STATUS_CSD0_SIZE         (0x00000001Lu)
#define capsense_STATUS_CSD0_SHIFT        (7u)
#define capsense_STATUS_CSD0_MASK         (capsense_STATUS_CSD0_SIZE << capsense_STATUS_CSD0_SHIFT)

/* Bits 8-14: Currently selected widget for CSD1 */
#define capsense_STATUS_WDGT1_SIZE        (0x0000007FLu)
#define capsense_STATUS_WDGT1_SHIFT       (8u)
#define capsense_STATUS_WDGT1_MASK        (capsense_STATUS_WDGT1_SIZE << capsense_STATUS_WDGT1_SHIFT)

/* Bit 15 */
#define capsense_STATUS_CSD1_SIZE         (0x00000001Lu)
#define capsense_STATUS_CSD1_SHIFT        (15u)
#define capsense_STATUS_CSD1_MASK         (capsense_STATUS_CSD1_SIZE << capsense_STATUS_CSD1_SHIFT)

/* Bit 16: WIDGET BUSY flag status for CSD0 */
#define capsense_STATUS_WDGT0_BUSY_SIZE   (0x00000001Lu)
#define capsense_STATUS_WDGT0_BUSY_SHIFT  (16u)
#define capsense_STATUS_WDGT0_BUSY_MASK   (capsense_STATUS_WDGT0_BUSY_SIZE << capsense_STATUS_WDGT0_BUSY_SHIFT)

/* Bit 17: WIDGET BUSY flag status for CSD1 */
#define capsense_STATUS_WDGT1_BUSY_SIZE   (0x00000001Lu)
#define capsense_STATUS_WDGT1_BUSY_SHIFT  (17u)
#define capsense_STATUS_WDGT1_BUSY_MASK   (capsense_STATUS_WDGT1_BUSY_SIZE << capsense_STATUS_WDGT1_BUSY_SHIFT)

/* Bits 18-23: Reserved */

/* Bits 24-31: CapSense component error code */
#define capsense_STATUS_ERR_SIZE          (0x000000FFLu)
#define capsense_STATUS_ERR_SHIFT         (24u)
#define capsense_STATUS_ERR_MASK          (capsense_STATUS_ERR_SIZE << capsense_STATUS_ERR_SHIFT)


/* WD_STATIC_CONFIG bit fields */

/* Bit 0: Sense/Tx frequency selection: 0 - Common, 1 - Individual (per widget) */
#define capsense_SNS_FREQ_SIZE            (0x00000001Lu)
#define capsense_SNS_FREQ_SHIFT           (0u)
#define capsense_SNS_FREQ_MASK            (capsense_SNS_FREQ_SIZE << capsense_SNS_FREQ_SHIFT)

/* Bit 1: Duplexing enable/disable for linear sliders. */
#define capsense_DIPLEXING_SIZE           (0x00000001Lu)
#define capsense_DIPLEXING_SHIFT          (1u)
#define capsense_DIPLEXING_MASK           (capsense_DIPLEXING_SIZE << capsense_DIPLEXING_SHIFT)

/* Bit 2: Widget contains sensor ganged to another sensors. */
#define capsense_GANGED_SNS_SIZE          (0x00000001Lu)
#define capsense_GANGED_SNS_SHIFT         (2u)
#define capsense_GANGED_SNS_MASK          (capsense_GANGED_SNS_SIZE << capsense_GANGED_SNS_SHIFT)

/* Bit 3: Some pin from this widget is used by other sensors to make ganged sensor. */
#define capsense_SHARED_IO_SIZE           (0x00000001Lu)
#define capsense_SHARED_IO_SHIFT          (3u)
#define capsense_SHARED_IO_MASK           (capsense_SHARED_IO_SIZE << capsense_SHARED_IO_SHIFT)

/* Bit 4: Centroid position IIR filter enable/disable. */
#define capsense_POS_IIR_FILTER_SIZE      (0x00000001Lu)
#define capsense_POS_IIR_FILTER_SHIFT     (4u)
#define capsense_POS_IIR_FILTER_MASK      (capsense_POS_IIR_FILTER_SIZE << capsense_POS_IIR_FILTER_SHIFT)

/* Bit 5: Centroid position median filter enable/disable. */
#define capsense_POS_MEDIAN_FILTER_SIZE   (0x00000001Lu)
#define capsense_POS_MEDIAN_FILTER_SHIFT  (5u)
#define capsense_POS_MEDIAN_FILTER_MASK   (capsense_POS_MEDIAN_FILTER_SIZE << capsense_POS_MEDIAN_FILTER_SHIFT)

/* Bit 6: Centroid position average filter enable/disable. */
#define capsense_POS_AVERAGE_FILTER_SIZE  (0x00000001Lu)
#define capsense_POS_AVERAGE_FILTER_SHIFT (6u)
#define capsense_POS_AVERAGE_FILTER_MASK  (capsense_POS_AVERAGE_FILTER_SIZE << capsense_POS_AVERAGE_FILTER_SHIFT)

/* Bit 7: Centroid position jitter filter enable/disable. */
#define capsense_POS_JITTER_FILTER_SIZE   (0x00000001Lu)
#define capsense_POS_JITTER_FILTER_SHIFT  (7u)
#define capsense_POS_JITTER_FILTER_MASK   (capsense_POS_JITTER_FILTER_SIZE << capsense_POS_JITTER_FILTER_SHIFT)

/* Bit 8: Multiphase Tx scan enable (CSX widgets only). */
#define capsense_MULTIPHASE_TX_SIZE       (0x00000001Lu)
#define capsense_MULTIPHASE_TX_SHIFT      (8u)
#define capsense_MULTIPHASE_TX_MASK       (capsense_MULTIPHASE_TX_SIZE << capsense_MULTIPHASE_TX_SHIFT)


/*******************************************************************************
* Define Data Structure Macro helpers
*******************************************************************************/

/*******************************************************************************
* Determines the widget specific word in the wdgtStatus array by widget ID
*******************************************************************************/
/* Divide by 32 - size of the wdgtStatus word in bits to find the word index */
#define capsense_GET_WDGT_STATUS_INDEX(wdgtId)  ((wdgtId) >> 5u)

/*******************************************************************************
* Determines the widget bitmask in wdgtStatus word by widget ID
*******************************************************************************/
/* Take the least 5 bits of widget id to find the bit number */
#define capsense_GET_WDGT_STATUS_MASK(wdgtId)   (1Lu << ((wdgtId) & 0x1FLu))

/*******************************************************************************
* Determines the sense method of the widget
*******************************************************************************/
#if (0u != capsense_TOTAL_CSD_WIDGETS) && (0u != capsense_TOTAL_CSX_WIDGETS)
    #define capsense_GET_SENSE_METHOD(ptrFlash) ((capsense_SENSE_METHOD_ENUM)(ptrFlash)->senseMethod)
#else
    #if (0u != capsense_TOTAL_CSX_WIDGETS)
        #define capsense_GET_SENSE_METHOD(ptrFlash) capsense_SENSE_METHOD_CSX_E
    #else
        #define capsense_GET_SENSE_METHOD(ptrFlash) capsense_SENSE_METHOD_CSD_E
    #endif /* #if (0u != capsense_TOTAL_CSX_WIDGETS) */
#endif /* #if (0u != capsense_TOTAL_CSD_WIDGETS) && (0u != capsense_TOTAL_CSX_WIDGETS) */

/*******************************************************************************
* Gets the widget type
*******************************************************************************/
#define capsense_GET_WIDGET_TYPE(ptrFlashWdgt)  ((capsense_WD_TYPE_ENUM)((ptrFlashWdgt)->wdgtType))

/*******************************************************************************
* Gets the number of the widget's sensors
*******************************************************************************/
#define capsense_GET_SENSOR_COUNT(widgetId)         capsense_dsFlash.wdgtArray[(widgetId)].totalNumSns
#define capsense_GET_SNS_CNT_BY_PTR(ptrFlashWidget) (ptrFlashWidget)->totalNumSns


/*******************************************************************************
* Increments the pointer to the Regular Filter History Object
*******************************************************************************/
#if ((0u != capsense_REGULAR_RC_FILTER_EN) &&                                               \
     ((0u != capsense_BUTTON_WIDGET_EN) || (0u != capsense_SLIDER_WIDGET_EN)  ||    \
      (0u != capsense_MATRIX_WIDGET_EN) || (0u != capsense_TOUCHPAD_WIDGET_EN)))

    #define capsense_INC_REG_FLTR_OBJ(fltrVariant)    \
    do {                                                      \
        (fltrVariant).ptrRegular++;                           \
    } while(0)

#else

    #define capsense_INC_REG_FLTR_OBJ(fltrVariant)

#endif /* #if ((0u != capsense_REGULAR_RC_FILTER_EN) &&                                 
               ((0u != capsense_BUTTON_WIDGET_EN) || (0u != capsense_SLIDER_WIDGET_EN)  ||
                (0u != capsense_MATRIX_WIDGET_EN) || (0u != capsense_TOUCHPAD_WIDGET_EN))) */


/*******************************************************************************
* Increments the pointer to the Proximity Filter History Object
*******************************************************************************/
#if ((0u != capsense_PROX_RC_FILTER_EN) && (0u != capsense_PROXIMITY_WIDGET_EN))
    #define capsense_INC_PROX_FLTR_OBJ(fltrVariant)     \
        do {                                                    \
            (fltrVariant).ptrProx++;                            \
        } while(0)
#else
    #define capsense_INC_PROX_FLTR_OBJ(fltrVariant)
#endif /* #if ((0u != capsense_PROX_RC_FILTER_EN) && (0u != capsense_PROXIMITY_WIDGET_EN)) */


/*******************************************************************************
* Increments the pointer to the Filter History Object Variant
*******************************************************************************/
#define capsense_INC_FLTR_OBJ_VARIANT(isProxHistObj, fltrVariant)   \
    do {                                                                    \
        if (0u == (isProxHistObj))                                          \
        {                                                                   \
            capsense_INC_REG_FLTR_OBJ(fltrVariant);                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            capsense_INC_PROX_FLTR_OBJ(fltrVariant);                \
        }                                                                   \
    } while(0)

/*******************************************************************************
* Gets a widget status in the global enable register dsRam.wdgtEnable[]
*******************************************************************************/
#if (0u != capsense_SELF_TEST_EN)
    #define capsense_GET_WIDGET_EN_STATUS(wdId)                                             \
                (capsense_GET_WDGT_STATUS_MASK(wdId) &                                      \
                capsense_dsRam.wdgtEnable[capsense_GET_WDGT_STATUS_INDEX(wdId)] &   \
                capsense_dsRam.wdgtWorking[capsense_GET_WDGT_STATUS_INDEX(wdId)])
#else
    #define capsense_GET_WIDGET_EN_STATUS(wdId)                    \
                (capsense_GET_WDGT_STATUS_MASK(wdId) &             \
                 capsense_dsRam.wdgtEnable[capsense_GET_WDGT_STATUS_INDEX(wdId)])
#endif /* #if (0u != capsense_SELF_TEST_EN) */

/*******************************************************************************
* Gets a widget active status from the dsRam.wdgtStatus[] register
*******************************************************************************/
#define capsense_GET_WIDGET_ACTIVE_STATUS(wdId)                    \
            (capsense_GET_WDGT_STATUS_MASK(wdId) &                 \
             capsense_dsRam.wdgtStatus[capsense_GET_WDGT_STATUS_INDEX(wdId)])

/*******************************************************************************
* Declares Flash and RAM Data Structure variables
*******************************************************************************/
extern capsense_RAM_STRUCT            capsense_dsRam;
extern const capsense_FLASH_STRUCT    capsense_dsFlash;
extern const capsense_FLASH_IO_STRUCT capsense_ioList[capsense_TOTAL_ELECTRODES];


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* High-level API
*******************************************************************************/

/**
* \if SECTION_CAPSENSE_HIGH_LEVEL
* \addtogroup group_capsense_high_level
* \{
*/

uint32 capsense_IsAnyWidgetActive(void);
uint32 capsense_IsWidgetActive(uint32 widgetId);
uint32 capsense_IsSensorActive(uint32 widgetId, uint32 sensorId);

#if (0u != capsense_MATRIX_WIDGET_EN)
    uint32 capsense_IsMatrixButtonsActive(uint32 widgetId);
#endif /* #if (0u != capsense_MATRIX_WIDGET_EN) */

#if (0u != capsense_PROXIMITY_WIDGET_EN)
    uint32 capsense_IsProximityTouchActive(uint32 widgetId, uint32 proxId);
#endif /* #if (0u != capsense_PROXIMITY_WIDGET_EN) */

#if (0u != capsense_SLIDER_WIDGET_EN)
    uint32 capsense_GetCentroidPos(uint32 widgetId);
#endif /* #if (0u != capsense_SLIDER_WIDGET_EN) */

#if (0u != capsense_TOUCHPAD_WIDGET_EN)
    uint32 capsense_GetXYCoordinates(uint32 widgetId);
#endif /* #if (0u != capsense_TOUCHPAD_WIDGET_EN) */

/** \}
* \endif */

/*******************************************************************************
* Low level API
*******************************************************************************/

/**
* \if SECTION_CAPSENSE_LOW_LEVEL
* \addtogroup group_capsense_low_level
* \{
*/

cystatus capsense_GetParam(uint32 paramId, uint32 *value);
cystatus capsense_SetParam(uint32 paramId, uint32 value);

/** \}
* \endif
*/

/*******************************************************************************
* Function Prototypes - internal functions
*******************************************************************************/
/**
* \if SECTION_CAPSENSE_INTERNAL
* \addtogroup group_capsense_internal
* \{
*/

void capsense_DsInitialize(void);
#if (0u != capsense_ADC_EN)
    void capsense_AdcDsInitialize(void);
#endif /* (0u != capsense_ADC_EN) */
#if (0u != capsense_SELF_TEST_EN)
    void capsense_DsUpdateWidgetCrc(uint32 widgetId);
    uint16 capsense_DsCalculateCrc16(uint8 *ptrData, uint32 len);
#endif /* #if (0u != capsense_SELF_TEST_EN) */

/** \}
* \endif */

#endif /* End CY_CAPSENSE_capsense_STRUCTURE_H */

/* [] END OF FILE */
