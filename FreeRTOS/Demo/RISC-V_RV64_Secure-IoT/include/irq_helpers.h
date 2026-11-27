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
 * @file irq_helpers.h
 * @brief Trap context save and restore helper macros.
 * @details This header file defines macros used for saving and restoring
 * processor context during interrupt and exception handling in a RISC-V
 * system. It provides:
 * - Macros for saving and restoring general-purpose integer registers.
 * - Macros for saving and restoring both integer and floating-point registers.
 *
 * These macros internally invoke low-level assembly routines to perform
 * stack-based context switching. They are typically used within interrupt
 * service routines (ISRs) and trap handlers.
 * @version 1.1
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Akash R B      (akash@mindgrovetech.in)
 * @date 07-07-2025
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 07-07-2025 | 1.0     | Vishwajith N S        | Initial implementation.
 * 27-02-2026 | 1.1     | Akash R B             | MISRA compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_IRQ_HELPERS_H_
#define BSP_INCLUDE_IRQ_HELPERS_H_

/* --- Context Save/Restore Macros --- */

/**
 * @defgroup IRQ_CONTEXT_INT Integer Context Save and Restore
 * @brief Macros for saving and restoring integer caller-saved registers.
 * @{
 */

/**
 * @brief Saves integer caller-saved registers onto the stack.
 *
 * @details Pushes the return address and invokes the low-level
 *          assembly routine Trap_entry() to save all integer
 *          caller-saved general-purpose registers onto the stack.
 *
 * @note Must be paired with a corresponding POP_CONTEXT macro.
 *
 * @warning Mismatched usage with POP_CONTEXT will cause stack
 *          corruption and unpredictable system behaviour.
 */
#define PUSH_CONTEXT do { \
    __asm__ volatile ("sd ra, -248(sp)\n\t"); \
    __asm__ volatile ("jal Trap_entry\n\t"); \
} while (0)

/**
 * @brief Restores integer caller-saved registers from the stack.
 *
 * @details Invokes the low-level assembly routine Trap_exit() to
 *          restore all integer caller-saved general-purpose registers
 *          previously saved by PUSH_CONTEXT.
 *
 * @note Must be paired with a corresponding PUSH_CONTEXT macro.
 *
 * @warning Mismatched usage with PUSH_CONTEXT will cause stack
 *          corruption and unpredictable system behaviour.
 */
#define POP_CONTEXT asm volatile("jal  Trap_exit\n\t")

/** @} */

/**
 * @defgroup IRQ_CONTEXT_INT_FLOAT Integer and Float Context Save and Restore
 * @brief Macros for saving and restoring integer and floating-point
 *        caller-saved registers.
 * @{
 */

/**
 * @brief Saves integer and floating-point caller-saved registers onto the stack.
 *
 * @details Invokes the low-level assembly routine Trap_entry_int_float()
 *          to save all integer and floating-point caller-saved registers
 *          onto the stack.
 *
 * @note Must be paired with a corresponding POP_CONTEXT_INT_FLOAT macro.
 *
 * @warning Mismatched usage with POP_CONTEXT_INT_FLOAT will cause stack
 *          corruption and unpredictable system behaviour.
 */
#define PUSH_CONTEXT_INT_FLOAT do { \
    __asm__ volatile ("jal Trap_entry_int_float\n\t"); \
} while (0)

/**
 * @brief Restores integer and floating-point caller-saved registers from the stack.
 *
 * @details Invokes the low-level assembly routine Trap_exit_int_float()
 *          to restore all integer and floating-point caller-saved registers
 *          previously saved by PUSH_CONTEXT_INT_FLOAT.
 *
 * @note Must be paired with a corresponding PUSH_CONTEXT_INT_FLOAT macro.
 *
 * @warning Mismatched usage with PUSH_CONTEXT_INT_FLOAT will cause stack
 *          corruption and unpredictable system behaviour.
 */
#define POP_CONTEXT_INT_FLOAT asm volatile("jal Trap_exit_int_float\n\t")

/** @} */

#endif  // INCLUDE_IRQ_HELPERS_H_