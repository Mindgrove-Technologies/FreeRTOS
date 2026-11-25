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
 * @file   bignum.h
 * @brief  This is the Big Number Integer Library header file for multi-precision
 *         arithmetic.
 * @details Provides definitions and prototypes to perform large integer
 *          operations beyond native data type limits.
 * @version 1.0
 * @authors Harini P (harinip@mindgrovetech.in)
 *          Natarajan S (natarajan@mindgrovetech.in)
 * @date 23-07-2026 
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 26-05-2026 | 1.0     | Harini P              | Initial release.              
 * 23-07-2026 | 1.1     | Natarajan S           | Added montgomery domain
 *            |         |                       | functions required for ECC
 * -----------------------------------------------------------------------------
 */

#ifndef INCLUDES_BIGNUM_H_
#define INCLUDES_BIGNUM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "memory_functions.h"
#include "crypto_api.h"

/**
 * @defgroup BIGNUM_SIZE_MACROS Big Number Size Macros
 * @brief Macros defining digit size and storage capacity.
 * @{
 */

/** @brief Maximum number of digits (limbs) supported. */
#define BIGNUM_SIZE   34U

/** @brief Number of bits per digit (limb). */
#define DIGIT_BITS     64U

/** @brief  Number of small primes in the sieve. */
#define PRIME_SIZE    256U
/** @} */

/**
 * @defgroup BIGNUM_SIGN_MACROS Big Number Sign Macros
 * @brief Macros representing the sign of a big number.
 * @{
 */

/** @brief Represents zero or positive sign. */
#define ZERO_POSITIVE   0U

/** @brief Represents negative sign. */
#define NEGATIVE        1U
/** @} */

/**
 * @defgroup BIGNUM_COMPARISON_MACROS Big Number Comparison Macros
 * @brief Macros defining comparison and boolean results.
 * @{
 */

/** @brief Boolean YES value. */
#define YES             1

/** @brief Boolean NO value. */
#define NO              0

/** @brief Indicates equality result. */
#define EQUAL_TO        0

/** @brief Indicates less-than result. */
#define LESS_THAN      -1

/** @brief Indicates greater-than result. */
#define GREATER_THAN    1
/** @} */

/**
 * @defgroup BIGNUM_UTILITY_MACROS Big Number Utility Macros
 * @brief Macros for initialization and basic operations.
 * @{
 */

/** @brief Initializes a big number structure to zero. */
#define BN_INIT(a)      (void *)memset_opt((a), 0, sizeof(bn_int))

/** @brief Sets a big number to zero. */
#define BN_ZERO(a)      BN_INIT(a)

/** @brief Checks if the big number is zero. */
#define BN_IS_ZERO(a)   (((a)->used == 0U) ? YES : NO)

/** @brief Frees the big number resources and resets it. */
#define BN_FREE(a)      do {               \
                            BN_ZERO(a);    \
                            (a)->used = 0; \
                        } while (0)

/** @brief Checks if the big number is even. */
#define BN_IS_EVEN(a)   ((((a)->used == 0U) || (((a)->dp[0] & 1U) == 0U)) \
                        ? YES : NO)

/** @brief Checks if the big number is odd. */
#define BN_IS_ODD(a)    ((((a)->used != 0U) && (((a)->dp[0] & 1U) != 0U)) \
                        ? YES : NO)

/** @brief Removes leading zero digits from the big number. */
#define BN_CLAMP(a)     do {                                             \
                            while (((a)->used > 0U) &&                   \
                                ((a)->dp[(a)->used - 1U] == 0U)) {       \
                                --((a)->used);                           \
                            }                                            \
                        } while (0)

/** @brief Copies one big number into another. */
#define BN_COPY(a, b)   do {                                             \
                            if ((a) != (b)) {                            \
                                (void)memcpy((b), (a), sizeof(bn_int));  \
                            }                                            \
                        } while (0)

/** @brief Computes absolute value of a big number. */
#define BN_ABS(a, b)    { BN_COPY(a, b); (b)->sign = ZERO_POSITIVE; }
/** @} */

/**
 * @brief Defines the digit type for big numbers.
 *
 * @details Each big integer is stored as an array of 64-bit unsigned
 *          digits (limbs) using this type.
 */
typedef uint64_t bn_digit;

/**
 * @brief Array of small prime numbers.
 *
 * @details This constant array stores predefined prime numbers
 *          that are typically used for prime checking in big number
 *          operations.
 */
extern const uint16_t primes[PRIME_SIZE];

/**
 * @brief Big integer structure.
 *
 * @details Represents a multi-precision integer using an array of
 *          fixed-size digits. It stores the digit array, the number
 *          of active digits, and the sign of the value.
 */
