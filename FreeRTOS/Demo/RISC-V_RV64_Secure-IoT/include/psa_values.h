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
 * @file psa_values.h
 * @brief PSA Crypto API macros and constants for Secure-IOT Processor.
 * @details This header file defines macros and constants used throughout the 
 *          PSA Crypto subsystem on Secure-IOT devices, including PSA Crypto API 
 *          version macros, key and algorithm type macros, key slot management 
 *          macros, RSA and AES cipher-specific values.

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
 * 02-02-2026 | 1.1     | Dayana Devi K         | Added Base value for OAEP
 *            |         |                       | asymmetric encryption padding.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PSA_VALUES_H_
#define BSP_INCLUDE_PSA_VALUES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "crypto_config.h"

/**
 * @defgroup PSA_VERSION_MACROS PSA Crypto API Version Macros
 *
 * @brief Defines the major and minor version of this PSA Crypto API
 *        implementation.
 *
 * @{
 */

/** @brief The major version of this implementation of the PSA Crypto API. */
#define PSA_CRYPTO_API_VERSION_MAJOR 1

/** @brief The minor version of this implementation of the PSA Crypto API. */
#define PSA_CRYPTO_API_VERSION_MINOR 0

/** @} */


/**
 * @defgroup PSA_KEY_ALG_MACROS PSA Key and Algorithm Macros
 *
 * @brief Constants for key slots, key types, and algorithms.
 *
 * @{
 */

/** @brief Maximum number of key slots supported in this implementation. */
#define MG_PSA_KEY_SLOT_COUNT 8U

/** @brief Hexadecimal radix used for key representation. */
#define MG_PSA_HEX_RADIX 16

/** @brief Represents an invalid or no algorithm. */
#define PSA_ALG_NONE ((psa_algorithm_t)0)

/** @brief Vendor-defined algorithm flag.
 *
 *  Standard algorithms will never have this bit set. Vendors defining new
 *  algorithms must set this bit.
 */
#define PSA_ALG_VENDOR_FLAG ((psa_algorithm_t)0x80000000)

/** @brief Algorithm category mask. */
#define PSA_ALG_CATEGORY_MASK ((psa_algorithm_t)0x7f000000)

/** @brief Algorithm categories. */
#define PSA_ALG_CATEGORY_HASH                   ((psa_algorithm_t)0x02000000)
#define PSA_ALG_CATEGORY_MAC                    ((psa_algorithm_t)0x03000000)
#define PSA_ALG_CATEGORY_CIPHER                 ((psa_algorithm_t)0x04000000)
#define PSA_ALG_CATEGORY_AEAD                   ((psa_algorithm_t)0x05000000)
#define PSA_ALG_CATEGORY_SIGN                   ((psa_algorithm_t)0x06000000)
#define PSA_ALG_CATEGORY_ASYMMETRIC_ENCRYPTION  ((psa_algorithm_t)0x07000000)
#define PSA_ALG_CATEGORY_KEY_DERIVATION         ((psa_algorithm_t)0x08000000)
#define PSA_ALG_CATEGORY_KEY_AGREEMENT          ((psa_algorithm_t)0x09000000)

/** @brief SHA-256 hash algorithm. */
#define PSA_ALG_SHA_256 ((psa_algorithm_t)0x02000009)

/** @brief CTR stream cipher mode. */
#define PSA_ALG_CTR ((psa_algorithm_t)0x04c01000)

/** @brief CFB stream cipher mode. */
#define PSA_ALG_CFB ((psa_algorithm_t)0x04c01100)

/** @brief OFB stream cipher mode. */
#define PSA_ALG_OFB ((psa_algorithm_t)0x04c01200)

/** @brief CBC block cipher mode, no padding. */
#define PSA_ALG_CBC_NO_PADDING ((psa_algorithm_t)0x04404000)

/** @brief Wildcard MAC length flag. */
#define PSA_ALG_MAC_AT_LEAST_THIS_LENGTH_FLAG ((psa_algorithm_t)0x00008000)

/** @brief Wildcard AEAD tag length flag. */
#define PSA_ALG_AEAD_AT_LEAST_THIS_LENGTH_FLAG ((psa_algorithm_t)0x00008000)

/** @brief Allow any hash algorithm in hash-and-sign policies. */
#define PSA_ALG_ANY_HASH ((psa_algorithm_t)0x020000ff)

/** @brief Mask for hash algorithm bits. */
#define PSA_ALG_HASH_MASK ((psa_algorithm_t)0x000000ff)

