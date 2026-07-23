/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2023-2026 Mindgrove Technologies. All rights reserved.
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
 * @file interrupt_control.h
 * @brief Interrupt control interface for RISC-V core trap handling.
 * @details This header file declares the public interface for the interrupt
 * control driver. It provides mode selection enumerations and APIs to
 * configure vectored and non-vectored trap handling for the RISC-V core.
 * @version 1.1
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Akash R B      (akash@mindgrovetech.in)
 * @date 27-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 07-07-2025 | 1.0     | Vishwajith N S        | Initial implementation.
 * 27-02-2026 | 1.1     | Akash R B             | MISRA compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE INTERRUPT_CONTROL_H_
#define BSP_INCLUDE_INTERRUPT_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @defgroup INTERRUPT_ENUMS Interrupt Enumerated Types
 * @{
 */

/**
 * @brief Interrupt mode selection for RISC-V core trap handling.
 * @details Defines the operating modes of the trap handling mechanism
 * in the RISC-V core. Controls how the mtvec register routes
 * traps to handler addresses.
 */
typedef enum
{
    /**
     * @brief Direct (non-vectored) mode.
     *
     * All traps (interrupts and exceptions) are routed to a single
     * entry point. Software is responsible for decoding mcause and
     * dispatching the appropriate handler.
     */
    NON_VECTORED_MODE = 0U,

    /**
     * @brief Vectored interrupt mode.
     *
     * Interrupts are automatically dispatched to specific handler
     * addresses based on the interrupt cause. The hardware computes
     * the handler address as: BASE + (4 × cause).
     */
    VECTORED = 1U

} interrupt_mode_t;

/** @} */

/* --- Interrupt Control Driver APIs --- */

/**
 * @defgroup INTERRUPT_API Interrupt Control Driver APIs
 * @{
 */

/**
 * @brief Non-vectored trap entry handler.
 *
 * @details This function is invoked automatically by the core when a trap
 * (interrupt or exception) occurs in non-vectored mode. It performs
 * the complete trap handling sequence as follows:
 * - Saves all general-purpose register values onto the stack.
 * - Reads the mcause register to determine the trap cause.
 * - If the trap is an interrupt, fetches and executes the corresponding
 *   handler from the machine interrupt table.
 * - If the trap is an exception, invokes the appropriate fault handler.
 * - Restores all register values from the stack before returning.
 *
 * @return None
 *
 * @note This function is triggered automatically by hardware on any trap.
 *       Users may override this function to implement custom trap handling,
 *       but must ensure proper context save and restore to maintain
 *       system stability.
 *
 * @warning Improper override without correct context save and restore
 *          will result in undefined system behaviour.
 */
void non_vectored_trap_entry(void);

/**
 * @brief Switches the core interrupt handling mode.
 *
 * @details Configures the interrupt handling behavior of the core
 * based on the selected mode. This may control how interrupts are
 * dispatched (e.g., direct mode vs vectored mode in RISC-V).
 *
 * @param mode Interrupt mode selection.
 * 
 * Possible values:
 * - 0U : Direct mode (all interrupts go to a single handler)
 * - 1U : Vectored mode (interrupts dispatched via vector table)
 *
 * @note Ensure interrupts are disabled before changing the mode
 * to avoid undefined behavior.
 */
void core_switch_interrupt_mode(uint8_t mode);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_INTERRUPT_CONTROL_H_