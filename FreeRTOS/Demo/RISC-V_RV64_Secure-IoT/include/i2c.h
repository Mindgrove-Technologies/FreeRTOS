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
 * @file i2c.h
 * @brief  This is a Baremetal I2C Driver header file for I2C Peripheral.
 * @details Provides prototypes for initializing the I2C peripheral and 
 *          performing transmit and receive operations.
 * @version 1.1
 * @authors Vishwajith.N.S (vishwajith@mindgrovetech.in),
 *          Harini P (harinip@mindgrovetech.in)
 * @date 13-11-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 17-10-2024 | 1.0     | Vishwajith.N.S        | Initial release.              
 * 13-11-2025 | 1.1     | Harini P              | Updated the header file for 
 *            |         |                       | MISRA-C compliance.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_I2C_H_
#define BSP_INCLUDE_I2C_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @defgroup I2C_Modes I2C Mode Macros
 *
 * @brief Macros used to configure I2C transaction modes.
 *
 * @{
 */

/** @brief Sends a START condition on the I2C bus. */
#define START_BIT   (1U << 1U)

/** @brief Sends a STOP condition on the I2C bus. */
#define STOP_BIT    (1U << 2U)

/** @} */ /* end of I2C_Modes */


/**
 * @defgroup I2C_Instance_Type I2C Instance Handle Type
 * @brief Opaque I2C hardware instance handle type.
 *
 * This module defines the I2C instance handle type used by the I2C driver.
 * The type is opaque and represents a specific I2C hardware block.
 *
 * Users must not create objects of this type directly. Valid instances
 * must only be obtained using the provided I2C instance constructors
 * or I2C instance macros.
 *
 * @{
 */

/**
 * @brief Opaque I2C hardware instance type.
 *
 * This type represents a specific I2C hardware block on the SoC.
 * The internal structure is hidden from the user and is only known
 * to the I2C driver implementation.
 *
 * @note This type must only be used as a handle.
 *       Do not attempt to allocate or define objects of this type.
 */
typedef struct I2C_Instance I2C_Instance_t;

/** @} */  /* end of I2C_Instance_Type */


/**
 * @defgroup I2C_Instance_Constructors I2C Instance Constructors
 * @brief I2C hardware instance constructors.
 *
 * These functions return opaque handles to I2C hardware instances.
 * The returned handles must be passed to the I2C driver configuration.
 *
 * @{
 */

/**
 * @brief Get handle for I2C instance 0.
 * @return Pointer to I2C0 hardware instance descriptor.
 */
const I2C_Instance_t *I2C_INSTANCE_0(void);

/**
 * @brief Get handle for I2C instance 1.
 * @return Pointer to I2C1 hardware instance descriptor.
 */
const I2C_Instance_t *I2C_INSTANCE_1(void);

/** @} */  /* end of I2C_Instance_Constructors */


/**
 * @defgroup I2C_Instance_Macros I2C Instance Macros
 * @brief I2C hardware instance macros.
 *
 * These macros expand to constant I2C hardware instance handles.
 * They provide a convenient and readable way to select I2C instances.
 *
 * @{
 */

/**
 * @def I2C0
 * @brief Macro for I2C instance 0 handle.
 */
#define I2C0 I2C_INSTANCE_0()

/**
 * @def I2C1
 * @brief Macro for I2C instance 1 handle.
 */
#define I2C1 I2C_INSTANCE_1()

/** @} */  /* end of I2C_Instance_Macros */


/* Function prototypes */

/**
 * @brief Used to set clock frequency for I2C communication.
 * 
 * @details This function initializes the specified I2C instance and sets its
 *          clock frequency for master transmit and receive operations.
 * 
 * @param i2c_num The parameter \a i2c_num is a pointer to the I2C instance handle.
 *                Specifies the I2C peripheral instance to configure. Use I2C0 or 
 *                I2C1 as defined in the I2C instance macros.
 * 
 * @param clock_frequency The parameter \a clock_frequency is an unsigned integer
 *                        specifying the desired I2C communication clock frequency.
 * 
 * @return SUCCESS when successfully initialised; on failure, returns an error code.
 */
uint16_t I2C_Init(const I2C_Instance_t *i2c_num, uint32_t clock_frequency);

/**
 * @brief Used to send data to an I2C slave device.
 * 
  * @details This function transmits a sequence of bytes from the specified
 *          data buffer to the given I2C slave address.
 * 
 * @param i2c_num The parameter \a i2c_num is a pointer to the I2C instance handle.
 *                Specifies the I2C peripheral instance to configure. Use I2C0 or 
 *                I2C1 as defined in the I2C instance macros.
 * 
 * @param slave_address The parameter \a slave_address is an 7 bit address that
 *                      represents the I2C slave address.
 * 
 * @param data The parameter \a data is an pointer to buffer containing data to send.
 * 
 * @param length The parameter \a length is the number of bytes to transmit from the 
 *               data buffer.
 * 
 * @param mode The parameter \a mode is used to set whether to send start bit,
 *             stop bit and repeated start bit in transaction.
 *             - START_BIT : sends start bit 
 *             - STOP_BIT : sends stop bit
 *             - (START_BIT | STOP_BIT ) : sends repeated start bit
 *             
 * 
 * @return SUCCESS if the transmission is successful; on failure, returns an error code.
 */
uint16_t I2C_Transmit(const I2C_Instance_t *i2c_num, uint8_t slave_address, \
                      uint8_t *data, uint8_t length, uint8_t mode);

/**
 * @brief Used to receive data from an I2C slave device.
 * 
 * @details This function reads a sequence of bytes from the specified I2C
 *          slave address into the provided data buffer.
 * 
 * @param i2c_num The parameter \a i2c_num is a pointer to the I2C instance handle.
 *                Specifies the I2C peripheral instance to configure. Use I2C0 or 
 *                I2C1 as defined in the I2C instance macros.
 * 
 * @param slave_address The parameter \a slave_address is an 7 bit address that 
 *                      represents the I2C slave address.
 * 
 * @param data The parameter \a data is an pointer to buffer containing data to send.
 * 
 * @param length The parameter \a length is the number of bytes to transmit from the 
 *               data buffer.
 * 
 * @param mode The parameter \a mode is used to set whether to send start
 *             bit, stop bit and repeated start in transaction.
 *             - START_BIT : sends start bit 
 *             - STOP_BIT : sends stop bit
 *             - (START_BIT | STOP_BIT ) : sends repeated start bit
 * 
 * @return SUCCESS if the reception is successful; on failure, returns an error code.
 */
uint16_t I2C_Receive(const I2C_Instance_t *i2c_num, uint8_t slave_address, \
                     uint8_t *data, uint8_t length, uint8_t mode);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_I2C_H_
