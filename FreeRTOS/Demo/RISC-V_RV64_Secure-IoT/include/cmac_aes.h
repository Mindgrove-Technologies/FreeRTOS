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
 * @file cmac_aes.h
 * @brief Contains APIs for performing CMAC-AES (Cipher-based Message
 *        Authentication Code) tag generation and verification.
 * @details This header file contains function prototypes, macro definitions,
 *          and data types required for implementing AES-CMAC as specified in
 *          NIST SP 800-38B and RFC 4493. Supports authenticated tag generation
 *          and verification using 128, 192, or 256-bit AES keys with
 *          configurable tag lengths between 32 and 128 bits.
 * @version 1.0
 * @authors Dayana Devi K (dayana@mindgrovetech.in)
 * @date 04-06-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 04-06-2026 | 1.0     | Dayana Devi K         | Initial release.
 * -----------------------------------------------------------------------------
 */

#ifndef CMAC_AES_H
#define CMAC_AES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @enum CMAC_AES_Mode_t
 * @brief CMAC-AES operation modes.
 *
 * This enumeration defines the supported modes for CMAC-AES operations,
 * specifying whether tag generation or verification is performed.
 */
typedef enum {
    /** CMAC-AES tag generation mode. Computes and stores the authentication
     *  tag for the given message and key. */
    CMAC_GENERATE = 0U,
    /** CMAC-AES tag verification mode. Computes the authentication tag and
     *  compares it against the provided tag using a constant-time comparison. */
    CMAC_VERIFY   = 1U
} CMAC_AES_Mode_t;

/**
 * @brief CMAC-AES configuration structure.
 *
 * @details This structure contains all parameters required to perform
 * AES-CMAC tag generation or verification, including the key, message,
 * authentication tag buffer, and operation mode.
 *
 * @note
 * 1. AES key length (`key_len_bits`) must be 128, 192, or 256 bits.
 * 2. Message length (`message_len_bits`) must be a multiple of 8 bits
 *    (byte-aligned). A zero-length message is supported.
 * 3. The `message` pointer may be `NULL` only if `message_len_bits` is 0.
 * 4. Authentication tag length (`tag_len_bits`) must be a multiple of 8 bits
 *    between 32 and 128 bits.
 * 5. In verification mode, `tag` must point to the expected tag to compare
 *    against. On mismatch, the tag buffer is zeroed and `AUTH_TAG_MISMATCH`
 *    is returned.
 */
typedef struct {
    /**
     * @brief AES key pointer.
     *
     * Pointer to the key used for CMAC tag generation or verification.
     */
    uint8_t *key;

    /**
     * @brief AES key length in bits.
     *
     * Valid values: 128, 192, 256.
     */
    size_t key_len_bits;

    /**
     * @brief Input message pointer.
     *
     * Pointer to the message over which the authentication tag is computed.
     * May be `NULL` if `message_len_bits` is 0.
     */
    uint8_t *message;

    /**
     * @brief Input message length in bits.
     *
     * Must be a multiple of 8 bits. Zero-length messages are supported.
     */
    size_t message_len_bits;

    /**
     * @brief Authentication tag buffer pointer.
     *
     * In generation mode, the computed tag is written into this buffer.
     * In verification mode, this buffer must contain the expected tag
     * to be compared against the computed tag.
     */
    uint8_t *tag;

    /**
     * @brief Authentication tag length in bits.
     *
     * Must be a multiple of 8 bits and between 32 and 128 bits (inclusive).
     * Only the leftmost `tag_len_bits` bits of the full 128-bit CMAC output
     * are used, per NIST SP 800-38B.
     */
    size_t tag_len_bits;

    /**
     * @brief CMAC-AES operation mode.
     *
     * Determines whether the operation is tag generation or verification.
     * Valid values:
     *   - CMAC_GENERATE
     *   - CMAC_VERIFY
     */
    CMAC_AES_Mode_t mode;
} CMAC_AES_Config;

/**
 * @brief Performs CMAC-AES authentication tag generation or verification.
 *
 * @details This function computes the AES-CMAC authentication tag over the
 * provided message using the specified key, following NIST SP 800-38B and
 * RFC 4493.
 *
 * - In generation mode, the computed tag is written to `cfg->tag`, truncated
 *   to `cfg->tag_len_bits` bits (leftmost bytes of the full 128-bit tag).
 * - In verification mode, the computed tag is compared against `cfg->tag`
 *   using a constant-time comparison. If verification fails, `cfg->tag` is
 *   zeroed and `AUTH_TAG_MISMATCH` is returned.
 *
 * Subkey generation (K1, K2) and final tag computation use hardware-
 * accelerated AES-ECB operations. Intermediate CBC chaining over message
 * blocks uses hardware-accelerated AES-CBC operations.
 *
 * @param cfg Pointer to a CMAC_AES_Config structure containing the key,
 * message, tag buffer, lengths, and operation mode.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS if the operation completes successfully,
 * - @ref AUTH_TAG_MISMATCH if tag verification fails in verify mode,
 * - @ref if any required pointer in `cfg` is NULL, and
 * - @ref if key length, tag length, message length, or mode are invalid.
 */
uint16_t CMAC_AES(CMAC_AES_Config *cfg);

#ifdef __cplusplus
}
#endif

#endif /* CMAC_AES_H */
