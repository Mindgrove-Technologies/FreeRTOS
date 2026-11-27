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
 * @file uart.h
 * @brief Contains the declarations for the UART interface.
 * @details This header file provides the function prototypes, macros,
 *          and data type definitions required to configure, transmit,
 *          and receive data over the UART interface.
 * @version 1.1
 * @authors Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 * @date 23-09-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 06-04-2025 | 1.0     | Vignesh Kumar J       | Initial release.              
 * 23-09-2025 | 1.1     | Vignesh Kumar J       | Added null pointer check using
 *            |         |                       | a macro and enum variable uart
 *            |         |                       | instance
 * 23-02-2026 | 1.2     | Vignesh Kumar J       | Added a timeout functionality
 *            |         |                       | in UART_Read function using 
 *            |         |                       | mcycles
 * -----------------------------------------------------------------------------
 */
#ifndef BSP_INCLUDE_UART_H_
#define BSP_INCLUDE_UART_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum UART_StopBits
 * @brief UART stop bit configuration values.
 *
 * Encoding:
 * - 00 : 1 stop bit
 * - 01 : 1.5 stop bits
 * - 10 : 2 stop bits
 * - 11 : Reserved (unused)
 *
 * These values must be used with UART_Config_t::stop_bits.
 */
enum UART_StopBits {
    STOP_BIT_1   = 0,   /**< 1 stop bit */
    STOP_BIT_1_5 = 1,   /**< 1.5 stop bits */
    STOP_BIT_2   = 2    /**< 2 stop bits */
};

/**
 * @enum UART_Parity
 * @brief UART parity configuration values.
 *
 * Encoding:
 * - 00 : No parity
 * - 01 : Odd parity
 * - 10 : Even parity
 * - 11 : Reserved (unused)
 *
 * These values must be used with UART_Config_t::parity.
 */
enum UART_Parity {
    NO_PARITY   = 0,   /**< No parity */
    ODD_PARITY  = 1,   /**< Odd parity */
    EVEN_PARITY = 2    /**< Even parity */
};

/**
 * @enum UART_CharSize
 * @brief UART character size configuration values.
 *
 * Encoding:
 * - 00 : 8-bit character size
 * - 01 : 7-bit character size
 * - 10 : 6-bit character size
 * - 11 : 5-bit character size
 *
 * These values must be used with UART_Config_t::char_size.
 */
enum UART_CharSize {
    CHAR_SIZE_8 = 0,   /**< 8-bit character size */
    CHAR_SIZE_7 = 1,   /**< 7-bit character size */
    CHAR_SIZE_6 = 2,   /**< 6-bit character size */
    CHAR_SIZE_5 = 3    /**< 5-bit character size */
};

/**
 * @enum UART_IRQn_Type
 * 
 * @brief UART interrupt ID
 * 
 * This enumeration defines the available UART interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  UART0_IRQn                =  53,              /*!< 53 UART0                                                                  */
  UART1_IRQn                =  54,              /*!< 54 UART1                                                                  */
  UART2_IRQn                =  55,              /*!< 55 UART2                                                                  */
  UART3_IRQn                =  56,              /*!< 56 UART3                                                                  */
  UART4_IRQn                =  57,              /*!< 57 UART4                                                                  */
} UART_IRQn_Type;

/**
 * @defgroup UART_Interrupt_Enable UART Interrupt Enable Bits
 * @brief UART interrupt enable bit definitions.
 *
 * This module defines the bit masks used to enable different UART
 * interrupt sources in the UART interrupt enable register.
 *
 * These macros can be combined using bitwise OR to enable multiple
 * interrupt sources.
 *
 * @{
 */

/**
 * @brief Enable RX threshold interrupt.
 */
#define ENABLE_RX_THRESHOLD     (1U << 8U)

/**
 * @brief Enable break error interrupt.
 */
#define ENABLE_BREAK_ERROR      (1U << 7U)

/**
 * @brief Enable frame error interrupt.
 */
#define ENABLE_FRAME_ERROR      (1U << 6U)

/**
 * @brief Enable overrun error interrupt.
 */
#define ENABLE_OVERRUN          (1U << 5U)

/**
 * @brief Enable parity error interrupt.
 */
#define ENABLE_PARITY_ERROR     (1U << 4U)

/**
 * @brief Enable RX full interrupt.
 */
