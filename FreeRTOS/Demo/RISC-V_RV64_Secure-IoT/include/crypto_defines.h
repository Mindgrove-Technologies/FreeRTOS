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
 * @file crypto_defines.h
 * @brief Cryptographic constant definitions for AES, RSA, and SHA-256.
 * @details This header file defines macros and constants used across
 *          cryptographic modules including AES, RSA, and SHA-256.
 * @version 1.2
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Jennifer Vinita J (jennifer@mindgrovetech.in)
 *          Dayana Devi K (dayana@mindgrovetech.in)
 * @date 07-04-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 12-11-2024 | 1.0     | Vishwajith N S        | Initial release.
 * 11-12-2025 | 1.1     | Jennifer Vinita J     | Added crypto accelerator
 *            |         |                       | specific constants.
 * 07-04-2026 | 1.2     | Dayana Devi K         | Removed error messages,
 *            |         |                       | endian enums and added 
 *            |         |                       | RSA constants and MISRA
 *            |         |                       | compliance updates.
 * -----------------------------------------------------------------------------
 */

#ifndef  BSP_INCLUDE_CRYPTO_DEFINES_H_
#define  BSP_INCLUDE_CRYPTO_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup AES_Macros AES Macros
 * @brief AES size and configuration macros.
 *
 * These macros define constants related to AES such as block size and key sizes.
 *
 * @{
 */

/**
 * @def AES_BLOCK_SIZE
 * @brief AES block size in bytes.
 */
#define AES_BLOCK_SIZE        16U

/**
 * @def AES_BLOCK_SIZE_BITS
 * @brief AES block size in bits.
 */
#define AES_BLOCK_SIZE_BITS   128U

/**
 * @def AES128_KEY_SIZE
 * @brief AES-128 key size in bytes.
 */
#define AES128_KEY_SIZE       16U

/**
 * @def AES192_KEY_SIZE
 * @brief AES-192 key size in bytes.
 */
#define AES192_KEY_SIZE       24U

/**
 * @def AES256_KEY_SIZE
 * @brief AES-256 key size in bytes.
 */
#define AES256_KEY_SIZE       32U

/**
 * @def AES_IV_SIZE
 * @brief AES Initialization Vector size in bytes.
 *
 * AES uses a fixed block size of 128 bits, so IV is always 16 bytes.
 */
#define AES_IV_SIZE           16U

/** @} */

/**
 * @defgroup RSA_Macros RSA Macros
 * @brief RSA-2048 size and configuration macros.
 *
 * These macros define constants related to RSA operations including
 * key sizes, modulus, exponents, and signature sizes.
 *
 * @{
 */

/**
 * @def RSA_MODULUS_SIZE
 * @brief RSA modulus size in bytes.
 */
#define RSA_MODULUS_SIZE               256U

/**
 * @def RSA_MODULUS_SIZE_BITS
 * @brief RSA modulus size in bits.
 */
#define RSA_MODULUS_SIZE_BITS          2048U

/**
 * @def RSA_SIGNATURE_SIZE
 * @brief RSA signature size in bytes.
 */
#define RSA_SIGNATURE_SIZE             256U

/**
 * @def RSA_SIGNATURE_SIZE_BITS
 * @brief RSA signature size in bits.
 */
#define RSA_SIGNATURE_SIZE_BITS        2048U

/**
 * @def RSA_PUBLIC_EXPONENT_MAX_SIZE
 * @brief Maximum public exponent size in bytes.
 *
 * Exponent is typically small (e.g., 65537),
 * but may be padded to match modulus size.
 */
#define RSA_PUBLIC_EXPONENT_MAX_SIZE   256U

/**
 * @def RSA_PUBLIC_EXPONENT_DEFAULT
 * @brief Default RSA public exponent value (65537).
 */
#define RSA_PUBLIC_EXPONENT_DEFAULT    0x10001

/**
 * @def RSA_PRIVATE_EXPONENT_SIZE
 * @brief RSA private exponent size in bytes.
 */
#define RSA_PRIVATE_EXPONENT_SIZE      256U

/**
 * @def RSA_BLOCK_SIZE
 * @brief RSA block size in bytes.
 *
 * RSA operates on blocks equal to the modulus size.
 */
#define RSA_BLOCK_SIZE                 256U

/**
 * @def RSA_BLOCK_SIZE_BITS
 * @brief RSA block size in bits.
 */
#define RSA_BLOCK_SIZE_BITS            2048U

/** @} */

/**
 * @defgroup SHA256_Macros SHA-256 Macros
 * @brief SHA-256 size and configuration macros.
 *
 * These macros define constants related to SHA-256 such as
 * digest size and block size in both bytes and bits.
 *
 * @{
 */

/**
 * @def SHA256_DIGEST_SIZE
 * @brief SHA-256 digest size in bytes.
 */
#define SHA256_DIGEST_SIZE        32U

/**
 * @def SHA256_DIGEST_SIZE_BITS
 * @brief SHA-256 digest size in bits.
 */
#define SHA256_DIGEST_SIZE_BITS   256U

/**
 * @def SHA256_BLOCK_SIZE
 * @brief SHA-256 block size in bytes.
 */
#define SHA256_BLOCK_SIZE         64U

/**
 * @def SHA256_BLOCK_SIZE_BITS
 * @brief SHA-256 block size in bits.
 */
#define SHA256_BLOCK_SIZE_BITS    512U

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_CRYPTO_DEFINES_H_
