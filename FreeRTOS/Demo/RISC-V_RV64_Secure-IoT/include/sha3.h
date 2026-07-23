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
 * @file sha3.h
 * @brief SHA-3 (Keccak) cryptographic hash header
 * @details This header file declares the public interface for the SHA-3
 * (Keccak) cryptographic hash driver compliant with FIPS 202.It provides data
 * types, macros, and function prototypes required to initialize, update, and
 * finalize SHA-3 hash computations for the following variants:
 * - SHA3-224
 * - SHA3-256
 * - SHA3-384
 * - SHA3-512
 *
 * @version 1.0
 * @authors Akash R B (akash@mindgrovetech.in)
 * @date 20-01-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by | Description
 * -----------|---------|-------------|----------------------------------
 * 20-01-2026 | 1.0     | Akash R B   | Initial release.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_SHA3_H_
#define BSP_INCLUDE_SHA3_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @defgroup SHA3_MACROS SHA-3 Configuration Macros
 *
 * @brief Macros defining digest and block sizes for various SHA-3 variants.
 *
 * @{
 */
/** @brief Digest size for SHA3-224 in bytes. */
#define SHA3_224_DIGEST_SIZE    (28u)
/** @brief Digest size for SHA3-256 in bytes. */
#define SHA3_256_DIGEST_SIZE    (32u)
/** @brief Digest size for SHA3-384 in bytes. */
#define SHA3_384_DIGEST_SIZE    (48u)
/** @brief Digest size for SHA3-512 in bytes. */
#define SHA3_512_DIGEST_SIZE    (64u)

/** @brief Block size (rate) for SHA3-224 in bytes. */
#define SHA3_224_BLOCK_SIZE     (144u)
/** @brief Block size (rate) for SHA3-256 in bytes. */
#define SHA3_256_BLOCK_SIZE     (136u)
/** @brief Block size (rate) for SHA3-384 in bytes. */
#define SHA3_384_BLOCK_SIZE     (104u)
/** @brief Block size (rate) for SHA3-512 in bytes. */
#define SHA3_512_BLOCK_SIZE     (72u)
/** @} */

/**
 * @brief SHA-3 Context Structure.
 *
 * @details This structure maintains the internal state, message buffer, 
 * and variant-specific parameters during hashing operations.
 */
typedef struct {
    /** @brief Internal 1600-bit state (5x5 matrix of 64-bit lanes). */
    uint64_t s[25];
    /** @brief Internal buffer to store partial message blocks. */
    uint8_t  buffer[200];
    /** @brief The rate (r) of the sponge construction in bytes. */
    uint32_t rate;
    /** @brief Current index in the internal buffer. */
    uint32_t byteIdx;
} Sha3;

/* --- SHA3-224 API --- */

/**
 * @brief Initializes the SHA-3 context for a 224-bit hash.
 * @details Resets the state and configures the rate for the SHA3-224 variant.
 * @param ctx Pointer to the Sha3 context structure.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_224_Init(Sha3* ctx);

/**
 * @brief Updates the SHA3-224 hash with a new message chunk.
 * @details Absorbs message data into the sponge state.
 * @param ctx Pointer to the Sha3 context structure.
 * @param data Pointer to the input message bytes.
 * @param len Length of the input data in bytes.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_224_Update(Sha3* ctx, const uint8_t* data, uint32_t len);

/**
 * @brief Finalizes the SHA3-224 hash and produces the digest.
 * @details Applies NIST padding, finishes absorption, and squeezes the digest.
 * @param ctx Pointer to the Sha3 context structure.
 * @param hash Pointer to the buffer where the 28-byte digest will be stored.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_224_Final(Sha3* ctx, uint8_t* hash);

/* --- SHA3-256 API --- */

/**
 * @brief Initializes the SHA-3 context for a 256-bit hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_256_Init(Sha3* ctx);

/**
 * @brief Updates the SHA3-256 hash with a new message chunk.
 * @param ctx Pointer to the Sha3 context structure.
 * @param data Pointer to input message.
 * @param len Length of input.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_256_Update(Sha3* ctx, const uint8_t* data, uint32_t len);

/**
 * @brief Finalizes the SHA3-256 hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @param hash Pointer to 32-byte output buffer.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_256_Final(Sha3* ctx, uint8_t* hash);

/* --- SHA3-384 API --- */

/**
 * @brief Initializes the SHA-3 context for a 384-bit hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_384_Init(Sha3* ctx);

/**
 * @brief Updates the SHA3-384 hash with a new message chunk.
 * @param ctx Pointer to the Sha3 context structure.
 * @param data Pointer to input message.
 * @param len Length of input.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_384_Update(Sha3* ctx, const uint8_t* data, uint32_t len);

/**
 * @brief Finalizes the SHA3-384 hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @param hash Pointer to 48-byte output buffer.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_384_Final(Sha3* ctx, uint8_t* hash);

/* --- SHA3-512 API --- */

/**
 * @brief Initializes the SHA-3 context for a 512-bit hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_512_Init(Sha3* ctx);

/**
 * @brief Updates the SHA3-512 hash with a new message chunk.
 * @param ctx Pointer to the Sha3 context structure.
 * @param data Pointer to input message.
 * @param len Length of input.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_512_Update(Sha3* ctx, const uint8_t* data, uint32_t len);

/**
 * @brief Finalizes the SHA3-512 hash.
 * @param ctx Pointer to the Sha3 context structure.
 * @param hash Pointer to 64-byte output buffer.
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS on success
 * - @ref EFAULT if the pointer is NULL
 */
uint16_t Sha3_512_Final(Sha3* ctx, uint8_t* hash);

#ifdef __cplusplus
}
#endif

#endif  /* BSP_INCLUDE_SHA3_H_ */