#define ENABLE_RX_FULL          (1U << 3U)

/**
 * @brief Enable RX not empty interrupt.
 */
#define ENABLE_RX_NOT_EMPTY     (1U << 2U)

/**
 * @brief Enable TX full interrupt.
 */
#define ENABLE_TX_FULL          (1U << 1U)

/**
 * @brief Enable TX empty interrupt.
 */
#define ENABLE_TX_EMPTY         (1U << 0U)

/** @} */  /* end of UART_Interrupt_Enable */

/**
 * @defgroup UART_Buffer UART Data Buffer Structure
 * @brief UART data buffer definition.
 *
 * This module defines the data buffer structure used for UART
 * transmit and receive operations.
 *
 * The buffer structure holds a pointer to the data and the
 * length of the data to be transmitted or received.
 *
 * @{
 */

/**
 * @brief UART data buffer structure.
 *
 * This structure is used to pass data buffers to UART transmit
 * and receive APIs.
 */
struct uart_buf {
    /**
     * @brief Pointer to UART data buffer.
     *
     * This pointer must point to a valid memory region containing
     * the data to be transmitted or received.
     */
    void *uart_data;

    /**
     * @brief Length of the UART data buffer.
     *
     * Specifies the number of bytes to be transmitted or received.
     */
    size_t len;
};

/** @} */  /* end of UART_Buffer */

/**
 * @defgroup UART_Instance_Type UART Instance Handle Type
 * @brief Opaque UART hardware instance handle type.
 *
 * This module defines the UART instance handle type used by the UART driver.
 * The type is opaque and represents a specific UART hardware block.
 *
 * Users must not create objects of this type directly. Valid instances
 * must only be obtained using the provided UART instance constructors
 * or UART instance macros.
 *
 * @{
 */

/**
 * @brief Opaque UART hardware instance type.
 *
 * This type represents a specific UART hardware block on the SoC.
 * The internal structure is hidden from the user and is only known
 * to the UART driver implementation.
 *
 * @note This type must only be used as a handle.
 *       Do not attempt to allocate or define objects of this type.
 */
typedef struct UART_Instance UART_Instance_t;

/** @} */  /* end of UART_Instance_Type */

/**
 * @defgroup UART_Instance_Constructors UART Instance Constructors
 * @brief UART hardware instance constructors.
 *
 * These functions return opaque handles to UART hardware instances.
 * The returned handles must be passed to the UART driver configuration.
 *
 * @{
 */

/**
 * @brief Get handle for UART instance 0.
 *
 * @note UART instance 0 is reserved for debug and print statements.
 *       It should not be used for general-purpose UART communication.
 *
 * @return Pointer to UART0 hardware instance descriptor.
 */
const UART_Instance_t *UART_INSTANCE_0(void);

/**
 * @brief Get handle for UART instance 1.
 * @return Pointer to UART1 hardware instance descriptor.
 */
const UART_Instance_t *UART_INSTANCE_1(void);

/**
 * @brief Get handle for UART instance 2.
 * @return Pointer to UART2 hardware instance descriptor.
 */
const UART_Instance_t *UART_INSTANCE_2(void);

/**
 * @brief Get handle for UART instance 3.
 * @return Pointer to UART3 hardware instance descriptor.
 */
const UART_Instance_t *UART_INSTANCE_3(void);

/**
 * @brief Get handle for UART instance 4.
 * @return Pointer to UART4 hardware instance descriptor.
 */
const UART_Instance_t *UART_INSTANCE_4(void);

/** @} */  /* end of UART_Instance_Constructors */

/**
 * @defgroup UART_Instance_Macros UART Instance Macros
 * @brief UART hardware instance macros.
 *
 * These macros expand to constant UART hardware instance handles.
 * They provide a convenient and readable way to select UART instances.
 *
 * @{
 */


/**
 * @def UART0
 * @brief Macro for UART instance 0 handle.
 *
 * @note UART instance 0 is reserved for debug and print statements.
 *       It should not be used for general-purpose UART communication.
 */
#define UART0 UART_INSTANCE_0()

/**
 * @def UART1
 * @brief Macro for UART instance 1 handle.
 */
#define UART1 UART_INSTANCE_1()

/**
 * @def UART2
 * @brief Macro for UART instance 2 handle.
 */
#define UART2 UART_INSTANCE_2()

