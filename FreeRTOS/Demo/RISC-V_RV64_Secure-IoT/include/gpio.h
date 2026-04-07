/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : gpio.h
 * Brief Description of file         : Header to Standard gpio driver
 * Name of Author                    : Kapil Shyam. M, Shri Mahaalakshmi S J
 * Email ID                          : kapilshyamm@gmail.com, mahaalakshmi@mindgrovetech.in 
 * 
 * @file gpio.h
 * @author Kapil Shyam. M (kapilshyamm@gmail.com)
 * @author Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 * @brief This is a Baremetal GPIO Driver's Header file for Mindgrove Silicon's GPIO Peripheral
 * @version 0.2
 * @date 2024-09-23
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2024. All rights reserved.
 * 
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "secure_iot.h"
#include "pinmux.h"
#include "log.h"
#include "errors.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_IN  0
#define GPIO_OUT 1
#define ALL_GPIO_PINS -1

#define GPIO_PINS(x) (1 << x)

/**
 * @fn GPIO_Config(bool direction, uint32_t gpio_pins)
 *
 * @brief The function `GPIO_Config` initializes the GPIO instance and set the direction.
 * 
 * @details The function `GPIO_Config` configures GPIO pins based on the specified direction and pin mask,
 * checking for PWM configuration on certain pins.
 * 
 * @param direction The `direction` parameter in the `GPIO_Config` function is a boolean value that
 * specifies the direction of the GPIO pins. 
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @return The function `GPIO_Config` returns a value of `SUCCESS` if the configuration is successful.
 * If the condition meets pinmux, it logs an error message and returns `EPERM`.
 */
uint8_t GPIO_Config(bool direction, uint32_t gpio_pins);

/**
 * @fn GPIO_Pin_Set(uint32_t gpio_pins)
 * 
 * @brief The function `GPIO_Pin_Set` sets multiple GPIO pins based on the input arguments provided.
 * 
 * @details The function GPIO_Pin_Set sets the specified GPIO pins to high.
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @return The function `GPIO_Pin_Set` is returns the value `SUCCESS`.
 */
uint8_t GPIO_Pin_Set(uint32_t gpio_pins);

/**
 * @fn GPIO_Pin_Clear(uint32_t gpio_pins)
 * 
 * @brief The function `GPIO_Pin_Clear` clears multiple GPIO pins based on the input arguments provided.
 * 
 * @details The function GPIO_Pin_Clear clears the specified GPIO pins.
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @return The function `GPIO_Pin_Set` is returns the value `SUCCESS`.
 */
uint8_t GPIO_Pin_Clear(uint32_t gpio_pins);

/**
 * @fn GPIO_Pin_Toggle(uint32_t gpio_pins)
 * 
 * @brief The function `GPIO_Pin_Toggle` toggles multiple GPIO pins based on the input arguments provided.
 * 
 * @details The function GPIO_Pin_Toggle toggles the specified GPIO pins.
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @return The function `GPIO_Pin_Toggle` is returns the value `SUCCESS`.
 */
uint8_t GPIO_Pin_Toggle(uint32_t gpio_pins);

/**
 * @fn GPIO_Interrupt_Config(uint32_t gpio_pins, uint8_t low_ena)
 *
 * @brief The function `GPIO_Interrupt_Config` configures the interrupt for a specific GPIO pin 
 *
 * @details The function `GPIO_Interrupt_Config` configures GPIO interrupt settings based on the input
 * parameters.
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @param low_ena The `low_ena` parameter is a flag that indicates whether the interrupt should be
 * enabled for low-level triggering.
 * 
 * @return The function `GPIO_Interrupt_Config` is returning the value `SUCCESS`.
 */
uint8_t GPIO_Interrupt_Config(uint32_t gpio_pins, uint8_t low_ena);

/**
 * @fn GPIO_Read_Data()
 *
 * @brief The function `GPIO_Read_Data` returns the value of the data register in a GPIO instance.
 * 
 * @details The function GPIO_Read_Data reads and returns the data from the GPIO data register.
 * 
 * @return The function `GPIO_Read_Data()` is returning the value stored in the `GPIO_DATA` register of
 * the GPIO peripheral.
 */
uint32_t GPIO_Read_Data();

/**
 * @fn GPIO_Read_Pin_Status(uint32_t gpio_pin)
 * 
 * @brief The function `GPIO_Read_Pin_Status` reads the status of GPIO instance.
 * 
 * @details The function `GPIO_Read_Pin_Status` reads the status of a specific GPIO pin and returns 1 if the pin
 * is high and 0 if the pin is low.
 * 
 * @param gpio_pins The `gpio_pins` parameter is a 32-bit unsigned integer that represents a bitmask
 * where each bit corresponds to a specific GPIO pin. 
 * 
 * @return The function `GPIO_Read_Pin_Status` returns 1 if the specified GPIO pin is high (set) and
 * returns 0 if the pin is low (clear).
 */
uint32_t GPIO_Read_Pin_Status(uint32_t gpio_pin);

/**
 * @fn GPIO_Write_Data(uint32_t data_word)
 *
 * @brief The function `GPIO_Write_Data` sets the data register of a GPIO instance to a given data.
 * 
 * @details The function `GPIO_Write_Data` sets the GPIO data register to the specified data word and returns a
 * success status.
 * 
 * @param data_word The `data_word` parameter in the `GPIO_Write_Data` function is a 32-bit unsigned
 * integer that represents the data to be written to the GPIO data register.
 * 
 * @return The function `GPIO_Write_Data` is returns the value `SUCCESS`.
 */
uint32_t GPIO_Write_Data(uint32_t data_word);

#ifdef __cplusplus
}
#endif

#endif