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
 * @file psa.h
 * @brief PSA Crypto API declarations for the MGS2401 SoC.
 * @details This header declares all Platform Security Architecture (PSA)
 *          Crypto APIs for the MGS2401 SoC. It provides interfaces for 
 *          symmetric cipher operations (AES), cryptographic hash operations
 *          (SHA-256), asymmetric operations (RSA sign, verify, encrypt, decrypt),
 *          key management, and random number generation, all compliant with the
 *          PSA Crypto specification.
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
 * 02-02-2026 | 1.1     | Dayana Devi K         | Added additional APIs for
 *            |         |                       | destroy key, signature 
 *            |         |                       | generation and verification 
 *            |         |                       | with deterministic salt values.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PSA_H_
#define BSP_INCLUDE_PSA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include "psa_errors.h"
#include "psa_types.h"

/**
 * @defgroup psa_cipher Cipher operations
 * @brief Functions for symmetric cipher encryption and decryption.
 *
 * @details This group provides APIs to perform single-shot and multipart
 *          symmetric cipher operations using PSA Crypto compliant interfaces.
 *          Supported operations include setup, IV configuration, update,
 *          finish, and abort of cipher operations. All operations support
 *          AES cipher modes: CBC, CTR, CFB, and OFB with key sizes of
 *          128, 192, and 256 bits.
 *
 * @{
 */

/**
 * @brief Encrypts a plaintext using a symmetric cipher key.
 *
 * @details This function performs single-shot symmetric encryption using the
 *          specified cipher algorithm. A random initialization vector (IV) is
 *          generated internally and prepended to the output buffer followed by
 *          the ciphertext. The key must permit encryption usage and be
 *          compatible with the selected algorithm. The input length must be
 *          a non-zero multiple of PSA_MG_AES_BLOCK_LENGTH.
 *
 * @param key           Identifier of the key to use for encryption.
 *                      Must be a valid non-zero key identifier with
 *                      PSA_KEY_USAGE_ENCRYPT permission.
 * @param alg           Cipher algorithm to use. Must satisfy
 *                      PSA_ALG_IS_CIPHER(alg) and be one of the supported
 *                      algorithms: PSA_ALG_CBC_NO_PADDING, PSA_ALG_CTR,
 *                      PSA_ALG_CFB, or PSA_ALG_OFB.
 * @param input         Pointer to the plaintext input buffer. Must not be NULL.
 * @param input_length  Size of the input buffer in bytes. Must be a non-zero
 *                      multiple of PSA_MG_AES_BLOCK_LENGTH.
 * @param output        Buffer where the IV followed by the ciphertext is
 *                      written. Must not be NULL and must be at least
 *                      PSA_MG_AES_IV_SIZE + input_length bytes.
 * @param output_size   Size of the output buffer in bytes.
 * @param output_length On success, set to the total number of bytes written
 *                      to the output buffer (PSA_MG_AES_IV_SIZE +
 *                      input_length). Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful encryption.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input, output, or output_length is NULL,
 *   the key identifier is zero, the input length is zero or not a multiple
 *   of PSA_MG_AES_BLOCK_LENGTH, or the algorithm is not a cipher algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is not one of the four
 *   supported cipher modes.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if output_size is smaller than
 *   PSA_MG_AES_IV_SIZE + input_length bytes.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow encryption usage.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier does not refer to a
 *   valid key.
 * - @ref PSA_ERROR_BAD_STATE if the crypto subsystem is not initialized.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 */
psa_status_t psa_cipher_encrypt(psa_key_id_t key, psa_algorithm_t alg,
                                const uint8_t *input, size_t input_length,
                                uint8_t *output, size_t output_size,
                                size_t *output_length);

/**
 * @brief Decrypts a ciphertext using a symmetric cipher key.
 *
 * @details This function performs single-shot symmetric decryption using the
 *          specified cipher algorithm. The input buffer must contain the IV
 *          followed by the ciphertext. The IV is extracted internally and used
 *          for decryption. The key must permit decryption usage and be
 *          compatible with the selected algorithm. The ciphertext portion
 *          (input minus IV) must be a non-zero multiple of
 *          PSA_MG_AES_BLOCK_LENGTH.
 *
 * @param key           Identifier of the key to use for decryption.
 *                      Must be a valid non-zero key identifier with
 *                      PSA_KEY_USAGE_DECRYPT permission.
 * @param alg           Cipher algorithm to use. Must satisfy
 *                      PSA_ALG_IS_CIPHER(alg) and be one of the supported
 *                      algorithms: PSA_ALG_CBC_NO_PADDING, PSA_ALG_CTR,
 *                      PSA_ALG_CFB, or PSA_ALG_OFB.
 * @param input         Pointer to the input buffer containing the IV
 *                      followed by the ciphertext. Must not be NULL.
 * @param input_length  Size of the input buffer in bytes. Must be greater
 *                      than PSA_MG_AES_IV_SIZE and the ciphertext portion
 *                      must be a non-zero multiple of PSA_MG_AES_BLOCK_LENGTH.
 * @param output        Buffer where the decrypted plaintext is written.
 *                      Must not be NULL.
 * @param output_size   Size of the output buffer in bytes. Must be at least
 *                      input_length - PSA_MG_AES_IV_SIZE bytes.
 * @param output_length On success, set to the number of bytes of plaintext
 *                      written to the output buffer. Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful decryption.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input, output, or output_length is NULL,
 *   the key identifier is zero, the input length is not greater than
 *   PSA_MG_AES_IV_SIZE, the ciphertext portion is not a multiple of
 *   PSA_MG_AES_BLOCK_LENGTH, or the algorithm is not a cipher algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is not one of the four
 *   supported cipher modes.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if output_size is smaller than
 *   input_length - PSA_MG_AES_IV_SIZE bytes.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow decryption usage.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier does not refer to a
 *   valid key.
 * - @ref PSA_ERROR_BAD_STATE if the crypto subsystem is not initialized.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 */
