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
 * @file psa_types.h
 * @brief Header file for declaring the types used in PSA crypto APIs in Mindgrove 
 *        Silicon Secure-IoT SoC.
 * @details This is the header file that declares types that encode algorithms, 
 *          key types, policies, etc. 
 * @version 1.0
 * @authors Jennifer Vinita J (jennifer@mindgrovetech.in) 
 * @date 12-08-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-08-2025  | 1.0     | Jennifer Vinita J     | Initial release.  
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PSA_TYPES_H_
#define BSP_INCLUDE_PSA_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "psa_values.h"
#include "psa_enums.h"
#include "bignum.h"

/** 
 * @defgroup Key and Algorithm Types
 * @{
 */

/**
 * @brief Encoding of identifiers for persistent keys.
 *
 * @details
 * - Applications may choose key identifiers in the range #PSA_KEY_ID_USER_MIN 
 *   to #PSA_KEY_ID_USER_MAX.
 * - Implementations may define additional identifiers in the range 
 *   #PSA_KEY_ID_VENDOR_MIN to #PSA_KEY_ID_VENDOR_MAX.
 * - 0 is reserved as an invalid key identifier.
 * - Identifiers outside these ranges are reserved for future use.
 */
typedef uint32_t psa_key_id_t;

/**
 * @brief Encoding of a key type.
 *
 * @details Values are generally constructed by macros `PSA_KEY_TYPE_xxx`.
 *
 * @note Values are stored in the persistent key store. Changing them
 *       requires bumping the storage format version and providing translation.
 */
typedef uint16_t psa_key_type_t;

/**
 * @brief Type used internally for key sizes.
 *
 * @details Public interfaces use size_t, but internally a smaller type is used.
 */
typedef uint16_t psa_key_bits_t;

/**
 * @brief Encoding of key lifetimes.
 *
 * @details
 * - Bits 0-7 indicate persistence level (#PSA_KEY_LIFETIME_GET_PERSISTENCE).
 * - Bits 8-31 indicate the key location (#PSA_KEY_LIFETIME_GET_LOCATION).
 *
 * Volatile keys are destroyed automatically on application termination or power reset.
 * Persistent keys survive until explicitly destroyed or by integration-specific events.
 *
 * Values are constructed using `PSA_KEY_LIFETIME_xxx` macros.
 *
 * @note Encoded in persistent storage; changes require storage version bump.
 */
typedef uint32_t psa_key_lifetime_t;

/**
 * @brief Encoding of permitted usage on a key.
 *
 * @details Values are bitwise ORs of `PSA_KEY_USAGE_xxx` macros.
 *
 * @note Encoded in persistent storage; changes require storage version bump.
 */
typedef uint32_t psa_key_usage_t;

/**
 * @brief Encoding of a cryptographic algorithm.
 *
 * @details Values are constructed by `PSA_ALG_xxx` macros. 
 *          For multi-key algorithms, this type encodes the mode/padding, not the key type.
 *
 * @note Encoded in persistent storage; changes require storage version bump.
 */
typedef uint32_t psa_algorithm_t;

/**
 * @brief Mask of flags for key attributes.
 *
 * @details Also used internally to store flags in slots.
 *          Internal flags are defined in library/psa_crypto_core.h.
 */
typedef uint16_t psa_key_attributes_flag_t;

/**
 * @brief Encoding of key persistence levels.
 *
 * @details
 * - 0 = #PSA_KEY_PERSISTENCE_VOLATILE
 * - 1 = #PSA_KEY_PERSISTENCE_DEFAULT
 * - 255 = #PSA_KEY_PERSISTENCE_READ_ONLY
 *
 * @note Key management interfaces encode persistence as the lower 8 bits of a lifetime.
 */
typedef uint8_t psa_key_persistence_t;

/**
 * @brief Encoding of key location indicators.
 *
 * @details
 * - 0 = primary local storage (always available)
 * - 1 = primary secure element
 * - 2–0x7FFFFF = other PSA-defined locations
 * - 0x800000–0xFFFFFF = vendor-defined locations
 *
 * @note Encoded as upper 24 bits in ::psa_key_lifetime_t.
 */
