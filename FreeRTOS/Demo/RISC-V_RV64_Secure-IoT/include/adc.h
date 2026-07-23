/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2023-2026 Mindgrove Technologies. All rights reserved.
 * 
 * @license Licensed under the Apache License, Version 2.0 (see LICENSE).
 * @licenseblock
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * @endlicenseblock
 * 
 * Project                   : MGS2401 SoC
 * @file adc.h
 * @brief Analog-to-Digital Converter (ADC) peripheral driver header
 * @details This header file declares the public interface for the ADC driver.
 * It provides configuration structures and APIs to control channel 
 * selection, resolution, conversion modes, and data acquisition.
 * @version 1.1
 * @authors Kishore J  (kishore@mindgrovetech.in) 
 *          Akash R B  (akash@mindgrovetech.in)
 * @date 27-01-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 18-06-2024 | 1.0     | Kishore J             | Initial draft.
 * 27-01-2026 | 1.1     | Akash R B             | Cleaned up and finalized APIs.
 * -----------------------------------------------------------------------------
 */
#ifndef BSP_INCLUDE_ADC_H_
#define BSP_INCLUDE_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @enum ADC_CHANNEL_ENUM ADC Input Channel Selection
 * @{
 */

/** @brief ADC Input Channel Selection. */
typedef enum {
    ADC_CHANNEL_0 ,
    ADC_CHANNEL_1 ,
    ADC_CHANNEL_2 ,
    ADC_CHANNEL_3 ,
    ADC_CHANNEL_4 ,
    ADC_CHANNEL_5 ,
    ADC_CHANNEL_6 ,
    ADC_CHANNEL_7
} adc_channel_t;

/** @} */

/**
 * @enum ADC_RESOLUTION_ENUM ADC Resolution Selection
 * @{
 */

/** @brief ADC Bit Resolution Selection. */
typedef enum {
    ADC_RES_6BIT ,
    ADC_RES_8BIT ,
    ADC_RES_10BIT,
    ADC_RES_12BIT
} adc_resolution_t;

/** @} */

/**
 * @enum ADC_IRQn_Type
 * 
 * @brief ADC interrupt ID
 * 
 * This enumeration defines the available ADC interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  ADC_INTR_IRQn             =  66,              /*!< 66 ADC_INTR                                                               */
} ADC_IRQn_Type;

/**
 * @defgroup ADC_Operation_Mode ADC Operation Modes
 * @brief ADC operational mode selection macros.
 * @{
 */

/**
 * @brief ADC performs a single conversion when start-of-conversion is triggered.
 */
#define ADC_SINGLE_CONVERSION   (0U)

/**
 * @brief ADC continuously performs conversions until disabled.
 */
#define ADC_FREERUN  (1U)

/** @} */

/**
 * @defgroup ADC_Interrupt_Control ADC Interrupt Control
 * @brief ADC end-of-conversion interrupt control macros.
 * @{
 */

/**
 * @brief ADC end-of-conversion interrupt is disabled.
 */
#define ADC_INTR_DISABLE      (0U)

/**
 * @brief ADC end-of-conversion interrupt is enabled.
 */
#define ADC_INTR_ENABLE      (1U)

/** @} */

/**
 * @defgroup ADC_VREF_Selection ADC Reference Voltage Selection
 * @brief ADC reference voltage source selection.
 * @{
 */

/**
 * @brief Select internal reference voltage.
 *
 * ADC uses the internally generated bandgap-based reference voltage.
 *
 */
#define ADC_INT_VREF   (1U)

/**
 * @brief Select external reference voltage.
 *
 * ADC uses reference voltage provided through external VREF pin.
 *
 */
#define ADC_EXT_VREF   (0U)

/** @} */

/**
 * @defgroup ADC_VBG_Control ADC Bandgap Generator Control
 * @brief ADC internal bandgap generator control.
 * @{
 */

