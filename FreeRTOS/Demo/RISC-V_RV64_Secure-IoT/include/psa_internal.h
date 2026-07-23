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
 * @file psa_internal.h
 * @brief Internal header for PSA Crypto APIs on the Secure-IoT Processor.
 * @details This header declares the internal interfaces, macros, and
 *          utility functions used by the PSA Crypto implementation
 *          for the Secure-IoT Device. It includes definitions for key
 *          management, hashing, random number generation, and other
 *          cryptographic operations used within the library.
 * @version 1.1
 * @authors Jennifer Vinita J (jennifer@mindgrovetech.in) 
 *          Dayana Devi K (dayana@mindgrovetech.in)
 * @date 02-02-2026
 *  
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-08-2025 | 1.0     | Jennifer Vinita J     | Initial release.  
 * 02-02-2026 | 1.1     | Dayana Devi K         | Added Macro for clean up and
 *            |         |                       | made MISRA-C compliance updates.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PSA_INTERNAL_H_
#define BSP_INCLUDE_PSA_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "psa_enums.h"
#include "psa_errors.h"
#include "psa_types.h"
#include "memory_functions.h"

/**
 * @defgroup PSA_UTILITY_MACROS PSA Utility Macros
 *
 * @brief Helper macros for key size conversions, rounding, cleanup, and
 *        byte extraction from multi-byte integers.
 *
 * @{
 */

/** @brief Converts bits to bytes (rounded up). */
#define PSA_BITS_TO_BYTES(bits) \
    ((((size_t)(bits)) + (size_t)7u) / (size_t)8u)

/** @brief Converts bytes to bits. */
#define PSA_BYTES_TO_BITS(bytes) ((bytes) * 8u)

/** @brief Returns the maximum of three values. */
#define PSA_MAX_OF_THREE(a, b, c) ((a) <= (b) ? (b) <= (c) ? \
                                   (c) : (b) : (a) <= (c) ? (c) : (a))

/** @brief Rounds length up to nearest multiple of block_size. */
#define PSA_ROUND_UP_TO_MULTIPLE(block_size, length) \
    (((length) + (block_size) - 1) / (block_size) * (block_size))

/** @brief Clears the output buffer and resets output length on failure. */
#define MG_CLEANUP_ON_FAILURE(out, out_size, out_len_ptr)     \
    do {                                                      \
        if ((out_len_ptr) != NULL) {                          \
            *(out_len_ptr) = 0U;                              \
        }                                                     \
        if (((out) != NULL) && ((out_size) > 0U)) {           \
            (void) memset((out), 0, (out_size));              \
        }                                                     \
    } while (0)

/** @brief Initializes a context structure to zero. */
#define MG_INIT_CONTEXT(ctx)                                  \
    do {                                                      \
        if ((ctx) != NULL) {                                  \
            (void) memset((ctx), 0, sizeof(*(ctx)));          \
        }                                                     \
    } while (0)

/** @brief Returns the length field of a context structure. */
#define MG_CONTEXT_LEN(ctx)     ((ctx)->len)

/** @brief Frees the RSA key components from the context. */
#define MG_FREE_RSA_CONTEXT(ctx)            \
    do {                                    \
        if ((ctx) != NULL) {                \
            BN_FREE(&(ctx)->D);      \
            BN_FREE(&(ctx)->E);      \
            BN_FREE(&(ctx)->N);      \
        }                                   \
    } while (0)

/** @brief Retrieve specific byte from a multi-byte integer. Byte 0 is LSB. */
#define MG_BYTE_0(x) ((uint8_t) ((x)          & 0xffU))
#define MG_BYTE_1(x) ((uint8_t) (((x) >>  8U) & 0xffU))
#define MG_BYTE_2(x) ((uint8_t) (((x) >> 16U) & 0xffU))
#define MG_BYTE_3(x) ((uint8_t) (((x) >> 24U) & 0xffU))
#define MG_BYTE_4(x) ((uint8_t) (((x) >> 32U) & 0xffU))
#define MG_BYTE_5(x) ((uint8_t) (((x) >> 40U) & 0xffU))
#define MG_BYTE_6(x) ((uint8_t) (((x) >> 48U) & 0xffU))
#define MG_BYTE_7(x) ((uint8_t) (((x) >> 56U) & 0xffU))

/** @} */

/**
 * @defgroup PSA_KEY_TYPE_MACROS PSA Key Type Macros
 *
 * @brief Macros for identifying key type categories and key pair relationships.
 *
 * @{
 */

