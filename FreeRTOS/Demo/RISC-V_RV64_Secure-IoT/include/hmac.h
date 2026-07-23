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
 * @file hmac.h
 * @brief Public API for HMAC driver
 * @details Declares HMAC context structures, macros, and function
 *          prototypes required to compute Hash-based Message
 *          Authentication Codes (HMAC) using SHA-2 and SHA-3
 *          hash algorithms.
 * @version 1.0
 * @authors Sai Girish S (saigirish@mindgrovetech.in)
 * @date 29-01-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 29-01-2026 | 1.0     | Sai Girish S          | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_HMAC_H_
#define BSP_INCLUDE_HMAC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @enum HMAC_HashType_t
 * @brief Supported hash algorithm identifiers for HMAC.
 *
 * This enumeration defines all hash variants supported by
 * the HMAC driver.
 */
typedef enum {
    /** The value of this enum is 0U. It represents SHA-256. */
    SHA_256 = 0U,

    /** The value of this enum is 1U. It represents SHA-384. */
    SHA_384 = 1U,

    /** The value of this enum is 2U. It represents SHA-512. */
    SHA_512 = 2U,

    /** The value of this enum is 3U. It represents SHA3-224. */
    SHA3_224 = 3U,

    /** The value of this enum is 4U. It represents SHA3-256. */
    SHA3_256 = 4U,

    /** The value of this enum is 5U. It represents SHA3-384. */
    SHA3_384 = 5U,

    /** The value of this enum is 6U. It represents SHA3-512. */
    SHA3_512 = 6U
} HMAC_HashType_t;

/* Opaque HMAC context structure defined inside the driver. */
typedef struct Hmac Hmac;

/**
 * @brief Get size of HMAC context structure
 *
 * This function returns the number of bytes required to store
 * an HMAC context. It allows the user to allocate memory without
 * exposing the internal structure.
 *
 * @return Size of HMAC context in bytes
 */
size_t HMAC_Get_Context_Size(void);

/**
 * @brief Clears the HMAC context.
 *
 * @details This function resets the entire HMAC structure by
 * zero-initializing all internal members.
 *
 * After calling this function, the context becomes uninitialized
 * and @ref HMAC_Set_Key() must be called again before performing
 * any HMAC operation.
 *
 * @param hmac Pointer to HMAC context.
 *
 * @return
 * - @ref SUCCESS  : Context successfully cleared
 * - @ref EFAULT   : hmac is NULL
 */
uint16_t HMAC_Reset(Hmac *hmac);

/**
 * @brief Sets the key and initializes the HMAC context.
 *
 * @details This function:
 * - Selects the hash algorithm
 * - Processes the user-provided key
 * - Generates inner and outer padded keys (ipad, opad)
 * - Initializes the internal hash state
 *
 * Must be called before @ref HMAC_Update() and @ref HMAC_Final().
 *
 * @param hmac   Pointer to HMAC context.
 * @param type   Hash algorithm type (see @ref HMAC_HashType_t).
 * @param key    Pointer to secret key.
 * @param length Length of the key in bytes.
 *
 * @return
 * - @ref SUCCESS   : Key successfully configured
 * - @ref EFAULT    : hmac is NULL
 * - @ref EINVAL    : Invalid hash type
 * - @ref EPERM     : key is NULL while length is non-zero
 * - @ref ETIMEDOUT : SHA hardware timeout (only for SHA-256)
 */
uint16_t HMAC_Set_Key(Hmac* hmac, HMAC_HashType_t type,
                      const uint8_t* key, size_t length);

/**
 * @brief Processes input message data.
 *
 * @details Feeds message data into the HMAC inner hash computation.
 * This function may be called multiple times to process data in chunks.
 *
 * @param hmac   Pointer to initialized HMAC context.
 * @param msg    Pointer to message buffer.
 * @param length Length of message in bytes.
 *
 * @return
 * - @ref SUCCESS   : Data successfully processed
 * - @ref EFAULT    : hmac is NULL
 * - @ref EINVAL    : Invalid HMAC state or hash type
 * - @ref EPERM     : msg is NULL while length is non-zero
 * - @ref ETIMEDOUT : SHA hardware timeout (SHA-256 only)
 */
uint16_t HMAC_Update(Hmac* hmac, const uint8_t* msg, size_t length);

/**
 * @brief Finalizes HMAC computation and produces the authentication code.
 *
 * @details This function completes the inner hash computation,
 * performs the outer hash processing, and writes the final
 * Message Authentication Code (MAC) to the output buffer.
 *
 * Upon successful completion:
 * - The intermediate inner hash buffer is cleared.
 * - The internal hash state is reinitialized.
 * - The context remains configured with the previously set key.
 *
 * The HMAC context is ready to process a new message using
 * the same key without requiring @ref HMAC_Set_Key() again.
 *
 * @param hmac Pointer to initialized HMAC context.
 * @param hash Output buffer to store the computed MAC.
 *
 * @return
 * - @ref SUCCESS   : MAC successfully generated
 * - @ref EFAULT    : hmac is NULL
 * - @ref EINVAL    : Invalid HMAC state or hash type
 * - @ref ETIMEDOUT : SHA hardware timeout (SHA-256 only)
 */
uint16_t HMAC_Final(Hmac* hmac, uint8_t* hash);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INCLUDE_HMAC_H_ */