typedef struct {
    /** Array of digits (least significant limb at index 0) */
    bn_digit dp[BIGNUM_SIZE];

    /** Number of active digits currently used (0 indicates the value is zero)*/
    uint16_t used;

    /** Sign of the integer (POSITIVE (include zero) or NEGATIVE) */
    uint8_t sign;
} bn_int;

/* Function prototypes */

/**
 * @brief Optimized memory set function.
 * 
 * @details Sets a block of memory to a given value using 64-bit operations
 *          for improved performance.
 * 
 * @param ptr Pointer to memory block.
 * @param value Value to set.
 * @param n Number of bytes to set.
 * 
 * @return Pointer to the memory block.
 */
void *memset_opt(void *ptr, int value, size_t n);

/**
 * @brief Prints a big integer in hexadecimal format.
 * 
 * @details This function prints the given big integer in hexadecimal
 *          representation, starting from the most significant limb.
 *          If the value is zero, it prints 0.
 * 
 * @param a Pointer to the bn_int structure to be printed.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if the input pointer is NULL.
 */
uint16_t BigNum_Print_Int_to_Hex(const bn_int *a);

/**
 * @brief Sets a big integer to a single digit value.
 * 
 * @details This function initializes the big integer to the specified
 *          digit value.
 * 
 * @param a Pointer to the bn_int to be initialized.
 * @param d Digit value to assign.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if the input pointer is NULL.
 */
uint16_t BigNum_Set_Digit(bn_int *a, bn_digit d);

/**
 * @brief Performs right shift on a big integer.
 * 
 * @details Shifts the big integer right by \a n bits and updates
 *          the used length. If the shift exceeds the value size,
 *          the result becomes zero.
 * 
 * @param a Pointer to the bn_int to be shifted.
 * @param n Number of bits to shift.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if the input pointer is NULL.
 */
uint16_t BigNum_Right_Shift(bn_int *a, uint16_t n);

/**
 * @brief Performs left shift operation on a big integer.
 * 
 * @details This function shifts the big integer left by n bits.
 *          It handles both limb-level and bit-level shifts and
 *          updates the used length accordingly.
 * 
 * @param a Pointer to the bn_int structure to be shifted.
 * @param n Number of bits to shift.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if the input pointer is NULL.
 */
uint16_t BigNum_Left_Shift(bn_int *a, uint16_t n);

 /**
 * @brief Performs constant-time XOR operation on two big integers.
 * 
 * @details This function computes the bitwise XOR of two big integers
 *          for the specified length and stores the result in \a result.
 * 
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 * @param len Number of digits to process.
 * @param result Pointer to store the XOR result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_XOR_CT(const bn_int* a, const bn_int* b, size_t len, \
                                                         bn_int* result);

/**
 * @brief Computes modulus of two big integers.
 * 
 * @details This function computes modulus of two big integers (a % b)
 *          using bit-wise long division from MSB to LSB.
 * 
 * @param a Pointer to dividend.
 * @param b Pointer to divisor.
 * @param rem Pointer to result (remainder).
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */

uint16_t BigNum_Mod(const bn_int *a, const bn_int *b, bn_int *rem);

/**
 * @brief Computes modulus of a big integer with a single digit.
 * 
 * @details This function computes modulus of a big integers and a digit,
 *          where \a a is a big integer and \a b is a single digit.
 * 
 * @param a         Pointer to dividend.
 * @param digit     Single digit divisor.
 * @param rem       Pointer to store the remainder.
 * 
 * @return Returns a 16-bit status code: 
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if the divisor is zero
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Mod_Digit(const bn_int *a, bn_digit digit, bn_digit *rem);

/**
 * @brief Computes R^2 mod n.
 * 
 * @details This function computes 2^4096 mod n using repeated
 *          left shifts and modular reduction at each step.
 * 
 * @param n Pointer to modulus.
 * @param result Pointer to store computed result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Calculate_R2_Mod_N(const bn_int *n, bn_int *result);

/**
 * @brief Compares two big integers.
 * 
 * @details This function compares two big integers and determines
 *          whether one is greater than, less than, or equal to the other.
 * 
 * @param a Pointer to first operand.
 * @param b Pointer to second operand.
 * 
 * @return Returns 1 if GREATER_THAN or -1 if LESS_THAN or 0 if EQUAL_TO.
 */
int8_t BigNum_Compare(const bn_int *value1, const bn_int *value2);