psa_status_t psa_cipher_decrypt(psa_key_id_t key, psa_algorithm_t alg,
                                const uint8_t *input, size_t input_length,
                                uint8_t *output, size_t output_size,
                                size_t *output_length);

/**
 * @brief Sets up a multipart cipher operation for encryption.
 *
 * @details This function initializes a multipart cipher operation context
 *          for encryption using the specified key and algorithm. Once set up,
 *          the operation must be used in the following sequence:
 *          psa_cipher_set_iv() → psa_cipher_update() → psa_cipher_finish().
 *
 *          psa_cipher_update() may be called once with the entire input or
 *          multiple times with block-aligned chunks. Each call to
 *          psa_cipher_update() must provide a non-zero multiple of
 *          PSA_MG_AES_BLOCK_LENGTH bytes.
 *
 *          On any failure after setup, call psa_cipher_abort() to clean up.
 *
 * @param operation Pointer to the cipher operation context to initialize.
 *                  Must not be NULL and must be in a freshly initialized
 *                  state (PSA_CIPHER_OPERATION_INIT).
 * @param key       Identifier of the key to use for encryption. Must be a
 *                  valid non-zero key identifier with PSA_KEY_USAGE_ENCRYPT
 *                  permission.
 * @param alg       Cipher algorithm to use. Must satisfy
 *                  PSA_ALG_IS_CIPHER(alg) and be one of the supported
 *                  algorithms: PSA_ALG_CBC_NO_PADDING, PSA_ALG_CTR,
 *                  PSA_ALG_CFB, or PSA_ALG_OFB.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the operation is successfully set up.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL or the operation
 *   has already been set up (alg or iv_set fields are non-zero).
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the key identifier is zero or the
 *   algorithm is not a cipher algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is not one of the four
 *   supported cipher modes.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow encryption usage.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier does not refer to a
 *   valid key.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 */
psa_status_t psa_cipher_encrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_id_t key,
                                      psa_algorithm_t alg);

/**
 * @brief Sets up a multipart cipher operation for decryption.
 *
 * @details This function initializes a multipart cipher operation context
 *          for decryption using the specified key and algorithm. Once set up,
 *          the operation must be used in the following sequence:
 *          psa_cipher_set_iv() → psa_cipher_update() → psa_cipher_finish().
 *
 *          psa_cipher_update() may be called once with the entire input or
 *          multiple times with block-aligned chunks. Each call to
 *          psa_cipher_update() must provide a non-zero multiple of
 *          PSA_MG_AES_BLOCK_LENGTH bytes.
 *
 *          On any failure after setup, call psa_cipher_abort() to clean up.
 *
 * @param operation Pointer to the cipher operation context to initialize.
 *                  Must not be NULL and must be in a freshly initialized
 *                  state (PSA_CIPHER_OPERATION_INIT).
 * @param key       Identifier of the key to use for decryption. Must be a
 *                  valid non-zero key identifier with PSA_KEY_USAGE_DECRYPT
 *                  permission.
 * @param alg       Cipher algorithm to use. Must satisfy
 *                  PSA_ALG_IS_CIPHER(alg) and be one of the supported
 *                  algorithms: PSA_ALG_CBC_NO_PADDING, PSA_ALG_CTR,
 *                  PSA_ALG_CFB, or PSA_ALG_OFB.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the operation is successfully set up.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL or the operation
 *   has already been set up (alg or iv_set fields are non-zero).
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the key identifier is zero or the
 *   algorithm is not a cipher algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is not one of the four
 *   supported cipher modes.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow decryption usage.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier does not refer to a
 *   valid key.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 */
psa_status_t psa_cipher_decrypt_setup(psa_cipher_operation_t *operation,
                                      psa_key_id_t key,
                                      psa_algorithm_t alg);

/**
 * @brief Sets the initialization vector (IV) for a multipart cipher operation.
 *
 * @details This function assigns the IV to an initialized multipart cipher
 *          operation context. The IV must be provided exactly once after
 *          psa_cipher_encrypt_setup() or psa_cipher_decrypt_setup() and
 *          before any call to psa_cipher_update(). On invalid input, the
 *          cipher operation is aborted automatically to prevent misuse.
 *
 * @param operation   Pointer to an initialized multipart cipher operation
 *                    context. Must not be NULL, must have been set up via
 *                    psa_cipher_encrypt_setup() or psa_cipher_decrypt_setup(),
 *                    and IV must not have been set already.
 * @param iv          Pointer to the initialization vector buffer.
 *                    Must not be NULL.
 * @param iv_length   Length of the IV in bytes. Must be exactly
 *                    PSA_MG_AES_IV_SIZE (16 bytes).
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the IV is successfully set.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL, the operation
 *   has not been set up (alg field is zero), or the IV has already been set.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the IV pointer is NULL or the IV length
 *   does not equal PSA_MG_AES_IV_SIZE.
 */
psa_status_t psa_cipher_set_iv(psa_cipher_operation_t *operation,
                               const uint8_t *iv, size_t iv_length);

/**
 * @brief Processes input data for a multipart cipher operation.
 *
 * @details This function encrypts or decrypts input data as part of an
 *          ongoing multipart cipher operation. The operation must have been
 *          initialized via psa_cipher_encrypt_setup() or
 *          psa_cipher_decrypt_setup() and the IV must be set via
 *          psa_cipher_set_iv() before calling this function.
 *
 *          This function may be called once with the entire input or multiple
 *          times with sequential block-aligned chunks. In both cases each
 *          call must provide a non-zero multiple of PSA_MG_AES_BLOCK_LENGTH
 *          bytes. Each call produces output of the same length as the input.
 *
 *          On any error, the cipher operation is aborted automatically to
 *          maintain a consistent state.
 *
 * @param operation      Pointer to an active multipart cipher operation
 *                       context. Must not be NULL, IV must be set, and
 *                       alg must be non-zero.
 * @param input          Pointer to the input data buffer. Must not be NULL.
 * @param input_length   Length of the input data in bytes. Must be a
 *                       non-zero multiple of PSA_MG_AES_BLOCK_LENGTH.
 * @param output         Buffer where the processed output data is written.
 *                       Must not be NULL and must be at least input_length
 *                       bytes.
 * @param output_size    Size of the output buffer in bytes.
 * @param output_length  On success, set to the number of bytes written to
 *                       the output buffer. Always equal to input_length.
 *                       Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the input data is successfully processed.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL, the IV has not
 *   been set, or the algorithm field is zero.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if any pointer is NULL, the input length
 *   is zero, or the input length is not a multiple of
 *   PSA_MG_AES_BLOCK_LENGTH.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if output_size is smaller than
 *   input_length bytes.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if the underlying AES operation fails.
 */
