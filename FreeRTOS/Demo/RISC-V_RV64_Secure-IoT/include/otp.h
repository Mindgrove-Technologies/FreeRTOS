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
 * @file otp.h
 * @brief This is a Baremetal OTP Driver header file for OTP.
 * @details Provides an API to read from and write to OTP memory 
 *          and includes additional functions for validating
 *          test patterns.
 * @version 1.1
 * @authors Rohit Srinivas R G (rohit@mindgrovetech.in)
 *          Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 * @date 16-04-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 13-06-2024 | 1.0     | Rohit Srinivas R G    | Initial release.
 * 16-04-2025 | 1.1     | Vignesh Kumar J       | Added test pattern and OTP
 *            |         |                       | lock function. Updated the
 *            |         |                       | driver for MISRA-C compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_OTP_H_
#define BSP_INCLUDE_OTP_H_

#include "io.h"
#include "perf_monitors.h"
#include "errors.h"
#include "log.h"
#include "secure_iot.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum OTP_LOCK
 * @brief OTP lock configuration values.
 *
 * Encoding:
 * - 0 : Write operations are locked (prohibited)
 * - 1 : Write and read operations are locked (prohibited)
 *
 * These values must be passed as the @p lock parameter in OTP_Lock()
 */
typedef enum {
    WRITE_LOCK          = 0,  /**< Write operations prohibited */
    WRITE_AND_READ_LOCK = 1   /**< Write and read operations prohibited */
}OTP_LOCK;

/**
 * @brief Reads data from a specified address in OTP memory.
 *
 * @details The function reads the specified number of bytes from the OTP
 *          (One-Time Programmable) memory starting from the given address.
 *
 * @param address Pointer to the starting address in the OTP memory from which the data is to be read.
 * 
 * @param bytes   Total number of bytes to read from the OTP memory.
 * 
 * @param data    Pointer to the buffer where the read data will be stored.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */
uint16_t OTP_Read(const uintptr_t *address, size_t bytes, uint8_t *data);

/**
 * @brief Writes data to a specified address in OTP memory, bit by bit.
 *
 * @details The function writes the specified number of bits from the provided
 *          data to the OTP (One-Time Programmable) memory starting from the
 *          given address.
 *
 * @param address  Pointer to the starting address in the OTP memory where the data is to be written.
 * 
 * @param num_bits Number of bits to be written to the OTP memory starting from the specified address.
 * 
 * @param data     Pointer to the integer value containing the data to be programmed into the OTP memory.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */
uint16_t OTP_Write(const uintptr_t *address, size_t num_bits, uint8_t *data);

/**
 * @brief Performs the TestDEC test pattern on OTP memory.
 *
 * @details The function executes the TestDEC test pattern, enabling users to
 *          verify the integrity of word-lines and bit-lines, as well as screen
 *          out any gross defects in the peripheral logic.
 *
 * @param address Pointer to the input address used for the TestDEC pattern.
 *                Expected input conditions:
 *                - If address A[3] = 0, A[10] = 0 and the rest are don't-cares,
 *                  the expected output data is 8'h55.
 *                - If address A[3] = 0, A[10] = 1 and the rest are don’t-cares,
 *                  the expected output data is 8’hAA.
 *                - If address A[3] = 1, A[10] = 0 and the rest are don’t-cares,
 *                  the expected output data is 8’hAA.
 *                - If address A[3] = 1, A[10] = 1 and the rest are don’t-cares,
 *                  the expected output data is 8’h55.
 *
 * @param data    Pointer to the buffer where the output test pattern result
 *                will be stored.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */
uint16_t OTP_TESTDEC_Test(const uintptr_t *address, uint8_t *data);

/**
 * @brief Performs the WRTEST test on OTP memory programming circuitry.
 *
 * @details The function executes the WRTEST Test Mode, enabling users to
 *          screen out gross defects in the programming circuitry before
 *          the actual XPM memory array is programmed.
 *
 * @param address Pointer to the input address of the XPM memory to be
 *                tested in WRTEST Mode.
 *
 * @param status  Pointer to the buffer where the WRTEST result will be
 *                stored.
 *                Expected output conditions:
 *                - 0 : Pass (Programming operation successful).
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */
uint16_t OTP_WRTEST_Test(const uintptr_t *address, uint8_t *status);

/**
 * @brief Reads the 128-bit CPU ID from OTP memory.
 *
 * @details The function reads the 128-bit CPU ID from the OTP memory and
 *          stores the result in the provided buffer.
 *
 * @param data Pointer to the buffer where the 128-bit CPU ID will be stored.
 *             The buffer must be at least 16 bytes in size to hold the
 *             complete 128-bit CPU ID.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */
uint16_t OTP_Read_CPUID(uint8_t *data);

/**
 * @brief  Locks the OTP memory region based on the specified lock mode.
 *
 * @param[in] lock  is Lock mode to apply to the OTP memory.
 *                  Must be one of @ref OTP_LOCK :
 *                  - @ref WRITE_LOCK          : Prohibits write operations
 *                  - @ref WRITE_AND_READ_LOCK : Prohibits write and read operations
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 *
 * @note   Once locked, the OTP region cannot be unlocked.
 *         Ensure the correct lock mode is selected before calling this function.
 *
 * @warning Applying @ref WRITE_AND_READ_LOCK permanently restricts
 *          read access. This action is irreversible.
 */
uint16_t OTP_Lock(OTP_LOCK lock);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_OTP_H_
