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
 * @file   plic.h
 * @brief  This is the PLIC Driver header file for Platform Level Interrupt
 *         Controller.
 * @details Provides function prototypes to configure interrupts, set interrupt
 *          priorities, and handle PLIC interrupt sources.
 * @version 1.1
 * @authors Vishwajith.N.S (vishwajith@mindgrovetech.in),
 *          Harini P (harinip@mindgrovetech.in)
 * @date 07-07-2025 
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 08-10-2024 | 1.0     | Vishwajith.N.S        | Initial release.              
 * 07-07-2025 | 1.1     | Harini P              | Updated the header file for 
 *            |         |                       | MISRA-C compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PLIC_H_
#define BSP_INCLUDE_PLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** @brief Defines the maximum interrupt source. */
#define PLIC_MAX_INTERRUPT_SRC  81U

/**
* @enum  PLIC_Priority_t
* @brief PLIC interrupt priority levels.
* @details Defines the available priority levels for configuring PLIC interrupts.
*/
enum {
  /** The value of this enum is 0U. It represents interrupt priority level 1,
   *  means'no interrupt threshold' */
  PLIC_PRIORITY_1 = 0U,
  /** The value of this enum is 1U. It represents interrupt priority level 2. */
  PLIC_PRIORITY_2 = 1U,
  /** The value of this enum is 2U. It represents interrupt priority level 3. */
  PLIC_PRIORITY_3 = 2U,
  /** The value of this enum is 3U. It represents interrupt priority level 4. */
  PLIC_PRIORITY_4 = 3U,
  /** The value of this enum is 4U. It represents interrupt priority level 5. */
  PLIC_PRIORITY_5 = 4U,
  /** The value of this enum is 5U. It represents interrupt priority level 6. */
  PLIC_PRIORITY_6 = 5U,
  /** The value of this enum is 6U. It represents interrupt priority level 7. */
  PLIC_PRIORITY_7 = 6U
};

/* Function pointer type for PLIC interrupt handlers. */
typedef void (*PLIC_IRQHandler_t)(void * args);

/* Function prototypes */

/**
 * @brief Used to mark a PLIC interrupt as completed
 * 
 * @details This function signals that the specified PLIC interrupt has 
 *          been serviced, allowing the core to handle subsequent interrupts.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer
 *                     that identifies the PLIC interrupt to be completed.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt marked as completed successfully.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 */
uint16_t PLIC_Interrupt_Complete(uint32_t interrupt_id);

/**
 * @brief Used to handle a PLIC interrupt.
 *
 * @details This function performs the following steps when a PLIC interrupt occurs:
 *          1. Reads the claim register to determine the source of the interrupt.
 *          2. Calls the corresponding interrupt handler to service the interrupt.
 *          3. Writes the serviced interrupt ID back to the claim register to
 *             notify the PLIC core that the interrupt has been handled.
 * 
 * @param none
 * 
 * @return none
 */
void PLIC_Handler(void);

/**
 * @brief Used to enable a particular PLIC interrupt.
 * 
 * @details This function enables the specified PLIC interrupt source,
 *          allowing it to be triggered and serviced by the PLIC.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned
 *                     integer that represents the interrupt id for which
 *                     interrupt has to be enabled.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt enabled successfully.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 */
uint16_t PLIC_Interrupt_Enable(uint32_t interrupt_id);

/**
 * @brief Used to disable a particular PLIC interrupt.
 * 
 * @details This function disables the specified PLIC interrupt source,
 *          preventing it from being triggered or serviced by the PLIC.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned 
 *                     integer that represents the interrupt id for
 *                     which interrupt has to be disabled.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt disabled successfully.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 */
uint16_t PLIC_Interrupt_Disable(uint32_t interrupt_id);

/**
 * @brief Used to set the interrupt priority threshold.
 * 
 * @details This function configures the minimum priority level required
 *          for a PLIC interrupt to be serviced. Interrupts with priority
 *          values greater than the configured threshold will be triggered.
 * 
 * @param priority_value The parameter \a priority_value is an unsigned
 *                       integer that represents the priority value
 *                       above which interrupt will be triggered.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt priority threshold configured successfully.
 * - @ref EDOM  Returned if the priority_value exceeds the maximum supported
 *              priority level of @ref PLIC_PRIORITY_7.
 */
uint16_t PLIC_Interrupt_Threshold(uint32_t priority_value);

/**
 * @brief Used to set the priority level for a specific PLIC interrupt.
 * 
 * @details This function configures the priority of a specified PLIC interrupt.
 *          Higher-priority interrupts are serviced before lower-priority ones,
 *          subject to the interrupt threshold.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer
 *                     that identifies the PLIC interrupt source.
 * 
 * @param priority_value The parameter \a priority_value is an unsigned
 *                       integer that specifies the interrupt priority level.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt priority configured successfully.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 * - @ref EDOM  Returned if the priority_value exceeds the maximum supported
 *              priority level of @ref PLIC_PRIORITY_7.
 */
uint16_t PLIC_Set_Interrupt_Priority(uint32_t int_id, uint32_t priority_value);

/**
 * @brief Used to check the pending status of a PLIC interrupt.
 * 
 * @details This function checks whether the specified PLIC interrupt
 *          source is pending and awaiting service.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer
 *                     that identifies the PLIC interrupt source to be checked.
 * 
 * @param pending_status The parameter \a pending_status is a pointer to a 
 *                       boolean that is set to true if the interrupt is
 *                       pending, false otherwise.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Pending status retrieved successfully.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 */
uint16_t PLIC_Interrupt_Pending(uint8_t interrupt_id, bool *pending_status);

/**
 * @brief Used to configure the PLIC interrupt mode as nested or non-nested.
 * 
 * @details This function configures PLIC interrupt handling mode. In nested 
 *          mode, higher-priority interrupts preempt lower-priority ones; 
 *          otherwise, interrupts are serviced sequentially.
 * 
 * @param enable The parameter \a enable enables nested interrupt mode when
 *               non-zero, and disables nested interrupt mode when zero.
 * 
 * @return none
 */
void PLIC_Nested_Interrupt(uint8_t enable);

/**
 * @brief Used to initializes the PLIC interrupt controller.
 * 
 * @details This function disables all the PLIC interrupts, configures the interrupt
 *          threshold, and enables global interrupt handling.
 * 
 * @param  none 
 * 
 * @return none
 */
void PLIC_Init(void);

/**
 * @brief Used to set an interrupt handler for a PLIC interrupt source.
 * 
 * @details This function associates a user-defined interrupt handler and its
 *           argument with the specified PLIC interrupt ID. When the interrupt
 *           is triggered, the registered handler is invoked with the provided
 *           argument.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that
 *                     represents to which interrupt handler should be mapped.
 * 
 * @param handler The parameter \a handler is a function pointer to the
 *                interrupt handler function to be executed.
 * 
 * @param args The parameter \a args is of void * type used to pass arguement to
 *             interrupt handler.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt handler registered successfully.
 * - @ref EFAULT  Returned if the provided handler function pointer is NULL.
 * - @ref ENODEV  Returned if the interrupt_id exceeds the maximum supported
 *                PLIC interrupt source @ref PLIC_MAX_INTERRUPT_SRC.
 */
uint16_t PLIC_Set_Handler(uint8_t interrupt_id, \
                         PLIC_IRQHandler_t handler, void *args);
#ifdef __cplusplus
}
#endif

#endif   // BSP_INCLUDE_PLIC_H_

