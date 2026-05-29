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
 * @file crypto_helpers.h
 * @brief Helper utilities for cryptographic operations.
 * @details Provides utility functions for memory validation, random number
 *          generation, endian conversion, hex parsing, and debug printing.
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
 * 11-12-2025 | 1.1     | Jennifer Vinita J     | Added hex string conversion
 *            |         |                       | implementation.
 * 07-04-2026 | 1.2     | Dayana Devi K         | MISRA compliance updates and
 *            |         |                       | API refinements.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_CRYPTO_HELPERS_H_
#define BSP_INCLUDE_CRYPTO_HELPERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Endianness selection for print utilities.
 */
typedef enum {
    PRINT_LITTLE_ENDIAN = 0U,
    PRINT_BIG_ENDIAN    = 1U
} print_endian_t;

/**
 * @brief Polarity selection for Crypto_Wait_For_Timeout.
 *
 * @details Specifies whether the target bit is expected to transition
 * from low to high (active-high) or from high to low (active-low)
 * to indicate completion.
 */
typedef enum {
    CRYPTO_WAIT_ACTIVE_HIGH = 0U,  /* bit goes 0→1 */
    CRYPTO_WAIT_ACTIVE_LOW  = 1U   /* bit goes 1→0 */
} crypto_wait_polarity_t;

/**
 * @brief Validates memory allocation.
 *
 * @details Checks whether the provided pointer is NULL. This function is
 * typically used after dynamic memory allocation to ensure that the
 * allocation was successful before proceeding further.
 *
 * @param ptr Pointer to allocated memory.
 *
 * @return `SUCCESS` if the pointer is valid (non-NULL).
 *         `EFAULT` if the pointer is NULL.
 */
uint16_t Check_Calloc(const uint8_t *ptr);

/**
 * @brief Generates a non-zero 8-bit random value.
 *
 * @details Continuously generates random 8-bit values using the standard
 * library until a non-zero value is obtained. This ensures that the output
 * can be safely used in cryptographic contexts where zero values are invalid
 * (e.g., padding or nonce fields).
 *
 * @return Non-zero 8-bit random number.
 */
uint8_t Get_Non_Zero_Rand_8(void);

/**
 * @brief Generates a non-zero 64-bit random value.
 *
 * @details Constructs a 64-bit random number by combining multiple 8-bit
 * non-zero random values. The function ensures that the final output is
 * non-zero, which is useful for cryptographic operations requiring valid
 * non-zero inputs.
 *
 * @return Non-zero 64-bit random number.
 */
uint64_t Get_Non_Zero_Rand_64(void);

/**
 * @brief Reverses byte order of a 64-bit value.
 *
 * @details Performs byte-wise reversal of the input 64-bit value.
 * This is commonly used for converting between little-endian and
 * big-endian representations.
 *
 * Example:
 * - Input :  0x1122334455667788
 * - Output:  0x8877665544332211
 *
 * @param input_number Input 64-bit value.
 *
 * @return Byte-reversed 64-bit value.
 */
uint64_t Reverse_64bit(uint64_t input_number);

/**
 * @brief Prints byte array in hexadecimal format.
 *
 * @details Prints each byte of the input data in hexadecimal format.
 * A space is inserted after every 8 bytes for improved readability,
 * which is useful when debugging block-based cryptographic data.
 *
 * @param length Length of data in bytes.
 * @param data Pointer to byte array.
 */
void Test_Print(size_t length, const uint8_t *data);

/**
 * @brief Prints data given length in bits.
 *
 * @details Converts the given bit length into byte length and prints
 * the corresponding number of bytes in hexadecimal format.
 * This is typically used when test vectors specify sizes in bits.
 *
 * @param data Pointer to byte array.
 * @param length_in_bits Length of data in bits.
 */
void Print_Test_String(const uint8_t *data, size_t length_in_bits);