psa_status_t psa_cipher_update(psa_cipher_operation_t *operation,
                               const uint8_t *input, size_t input_length,
                               uint8_t *output, size_t output_size,
                               size_t *output_length);

/**
 * @brief Completes a multipart cipher operation.
 *
 * @details This function completes an ongoing multipart cipher operation
 *          after all input data has been processed using psa_cipher_update().
 *          It validates the operation state and ensures that no unprocessed
 *          data remains. Since padding is not supported by the underlying
 *          AES hardware, no additional output is produced during finalization
 *          and output_length is always set to 0 on success. Once this
 *          function returns, regardless of success or failure, the operation
 *          context is automatically zeroized via psa_cipher_abort() and must
 *          not be used for further processing.
 *
 * @param operation      Pointer to an active cipher operation context.
 *                       Must not be NULL, IV must be set, and alg must be
 *                       non-zero.
 * @param output         Buffer provided for final output. No data is written
 *                       to this buffer by this implementation since padding
 *                       is not supported.
 * @param output_size    Size of the output buffer in bytes.
 * @param output_length  On success, always set to 0 since no additional
 *                       output is produced. Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the cipher operation is finalized successfully.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL, the IV has not
 *   been set, the algorithm field is zero, or there is remaining unprocessed
 *   data (unprocessed_len != 0).
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the output_length pointer is NULL.
 */
psa_status_t psa_cipher_finish(psa_cipher_operation_t *operation,
                               uint8_t *output, size_t output_size,
                               size_t *output_length);

/**
 * @brief Aborts an ongoing cipher operation and securely clears all state.
 *
 * @details This function terminates an active multipart cipher operation.
 *          It securely zeroizes the entire operation structure using
 *          mg_zeroize(), resetting all internal cipher state including key
 *          material, IV, and operation flags. After this call the operation
 *          context is reset to an unused state and can be safely reused for
 *          a new cipher setup or discarded. This function can be called at
 *          any point including after an error to ensure proper cleanup.
 *
 * @param operation  Pointer to the cipher operation context to abort.
 *                   Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the operation was successfully aborted and cleaned up.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL.
 */
psa_status_t psa_cipher_abort(psa_cipher_operation_t *operation);

/**
 * @brief Executes a complete multipart cipher operation in a single call.
 *
 * @details This function performs a complete AES cipher operation by
 *          internally using the multipart cipher flow — mg_cipher_setup_operation(),
 *          psa_cipher_set_iv(), psa_cipher_update(), and psa_cipher_finish().
 *          The entire input is processed in fixed-size chunks of
 *          PSA_MG_AES_BLOCK_LENGTH bytes using repeated psa_cipher_update()
 *          calls. All internal resources are automatically cleaned up on both
 *          success and failure.
 *
 * @warning This function must be called with the entire input in a single
 *          call. It does not support incremental invocation — the complete
 *          plaintext or ciphertext must be provided at once. For incremental
 *          processing use psa_cipher_encrypt_setup() / psa_cipher_decrypt_setup()
 *          with psa_cipher_update() directly.
 *
 * @param key              Identifier of the key to use. Must be a valid
 *                         non-zero key identifier with appropriate usage
 *                         permission (PSA_KEY_USAGE_ENCRYPT for encryption,
 *                         PSA_KEY_USAGE_DECRYPT for decryption).
 * @param alg              Cipher algorithm to use. Must be one of the
 *                         supported algorithms: PSA_ALG_CBC_NO_PADDING,
 *                         PSA_ALG_CTR, PSA_ALG_CFB, or PSA_ALG_OFB.
 * @param iv               Pointer to the initialization vector buffer.
 *                         Must not be NULL and must be exactly
 *                         PSA_MG_AES_IV_SIZE bytes.
 * @param iv_length        Length of the IV in bytes. Must equal
 *                         PSA_MG_AES_IV_SIZE.
 * @param input            Pointer to the input data buffer. Must not be NULL.
 * @param input_length     Length of the input data in bytes. Must be a
 *                         non-zero multiple of PSA_MG_AES_BLOCK_LENGTH.
 * @param output           Buffer where the encrypted or decrypted data is
 *                         written. Must be at least input_length bytes.
 * @param output_size      Size of the output buffer in bytes.
 * @param output_length    On success, set to the total number of bytes
 *                         written to the output buffer. Must not be NULL.
 * @param cipher_operation Selects the direction of the operation. Must be
 *                         MG_PSA_AES_ENCRYPT or MG_PSA_AES_DECRYPT.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the operation completes successfully.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input_length is zero, not a multiple of
 *   PSA_MG_AES_BLOCK_LENGTH, or cipher_operation is neither
 *   MG_PSA_AES_ENCRYPT nor MG_PSA_AES_DECRYPT.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is not one of the four
 *   supported cipher modes.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow the requested usage.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier does not refer to a
 *   valid key.
 * - @ref PSA_ERROR_BAD_STATE if the internal operation state is invalid.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity error is detected.
 */
psa_status_t mg_psa_cipher_multirun(psa_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *iv,
                                    size_t iv_length,
                                    const uint8_t *input,
                                    size_t input_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length,
                                    mg_operation_t cipher_operation);

/** @} */ /* end of psa_cipher */


/**
 * @defgroup psa_hash Hash operations
 * @brief Functions for cryptographic hash computation.
 *
 * @details This group provides APIs for single-shot and multipart hash
 * operations using PSA Crypto compliant interfaces. Supported algorithms
 * currently include SHA-256.
 *
 * @{
 */

