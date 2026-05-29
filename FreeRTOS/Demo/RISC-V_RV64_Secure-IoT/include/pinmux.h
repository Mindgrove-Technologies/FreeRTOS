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
 * @copyright Copyright (c) 2026 Mindgrove Technologies.
 * All rights reserved.
 * @file pinmux.h
 * @brief Contains APIs for configuring and resetting the Pinmux functionality.
 * @details Provides APIs to enable the required peripheral such as GPIO, PWM,
 *          GPtimer, UART and SPI.
 * @version 1.1
 * @authors Kapil Shyam.M,Harini Sree.S
 * @date 22-01-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description
 * -----------|---------|-----------------------|-------------------------------
 * 24-04-2025 | 1.0     | Kapil Shyam.M         | Initial release.
 * 22-01-2026 | 1.1     | Harini Sree.S         | SecureIOT Production changes
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PINMUX_H_
#define BSP_INCLUDE_PINMUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief The function PINMUX_Reset() enables GPIO functionality.
 *
 * @details The function PINMUX_Reset() enables the pinmux for GPIOs 32–44
 * to select GPIO functionality. Since all other GPIOs are already enabled
 * for GPIO functionality in the default state, they are not reconfigured.
 * 
 * @param none
 * 
 * @return SUCCESS after the Pinmux for GPIO 32-44 is configured.
 */
uint16_t PINMUX_Reset(void);

/**
 * @brief The function PINMUX_PWM enables PWM functionality for a
 * specified pin.
 *
 * @details The function PINMUX_PWM checks whether the provided PWM
 * channel number is less than 14. If it is, the corresponding pinmux
 * configuration register is set to 1 to enable the PWM channel.
 *
 * @param num The PWM channel number to be enabled.
 * @param enable  If set to true, configures the pin for PWM functionality.
 * If set to false, reverts the pin to GPIO functionality.
 * 
 * @return SUCCESS if the pinmux configuration was applied successfully;
 * otherwise, ENODEV if the channel number is invalid.
 */
uint16_t PINMUX_PWM(uint8_t num, bool enable);

/**
 * @brief The function pinmux__all_pwm enables PWM functionality on all
 * supported pins.
 *
 * @details The function pinmux__all_pwm configures the pinmux registers
 * for all PWM-capable pins to enable PWM output.
 *
 * @param enable  If set to true, configures the pin for PWM functionality.
 * If set to false, reverts the pin to GPIO functionality.
 * 
 * @return SUCCESS if all PWM pins are successfully enabled.
 */
uint16_t PINMUX_AllPWM(bool enable);

/**
 * @brief The function PINMUX_SPI enables the SPI peripheral(SPI2 or SPI3)
 *
 * @details The function PINMUX_SPI checks whether the provided SPI number
 * is 2 or 3 and whether the pin is already in use. If the pin is not in use,
 * the function configures the corresponding pinmux register to enable SPI
 * functionality. If the pin number is invalid or the pin is already in use, an
 * error is returned.
 *
 * @param num The SPI number to be enabled for pin multiplexing.
 * @param enable  If set to true, configures the pin for PWM functionality.
 * If set to false, reverts the pin to GPIO functionality.
 *
 * @return SUCCESS if the SPI peripheral is enabled successfully; otherwise,
 * ENODEV if the SPI number is invalid.
 */
uint16_t PINMUX_SPI(uint8_t num, bool enable);

/**
 * @brief The function PINMUX_UART enables one of the UART peripherals
 * (UART3 or UART4).
 *
 * @details The function PINMUX_UART checks whether the provided UART
 * number is 3 or 4. If valid, it sets the corresponding pinmux configuration
 * register to enable the UART peripheral. If the UART number is invalid, an
 * error is returned.
 *
 * @param num The UART number to be enabled for pin multiplexing.
 * @param enable  If set to true, configures the pin for PWM functionality.
 * If set to false, reverts the pin to GPIO functionality.
 * 
 * @return SUCCESS if the UART peripheral is enabled successfully; otherwise,
 * ENODEV if the UART number is invalid.
 */
uint16_t PINMUX_UART(uint8_t num, bool enable);

/**
 * @brief The function PINMUX_GPTimer enables the GPTimer peripheral
 * (GPTIimer0 to GPTimer3).
 *
 * @details The function PINMUX_GPTimer checks whether the provided
 * GPTimer number is in the range 0–3. If valid, it checks whether the
 * corresponding pinmuxed pin (GPIO 38–41) is already in use. If the pin
 * is not in use, the function sets the corresponding pinmux configuration
 * register to enable the GPTimer peripheral. If the number is invalid,
 * an error is returned.
 *
 * @param num The GPTIimer number to be enabled for pin multiplexing.
 * @param enable  If set to true, configures the pin for PWM functionality.
 * If set to false, reverts the pin to GPIO functionality.
 *
 * @return SUCCESS if the GPTimer peripheral is enabled successfully;
 * otherwise, ENODEV if the number is invalid.
 */
uint16_t PINMUX_GPTimer(uint8_t num, bool enable);

/**
 * @brief The function PINMUX_EnableJTAG enables the JTAG pins.
 *
 * @details The function PINMUX_EnableJTAG configures the pinmux register to
 * enable the JTAG_TRST functionality.
 *
 * @param none
 *
 * @return SUCCESS if the JTAG pin is enabled successfully.
 */
uint16_t PINMUX_EnableJTAG(void);

/**
 * @brief The function PINMUX_DisableJTAG enables the JTAG pin.
 *
 * @details The function PINMUX_DisableJTAG configures the pinmux register
 * to disable the JTAG functionality.
 *
 * @param none
 *
 * @return SUCCESS if the JTAG pin is disabled successfully.
 */
uint16_t PINMUX_DisableJTAG(void);


#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_PINMUX_H_
