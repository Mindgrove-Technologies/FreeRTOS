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
 * @details Provides the APIs for hardware control of General Purpose Timer
 * (GPTimer) including initialization/configuration, counter reset, counter/PWM disable, and reading the capture input register.
 * @version 1.1
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 *          Deeptha G             (deeptha@mindgrovetech.in)
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
 * @brief GPTimer Operational Modes
 * @details Enum that selects the GPTIMER mode: PWM mode or one of the counting
 * modes.
 */
typedef enum {
    GPT_PWM_MODE = 0U,    /**< Pulse Width Modulation mode */
    GPT_UP_COUNT = 1U,    /**< Up-counting mode */
    GPT_DOWN_COUNT = 2U,  /**< Down-counting mode */
    GPT_UPDOWN_COUNT = 3U /**< Up-Down counting mode */
} GPTIMER_Mode_t;

/**
 * @brief Interrupt Configuration Options for the GPTIMER.
 * @details Selects how GPTIMER interrupts are configured:
 *          - Disabled: no interrupts are generated.
 *          - General enable: interrupts are enabled (except for pwm mode).
 *          - PWM-specific options: enable interrupts only on PWM rising edges,
 * only on PWM falling edges, or on both rising and falling edges.
 */
typedef enum {
    GPT_INTR_DISABLE = 0x00U, /* Interrupts disabled. */
    GPT_INTR_EN = 0x01U, /*Interrupts enabled (for non-PWM counter modes) */
    GPT_PWM_RISE_INTR =
        0x02U, /* Enable interrupts only on PWM rising edges. */
    GPT_PWM_FALL_INTR =
        0x03U, /* Enable interrupts only on PWM falling edges. */
    GPT_PWM_RISE_FALL_INTR =
        0x04U /* Enable interrupts on both PWM rising and falling edges. */
} GPTIMER_Interrupt_Opt_t;

/**
 * @defgroup GPTIMER_Instance_Type GPTIMER Instance Handle Type
 * @brief Opaque GPTIMER hardware instance handle type.
 *
 * This module defines the GPTIMER instance handle type used by the GPTIMER
 * driver. The type is opaque and represents a specific GPTIMER hardware block.
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
const GPTIMER_Instance_t* GPTIMER_INSTANCE_0(void);

/**
 * @brief Get handle for GPTIMER instance 1.
 * @return Pointer to GPTIMER1 hardware instance descriptor.
 */
const GPTIMER_Instance_t* GPTIMER_INSTANCE_1(void);

/**
 * @brief Get handle for GPTIMER instance 2.
 * @return Pointer to GPTIMER2 hardware instance descriptor.
 */
const GPTIMER_Instance_t* GPTIMER_INSTANCE_2(void);

/**
 * @brief Get handle for GPTIMER instance 3.
 * @return Pointer to GPTIMER3 hardware instance descriptor.
 */
const GPTIMER_Instance_t* GPTIMER_INSTANCE_3(void);

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
 * This enumeration defines the available GPTIMER interrupt numbers supported by
 * the platform.
 */
typedef enum {
    /* =========================================  Secure_IoT Specific Interrupt
       Numbers  ========================================= */
    GPTIMER0_IRQn = 47, /*!< 47 GPTIMER0 */
    GPTIMER1_IRQn = 48, /*!< 48 GPTIMER1 */
    GPTIMER2_IRQn = 49, /*!< 49 GPTIMER2 */
    GPTIMER3_IRQn = 50, /*!< 50 GPTIMER3 */
} GPT_IRQn_Type;

/**
 * @brief GPTIMER configuration structure.
 *
 * This structure contains all parameters required to initialize
 * and configure a General Purpose Timer (GPTIMER).
 */