/** @brief Checks whether a key type is unstructured (raw/symmetric). */
#define PSA_KEY_TYPE_IS_UNSTRUCTURED(type) \
    (((type) & PSA_KEY_TYPE_CATEGORY_MASK) == PSA_KEY_TYPE_CATEGORY_RAW || \
     ((type) & PSA_KEY_TYPE_CATEGORY_MASK) == PSA_KEY_TYPE_CATEGORY_SYMMETRIC)

/** @brief Checks whether a key type is asymmetric (public or key pair). */
#define PSA_KEY_TYPE_IS_ASYMMETRIC(type) \
    (((psa_key_type_t)(type) & (psa_key_type_t)PSA_KEY_TYPE_CATEGORY_MASK & \
      (psa_key_type_t)(~PSA_KEY_TYPE_CATEGORY_FLAG_PAIR)) == \
     (psa_key_type_t)PSA_KEY_TYPE_CATEGORY_PUBLIC_KEY)

/** @brief Checks whether a key type is a public key. */
#define PSA_KEY_TYPE_IS_PUBLIC_KEY(type) \
    (((type) & PSA_KEY_TYPE_CATEGORY_MASK) == PSA_KEY_TYPE_CATEGORY_PUBLIC_KEY)

/** @brief Checks whether a key type is a key pair (private+public). */
#define PSA_KEY_TYPE_IS_KEY_PAIR(type) \
    (((type) & PSA_KEY_TYPE_CATEGORY_MASK) == PSA_KEY_TYPE_CATEGORY_KEY_PAIR)

/** @brief Returns the key pair type corresponding to a public key type. */
#define PSA_KEY_TYPE_KEY_PAIR_OF_PUBLIC_KEY(type) \
    ((type) | PSA_KEY_TYPE_CATEGORY_FLAG_PAIR)

/** @brief Returns the public key type corresponding to a key pair type. */
#define PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(type) \
    ((psa_key_type_t)(type) & (psa_key_type_t) \
     (~PSA_KEY_TYPE_CATEGORY_FLAG_PAIR))

/** @brief Checks whether a key type is RSA (pair or public-only). */
#define PSA_KEY_TYPE_IS_RSA(type) \
    (PSA_KEY_TYPE_PUBLIC_KEY_OF_KEY_PAIR(type) == PSA_KEY_TYPE_RSA_PUBLIC_KEY)

/** @} */

/**
 * @defgroup PSA_KEY_LIFETIME_MACROS PSA Key Lifetime Macros
 *
 * @brief Macros for handling key persistence, location, and volatility.
 *
 * @{
 */

/** @brief Extracts persistence from key lifetime. */
#define PSA_KEY_LIFETIME_GET_PERSISTENCE(lifetime) \
    ((psa_key_persistence_t)((lifetime) & 0x000000ffU))

/** @brief Extracts location from key lifetime. */
#define PSA_KEY_LIFETIME_GET_LOCATION(lifetime) \
    ((psa_key_location_t)((lifetime) >> 8))

/** @brief Checks whether a key lifetime indicates a volatile key. */
#define PSA_KEY_LIFETIME_IS_VOLATILE(lifetime) \
    (PSA_KEY_LIFETIME_GET_PERSISTENCE(lifetime) == PSA_KEY_PERSISTENCE_VOLATILE)

/** @brief Constructs a key lifetime from persistence and location. */
#define PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(persistence, location) \
    (((location) << 8) | (persistence))

/** @} */

/**
 * @defgroup PSA_ALG_MACROS PSA Algorithm Macros
 *
 * @brief Macros to classify algorithms (hash, cipher, MAC, AEAD, signature).
 *
 * @{
 */

/** @brief Checks if an algorithm is a hash algorithm. */
#define PSA_ALG_IS_HASH(alg) \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_HASH)

/** @brief Checks if an algorithm is a supported hash algorithm. */
#define MG_PSA_ALG_IS_SUPPORTED_HASH(alg) \
    ((alg) == PSA_ALG_SHA_256)

/** @brief Checks if an algorithm is a symmetric cipher. */
#define PSA_ALG_IS_CIPHER(alg) \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_CIPHER)