/**
 * @def UART3
 * @brief Macro for UART instance 3 handle.
 */
#define UART3 UART_INSTANCE_3()

/**
 * @def UART4
 * @brief Macro for UART instance 4 handle.
 */
#define UART4 UART_INSTANCE_4()

/** @} */  /* end of UART_Instance_Macros */

/**
 * @defgroup UART_Config UART Configuration Structure
 * @brief UART configuration parameters.
 *
 * This structure defines all configuration parameters required to
 * initialize and operate a UART hardware instance.
 *
 * The configuration must be filled by the user.
 *
 * @{
 */

/**
 * @brief UART configuration structure.
 *
 * This structure holds all parameters required for UART initialization
 * and runtime configuration.
 */
typedef struct {
    /**
     * @brief UART hardware instance handle.
     *
     * This parameter selects the UART hardware block to be used.
     * Valid instances are UART1, UART2, UART3 and UART4.
     *
     * @note UART instance 0 is reserved for serial debug communication.
     */
    const UART_Instance_t *uart_num;

    /**
     * @brief UART baud rate.
     *
     * Specifies the desired baud rate for UART communication.
     * Example: 9600, 115200, etc.
     */
    uint32_t baudrate;

    /**
     * @brief Number of stop bits.
     *
     * This is a 2-bit field.
     * Valid values:
     * - 0 : 1 stop bit
     * - 1 : 1.5 stop bits
     * - 2 : 2 stop bits
     *
     * Value 3 is reserved and must not be used.
     */
    uint8_t stop_bits : 2;

    /**
     * @brief Parity configuration.
     *
     * This is a 2-bit field.
     * Valid values:
     * - 0 : No parity
     * - 1 : Odd parity
     * - 2 : Even parity
     *
     * Value 3 is reserved and must not be used.
     */
    uint8_t parity : 2;

    /**
     * @brief Character size configuration.
     *
     * This is a 2-bit field.
     * Valid character sizes:
     * - 5 bits
     * - 6 bits
     * - 7 bits
     * - 8 bits
     */
    uint8_t char_size : 2;

    /**
     * @brief Delay value.
     *
     * Specifies a delay value used internally by the driver.
     */
    uint16_t delay;

    /**
     * @brief RX FIFO threshold.
     *
     * Specifies the RX FIFO threshold level for the UART instance.
     * This value is used to configure the RX threshold register.
     */
    uint8_t rxthreshold;

    /**
     * @brief Transmit data width mode.
     *
     * Specifies the data width used while writing to TX register.
     *
     * Valid values:
     * - 0 : 8-bit access
     * - 1 : 16-bit access
     * - 2 : 32-bit access
     */
    uint8_t transfer_mode : 3;

    /**
     * @brief Receive data width mode.
     *
     * Specifies the data width used while reading from RX register.
     *
     * Valid values:
     * - 0 : 8-bit access
     * - 1 : 16-bit access
     * - 2 : 32-bit access
     */
    uint8_t receive_mode : 3;

    /**
     * @brief RX pin pull-up configuration.
     *
     * Determines whether RX pin pull-up is enabled.
     *
     * Valid values:
     * - 0 : Pull-down
     * - 1 : Pull-up
     */
    uint8_t pullup : 1;
} UART_Config_t;

/** @} */  /* end of UART_Config */


/**
 * @brief Check whether data is available in the UART receive buffer.
 *
 * @details The function `UART_Available` checks if there is data available to be read
 * from the UART receive buffer.
 *
 * @param uart_config Pointer to UART configuration structure used to configure
 *        the UART hardware instance.
 * 
 * @return Returns 1 if the UART receive buffer is not empty,
 *         otherwise returns 0.
 */

uint16_t UART_Available(UART_Config_t const *uart_config);

/**
 * @brief Set the receive threshold value for UART communication.
 *
 * @details The function sets the receive threshold for UART communication.
 *
 * @param uart_config Pointer to UART configuration structure used to configure
 *        the UART hardware instance.
 *
 * @return Returns 0 on SUCCESS, or a error code on failure.
 */

uint16_t UART_RX_Threshold(UART_Config_t const *uart_config);

/**
 * @brief Enable UART interrupts for the specified interrupt mask.
 *
 * @details The function enables UART interrupt sources based on the
 *          provided interrupt bitmask.
 * 
 * @param uart_config Pointer to UART configuration structure used to configure
 *        the UART hardware instance.
 *
 * @param interrupt Bitmask representing one or more UART interrupt
 *        enable bits. The value can be formed by combining macros
 *        defined in @ref UART_Interrupt_Enable using the bitwise OR (|) operator.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */

