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
 * @file ascon.h
 * @brief Contains APIs for the ASCON cryptographic functions.
 * @details Provides APIs for authenticated encryption and decryption 
 *          (AEAD-128a), hash computation (ASCON-Hash256), extensible-output 
 *          functions (ASCON-XOF128), and customizable XOF functions 
 *          (ASCON-CXOF128).
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

#ifndef BSP_INCLUDE_ASCON_H_
#define BSP_INCLUDE_ASCON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @enum ASCON_Mode
 * @brief ASCON AEAD operation modes.
 *
 * This enumeration defines the supported operation modes for
 * ASCON AEAD, specifying whether encryption or decryption
 * is performed.
 */
typedef enum {
    /** ASCON encryption mode. Encrypts plaintext and 
        generates authentication tag. */
    ASCON_ENCRYPT,

    /** ASCON decryption mode. Decrypts ciphertext and 
        verifies authentication tag. */
    ASCON_DECRYPT
} ASCON_Mode;

/**
 * @brief ASCON AEAD-128a configuration structure.
 *
 * @details This structure contains all parameters required to perform
 * authenticated encryption or decryption using the ASCON AEAD-128a algorithm.
 */
typedef struct {
    /**
     * @brief Pointer to the output buffer.
     *
     * - In encrypt mode: receives the ciphertext followed
     *   by the authentication tag.
     * - In decrypt mode: receives the decrypted plaintext.
     */
    uint8_t *output;

    /**
     * @brief Pointer to a variable that receives the total length
     *        of the output buffer.
     *
     * - In encrypt mode: set to plaintext length + ASCON_TAG_SIZE.
     * - In decrypt mode: set to ciphertext length - ASCON_TAG_SIZE.
     */
    size_t *output_len;

    /**
     * @brief Pointer to the input buffer.
     *
     * - In encrypt mode: the plaintext message.
     * - In decrypt mode: the ciphertext including the
     *   authentication tag.
     */
    const uint8_t *input;

    /**
     * @brief Length of the input buffer in bytes.
     *
     * In decrypt mode, the input length must be greater than
     * or equal to ASCON_TAG_SIZE.
     */
    size_t input_len;

    /**
     * @brief Pointer to additional authenticated data (AAD).
     *
     * If unused, this parameter should be NULL.
     */
    const uint8_t *aad;

    /**
     * @brief Length of the AAD in bytes.
     *
     * Ignored if aad is NULL.
     */
    size_t aad_len;

    /**
     * @brief Pointer to the secret nonce.
     *
     * This parameter is not used and should be NULL.
     */
    const uint8_t *sec_nonce;

    /**
     * @brief Pointer to the public nonce.
     *
     * The length of the public nonce must be exactly 16 bytes.
     */
    const uint8_t *pub_nonce;

    /**
     * @brief Pointer to the secret key.
     *
     * The length of the key must be exactly 16 bytes.
     */
    const uint8_t *key;

    /**
     * @brief Operation mode.
     *
     * Must be one of:
     * - `ASCON_ENCRYPT` to perform authenticated encryption.
     * - `ASCON_DECRYPT` to perform authenticated decryption.
     */
    ASCON_Mode mode;
} ASCON_AEAD128a_Config;

/**
 * @brief ASCON Hash256 configuration structure.
 *
 * @details This structure contains all parameters required to compute
 * the ASCON-Hash256 digest of an input message.
 */
typedef struct {
    /**
     * @brief Pointer to the buffer where the hash output is stored.
     *
     * The buffer must be at least 32 bytes in size to store
     * the 256-bit (32-byte) hash result.
     */
    uint8_t *hash_output;

    /**
     * @brief Pointer to the input message.
     *
     * Points to the message data that will be hashed.
     */
    const uint8_t *input;

    /**
     * @brief Length of the input message in bytes.
     */
    size_t input_len;
} ASCON_Hash256_Config;

/**
 * @brief ASCON XOF128 configuration structure.
 *
 * @details This structure contains all parameters required to compute
 * a variable-length hash output using the ASCON-XOF128 algorithm.
 */
typedef struct {
    /**
     * @brief Pointer to the output buffer.
     *
     * The buffer must be large enough to store the number
     * of bytes specified by hash_output_len.
     */
    uint8_t *hash_output;

    /**
     * @brief Requested output length in bytes.
     *
     * Specifies the number of output bytes to be generated
     * by the ASCON-XOF128 function.
     */
    size_t hash_output_len;

    /**
     * @brief Pointer to the input message.
     *
     * Points to the message data that will be processed
     * by the XOF function.
     */
    const uint8_t *input;

    /**
     * @brief Length of the input message in bytes.
     */
    size_t input_len;
} ASCON_XOF128_Config;

