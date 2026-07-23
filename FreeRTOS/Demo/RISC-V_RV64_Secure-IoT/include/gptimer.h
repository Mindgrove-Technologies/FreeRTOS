/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. All rights reserved.
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
 * @file gptimer.h
 * @brief Contains driver apis for GPTimer Interface
 * @details Provides the API for hardware control of General Purpose Timer (GPTimer)
 *          including configuration and resetting the counter.
 * @version 1.1
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 *          Deeptha G             (deeptha@mindgrovetch.in)
 * @date 17-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 07-02-2024 | 1.0     | Shri Mahaalakshmi S J | Initial release.
 * 17-02-2026 | 1.1     | Deeptha G             | Optimized driver and updated
 *            |         |                       | it to be MISRA-compliant.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_GPTIMER_H_
#define BSP_INCLUDE_GPTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup GPTIMER_MODES GPTimer Operational Modes
 * @brief Definitions for the timer counting logic and waveform generation.
 * @{
 */
/** @brief The value of this macro is 0U. Pulse Width Modulation mode.
 */
#define GPT_PWM_MODE 0U
/** @brief The value of this macro is 1U. Standard Up-counting mode.
 */
#define GPT_UP_COUNT 1U
/** @brief The value of this macro is 2U. Standard Down-counting mode.
 */
#define GPT_DOWN_COUNT 2U
/** @brief The value of this macro is 3U. Up-Down counting mode.
 */
#define GPT_UPDOWN_COUNT 3U
/** @} */

/**
 * @defgroup GPTIMER_Instance_Type GPTIMER Instance Handle Type
 * @brief Opaque GPTIMER hardware instance handle type.
 *
 * This module defines the GPTIMER instance handle type used by the GPTIMER driver.
 * The type is opaque and represents a specific GPTIMER hardware block.
 *
 * Users must not create objects of this type directly. Valid instances
 * must only be obtained using the provided GPTIMER instance constructors
 * or GPTIMER instance macros.
 *
 * @{
 */
/**
 * @brief Opaque GPTIMER hardware instance type.
 *
 * This type represents a specific GPTIMER hardware block on the SoC.
 * The internal structure is hidden from the user and is only known
 * to the GPTIMER driver implementation.
 *
 * @note This type must only be used as a handle.
 *       Do not attempt to allocate or define objects of this type.
 */
typedef struct GPTIMER_Instance GPTIMER_Instance_t;

/**
 * @brief Get handle for GPTIMER instance 0.
 *
 * @note GPTIMER instance 0 is reserved for debug and print statements.
 *       It should not be used for general-purpose GPTIMER communication.
 *
 * @return Pointer to GPTIMER0 hardware instance descriptor.
 */
const GPTIMER_Instance_t *GPTIMER_INSTANCE_0(void);

/**
 * @brief Get handle for GPTIMER instance 1.
 * @return Pointer to GPTIMER1 hardware instance descriptor.
 */
const GPTIMER_Instance_t *GPTIMER_INSTANCE_1(void);

/**
 * @brief Get handle for GPTIMER instance 2.
 * @return Pointer to GPTIMER2 hardware instance descriptor.
 */
const GPTIMER_Instance_t *GPTIMER_INSTANCE_2(void);

/**
 * @brief Get handle for GPTIMER instance 3.
 * @return Pointer to GPTIMER3 hardware instance descriptor.
 */
const GPTIMER_Instance_t *GPTIMER_INSTANCE_3(void);

/** @} */ /* end of GPTIMER_Instance_Constructors */

/**
 * @defgroup GPTIMER_Instance_Macros GPTIMER Instance Macros
 * @brief GPTIMER hardware instance macros.
 *
 * These macros expand to constant GPTIMER hardware instance handles.
 * They provide a convenient and readable way to select GPTIMER instances.
 *
 * @{
 */
/**
 * @def GPTIMER0
 * @ingroup GPTIMER_Instance_Macros
 * @brief Macro for GPTIMER instance 0 handle.
 */
#define GPTIMER0 GPTIMER_INSTANCE_0()

/**
 * @def GPTIMER1
 * @ingroup GPTIMER_Instance_Macros
 * @brief Macro for GPTIMER instance 1 handle.
 */
#define GPTIMER1 GPTIMER_INSTANCE_1()

/**
 * @def GPTIMER2
 * @ingroup GPTIMER_Instance_Macros
 * @brief Macro for GPTIMER instance 2 handle.
 */
#define GPTIMER2 GPTIMER_INSTANCE_2()

/**
 * @def GPTIMER3
 * @ingroup GPTIMER_Instance_Macros
 * @brief Macro for GPTIMER instance 3 handle.
 */
#define GPTIMER3 GPTIMER_INSTANCE_3()

/** @} */ /* end of GPTIMER_Instance_Macros */

/**
 * @enum GPTIMER_IRQn_Type
 * 
 * @brief GPTIMER interrupt ID
 * 
 * This enumeration defines the available GPTIMER interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  GPTIMER0_IRQn             =  47,              /*!< 47 GPTIMER0                                                               */
  GPTIMER1_IRQn             =  48,              /*!< 48 GPTIMER1                                                               */
  GPTIMER2_IRQn             =  49,              /*!< 49 GPTIMER2                                                               */
  GPTIMER3_IRQn             =  50,              /*!< 50 GPTIMER3                                                               */
} GPT_IRQn_Type;

