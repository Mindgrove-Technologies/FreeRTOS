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
 * @file faults.h
 * @brief Fault and exception handling interface
 *
 * @details This header file declares the interfaces for handling RISC-V
 * exceptions (faults). It provides:
 * - A fault handler function pointer type
 * - A fault handler dispatch table indexed by mcause
 * - A lookup table of fault messages for debugging/logging
 * - Weak user-overridable fault handler function prototypes
 *
 * Each fault handler corresponds to a specific mcause value defined by
 * the RISC-V privileged architecture. The handler tables and message
 * arrays must remain aligned in size and ordering.
 *
 * @note Users can override any fault handler by providing their own
 * implementation with the same function name.
 *
 * @version 1.0
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Akash R B (akash@mindgrovetech.in)
 * @date 27-03-2025
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 07-07-2025 | 1.0     | Vishwajith N S        | Initial implementation.
 * 27-03-2026 | 1.1     | Akash R B             | MISRA compliance 
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_FAULTS_H
#define BSP_INCLUDE_FAULTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*fault_handler_t)(void);
extern const fault_handler_t fault_handlers[16];

/**
 * @brief Handles instruction address misaligned exceptions
 * @details Invoked when the target address of a branch or jump instruction is
 * not naturally aligned to the instruction boundary requirements.
*/
void on_instruction_address_misaligned(void);

/**
 * @brief Handles instruction access fault exceptions
 * @details Invoked when an attempt to fetch an instruction from a memory address
 * is rejected by the memory protection or bus system.
*/
void on_instruction_access_fault(void);

/**
 * @brief Handles illegal instruction exceptions
 * @details Invoked when the processor attempts to decode or execute an instruction
 * that is invalid, malformed, or unsupported by the current architecture.
*/
void on_illegal_instruction(void);

/**
 * @brief Handles breakpoint exceptions
 * @details Invoked when a software breakpoint instruction is executed by the
 * processor to halt code execution.
*/
void on_breakpoint(void);

/**
 * @brief Handles load address misaligned exceptions
 * @details Invoked when a data load instruction attempts to read from a memory
 * address that is not naturally aligned to the data type size.
*/
void on_load_address_misaligned(void);

/**
 * @brief Handles load access fault exceptions
 * @details Invoked when a memory read/load operation is attempted on a restricted,
 * protected, or non-existent physical memory region.
*/
void on_load_access_fault(void);

/**
 * @brief Handles store/AMO address misaligned exceptions
 * @details Invoked when a memory write or Atomic Memory Operation(AMO) attempts
 * to target an unaligned memory address.
*/
void on_store_amo_address_misaligned(void);

/**
 * @brief Handles store/AMO access fault exceptions
 * @details Invoked when a memory write or Atomic Memory Operation(AMO) is attempted
 * on a write-protected or invalid memory region.
*/
void on_store_amo_access_fault(void);
/**
 * @brief Handles environment call exceptions from User mode (U-mode)
 * @details Invoked when an ecall instruction is executed in User mode to request
 * a service or system function from a higher privilege level.
*/
void on_ecall_from_umode(void);

/**
 * @brief Handles environment call exceptions from Supervisor mode (S-mode)
 * @details Invoked when an ecall instruction is executed in Supervisor mode to
 * request system services from the Machine mode runtime environment.
*/
void on_ecall_from_smode(void);

/**
 * @brief Handles the exception for reserved interrupt vector 10
 * @details Serves as a placeholder or trap handler for the architectural exception
 * code 10, which is currently reserved.
*/
void on_reserved_10(void);

/**
 * @brief Handles environment call exceptions from Machine mode (M-mode)
 * @details Invoked when an ecall instruction is executed in Machine mode, used
 * to interface with a security monitor or underlying firmware.
*/
void on_ecall_from_mmode(void);

/**
 * @brief Handles instruction page fault exceptions
 * @details Invoked when an instruction fetch attempts to access a virtual address
 * mapping that is either missing from physical memory or violates page execution permissions.
*/
void on_instruction_page_fault(void);

/**
 * @brief Handles load page fault exceptions
 * @details Invoked when a memory read/load operation targets a virtual memory page
 * that is not present in physical RAM or violates page read permissions.
*/
void on_load_page_fault(void);

/**
 * @brief Handles the exception for reserved interrupt vector 14
 * @details Serves as a placeholder or trap handler for the architectural exception code 14, which is currently reserved.
*/
void on_reserved_14(void);

/**
 * @brief Handles store/AMO page fault exceptions
 * @details Invoked when a memory write or Atomic Memory Operation (AMO) targets
 * a virtual memory page that is not present, marked read-only, or violates page write permissions.
*/
void on_store_amo_page_fault(void);

#ifdef __cplusplus
}
#endif

#endif   // BSP_INCLUDE_FAULTS_H