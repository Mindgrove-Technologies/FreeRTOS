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
 * @file qspi_psram_driver.h
 * @brief Declarations for PSRAM driver using QSPI.
 * @details This header file provides the public APIs,
 *          to configure and operate PSRAM through the QSPI interface.
 * @version 1.0
 * @authors Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 * @date 18-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by     | Description
 * -----------|---------|-----------------|-------------------------------------
 * 08-07-2023 | 1.0     | Vignesh Kumar J | Initial release.
 * -----------------------------------------------------------------------------
*/

#ifndef BSP_INCLUDE_QSPI_PSRAM_H_
#define BSP_INCLUDE_QSPI_PSRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "qspi.h"

/**
 * @brief Initializes the PSRAM device for memory-mapped operation.
 *
 * @details This function configures the PSRAM through the given QSPI
 *          instance and places it into RAM mode. After successful
 *          initialization, the PSRAM can be accessed like normal SRAM
 *          using standard memory read and write operations.
 *
 * @param[in] qspi_inst Pointer to the QSPI instance used to interface
 *                      with the PSRAM device.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */

uint16_t PSRAM_Init(const QSPI_Instance_t *qspi_inst);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_QSPI_PSRAM_H_
