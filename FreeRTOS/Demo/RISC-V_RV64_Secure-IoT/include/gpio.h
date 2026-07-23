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
* @enum GPIO_IRQn_Type
* @brief GPIO interrupt instance identifiers.
*
* This enumeration defines the available GPIO Interrupt instances supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  GPIO0_IRQn                =   1,              /*!< 1  GPIO0                                                                  */
  GPIO1_IRQn                =   2,              /*!< 2  GPIO1                                                                  */
  GPIO2_IRQn                =   3,              /*!< 3  GPIO2                                                                  */
  GPIO3_IRQn                =   4,              /*!< 4  GPIO3                                                                  */
  GPIO4_IRQn                =   5,              /*!< 5  GPIO4                                                                  */
  GPIO5_IRQn                =   6,              /*!< 6  GPIO5                                                                  */
  GPIO6_IRQn                =   7,              /*!< 7  GPIO6                                                                  */
  GPIO7_IRQn                =   8,              /*!< 8  GPIO7                                                                  */
  GPIO8_IRQn                =   9,              /*!< 9  GPIO8                                                                  */
  GPIO9_IRQn                =  10,              /*!< 10 GPIO9                                                                  */
  GPIO10_IRQn               =  11,              /*!< 11 GPIO10                                                                 */
  GPIO11_IRQn               =  12,              /*!< 12 GPIO11                                                                 */
  GPIO12_IRQn               =  13,              /*!< 13 GPIO12                                                                 */
  GPIO13_IRQn               =  14,              /*!< 14 GPIO13                                                                 */
  GPIO14_IRQn               =  15,              /*!< 15 GPIO14                                                                 */
  GPIO15_IRQn               =  16,              /*!< 16 GPIO15                                                                 */
  GPIO16_IRQn               =  17,              /*!< 17 GPIO16                                                                 */
  GPIO17_IRQn               =  18,              /*!< 18 GPIO17                                                                 */
  GPIO18_IRQn               =  19,              /*!< 19 GPIO18                                                                 */
  GPIO19_IRQn               =  20,              /*!< 20 GPIO19                                                                 */
  GPIO20_IRQn               =  21,              /*!< 21 GPIO20                                                                 */
  GPIO21_IRQn               =  22,              /*!< 22 GPIO21                                                                 */
  GPIO22_IRQn               =  23,              /*!< 23 GPIO22                                                                 */
  GPIO23_IRQn               =  24,              /*!< 24 GPIO23                                                                 */
  GPIO24_IRQn               =  25,              /*!< 25 GPIO24                                                                 */
  GPIO25_IRQn               =  26,              /*!< 26 GPIO25                                                                 */
  GPIO26_IRQn               =  27,              /*!< 27 GPIO26                                                                 */
  GPIO27_IRQn               =  28,              /*!< 28 GPIO27                                                                 */
  GPIO28_IRQn               =  29,              /*!< 29 GPIO28                                                                 */
  GPIO29_IRQn               =  30,              /*!< 30 GPIO29                                                                 */
  GPIO30_IRQn               =  31,              /*!< 31 GPIO30                                                                 */
  GPIO31_IRQn               =  32,              /*!< 32 GPIO31                                                                 */
  GPIOP0_IRQn               =  70,              /*!< 69 GPIOP0                                                                 */
  GPIOP1_IRQn               =  71,              /*!< 70 GPIOP1                                                                 */
  GPIOP2_IRQn               =  72,              /*!< 71 GPIOP2                                                                 */
  GPIOP3_IRQn               =  73,              /*!< 72 GPIOP3                                                                 */
  GPIOP4_IRQn               =  74,              /*!< 73 GPIOP4                                                                 */
  GPIOP5_IRQn               =  75,              /*!< 74 GPIOP5                                                                 */
  GPIOP6_IRQn               =  76,              /*!< 75 GPIOP6                                                                 */
  GPIOP7_IRQn               =  77,              /*!< 76 GPIOP7                                                                 */
  GPIOP8_IRQn               =  78,              /*!< 77 GPIOP8                                                                 */
  GPIOP9_IRQn               =  79,              /*!< 78 GPIOP9                                                                 */
  GPIOP10_IRQn              =  80,              /*!< 79 GPIOP10                                                                */
  GPIOP11_IRQn              =  81,              /*!< 80 GPIOP11                                                                */
  GPIOP12_IRQn              =  82               /*!< 81 GPIOP12                                                               */
} GPIO_IRQn_Type;

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