/**
 * @brief Prints byte array in selected endian format.
 *
 * @details Prints the given byte array in hexadecimal format based on
 * the selected endianness:
 *
 * - PRINT_LITTLE_ENDIAN: Prints bytes in forward order.
 * - PRINT_BIG_ENDIAN: Prints bytes in reverse order.
 *
 * @param name Label string printed before the data.
 * @param data Pointer to byte array.
 * @param len_bytes Length of data in bytes.
 * @param mode Endianness selection.
 */
void Print_uchar8(const char *name, const uint8_t *data,
                  size_t len_bytes, print_endian_t mode);

/**
 * @brief Prints 64-bit blocks (dwords) in selected endian format.
 *
 * @details Interprets the input buffer as a sequence of bytes (multiple
 * 64-bit blocks) and prints them in hexadecimal format. The output depends
 * on the selected endianness:
 *
 * - PRINT_LITTLE_ENDIAN: Prints bytes in memory order (forward), exactly
 *   as stored in the buffer. This reflects true little-endian representation.
 *
 * - PRINT_BIG_ENDIAN: Prints bytes in reverse order (from last byte to
 *   first byte), so that the most significant byte is printed first.
 *
 * @param name Label string printed before the data.
 * @param data Pointer to the byte array to print.
 * @param len_dword Number of 64-bit blocks (each block = 8 bytes).
 * @param mode Endianness selection: PRINT_LITTLE_ENDIAN or PRINT_BIG_ENDIAN.
 */
void Print_uchar64(const char *name, const uint8_t *data,
                   size_t len_dword, print_endian_t mode);

/**
 * @brief Converts hex string to byte array.
 *
 * @details Parses an ASCII hexadecimal string and converts it into a
 * corresponding binary byte array. Each pair of hex characters is
 * converted into one byte.
 *
 * The input size must be even and non-zero. Invalid characters will
 * cause the function to return an error.
 *
 * Example:
 * - Input : "A1B2"
 * - Output: {0xA1, 0xB2}
 *
 * @param hex_str Input null-terminated hex string.
 * @param byte_array Output buffer to store converted bytes.
 * @param size Input: length of hex string.
 *             Output: number of bytes written.
 *
 * @return `SUCCESS` on successful conversion.
 *         `EINVAL` if input length is invalid or contains non-hex characters.
 */
uint16_t Hex_String_To_Byte_Array(const char *hex_str, uint8_t *byte_array,
                                  size_t *size);

/**
 * @brief Polls a hardware status register bit in constant-time using mcycles.
 *
 * @details Always waits the full timeout duration regardless of when the
 * bit transitions, ensuring no timing side channel. The polling behaviour
 * depends on the polarity parameter:
 *
 * - CRYPTO_WAIT_ACTIVE_HIGH: status is accumulated using |= on each
 *   iteration. A non-zero accumulated value indicates the bit was seen
 *   set at least once within the timeout window.
 *
 * - CRYPTO_WAIT_ACTIVE_LOW: status holds the last reading on each
 *   iteration. A zero final value indicates the bit cleared within
 *   the timeout window.
 *
 * @param reg Pointer to the hardware status register to poll.
 * @param bit Bit mask to check within the register.
 * @param timeout Maximum mcycle count to wait.
 * @param polarity CRYPTO_WAIT_ACTIVE_HIGH or CRYPTO_WAIT_ACTIVE_LOW.
 *
 * @return `SUCCESS` if the bit transitioned as expected within the
 *          timeout window.
 *         `ETIMEDOUT` if the bit did not transition within the timeout
 *          window.
 */
uint16_t Crypto_Wait_For_Timeout(const volatile uint8_t *reg,
                                 uint8_t bit,
                                 uint64_t timeout,
                                 crypto_wait_polarity_t polarity);

#ifdef __cplusplus
}
#endif

#endif   // BSP_INCLUDE_CRYPTO_HELPERS_H_