/**
 * @brief Computes a hash for the given input in a single call.
 *
 * @details This function performs a single-shot hash computation using the
 * specified PSA hash algorithm. The entire input buffer is processed in one
 * call and the resulting hash is written to the output buffer. This
 * implementation currently supports only the SHA-256 algorithm.
 *
 * @note For the multipart internal path, use mg_psa_hash_compute_multirun()
 *       instead.
 *
 * @param alg          Hash algorithm to use. Must satisfy PSA_ALG_IS_HASH(alg).
 * @param input        Pointer to the input data to be hashed.
 * @param input_length Size of the input data in bytes. May be 0 for an
 *                     empty message.
 * @param hash         Buffer where the computed hash is written.
 * @param hash_size    Size of the hash output buffer in bytes. Must be at
 *                     least PSA_MG_SHA256_OUTPUT_LEN.
 * @param hash_length  On success, number of bytes written to the hash buffer.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the hash computation is successful.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input, hash, or hash_length is NULL, or
 *   the algorithm is not a valid hash algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the requested hash algorithm is not supported.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if hash_size is less than
 *   PSA_MG_SHA256_OUTPUT_LEN.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if the hash computation fails internally.
 */
psa_status_t psa_hash_compute(psa_algorithm_t alg, const uint8_t *input,
                              size_t input_length, uint8_t *hash,
                              size_t hash_size, size_t *hash_length);

/**
 * @brief Compares a computed hash of the input data with a reference hash.
 *
 * @details This function computes the hash of the provided input buffer using
 * the specified hash algorithm and compares it against the given reference
 * hash. The reference hash length must exactly match PSA_MG_SHA256_OUTPUT_LEN.
 * This implementation currently supports only the SHA-256 algorithm.
 *
 * @param alg          Hash algorithm to use. Must satisfy PSA_ALG_IS_HASH(alg).
 * @param input        Pointer to the input data whose hash is to be computed.
 * @param input_length Size of the input data in bytes. May be 0 for an
 *                     empty message.
 * @param hash         Pointer to the reference hash to compare against.
 * @param hash_length  Size of the reference hash in bytes. Must be equal to
 *                     PSA_MG_SHA256_OUTPUT_LEN.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the computed hash matches the reference hash.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input or hash is NULL, the algorithm is
 *   not a valid hash algorithm, or hash_length does not equal
 *   PSA_MG_SHA256_OUTPUT_LEN.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the requested hash algorithm is not supported.
 * - @ref PSA_ERROR_INVALID_SIGNATURE if the computed hash does not match the
 *   provided reference hash.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal failure occurs during
 *   hash computation or if the computed hash length is unexpected.
 */
psa_status_t psa_hash_compare(psa_algorithm_t alg, const uint8_t *input,
                              size_t input_length, const uint8_t *hash,
                              size_t hash_length);

/**
 * @brief Initializes a hash operation context for multipart hashing.
 *
 * @details This function prepares a hash operation context for incremental
 * hashing using the specified algorithm. The operation context must be
 * initialized to PSA_HASH_OPERATION_INIT before calling this function.
 * Once set up, the operation must be followed by exactly one call to
 * psa_hash_update() with the complete input, and then psa_hash_finish()
 * to retrieve the result.
 *
 * @param operation Pointer to the hash operation object to be initialized.
 *                  Must not be NULL and must be in a zeroed or freshly
 *                  initialized state (PSA_HASH_OPERATION_INIT).
 * @param alg       Hash algorithm to use for the operation.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the hash operation context is successfully initialized.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL or the operation
 *   has already been set up.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the algorithm is not a valid hash algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the algorithm is a valid hash algorithm but
 *   is not supported by this implementation.
 */
psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg);

/**
 * @brief Provides input data to an ongoing hash operation.
 *
 * @details This function processes the input data as part of a multipart
 * hash operation. The operation must have been successfully initialized
 * using psa_hash_setup() before calling this function.
 *
 * @warning This implementation supports only a single call to
 *          psa_hash_update() per operation. The entire input must be
 *          provided in one call — multiple sequential calls to
 *          psa_hash_update() on the same operation are not supported and
 *          will produce an incorrect hash result. After this call,
 *          psa_hash_finish() must be called to retrieve the result.
 *
 * Large inputs are handled transparently by processing them in multiple
 * internal chunks. On any failure, the operation is aborted automatically
 * and must be reinitialized before reuse.
 *
 * @param operation    Pointer to an active hash operation context initialized
 *                     via psa_hash_setup().
 * @param input        Pointer to the input data to be hashed. Must not be
 *                     NULL. A zero-length input is permitted.
 * @param input_length Length of the input data in bytes. A value of 0 is
 *                     permitted and correctly handles the empty message case.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the input data is successfully processed.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL or the operation
 *   has not been properly initialized via psa_hash_setup().
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the input pointer is NULL.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal processing error occurs
 *   during hashing.
 */
psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input, size_t input_length);

/**
 * @brief Finalizes a multipart hash operation and outputs the computed hash.
 *
 * @details This function finalizes an ongoing hash operation that was
 * initialized using psa_hash_setup() and updated using psa_hash_update().
 * It retrieves the final hash value and writes it to the provided output
 * buffer. After this call, regardless of success or failure, the operation
 * is aborted and must not be used again unless reinitialized via
 * psa_hash_setup().
 *
 * @param operation   Pointer to an active hash operation context that has
 *                    been updated via psa_hash_update().
 * @param hash        Buffer where the computed hash value is written.
 * @param hash_size   Size of the hash output buffer in bytes. Must be at
 *                    least PSA_MG_SHA256_OUTPUT_LEN.
 * @param hash_length On success, set to the number of bytes written to the
 *                    hash buffer. This will equal PSA_MG_SHA256_OUTPUT_LEN.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful completion of the hash operation.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL or the operation
 *   has not been properly initialized.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if hash or hash_length is NULL.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if hash_size is less than
 *   PSA_MG_SHA256_OUTPUT_LEN.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal error occurs during hash
 *   finalization or if the output length is unexpected.
 */
