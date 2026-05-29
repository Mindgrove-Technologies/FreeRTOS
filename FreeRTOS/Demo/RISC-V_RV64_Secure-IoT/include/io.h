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
 * @file io.h
 * @brief Header file for standard library functions.
 * @details Provides function prototypes and definitions required for
 *          standard library utility functions.
 * @version 1.1
 * @authors Kapil Shyam. M (kapil@mindgrovetech.in),
 *          Harini P (harinip@mindgrovetech.in)
 * @date 18-12-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 21-09-2024 | 1.0     | Kapil Shyam. M        | Initial release.              
 * 18-12-2025 | 1.1     | Harini P              | Updated the IO library header 
 *            |         |                       | for MISRA-C compliance
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_IO_H_
#define BSP_INCLUDE_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <ctype.h>

/**
 * @brief Internal formatted output implementation.
 *
 * @details Parses the format string and prints formatted output
 *          to the standard output using putchar().
 *          Supports character, string, signed/unsigned integers,
 *          hexadecimal, octal, float, zero padding and precision handling.
 *
 * @param fmt Format control string.
 * @param ap  Variable argument list containing values to print.
 *
 * @return None.
 */
void _print_(const char *fmt, va_list ap);

/**
 * @brief Prints formatted output to the standard output.
 *
 * @details Wrapper function over _print_(). Processes the format
 *          string and corresponding arguments, then outputs the
 *          formatted result to the console.
 *
 * @param fmt Format control string.
 * @param ... Variable arguments corresponding to format specifiers.
 *
 * @return Returns SUCCESS if successfully printed the output.
 */
int printf(const char *fmt, ...);

/**
 * @brief Reads formatted input from the standard input.
 *
 * @details Parses input from the console according to the
 *          specified format string. Supports string, character,
 *          decimal, float, hexadecimal, octal and unsigned conversions.
 *
 * @param format Format string specifying expected input pattern.
 * @param ...    Pointers to variables where parsed values are stored.
 *
 * @return Returns SUCCESS on successful parsing.
 *         Returns error code on failure.
 */
int scanf(const char* format, ...);

/**
 * @brief Reads formatted data from a string buffer.
 *
 * @details Parses the input buffer according to the given format
 *          string and stores extracted values into provided variables.
 *          Supports integer, string, character, pointer and scanset
 *          conversions with width and length modifiers.
 *
 * @param buf Input string buffer to parse.
 * @param fmt Format string specifying parsing rules.
 * @param ... Pointers to variables where extracted values are stored.
 *
 * @return Number of successfully assigned input items.
 *         Returns -1 if input failure occurs before any conversion.
 */
int sscanf(const char *buf, const char *fmt, ...);

// int sprintf(char *buffer, const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_IO_H_
