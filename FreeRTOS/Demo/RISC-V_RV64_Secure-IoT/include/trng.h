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
 * @file trng.h
 * @brief Public APIs for TRNG (True Random Number Generator)
 * @details Declares APIs for generating true random numbers using VTRNG
 *          hardware.
 * @version 1.0
 * @authors Narasimha R V (narasimha@mindgrovetech.in)
 * @date 18-03-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by   | Description                   
 * -----------|---------|---------------|-------------------------------
 * 18-03-2026 | 1.0     | Narasimha R V | Initial TRNG header definition
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_TRNG_H_
#define BSP_INCLUDE_TRNG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @enum TRNG_IRQn_Type
 * 
 * @brief TRNG interrupt ID
 * 
 * This enumeration defines the available TRNG interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  TRNG_INTR_IRQn            =  69,              /*!< 69 TRNG_INTR_IRQn                                                         */
} TRNG_IRQn_Type;

/**
 * @brief Generate true random bytes using VTRNG hardware.
 *
 * This function triggers the TRNG hardware to generate random data and
 * reads the generated values from the VRAND registers. It supports
 * generating arbitrary-length random data by fetching multiple blocks
 * (each block = 16 bytes).
 *
 * The function operates in polling mode and waits until the TRNG hardware
 * indicates completion (BUSY bit cleared). A timeout mechanism is used to
 * prevent indefinite blocking.
 *
 * @param[out] random_bits     Pointer to buffer where random data will be 
 *                             stored.
 * @param[in]  req_num_bytes   Number of random bytes required.
 * @param[in]  vtrng           VTRNG channel no (valid values: 0 or 1).
 * @param[in]  timeout         Timeout for getting the data.
 *
 * @retval SUCCESS    Random data generated successfully.
 * @retval ECHRNG     Invalid VTRNG channel number.
 * @retval EFAULT     Null pointer passed for output buffer.
 * @retval ETIME      Timeout occurred while waiting for TRNG hardware.
 */
uint16_t TRNG_Generate(void *random_bits,
                       size_t req_num_bytes,
                       uint8_t vtrng,
                       uint64_t timeout);

/**
 * @brief Perform Known Answer Test (KAT).
 *
 * @details
 * This function performs the Known Answer Test (KAT)
 * supported by the NIST TRNG hardware.
 *
 * The TRNG supports KAT execution on:
 * - Deterministic Random Bit Generator (DRBG)
 * - Derivation function inside the entropy source
 *
 * Two different KAT vectors are supported for validation.
 *
 * The KAT configuration is controlled using:
 * - kat_sel : Selects the target block
 * - DRBG
 * - Derivation function
 *
 * - kat_vec : Selects the KAT test vector
 *
 * These selections are configured through the MODE register.
 *
 * The function performs KAT for all four possible
 * combinations of:
 * - KAT_SEL = 0, KAT_VEC = 0
 * - KAT_SEL = 0, KAT_VEC = 1
 * - KAT_SEL = 1, KAT_VEC = 0
 * - KAT_SEL = 1, KAT_VEC = 1
 *
 * If any KAT execution fails, the function returns an error.
 *
 * @note The hardware remains locked in test mode during KAT execution. 
 * A hardware reset is required before calling TRNG_Generate 
 * to restore the peripheral to normal operational mode.
 *
 * @return
 * - SUCCESS : All KAT combinations passed successfully
 * - ETIMEDOUT : KAT test failed.
 */
uint16_t TRNG_Full_KAT(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_TRNG_H_
