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
 * @file aes.h
 * @brief Contains APIs, structures, and enums for AES operations.
 * @details This header file defines macros, enumerations, function prototypes,
 *          and the AES_Config_t structure for performing AES encryption
 *          and decryption using the AES hardware accelerator. 
 * @version 1.2
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Jennifer Vinita J (jennifer@mindgrovetech.in)
 *          Dayana Devi K (dayana@mindgrovetech.in)
 * @date 25-02-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-11-2024 | 1.0     | Vishwajith N S        | Initial release.
 * 11-12-2025 | 1.1     | Jennifer Vinita J     | Removed multi-run APIs and 
 *            |         |                       | added enums. 
 * 25-02-2026 | 1.2     | Dayana Devi K         | Added AES_Config_t structure, 
 *            |         |                       | formatted header file in 
 *            |         |                       | Doxygen style and MISRA compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_AES_H_
#define BSP_INCLUDE_AES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
* @enum AES_OPERATION
* @brief AES execution operation type.
*
* This enumeration defines whether the AES hardware performs
* encryption or decryption.
*/
typedef enum {
    /** The value of this enum is 0U. It represents AES encryption operation. */
    AES_ENCRYPT = 0U,
    /** The value of this enum is 1U. It represents AES decryption operation. */
    AES_DECRYPT = 1U
} AES_OPERATION;

/**
* @enum AES_MODE
* @brief AES block cipher operating modes.
*
* This enumeration defines the supported AES hardware
* block cipher modes of operation.
*/
typedef enum {
    /** The value of this enum is 0U. It represents AES CBC mode. */
    AES_CBC = 0U,
    /** The value of this enum is 1U. It represents AES CFB mode. */
    AES_CFB = 1U,
    /** The value of this enum is 2U. It represents AES OFB mode. */
    AES_OFB = 2U,
    /** The value of this enum is 3U. It represents AES CTR mode. */
    AES_CTR = 3U
} AES_MODE;

/**
 * @brief AES configuration structure.
 *
 * @details This structure contains all parameters required to configure
 * and execute an AES operation using the AES hardware module.
 * It provides input/output buffers, key and IV configuration,
 * operation mode selection, and iteration control parameters.
 *
 * @note AES_ECB mode is not supported by this implementation.
 */
typedef struct {
    /**
     * @brief Pointer to AES output buffer.
     *
     * Points to the buffer where the encrypted or decrypted
     * output data will be stored.
     *
     * @note The buffer must be large enough to hold input_len_bits.
     */
    uint8_t *aes_output;

    /**
     * @brief Pointer to input text buffer.
     *
     * Points to the input message to be encrypted or decrypted.
     *
     * @note The input length must be a multiple of 128 bits.
     */
    const uint8_t *input_text;

    /**
     * @brief Pointer to AES key.
     *
     * Points to the encryption/decryption key used by the AES hardware.
     *
     * @note Supported key lengths are 128, 192, or 256 bits only.
     */
    const uint8_t *key;

    /**
     * @brief Pointer to initialization vector (IV).
     *
     * Points to the 128-bit IV used in supported block cipher modes.
     *
     * @note Required for CBC, CFB, OFB, and CTR modes.
     */
    const uint8_t *iv;

    /**
     * @brief Input message length in bits.
     *
     * Specifies the total length of the input data to be processed.
     *
     * @note Must be a multiple of 128 bits.
     */
    size_t input_len_bits;

    /**
     * @brief AES key length in bits.
     *
     * Defines the size of the AES key.
     *
     * @note Valid values are 128U, 192U, or 256U.
     */
    size_t key_len_bits;

    /**
     * @brief AES block cipher mode of operation.
     *
     * Selects the hardware-supported AES mode.
     *
     * @note Supported modes: AES_CBC, AES_CFB, AES_OFB, AES_CTR.
     *       AES_ECB is not supported.
     */
    AES_MODE mode;

    /**
     * @brief AES operation type.
     *
     * Selects whether the hardware performs encryption or decryption.
     *
     * @note AES_ENCRYPT = 0U, AES_DECRYPT = 1U.
     */
    AES_OPERATION encrypt_or_decrypt;

    /**
     * @brief Previously processed length in bits.
     *
     * Indicates the number of bits already processed in earlier
     * AES iterations.
     *
     * @note Set to 0U for the initial AES operation.
     */
    size_t iterated_length_bits;
} AES_Config_t;

/**
 * @brief Performs AES encryption or decryption on input data.
 *
 * @details This function encrypts or decrypts the provided input buffer
 * using the AES hardware accelerator. Data is processed in 128-bit blocks
 * and the resulting output is stored in the user-provided output buffer.
 *
 * This function supports both single-call and multi-call (iterative)
 * operation. In single-call mode, the entire input is processed in one
 * invocation by setting iterated_length_bits to 0. In multi-call mode,
 * the input is split across multiple calls — on the first call
 * iterated_length_bits must be 0, and on subsequent calls it must reflect
 * the total number of bits already processed. The hardware is zeroized and
 * reconfigured only on the first call (iterated_length_bits == 0), and the
 * hardware state is preserved across calls to allow chained block processing.
 *
 * @param aes_config Pointer to an AES_Config_t structure containing 
 * input/output buffers, AES key, IV, operation mode, key length, input 
 * length, and iteration parameters.
 *
 * @return `SUCCESS` if the operation completes successfully,
 *         `EFAULT` if any required pointer is NULL,
 *         `EINVAL` if any parameter is invalid,
 *         `ETIMEDOUT` if the AES hardware does not respond within the
 *          timeout window.
 */
uint16_t AES_Run(AES_Config_t *aes_config);

/**
 * @brief Triggers zeroization of all internal AES hardware registers.
 *
 * @details Writes 1 to the AES_ZEROIZE register to initiate a hardware
 * clear of all internal AES state. Polls the AES_ZEROIZE_STATUS register
 * using a constant-time busy-wait until zeroization is confirmed complete
 * or the timeout window expires.
 *
 * This function may be called independently to ensure the hardware is in
 * a clean state before a new AES operation.
 *
 * @return `SUCCESS` if zeroization completed within the timeout window,
 *         `ETIMEDOUT` if the AES hardware zeroization does not complete
 *          within the timeout window.
 */
uint16_t AES_Zeroize(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_AES_H_
