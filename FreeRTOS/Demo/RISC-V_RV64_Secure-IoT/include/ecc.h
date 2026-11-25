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
 * @file ecc.h
 * @brief Provides APIs for Elliptic Curve Cryptography (ECC) functions.
 * @details Contains APIs for software-based implementations of ECC cryptographic 
 *          primitives. It supports both ECDSA (Elliptic Curve Digital Signature
 *          Algorithm) and ECDH (Elliptic Curve Diffie-Hellman) across the 
 *          following standard NIST curves:  P-224, P-256, P-384, and P-521.
 * @version 1.0
 * @authors Natarajan S (natarajan@mindgrovetech.in)
 * @date 06-04-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 06-04-2026 | 1.0     | Natarajan S           | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef INCLUDES_ECC_H_
#define INCLUDES_ECC_H_

#include <stdint.h>
#include <stddef.h>
#include "crypto_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum ECC_Curve_ID_t
 * @brief ECC Curve identifiers.
 *
 * This enumeration defines the available NIST P-curves supported by the
 * cryptographic driver. These identifiers are used to configure key 
 * parameters and select the appropriate mathematical field operations.
 */
typedef enum {
    /** It represents the NIST P-224 curve. */
    ECC_SECP224R1 = 0U,
    /** It represents the NIST P-256 curve. */
    ECC_SECP256R1 = 1U,
    /** It represents the NIST P-384 curve. */
    ECC_SECP384R1 = 2U,
    /** It represents the NIST P-521 curve. */
    ECC_SECP521R1 = 3U,
} ECC_Curve_ID_t;

/**
 * @brief Opaque ECC key handle type.
 *
 * This type represents an ECC key container managed within the internal 
 * driver pool. The concrete structure fields are hidden from the user 
 * application to enforce encapsulation and data security.
 *
 * @note This type must only be used as a handle. 
 * Do not attempt to allocate or define objects of this type directly; 
 * use the driver's allocation API instead.
 */
typedef struct ecc_key ecc_key_t;

/**
 * @brief Allocates an available ECC key slot from the static internal pool.
 *
 * @details Searches the internal static pool for an unallocated key descriptor.
 * If found, it zeros out the memory slot, flags it as allocated, and
 * returns its handle to the caller.
 *
 * @return Pointer to an allocated, zeroed opaque ECC key handle on success,
 * or NULL if the pool is full.
 */
ecc_key_t* ECC_Key_Alloc(void);

/**
 * @brief Frees resources associated with an ECC key.
 *
 * @details Releases memory held by the given ECC key. Call this when the key is
 *          no longer required to free up the key slot.
 *
 * @param[in,out] key Pointer to the ECC key structure to free.
 *
 * @return
 * - @ref SUCCESS   : Key slot is freed
 * - @ref EFAULT    : Null pointer passed
 */
uint16_t ECC_Free_Key(ecc_key_t* key);

/**
 * @brief Generates a new ECC key pair for the selected curve.
 *
 * @details
 * Creates a private/public ECC key pair using the specified curve identifier.
 * The generated key can be used for ECDH shared secret generation, ECDSA
 * signing, or public key export depending on the selected curve support.
 *
 * @param[in]  curve_id ECC curve identifier. (see @ref ECC_Curve_ID_t).
 * @param[out] key      Pointer to the ECC key structure where the generated key
 *                      will be stored.
 *
 * @return
 * - @ref SUCCESS   : Key pair generated
 * - @ref EFAULT    : Null pointer passed
 * - @ref EINVAL    : Invalid curve id passed.
 * - @ref ETIME     : Timeout occurred while waiting for TRNG hardware.
 */
uint16_t ECC_Generate_Key_Pair(ECC_Curve_ID_t curve_id, ecc_key_t* key);

/**
 * @brief Computes an ECC shared secret using a private key and peer public key.
 *
 * @details
 * Performs ECDH shared secret generation using the local private key and the
 * peer public key. The output buffer must be large enough to store the shared
 * secret size for the selected curve.
 *
 * @param[in]  private_key Pointer to the local ECC private key.
 * @param[in]  public_key  Pointer to the peer ECC public key.
 * @param[out] out         Buffer to store the generated shared secret.
 *
 * @return
 * - @ref SUCCESS   : Shared Secred Computed.
 * - @ref EINVAL    : Invalid Key
 * - @ref EFAULT    : NULL pointer passed
 */
uint16_t ECDH_Shared_Secret(ecc_key_t* private_key,
                           ecc_key_t* public_key,
                           uint8_t* out);

/**
 * @brief Signs a message hash using an ECC private key.
 *
 * @details
 * Generates an ECDSA signature for the supplied hash using the given ECC
 * private key. The caller must provide an output buffer and its size through
 * outlen; after success, outlen contains the actual signature length.
 *
 * @param[in]     hash    Pointer to the input hash buffer.
 * @param[in]     hashlen Length of the hash in bytes.
 * @param[out]    out     Buffer to store the generated signature.
 * @param[in,out] outlen  On input, size of the output buffer in bytes.
 *                        On output, actual signature length in bytes.
 * @param[in]     key     Pointer to the ECC private key used for signing.
 *
 * @return
 * - @ref SUCCESS   : Signature Generated.
 * - @ref EFAULT    : NULL pointer passed.
 * - @ref EINVAL    : Invalid Key.
 * - @ref ETIME     : Timeout occurred while waiting for TRNG hardware.
 */
uint16_t ECC_Sign_Hash(const uint8_t* hash,
                       uint32_t hashlen,
                       uint8_t* out,
                       uint32_t* outlen,
                       ecc_key_t* key);

