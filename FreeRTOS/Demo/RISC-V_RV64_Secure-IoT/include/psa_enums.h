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
 * @file psa_enums.h
 * @brief Header file for enums for PSA Crypto APIs in Mindgrove Silicon Secure-IoT SoC
 * @details This is the header file for the enums used in  PSA Crypto APIs in 
 *          Secure-IOT Device. 
 * @version 1.0
 * @authors Jennifer Vinita J (jennifer@mindgrovetech.in) 
 * @date 12-08-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-08-2025 | 1.0     | Jennifer Vinita J     | Initial release.  
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PSA_ENUMS_H_
#define BSP_INCLUDE_PSA_ENUMS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum psa_key_creation_method_t
 * @brief Indicates how a cryptographic key is created.
 *
 * This enumeration is used to track the origin of a key during its
 * creation, whether by import, generation, derivation, or copy.
 */
typedef enum {
    /** Key created during psa_import_key(). */
    PSA_KEY_CREATION_IMPORT,
    /** Key created during psa_generate_key(). */
    PSA_KEY_CREATION_GENERATE,
    /** Key created during psa_key_derivation_output_key(). */
    PSA_KEY_CREATION_DERIVE,
    /** Key created during psa_copy_key(). */
    PSA_KEY_CREATION_COPY
} psa_key_creation_method_t;

/**
 * @enum psa_key_slot_state_t
 * @brief Represents the state of a key slot in the PSA key store.
 */
typedef enum {
    /** Key slot is empty and available for use. */
    PSA_SLOT_EMPTY = 0U,
    /** Key slot is being filled (key creation in progress). */
    PSA_SLOT_FILLING,
    /** Key slot is full and contains a valid key. */
    PSA_SLOT_FULL,
    /** Key slot is pending deletion and may be wiped soon. */
    PSA_SLOT_PENDING_DELETION
} psa_key_slot_state_t;

/**
 * @enum mg_operation_t
 * @brief Type of cipher operation.
 */
typedef enum {
    /** No operation. */
    MG_PSA_AES_OPERATION_NONE = -1,
    /** Encryption operation. */
    MG_PSA_AES_ENCRYPT = 0,
    /** Decryption operation. */
    MG_PSA_AES_DECRYPT = 1
} mg_operation_t;

/**
 * @enum mg_cipher_mode_t
 * @brief Supported cipher modes.
 */
typedef enum {
    MG_MODE_NONE = -1, /**< No cipher mode. */
    MG_MODE_CBC = 0,   /**< CBC mode. */
    MG_MODE_CFB,       /**< CFB mode. */
    MG_MODE_OFB,       /**< OFB mode. */
    MG_MODE_CTR        /**< CTR mode. */
} mg_cipher_mode_t;

/**
 * @enum mg_cipher_psa_key_ownership
 * @brief Indicates whether a PSA-based cipher context owns its key.
 */
typedef enum {
    /** Key is unset. */
    MG_CIPHER_PSA_KEY_UNSET = 0,
    /** Context owns a raw volatile key and must destroy it on free. */
    MG_CIPHER_PSA_KEY_OWNED,
    /** Context uses a user-provided key and will not destroy it on free. */
    MG_CIPHER_PSA_KEY_NOT_OWNED
} mg_cipher_psa_key_ownership;

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PSA_ENUMS_H_
