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
 * Project                   : Secure IoT SoC
 * @file psa_errors.h
 * @brief Header file for all error codes for PSA Crypto APIs in Mindgrove Silicon 
 *        Secure-IoT SoC.
 * @details This is the header file for the error codes used in  PSA Crypto APIs 
 *          in Secure-IOT Device. 
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

#ifndef BSP_INCLUDE_PSA_ERRORS_H_
#define BSP_INCLUDE_PSA_ERRORS_H_

#include <stdint.h>

/**
 * @defgroup PSA_STATUS_TYPE PSA Status Type
 *
 * @brief Defines the type used for function return status in PSA Crypto.
 *
 * @{
 */

/**
 * @brief Function return status type.
 *
 * This is either #PSA_SUCCESS (zero), indicating success,
 * or a small negative value indicating an error.
 */
#ifndef PSA_SUCCESS
typedef int32_t psa_status_t;
#endif

/** @} */

/**
 * @defgroup PSA_ERROR_CODES PSA Error Codes
 *
 * @brief PSA Crypto standard error codes for functions and modules.
 *
 * @{
 */

/** @brief The action was completed successfully. */
#ifndef PSA_SUCCESS
#define PSA_SUCCESS ((psa_status_t)0)
#endif

/**
 * @name ASN1 Error Codes
 * @{
 */

/** @brief Out of data when parsing an ASN1 data structure. */
#define PSA_ERROR_ASN1_OUT_OF_DATA        ((psa_status_t)-60)
/** @brief ASN1 tag was of an unexpected value. */
#define PSA_ERROR_ASN1_UNEXPECTED_TAG     ((psa_status_t)-61)
/** @brief Invalid length when parsing ASN1 structure. */
#define PSA_ERROR_ASN1_INVALID_LENGTH     ((psa_status_t)-62)
/** @brief Actual length differs from expected length. */
#define PSA_ERROR_ASN1_LENGTH_MISMATCH    ((psa_status_t)-63)
/** @brief ASN1 data is invalid. */
#define PSA_ERROR_ASN1_INVALID_DATA       ((psa_status_t)-64)
/** @brief Memory allocation failed. */
#define PSA_ERROR_ASN1_ALLOC_FAILED       ((psa_status_t)-65)
/** @brief Buffer too small when writing ASN1 data. */
#define PSA_ERROR_ASN1_BUF_TOO_SMALL      ((psa_status_t)-66)
/** @brief Bad input state during ASN1 processing. */
#define PSA_ERROR_ASN1_BAD_INPUT_STATE    ((psa_status_t)-67)

/** @} */  // end ASN1 Error Codes

/**
 * @name RSA Error Codes
 * @{
 */

/** @brief Bad input parameters to function. */
#define PSA_ERROR_RSA_BAD_INPUT_DATA      ((psa_status_t)-70)
/** @brief Input data contains invalid padding. */
#define PSA_ERROR_RSA_INVALID_PADDING     ((psa_status_t)-71)
/** @brief Key generation failed. */
#define PSA_ERROR_RSA_KEY_GEN_FAILED      ((psa_status_t)-72)
/** @brief Key failed validity check. */
#define PSA_ERROR_RSA_KEY_CHECK_FAILED    ((psa_status_t)-73)
/** @brief Public key operation failed. */
#define PSA_ERROR_RSA_PUBLIC_FAILED       ((psa_status_t)-74)
/** @brief Private key operation failed. */
#define PSA_ERROR_RSA_PRIVATE_FAILED      ((psa_status_t)-75)
/** @brief PKCS#1 verification failed. */
#define PSA_ERROR_RSA_VERIFY_FAILED       PSA_ERROR_INVALID_SIGNATURE
/** @brief Output buffer for decryption too small. */
#define PSA_ERROR_RSA_OUTPUT_TOO_LARGE    ((psa_status_t)-76)
/** @brief Random generator failed to generate non-zero bytes. */
#define PSA_ERROR_RSA_RNG_FAILED          ((psa_status_t)-77)
/** @brief Illegal length in RSA fp_int. */
#define PSA_ERROR_FPI_ILLEGAL_LENGTH      ((psa_status_t)-78)
/** @brief Bad input data from RSA fp_int. */
#define PSA_ERROR_FPI_BAD_INPUT_DATA      ((psa_status_t)-79)

/** @} */  // end RSA Error Codes


/**
 * @name Generic Error Codes
 * @{
 */

/** @brief An unspecified error occurred. */
#define PSA_ERROR_GENERIC_ERROR                 ((psa_status_t)-132)
/** @brief Requested action denied by policy. */
#define PSA_ERROR_NOT_PERMITTED                 ((psa_status_t)-133)
/** @brief Operation or parameter not supported. */
#define PSA_ERROR_NOT_SUPPORTED                 ((psa_status_t)-134)
/** @brief Invalid parameter(s). */
#define PSA_ERROR_INVALID_ARGUMENT              ((psa_status_t)-135)
/** @brief Key identifier is invalid. */
#define PSA_ERROR_INVALID_HANDLE                ((psa_status_t)-136)
/** @brief Action cannot be performed in current state. */
#define PSA_ERROR_BAD_STATE                     ((psa_status_t)-137)
/** @brief Output buffer too small. */
#define PSA_ERROR_BUFFER_TOO_SMALL              ((psa_status_t)-138)
/** @brief Item already exists. */
#define PSA_ERROR_ALREADY_EXISTS                ((psa_status_t)-139)
/** @brief Item does not exist. */
#define PSA_ERROR_DOES_NOT_EXIST                ((psa_status_t)-140)
/** @brief Not enough runtime memory. */
#define PSA_ERROR_INSUFFICIENT_MEMORY           ((psa_status_t)-141)
/** @brief Not enough persistent storage. */
#define PSA_ERROR_INSUFFICIENT_STORAGE          ((psa_status_t)-142)
/** @brief Permanent storage failure. */
#define PSA_ERROR_STORAGE_FAILURE               ((psa_status_t)-146)
/** @brief Hardware failure detected. */
#define PSA_ERROR_HARDWARE_FAILURE              ((psa_status_t)-147)
/** @brief Not enough entropy for requested action. */
#define PSA_ERROR_INSUFFICIENT_ENTROPY          ((psa_status_t)-148)
/** @brief Signature, MAC, or hash is incorrect. */
#define PSA_ERROR_INVALID_SIGNATURE             ((psa_status_t)-149)
/** @brief Decrypted padding is incorrect. */
#define PSA_ERROR_INVALID_PADDING               ((psa_status_t)-150)
/** @brief Tampering detected. */
#define PSA_ERROR_CORRUPTION_DETECTED           ((psa_status_t)-151)
/** @brief Stored data has been corrupted. */
#define PSA_ERROR_DATA_CORRUPT                  ((psa_status_t)-152)
/** @brief Data read from storage is not valid. */
#define PSA_ERROR_DATA_INVALID                  ((psa_status_t)-153)

/** @} */  // end Generic Error Codes

/** @} */  // end PSA_ERROR_CODES

#endif  // BSP_INCLUDE_PSA_ERRORS_H_