psa_status_t psa_hash_finish(psa_hash_operation_t *operation, uint8_t *hash,
                             size_t hash_size, size_t *hash_length);

/**
 * @brief Aborts an ongoing hash operation and clears all associated state.
 *
 * @details This function terminates a hash operation that was previously
 * initialized using psa_hash_setup(). It securely zeroes all internal state
 * associated with the operation and resets the operation object to its
 * initial state. Calling this function on an inactive or already-aborted
 * operation is permitted and results in no error.
 *
 * @param operation Pointer to the hash operation structure to abort.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the hash operation is successfully aborted or was already
 *   inactive.
 * - @ref PSA_ERROR_BAD_STATE if the operation pointer is NULL.
 */
psa_status_t psa_hash_abort(psa_hash_operation_t *operation);

/**
 * @brief Computes a hash value using the multipart hash flow in a single call.
 *
 * @details This function is a convenience wrapper that performs a complete
 * hash computation by internally executing psa_hash_setup(),
 * psa_hash_update(), and psa_hash_finish() in sequence. It uses the
 * multipart internal path, which allows the hash output to be retrieved
 * after processing. Use this function when the multipart internal path is
 * required. For a direct single-shot computation, use psa_hash_compute()
 * instead.
 *
 * @param alg          Hash algorithm to use. Must be a supported hash
 *                     algorithm (e.g. PSA_ALG_SHA_256).
 * @param input        Pointer to the input data to be hashed. Must not be
 *                     NULL. A zero-length input is permitted.
 * @param input_length Size of the input data in bytes.
 * @param hash         Buffer where the computed hash is written.
 * @param hash_size    Size of the hash output buffer in bytes. Must be at
 *                     least PSA_MG_SHA256_OUTPUT_LEN.
 * @param hash_length  On success, set to the number of bytes written to the
 *                     hash buffer. This will equal PSA_MG_SHA256_OUTPUT_LEN.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the hash computation completes successfully.
 * - @ref PSA_ERROR_BAD_STATE if the internal hash operation cannot be initialized
 *   or is in an invalid state.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the algorithm is not a valid hash algorithm.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the requested hash algorithm is not supported.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if hash_size is less than
 *   PSA_MG_SHA256_OUTPUT_LEN.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal error occurs during hash
 *   processing.
 */
psa_status_t mg_psa_hash_compute_multirun(psa_algorithm_t alg,
                                          const uint8_t *input,
                                          size_t input_length,
                                          uint8_t *hash,
                                          size_t hash_size,
                                          size_t *hash_length);

/** @} */ /* end of psa_hash */


/**
 * @defgroup psa_rsa RSA operations
 * @brief Functions for RSA encryption, decryption, signing, and verification.
 *
 * @details This group provides APIs to perform asymmetric cryptography using
 *          PSA Crypto compliant interfaces on Secure-IOT devices. Supported
 *          operations include:
 *          - RSA message signing and verification
 *          - RSA public-key encryption and private-key decryption
 *
 *          Supported algorithms:
 *          - PKCS#1 v1.5 signature: #PSA_ALG_RSA_PKCS1V15_SIGN_RAW
 *          - RSA-PSS signature: #PSA_ALG_RSA_PSS_BASE
 *          - PKCS#1 v1.5 encryption: #PSA_ALG_RSA_PKCS1V15_CRYPT
 *          - RSA-OAEP encryption: #PSA_ALG_RSA_OAEP_SHA256
 *
 *          The APIs handle key slot management, padding/unpadding, and error
 *          handling internally. For RSA-OAEP, the salt parameter acts as
 *          the label.
 *
 * @{
 */