/**
 * @brief ASCON CXOF128 configuration structure.
 *
 * @details This structure contains all parameters required to compute
 * a customizable variable-length output using the ASCON-CXOF128 algorithm.
 */
typedef struct {
    /**
     * @brief Pointer to the output buffer.
     *
     * The buffer must be large enough to store the number
     * of bytes specified by hash_output_len.
     */
    uint8_t *hash_output;

    /**
     * @brief Requested output length in bytes.
     *
     * Specifies the number of output bytes to be generated
     * by the ASCON-CXOF128 function.
     */
    size_t hash_output_len;

    /**
     * @brief Pointer to the input message.
     *
     * Points to the message data that will be processed
     * by the CXOF function.
     */
    const uint8_t *input;

    /**
     * @brief Length of the input message in bytes.
     */
    size_t input_len;

    /**
     * @brief Pointer to the customization string.
     *
     * If unused, this parameter should be NULL.
     */
    const uint8_t *custom_string;

    /**
     * @brief Length of the customization string in bytes.
     *
     * Ignored if custom_string is NULL.
     */
    size_t custom_string_len;
} ASCON_CXOF128_Config;

/**
 * @brief Performs authenticated encryption or decryption using the ASCON
 *        AEAD algorithm.
 *
 * @details This function either encrypts plaintext and generates an
 * authentication tag, or verifies an authentication tag and decrypts
 * ciphertext, depending on the specified mode.
 *
 * Steps performed:
 * 1. Initializes the ASCON AEAD state using the key and public nonce.
 * 2. Absorbs the associated authenticated data (AAD), if provided.
 * 3. Encrypts or decrypts the input data depending on the mode.
 * 4. Finalizes the state and either appends (encrypt) or verifies
 *    (decrypt) the authentication tag.
 *
 * @param cfg Pointer to an ASCON_AEAD128a_Config structure containing
 *            all required input, output, key, nonce, and mode parameters.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS On successful encryption or decryption,
 * - @ref AUTHTAGMISMATCH If authentication tag verification fails
 * (decrypt mode only),
 * - @ref EFAULT If any required buffer pointer inside cfg is NULL, and
 * - @ref EINVAL If the input length is smaller than the authentication
 *         tag length (decrypt mode only).
 */
uint16_t ASCON_AEAD128a(ASCON_AEAD128a_Config *cfg);

/**
 * @brief Computes the ASCON-Hash256 digest of an input message.
 *
 * @details This function computes a fixed-length 256-bit cryptographic hash
 * using the ASCON hash algorithm.
 *
 * Steps performed:
 * 1. Initializes the ASCON hash state.
 * 2. Absorbs the input message.
 * 3. Applies the ASCON permutation.
 * 4. Produces the final 256-bit hash output.
 *
 * @param cfg Pointer to an ASCON_Hash256_Config structure containing
 *            the input message and output buffer.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS On successful hash computation, and
 * - @ref EFAULT If the required buffers inside cfg are NULL.
 */
uint16_t ASCON_Hash256(ASCON_Hash256_Config *cfg);

/**
 * @brief Computes an extensible-output hash using ASCON-XOF128.
 *
 * @details This function generates a variable-length cryptographic hash output
 * from the input message.
 *
 * Steps performed:
 * 1. Initializes the ASCON XOF state.
 * 2. Absorbs the input message.
 * 3. Applies the ASCON permutation.
 * 4. Squeezes the requested number of output bytes.
 *
 * @param cfg Pointer to an ASCON_XOF128_Config structure containing
 *            the input message, output buffer, and requested output length.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS On successful XOF hash computation, and
 * - @ref EFAULT If the required buffers inside cfg are NULL.
 */
uint16_t ASCON_XOF128(ASCON_XOF128_Config *cfg);

/**
 * @brief Computes a customized extensible-output hash using ASCON-CXOF128.
 *
 * @details This function generates a variable-length hash using a customization
 * string for domain separation.
 *
 * Steps performed:
 * 1. Initializes the ASCON CXOF state.
 * 2. Absorbs the customization string.
 * 3. Absorbs the input message.
 * 4. Applies the ASCON permutation.
 * 5. Squeezes the requested number of output bytes.
 *
 * @param cfg Pointer to an ASCON_CXOF128_Config structure containing
 *            the input message, customization string, output buffer,
 *            and requested output length.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS On successful Customizable XOF computation, and
 * - @ref EFAULT If the required buffers inside cfg are NULL.
 */
uint16_t ASCON_CXOF128(ASCON_CXOF128_Config *cfg);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INCLUDE_ASCON_H_ */