typedef uint32_t psa_key_location_t;

/**
 * @brief Flags used when creating a data entry.
 */
typedef uint32_t psa_storage_create_flags_t;

/**
 * @brief Type for UIDs used for identifying stored data.
 */
typedef uint64_t psa_storage_uid_t;

/**
 * @brief Key policy structure.
 *
 * @details Specifies the usage and permitted algorithms for a key.
 *          Each key can have one primary algorithm (`alg`) and optionally
 *          a secondary algorithm (`alg2`), along with usage flags.
 */
struct psa_key_policy_s {
    /**
     * @brief Permitted operations on the key.
     *
     * @details Bitmask indicating what operations are allowed on the key,
     *          such as encrypt, decrypt, sign, verify, etc.
     */
    psa_key_usage_t usage;

    /**
     * @brief Primary permitted algorithm.
     *
     * @details The main cryptographic algorithm that the key is allowed to use.
     */
    psa_algorithm_t alg;

    /**
     * @brief Secondary permitted algorithm.
     *
     * @details Optional second algorithm permitted for the key. Some key types
     *          may support two algorithms (e.g., certain hybrid operations).
     */
    psa_algorithm_t alg2;
};
typedef struct psa_key_policy_s psa_key_policy_t;

/**
 * @brief Key attributes structure.
 *
 * @details Represents metadata for a key object, including type, size, lifetime,
 *          usage policy, and internal implementation flags.
 */
typedef struct {
    /**
     * @brief Key type.
     *
     * @details Specifies the type of the key (e.g., symmetric, RSA, ECC).
     */
    psa_key_type_t type;

    /**
     * @brief Key size in bits.
     *
     * @details Number of bits of the key material.
     */
    psa_key_bits_t bits;

    /**
     * @brief Lifetime of the key.
     *
     * @details Indicates whether the key is volatile or persistent and its
     *          storage location.
     */
    psa_key_lifetime_t lifetime;

    /**
     * @brief Key identifier.
     *
     * @details Unique identifier for the key. Only valid for persistent keys.
     */
    psa_key_id_t id;

    /**
     * @brief Key usage policy.
     *
     * @details Embedded `psa_key_policy_t` structure specifying allowed operations
     *          and permitted algorithms.
     */
    psa_key_policy_t policy;

    /**
     * @brief Internal flags.
     *
     * @details Implementation-specific flags used internally by the library.
     */
    psa_key_attributes_flag_t flags;
} psa_key_attributes_t;

/**
 * @brief Key slot structure.
 *
 * @details Represents a key stored in memory, including its attributes, state,
 *          and key material. Supports dynamic slices if enabled.
 */
typedef struct {
    /**
     * @brief Key attributes.
     *
     * @details Must be the first field. Contains all metadata about the key.
     */
    psa_key_attributes_t attr;

    /**
     * @brief Current state of the key slot.
     *
     * @details Indicates whether the slot is free, being filled, or occupied.
     */
    psa_key_slot_state_t state;

    /**
     * @brief Lock counter.
     *
     * @details Number of active library locks on the slot, used to prevent
     *          concurrent modifications.
     */
    size_t lock_count;

#if defined(MG_PSA_KEY_STORE_DYNAMIC)
    /**
     * @brief Slice index.
     *
     * @details Index of the dynamic slice containing this slot.
     */
    uint8_t slice_index;
#endif /* MG_PSA_KEY_STORE_DYNAMIC */

    /**
     * @brief Slot-specific union.
     *
     * @details Holds either free list information or reader count depending
     *          on the slot state.
     */
    union {
        struct {
            /**
             * @brief Next free slot relative to this one.
             *
             * @details Used for maintaining free slot lists within a slice.
             */
            int32_t next_free_relative_to_next;
        } free_slot;

        struct {
            /**
             * @brief Registered readers count.
             *
             * @details Number of library functions currently reading the slot.
             */
            size_t registered_readers;
        } occupied;
    } var;

    /**
     * @brief Key material buffer.
     *
     * @details Holds the actual bytes of the key along with the number of valid bytes.
     */
    struct key_data {
        uint8_t data[2048];  /**< Buffer for key material */
        size_t bytes;        /**< Number of valid bytes in the buffer */
    } key;
} psa_key_slot_t;

