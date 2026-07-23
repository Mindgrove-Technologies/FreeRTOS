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
 * @copyright Copyright (c) 2026 Mindgrove Technologies.
 * All rights reserved.
 * @file pwm.h
 * @brief Contains APIs for configuring and resetting the PWM functionality.
 * @details Provides APIs for hardware control of Pulse Width Modulation(PWM) .
 * @version 1.1
 * @authors Harini Sree.S
 * @date 22-01-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 24-04-2025 | 1.0     | Harini Sree.S         | Initial release.              
 * 22-01-2026 | 1.1     | Harini Sree.S         | SecureIOT Production changes
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PWM_H_
#define BSP_INCLUDE_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup PWM_MACROS PWM Configuration Macros
 *
 * @brief Macros used to configure specific instance of the PWM channel.
 *
 * @{
 */

/** @brief Macro is used to mention the PWM instances needed 
 *  @note Enter a number between 0-13 as x
*/
#define PWM_PIN(x)  ((1U) << (x))

/** @} */

/**
 * @enum PWM_Interrupt_Modes
 *
 * @brief PWM interrupt configuration modes.
 *
 * @details
 * This enumeration defines the available interrupt trigger modes
 * for the PWM peripheral.
 */
typedef enum {
    /** Disable all PWM interrupts */
    PWM_INTR_NONE       = 0x00U,

    /** Enable interrupt on rising edge */
    PWM_INTR_RISE       = 0x01U,

    /** Enable interrupt on falling edge */
    PWM_INTR_FALL       = 0x02U,

    /** Enable interrupt on half-period event */
    PWM_INTR_HALFPERIOD = 0x03U
} PWM_Interrupt_Modes;

/**
 * @enum PWM_IRQn_Type
 * 
 * @brief PWM interrupt ID
 * 
 * This enumeration defines the available PWM interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  PWM0_IRQn                 =  33,              /*!< 33 PWM0                                                                   */
  PWM1_IRQn                 =  34,              /*!< 34 PWM1                                                                   */
  PWM2_IRQn                 =  35,              /*!< 35 PWM2                                                                   */
  PWM3_IRQn                 =  36,              /*!< 36 PWM3                                                                   */
  PWM4_IRQn                 =  37,              /*!< 37 PWM4                                                                   */
  PWM5_IRQn                 =  38,              /*!< 38 PWM5                                                                   */
  PWM6_IRQn                 =  39,              /*!< 39 PWM6                                                                   */
  PWM7_IRQn                 =  40,              /*!< 40 PWM7                                                                   */
  PWM8_IRQn                 =  41,              /*!< 41 PWM8                                                                   */
  PWM9_IRQn                 =  42,              /*!< 42 PWM9                                                                   */
  PWM10_IRQn                =  43,              /*!< 43 PWM10                                                                  */
  PWM11_IRQn                =  44,              /*!< 44 PWM11                                                                  */
  PWM12_IRQn                =  45,              /*!< 45 PWM12                                                                  */
  PWM13_IRQn                =  46,              /*!< 46 PWM13                                                                  */
} PWM_IRQn_Type;

/**
 * @brief PWM configuration structure.
 *
 * @details
 * This structure defines all configurable parameters required to
 * initialize and control a PWM instance, including timing,
 * interrupt behavior, output polarity, and deadband settings.
 *
 * @note
 * The duty value must be less than or equal to the period value.
 * The prescaler and deadband parameters must be configured according
 * to the target hardware capabilities.
 */
typedef struct {
    /**
     * @brief PWM duty cycle value
     * 
     * The duty value must be less than or equal to the period value
     */
    uint32_t duty;

    /**
     * @brief PWM period value
     * 
     * The period value must be higher than zero
     */
    uint32_t period;

    /**
     * @brief PWM interrupt mode configuration
     * 
     * The interrupt mode must be between 0-3
     */
    PWM_Interrupt_Modes interrupt_mode;

    /**
     * @brief Output polarity control flag
     */
    bool change_output_polarity;

    /**
     * @brief Prescaler value for PWM clock division
     * 
     * The prescaler value must be lesser than 32768U
     */
    uint16_t prescalar_value;

    /**
     * @brief PWM Deadband delay value
     * 
     * The deadband value must be within duty cycle value
     */
    uint32_t deadband_delay;
} PWM_Config_t;

/**
 * @brief Starts a specific PWM module.
 *
 * @details This function initializes and enables the specified PWM module
 * using the provided configuration parameters and activates the
 * selected PWM output channels.
 *
 * @param config Pointer to a PWM_Config_t structure containing the parameters
 * required to configure the PWM signal.
 *
 * @param pwm_pins Bitwise OR’ed value of PWM pin identifiers representing the
 * PWM output channels to be enabled.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if the PWM module is started successfully
 * - @ref ENODEV if the pwm_pins given, is not within 0-13
 * - @ref EINVAL if the configuration values are out of range
 */
uint16_t PWM_Start(PWM_Config_t *config, uint32_t pwm_pins);

/**
 * @brief Stops a specific PWM module.
 *
 * @details This function disables the specified PWM output channels and
 * stops PWM signal generation for the selected pins.
 *
 * @param pwm_pins Bitwise OR’ed value of PWM pin identifiers representing the
 * PWM output channels to be stopped.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if the PWM module is stopped successfully
 * - @ref ENODEV if the pwm_pins given, is not within 0-13
 */
uint16_t PWM_Stop(uint32_t pwm_pins);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_PWM_H_