/** @brief Checks if an algorithm is a supported symmetric cipher. */
#define MG_PSA_ALG_IS_SUPPORTED_CIPHER(alg)        \
    (((alg) == PSA_ALG_CBC_NO_PADDING) ||           \
     ((alg) == PSA_ALG_CTR)            ||           \
     ((alg) == PSA_ALG_CFB)            ||           \
     ((alg) == PSA_ALG_OFB))

/** @brief Checks if an algorithm is a MAC. */
#define PSA_ALG_IS_MAC(alg) \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_MAC)

/** @brief Checks if an algorithm is AEAD. */
#define PSA_ALG_IS_AEAD(alg) \
    (((alg) & PSA_ALG_CATEGORY_MASK) == PSA_ALG_CATEGORY_AEAD)

/** @brief Get the block size exponent from a block cipher key type. */
#define PSA_GET_KEY_TYPE_BLOCK_SIZE_EXPONENT(type) \
    ((unsigned int)((((psa_key_type_t)(type)) >> 8) & (psa_key_type_t)7U))

/** @brief Get the block size of a block cipher key type.*/
#define PSA_BLOCK_CIPHER_BLOCK_LENGTH(type)                             \
    ((((psa_key_type_t)(type) & PSA_KEY_TYPE_CATEGORY_MASK) ==          \
      PSA_KEY_TYPE_CATEGORY_SYMMETRIC) ?                                \
     ((size_t)1U << PSA_GET_KEY_TYPE_BLOCK_SIZE_EXPONENT(type)) :       \
     (size_t)0U)

/** @brief Get the default IV length for a cipher algorithm. */
#define PSA_CIPHER_IV_LENGTH(key_type, alg)                               \
    ((PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type) > (size_t)1U) ?             \
      (((alg) == PSA_ALG_CTR) ? PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type) : \
       ((alg) == PSA_ALG_CFB) ? PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type) : \
       ((alg) == PSA_ALG_OFB) ? PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type) : \
       ((alg) == PSA_ALG_CBC_NO_PADDING) ?                                \
        PSA_BLOCK_CIPHER_BLOCK_LENGTH(key_type) : (size_t)0U) : (size_t)0U)

/** @brief Checks if an algorithm is RSA PSS with standard salt. */
#define PSA_ALG_IS_RSA_PSS_STANDARD_SALT(alg) \
    ((((alg) & (~PSA_ALG_HASH_MASK)) == PSA_ALG_RSA_PSS_BASE) ? 1U : 0U)

/** @brief Checks if an algorithm is RSA PSS with any salt. */
#define PSA_ALG_IS_RSA_PSS_ANY_SALT(alg) \
    ((((alg) & (~PSA_ALG_HASH_MASK)) == PSA_ALG_RSA_PSS_ANY_SALT_BASE) ? \
     1U : 0U)

/** @brief Checks if an algorithm is RSA PSS. */
#define PSA_ALG_IS_RSA_PSS(alg) \
    (((PSA_ALG_IS_RSA_PSS_STANDARD_SALT(alg) != 0U) || \
      (PSA_ALG_IS_RSA_PSS_ANY_SALT(alg) != 0U)) ? 1U : 0U)

/** @brief Checks if an algorithm is RSA PKCS#1 v1.5 signature. */
#define PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) \
    ((((alg) & (~PSA_ALG_HASH_MASK)) == PSA_ALG_RSA_PKCS1V15_SIGN_BASE) ? \
     1U : 0U)

/** @brief Checks if an algorithm is ECDSA. */
#define PSA_ALG_IS_ECDSA(alg) \
    ((((alg) & (~PSA_ALG_HASH_MASK) & (~PSA_ALG_ECDSA_DETERMINISTIC_FLAG)) == \
      PSA_ALG_ECDSA_BASE) ? 1U : 0U)

/** @brief Checks if an algorithm is hash-based EdDSA. */
#define PSA_ALG_IS_HASH_EDDSA(alg) \
    ((((alg) & (~PSA_ALG_HASH_MASK)) == PSA_ALG_HASH_EDDSA_BASE) ? 1U : 0U)

/** @brief Placeholder for vendor-specific hash-and-sign algorithms. */
#define PSA_ALG_IS_VENDOR_HASH_AND_SIGN(alg) 0U

