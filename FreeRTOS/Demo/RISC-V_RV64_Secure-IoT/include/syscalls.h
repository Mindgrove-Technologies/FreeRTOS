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
 * @file syscalls.h
 * @brief Declaration for syscall function
 * @details This header file provides function prototype for syscall function,
 *          which escalates privilege to M-mode, executes the passed function 
 *          and returns back by lowering the privilege to U-mode.
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

#ifndef BSP_INCLUDE_SYSCALLS_H_
#define BSP_INCLUDE_SYSCALLS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Executes a target function with Machine Mode privilege.
 * @param fptr Pointer to the function to be executed.
 * @param ... Up to 7 standard arguments to pass to the target function.
 * @return The 64-bit return value from the target function.
 */
extern uint64_t syscall(void *fptr, ...);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INCLUDE_SYSCALLS_H_ */
