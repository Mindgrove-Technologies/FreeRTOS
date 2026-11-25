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
 * @file aes_xts.h
 * @brief Contains APIs for performing AES-XTS (XEX-based Tweaked-codebook
 *        mode with ciphertext stealing) encryption and decryption.
* @details This header file contains function prototypes, macro definitions,
 *         and data types required for implementing AES 
 *         (Advanced Encryption Standard) in XEX-based Tweaked-codebook mode
 *         with ciphertext stealing (XTS), per IEEE Std 1619-2007 and
 *         NIST SP 800-38E.
 * @version 1.0
 * @authors Dayana Devi K (dayana@mindgrovetech.in)
 * @date 29-07-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 29-07-2026 | 1.0     | Dayana Devi K         | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_AES_XTS_H_
#define BSP_INCLUDE_AES_XTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "crypto_api.h"

/**
 * @enum AES_XTS_Mode_t
 * @brief AES-XTS operation modes.
 *
 * This enumeration defines the supported modes for AES-XTS operations,
 * specifying whether encryption or decryption is performed.
 */
typedef enum {
    /** AES-XTS encryption mode. Used to encrypt plaintext into ciphertext. */
    AES_XTS_ENCRYPT = 0U,
    /** AES-XTS decryption mode. Used to decrypt ciphertext into plaintext. */
    AES_XTS_DECRYPT = 1U
} AES_XTS_Mode;

/**
 * @enum AES_XTS_Tweak_Format
 * @brief Format of the tweak seed value supplied to AES-XTS.
 *
 * This enumeration defines which field of `AES_XTS_Config` is used to
 * derive the base tweak.
 */
typedef enum {
    /** Use `sequence_number`: a 64-bit sequence number, passed by value.
     *  Internally encoded as a little-endian byte string and zero-padded
     *  to 16 bytes per IEEE Std 1619-2007. */
    AES_XTS_TWEAK_FORMAT_SEQNUM = 0U,
    /** Use `raw_tweak_seed`: a caller-supplied, pre-formatted 128-bit
     *  (16-byte) tweak seed, used as-is. */
    AES_XTS_TWEAK_FORMAT_RAW = 1U
} AES_XTS_Tweak_Format;

/**
 * @brief AES-XTS configuration structure.
 *
 * @details This structure contains all parameters required to perform
 * AES encryption or decryption in XEX-based Tweaked-codebook mode with
 * ciphertext stealing (XTS), including the two AES keys, the tweak seed,
 * and the input/output data buffers.
 *
 * @note
 * 1. `key_len_bits` and `tweak_key_len_bits` must each be `128` or `256` bits,
 *    and must be equal to one another.
 * 2. `input_len_bits` must be at least `128` bits (16 bytes) and must be
 *    byte-aligned (multiple of 8 bits).
 * 3. Exactly one of `sequence_number` / `raw_tweak_seed` is consulted,
 *    selected by `tweak_value_format`:
 *      - AES_XTS_TWEAK_FORMAT_SEQNUM -> `sequence_number` (by value).
 *      - AES_XTS_TWEAK_FORMAT_RAW    -> `raw_tweak_seed`, which must point
 *        to exactly `raw_tweak_seed_len` = AES_XTS_BLOCK_SIZE (16) bytes.
 * 4. All buffers (`key`, `tweak_key`, `raw_tweak_seed` when used, `input`,
 *    `output`) must point to valid memory locations with sufficient size.
 */
typedef struct {
    /**
     * @brief Output buffer pointer.
     *
     * Pointer to the buffer used for storing ciphertext during encryption
     * or decrypted plaintext during decryption.
     */
    uint8_t *output;

    /**
     * @brief Input buffer pointer.
     *
     * Pointer to the buffer providing plaintext during encryption or
     * ciphertext during decryption.
     */
    uint8_t *input;

    /**
     * @brief Length of input data in bits.
     *
     * @note Must be at least 128 bits and must be a multiple of 8 bits
     * (byte-aligned).
     */
    size_t input_len_bits;

    /**
     * @brief Key pointer.
     *
     * Pointer to the AES key used to encrypt or decrypt the data blocks.
     */
    uint8_t *key;

    /**
     * @brief Length of `key` in bits.
     *
     * Valid values: 128, 256.
     */
    size_t key_len_bits;

    /**
     * @brief tweak_key pointer.
     *
     * Pointer to the AES key used to encrypt the tweak seed when deriving
     * the base tweak.
     */
    uint8_t *tweak_key;

    /**
     * @brief Length of `tweak_key` in bits.
     *
     * Valid values: 128, 256. Must equal `key_len_bits`.
     */
    size_t tweak_key_len_bits;

    /**
     * @brief Format of the tweak seed.
     *
     * Selects which of `sequence_number` / `raw_tweak_seed` is used to
     * derive the base tweak. Valid values:
     *   - AES_XTS_TWEAK_FORMAT_SEQNUM
     *   - AES_XTS_TWEAK_FORMAT_RAW
     */
    AES_XTS_Tweak_Format tweak_value_format;

    /**
     * @brief 64-bit sequence number, passed by value.
     *
     * Used only when `tweak_value_format == AES_XTS_TWEAK_FORMAT_SEQNUM`.
     * Encoded internally as little-endian bytes per IEEE Std 1619-2007.
     */
    uint64_t sequence_number;

    /**
     * @brief Raw, pre-formatted 128-bit tweak seed pointer.
     *
     * Used only when `tweak_value_format == AES_XTS_TWEAK_FORMAT_RAW`.
     * Must point to exactly `raw_tweak_seed_len` bytes.
     */
    uint8_t *raw_tweak_seed;

    /**
     * @brief Length of the buffer at `raw_tweak_seed`, in bits.
     *
     * Used only when `tweak_value_format == AES_XTS_TWEAK_FORMAT_RAW`.
     * Must equal AES_XTS_BLOCK_SIZE_BITS (128).
     */
    size_t raw_tweak_seed_len_bits;

    /**
     * @brief AES-XTS operation mode.
     *
     * Determines whether the operation is encryption or decryption.
     * Valid values:
     *   - AES_XTS_ENCRYPT
     *   - AES_XTS_DECRYPT
     */
    AES_XTS_Mode mode;
} AES_XTS_Config;

/**
 * @brief Performs AES-XTS encryption or decryption.
 *
 * @details This function encrypts or decrypts input data using AES in
 * XEX-based Tweaked-codebook mode with ciphertext stealing (XTS).
 *
 * - The base tweak is generated by AES-encrypting the tweak seed
 *   (derived per `tweak_value_format`) with `tweak_key`.
 * - Each subsequent block's tweak is produced via GF(2^128) multiplication
 *   of the previous tweak.
 * - If the input length is not a multiple of the AES block size,
 *   ciphertext stealing is applied to the final two blocks.
 *
 * AES block operations are performed using hardware acceleration; tweak
 * generation and GF(2^128) multiplication are performed in software.
 *
 * @param cfg Pointer to an AES_XTS_Config structure containing input/output
 * buffers, keys, tweak seed, lengths, tweak format, and operation mode.
 *
 * @return `SUCCESS` if operation succeeds,
 * `EINVAL` if key length, key-length parity, input length, tweak format,
 *  raw tweak seed length, or mode are invalid, and
 * `EFAULT` if any required pointer in `cfg` is NULL.
 */
uint16_t AES_XTS(AES_XTS_Config *cfg);

#ifdef __cplusplus
}
#endif

#endif   // BSP_INCLUDE_AES_XTS_H_