typedef struct {
    /**
     * @brief GPTIMER hardware instances.
     *
     * This parameter selects the GPTIMER hardware instance to be used.
     * Valid instances are @ref GPTIMER0, @ref GPTIMER1, @ref GPTIMER2 and @ref
     * GPTIMER3.
     *
     */
    const GPTIMER_Instance_t* gpt_num;
    /**
     * @brief GPTIMER operating mode.
     *
     * @details Selects the counting mode (up,down,up-down) or PWM mode.
     * Valid values map to @ref GPTIMER_Mode_t (0..3).
     */
    GPTIMER_Mode_t mode : 2;
    /**
     * @brief Timer period value.
     *
     * Specifies the terminal count value for one timer cycle.
     * - Up-count mode : counts up to `period`, then overflows.
     * - Down-count mode : counts down to `period`, then underflows.
     * - Up-down mode: counts between the configured limits and turns around at the
     * period limit.
     * - Pwm mode: `period` defines the PWM period (the PWM time base).
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
     * @brief Interrupt enable option.
     *
     * @details Selects which GPTIMER interrupts are enabled.
     * Valid values map to @ref GPTIMER_Interrupt_Opt_t:
     * - 0: interrupts disabled
     * - 1: interrupts enabled (for any counter mode excluding PWM mode).
     * - 2: PWM rising-edge interrupts only
     * - 3: PWM falling-edge interrupts only
     * - 4: PWM rising- and falling-edge interrupts
     */
    GPTIMER_Interrupt_Opt_t interrupt_en : 3;

    /**
     * @brief Continuous count enable flag.
     *
     * @details Enables continuous counting for the GPTIMER (the timer continues
     * counting without stopping).
     *
     */
    bool cnt_en;
    /**
     * @brief Capture input selection.
     *
     * Selects which GPTIMER input event triggers a capture. When the selected
     * event occurs on the GPTIMER input, the current counter value is stored
     * into the capture input register.
     *
     * @details capture_val selects the capture event based on its value (0 or
     * 1).
     */
    uint8_t capture_val : 1;
    /**
     * @brief Output enable flag.
     *
     * @details Enable the output signal in PWM mode by setting
     * the output enable bit in the control register. For
     * non-PWM modes, this flag is ignored.
     */
    bool output_en;
} GPTIMER_Config_t;

/**
 * @brief GPT_Init initializes the GPTIMER with the selected mode and configures
 * required registers.
 * @details Programs the GPTIMER instance settings including mode, prescaler,
 * period, duty cycle, continuous counting enable, capture input selection, and
 * output enable.
 *
 * @param gptimer_config Pointer to a GPTIMER_Config_t structure used to
 * configure the GPTIMER instance.
 *
 * @return 16-bit status code.
 * - @ref SUCCESS Configuration applied successfully.
 * - @ref EFAULT Returned if gptimer_config is NULL.
 * - @ref EPERM Returned if prescaler is invalid (e.g., prescaler == 0).
 * - @ref EINVAL Returned if invalid configuration values are provided (e.g.,
 * mode or interrupt_en).
 */
uint16_t GPT_Init(GPTIMER_Config_t const* gptimer_config);

/**
 * @brief The function `GPT_Reset` resets the counter, interrupts and register
 * values.
 * @details Resets the GPTIMER counter and related GPTIMER state for the
 * selected GPTIMER instance, including interrupt and register settings as
 * applicable.
 *
 * @param gptimer_config Pointer to a GPTIMER_Config_t structure used to
 * select/configure the GPTIMER instance.
 *
 * @return 16-bit status code.
 * - @ref SUCCESS Reset completed successfully.
 * - @ref EFAULT Returned by if gptimer_config is NULL.
 */
uint16_t GPT_Reset(GPTIMER_Config_t const* gptimer_config);

/**
 * @brief The function `GPT_Disable` disables the GPTIMER instance.
 * @details Disables the GPTIMER counter, PWM output enable, and clears GPTIMER
 * interrupt enable bits for the selected GPTIMER instance.
 *
 * @param gptimer_config Pointer to a GPTIMER_Config_t structure used to
 * select/configure the GPTIMER instance.
 *
 * @return 16-bit status code.
 * - @ref SUCCESS Disabled successfully.
 * - @ref EFAULT Returned if gptimer_config is NULL.
 */
uint16_t GPT_Disable(GPTIMER_Config_t const* gptimer_config);

/**
 * @brief The function `GPT_Read_Capture_Input` reads the captured value from
 * the GPTIMER capture register.
 * @details Reads the captured value generated by the GPTIMER capture event and
 * writes it to captured_value.
 *
 * @param gptimer_config Pointer to a GPTIMER_Config_t structure used to
 * select/configure the GPTIMER instance.
 * @param capture_val Pointer to a uint16_t variable where the captured value
 * will be stored.
 *
 * @return 16-bit status code.
 * - @ref SUCCESS Captured value read successfully.
 * - @ref EFAULT Returned if gptimer_config is NULL.
 */
uint16_t GPT_Read_Capture_Input(GPTIMER_Config_t const* gptimer_config,
                                uint16_t* captured_value);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_GPTIMER_H_
