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
 * @file string_functions.h
 * @brief Header file for strings related functions.
 * @details Functions like HextoInt Conversion
 * @version 1.0
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in)
 * @date 21-09-2024 
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 21-09-2024 | 1.0     | Kapil Shyam. M        | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef STRING_FUNTIONS_h
#define STRING_FUNTIONS_h

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <ctype.h>


#define isalpha(c)    (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define isupper(c)    ((c) >= 'A' && (c) <= 'Z')
#define islower(c)    ((c) >= 'a' && (c) <= 'z')
#define isdigit(c)    ((c) >= '0' && (c) <= '9')
#define isxdigit(c)   (isdigit(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#define isspace(c)    ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\v' || (c) == '\f' || (c) == '\r')
#define ispunct(c)    (((c) >= 33 && (c) <= 47) || ((c) >= 58 && (c) <= 64) || \
                       ((c) >= 91 && (c) <= 96) || ((c) >= 123 && (c) <= 126))
#define isalnum(c)    (isalpha(c) || isdigit(c))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct str_info - Input string parameters
 * @neg: negative number or not
 *	 0 - not negative
 *	 1 - negative
 * @any: set any if any `digits' consumed; make it negative to indicate
 *	 overflow
 * @acc: accumulated value
 */
struct str_info {
	int neg;
	int any;
	unsigned int acc;
};

/**
 * @brief Converts a string to an integer.
 * 
 * @details This function takes a string representation of an integer and converts it to an
 * integer value. It handles optional leading whitespace, a leading sign (+ or -),
 * and ensures that the result does not overflow the range of a standard integer.
 * 
 * @param str A pointer to the null-terminated string to be converted.
 *            The string may contain leading whitespace and an optional
 *            sign. It should only contain digits after the optional sign.
 * 
 * @return The integer representation of the string. If the string is not a valid
 *         integer or exceeds the range of an integer, the function returns
 *         INT_MAX or INT_MIN.
 * 
 * @note The function does not handle non-numeric characters gracefully. If
 *       the first non-whitespace character is not a valid digit or sign, the
 *       function will return 0.
 * 
 * @warning This function does not perform error handling for invalid input
 *          beyond the overflow checks. It assumes the input will be a valid
 *          representation of an integer or a whitespace-padded string.
 * 
 * @see StrToInt, StrToLong
 */
int StrInt(const char *str);

/**
 * @brief Compares two strings.
 *
 * @details function compares two null-terminated strings lexicographically.
 * It behaves similarly to the standard `strcmp` function.
 *
 * @param str1 Pointer to the first string to compare.
 * @param str2 Pointer to the second string to compare.
 * 
 * @return 
 *  - A negative value if `str1` is less than `str2`.
 *  - Zero if `str1` is equal to `str2`.
 *  - A positive value if `str1` is greater than `str2`.
 *
 * @note The comparison is done using unsigned characters to avoid issues
 * with negative values in character representation.
 */
int StrCmp(const char *str1, const char *str2);

/**
 * @brief Reverse a string and store in the same string pointer.
 * 
 * @param char *str
 * @param int length
 */
void StrReverse(char *str, int len);

/**
 * @brief convert decimal numbers to string
 * 
 * @details Takes num as input and converts it to string. The converted
 *  string is stored in str. The position of last character in the str is 
 * returned.This function is tailored to support ftoa.
 * 
 * @param int number
 * @param char str[]
 * @param int afterpoint
 * 
 * @return int The position of last character in the str is returned.
 */
int IntToStr(int number, char str[], unsigned int afterpoint);

/**
 * @brief Converts a floating-point number to a string and prints it to uart
 * 
 * @param f The floating-point number to be converted
 * @param precision The number of digits to be printed after the decimal point
 * 
 * @return None : The number is printed while in the function
 */
void FloatToStr(double, int, char) ;

/**
 * @brief The function "atox" converts a hexadecimal string to an integer value.
 * 
 * @param p The parameter `p` is a character array that represents a hexadecimal number.
 * @param pos The parameter "pos" represents the position of the character in the string "p" that we
 * want to convert to an integer.
 * 
 * @return The converted hexadecimal value of the input string.
 */
int HexToInt(const char p[], int pos);

/**
 * @brief Converts a string to a long integer
 * 
 * @param nptr A pointer to the string to be converted
 * @param endptr If not NULL, stores the address of the first invalid character in nptr
 * @param base The base of the number represented in nptr
 * 
 * @return The converted long int
 */
long StrToLong(const char *nptr, char **endptr, int base);

/**
 * @brief Converts a string to a float
 * 
 * @param nptr A pointer to the string to be converted
 * @param endptr If not NULL, stores the address of the first invalid character in nptr
 * 
 * @return The converted float
 */
float StrToFloat(const char* str, char** endptr);

/**
 * @brief Write string data to structure
 * 
 * @nptr: pointer to string
 * @base: number's base
 * @unsign: describes what integer is expected
 *	    0 - not unsigned
 *	    1 - unsigned
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 *
 * @return struct str_info *, which contains string data to future process
 */
struct str_info *StrToInt(const char **nptr, int base, unsigned int unsign);

/**
 * @brief Convert a string to a quad integer
 * 
 * @nptr: pointer to string
 * @endptr: pointer to number's end in the string
 * @base: number's base
 *
 * @return s64 quad integer number converted from input string
 */
signed int StrToQInt(const char *nptr, char **endptr, int base);

/**
 * @brief Convert a string to an unsigned quad integer
 * 
 * @nptr: pointer to string
 * @endptr: pointer to number's end in the string
 * @base: number's base
 *
 * @return s64 unsigned quad integer number converted from
 *         input string
 */
unsigned int StrToUQInt(const char *nptr, char **endptr, int base);

/**
 * @brief Locates a substring within a string.
 * 
 * @param haystack Pointer to the null-terminated string to be scanned.
 * @param needle Pointer to the null-terminated substring to search for.
 * 
 * @return Pointer to the first occurrence of `needle` in `haystack`,
 *         or NULL if `needle` is not found.
 * 
 * @note If `needle` is an empty string, `haystack` is returned.
 */
char *StrStr(const char *haystack, const char *needle);

/**
 * @brief Finds the first occurrence of a character in a string.
 * 
 * @param str Pointer to the null-terminated input string to be scanned.
 * @param c Character to be located (interpreted as unsigned char).
 * 
 * @return Pointer to the first occurrence of the character in the string,
 *         or NULL if the character is not found.
 */
char *StrChr(const char *str, int c);

/**
 * @brief Copies up to n characters from the source string to destination.
 * 
 * @param dest Pointer to the destination array where the content is to be copied.
 * @param src Pointer to the source of data to be copied.
 * @param n Maximum number of characters to copy.
 * 
 * @return Pointer to the destination string `dest`.
 * 
 * @note If the length of `src` is less than `n`, the remainder of `dest` 
 *       will be padded with null bytes.
 */
char *StrnCpy(char *dest, const char *src, unsigned long n);

/**
 * @brief Converts an integer to a string representation.
 * 
 * @param value The integer value to convert.
 * @param str Pointer to the output string buffer.
 * @param is_unsigned Non-zero for unsigned conversion, zero for signed.
 * @param is_hex Non-zero to format the output as hexadecimal.
 * 
 * @return Length of the resulting string.
 * 
 * @note The output string is null-terminated.
 */
int IntToZeroPaddedStr(int value, char *str, int is_unsigned, int is_hex);

/**
 * @brief Computes the length of the string.
 * 
 * @param str Pointer to the null-terminated string.
 * 
 * @return The number of characters in the string excluding the null-terminator.
 */
size_t StrLen(const char *str);
#ifdef __cplusplus
}
#endif
#endif