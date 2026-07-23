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
 * @file qspi_psram_driver.h
 * @brief Declarations for PSRAM driver using QSPI.
 * @details This header file provides the public APIs,
 *          to configure and operate PSRAM through the QSPI interface.
 * @version 1.0
 * @authors Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 *          Narasimha R V (narasimha@mindgrovetech.in)
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

#define PSRAM_DEFAULT_PRESCALER    20U

/**
 * @brief Initialize PSRAM in memory-mapped RAM mode.
 *
 * @details
 * This function configures the QSPI peripheral to interface
 * with the external PSRAM device using Memory-Mapped Mode (MMM)
 * with RAM mode enabled.
 *
 * After successful initialization:
 * - PSRAM can be accessed like normal SRAM
 * - Standard pointer-based read/write operations can be used
 * - No explicit QSPI read/write commands are required
 * - CPU can directly access PSRAM through the mapped address range
 *
 * The function internally configures:
 * - Memory-mapped mode (MMM)
 * - RAM mode selection
 * - Read instruction and dummy cycles
 * - Write instruction and dummy cycles
 * - Device configuration parameters required for PSRAM access
 *
 * This allows the external PSRAM to behave like internal memory
 * from the software point of view.
 *
 * Example:
 * After initialization:
 *
 * volatile uint32_t *psram = (uint32_t *)PSRAM_BASE_ADDR;
 * psram[0] = 0xCAFEBABE;
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance used to interface
 * with the PSRAM device.
 *
 * @param[in] psram_mem_size
 * PSRAM size configuration value used to program the
 * memory size field of the QSPI controller.
 *
 * Example:
 * If PSRAM size is 8 MB:
 * psram_mem_size = 22
 * because:
 * Memory Size = 2^(psram_mem_size + 1)
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * The configured frequency must not exceed the maximum
 * operating frequency supported by the PSRAM device.
 *
 * @return
 * - error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Before starting a new indirect QSPI transaction after PSRAM
 * memory-mapped mode is enabled, the user must call
 * @ref QSPI_Abort_Transaction() to exit memory-mapped mode
 * and allow peripheral reconfiguration.
 *
 */
uint16_t PSRAM_Init(const QSPI_Instance_t *qspi_inst,
                    size_t psram_mem_size,
                    uint8_t prescaler);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_QSPI_PSRAM_H_