/**
 * @brief Enable internal bandgap generator.
 *
 * Enables the on-chip bandgap circuit which generates
 * a stable temperature-independent reference voltage.
 *
 */
#define ADC_INT_VBG   (1U)

/**
 * @brief Disable internal bandgap generator.
 *
 * Disables the internal bandgap circuit.
 *
 * Used when:
 * - External reference is selected
 */
#define ADC_EXT_VBG   (0U)

/** @} */

/**
 * @brief ADC Configuration Structure.
 * @details Defines the parameters required to initialize and control 
 * the ADC peripheral via the Control Configuration Register (CCR).
 */

typedef struct {
    /**
     * @brief Selects the ADC input channel.
     *
     * Determines which analog input pin (channel 0–7)
     * is internally connected to the ADC for conversion.
     * Only the selected channel is sampled during operation.
     */
    adc_channel_t channel;

    /**
     * @brief Sets the ADC conversion resolution.
     *
     * Defines the number of bits used to represent the digital result.
     * Higher resolution increases measurement precision
     */
    adc_resolution_t resolution;

    /**
     * @brief Enables or disables free-running mode.
     *
     * If enabled, the ADC continuously performs conversions.
     * If disabled, the ADC performs a single conversion per trigger.
     */
    bool is_freerun;

    /**
     * @brief Enables End-of-Conversion (EOC) interrupt.
     *
     * When enabled, an interrupt is generated after each
     * completed conversion. If disabled, software must
     * poll the status flag to detect completion.
     */
    bool is_intr_en;

    /**
     * @brief Reference voltage source selection.
     * true  → Internal reference
     * false → External VREF pin
     */
    bool is_vref_int;

    /**
     * @brief Bandgap generator control.
     * true  → Internal bandgap enabled
     * false → Bandgap disabled
     *
     */
    bool is_vbg_int;
} ADC_Config_t;

/* --- ADC Driver APIs --- */

/**
 * @brief Configures the ADC Control Register.
 * 
 * @details Validates the provided configuration and writes to the 
 * hardware control register to initialize the ADC, select the channel, 
 * and start the Start of Conversion (SOC) sequence.
 * 
 * @param cfg Pointer to a constant ADC_Config_t structure.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful configuration
 * - @ref EFAULT Null pointer provided.
 * - @ref ECHRNG Channel selection out of valid range.
 * - @ref EINVAL Invalid resolution or mode parameters.
 */
uint16_t ADC_CCR(const ADC_Config_t *cfg);

/**
 * @brief Reads the converted digital output value.
 * 
 * @details Blocks until the End of Conversion (EOC) flag is set in hardware.
 * Once ready, it extracts the data from the output register, applies the 
 * appropriate bit-mask based on the configured resolution, and shifts the 
 * result to provide a normalized value.
 * 
 * @param cfg Pointer to the configuration structure used during initialization.
 * @param output Pointer to a variable where the converted ADC digital value 
 *               will be stored.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful completion
 * - @ref EFAULT Null pointer provided.
 * - @ref ETIMEDOUT Conversion did not complete within timeout.
 * 
 * @note This is a blocking call. Ensure the ADC has been triggered via 
 * ADC_CCR() before calling this function.
 */
uint16_t ADC_Read_Output(const ADC_Config_t *cfg , uint16_t * output);

/**
 * @brief Deactivates the ADC peripheral by clearing the control register.
 *
 * @details Writes the ADC_DISABLE mask to the ADC control register,
 *          immediately stopping any active conversion. The peripheral's
 *          clock or power gated circuitry is halted depending on the
 *          hardware abstraction. To restart, the peripheral must be
 *          reconfigured via ADC_Init().
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful completion
 * - @ref EFAULT Null pointer or invalid register access.
 *
 * @note Any conversion currently in progress will be terminated abruptly.
 *       Ensure all required data has been read via ADC_Read_Output()
 *       before calling this function.
 */
uint16_t ADC_Disable(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_ADC_H_

