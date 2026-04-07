/***************************************************************************
* Project           		:  shakti devt board
* Name of the file	     	:  clint_driver.h
* Brief Description of file     :  Header file for clint.
* Name of Author    	        :  Sathya Narayanan N
* Email ID                      :  sathya281@gmail.com

Copyright (C) 2019  IIT Madras. All rights reserved.

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
***************************************************************************/
/**
 * @file clint_driver.h
 * @brief  Header file for clint
 * @details This is the header file for clint_driver.c
 */

#ifndef CLIC_DRIVER_H
#define CLIC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "traps.h"

#define MSIP       0x02000000
#define MTIME      0x0200BFF8
#define MTIMECMP   0x02004000
#define CLINT_DIVISOR   256

extern volatile uint64_t* mtime;
extern volatile uint64_t* mtimecmp;

/* function prototype */
/** @fn static unsigned long mtime_low( )
 * @brief return the lower 32bit of mtime.
 * @details return the lower half of mtime. And this is needed mostly in dealing mtime in 32 bit machines.
 * @return unsigned long
 */
static uint32_t mtime_low(void);
/** @fn static uint32_t mtime_high(void)
 * @brief return the upper 32 bit of mtime
 * @details return the upper 32 bit of mtime register. This is very useful incase of 32 bit core.
 *          Incase of 64 bit core this has to be appended with lower 32 bits adn sent.
 * @return unsigned 32bit int
 */
static uint32_t mtime_high(void);
/** @fn uint64_t Get_MTIME()
 * @brief return the mtime value for a 32 bit or 64 bit machine
 * @details return the mtime value based on the __riscv_xlen. Incase of 64 bit, this joins the upper
 *          and lower 32 bits of mtime and return
 * @return unsigned 64bit int
 */
uint64_t Get_MTIME(void);
/** @fn void Config_Counter( uint64_t value)
 * @brief Sets up the CLINT timer
 * @details Sets the mtimecmp to current mtime + delta
 * @param unsigned 64bit int (delta value after which interrupt happens)
 */
inline void Config_Counter(uint64_t value);
/**
 * @brief Configures the CLINT timer and triggers a software interrupt.
 * 
 * @details This function performs the following operations:
 * 
 * 1. Triggers a software interrupt by writing to the MSIP (Machine Software Interrupt Pending) register.
 * 2. Disables specific interrupts by clearing bits in the MIE (Machine Interrupt Enable) register.
 * 3. Enables global interrupts by setting the relevant bits in the MSTATUS (Machine Status) register.
 * 4. Configures the timer with the provided value using the `Config_Counter` function.
 * 
 * @param timer_value The value to configure the timer.
 * 
 * @note 
 * - The function assumes that the `msip` pointer is correctly initialized and points to the MSIP register.
 * - The specific bits disabled in the MIE register and the bit set in the MSTATUS register are
 *   hardcoded and might need adjustment based on the system's interrupt configuration and requirements.
 * 
 * @see Config_Counter
 */
inline void CLINT_Timer(uint64_t timer_value);
/** @fn __attribute__((weak)) CLINT_Handler(uintptr_t int_id, uintptr_t epc)
 * @brief Handler for Machine Timer Interrupt
 * @details Handler for Machine Timer Interrupt. This handles the timer interrupt and sets mtimecmp to clear timer interrupt.
 * @param unsigned int ptr int_id
 * @param unsigned int ptr epc
 */
__attribute__((weak)) inline void CLINT_Handler(uintptr_t int_id, uintptr_t epc);

#ifdef __cplusplus
}
#endif

#endif