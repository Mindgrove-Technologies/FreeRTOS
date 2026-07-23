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
 * @file wdtimer.h
 * @brief This is Baremetal Header file for Mindgrove Silicon's Watchdog Timer.
 * @details The Watchdog Timer header has software function declarations to
 *          configure and use Watchdog Timer.
 * @version 1.1
 * @authors Harini Sree.S
 * @date 22-01-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 18-03-2025 | 1.0     | Harini Sree.S         | Initial release.              
 * 22-01-2026 | 1.1     | Harini Sree.S         | SecureIOT Production changes
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_WDTIMER_H_
#define BSP_INCLUDE_WDTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup WDT_MACROS WDT Configuration Macros
 *
 * @brief Macros used to configure the WDT mode.
 *
 * @{
 */

/** @brief Macro is used to mention the WDT HARD reset mode
 *  
 *  @note Resets the core after downcounting the value written in wcycles.
*/
#define HARD_RESET    0
/** @brief Macro is used to mention the WDT SOFT reset mode
 *  
 *  @note  Resets the core instantly.
*/
#define SOFT_RESET    1

/** @} */

/**
 * @brief Start the watchdog timer.
 *
 * @details The function Wdtimer_start enables the watchdog timer and configures
 * the reset behavior based on the selected mode (hard reset or soft reset).
 * The timer will trigger a reset after the specified number of seconds.
 *
 * @param mode Boolean value to select the watchdog reset mode: hard reset or
 * soft reset.
 *
 * @param wcycles Number of seconds before the reset is triggered.
 * Ensure the value, when multiplied by the base frequency, does not exceed the
 * 64-bit range.
 * 
 * @note wcycles is applicable only for hard reset, as soft reset occurs
 * immediately.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS- Watchdog timer is started successfully.
 * - @ref EINVAL- wcycles exceeds the 64-bit range when multiplied with base frequency.
 */
uint16_t WDT_Start(bool mode, uint64_t wcycles);

/**
 * @brief The function wdtimer_disable disables the Watchdog Timer.
 *
 * @details The function wdtimer_disable stops the Watchdog Timer by clearing
 * the enable bit in the control register, after which the watchdog no longer
 * monitors software execution or generates reset events.
 *
 * @param None
 *
 * @return Returns a 16-bit status code:
 * - @ref Success on successfully disabling the Watchdog Timer.
 */
uint16_t WDT_Disable(void);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_WDTIMER_H_