/** @brief Checks if an algorithm is a sign hash algorithm. */
#define PSA_ALG_IS_SIGN_HASH(alg) \
    (((PSA_ALG_IS_RSA_PSS(alg) != 0U)            || \
      (PSA_ALG_IS_RSA_PKCS1V15_SIGN(alg) != 0U)  || \
      (PSA_ALG_IS_ECDSA(alg) != 0U)               || \
      (PSA_ALG_IS_HASH_EDDSA(alg) != 0U)          || \
      (PSA_ALG_IS_VENDOR_HASH_AND_SIGN(alg) != 0U)) ? 1U : 0U)

/** @brief Checks if an algorithm is a hash-and-sign algorithm. */
#define PSA_ALG_IS_HASH_AND_SIGN(alg) \
    (((PSA_ALG_IS_SIGN_HASH(alg) != 0U) && \
      (((alg) & PSA_ALG_HASH_MASK) != (psa_algorithm_t)0U)) ? 1U : 0U)

/** @brief Extracts the hash algorithm from a hash-and-sign 
 * signature algorithm. */
#define PSA_ALG_SIGN_GET_HASH(alg) \
    ((PSA_ALG_IS_HASH_AND_SIGN(alg) != 0U) ? \
     ((psa_algorithm_t)(((alg) & PSA_ALG_HASH_MASK) | \
                         PSA_ALG_CATEGORY_HASH)) : ((psa_algorithm_t)0U))

/** @brief Checks if an algorithm is an asymmetric encryption algorithm. */
#define PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(alg) \
    (((alg) == PSA_ALG_RSA_PKCS1V15_CRYPT) || \
     ((alg) == PSA_ALG_RSA_OAEP_SHA256))

/** @brief Checks if an algorithm is a wildcard. */
#define PSA_ALG_IS_WILDCARD(alg)                                              \
    (PSA_ALG_IS_HASH_AND_SIGN(alg) ?                                          \
        (PSA_ALG_SIGN_GET_HASH(alg) == PSA_ALG_ANY_HASH) :                    \
     PSA_ALG_IS_MAC(alg) ?                                                    \
        (((alg) & PSA_ALG_MAC_AT_LEAST_THIS_LENGTH_FLAG) != 0U) :             \
     PSA_ALG_IS_AEAD(alg) ?                                                   \
        (((alg) & PSA_ALG_AEAD_AT_LEAST_THIS_LENGTH_FLAG) != 0U) :            \
        ((alg) == PSA_ALG_ANY_HASH))

/** @brief Checks if a key length is valid for AES (128, 192, or 256 bits). */
#define MG_PSA_IS_VALID_AES_KEY_BITS(key_bits)              \
    (((key_bits) == PSA_MG_AES_KEY_SIZE_128_BITS) ||             \
     ((key_bits) == PSA_MG_AES_KEY_SIZE_192_BITS) ||             \
     ((key_bits) == PSA_MG_AES_KEY_SIZE_256_BITS))

/** @brief Maps a supported PSA cipher algorithm to its corresponding
 *         mg cipher mode. */
#define MG_PSA_ALG_TO_CIPHER_MODE(alg)                              \
    (((alg) == PSA_ALG_CBC_NO_PADDING) ? MG_MODE_CBC :              \
     ((alg) == PSA_ALG_CFB)            ? MG_MODE_CFB :              \
     ((alg) == PSA_ALG_OFB)            ? MG_MODE_OFB :              \
     ((alg) == PSA_ALG_CTR)            ? MG_MODE_CTR : MG_MODE_NONE)

/** @} */

/**
 * @defgroup Key_Management_Functions Key Management Functions
 * @brief Functions for managing PSA key slots and validating key attributes.
 *
 * @details This module provides APIs to allocate, lock, wipe, destroy, and
 *          validate keys, as well as manipulate key attributes (type, bits,
 *          algorithm, lifetime, usage). It ensures secure key handling,
 *          enforces policies, and manages internal slot state for both
 *          volatile and persistent keys.
 *
 * @{
 */

/**
 * @brief Validates the bit size of an unstructured key based on its type.
 *
 * @details This function checks whether a given bit size is valid for the
 *          specified key type. It ensures compatibility with supported key
 *          types and enforces constraints such as AES key lengths (128, 192,
 *          or 256 bits) and byte-alignment requirements. This validation is
 *          important before key import or creation to prevent invalid or
 *          unsupported keys.
 *
 * @param type  Key type to validate. Supported types include:
 *              - PSA_KEY_TYPE_RAW_DATA
 *              - PSA_KEY_TYPE_HMAC
 *              - PSA_KEY_TYPE_DERIVE
 *              - PSA_KEY_TYPE_AES
 * @param bits  Size of the key in bits.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the bit size is valid for the key type.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the bit size is not allowed or not a
 *   multiple of 8.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the key type is not supported.
 */
