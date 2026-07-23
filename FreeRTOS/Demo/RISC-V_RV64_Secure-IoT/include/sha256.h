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
 * Project                   : MGS2401 SoC
 * @file sha256.h
 * @brief Contains APIs for SHA-256 operations.
 * @details This header file defines function prototypes required to
 *          perform SHA-256 hashing using the SHA hardware accelerator.
 *
 * @version 1.2
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Jennifer Vinita J (jennifer@mindgrovetech.in)
 *          Dayana Devi K (dayana@mindgrovetech.in)
 * @date 26-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 12-11-2024 | 1.0     | Vishwajith N S        | Initial release.
 * 11-12-2025 | 1.1     | Jennifer Vinita J     | MISRA compliance
 *            |         |                       | updates and optimizations.
 * 26-02-2026 | 1.2     | Dayana Devi K         | Code refinement, formatted 
 *            |         |                       | header file in doxygen style 
 *            |         |                       | and MISRA compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_SHA256_H_
#define BSP_INCLUDE_SHA256_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Performs a complete SHA-256 hashing operation on a single
 * contiguous input buffer.
 *
 * @details This function processes the entire input message in one call
 * using the SHA-256 hardware accelerator. It calculates the required
 * padding, processes each 512-bit block sequentially, and handles the
 * overflow case where the final block requires a double run when padding
 * spills into an additional block.
 *
 * Once all blocks are processed, the final 256-bit hash is retrieved
 * automatically via SHA256_Read_Output(). The computed 32-byte hash
 * is written into sha_output.
 *
 * @param sha_output Pointer to the 32-byte buffer where the final
 * hash output will be stored.
 * @param input_text Pointer to the input message to be hashed.
 * @param input_len_bits Length of the input message in bits.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if hashing completes successfully
 * - @ref EFAULT if any required pointer is NULL
 * - @ref EINVAL if an invalid block length condition occurs
 * - @ref ETIMEDOUT if the SHA-256 hardware does not respond within
 *        the timeout window.
 */
uint16_t SHA256_Single_Run(uint8_t *sha_output,
                           const uint8_t *input_text,
                           size_t input_len_bits);

/**
 * @brief Performs incremental SHA256 hashing across multiple runs.
 *
 * @details This function allows processing of large messages
 * in multiple calls. Each call processes a message chunk and
 * updates the SHA hardware state.
 *
 * When the final portion of the message is detected
 * ((iterated_length_bits + input_len_bits) == total_length),
 * padding and finalization are performed automatically.
 *
 * This function does not return the final hash directly.
 * The user must call SHA256_Read_Output() after completion
 * to retrieve the 256-bit result.
 *
 * @param input_text Pointer to the current message chunk.
 * @param input_len_bits Length of the current chunk in bits.
 * @param total_length Total length of the complete message in bits.
 * @param iterated_length_bits Number of bits already processed.
 * For the first call, this value should be 0.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if the chunk is processed successfully
 * - @ref EFAULT if input_text is NULL
 * - @ref EINVAL if input parameters are invalid
 * - @ref ETIMEDOUT if the SHA-256 hardware does not respond within
 *        the timeout window.
 */
uint16_t SHA256_Multi_Run(const uint8_t *input_text,
                          size_t input_len_bits,
                          size_t total_length,
                          size_t iterated_length_bits);

/**
 * @brief Reads the 256-bit SHA-256 hash result from the hardware output
 * registers.
 *
 * @details Reads 256 bits (32 bytes) from the SHA_OUTPUT register in
 * big-endian (MSB-first) byte order and stores them in sha_output.
 * After the read, SHA256_Zeroize() is called to securely clear all
 * internal SHA-256 hardware registers before returning.
 *
 * @param sha_output Pointer to the 32-byte buffer where the hash
 * result will be stored.
 * @param output_length Pointer to where the output length in bytes
 * will be written. Set to 32 on success.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if the output was read and zeroization completed
 * - @ref EFAULT if sha_output or output_length is NULL
 * - @ref ETIMEDOUT if the SHA-256 hardware zeroization does not complete 
 *        within the timeout window.
 */
uint16_t SHA256_Read_Output(uint8_t *sha_output, size_t *output_length);

/**
 * @brief Triggers zeroization of all internal SHA-256 hardware registers.
 *
 * @details Writes 1 to SHA_ZEROIZE register to initiate a hardware clear
 * of all internal SHA-256 state. Polls SHA_ZEROIZE_STATUS register using
 * a constant-time busy-wait until the zeroization is complete or the
 * timeout window expires.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if zeroization completed within the timeout window
 * - @ref ETIMEDOUT if the SHA-256 hardware zeroization does not complete 
 *        within the timeout window.
 */
uint16_t SHA256_Zeroize(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_SHA256_H_
