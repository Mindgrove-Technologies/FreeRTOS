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
 * @file aes_gcm.h
 * @brief Contains APIs for performing AES-GCM (Galois/Counter Mode) 
 *        encryption, decryption, and authentication. 
* @details This header file contains function prototypes, macro definitions,
 *         and data types required for implementing AES 
 *         (Advanced Encryption Standard) in Galois/Counter Mode (GCM), 
 *         providing authenticated encryption with associated data (AEAD).
 * @version 1.0
 * @authors Dayana Devi K (dayana@mindgrovetech.in)
 * @date 24-02-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 24-02-2026 | 1.0     | Dayana Devi K         | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_AES_GCM_H_
#define BSP_INCLUDE_AES_GCM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @enum AES_GCM_Mode
 * @brief AES-GCM operation modes.
 *
 * This enumeration defines the supported modes for AES-GCM operations,
 * specifying whether encryption or decryption is performed.
 */
typedef enum {
    /** AES-GCM encryption mode. Used to encrypt plaintext into ciphertext. */
    AES_GCM_ENCRYPT = 0U,
    /** AES-GCM decryption mode. Used to decrypt ciphertext into plaintext. */
    AES_GCM_DECRYPT = 1U
} AES_GCM_Mode;

/**
 * @brief AES-GCM configuration structure.
 *
 * @details This structure contains all parameters required to perform
 * AES encryption or decryption in Galois/Counter Mode (GCM), including
 * input/output buffers, key, initialization vector (IV), Additional
 * Authenticated Data (AAD), and the authentication tag.
 *
 * @note
 * 1. IV length (`iv_len_bits`) must not be zero, at least 8 bits is expected.
 * 2. The `aad` pointer can be `NULL` if no Additional Authenticated Data
 *    is used.
 * 3. All buffers (`cipher_text`, `input_text`, `key`, `iv`, `tag`) must
 *    point to valid memory locations with sufficient size.
 * 4. AES key length (`key_len_bits`) must be 128, 192, or 256 bits.
 * 5. Authentication tag length (`tag_len_bits`) must be one of the supported
 *    values: 128, 120, 112, 104, 96, 64, or 32 bits.
 */
typedef struct {
    /**
     * @brief Ciphertext buffer pointer.
     *
     * Pointer to the buffer used for storing ciphertext during
     * encryption or providing ciphertext during decryption.
     */
    uint8_t *cipher_text;

    /**
     * @brief Plaintext buffer pointer.
     *
     * Pointer to the buffer used for providing plaintext during 
     * encryption or storing decrypted plaintext during decryption.
     */
    uint8_t *input_text;

    /**
     * @brief Length of input data in bits.
     */
    size_t input_len_bits;

    /**
     * @brief Additional Authenticated Data (AAD) pointer.
     *
     * Optional data that is authenticated but not encrypted.
     * Can be `NULL` if no AAD is used.
     */
    uint8_t *aad;

    /**
     * @brief Length of AAD in bits.
     */
    size_t aad_len_bits;

    /**
     * @brief AES key pointer.
     *
     * Pointer to the key used for AES encryption/decryption.
     */
    uint8_t *key;

    /**
     * @brief AES key length in bits.
     *
     * Valid values: 128, 192, 256.
     */
    size_t key_len_bits;

    /**
     * @brief Initialization Vector (IV) pointer.
     *
     * Pointer to the IV used for AES-GCM counter generation.
     */
    uint8_t *iv;

    /**
     * @brief IV length in bits.
     *
     * @note IV length must be at least 8 bits.
     */
    size_t iv_len_bits;

    /**
     * @brief Authentication tag buffer pointer.
     *
     * Used to store the computed authentication tag during encryption
     * or provide the expected tag during decryption for verification.
     */
    uint8_t *tag;

    /**
     * @brief Authentication tag length in bits.
     *
     * Supported lengths: 128, 120, 112, 104, 96, 64, 32.
     */
    size_t tag_len_bits;

    /**
     * @brief AES-GCM operation mode.
     *
     * Determines whether the operation is encryption or decryption.
     * Valid values:
     *   - AES_GCM_ENCRYPT
     *   - AES_GCM_DECRYPT
     */
    AES_GCM_Mode mode;
} AES_GCM_Config;

/**
 * @brief Performs AES-GCM authenticated encryption or decryption.
 *
 * @details This function encrypts or decrypts input data using AES in
 * Galois/Counter Mode (GCM) and computes/verifies the authentication tag.
 *
 * - In encryption mode, plaintext is converted to ciphertext and an
 *   authentication tag is generated.
 * - In decryption mode, ciphertext is converted to plaintext, and the
 *   authentication tag is verified. If verification fails, the plaintext
 *   buffer is cleared to prevent use of unauthenticated data.
 *
 * The function handles key, IV (nonce), Additional Authenticated Data (AAD),
 * and tag lengths. AES block operations (CTR and ECB modes) are performed
 * using hardware acceleration; GCM-specific operations (hash subkey generation,
 * GHASH, pre-counter J0 computation, tag XOR) are performed in software.
 *
 * @param cfg Pointer to an AES_GCM_Config structure containing input/output
 * buffers, key, IV, AAD, tag, lengths, and operation mode.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if operation succeeds with valid authentication tag,  
 * - @ref AUTHTAGMISMATCH if authentication fails during decryption,  
 * - @ref EFAULT if any required pointer in `cfg` is NULL, and
 * - @ref EINVAL if key length, tag length, IV length, or mode are invalid.
 */
uint16_t AES_GCM(AES_GCM_Config *cfg);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_AES_GCM_H_