/**
 * @brief GPTIMER configuration structure.
 *
 * This structure contains all parameters required to initialize
 * and configure a General Purpose Timer (GPTIMER).
 */
typedef struct {
    /**
     * @brief GPTIMER hardware instance handle.
     *
     * This parameter selects the GPTIMER hardware block to be used.
     * Valid instances are GPTIMER1, GPTIMER2, GPTIMER3 and GPTIMER4.
     *
     * @note GPTIMER instance 0 is reserved for serial debug communication.
     */
    const GPTIMER_Instance_t *gpt_num;
    /**
     * @brief GPTIMER operating mode.
     *
     * Selects the counting or PWM mode of the timer.
     *
     * @note Valid range: 0 <= mode < 4.
     */
    uint8_t mode;
    /**
     * @brief Timer period value.
     *
     * Specifies the period up to which the GPTIMER counts
     * before overflowing, underflowing, or resetting
     * (depending on the selected mode).
     */
    uint32_t period;
    /**
     * @brief Clock prescaler value.
     *
     * Divides the input clock frequency before it is fed
     * to the GPTIMER counter.
     */
    uint32_t prescaler;
    /**
     * @brief PWM duty cycle value.
     *
     * Defines the duty cycle of the PWM output when the timer
     * operates in PWM mode.
     *
     * @note The value provided by the user is normalized internally.
     *       The input is taken modulo 100, ensuring the effective
     *       duty cycle is always in the range 0 to 100 (inclusive).
     *       A modulo result of 0 represents a 100% duty cycle.
     *       A duty cycle of 0% is achieved only when the user
     *       explicitly provides a value of 0U.
     */
    uint32_t dutycycle;
    /**
     * @brief Interrupt enable flag.
     *
     * Enables or disables GPTIMER interrupts based
     * on the selected mode.
     *
     * @note 1 = Enable interrupt, 0 = Disable interrupt
     */
    bool interrupt_en;
    /**
     * @brief Continuous count enable flag.
     *
     * Enables continuous counting mode for the GPTIMER.
     *
     * @note 1 = Continuous counting enabled
     */
    bool cnt_en;
    /**
     * @brief Capture input enable flag.
     *
     * Enables capturing of the external input value.
     */
    bool capture_val;
    /**
     * @brief Output enable flag.
     *
     * @details Enable the output signal in PWM mode by setting
     * the output enable bit in the control/status register.
     */
    bool output_en;
} GPTIMER_Config_t;

/**
 *
 * @brief The function `GPT_Init` initialises the gptimer with different modes and configures necessary registers.
 *
 * @details It is used to select the specific GPT instance, set the mode, set the prescaler, set the dutycycle, enable
 * continous count and set the input bit to be captured.
 *
 * @param gptimer The `gptimer` parameter in the `GPT_Init` function is a struct that
 * contains several properties related to configuring a General Purpose Timer (GPT).It has the following properties:
 * gpt_num, mode, interrupt_en, period, prescaler, dutycycle, cnt_en, capture_val, output_en.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS Configuration applied successfully.
 * - @ref EFAULT Returned by @ref CHECK_NULL if null pointer is given as input.
 * - @ref EPERM  Returned if prescaler is less than or equal to zero. 
 * - @ref EINVAL Returned if invalid inconfiguration value is given such as mode or interrupt_enable
 */
uint16_t GPT_Init(GPTIMER_Config_t const *gptimer_config);

/**
 *
 * @brief The function ` GPT_Reset` resets the counter, interrupts and register values.
 *
 * @details It is used to reset the counter, interrupts and register values. for the specified GPTIMER
 * instance.
 *
 * @param gptimer The `gptimer` parameter in the `GPT_Init` function is a struct that
 * contains several properties related to configuring a General Purpose Timer (GPT).It has the following properties:
 * gpt_num, mode, interrupt_en, period, prescaler, dutycycle, cnt_en, capture_val, output_en.
 * @return SUCCESS if the configuration was successful, else ERROR code.
 */
uint16_t GPT_Reset(GPTIMER_Config_t const *gptimer_config);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_GPTIMER_H_