psa_status_t psa_validate_unstructured_key_bit_size(psa_key_type_t type,
                                                    size_t bits);

/**
 * @brief Validates a set of key attributes for correctness and consistency.
 *
 * @details This function checks that the specified key attributes conform to
 *          PSA requirements. It validates the key lifetime, key identifier,
 *          key policy, and ensures that the key size does not exceed
 *          implementation limits. Volatile keys must not have a predefined
 *          key identifier, while persistent keys must have a valid key ID.
 *
 * @param attributes Pointer to the key attributes to validate.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the attributes are valid.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the key ID is invalid or inconsistent with
 *   the lifetime, or if the key policy is invalid.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the key size exceeds the maximum supported
 *   key length.
 */
psa_status_t psa_validate_key_attributes(const psa_key_attributes_t
                                         *attributes);

/**
 * @brief Validates optional key attributes against an existing key slot.
 *
 * @details This function ensures that any optional attributes specified in
 *          `attributes` do not conflict with the attributes of an existing
 *          key slot. It is used during key import or creation to enforce
 *          consistency without requiring all attributes to be specified.
 *
 * @param slot       Pointer to the key slot containing existing key attributes.
 * @param attributes Pointer to the optional attributes to validate.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the optional attributes are consistent with the key slot.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if any specified attribute conflicts with the
 *   corresponding attribute of the key slot.
 */
psa_status_t psa_validate_optional_attributes(const psa_key_slot_t *slot,
                                    const psa_key_attributes_t *attributes);

/**
 * @brief Transitions a key slot from an expected state to a new state.
 *
 * @details This function checks whether a key slot is currently in the
 *          specified expected state. If it is, the slot's state is updated
 *          to the new state. This ensures that key slot state transitions
 *          occur in a controlled and predictable manner, preventing
 *          corruption or invalid state changes.
 *
 * @param slot            Pointer to the key slot to transition.
 * @param expected_state  The state that the slot is expected to be in before
 *                        the transition.
 * @param new_state       The new state to set the slot to.
 *
 * @return Returns a 32-bit status code: 
 * - @ref PSA_SUCCESS if the key slot was in the expected state and the transition
 *   succeeded.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if the key slot was not in the expected
 *   state.
 */
psa_status_t psa_key_slot_state_transition(psa_key_slot_t *slot,
                                           psa_key_slot_state_t expected_state,
                                           psa_key_slot_state_t new_state);

/**
 * @brief Unregisters a read operation on a key slot.
 *
 * @details This function decrements the count of active readers for a key
 *          slot. If the slot is marked for deletion and this was the last
 *          reader, the key material is securely wiped. The function ensures
 *          integrity by checking that the slot is in a valid state for read
 *          operations.
 *
 * @param slot Pointer to the key slot structure.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the read was successfully unregistered or the slot pointer
 *   is NULL.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if the slot state is invalid or if there
 *   is an inconsistency in the registered reader count.
 */
psa_status_t psa_unregister_read(psa_key_slot_t *slot);

/**
 * @brief Initializes the key slot storage subsystem.
 *
 * @details This function sets up the key slot structures used by the PSA
 *          Crypto subsystem to store keys. If dynamic key storage is enabled
 *          (MG_PSA_KEY_STORE_DYNAMIC), memory for key slices is allocated
 *          at initialization. After this function successfully completes,
 *          the key slots are marked as initialized and ready for key
 *          operations.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the key slots are successfully initialized.
 * - @ref PSA_ERROR_INSUFFICIENT_MEMORY if dynamic key storage is enabled and
 *   memory allocation fails.
 * - @ref PSA_ERROR_GENERIC_ERROR if PMP protection setup fails.
 */
psa_status_t psa_key_slots_init(void);

/**
 * @brief Retrieves the initialization status of the key slot subsystem.
 *
 * @details This function returns whether the key slots have been initialized
 *          by psa_key_slots_init(). It is typically used internally to verify
 *          that key operations can safely proceed.
 *
 * @return Returns a 32-bit status code:
 * - @ref true if the key slots are initialized and ready for key operations.
 * - @ref false if the key slots are not yet initialized.
 */
bool psa_get_key_slots_initialized(void);

