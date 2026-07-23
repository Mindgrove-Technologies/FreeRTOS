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
 * @file sha2.h
 * @brief Contains APIs for configuring and resetting the SHA2 functionality.
 * @details Provides APIs for software implementation of SHA2, including
 *          SHA384 and SHA512.
 * @version 1.0
 * @authors Harini Sree.S
 * @date 14-01-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 14-01-2026 | 1.0     | Harini Sree.S         | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_SHA2_H_
#define BSP_INCLUDE_SHA2_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup SHA2_MACROS SHA2 Configuration Macros
 *
 * @brief Macros used to configure the size of Digest and Message block.
 *
 * @{
 */
/**
 * @brief SHA-512 digest size in bytes.
 *
 * @details
 * Defines the length of the SHA-512 hash output.
 */
#define SHA512_DIGEST_SIZE 64

/**
 * @brief SHA-384 digest size in bytes.
 *
 * @details
 * Defines the length of the SHA-384 hash output.
 */
#define SHA384_DIGEST_SIZE 48

/**
 * @brief SHA-384 context structure.
 *
 * @details
 * This structure holds the internal state and buffers required
 * to compute a SHA-2 hash.
 */
typedef struct {
    /**
     * @brief Internal hash state.
     *
     * Contains the current intermediate hash value,
     * represented as eight 64-bit words.
     */
    uint64_t state[8];

    /**
     * @brief Total message length in bits.
     *
     * Stored as a 128-bit value split into two 64-bit words.
     */
    uint64_t bitlen[2];

    /**
     * @brief Message block buffer.
     *
     * Temporary buffer used to store input data until a full
     * 1024-bit (128-byte) block is available for processing.
     */
    uint8_t buffer[128];

    /**
     * @brief Number of bytes currently stored in the buffer.
     */
    uint32_t buffer_len;
} SHA2;


/**
 * @brief Initialize a SHA-512 hashing context.
 *
 * @details
 * Initializes the SHA-512 context with the standard initial hash values
 * defined in FIPS 180-4. This function must be called before any update
 * or final operations are performed.
 *
 * @param ctx Pointer to the SHA-512 context to initialize.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on successuful initialisation
 */
uint16_t SHA512_Init(SHA2 *ctx);

/**
 * @brief Update the SHA-512 hash with input data.
 *
 * @details
 * Processes the input data and updates the internal SHA-512 state.
 * This function may be called multiple times to hash streaming data.
 *
 * @param ctx  Pointer to an initialized SHA-512 context.
 * @param data Pointer to the input data buffer.
 * @param len  Length of the input data in bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 * - @ref EINVAL if length of the message is exceeds the range
 */
uint16_t SHA512_Update(SHA2 *ctx, const uint8_t *data, size_t len);

/**
 * @brief Finalize the SHA-512 hash computation.
 *
 * @details
 * Completes the hash computation, performs padding, and writes the
 * final 512-bit (64-byte) hash output to the provided buffer.
 * After this call, the context should not be reused unless reinitialized.
 *
 * @param ctx  Pointer to the SHA-512 context.
 * @param hash Pointer to a buffer of at least SHA512_DIGEST_SIZE bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t SHA512_Final(SHA2 *ctx, uint8_t *hash);

/**
 * @brief Generate the SHA-512 hash with input data.
 *
 * @details
 * Processes the input data and updates the internal SHA-512 state.
 * This function may be called multiple times to hash streaming data.
 *
 * @param data Pointer to the input data buffer.
 * @param len  Length of the input data in bytes.
 * @param hash Pointer to a buffer of at least SHA512_DIGEST_SIZE bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 * - @ref EINVAL if length of the message is exceeds the range
 */
uint16_t SHA512_Run(const uint8_t* data, size_t len, uint8_t* hash);

/**
 * @brief Initialize a SHA-384 hashing context.
 *
 * @details
 * Initializes the SHA-384 context with the standard initial hash values
 * defined in FIPS 180-4.
 *
 * @param ctx Pointer to the SHA-384 context to initialize.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on successuful initialisation
 */
uint16_t SHA384_Init(SHA2 *ctx);

/**
 * @brief Update the SHA-384 hash with input data.
 *
 * @details
 * Processes input data and updates the internal SHA-384 state.
 * This function supports hashing data in multiple chunks.
 *
 * @param ctx  Pointer to an initialized SHA-384 context.
 * @param data Pointer to the input data buffer.
 * @param len  Length of the input data in bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 * - @ref EINVAL if length of the message is exceeds the range */
uint16_t SHA384_Update(SHA2 *ctx, const uint8_t *data, size_t len);

/**
 * @brief Finalize the SHA-384 hash computation.
 *
 * @details
 * Completes the hash computation and writes the final 384-bit
 * (48-byte) hash output to the provided buffer.
 *
 * @param ctx  Pointer to the SHA-384 context.
 * @param hash Pointer to a buffer of at least SHA384_DIGEST_SIZE bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t SHA384_Final(SHA2 *ctx, uint8_t *hash);

/**
 * @brief Generate the SHA-384 hash with input data.
 *
 * @details
 * Processes the input data and updates the internal SHA-384 state.
 * This function may be called multiple times to hash streaming data.
 *
 * @param data Pointer to the input data buffer.
 * @param len  Length of the input data in bytes.
 * @param hash Pointer to a buffer of at least SHA384_DIGEST_SIZE bytes.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 * - @ref EINVAL if length of the message is exceeds the range
 */
uint16_t SHA384_Run(const uint8_t* data, size_t len, uint8_t* hash);

#ifdef __cplusplus
}
#endif
#endif  //  BSP_INCLUDE_SHA2_H_
