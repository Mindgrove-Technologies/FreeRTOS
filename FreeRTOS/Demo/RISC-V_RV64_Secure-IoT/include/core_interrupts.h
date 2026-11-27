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
 * @file core_interrupts.h
 * @brief Core interrupt handling interface
 * @details This header file declares APIs and data structures for
 * handling core-level interrupts and exceptions in the RISC-V system.
 * It provides:
 * - A machine interrupt handler dispatch table
 * - Weak user-overridable interrupt callback functions
 * - Trap entry handler interfaces
 * 
 * @version 1.0
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Akash R B (akash@mindgrovetech.in)
 * @date 26-03-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 07-07-2025 | 1.0     | Vishwajith N S        | Initial implementation.
 * 26-03-2026 | 1.1     | Akash R B             | MISRA compliance 
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_CORE_INTERRUPTS_H_
#define BSP_INCLUDE_CORE_INTERRUPTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MACHINE_INTERRUPT_COUNT 16U

/**
 * @defgroup CORE_INTERRUPT_TYPES Core Interrupt Types
 * @{
 */

/**
 * @brief Function pointer type for core interrupt handlers.
 *
 * Each interrupt handler is a function that takes no arguments
 * and returns no value.
 */
typedef void (*core_interrupt_handler_t)(void);

/** @} */

/**
 * @defgroup CORE_INTERRUPT_TABLE Core Interrupt Table
 * @{
 */

/**
 * @brief Machine interrupt handler dispatch table.
 *
 * @details This table holds function pointers to interrupt handlers
 * corresponding to machine-level interrupt sources.
 * The index into the table maps to a specific interrupt ID.
 *
 * The table is defined elsewhere and can be overridden if required.
 */
extern const core_interrupt_handler_t \
machine_interrupt_table[MACHINE_INTERRUPT_COUNT];

/** @} */

/**
 * @defgroup CORE_INTERRUPT_CALLBACKS User Overridable Callbacks
 * @brief Weak callback functions that can be overridden by the user.
 * @{
 */

/**
 * @brief Callback invoked on fault/exception.
 */
void on_fault(void);

/**
 * @brief Callback for supervisor software interrupt.
 */
void on_supervisor_software_interrupt(void);

/**
 * @brief Callback for machine software interrupt.
 */
void on_machine_software_interrupt(void);

/**
 * @brief Callback for supervisor timer interrupt.
 */
void on_supervisor_timer_interrupt(void);

/**
 * @brief Callback for machine timer interrupt.
 */
void on_machine_timer_interrupt(void);

/**
 * @brief Callback for supervisor external interrupt.
 */
void on_supervisor_external_interrupt(void);

/**
 * @brief Callback for machine external interrupt.
 */
void on_machine_external_interrupt(void);

/**
 * @brief Default callback for unhandled interrupts.
 */
void on_default_interrupt(void);

/** @} */

/**
 * @defgroup CORE_TRAP_HANDLERS Trap Entry Handlers
 * @brief Low-level trap handling functions mapped to interrupt vectors.
 * @{
 */

/**
 * @brief Fault/exception handler entry.
 *
 * @details This function is invoked when an exception occurs.
 * It typically calls @ref on_fault().
 */
void fault_handler(void);

/**
 * @brief Supervisor software interrupt handler.
 */
void supervisor_software_interrupt_handler(void);

/**
 * @brief Machine software interrupt handler.
 */
void machine_software_interrupt_handler(void);

/**
 * @brief Supervisor timer interrupt handler.
 */
void supervisor_timer_interrupt_handler(void);

/**
 * @brief Machine timer interrupt handler.
 */
void machine_timer_interrupt_handler(void);

/**
 * @brief Supervisor external interrupt handler.
 */
void supervisor_external_interrupt_handler(void);

/**
 * @brief Machine external interrupt handler.
 */
void machine_external_interrupt_handler(void);

/**
 * @brief Default interrupt handler.
 *
 * @details This handler is invoked when no specific handler
 * is registered for an interrupt source.
 */
void default_interrupt_handler(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_CORE_INTERRUPTS_H_