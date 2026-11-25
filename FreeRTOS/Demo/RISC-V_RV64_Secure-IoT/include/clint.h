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
 * @file clint.h
 * @brief Contains driver apis for CLINT Interface
 * @details Provides the API for configuring CLINT timers and counters.
 * @version 1.1
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in)
 *          Deeptha G      (deeptha@mindgrovetech.in)
 * @date 17-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 07-02-2024 | 1.0     | Kapil Shyam. M        | Initial release.
 * 17-02-2026 | 1.1     | Deeptha G             | Optimized driver and updated
 *            |         |                       | it to be MISRA-compliant.
 * -----------------------------------------------------------------------------
 */

#ifndef  BSP_INCLUDE_CLINT_H_
#define  BSP_INCLUDE_CLINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "log.h"


/**
 * @defgroup CLINT_MACROS CLINT Macros
 * @brief Constant definitions used by CLINT utility functions.
 * @{
 */
/**
 * @brief Clock divisor applied to the CLINT input clock.
 *
 * This value represents the division factor used to derive the
 * effective CLINT timer clock from the system clock.
 */
#define CLINT_DIVISOR 2U
/** @} */

/**
 * @brief The function `CLINT_Get_MTIME` gets the current machine timer value (MTIME).
 * 
 * @details Reads the CLINT MTIME register and stores the 64-bit value into
 * the provided pointer.
 * 
 * @param mtime_val Pointer to a 64-bit variable that will receive the MTIME
 *  value.
 * @return @ref SUCCESS MTIME value successfully read.
 * 
 */
uint16_t CLINT_Get_MTIME(uint64_t *mtime_val);

/**
 * @brief The function `CLINT_Config_Counter` sets up the CLINT timer threshold.
 * @details Programs the mtimecmp register to trigger the next timer
 * interrupt at (current MTIME + value).
 * @param value Value added to the current MTIME to determine the interrupt time.
 * @return @ref SUCCESS On successful configuration.
 */
uint16_t CLINT_Config_Counter(uint64_t value);

/**
 * @brief The function `CLINT_Timer` configures the CLINT timer and triggers a software interrupt.
 * @details This function:
 * - Triggers a software interrupt by writing to the MSIP register.
 * - Disables specific interrupts by clearing bits in the MIE register.
 * - Enables global interrupts by setting the appropriate bits in MSTATUS.
 * - Configures the next timer interrupt threshold using @ref CLINT_Config_Counter.
 * @param timer_value Value passed to @ref CLINT_Config_Counter to set mtimecmp.
 * @return @ref SUCCESS Timer configured and interrupt handling updated successfully.
 */
uint16_t CLINT_Timer(uint64_t timer_value);

/**
 * @brief Weak handler for the Machine Timer Interrupt.
 *
 * @details Sets MTIMECMP0 to a maximum value so the timer
 *          interrupt won’t fire again until MTIMECMP0 is updated.
 *
 */
__attribute__((weak)) void on_machine_timer_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif  //  BSP_INCLUDE_CLINT_H_