/**
 * @brief Signs a message using a private key.
 *
 * @details This function generates a digital signature over the input message
 *          using the private key. It supports RSA-based signature algorithms.
 *          The output signature is written to the signature buffer.
 *
 * @param key               Identifier of the key to use for signing.
 * @param alg               Signature algorithm to use. Supported values:
 *                          - #PSA_ALG_RSA_PKCS1V15_SIGN_RAW
 *                          - #PSA_ALG_RSA_PSS_BASE
 * @param input             Pointer to the message to sign.
 * @param input_length      Length of the message in bytes.
 * @param signature         Pointer to buffer where the signature will be
 *                          written.
 * @param signature_size    Size of the signature buffer in bytes.
 * @param signature_length  Pointer to variable that will receive the actual
 *                          signature length.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful signature generation.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid or the
 *   algorithm is unsupported.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow signing.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if signature_size is insufficient.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_sign_message(psa_key_id_t key, psa_algorithm_t alg,
                              const uint8_t *input, size_t input_length,
                              uint8_t *signature, size_t signature_size,
                              size_t *signature_length);

/**
 * @brief Signs a message using a private key with a user-provided salt.
 *
 * @details Similar to psa_sign_message(), but allows the caller to specify
 *          a salt for PSS signature algorithms. The salt is only used when
 *          alg is #PSA_ALG_RSA_PSS_BASE.
 *
 * @param key               Identifier of the key to use for signing.
 * @param alg               Signature algorithm. Supported values:
 *                          - #PSA_ALG_RSA_PKCS1V15_SIGN_RAW
 *                          - #PSA_ALG_RSA_PSS_BASE
 * @param input             Pointer to the message to sign.
 * @param input_length      Length of the message in bytes.
 * @param signature         Pointer to buffer where the signature will be
 *                          written.
 * @param signature_size    Size of the signature buffer in bytes.
 * @param signature_length  Pointer to variable that will receive the actual
 *                          signature length.
 * @param salt              Pointer to user-provided salt. May be NULL if
 *                          salt_length is 0.
 * @param salt_length       Length of the user-provided salt in bytes.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful signature generation.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid or the
 *   algorithm is unsupported.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow signing.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if signature_size is insufficient.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_sign_message_with_salt(psa_key_id_t key, psa_algorithm_t alg,
                                        const uint8_t *input,
                                        size_t input_length,
                                        uint8_t *signature,
                                        size_t signature_size,
                                        size_t *signature_length,
                                        const uint8_t *salt,
                                        size_t salt_length);

/**
 * @brief Verifies a digital signature on a message using a public key.
 *
 * @details This function verifies that signature matches the hash of input
 *          using the public key identified by key. Supported algorithms are
 *          RSA-based signature schemes.
 *
 * @param key               Identifier of the key to use for verification.
 * @param alg               Signature algorithm. Supported values:
 *                          - #PSA_ALG_RSA_PKCS1V15_SIGN_RAW
 *                          - #PSA_ALG_RSA_PSS_BASE
 * @param input             Pointer to the message whose signature is being
 *                          verified.
 * @param input_length      Length of the message in bytes.
 * @param signature         Pointer to the signature to verify.
 * @param signature_length  Length of the signature in bytes.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the signature is valid.
 * - @ref PSA_ERROR_INVALID_SIGNATURE if the signature does not match.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid or the
 *   algorithm is unsupported.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow verification.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_verify_message(psa_key_id_t key, psa_algorithm_t alg,
                                const uint8_t *input, size_t input_length,
                                const uint8_t *signature,
                                size_t signature_length);

/**
 * @brief Verifies a digital signature on a message with an expected salt
 *        length.
 *
 * @details Similar to psa_verify_message(), but allows specifying the
 *          expected salt length for PSS signature verification.
 *
 * @param key                   Identifier of the key to use for verification.
 * @param alg                   Signature algorithm. Supported values:
 *                              - #PSA_ALG_RSA_PKCS1V15_SIGN_RAW
 *                              - #PSA_ALG_RSA_PSS_BASE
 * @param input                 Pointer to the message whose signature is
 *                              being verified.
 * @param input_length          Length of the message in bytes.
 * @param signature             Pointer to the signature to verify.
 * @param signature_length      Length of the signature in bytes.
 * @param expected_salt_length  Expected salt length for PSS verification.
 *                              Must match the salt length used during signing.
 *                              Pass 32 for the default randomly generated salt.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the signature is valid.
 * - @ref PSA_ERROR_INVALID_SIGNATURE if the signature does not match.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid or the
 *   algorithm is unsupported.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow verification.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_verify_message_with_salt(psa_key_id_t key,
                                          psa_algorithm_t alg,
                                          const uint8_t *input,
                                          size_t input_length,
                                          const uint8_t *signature,
                                          size_t signature_length,
                                          size_t expected_salt_length);

/**
 * @brief Encrypts a message using a public key.
 *
 * @details This function performs asymmetric encryption using the key
 *          identified by key. For RSA-OAEP, the salt parameter serves as
 *          the label.
 *
 * @param key               Identifier of the key to use for encryption.
 * @param alg               Encryption algorithm. Supported values:
 *                          - #PSA_ALG_RSA_PKCS1V15_CRYPT
 *                          - #PSA_ALG_RSA_OAEP_SHA256
 * @param input             Pointer to the plaintext input.
 * @param input_length      Length of the plaintext in bytes.
 * @param salt              Optional label for OAEP encryption. Ignored for
 *                          PKCS#1 v1.5. May be NULL if salt_length is 0.
 * @param salt_length       Length of the salt/label in bytes.
 * @param output            Pointer to buffer where ciphertext will be written.
 * @param output_size       Size of the output buffer in bytes.
 * @param output_length     Pointer to variable that will receive the
 *                          ciphertext length.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful encryption.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid or the
 *   algorithm is unsupported.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow encryption.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if output_size is less than MG_RSA_256_BYTES.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_asymmetric_encrypt(psa_key_id_t key, psa_algorithm_t alg,
                                    const uint8_t *input, size_t input_length,
                                    const uint8_t *salt, size_t salt_length,
                                    uint8_t *output, size_t output_size,
                                    size_t *output_length);

/**
 * @brief Decrypts a message using a private key.
 *
 * @details This function performs asymmetric decryption using the key
 *          identified by key. For RSA-OAEP, the salt parameter serves as
 *          the label.
 *
 * @param key               Identifier of the key to use for decryption.
 * @param alg               Decryption algorithm. Supported values:
 *                          - #PSA_ALG_RSA_PKCS1V15_CRYPT
 *                          - #PSA_ALG_RSA_OAEP_SHA256
 * @param input             Pointer to the ciphertext input.
 * @param input_length      Length of the ciphertext in bytes. Must be
 *                          exactly MG_RSA_256_BYTES.
 * @param salt              Optional label for OAEP decryption. Ignored for
 *                          PKCS#1 v1.5. May be NULL if salt_length is 0.
 * @param salt_length       Length of the salt/label in bytes.
 * @param output            Pointer to buffer where plaintext will be written.
 * @param output_size       Size of the output buffer in bytes.
 * @param output_length     Pointer to variable that will receive the
 *                          plaintext length.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful decryption.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if input arguments are invalid, the algorithm
 *   is unsupported, or input_length is not MG_RSA_256_BYTES.
 * - @ref PSA_ERROR_NOT_PERMITTED if the key does not allow decryption.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if output_size is insufficient.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED for internal errors.
 * - @ref PSA_ERROR_INVALID_HANDLE if the key identifier is invalid.
 */
psa_status_t psa_asymmetric_decrypt(psa_key_id_t key, psa_algorithm_t alg,
                                    const uint8_t *input, size_t input_length,
                                    const uint8_t *salt, size_t salt_length,
                                    uint8_t *output, size_t output_size,
                                    size_t *output_length);

/** @} */ /* end of psa_rsa */


/**
 * @defgroup psa_random Random generation
 * @brief Functions for cryptographically secure random number generation.
 *
 * @details This group provides APIs to generate random data suitable for
 * cryptographic use. The implementation may use a hardware true random
 * number generator (TRNG) when available, or a software-based fallback
 * mechanism otherwise.
 *
 * @{
 */

