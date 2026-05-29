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
 * @file gpio.h
 * @brief Contains driver header for GPIO Interface
 * @details Provides the API for hardware control of General Purpose I/O (GPIO) 
 *          pins,including configuration, setting/clearing, read/write and  
 *          sequential read/write operations for the hardware GPIO buffers.
 * @version 1.1
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 * @date 23-09-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 06-04-2025 | 1.0     | Shri Mahaalakshmi S J | Initial release.              
 * 23-09-2025 | 1.1     | Shri Mahaalakshmi S J | Added sequential read/write 
 *            |         |                       | support to GPIO buffer APIs
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_GPIO_H_
#define BSP_INCLUDE_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup GPIO_MACROS GPIO Configuration Macros
 *
 * @brief Macros used to assign GPIO pin and configure the direction of GPIO.
 *
 * @{
 */

/** @brief Represents the GPIO input direction. */
#define GPIO_IN  0

/** @brief Represents the GPIO output direction. */
#define GPIO_OUT 1

/** @brief Sets bit x in a 64-bit mask. 
 * e.g., Assign gpio pin 5 as GPIO_PIN(5) */
#define GPIO_PIN(x) (1ULL << (x))

/** @} */

/**
 * @brief The function `GPIO_Config` initializes the GPIO instance and set the 
 * direction.
 * 
 * @details It is used to initializes the GPIO instance and set the direction 
 * of a GPIO pin which configure the GPIO port as input or output.  
 * 
 * @param gpio_pins The `gpio_pins` parameter specifies the pins that you want 
 * to set in the GPIO_DIRECTION register.
 * 
 * @param direction The direction parameter is an bool value that specifies the 
 * direction of the GPIO pin. It can be either 0 or 1, where 0 represents input
 * and 1 represents output.
 * 
 * @return The function `GPIO_Config` returns `SUCCESS` 
 */
uint16_t GPIO_Config(uint64_t gpio_pins, bool direction);

/**
 * @brief The function `GPIO_Pin_Set` sets multiple GPIO pins based on the input
 * arguments provided.
 * 
 * @details It is used to set the value of a GPIO pin in a GPIO instance, 
 * it also sets for multiple GPIO pins.
 * 
 * @param gpio_pins The `gpio_pins` parameter specifies the pins that you want 
 * to set in the GPIO_SET register.
 * 
 * @return The function `GPIO_Pin_Set` returns `SUCCESS`
 */
uint16_t GPIO_Pin_Set(uint64_t gpio_pins);

/**
 * @brief The function `GPIO_Pin_Clear` clears multiple GPIO pins based on the 
 * input arguments provided.
 * 
 * @details It is used to clear the value of a GPIO pin in a GPIO instance, 
 * it also clears for multiple GPIO pins.
 * 
 * @param gpio_pins The `gpio_pins` parameter specifies the pins that you want 
 * to clear in the GPIO_CLEAR register.
 * 
 * @return The function `GPIO_Pin_Clear` returns `SUCCESS`
 */
uint16_t GPIO_Pin_Clear(uint64_t gpio_pins);

/**
 * @brief The function `GPIO_Pin_Toggle` toggles multiple GPIO pins based on the
 * input arguments provided.
 * 
 * @details It is used to toggle the value of a GPIO pin in a GPIO instance, 
 * it also toggle for multiple GPIO pins.
 * 
 * @param gpio_pins The `gpio_pins` parameter specifies the pins that you want 
 * to toggle in the GPIO_TOGGLE register.
 * 
 * @return The function `GPIO_Pin_Toggle` returns `SUCCESS`
 */
uint16_t GPIO_Pin_Toggle(uint64_t gpio_pins);

/**
 * @brief The function `GPIO_Interrupt_Config` configures the interrupt for a 
 * specific GPIO pin 
 * 
 * @details It is used to configure the interrupt for GPIO pins based on whether
 * it is active low or active high.
 * 
 * @param gpio_pin The pin number for which the interrupt configuration is 
 * being set.
 *
 * @param pin_state This parameter is a boolean value that determines whether the 
 * interrupt is active low or active high. If it is set to true (non-zero), the
 * interrupt is active low, and if it is set to false (zero), the interrupt is 
 * active high.
 * 
 * @return The function `GPIO_Interrupt_Config` returns `SUCCESS`
 */
uint16_t GPIO_Interrupt_Config(uint64_t gpio_pins, uint8_t pin_state);

/**
 * @brief The function `GPIO_Read_Data` returns the value of the data register
 * in a GPIO instance.
 * 
 * @details It is used to return the value of the GPIO_DATA register
 * 
 * @param read_data The `read_data` reads data from GPIO registers and
 * stores the result in the memory location pointed to by `read_data`.
 * 
 * @return The function `GPIO_Read_Data` returns `SUCCESS`
 */
uint16_t GPIO_Read_Data(uint64_t *read_data);

/**
 * @brief The function `GPIO_Read_Pin_Status` reads the status of GPIO instance.
 * 
 * @details It is used to read the status of the specific pin in the 
 * GPIO instance
 * 
 * @param gpio_pin gpio_pin The `gpio_pin`represents a GPIO pin.
 * 
 * @param pin_status The `pin_status` parameter is a pointer where the status 
 * of the GPIO pin will be stored after reading.
 * 
 * @return The function `GPIO_Read_Data` returns `SUCCESS`
 */
uint16_t GPIO_Read_Pin_Status(uint64_t gpio_pin, uint8_t *pin_status);

/**
 * @brief The function `GPIO_Write_Data` sets the data register of a GPIO 
 * instance to a given data.
 * 
 * @details It is used to sets the data register of a GPIO instance to a 
 * given data.
 * 
 * @param gpio_pin The pin number to write the data.
 * 
 * @param data_word The `data_word` parameter contains data to be written to 
 * GPIO pins.
 * 
 * @return The function `GPIO_Write_Data` is returns `SUCCESS`.
 */
uint16_t GPIO_Write_Data(uint64_t data_word);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_GPIO_H_