uint16_t UART_Interrupt_Enable(UART_Config_t const *uart_config, \
    uint16_t interrupt);

/**
 * @brief Disable UART interrupts for the specified interrupt mask.
 *
 * @details The function disables UART interrupt sources based on the
 *          provided interrupt bitmask.
 *
 * @param uart_config Pointer to UART configuration structure used to configure
 *        the UART hardware instance.
 *
 * @param interrupt Bitmask representing one or more UART interrupt
 *        enable bits. The value can be formed by combining macros
 *        defined in @ref UART_Interrupt_Enable using the bitwise OR (|) operator.
 *
 * @return Returns 0 on SUCCESS, or an error code on failure.
 */

uint16_t UART_Interrupt_Disable(UART_Config_t const *uart_config, \
    uint16_t interrupt);

/**
 * @brief Initialize the baud rate for a UART instance.
 *
 * @details The function initializes the baud rate for a UART instance based on the
 * provided configuration.
 *
 * @param uart_config Pointer to UART configuration structure used to configure
 *        the UART hardware instance.
 *
 * @return It returns 0 which means operation is success.
 */

uint16_t UART_Set_Baudrate(UART_Config_t const *uart_config);

/**
 * @brief Configure UART settings for a hardware instance.
 *
 * @details The function `UART_Config` configures UART settings based on the
 * provided UART_Config_t structure.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Config(UART_Config_t const *uart_config);

/**
 * @brief Initialize a UART hardware instance.
 *
 * @details The function `UART_Init` initializes a UART instance based on the
 * provided configuration parameters.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Init(UART_Config_t const *uart_config);

#undef putchar

/**
 * 
 * @brief The function sends a character over UART communication.
 * 
 * @details This function will be called to printf a single character to the stdout by passing
 * character as an integer.
 * 
 * @param ch The parameter "ch" is of type int and represents the character to be transmitted over UART
 * (Universal Asynchronous Receiver/Transmitter).
 * 
 * @note Instance 0 cannot be initialized as it is reserved for print statements.
 *       If instance 0 must be configured, use the alternate APIs provided for
 *       performing the required operations.
 * 
 * @return An integer value of 0.
 */

int putchar(int ch);

#undef getchar

/**
 * 
 * @brief Function to read a single character from the standard input device.
 * 
 * @details The function reads a character from a UART instance and waits until a character is available.
 * 
 * @return A character received through UART communication.
 */

int getchar(void);

/**
 * @brief Transmit data via UART.
 *
 * @details The function `UART_Write` transmits data via UART based on the specified
 * transfer mode and UART configuration.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @param tx_bufs Pointer to a uart_buf structure containing the transmit buffer
 *        and the length of the data to be transmitted.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Write(UART_Config_t const *uart_config, struct uart_buf *rx_bufs);

/**
 * @brief Wait until the UART transmit buffer becomes empty.
 *
 * @details The function `UART_Write_Wait` waits for the UART transmit buffer
 * to be empty before returning.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Write_Wait(UART_Config_t const *uart_config);

/**
 * @brief Receive data from the UART interface.
 *
 * @details The function `UART_Read` reads data from a UART interface based on the
 * provided configuration and stores it in the specified receive buffer.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @param rx_bufs Pointer to a uart_buf structure containing the receive buffer
 *        and the maximum length of data to be received.
 *
 * @param timeout Maximum number of CPU cycles to wait.
 *        This value is compared against the difference between the current
 *        `mcycle` count and the start cycle count.
 * 
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Read(UART_Config_t const *uart_config, \
    struct uart_buf *rx_bufs, uint64_t timeout);

/**
 * @brief Flush the UART receive buffer.
 *
 * @details The function `UART_Flush` clears the receive buffer of a UART module
 * specified by the given configuration.
 *
 * @param uart_config Pointer to a UART_Config_t structure used to configure
 *        the UART hardware instance.
 *
 * @return Returns 0 on success, or a negative error code on failure.
 */

uint16_t UART_Flush(UART_Config_t const *uart_config);

#ifdef __cplusplus
}
#endif
#endif   // BSP_INCLUDE_UART_H_