/**
 * @brief Compares a big integer with a single digit.
 * 
 * @details This function compares the big integer with a given
 *          digit value.
 * 
 * @param a     Pointer to big integer.
 * @param digit Digit value for comparison.
 * 
 * @return Returns 1 if GREATER_THAN or -1 if LESS_THAN or 0 if EQUAL_TO.
 */
int8_t BigNum_Compare_Digit(const bn_int *a, bn_digit digit);

/**
 * @brief Subtracts two big integers.
 * 
 * @details This function computes subtraction of two big integers
 *          (a - b) assuming a >= b. Borrow handling is performed
 *          using extended precision.
 * 
 * @param a Pointer to minuend.
 * @param b Pointer to subtrahend.
 * @param result Pointer to result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if a less than b.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Subtract(const bn_int *a, const bn_int *b, bn_int *result);

/**
 * @brief Subtracts a single digit from a big integer.
 * 
 * @details This function computes the subtraction of a big integer
 *          and a single digit (a - digit). Borrow handling is
 *          performed internally using extended precision.
 * 
 * @param a      Pointer to the minuend.
 * @param digit  Single digit to be subtracted.
 * @param result Pointer to the result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if a less than digit.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Subtract_Digit(const bn_int *a, bn_digit digit, bn_int *result);

/**
 * @brief Performs constant-time modular subtraction of two big integers.
 * 
 * @details This function computes the modular subtraction
 *          ((a - b) % mod) in constant time. The inputs should
 *          satisfy (a < mod) and (b < mod).
 * 
 * @param a Pointer to the first operand.
 * @param b Pointer to the second operand.
 * @param mod Pointer to the modulus value.
 * @param result Pointer to store the result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_SubMod_CT(const bn_int* a, const bn_int* b, const bn_int* mod, \
                                                            bn_int* result);

/**
 * @brief Adds two big integers.
 * 
 * @details Computes the addition operation for two bignum integers 
 *          with proper carry handling and store it in \a result.
 * 
 * @param a      Pointer to first operand.
 * @param b      Pointer to second operand.
 * @param result Pointer to result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Addition(const bn_int *a, const bn_int *b, bn_int *result);

/**
 * @brief Performs constant-time modular addition of two big integers.
 * 
 * @details This function computes the modular addition
 *          (a + b) % mod in constant time. The inputs should
 *          satisfy (a < mod) and (b < mod).
 * 
 * @param a      Pointer to the first operand.
 * @param b      Pointer to the second operand.
 * @param mod    Pointer to the modulus value.
 * @param result Pointer to store the result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if the any pointer is NULL.
 */
uint16_t BigNum_AddMod_CT(const bn_int* a, const bn_int* b, const bn_int* mod, \
                                                            bn_int* result);