/**
 * @brief Verifies an ECC signature against a message hash.
 *
 * @details
 * Checks whether the given ECDSA signature matches the supplied hash using the
 * provided ECC public key. The verification operation result is written to res,
 * while the function return value indicates whether the operation itself ran.
 *
 * @param[in]  sig     Pointer to the input signature buffer.
 * @param[in]  hash    Pointer to the input hash buffer.
 * @param[in]  hashlen Length of the hash in bytes.
 * @param[out] res     Verification result.
 *                     Set to 1 if the signature is valid, 0 otherwise.
 * @param[in]  key     Pointer to the ECC public key used for verification.
 *
 * @return
 * - @ref SUCCESS   : Successful completion of verification operation
 * - @ref EFAULT    : Null pointer passed
 * - @ref EINVAL    : Invalid signature or Key
 */
uint16_t ECC_Verify_Hash(const uint8_t* sig,
                         const uint8_t* hash,
                         uint32_t hashlen,
                         uint8_t* res,
                         ecc_key_t* key);

/**
 * @brief Imports an ECC public key.
 *
 * @details
 * Loads an ECC public key from raw X and Y coordinate byte arrays for the
 * selected curve. The key_bytes argument must match the coordinate size of the
 * curve, for example 32 bytes for a 256-bit curve.
 *
 * @param[in]  curve_id  ECC curve identifier. (see @ref ECC_Curve_ID_t).
 * @param[out] key       Pointer to the ECC key structure where the public key
 *                       will be stored.
 * @param[in]  qx        Pointer to the raw public key X coordinate.
 * @param[in]  qy        Pointer to the raw public key Y coordinate.
 * @param[in]  key_bytes Size of each coordinate in bytes.
 *
 * @return
 * - @ref SUCCESS   : Public Key copied to internal struct
 * - @ref EINVAL    : Invalid Curve or Key.
 * - @ref EFAULT    : Null pointer passed.
 */
uint16_t ECC_Import_Public_Key(ECC_Curve_ID_t curve_id,
                               ecc_key_t* key,
                               const uint8_t* qx,
                               const uint8_t* qy,
                               uint32_t key_bytes);

/**
 * @brief Imports an ECC private key.
 *
 * @details
 * Loads an ECC private key from a raw private scalar byte buffer for the
 * selected curve. The private key length must be valid for the selected curve
 * and should normally match the curve size in bytes.
 *
 * @param[in]  curve_id   ECC curve identifier. (see @ref ECC_Curve_ID_t).
 * @param[out] key        Pointer to the ECC key structure where the private key
 *                        will be stored.
 * @param[in]  priv_bytes Pointer to the raw private key buffer.
 * @param[in]  priv_len   Length of the private key in bytes.
 *
 * @return
 * - @ref SUCCESS   : Private key copied to internal struct
 * - @ref EINVAL    : Invalid Curve ID or Key.
 * - @ref EFAULT    : Null pointer passed
 */
uint16_t ECC_Import_Private_Key(ECC_Curve_ID_t curve_id,
                                ecc_key_t* key,
                                const uint8_t* priv_bytes,
                                uint32_t priv_len);

/**
 * @brief Exports an ECC public key in raw format.
 *
 * @details
 * Writes the public key coordinates from the given ECC key into separate raw X
 * and Y coordinate buffers. The caller must provide qxLen and qyLen with the
 * available buffer sizes; these are updated with the actual exported lengths.
 *
 * @param[in]     key   Pointer to the ECC key containing the public key.
 * @param[out]    qx    Buffer to store the public key X coordinate.
 * @param[in,out] qxLen On input, size of the qx buffer in bytes.
 *                      On output, actual X coordinate length in bytes.
 * @param[out]    qy    Buffer to store the public key Y coordinate.
 * @param[in,out] qyLen On input, size of the qy buffer in bytes.
 *                      On output, actual Y coordinate length in bytes.
 *
 * @return
 * - @ref SUCCESS   : Public key x,y co-ordinates copied to passed pointers.
 * - @ref EINVAL    : Invalid key
 * - @ref EFAULT    : Null pointer passed.
 * - @ref ETOOSMALL : Passed buffer too small to export the key.
 */
uint16_t ECC_Export_Public_Key(ecc_key_t* key,
                               uint8_t* qx,
                               uint32_t* qxLen,
                               uint8_t* qy,
                               uint32_t* qyLen);

/**
 * @brief Signs a message hash using an ECC private key and a fixed scalar.
 *
 * @details Generates an ECDSA signature for a given hash using the provided 
 * private key and a mandatory, pre-determined ephemeral scalar (k). This is
 * primarily used for compliance verification against NIST KAT vectors.
 *
 * @param[in]     hash     Pointer to the input hash buffer.
 * @param[in]     hashlen  Length of the hash in bytes.
 * @param[out]    out      Buffer to store the generated signature.
 * @param[in,out] outlen   On input, size of the output buffer. 
 *                         On output, actual signature length in bytes.
 * @param[in]     key      Pointer to the ECC private key used for signing.
 * @param[in]     fixed_k  Pointer to the fixed ephemeral scalar buffer.
 *
 * @return
 * - @ref SUCCESS   : Signature Generated.
 * - @ref EFAULT    : NULL pointer passed.
 * - @ref EINVAL    : Invalid Key.
 */
uint16_t ECC_Sign_Hash_Fixed_K(const uint8_t* hash, uint32_t hashlen, \
        uint8_t* out, uint32_t *outlen, ecc_key_t* key, const uint8_t* fixed_k);

#ifdef __cplusplus
}
#endif

#endif   // INCLUDES_ECC_H_
