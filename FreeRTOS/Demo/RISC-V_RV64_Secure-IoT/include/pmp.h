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
 * @file pmp.h
 * @brief Declarations for PMP configuration.
 * @details This header file provides function prototypes, macros,
 *          and data type definitions required to configure permissions
 *          and matching mode to protect memory regions using PMP registers.
 * @version 1.0
 * @authors Natarajan S (natarajan@mindgrovetech.in)
 * @date 18-02-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 18-02-2026 | 1.0     | Natarajan S           | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PMP_H_
#define BSP_INCLUDE_PMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @defgroup PMP_Limits PMP hardware limits
 * @brief Macros defining limitations of this PMP hardware
 * @{
 */

/** @brief Maximum number of PMP entries supported by the hardware */
#define PMP_MAX_ENTRIES   4U

/** @brief PMP address granularity in bytes */
#define PMP_GRANULARITY   8U

/** @} */  /* end of PMP_Limits */

/**
 * @defgroup PMP_Configuration PMP Entry Configuration macros
 *
 * @brief Used to configure PMP Entry attributes in pmpcfg0 register.
 *
 * These flags must be combined as required and passed to
 * @ref PMP_Set_Entry to program the corresponding fields
 * in the pmpcfg0 register.
 *
 * @{
 */

/** @brief Enables read access for the PMP Entry. */
#define PMP_READ_ACCESS      (1U << 0)

/** @brief Enables write access for the PMP Entry. */
#define PMP_WRITE_ACCESS     (1U << 1)

/** @brief Enables execute access for the PMP Entry. */
#define PMP_EXECUTE_ACCESS   (1U << 2)

/**
 * @brief Enables TOR (Top of Range) address matching mode.
 *
 * Uses the previous PMP address register as the lower bound
 * and the current PMP address register as the upper bound.
 */
#define PMP_TOR_MATCHING     (1U << 3)

/** @brief Enables NAPOT (Naturally Aligned Power-Of-Two) address 
 *         matching mode.*/
#define PMP_NAPOT_MATCHING   (3U << 3)

/** @brief Locks the PMP Entry configuration.*/
#define PMP_LOCK_BIT         (1U << 7)

/** @} */  /* end of PMP_Configuration */

/**
 * @brief Configure a PMP (Physical Memory Protection) Entry.
 *
 * @details This function sets up a PMP entry with the specified 
 *          configuration, entry number, base address, and size. 
 *          It writes the configuration to pmpcfg0 register,
 *          encodes and writes the address to pmpaddrX registers
 *          as per the given mode
 *
 * @param config  The PMP configuration byte, defining permissions 
 *                (read, write, execute), addressing mode (NAPOT, TOR)
 *                and lock bit. Use the macros for setting configuration
 * 
 * @param entry  The pmpaddrX register to be used for PMP Configuration(0-3)
 * @param address The actual address to be protected (8-byte aligned).           
 * @param size    The size of the memory region in bytes. Must be a power 
 *                of 2 for NAPOT addressing mode and 0 for TOR Mode.
 *
 * @return SUCCESS when PMP registers are configured, EINVAL for invalid
 *         arguments, EPERM if lock bit is set for given entry.
 */
uint16_t PMP_Set_Entry(uint8_t config, uint8_t entry, uint32_t* address, \
                        size_t size);

/**
 * @brief Clears a specific PMP entry.
 * 
 * @details Clears the pmpaddrX register and the fields of the pmpcfg0 register 
 *          for the given entry if the lock bit is not set.
 * 
 * @param entry  PMP entry which is to be cleared (0-3).
 *
 * @return SUCCESS when cleared, EINVAL for invalid PMP entry, 
 *         EPERM if lock bit for the given entry is set.
 */
uint16_t PMP_Clear_Entry(uint8_t entry);

/**
 * @brief Clears all PMP entries
 * 
 * @details This function resets all pmpaddrX registers and the pmpcfg0
 *          register. If any PMP entry has the lock bit set, the operation is 
 *          aborted without clearing any of the entries.
 *
 * @return SUCCESS when cleared, EPERM if any entry has its lock bit set.
 */
uint16_t PMP_Clear_All(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PMP_H_

