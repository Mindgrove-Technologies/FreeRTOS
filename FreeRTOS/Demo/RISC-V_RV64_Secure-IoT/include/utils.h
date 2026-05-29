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
 * @file utils.h
 * @brief  Contains the APIs for basic utility functions and millis.
 * @details Provides the APIs for reading and writing data as well as for calculating millis.
 * @version 1.1
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in)
 *          Deeptha G      (deeptha@mindgrovetech.in)
 *          Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 *          Natarajan S (natarajan@mindgrovetech.in)
 * @date 03-03-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 13-09-2024 | 1.0     | Kapil Shyam. M        | Initial release.
 * 17-02-2026 | 1.1     | Deeptha G             | Updated it to be
 *            |         |                       | MISRA-compliant.
 * 23-02-2026 | 1.2     | Vignesh Kumar J       | Added a generalised timeout
 *            |         |                       | function and supporting
 *            |         |                       | macros.
 * 03-03-2026 | 1.3     | Natarajan S           | Added macros to read and write
 *                                                CSRs.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_UTILS_H_
#define BSP_INCLUDE_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "log.h"
#include "clint.h"
#include "errors.h"

/**
 * @defgroup DIV_HELPERS Integer Division Helpers
 *
 * @brief Utility macros for performing ceiling and floor integer
 * division.
 *
 * @{
 */

/** @brief Performs ceiling division: (x + y - 1) / y */
#define CEIL_DIV_US(x, y) ((volatile uint16_t)(((x) + (y) - 1U) / (y)))

/** @brief Performs floor division: x / y */
#define FLOOR_DIV_US(x, y) ((volatile uint16_t)((x) / (y)))

/**
 * @brief Generates a bitmask with the lowest @p n bits set.
 *
 * Expands to (2^n - 1).
 * Example: n = 3 → 0b00000111
 *
 * @param n Number of least significant bits to set.
 *
 * @return Bitmask with lowest n bits set to 1.
 */
#define POW2_MINUS1(n)   ((1U << (n)) - 1U)

/** @} */ /* end of DIV_HELPERS */

/**
 * @defgroup GUARD_MACROS Safety Guard Macros
 * @brief Runtime checks to ensure pointer validity and safe execution.
 * @{
 */