/**
 * @brief Key slot data container.
 *
 * @details Holds all key slots or slices and indicates whether
 *          they have been initialized. Supports both fixed and dynamic
 *          key slot allocation depending on configuration.
 */
typedef struct {
#if defined(MG_PSA_KEY_STORE_DYNAMIC)
    /**
     * @brief Array of pointers to dynamic slices.
     *
     * @details Each slice contains multiple key slots.
     */
    psa_key_slot_t *key_slices[KEY_SLICE_COUNT];

    /**
     * @brief First free slot index per slice.
     *
     * @details Tracks the first free slot for each volatile slice.
     */
    size_t first_free_slot_index[KEY_SLOT_VOLATILE_SLICE_COUNT];
#else
    /**
     * @brief Fixed array of key slots.
     *
     * @details Preallocated slots for storing keys.
     */
    psa_key_slot_t key_slots[MG_PSA_KEY_SLOT_COUNT];
#endif

    /**
     * @brief Key slot container.
     *
     * @details Holds all key slots used for storing key material.
     */
    psa_key_slot_t key_slots[MG_PSA_KEY_SLOT_COUNT];

    /**
     * @brief Key slots initialization flag.
     *
     * @details Indicates whether the key slot container has been initialized.
     */
    uint8_t key_slots_initialized : 1;
} psa_key_slot_data_t;

/**
 * @brief Persistent key storage format.
 *
 * @details Represents the serialized layout of a key in persistent storage.
 *          This structure defines the header and key data as stored in memory
 *          or on disk. All fields are stored in a fixed order to allow
 *          correct reading and writing by the library.
 */
typedef struct psa_persistent_key_storage_format_s {
    /**
     * @brief Magic header.
     *
     * @details Used to verify that the stored data is a valid key
     *          and that the storage format is correct.
     */
    uint8_t magic[PSA_KEY_STORAGE_MAGIC_HEADER_LENGTH];

    /**
     * @brief Storage format version.
     *
     * @details A 4-byte value indicating the version of the key storage
     *          format. Used to ensure compatibility when reading stored keys.
     */
    uint8_t version[4];

    /**
     * @brief Key lifetime.
     *
     * @details Encoded as sizeof(psa_key_lifetime_t) bytes.
     *          Indicates where the key is stored and its persistence
     *          properties (volatile or persistent).
     */
    uint8_t lifetime[sizeof(psa_key_lifetime_t)];

    /**
     * @brief Key type.
     *
     * @details Encoded as 2 bytes. Specifies the type of key, such as
     *          symmetric, RSA, ECC, etc.
     */
    uint8_t type[2];

    /**
     * @brief Key size in bits.
     *
     * @details Encoded as 2 bytes. Indicates the size of the key material
     *          in bits.
     */
    uint8_t bits[2];

    /**
     * @brief Key policy.
     *
     * @details Encoded as sizeof(psa_key_policy_t) bytes.
     *          Contains usage flags and permitted algorithms for the key.
     */
    uint8_t policy[sizeof(psa_key_policy_t)];

    /**
     * @brief Length of the key data.
     *
     * @details Encoded as 4 bytes. Indicates the size of the actual key material
     *          stored in the `key_data` field.
     */
    uint8_t data_len[4];

    /**
     * @brief Key material.
     *
     * @details A flexible array containing the serialized key bytes.
     *          The actual size is specified by the `data_len` field.
     */
    uint8_t key_data[PSA_MAX_KEY_DATA_SIZE];
} psa_persistent_key_storage_format;