/**
 * @brief Reserves a free key slot for a new key.
 *
 * @details This function searches for an available key slot in the key slot
 *          cache and reserves it for a new key. It handles both volatile and
 *          persistent keys. If a free slot is unavailable, it can recycle a
 *          persistent key slot that is not currently in use. For dynamic key
 *          stores, volatile keys are allocated differently from persistent
 *          keys. The reserved slot is marked as filling until the key
 *          creation or import operation completes.
 *
 * @param volatile_key_id Pointer to a variable to receive the key identifier
 *                        for volatile keys. Can be NULL for non-volatile keys.
 * @param p_slot          Output pointer that will point to the reserved key
 *                        slot on success. NULL on failure.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if a key slot was successfully reserved.
 * - @ref PSA_ERROR_BAD_STATE if the key slots subsystem has not been initialized.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 * - @ref PSA_ERROR_INSUFFICIENT_MEMORY if no free or recyclable slot is available.
 */
psa_status_t psa_reserve_free_key_slot(psa_key_id_t *volatile_key_id,
                                       psa_key_slot_t **p_slot);

/**
 * @brief Imports an RSA key from a DER-encoded buffer.
 *
 * @details Parses a DER-encoded representation of a public or private RSA key
 *          and copies it into a buffer suitable for PSA Crypto key management.
 *          The function validates all input parameters, checks buffer sizing
 *          before parsing, and ensures the RSA context is properly initialized
 *          and freed on both success and failure paths.
 *          The key size in bits is derived from the parsed RSA context.
 *
 * @param attributes        Pointer to a psa_key_attributes_t structure
 *                          describing the key type to import. Must not be NULL.
 * @param data              Pointer to the DER-encoded key data to import.
 *                          Must not be NULL.
 * @param data_length       Length of the input DER-encoded data in bytes.
 *                          Must be non-zero and must not exceed key_buffer_size.
 * @param key_buffer        Pointer to the buffer where the imported key will
 *                          be stored. Must not be NULL.
 * @param key_buffer_size   Size of the key buffer in bytes. Must be non-zero
 *                          and at least as large as data_length.
 * @param key_buffer_length Output parameter set to the number of bytes written
 *                          to key_buffer on success. Must not be NULL.
 * @param bits              Output parameter set to the key size in bits,
 *                          derived from the parsed RSA context. Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful import. key_buffer contains the DER-encoded
 *   key, key_buffer_length is set to data_length, and bits is set to the
 *   key size in bits.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if any pointer parameter is NULL, or if
 *   data_length or key_buffer_size is zero.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if data_length exceeds key_buffer_size.
 * - @ref PSA_ERROR_RSA_BAD_INPUT_DATA if the DER-encoded data cannot be parsed
 *   as a valid RSA key (returned from mg_psa_rsa_load_representation).
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 *
 * @note The internal RSA context is always freed before returning,
 *       on both success and failure paths.
 * @note No output parameters are modified on failure.
 */
psa_status_t mg_psa_rsa_import_key(const psa_key_attributes_t *attributes,
                                    const uint8_t *data, size_t data_length,
                                    uint8_t *key_buffer, size_t key_buffer_size,
                                    size_t *key_buffer_length, size_t *bits);

/**
 * @brief Retrieves an available key slot for storing a new key.
 *
 * @details This function searches for an unused key slot in the system. If
 *          no free slots are available, it attempts to recycle an unlocked
 *          slot containing a persistent key by wiping it. The function then
 *          locks the selected slot and returns a volatile key ID that can be
 *          used to reference it. This is typically used during key creation
 *          or import operations.
 *
 * @param[out] volatile_key_id  Pointer to a variable where the assigned
 *                              volatile key ID will be stored.
 * @param[out] p_slot           Pointer to a variable where the allocated key
 *                              slot pointer will be returned.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if a key slot was successfully allocated and locked.
 * - @ref PSA_ERROR_BAD_STATE if the key slot subsystem is not initialized.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal inconsistency is detected.
 * - @ref PSA_ERROR_INSUFFICIENT_MEMORY if no free or recyclable key slots are
 *   available.
 */
psa_status_t psa_get_empty_key_slot(psa_key_id_t *volatile_key_id,
                                    psa_key_slot_t **p_slot);

