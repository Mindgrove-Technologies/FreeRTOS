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
 * @file rsa.h
 * @brief Contains APIs for RSA cryptographic operations.
 * @details  This header file defines function prototypes required to perform 
 *           RSA encryption, decryption, and related cryptographic operations 
 *           using the hardware accelerator. 
 * @version 1.3
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Jennifer Vinita J (jennifer@mindgrovetech.in)
 *          Dayana Devi K (dayana@mindgrovetech.in)
 *          Harini P (harinip@mindgrovetech.in)
 * @date 29-05-2026
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
 * 26-05-2026 | 1.3     | Dayana Devi K         | Added RSA_Config_t structure
 *            |         |                       | and corresponding documentation.
 * 29-05-2026 | 1.3     | Harini P              | Included the implementation 
 *            |         |                       | for RSA Key-Pair Generation. 
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_RSA_H_
#define BSP_INCLUDE_RSA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief RSA key pair structure.
 *
 * @details @details This structure stores the generated RSA key pair which
 *          contains the modulus, public exponent, and private exponent.
 */
typedef struct {
    /**
     * @brief Pointer to private exponent (d).
     *
     * Used for decryption and signing operations.
     */
    uint8_t *private_expo;

    /**
     * @brief Pointer to Public exponent (e).
     *
     * Used for encryption and signature verification.
     */
    uint8_t *public_expo;

    /**
     * @brief Pointer to RSA modulus (n).
     *
     * This is the modulus used in RSA operations.
     */
    uint8_t *modulus;
} RSA_key_pair;

/**
 * @brief RSA configuration structure.
 *
 * @details This structure contains all parameters required to configure
 * and execute an RSA operation using the RSA hardware module.
 * It provides input/output buffers and the modulus and exponent
 * required for modular exponentiation.
 *
 * @note All pointer fields must be non-NULL before passing to RSA functions.
 *       All length fields must equal RSA_BLOCK_SIZE_BITS (2048 bits).
 */
typedef struct {
    /**
     * @brief Pointer to RSA output buffer.
     *
     * Points to the buffer where the result of the RSA modular
     * exponentiation operation will be stored.
     *
     * @note The buffer must be at least RSA_BLOCK_SIZE bytes in size.
     */
    uint8_t *output;

    /**
     * @brief Pointer to RSA input buffer.
     *
     * Points to the input message to be processed by the RSA operation.
     *
     * @note Must be strictly less than the modulus.
     *       Length must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    const uint8_t *input;

    /**
     * @brief Pointer to RSA exponent.
     *
     * Points to the public or private exponent used in the RSA
     * modular exponentiation operation.
     *
     * @note Must be an odd value.
     *       Must be strictly less than the modulus.
     *       Length must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    const uint8_t *exponent;

    /**
     * @brief Pointer to RSA modulus.
     *
     * Points to the RSA modulus used in the modular exponentiation
     * operation.
     *
     * @note Must be an odd value.
     *       Must be greater than both the input and the exponent.
     *       Length must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    const uint8_t *modulus;

    /**
     * @brief Input message length in bits.
     *
     * Specifies the length of the input data to be processed.
     *
     * @note Must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    size_t input_len_bits;

    /**
     * @brief RSA exponent length in bits.
     *
     * Specifies the length of the public or private exponent.
     *
     * @note Must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    size_t exponent_len_bits;

    /**
     * @brief RSA modulus length in bits.
     *
     * Specifies the length of the RSA modulus.
     *
     * @note Must equal RSA_BLOCK_SIZE_BITS (2048 bits).
     */
    size_t modulus_len_bits;
} RSA_Config_t;

/**
 * @brief Generates an RSA key pair for cryptographic operations.
 * 
 * @details This function generates the RSA key pair including modulus,
 *          public exponent, and private exponent. The generated key pair
 *          can be used for encryption, decryption, and digital signatures.
 * 
 * @param key_pair_output Pointer to structure to store the generated RSA
 *                        key pair (ie. private exponent, public exponent
 *                        and modulus).
 * 
 * @param input_public_expo Pointer to input public exponent, if NULL the
 *                          default value (65537) is taken as public exponent.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESSif the operation completes successfully,
 * - @ref EFAULT if any required pointer is NULL,
 * - @ref EINVAL if the public exponent is invalid,
 * - @ref EDOM if an internal computation exceeds supported limits or the
 *          inverse does not exist,
 * - @ref ETIMEDOUT if the busy bit is not cleared in TRNG within the 
 *          timeout window.
 */
uint16_t RSA_Generate_Key_Pair(RSA_key_pair *key_pair_output, \
                               const uint8_t *input_public_expo);

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
 * @return Returns a 16-bit status code:
 * - @ref SUCCESSif zeroization completed within the timeout window,
 * - @ref ETIMEDOUT if the RSA hardware zeroization does not complete 
 *          within the timeout window.
 */
uint16_t RSA_Zeroize(void);

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
 * @param rsa_config Pointer to an RSA_Config_t structure containing
 * input/output buffers, the public/private exponent, the modulus, and
 * their respective lengths in bits.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  if the operation completes successfully
 * - @ref EFAULT    if any required pointer is NULL
 * - @ref EINVAL    if any of the following input validation checks fail:
 *                      - input, modulus, or exponent length is not 
 *                        RSA_BLOCK_SIZE_BITS (2048 bits),
 *                      - modulus or exponent is not an odd value,
 *                      - exponent is not strictly less than the modulus,
 *                      - input is not strictly less than the modulus,
 * - @ref ETIMEDOUT if the RSA hardware does not respond within the
 *                     timeout window.
 */
uint16_t RSA_Run(RSA_Config_t *rsa_config);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_RSA_H_
