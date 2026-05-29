/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. 
 * All rights reserved.
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
 * @file rsa.h
 * @brief Contains APIs for RSA cryptographic operations.
 * @details  This header file defines function prototypes required to perform 
 *           RSA encryption, decryption, and related cryptographic operations 
 *           using the hardware accelerator. 
 * @version 1.2
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Jennifer Vinita J (jennifer@mindgrovetech.in)
 *          Dayana Devi K (dayana@mindgrovetech.in)
 * @date 27-02-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-11-2024 | 1.0     | Vishwajith N S        | Initial release.
 * 11-12-2025 | 1.1     | Jennifer Vinita J     | MISRA compliance updates and 
 *            |         |                       | code optimizations.
 * 27-02-2026 | 1.2     | Dayana Devi K         | Code refinement, Formatted 
 *            |         |                       | header file in Doxygen style 
 *            |         |                       | and MISRA compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_RSA_H_
#define BSP_INCLUDE_RSA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Performs an RSA modular exponentiation operation.
 *
 * @details This function performs the RSA core operation:
 *
 *     output = input^exponent mod modulus
 *
 * All operands are expected to be 2048-bit (256-byte) big-endian values.
 * The function computes the Montgomery constant R² mod n in software,
 * loads all four operands into the hardware registers, waits for the
 * hardware to complete, reads the 256-byte result, and triggers hardware
 * zeroization before returning.
 *
 * This API is used for RSA encryption, decryption, signature generation,
 * and signature verification depending on the exponent provided.
 *
 * @param output   Pointer to a 256-byte buffer where the RSA result
 *                 will be stored.
 * @param input    Pointer to a 256-byte buffer containing the input.
 * @param exponent Pointer to a 256-byte buffer containing the RSA
 *                 exponent (public or private).
 * @param modulus  Pointer to a 256-byte buffer containing the RSA modulus.
 *
 * @return `SUCCESS`   if the operation completes successfully,
 *         `EFAULT`    if any required pointer is NULL,
 *         `ETIMEDOUT` if the RSA hardware does not respond within the
 *                     timeout window.
 */
uint16_t RSA_Run(uint8_t *output, const uint8_t *input,
                 const uint8_t *exponent, const uint8_t *modulus);

/**
 * @brief Triggers zeroization of all internal RSA hardware registers.
 *
 * @details Writes 1 to the RSA_ZEROIZE register to initiate a hardware
 * clear of all internal RSA state. Polls the RSA_ZEROIZE_STATUS register
 * using a constant-time busy-wait until zeroization is confirmed complete
 * or the timeout window expires.
 *
 * This function may be called independently to ensure the hardware is in
 * a clean state before a new RSA operation.
 *
 * @return `SUCCESS` if zeroization completed within the timeout window,
 *         `ETIMEDOUT` if the RSA hardware zeroization does not complete 
 *          within the timeout window.
 */
uint16_t RSA_Zeroize(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_RSA_H_
