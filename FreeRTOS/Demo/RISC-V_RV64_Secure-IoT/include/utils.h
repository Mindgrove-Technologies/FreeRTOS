/****************************************************************************
 * Project           		    : Mindgrove Silicon's Secure-IoT SoC
 * Name of the file	     	    : utils.h
 * Brief Description of file    : Header file for Basic Utilites
 * Name of Author    	        : Kapil Shyam. M
 * Email ID                     : kapil@mindgrovetech.in

 Copyright (C) 2024 Mindgrove Technologies Pvt Ltd. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
/**
 * @file utils.h
 * @brief header file for basic utilities
 * @details This is the header file for util.c
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @fn unsigned long int ReadData(size_t *addr)
 * @brief Returns the value stored at a given address
 * @param  addr The address at which value is located
 * @return unsigned Long
 */
unsigned long ReadData(size_t *addr);

/** @fn void WriteData(size_t *addr, unsigned long val)
 * @brief  Writes a value to an address
 * @param  addr The address to which data has to be written to
 * @param  val  The value to be written.
 * @return void
 */
 void WriteData(size_t *addr, unsigned long val);

 /**
 * @fn void millis_init() 
 * @brief Initializes the millisecond counter.
 * @details This function updates the *mtimecmp value, which is used for 
 *          generating timer interrupts and calculates elapsed milliseconds.
 */
void millis_init();

/** 
 * @fn uint64_t millis()
 * @brief Returns the number of milliseconds since the program started.
 * @details It retrieves the current value of a millisecond counter variable, 
 *          which is updated in the CLINT_Handler.
 * @return The number of milliseconds.
 */
uint64_t millis();

/**
 * @brief Terminates the calling process with the specified exit status.
 * 
 * This function implements process termination for RISC-V architecture using
 * the Supervisor Binary Interface (SBI). It performs necessary cleanup operations
 * and attempts graceful termination through SBI, falling back to an infinite
 * wait loop if SBI is unavailable.
 * 
 * @details The function performs the following operations in sequence:
 * 1. Calls any registered exit handlers (implementation dependent)
 * 2. Executes a memory fence to flush pending operations
 * 3. Attempts termination via SBI exit syscall (function ID 93)
 * 4. Falls back to infinite WFI (Wait For Interrupt) loop if SBI fails
 * 
 * @param[in] status Exit status code to be returned to the parent process.
 *                   Conventionally, 0 indicates successful termination,
 *                   non-zero values indicate error conditions.
 * 
 * @return This function does not return. Process termination is guaranteed
 *         either through SBI exit syscall or infinite loop.
 * 
 * @note This implementation is specific to RISC-V architecture and requires:
 *       - SBI (Supervisor Binary Interface) support for graceful termination
 *       - Privilege level sufficient for executing ECALL instruction
 * 
 * @warning This function never returns. All cleanup operations must be
 *          completed before calling this function.
 * 
 * @see atexit() for registering exit handlers
 * @see _exit() for immediate process termination without cleanup
 * 
 * @since Version 1.0
 * @author [Author Name]
 * 
 * @par Example Usage:
 * @code
 * int main() {
 *     // Program logic here
 *     
 *     if (error_condition) {
 *         exit(1);  // Exit with error status
 *     }
 *     
 *     exit(0);  // Normal termination
 * }
 * @endcode
 * 
 * @par Implementation Notes:
 * - Uses RISC-V FENCE instruction for memory synchronization
 * - SBI exit syscall number 93 follows RISC-V SBI specification
 * - WFI instruction reduces power consumption during infinite wait
 * - Assembly constraints ensure proper register allocation for SBI call
 */
void exit(int status);

#ifdef __cplusplus
}
#endif

#endif
