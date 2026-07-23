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
 *          Deeptha G      (deeptha@mindgrovetch.in)
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
#include "secure_iot.h"
#include "log.h"

/**
 * @defgroup CLINT_REGISTERS CLINT Hardware Register Pointers
 * @brief External pointers to the memory-mapped CLINT register interface.
 * @{
 */
/** @brief Machine Software Interrupt Pending register. */
extern volatile uint64_t *msip;
/** @brief Machine Time register. */
extern volatile uint64_t *mtime;
/** @brief Machine Time Compare register. */
extern volatile uint64_t *mtimecmp;
/** @} */

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
 * @brief The function `Get_MTIME` returns the current 64-bit machine time.
 * @details It returns the mtime value.
 * @return 64-bit unsigned integer representing the current ticks.
 */
uint64_t Get_MTIME(void);

/**
 * @brief The function `Config_Counter` sets up the CLINT timer threshold.
 * @details It is used to set the mtimecmp register to the current mtime value
 * plus the provided delta, scheduling the next timer interrupt.
 * @param value The delta value after which the interrupt happens.
 * @return SUCCESS if configuration was successful otherwise error code.
 */
void Config_Counter(uint64_t value);

/**
 * @brief The function `CLINT_Timer` configures the CLINT timer and triggers a software interrupt.
 * @par Detailed Description:
 * This function performs the following operations:
 * - Triggers a software interrupt by writing to the MSIP register.
 * - Disables specific interrupts by clearing bits in the MIE register.
 * - Enables global interrupts by setting bits in the MSTATUS register.
 * - Configures the next timer threshold using @ref Config_Counter.
 * @param timer_value The value to configure the timer via `Config_Counter`.
 * @return SUCCESS if the operation was successful otherwise error code.
 */
void CLINT_Timer(uint64_t timer_value);

#ifdef __cplusplus
}
#endif

#endif  //  BSP_INCLUDE_CLINT_H_