/** @brief Deterministic ECDSA flag. */
#define PSA_ALG_ECDSA_DETERMINISTIC_FLAG ((psa_algorithm_t)0x00000100)

/** @brief Base algorithms for signing. */
#define PSA_ALG_RSA_PKCS1V15_SIGN_BASE ((psa_algorithm_t)0x06000200U)
#define PSA_ALG_RSA_PSS_BASE           ((psa_algorithm_t)0x06000300U)
#define PSA_ALG_ECDSA_BASE             ((psa_algorithm_t)0x06000600U)
#define PSA_ALG_HASH_EDDSA_BASE        ((psa_algorithm_t)0x06000900U)
#define PSA_ALG_RSA_PSS_ANY_SALT_BASE  ((psa_algorithm_t)0x06001300U)

/** @brief Base algorithms for asymmetric encryption. */
#define PSA_ALG_RSA_PKCS1V15_CRYPT ((psa_algorithm_t)0x07000200U)
#define PSA_ALG_RSA_OAEP_SHA256    ((psa_algorithm_t)0x07000300U)

/** @brief Raw PKCS#1 v1.5 signature. */
#define PSA_ALG_RSA_PKCS1V15_SIGN_RAW PSA_ALG_RSA_PKCS1V15_SIGN_BASE

/** @brief RSA PSS base algorithm. */
#define PSA_ALG_RSA_PSS_BASE_ALG    PSA_ALG_RSA_PSS_BASE

/** @brief Invalid key type. */
#define PSA_KEY_TYPE_NONE ((psa_key_type_t)0x0000U)

/** @brief Key type categories. */
#define PSA_KEY_TYPE_CATEGORY_MASK       ((psa_key_type_t)0x7000U)
#define PSA_KEY_TYPE_CATEGORY_RAW        ((psa_key_type_t)0x1000U)
#define PSA_KEY_TYPE_CATEGORY_SYMMETRIC  ((psa_key_type_t)0x2000U)
#define PSA_KEY_TYPE_CATEGORY_PUBLIC_KEY ((psa_key_type_t)0x4000U)
#define PSA_KEY_TYPE_CATEGORY_KEY_PAIR   ((psa_key_type_t)0x7000U)
#define PSA_KEY_TYPE_CATEGORY_FLAG_PAIR  ((psa_key_type_t)0x3000U)

/** @brief Raw data key (cannot be used for crypto operations). */
#define PSA_KEY_TYPE_RAW_DATA ((psa_key_type_t)0x1001U)

/** @brief HMAC key. */
#define PSA_KEY_TYPE_HMAC ((psa_key_type_t)0x1100U)

/** @brief High-entropy secret for key derivation. */
#define PSA_KEY_TYPE_DERIVE ((psa_key_type_t)0x1200U)

/** @brief Low-entropy password or key derivation secret. */
#define PSA_KEY_TYPE_PASSWORD ((psa_key_type_t)0x1203U)

/** @brief Secret for verifying a password hash. */
#define PSA_KEY_TYPE_PASSWORD_HASH ((psa_key_type_t)0x1205U)

/** @brief Secret used in computing a password hash (pepper). */
#define PSA_KEY_TYPE_PEPPER ((psa_key_type_t)0x1206U)

/** @brief AES key for cipher, AEAD, or MAC algorithms. */
#define PSA_KEY_TYPE_AES ((psa_key_type_t)0x2400U)

/** @brief RSA public key. */
#define PSA_KEY_TYPE_RSA_PUBLIC_KEY ((psa_key_type_t)0x4001U)

/** @brief RSA key pair (private + public key). */
#define PSA_KEY_TYPE_RSA_KEY_PAIR ((psa_key_type_t)0x7001U)

/** @} */


/**
 * @defgroup PSA_KEY_SLOT_MACROS Key Slot Management Macros
 *
 * @brief Macros and constants used for key attributes, key identifiers,
 *        lifetimes, persistence levels, key usage flags, and storage.
 *
 * @{
 */

/** @brief Returns a suitable initializer for a key attribute object of
 *         type psa_key_attributes_t. */
#define PSA_KEY_ATTRIBUTES_INIT { 0, 0, 0, 0, 0, 0, 0, 0 }

/** @brief Initial key identifier value. */
#define PSA_KEY_ID_INIT ((psa_key_id_t)0)

/** @brief The null key identifier. */
#define PSA_KEY_ID_NULL ((psa_key_id_t)0)