/**
 * @brief Generates cryptographically secure random data.
 *
 * @details This function generates random bytes and writes them to the output
 * buffer. If a hardware-based true random number generator (TRNG) is available,
 * it is used as the entropy source. Otherwise, an implementation-defined
 * software random generator is used. This function provides a single-call
 * interface for obtaining random data suitable for cryptographic operations.
 *
 * @param output        Pointer to the buffer where generated random bytes
 *                      will be written.
 * @param output_size   Number of random bytes to generate. When PSA_TRNG_AVAILABLE
 *                      is not defined, must be a multiple of 4 and must not
 *                      exceed 256 bytes.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if random data is generated successfully.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if output is NULL, output_size is zero, or
 *   when PSA_TRNG_AVAILABLE is not defined, output_size exceeds 256 bytes
 *   or is not a multiple of 4 bytes.
 * - @ref PSA_ERROR_HARDWARE_FAILURE if the TRNG hardware times out while
 *   generating random data.
 *
 * @note When PSA_TRNG_AVAILABLE is not defined, the software fallback is
 *       not cryptographically secure and should be used for testing only.
 */
psa_status_t psa_generate_random(uint8_t *output, size_t output_size);
/** @} */ /* end of psa_random */

/**
 * @brief Initializes the PSA Crypto subsystem.
 *
 * @details This function initializes all internal components required by the
 *          PSA Crypto API. It sets up the True Random Number Generator (TRNG),
 *          seeds the entropy source, initializes cryptographic driver wrappers,
 *          and prepares the key slot management subsystem. Calling this function
 *          multiple times is explicitly permitted — if the subsystem is already
 *          fully initialized, the function returns  immediately
 *          without reinitializing any resources.
 *
 *          This function must be called before invoking any other PSA Crypto
 *          operation such as key import, cipher, hash, or random number
 *          generation. Failure to do so will result in PSA_ERROR_BAD_STATE
 *          from the called operation.
 *
 *          On any initialization failure, all partially initialized subsystems
 *          are cleaned up automatically before the error is returned.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the crypto subsystem is successfully initialized or was
 *   already fully initialized.
 * - @ref PSA_ERROR_INSUFFICIENT_MEMORY if key slot memory allocation fails
 *   (dynamic key store only).
 * - @ref PSA_ERROR_GENERIC_ERROR if hardware memory protection (PMP) setup fails
 *   (dynamic key store only).
 */
psa_status_t psa_crypto_init(void);

/**
 * @brief Sets the key type in a key attributes structure.
 *
 * @details This function assigns the key type (for example, AES, RSA) to a
 *          key attributes object. The key type defines the cryptographic
 *          algorithm family and the structure of the key material. This
 *          function does not perform validation; validation occurs when the
 *          key is created or imported.
 *
 * @param attributes Pointer to the key attributes structure to modify.
 * @param type       Key type to assign.
 */
void psa_set_key_type(psa_key_attributes_t *attributes, psa_key_type_t type);

/**
 * @brief Sets the permitted algorithm for a key.
 *
 * @details This function specifies the cryptographic algorithm or algorithm
 *          policy that the key is allowed to be used with. The algorithm
 *          restriction is enforced during cryptographic operations such as
 *          encryption, signing, or verification.
 *
 * @param attributes Pointer to the key attributes structure to modify.
 * @param alg        Algorithm or algorithm policy permitted for the key.
 */
void psa_set_key_algorithm(psa_key_attributes_t *attributes,
                           psa_algorithm_t alg);

/**
 * @brief Sets the key size in bits in a key attributes structure.
 *
 * @details This function records the key size to be associated with a key
 *          when it is later created or imported. If the provided size exceeds
 *          PSA_MAX_KEY_BITS, the size is marked as invalid by storing
 *          PSA_KEY_BITS_TOO_LARGE in the attributes structure. The value is
 *          not validated until key creation or import is attempted.
 *
 * @param attributes Pointer to the key attributes structure to modify.
 * @param bits       Key size in bits to set.
 */
void psa_set_key_bits(psa_key_attributes_t *attributes, size_t bits);

/**
 * @brief Sets the lifetime of a key.
 *
 * @details This function assigns a lifetime to the key, defining whether the
 *          key is volatile or persistent and where it is stored. If a volatile
 *          lifetime is selected, any previously assigned key identifier is
 *          cleared to zero to ensure the key is not treated as persistent.
 *
 * @param attributes Pointer to the key attributes structure to modify.
 * @param lifetime   Lifetime value to assign to the key.
 */
void psa_set_key_lifetime(psa_key_attributes_t *attributes,
                          psa_key_lifetime_t lifetime);

/**
 * @brief Sets the usage permissions for a key.
 *
 * @details This function defines the operations that the key is permitted to
 *          perform, such as encryption, decryption, signing, or verification.
 *          If hash-based signing or verification permissions are requested,
 *          the corresponding message-based permissions are automatically
 *          enabled.
 *
 * @param attributes  Pointer to the key attributes structure to modify.
 * @param usage_flags Bitmask of permitted key usage operations.
 */
void psa_set_key_usage_flags(psa_key_attributes_t *attributes,
                             psa_key_usage_t usage_flags);

/**
 * @brief Sets the key identifier in a key attributes structure.
 *
 * @details This function assigns a key identifier to the attributes structure.
 *          If the current lifetime is volatile, it is automatically updated to
 *          persistent to reflect that the key now has a fixed identifier.
 *
 * @param attributes Pointer to the key attributes structure to modify.
 * @param key        Key identifier to assign.
 */
void psa_set_key_id(psa_key_attributes_t *attributes, psa_key_id_t key);

/**
 * @brief Retrieves the key type from a key attributes structure.
 *
 * @details This function returns the key type that was previously set in the
 *          key attributes structure using psa_set_key_type().
 *
 * @param attributes Pointer to the key attributes structure.
 *
 * @return The key type stored in the attributes.
 */
psa_key_type_t psa_get_key_type(const psa_key_attributes_t *attributes);

/**
 * @brief Retrieves the permitted algorithm from a key attributes structure.
 *
 * @details This function returns the algorithm policy associated with the key,
 *          as previously set using psa_set_key_algorithm().
 *
 * @param attributes Pointer to the key attributes structure.
 *
 * @return The algorithm stored in the key policy.
 */