/**
 * @brief Container for metadata associated with a specific UID.
 *
 * @details This structure stores the size and flags of a UID in
 *          persistent storage. The size indicates the number of bytes
 *          associated with the UID, and flags represent creation properties.
 */
typedef struct psa_storage_info_t {
    /**
     * @brief Size of the data associated with the UID.
     *
     * @details Specifies the length of the data in bytes.
     */
    uint32_t size;

    /**
     * @brief Flags set when the UID was created.
     *
     * @details Can indicate properties such as read-only, write-once, or other
     *          implementation-specific creation flags.
     */
    psa_storage_create_flags_t flags;
} psa_storage_info_t;

/**@}*/


/** 
 * @defgroup Cipher Types
 * @{
 */

/**
 * @brief Generic cipher context structure.
 *
 * @details Stores the complete state of an ongoing AES cipher operation,
 *          including cipher mode, key material, IV, unprocessed data buffer,
 *          and key ownership state. Used for both single-part and multi-part
 *          cipher operations. No heap allocation is required.
 */
typedef struct mg_cipher_context_t {
    /**
     * @brief Cipher mode for the current operation.
     *
     * @details Specifies the AES mode of operation. Valid values are:
     *          - MG_MODE_CBC : Cipher Block Chaining
     *          - MG_MODE_CTR : Counter mode
     *          - MG_MODE_CFB : Cipher Feedback mode
     *          - MG_MODE_OFB : Output Feedback mode
     *          Set during cipher setup via psa_cipher_setup() and
     *          used in AES_Config_t during psa_cipher_update().
     */
    mg_cipher_mode_t mode;

    /**
     * @brief Key length in bits.
     *
     * @details Valid values are 128, 192, or 256 bits for AES.
     *          Set during key configuration via mg_cipher_setkey()
     *          and used in AES_Config_t during psa_cipher_update().
     */
    size_t key_bitlen;

    /**
     * @brief Key data buffer.
     *
     * @details Holds the raw AES key material copied from the key slot.
     *          Size is determined by PSA_MG_MAX_AES_KEY_SIZE which
     *          accommodates up to AES-256 keys. Set via mg_cipher_setkey()
     *          and used in AES_Config_t during psa_cipher_update().
     */
    uint8_t key[PSA_MG_MAX_AES_KEY_SIZE];

    /**
     * @brief Direction of the cipher operation.
     *
     * @details Specifies whether this context is configured for
     *          encryption (MG_PSA_AES_ENCRYPT) or
     *          decryption (MG_PSA_AES_DECRYPT).
     *          Set during cipher setup via psa_cipher_setup() and
     *          used in AES_Config_t during psa_cipher_update().
     */
    mg_operation_t operation;

    /**
     * @brief Number of bytes currently stored in unprocessed_data.
     *
     * @details Tracks how many bytes of a partial block are pending
     *          processing. Must be zero at the start of each
     *          psa_cipher_update() call. Reset to zero after
     *          successful processing.
     */
    size_t unprocessed_len;

    /**
     * @brief Total number of bytes successfully processed so far.
     *
     * @details Tracks cumulative input processed across multiple
     *          psa_cipher_update() calls. Used to compute the
     *          iterated_length_bits field in AES_Config_t for
     *          chained block operations.
     */
    size_t iterated_length;

    /**
     * @brief Initialization vector (IV) for the cipher operation.
     *
     * @details Holds the IV provided by the caller via psa_cipher_set_iv().
     *          Must be set before the first call to psa_cipher_update().
     *          Size is PSA_MG_AES_IV_SIZE bytes. The AES hardware uses
     *          this IV internally based on the configured cipher mode.
     */
    uint8_t iv[PSA_MG_AES_IV_SIZE];

    /**
     * @brief Ownership state of the key material.
     *
     * @details Tracks whether the AES key has been configured in this
     *          context. Valid values:
     *          - MG_CIPHER_PSA_KEY_UNSET : no key configured yet
     *          - MG_CIPHER_PSA_KEY_OWNED : key successfully copied
     *          Checked in mg_cipher_setkey() to prevent duplicate
     *          key assignment on the same cipher context.
     */
    mg_cipher_psa_key_ownership slot_state;
} mg_cipher_context_t;