/** @brief Minimum key identifier chosen by the application. */
#define PSA_KEY_ID_USER_MIN ((psa_key_id_t)0x00000001)

/** @brief Maximum key identifier chosen by the application. */
#define PSA_KEY_ID_USER_MAX ((psa_key_id_t)0x3fffffff)

/** @brief Minimum key identifier chosen by the implementation. */
#define PSA_KEY_ID_VENDOR_MIN ((psa_key_id_t)0x40000000)

/** @brief Maximum key identifier chosen by the implementation. */
#define PSA_KEY_ID_VENDOR_MAX ((psa_key_id_t)0x7fffffff)

/** @brief Minimum built-in key identifier. */
#define MG_PSA_KEY_ID_BUILTIN_MIN ((psa_key_id_t)0x7fff0000)

/** @brief Maximum built-in key identifier. */
#define MG_PSA_KEY_ID_BUILTIN_MAX ((psa_key_id_t)0x7fffefff)

/** @brief Minimum volatile key identifier. */
#define PSA_KEY_ID_VOLATILE_MIN PSA_KEY_ID_VENDOR_MIN

/** @brief Maximum volatile key identifier. */
#define PSA_KEY_ID_VOLATILE_MAX ((psa_key_id_t)(MG_PSA_KEY_ID_BUILTIN_MIN - \
                                 (psa_key_id_t)1U))

/** @brief Base value for volatile key IDs.
 *
 *  @details Marks bit 30 to distinguish volatile keys from persistent keys
 *           in the PSA key ID space. Used when constructing a volatile key ID
 *           from slice and slot indices.
 */
#define PSA_VOLATILE_KEY_ID_BASE    ((psa_key_id_t)0x40000000U)

/** @brief Mask to extract slot index bits from a volatile key ID. */
#define MG_KEY_ID_SLOT_MASK \
    (((psa_key_id_t)1U << KEY_ID_SLOT_INDEX_WIDTH) - 1U)

/** @brief Mask to extract slice index bits from a volatile key ID after
 *         shifting right by KEY_ID_SLOT_INDEX_WIDTH. */
#define MG_KEY_ID_SLICE_MASK \
    (((psa_key_id_t)1U << KEY_ID_SLICE_INDEX_WIDTH) - 1U)

/** @brief Default lifetime for volatile keys. */
#define PSA_KEY_LIFETIME_VOLATILE ((psa_key_lifetime_t)0x00000000)

/** @brief Default lifetime for persistent keys. */
#define PSA_KEY_LIFETIME_PERSISTENT ((psa_key_lifetime_t)0x00000001)

/** @brief Persistence level for volatile keys. */
#define PSA_KEY_PERSISTENCE_VOLATILE ((psa_key_persistence_t)0x00U)

/** @brief Default persistence level for persistent keys. */
#define PSA_KEY_PERSISTENCE_DEFAULT ((psa_key_persistence_t)0x01)

/** @brief Persistence level indicating a key is read-only and never
 *         destroyed. */
#define PSA_KEY_PERSISTENCE_READ_ONLY ((psa_key_persistence_t)0xff)

/** @brief Local storage area for persistent keys. */
#define PSA_KEY_LOCATION_LOCAL_STORAGE ((psa_key_location_t)0x000000)

/** @brief Test driver key location. */
#define PSA_CRYPTO_TEST_DRIVER ((psa_key_location_t)0x000001)

/** @brief Key usage flags. */
#define PSA_KEY_USAGE_EXPORT            ((psa_key_usage_t)0x00000001)
#define PSA_KEY_USAGE_COPY              ((psa_key_usage_t)0x00000002)
#define PSA_KEY_USAGE_ENCRYPT           ((psa_key_usage_t)0x00000100)
#define PSA_KEY_USAGE_DECRYPT           ((psa_key_usage_t)0x00000200)
#define PSA_KEY_USAGE_SIGN_MESSAGE      ((psa_key_usage_t)0x00000400)
#define PSA_KEY_USAGE_VERIFY_MESSAGE    ((psa_key_usage_t)0x00000800)
#define PSA_KEY_USAGE_SIGN_HASH         ((psa_key_usage_t)0x00001000)
#define PSA_KEY_USAGE_VERIFY_HASH       ((psa_key_usage_t)0x00002000)
#define PSA_KEY_USAGE_DERIVE            ((psa_key_usage_t)0x00004000)
#define PSA_KEY_USAGE_VERIFY_DERIVATION ((psa_key_usage_t)0x00008000)