/** @brief Checks if a pointer is NULL and returns EFAULT if true. */
#define CHECK_NULL(ptr)                          \
do {                                             \
    if (!(ptr)) {                                \
        log_emit(ERROR, #ptr " is NULL\n\r");      \
        return EFAULT;                           \
    }                                            \
} while (0)

/**
 * @brief Checks whether an address is aligned to a given size.
 *
 * Casts the input pointer to `uint8_t*` first to avoid a direct
 * `void*` to integer cast (MISRA compliance). It is then cast to
 * `uintptr_t` so that alignment masking can be performed safely
 * using an integer type capable of holding a pointer.
 *
 * @param addr Pointer/address to check.
 * @param size Alignment size (must be power of 2).
 *
 * @return true if aligned, false otherwise.
 */
#define IS_ALIGNED(addr, size) \
    ((((uintptr_t)(const uint8_t *)(addr)) & ((size) - 1U)) == 0U)

/** @} */ /* end of GUARD_MACROS */

/**
 * @defgroup CSR_MACROS Control and Status Register (CSR) Access Macros
 * @brief Low-level macros for reading from and writing to Control and 
 * Status Registers.
 * 
 * @{
 */

/**
 * @brief Reads the value of a Control and Status Register (CSR).
 *
 * The destination must be a variable compatible with the target
 * register width.
 *
 * @param csr  The CSR name (e.g., pmpaddr0, pmpcfg0).
 * @param dest Variable where the CSR value will be stored.
 */
#define CSR_READ(csr, dest) \
    __asm__ volatile ("csrr %0, " #csr : "=r"(dest))

/**
 * @brief Writes a value to a Control and Status Register (CSR).
 *
 * The value must be compatible with the target register width.
 *
 * @param csr  The CSR name (e.g., pmpaddr0, pmpcfg0).
 * @param val  Value to write to the CSR.
 */
#define CSR_WRITE(csr, val) \
    __asm__ volatile ("csrw " #csr ", %0" :: "r"(val))
/** @} */  /* end of CSR_MACROS */

/**
 * @defgroup Data_Size Register access
 * @brief Data width definitions.
 *
 * This module defines the data width modes used for register access.
 *
 * @{
 */
/**
 * @brief 8-bit data width mode.
 *
 * Selects 8-bit access for registers.
 */
#define DATA_SIZE_8   0x01U
/**
 * @brief 16-bit data width mode.
 *
 * Selects 16-bit access for registers.
 */
#define DATA_SIZE_16  0x02U
/**
 * @brief 32-bit data width mode.
 *
 * Selects 32-bit access for registers.
 */
#define DATA_SIZE_32  0x03U
/**
 * @brief 64-bit data width mode.
 *
 * Selects 64-bit access for registers.
 */
#define DATA_SIZE_64  0x04U
/** @} */  /* end of Data_Size */

/**
 * @defgroup BIT_CHECK_UTILS Bit Check Utilities
 * @brief Bit condition checking utilities.
 *
 * This module provides macros to check whether a specific bit or bit field
 * in a memory-mapped register matches a given condition (set or clear).
 *
 * The macro reads the register using the selected data width and evaluates
 * whether the masked bit value matches the specified condition.
 *
 * @{
 */

/**
 * @ingroup BIT_CHECK_UTILS
 *
 * @brief Check bit condition (set/clear) for a memory-mapped register.
 *
 * This macro reads a value from the given address using the specified
 * data width, applies the provided bit mask, and evaluates whether the
 * masked bit match the expected condition.
 *
 * It supports multiple access sizes (8, 16, 32, and 64-bit) and performs
 * the appropriate type casting internally.
 *
 * @param[in] addr  Address of the register or memory location.
 * @param[in] mask  Bit mask indicating which bit to check.
 * @param[in] size  Data access size:
 *                  - DATA_SIZE_8
 *                  - DATA_SIZE_16
 *                  - DATA_SIZE_32
 *                  - DATA_SIZE_64
 * @param[in] cond  Expected condition:
 *                  - 1U (BIT_SET)   : Check if bit is set
 *                  - 0U (BIT_CLEAR) : Check if bit is clear
 *
 * @return
 * - 1U : If the specified condition is satisfied
 * - 0U : If the condition is not satisfied or invalid size is provided
 *
 * @note
 * - The address is cast internally to a volatile pointer of the specified size.
 * - The address must be properly aligned for the given data size.
 * - The mask should correspond to the register width.
 * - Invalid size values result in a return value of 0U.
 *
 * @warning
 * - Misaligned access may lead to hardware faults.
 * - Ensure that the address points to a valid memory-mapped register.
 *
 * @par Example
 * @code
 * Wait until bit becomes clear (cond = 0U)
 * while (CHECK_BIT(addr, mask, DATA_SIZE_32, 0U) == 0U);
 *
 * Check if bit is set
 * if (CHECK_BIT(addr, mask, DATA_SIZE_32, 1U))
 * {
 *     bit is set
 * }
 * @endcode
 */
#define CHECK_BIT(addr, mask, size, cond)                           \
(                                                                   \
    (size) == DATA_SIZE_8  ?                                        \
    ((((*(volatile uint8_t  *)(addr)) & (mask)) != 0U) == (cond)) : \
    (size) == DATA_SIZE_16 ?                                        \
    ((((*(volatile uint16_t *)(addr)) & (mask)) != 0U) == (cond)) : \
    (size) == DATA_SIZE_32 ?                                        \
    ((((*(volatile uint32_t *)(addr)) & (mask)) != 0U) == (cond)) : \
    (size) == DATA_SIZE_64 ?                                        \
    ((((*(volatile uint64_t *)(addr)) & (mask)) != 0U) == (cond)) : \
    0U                                                              \
)
/** @} */  /* end of BIT_CHECK_UTILS */

/**
 * @brief Returns the value stored at a given address
 * @param  addr The address at which value is located
 * @return uint64_t value
 */
uint64_t Read_Data(const volatile size_t  *addr);

/** 
 * @brief The function `Write_Data` writes a value to a specific memory address.
 * @details This function performs a direct memory write operation.
 * @param addr The target memory address to which data will be written.
 * @param val  The value to be written to the address.
 */
void Write_Data(volatile size_t *addr, size_t val);

/**
 * @brief The function `Millis_Init` initializes the millisecond counter.
 * @details This function initializes the timer hardware and updates the
 * mtimecmp value to begin generating interrupts for elapsed time calculation.
 */
void Millis_Init(void);

/** 
 * @brief The function `Millis` returns the milliseconds since the program started.
 * @details It retrieves the current value of a global millisecond counter
 * which is incremented within the CLINT handler.
 * @return The number of milliseconds elapsed since initialization.
 */
uint64_t Millis(void);

/**
 * @brief The function `Exit` terminates the calling process with a status code.
 * @details This function implements process termination for RISC-V using the
 * SBI System Reset Extension. It flushes memory via a FENCE instruction
 * and calls SBI function ID 93.
 * @param status Exit status code (0 for success, non-zero for errors).
 * @note This function is guaranteed not to return. If SBI is unavailable,
 * it falls back to an infinite Wait-For-Interrupt (WFI) loop.
 */
void Exit(int status);

/**
 * @brief Wait until a specific register bit matches the expected condition
 *        or a timeout occurs.
 *
 * @details This function continuously polls a memory-mapped register at the
 *          specified address. The register is accessed using the given data
 *          width (8/16/32/64-bit), and the provided bit mask is applied.
 *
 *          The function checks whether the resulting bit state matches the
 *          expected condition (`cond`). It exits successfully when the 
 *          condition is met.
 *
 *          A timeout mechanism is implemented using the RISC-V `mcycle`
 *          CSR (machine cycle counter). The function tracks elapsed CPU cycles
 *          from the start and returns a timeout error if the specified limit
 *          is exceeded.
 *
 * @param address Memory-mapped register address to monitor.
 *                (Must be the actual address, e.g., (uintptr_t)&REG->FIELD)
 *
 * @param bit_mask Bit mask used to isolate the target bit(s)
 *                 (e.g., (1U << bit_position)).
 *
 * @param cond Expected bit condition:
 *             - 0U : Wait until the bit becomes clear
 *             - 1U : Wait until the bit becomes set
 *
 * @param size Data width used for register access:
 *             - DATA_SIZE_8   : 8-bit access
 *             - DATA_SIZE_16  : 16-bit access
 *             - DATA_SIZE_32  : 32-bit access
 *             - DATA_SIZE_64  : 64-bit access
 *
 * @param timeout Maximum number of CPU cycles to wait before timing out.
 *                This is compared against (current_mcycle - start_mcycle).
 *
 * @return
 *         SUCCESS     : Condition met within timeout
 *         ETIMEDOUT   : Timeout occurred before condition was met
 *
 * @note This function uses a blocking polling loop and does not yield CPU
 *       control. Use carefully in time-critical or RTOS-based environments.
 *
 * @warning Ensure that the register address (not the register value) is passed.
 *          Passing a value instead of an address will result in invalid memory
 *          access.
 * 
 * @example Wait until a BUSY bit clears
 * @code
 * if (Wait_For_Timeout((uintptr_t)&PERIPH->STATUS,
 *                      BUSY_BIT_MASK,
 *                      0U,
 *                      DATA_SIZE_32,
 *                      TIMEOUT_CYCLES) == SUCCESS)
 * {
 *     Peripheral is ready
 * }
 * @endcode
 * 
 * @example Wait until a READY bit is set
 * @code
 * if (Wait_For_Timeout((uintptr_t)&PERIPH->STATUS,
 *                      READY_BIT_MASK,
 *                      1U,
 *                      DATA_SIZE_32,
 *                      TIMEOUT_CYCLES) == SUCCESS)
 * {
 *     Peripheral is ready
 * }
 * @endcode
 * 
 */

uint16_t  Wait_For_Timeout(uintptr_t address,
                           uint32_t bit_mask,
                           uint8_t cond,
                           uint8_t size,
                           uint64_t timeout);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_UTILS_H_
