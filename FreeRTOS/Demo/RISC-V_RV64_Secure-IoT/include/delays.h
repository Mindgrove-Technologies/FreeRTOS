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
 * Project                   : Secure IoT SoC
 * @file delays.h
 * @brief Contains apis for generating delays using mcycle.
 * @details Provides the API for generating delays in milliseconds and microseconds using mcycle.
 * @version 1.1
 * @authors Deeptha G      (deeptha@mindgrovetch.in)
 * @date 17-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 08-07-2025 | 1.0     | Deeptha G             | Implement delayms using millis.
 * 17-02-2026 | 1.1     | Deeptha G             | Implement delayms using mcycle
 *            |         |                       | and Updated it to be
 *            |         |                       | MISRA-compliant.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_DELAYS_H_
#define BSP_INCLUDE_DELAYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io.h"

/**
 * @brief The function `Delay_MS` is used to create a delay in milliseconds.
 * @details This function utilizes the RISC-V `mcycle` CSR to perform a busy-wait
 * loop based on the CPU frequency, stalling execution for the requested
 * duration in milliseconds.
 * @param delayms The duration of the delay in milliseconds (ms).
 * @return void
 */
void Delay_MS(uint32_t delayms);

/**
 * @brief The function `Delay_US` is used to create a delay in microseconds.
 * @details This function utilizes the RISC-V `mcycle` CSR to perform a busy-wait
 * loop based on the CPU frequency, stalling execution for the requested
 * duration in microseconds.
 * @param delayus The duration of the delay in microseconds (us).
 * @return void
 */
void Delay_US(uint32_t delayus);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_DELAYS_H_