/**
 * @brief Divides two big integers.
 * 
 * @details This function performs the division of big integer \a a by
 *          \a b and stores the quotient in \a quot and the remainder
 *          in \a rem . If only the quotient is required, \a rem can 
 *          be passed as NULL.
 * 
 * @param a    Pointer to the dividend.
 * @param b    Pointer to the divisor.
 * @param quot Pointer to store the quotient.
 * @param rem  Pointer to store the remainder.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if divisor is zero.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Divide(const bn_int *a, const bn_int *b, bn_int *quot, \
                                                         bn_int *rem);

/**
 * @brief Divides a big integer by 2^shift.
 * 
 * @details This function shifts the big integer right by the specified
 *          number of bits and stores the result in \a c.
 * 
 * @param a     Pointer to the input big integer.
 * @param shift Number of bits to shift.
 * @param c     Pointer to store the result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Divide_2d(const bn_int *a, uint16_t shift, bn_int *result);

/**
 * @brief Performs constant-time modular division by 2.
 * 
 * @details This function performs the modular division of the
 *          input value by 2 under the given modulus in constant
 *          time. The modulus must be an odd value.
 * 
 * @param a      Pointer to the input value.
 * @param mod    Pointer to the modulus value.
 * @param result Pointer to store the computed result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Div2Mod_CT(const bn_int* a, const bn_int* mod, bn_int* result);

/**
 * @brief Multiplies two big integers.
 * 
 * @details Computes product of two bignum integer with proper carry
 *          handling and also updates the used length.
 * 
 * @param a       Pointer to first operand.
 * @param b       Pointer to second operand.
 * @param result  Pointer to result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Multiply(const bn_int *a, const bn_int *b, bn_int *result);

/**
 * @brief Performs modular multiplication of two big integers.
 *
 * @details This function computes the modular multiplication of two big
 *          integers by multiplying a and b, and then taking the result
 *          modulo mod.
 *
 * @param a   Pointer to the first operand.
 * @param b   Pointer to the second operand.
 * @param mod Pointer to the modulus value.
 * @param res Pointer to store the result.
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_MulMod(const bn_int* a, const bn_int* b, const bn_int* mod, \
                                                         bn_int* res);

/**
 * @brief Computes the square of a big integer.
 * 
 * @details This function calculates the square of the input big integer
 *          and stores the result in the provided result structure.
 * 
 * @param base   Pointer to the input big integer.
 * @param result Pointer to store the squared result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Square(const bn_int* base, bn_int* result);

/**
 * @brief Computes the GCD of two big integers using the binary method.
 * 
 * @details This function calculates the greatest common divisor (GCD)
 *          of two bignum integers using the binary GCD algorithm.
 * 
 * @param a   Pointer to the first input value.
 * @param b   Pointer to the second input value.
 * @param res Pointer to store the GCD result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_GCD_Binary(const bn_int *a, const bn_int *b, bn_int *res);

/**
 * @brief Computes the LCM of two big integers.
 * 
 * @details This function calculates the least common multiple (LCM)
 *          of two bignum integers and stores the result in \a res.
 * 
 * @param a   Pointer to the first input value.
 * @param b   Pointer to the second input value.
 * @param res Pointer to store the LCM result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EDOM if GCD is zero.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_LCM(const bn_int *a, const bn_int *b, bn_int *res);

/**
 * @brief Performs modular exponentiation on big integers.
 *
 * @details This function computes the modular exponentiation of
 *          the given big integers ((base^exp) % mod) and stores
 *          the result in \a result.
 *
 * @param base   Pointer to the base value.
 * @param exp    Pointer to the exponent value.
 * @param mod    Pointer to the modulus value.
 * @param result Pointer to store the computed result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EDOM if an internal computation exceeds supported limits
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t  BigNum_Exptmod(const bn_int *base, const bn_int *exp, \
                         const bn_int *mod, bn_int *result);

/**
 * @brief Computes the modular inverse of a big integer.
 * 
 * @details This function calculates the modular inverse of \a a
 *          with respect to modulus \a b and stores the result in
 *          \a res such that (a × res) % b = 1.
 * 
 * @param a Pointer to the input value.
 * @param b Pointer to the modulus.
 * @param res Pointer to store the modular inverse result.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EDOM if the inverse does not exist
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Invmod(const bn_int *a, const bn_int *b, bn_int *res);

/**
 * @brief Checks whether a big integer is prime.
 * 
 * @details This function performs a primality test on the given
 *          big integer using trial checks and probabilistic testing
 *          with the specified number of rounds. The result is
 *          returned through the output parameter.
 * 
 * @param num     Pointer to the big integer to be tested.
 * @param rounds  Number of test iterations to perform. If 0 is provided,
 *                a default minimum of 1 round is used, and the maximum
 *                allowed is 12 rounds.
 * @param isprime Pointer to store the result (YES or NO).
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EDOM if an internal computation exceeds supported limits
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Is_Prime(bn_int *num, uint8_t rounds, bool *isprime);

/**
 * @brief Returns the byte size of a big integer.
 *
 * @details Calculates the byte size of the big integer in unsigned
 *          binary format. The size will be 1 if the big integer is zero.
 *
 * @param a    Pointer to the source big integer.
 * @param size Pointer to a size_t variable that will hold the computed
 *             byte size on success.
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Unsigned_Bin_Size(const bn_int *a, size_t *size);

/**
 * @brief Reads an unsigned binary buffer into a big integer.
 * 
 * @details This function converts a big-endian byte array into
 *          an internal bn_int representation.
 * 
 * @param a Pointer to big integer to be populated.
 * @param byte_array Pointer to input byte buffer.
 * @param len Length of input buffer.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Read_Unsigned_Bin(bn_int *a, const uint8_t *byte_array,
                                                   uint16_t len);

/**
 * @brief Writes a big integer to an unsigned binary buffer.
 * 
 * @details This function converts the big integer into a big-endian
 *          byte array representation.
 * 
 * @param a Pointer to big integer.
 * @param byte_array Pointer to output buffer.
 * @param len Size of output buffer in bytes.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if buffer is too small than the size of bignum int.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Write_Unsigned_Bin(const bn_int *a, uint8_t *byte_array,
                                                    uint16_t len);

/**
 * @brief Writes a big integer to a fixed-size unsigned binary buffer.
 * 
 * @details This function converts the given big integer into a big-endian
 *          byte array and writes it into the specified output buffer.
 *          If the output buffer is larger than the number, the remaining
 *          bytes are padded with leading zeros. If the buffer size is
 *          smaller, only the least significant bytes (LSB) are stored.
 * 
 * @param a Pointer to the input big integer.
 * @param byte_array Pointer to the output buffer.
 * @param out_size Size of the output buffer in bytes.
 * 
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if output buffer size is zero
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Write_Unsigned_Bin_Len(const bn_int *a, uint8_t *byte_array,
                                                        uint16_t out_size);

/**
 * @brief Counts the number of significant bits in a big integer.
 *
 * @details This function computes the bit length of the given big integer
 *          by first accounting for all fully occupied limbs, then counting
 *          the remaining bits in the most significant limb by right-shifting
 *          until it reaches zero.
 *
 * @param a Pointer to the input big integer.
 *
 * @return Returns 0 if the big integer is zero, otherwise returns the
 *         number of significant bits in a.
 */
