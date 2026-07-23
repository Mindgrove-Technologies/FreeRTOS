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
 * @copyright Copyright (c) 2026 Mindgrove Technologies.
 * All rights reserved.
 * @file pll.h
 * @brief Contains API for PLL configuration.
 * @details Provides API for hardware control of PLL.
 * @version 1.1
 * @authors Harini Sree.S
 * @date 2026-02-26
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 15-07-2024 | 1.0     | Suneeth               | Initial release.
 * 26-02-2026 | 1.1     | Harini Sree.S         | SecureIOT Production changes
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PLL_H_
#define BSP_INCLUDE_PLL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @enum PLL_frequency_Modes
 *
 * @brief Defines the available PLL clock frequency modes.
 *
 * @details This enumeration defines the available clock modes in PLL.
 */
typedef enum {
    /** Clock frequency 700MHz */
    PLL_CLK_700 = 0x00U,

    /** Clock frequency 350MHz */
    PLL_CLK_350 = 0x01U,

    /** Clock frequency 170MHz */
    PLL_CLK_175 = 0x02U,

    /** Clock frequency 35MHz */
    PLL_CLK_35 = 0x03U
} PLL_Frequency_Mode;

/**
 * @brief Changes the PLL operating frequency.
 *
 * @details Changes PLL operating frequency to 700MHz, 350MHz, 175MHz or 35MHz.
 *
 * @param freq Parameter of type PLL_Frequency_Mode, to select the Frequency.
 *
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS    Frequency changed successfully  
 * - @ref ECANCELED  Frequency change failed because a peripheral or timer is active
 * - @ref EINVAL     Frequency is out of range
 */
uint16_t PLL_Change_Freq(PLL_Frequency_Mode freq);

/**
 * @brief Indicates the PLL frequency.
 *
 * @details Indicates the current PLL frequency is and returns the value.
 *
 * @param freq Pointer to return the current operating frequency value.
 *
 * @return Returns a 16-bit status code:
 * - @ref Returns SUCCESS after checking the frequency.
 */
uint16_t PLL_Check_Freq(uint16_t *freq);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_PLL_H_