/**
 * @brief Wipes all key slots and resets the key slot subsystem.
 *
 * @details This function iterates over all key slots in the system, securely
 *          erasing their contents and resetting associated metadata. After
 *          this operation, the key slot subsystem is marked as uninitialized.
 *          It can be used during system shutdown or when reinitializing the
 *          cryptographic environment to ensure no sensitive key material
 *          remains in memory.
 *
 * @note This operation will forcibly lock and wipe all slots, including
 *       those currently in use.
 */
void psa_wipe_all_key_slots(void);

/**
 * @brief Allocates a key buffer of the specified length within a key slot.
 *
 * @details This function prepares a key slot to hold key material by setting
 *          its buffer size and zeroing any existing data. For static key
 *          slots, the requested buffer length must not exceed the predefined
 *          maximum slot size.
 *
 * @param slot          Pointer to the key slot to allocate the buffer in.
 * @param buffer_length Desired size of the key buffer in bytes.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the buffer is successfully allocated and initialized.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the requested buffer length exceeds the
 *   maximum supported size for static key slots.
 */
psa_status_t psa_allocate_buffer_to_slot(psa_key_slot_t *slot,
                                         size_t buffer_length);

/**
 * @brief Determines the buffer size required to store key data for import.
 *
 * @details This function calculates the number of bits needed in a key slot
 *          to hold the provided key data based on the key's attributes.
 *          Currently, only keys with location PSA_CRYPTO_TEST_DRIVER are
 *          supported. Unsupported locations or zero-length data result in
 *          an error.
 *
 * @param attributes       Pointer to the key attributes describing the key.
 * @param data             Pointer to the key material to import.
 * @param data_length      Length of the key material in bytes.
 * @param key_buffer_size  On success, contains the required buffer size in
 *                         bits.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the buffer size was successfully determined.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the data length is zero or cannot be handled.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the key location is unsupported.
 */
psa_status_t psa_get_key_buffer_size_from_key_data(
                const psa_key_attributes_t *attributes,
                const uint8_t *data, size_t data_length,
                size_t *key_buffer_size);

/**
 * @brief Retrieves and locks a key slot for a specific operation while
 *        enforcing its usage policy.
 *
 * @details This function locates the key slot corresponding to the given key
 *          identifier and locks it for exclusive access. It verifies that the
 *          requested usage and algorithm are permitted by the key's policy.
 *          Public keys bypass the export restriction. If any policy check
 *          fails, the slot is unlocked and an error is returned.
 *
 * @param key    Identifier of the key to retrieve.
 * @param p_slot On success, points to the locked key slot.
 * @param usage  Intended usage of the key (e.g., PSA_KEY_USAGE_ENCRYPT).
 * @param alg    Algorithm to be used with the key. If 0, algorithm check is
 *               skipped.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the key slot is successfully locked and the policy allows
 *   the requested usage and algorithm.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key does not exist.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key's policy does not allow the requested
 *   usage or algorithm.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if internal integrity checks fail.
 */
psa_status_t psa_get_and_lock_key_slot_with_policy(psa_key_id_t key,
                                                   psa_key_slot_t **p_slot,
                                                   psa_key_usage_t usage,
                                                   psa_algorithm_t alg);

/**
 * @brief Aborts key creation and cleans up the key slot.
 *
 * @details This function is called when a key creation or import operation
 *          fails. It wipes the key slot to ensure no partially-initialized
 *          or sensitive data remains. Calling this function with a NULL
 *          pointer is safe and has no effect.
 *
 * @param slot Pointer to the key slot to wipe. If NULL, the function does
 *             nothing.
 */
void psa_fail_key_creation(psa_key_slot_t *slot);

/** @} */  /* end of Key Management Functions */


/**
 * @defgroup PSA_UTILITY_FUNCTIONS PSA Utility Functions
 *
 * @brief General-purpose helper functions for secure memory handling
 *        and other common cryptographic utilities.
 *
 * @{
 */

/**
 * @brief Securely zeroizes a memory buffer.
 *
 * @details This function overwrites the memory region pointed to by buf
 *          with zeros using a volatile pointer to prevent the compiler from
 *          optimizing away the zeroing operation. This is important for
 *          securely clearing sensitive data such as key material from memory.
 *          If buf is NULL or len is 0, the function does nothing.
 *
 * @param buf  Pointer to the buffer to zeroize. If NULL, the function does
 *             nothing.
 * @param len  Length of the buffer in bytes. If 0, the function does nothing.
 */
void mg_zeroize(void *buf, size_t len);

/** @} */  /* end of PSA Utility Functions */

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PSA_INTERNAL_H_