uint32_t BigNum_Count_Bits(const bn_int *a);

/**
 * @brief Computes the Montgomery reduction constant mp = -m^(-1) mod 2^w.
 *
 * @details Calculates the Montgomery constant required by BigNum_Mont_Reduce().
 *          The modular inverse is computed using 5 Newton-Raphson iterations,
 *          sufficient for a 64-bit word size.
 *
 * @param m  Pointer to the modulus. Must be a non-zero odd integer.
 * @param mp Pointer to the output location that receives the computed
 *           Montgomery constant.
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if m is zero or even.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Mont_Setup(const bn_int *m, bn_digit *mp);

/**
 * @brief Computes the Montgomery normalisation value R mod m,
 *        where R = 2^(bit-length of m).
 *
 * @details If the bit length of m is an exact multiple of the word size,
 *          BigNum_Mod() is used; otherwise the result is computed via a
 *          direct subtraction. The output is zero-padded to match the
 *          limb count of m.
 *
 * @param norm Pointer to the output big integer that receives R mod m.
 * @param m    Pointer to the modulus. Must be non-zero.
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if m is zero or if R exceeds BIGNUM_SIZE.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Mont_Norm(bn_int *norm, const bn_int *m);

/**
 * @brief Performs in-place Montgomery reduction, computing a = a * R^(-1) mod m
 *
 * @details On entry a must satisfy 0 <= a < m*R. The final conditional
 *          subtraction uses bitmask selection to avoid secret-dependent
 *          branches. Non-power-of-two moduli are handled via a right-shift
 *          extraction step after the main reduction loop.
 *
 * @param a  Pointer to the value to reduce in place. On entry must have
 *           at most 2 * m->used limbs; holds the reduced result on exit.
 * @param m  Pointer to the modulus. Must be non-zero and odd, and
 *           2 * m->used must not exceed BIGNUM_SIZE.
 * @param mp Montgomery constant for m, as returned by BigNum_Mont_Setup().
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if m is zero or 2 * m->used exceeds BIGNUM_SIZE.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Mont_Reduce(bn_int *a, const bn_int *m, bn_digit mp);

/**
 * @brief Computes the modular inverse r = a^(-1) mod m using sliding-window
 *        exponentiation in Montgomery form.
 *
 * @details Evaluates a^(m-2) mod m via Fermat's little theorem, requiring
 *          m to be prime. A sliding window of up to BN_CT_INV_PRE_CNT (8)
 *          bits is used over a pre-computed table of odd powers of a, with
 *          Montgomery reduction applied after each squaring and multiplication.
 *
 * @param a  Pointer to the base value in Montgomery form.
 * @param m  Pointer to the prime modulus.
 * @param r  Pointer to the output big integer that receives a^(-1) mod m
 *           in Montgomery form.
 * @param mp Montgomery constant for m, as returned by BigNum_Mont_Setup().
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EINVAL if the exponent m-2 has fewer than 2 bits.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_Invmod_Mont_CT(const bn_int *a, const bn_int *m,
                                bn_int *r, bn_digit mp);

/**
 * @brief Conditionally swaps two big integers in constant time.
 *
 * @details Swaps the limb arrays and used fields of a and b if and only
 *          if swap is non-zero, using bitmask arithmetic throughout.
 *          All BIGNUM_SIZE limbs are always processed regardless of the
 *          swap condition, preventing data-dependent memory access patterns.
 *
 * @param a    Pointer to the first big integer operand.
 * @param b    Pointer to the second big integer operand.
 * @param swap If the least-significant bit is 1, a and b are exchanged;
 *             if 0, they are left unchanged.
 *
 * @return Returns a 16-bit status code:
 *  - @ref SUCCESS if the operation completes successfully.
 *  - @ref EFAULT if any pointer is NULL.
 */
uint16_t BigNum_CondSwapCT(bn_int *a, bn_int *b, uint8_t swap);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_BIGNUM_H_