psa_algorithm_t psa_get_key_algorithm(const psa_key_attributes_t *attributes);

/**
 * @brief Retrieves the key size in bits from a key attributes structure.
 *
 * @details This function returns the key size that is currently set in the
 *          key attributes object. The value reflects what was configured
 *          using psa_set_key_bits() and is not validated until the key is
 *          created or imported.
 *
 * @param attributes Pointer to the key attributes structure to query.
 *
 * @return The key size in bits stored in the attributes structure.
 */
size_t psa_get_key_bits(const psa_key_attributes_t *attributes);

/**
 * @brief Retrieves the lifetime of a key from a key attributes structure.
 *
 * @details This function returns the key lifetime that was previously set
 *          using psa_set_key_lifetime().
 *
 * @param attributes Pointer to the key attributes structure.
 *
 * @return The key lifetime stored in the attributes.
 */
psa_key_lifetime_t psa_get_key_lifetime(const psa_key_attributes_t *attributes);

/**
 * @brief Retrieves the usage flags from a key attributes structure.
 *
 * @details This function returns the usage permissions associated with the
 *          key, as previously set using psa_set_key_usage_flags().
 *
 * @param attributes Pointer to the key attributes structure.
 *
 * @return The key usage flags stored in the attributes.
 */
psa_key_usage_t psa_get_key_usage_flags(const psa_key_attributes_t *attributes);

/**
 * @brief Retrieves the key identifier from a key attributes structure.
 *
 * @details This function returns the key identifier associated with the key
 *          attributes. For volatile keys, this value is zero until the key
 *          is created.
 *
 * @param attributes Pointer to the key attributes structure.
 *
 * @return The key identifier stored in the attributes.
 */
psa_key_id_t psa_get_key_id(const psa_key_attributes_t *attributes);

/**
 * @brief Imports a key into the PSA Crypto key store.
 *
 * @details This function creates a new key object from raw key material
 *          provided in the input buffer and stores it in the PSA Crypto key
 *          storage. The key is assigned a unique identifier and its attributes
 *          are set according to the provided `attributes` structure. This
 *          function handles both volatile and persistent keys, ensures
 *          bit-size limits, and invokes the driver to store key material
 *          securely.
 *
 * @param attributes   Pointer to a psa_key_attributes_t structure describing
 *                     the properties of the key (type, usage, algorithm,
 *                     lifetime, etc.). Must not be NULL.
 * @param data         Pointer to the raw key material to import.
 *                     Must not be NULL.
 * @param data_length  Size of the key material in bytes. Must be non-zero.
 * @param key          Pointer to a variable where the assigned key identifier
 *                     will be written upon successful import. Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the key is successfully imported and stored.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if any input pointer is NULL, the key length
 *   is zero, or the provided key bits do not match the expected values.
 * - @ref PSA_ERROR_NOT_SUPPORTED if the key size exceeds PSA_MAX_KEY_BITS or the
 *   key type is not supported.
 * - @ref PSA_ERROR_BAD_STATE if the PSA Crypto subsystem is not initialized.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if the allocated key buffer is too small for
 *   the provided key material.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity failure occurs
 *   during key import.
 */
psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data, size_t data_length,
                            psa_key_id_t *key);

/**
 * @brief Exports an RSA key (public or private) into a buffer in DER format.
 *
 * @details This function exports either an RSA private key pair or a public
 *          key from an mg_rsa_context into the provided buffer. The key is
 *          encoded in DER format as an ASN.1 SEQUENCE containing the modulus
 *          and either the private exponent (for key pairs) or the public
 *          exponent (for public keys). The exported data is aligned to the
 *          beginning of the output buffer.
 *
 * @param type        Type of the key to export. Must be either a key pair
 *                    type (PSA_KEY_TYPE_IS_KEY_PAIR) or a public key type
 *                    (PSA_KEY_TYPE_IS_PUBLIC_KEY). Any other type returns
 *                    PSA_ERROR_INVALID_ARGUMENT.
 * @param rsa         Pointer to an initialized mg_rsa_context holding the
 *                    RSA key material to export. Must not be NULL.
 * @param data        Pointer to the output buffer where the DER-encoded key
 *                    is written. Must not be NULL.
 * @param data_size   Size of the output buffer in bytes. Must be non-zero
 *                    and large enough to hold the DER-encoded key.
 * @param data_length On success, set to the number of bytes written to the
 *                    output buffer. Must not be NULL.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS on successful export.
 * - @ref PSA_ERROR_INVALID_ARGUMENT if the key type is neither a key pair
 *   nor a public key type, or if any input pointer is NULL, or if
 *   data_size is zero.
 * - @ref PSA_ERROR_BUFFER_TOO_SMALL if the output buffer is too small to
 *   hold the DER-encoded key.
 * - @ref PSA_ERROR_RSA_BAD_INPUT_DATA if a private exponent export is
 *   attempted on a context that holds only a public key (no private
 *   parameters).
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity check fails.
 *
 * @note On any failure, the output buffer is zeroed before returning.
 */
psa_status_t mg_psa_rsa_export_key(psa_key_type_t type, mg_rsa_context *rsa,
                                   uint8_t *data, size_t data_size,
                                   size_t *data_length);

/**
 * @brief Destroys a key identified by its key ID.
 *
 * @details This function securely deletes a key from the key store, zeroizing
 *          all associated key material and releasing the key slot. If the key
 *          ID is zero or the key does not exist in the store, the function
 *          treats it as a no-op and returns success. This ensures safe key
 *          management without revealing whether a key existed.
 *
 * @param key Identifier of the key to destroy.
 *
 * @return Returns a 32-bit status code:
 * - @ref PSA_SUCCESS if the key was successfully destroyed or did not exist.
 * - @ref PSA_ERROR_CORRUPTION_DETECTED if an internal integrity failure is
 *   detected during key slot cleanup.
 */
psa_status_t psa_destroy_key(psa_key_id_t key);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PSA_H_