/**
 * @brief PSA cipher operation structure.
 *
 * @details Represents the complete state of a multi-part PSA cipher
 *          operation. Must be initialized by calling psa_cipher_setup()
 *          before use. Operations must be finalized with
 *          psa_cipher_finish() or abandoned with psa_cipher_abort().
 */
typedef struct {
    /**
     * @brief Algorithm used for this cipher operation.
     *
     * @details Stores the PSA algorithm identifier (e.g.
     *          PSA_ALG_CBC_NO_PADDING, PSA_ALG_CTR) set during
     *          psa_cipher_setup(). Used to identify the active
     *          algorithm throughout the operation lifecycle.
     */
    psa_algorithm_t alg;

    /**
     * @brief Internal cipher context holding full operation state.
     *
     * @details Contains key material, IV, mode, unprocessed data buffer,
     *          and all other state required to execute and resume
     *          the cipher operation across multiple update calls.
     */
    mg_cipher_context_t cipher;

    /**
     * @brief Flag indicating whether the IV has been set.
     *
     * @details Set to 1U by psa_cipher_set_iv() after a valid IV is
     *          provided. Set to 0U during psa_cipher_setup().
     *          psa_cipher_update() will return PSA_ERROR_BAD_STATE
     *          if this flag is not set before calling it.
     */
    uint8_t iv_set;
} psa_cipher_operation_t;

/**@}*/


/** 
 * @defgroup Hash Types
 * @{
 */

/**
 * @brief SHA-256 context structure.
 *
 * @details This structure holds the intermediate state required to perform
 *          SHA-256 hash computations. The hardware driver maintains the
 *          internal hash state in hardware registers.
 */
typedef struct mg_sha256_context {
    /**
     * @brief Number of bytes processed so far.
     *
     * @details Keeps track of the total input length processed across
     *          all chunks for computing the final hash value.
     */
    uint64_t iterated_length;
} mg_sha256_context;

/**
 * @brief PSA hash operation structure.
 *
 * @details This structure represents an ongoing or completed hash operation
 *          in the PSA Crypto API. It encapsulates:
 *          - A unique operation identifier
 *          - The algorithm being used
 *          - The internal SHA-256 context
 */
typedef struct {
    /**
     * @brief Unique identifier for the hash operation.
     *
     * @details Set to 1 when the operation is active, and cleared to 0
     *          when the operation is aborted or completed.
     */
    uint32_t id;

    /**
     * @brief Algorithm used for this hash operation.
     *
     * @details Set to PSA_ALG_SHA_256 during setup.
     */
    psa_algorithm_t alg;

    /**
     * @brief Internal SHA-256 context.
     *
     * @details Stores intermediate state for the hash computation.
     */
    mg_sha256_context sha256_ctx;
} psa_hash_operation_t;

/**@}*/


/** 
 * @defgroup RSA Types
 * @{
 */

/**
 * @brief RSA context structure.
 *
 * @details This structure holds all parameters required for an RSA key pair,
 *          including the public modulus, public exponent, and private exponent.
 *          It is used for RSA encryption, decryption, signing, and verification
 *          operations.
 */
typedef struct rsa_context {
    /**
     * @brief Size of the modulus N in bytes.
     *
     * @note Determines the key strength (e.g., 256 bytes for a 2048-bit key).
     */
    size_t len;

    /**
     * @brief Public modulus (N) of the RSA key.
     *
     * @note Used in both encryption and signature verification.
     */
    bn_int N;

    /**
     * @brief Public exponent (E) of the RSA key.
     */
    bn_int E;

    /**
     * @brief Private exponent (D) of the RSA key.
     */
    bn_int D;
} mg_rsa_context;

/**@}*/

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PSA_TYPES_H_