/** @brief Maximum invalid key size (used to mark errors). */
#define PSA_KEY_BITS_TOO_LARGE ((psa_key_bits_t) -1)

/** @brief Maximum key size in bits (rounded down to whole bytes). */
#define PSA_MAX_KEY_BITS 0xfff8U

/** @brief Maximum key size in bytes. */
#define PSA_MAX_KEY_DATA_SIZE 256U

/** @brief Maximum storage size for keys in bytes. */
#define PSA_CRYPTO_MAX_STORAGE_SIZE (PSA_BITS_TO_BYTES(PSA_MAX_KEY_BITS))

/** @brief Persistent key storage magic header. */
#define PSA_KEY_STORAGE_MAGIC_HEADER "PSA\0KEY"
#define PSA_KEY_STORAGE_MAGIC_HEADER_LENGTH \
        (sizeof(PSA_KEY_STORAGE_MAGIC_HEADER))

/** @brief ITS storage filename and pattern constants. */
#define PSA_ITS_STORAGE_PREFIX ""
#define PSA_ITS_STORAGE_FILENAME_PATTERN "%08x%08x"
#define PSA_ITS_STORAGE_SUFFIX ".psa_its"
#define PSA_ITS_STORAGE_FILENAME_LENGTH \
    (sizeof(PSA_ITS_STORAGE_PREFIX) - 1 + 16 + sizeof(PSA_ITS_STORAGE_SUFFIX) \
     - 1 + 1)
#define PSA_ITS_STORAGE_TEMP \
    PSA_ITS_STORAGE_PREFIX "tempfile" PSA_ITS_STORAGE_SUFFIX

/** @brief Maximum size of a PSA ITS object. */
#define PSA_ITS_MAX_SIZE 0xffffffff

/** @brief PSA ITS magic string and length. */
#define PSA_ITS_MAGIC_STRING "PSA\0ITS\0"
#define PSA_ITS_MAGIC_LENGTH 8

#if defined(MG_PSA_KEY_STORE_DYNAMIC)

/** @brief Number of bits used to encode the slot index within a key ID. */
#define KEY_ID_SLOT_INDEX_WIDTH     25U

/** @brief Number of bits used to encode the slice index within a key ID. */
#define KEY_ID_SLICE_INDEX_WIDTH    5U

/** @brief Base number of slots in the first volatile slice (slice 0).
 *         Each subsequent slice doubles in size. */
#define KEY_SLOT_VOLATILE_SLICE_BASE_LENGTH 16U

/** @brief Total number of volatile slices. */
#define KEY_SLOT_VOLATILE_SLICE_COUNT 22U

/** @brief Total number of slices including the cache slice. */
#define KEY_SLICE_COUNT (KEY_SLOT_VOLATILE_SLICE_COUNT + 1U)

/** @brief Index of the persistent key cache slice. */
#define KEY_SLOT_CACHE_SLICE_INDEX  KEY_SLOT_VOLATILE_SLICE_COUNT

/** @brief Number of persistent key slots in the cache slice. */
#define PERSISTENT_KEY_CACHE_COUNT  MG_PSA_KEY_SLOT_COUNT

/** @brief Special value indicating no free slot is available in a slice. */
#define FREE_SLOT_INDEX_NONE        ((size_t) -1)

/** 
 * @brief Maximum key material size per key slot in bytes.
 * @details Sized to support up to 2048-bit RSA keys (256 bytes).
 */
#define MG_PSA_STATIC_KEY_SLOT_BUFFER_SIZE 256U

#else /* MG_PSA_KEY_STORE_DYNAMIC */

/** @brief Number of persistent key slots (static store). */
#define PERSISTENT_KEY_CACHE_COUNT  MG_PSA_KEY_SLOT_COUNT

/** @brief Total number of slices in the static store. */
#define KEY_SLICE_COUNT             1U

/** @brief Index of the sole key slice in the static store. */
#define KEY_SLOT_CACHE_SLICE_INDEX  0

#endif /* MG_PSA_KEY_STORE_DYNAMIC */

/** @} */


/**
 * @defgroup PSA_AES_MACROS AES Cipher Values
 *
 * @brief Macros defining maximum sizes and initialization values for AES
 *        cipher operations.
 *
 * @{
 */

/** @brief Maximum block size of any supported AES cipher, in bytes.
 *         AES always operates on 128-bit (16-byte) blocks regardless of key
 *         size. */
