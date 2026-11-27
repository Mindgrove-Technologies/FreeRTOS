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
 * @file privilege_internal.h
 * @brief Declarations for privilege switch functions
 * @details This header file provides function prototypes for privilege switch
 *          functions, which is used internally inside drivers and not exposed
 *          to Apps.
 * @authors Natarajan S (natarajan@mindgrovetech.in)
 * @date 22-06-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 22-06-2026 | 1.0     | Natarajan S           | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PRIVILEGE_INTERNAL_H_
#define BSP_INCLUDE_PRIVILEGE_INTERNAL_H_

#include <stdint.h>
#ifdef SECURE_SDK

extern volatile uint8_t privilege_depth;

/**
 * @brief Restricts access to peripheral memory space for U-Mode .
 * @details Configures PMP entry 0 disabling R/W/X access for U-mode till the
 *          address 0x10000000.
 */
void Restrict_Peripherals_In_U_Mode(void);
/**
 * @brief Elevates execution to Machine Mode (M-Mode).
 * @details Uses ecall to trap into M-Mode if not already elevated.
 * Maintains a depth counter to support nested calls.
 */
void Enter_M_Mode(void);

/**
 * @brief Returns execution to User Mode (U-Mode).
 * @details Decrements the depth counter, zeroize MPP and executes mret when 
 * depth reaches zero.
 */
void Enter_U_Mode(void);

#endif /* SECURE_SDK */

#endif /* BSP_INCLUDE_PRIVILEGE_INTERNAL_H_ */