#define PSA_MG_AES_BLOCK_LENGTH     16U

/** @brief Maximum AES key size supported, in bytes.
 *         Supported key sizes are 16 bytes (AES-128), 24 bytes (AES-192),
 *         and 32 bytes (AES-256). */
#define PSA_MG_MAX_AES_KEY_SIZE     32U

/** @brief IV size used for all supported AES cipher algorithms
 *         (CBC, CTR, CFB, OFB), in bytes. */
#define PSA_MG_AES_IV_SIZE          16U

/** @brief Valid AES key sizes in bits. */
#define PSA_MG_AES_KEY_SIZE_128_BITS     128U
#define PSA_MG_AES_KEY_SIZE_192_BITS     192U
#define PSA_MG_AES_KEY_SIZE_256_BITS     256U

/** @brief Valid AES key sizes in bytes. */
#define PSA_MG_AES_KEY_SIZE_128          16U
#define PSA_MG_AES_KEY_SIZE_192          24U
#define PSA_MG_AES_KEY_SIZE_256          32U

/** @brief Initializer for a cipher operation structure. Must be used to
 *         initialize a psa_cipher_operation_t object before use. */
#define PSA_CIPHER_OPERATION_INIT { 0, { 0 }, 0 }

/** @} */


/**
 * @defgroup PSA_SHA256_MACROS SHA256 Hash Values
 *
 * @brief Macros for SHA256 hash output size, maximum input length, and
 *        initialization.
 *
 * @{
 */

/** @brief Initializer for a hash operation structure. */
#define PSA_HASH_OPERATION_INIT     { 0U, 0U, { 0U } }

/** @brief SHA256 output length in bits. */
#define PSA_MG_SHA256_OUTPUT_LEN_BITS   256U

/** @brief SHA256 output length in bytes. */
#define PSA_MG_SHA256_OUTPUT_LEN    (PSA_MG_SHA256_OUTPUT_LEN_BITS / 8U)

/** @brief Maximum input length for SHA256 in bits. */
#define PSA_MG_SHA256_MAX_INPUT_LEN_BITS 512U

/** @brief Maximum input length for SHA256 in bytes. */
#define PSA_MG_SHA256_MAX_INPUT_LEN (PSA_MG_SHA256_MAX_INPUT_LEN_BITS / 8U)

/** @brief Maximum hash output size in bytes (generic). */
#define PSA_MG_HASH_MAX_SIZE        32U

/** @} */


/**
 * @defgroup PSA_RSA_MACROS RSA Specific Values
 *
 * @brief Macros for RSA key sizes, PKCS versions, operation identifiers,
 *        and ASN.1 constants.
 *
 * @{
 */

/** @brief Maximum RSA key size in bytes (implementation-specific). */
#define PSA_MG_RSA_MAX_SIZE               2048U

/** @brief Size of a 2048-bit RSA key in bytes. */
#define PSA_MG_RSA_256_BYTES              256U

/** @brief RSA block size in bytes. */
#define PSA_MG_RSA_BLOCK_SIZE             256U

/** @brief RSA signature size in bytes.*/
#define PSA_MG_RSA_SIGNATURE_SIZE         256U

/** @brief RSA modulus size in bytes. */
#define PSA_MG_RSA_MODULUS_SIZE           256U

/** @brief RSA signature size in bytes. */
#define PSA_MG_RSA_SIGNATURE_SIZE         256U

/** @brief RSA private exponent size in bytes. */
#define PSA_MG_RSA_PRIVATE_EXPONENT_SIZE  256U

/** @brief Minimum valid RSA public exponent. */
#define PSA_MG_RSA_MIN_PUBLIC_EXPONENT    3U

/** @brief Maximum valid RSA public exponent. */
#define PSA_MG_RSA_MAX_PUBLIC_EXPONENT    256U


/** @brief ASN.1 primitive and constructed type identifiers. */
#define PSA_MG_ASN1_INTEGER_TAG         0x02U
#define PSA_MG_ASN1_SEQUENCE_TAG        0x30U
#define PSA_MG_ASN1_LONG_FORM_FLAG      0x80U
#define PSA_MG_ASN1_LENGTH_MASK         0x7FU
#define PSA_MG_ASN1_MAX_LENGTH_BYTES    4U
#define PSA_MG_ASN1_BYTE_SHIFT          8U

/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* BSP_INCLUDE_PSA_VALUES_H_ */
