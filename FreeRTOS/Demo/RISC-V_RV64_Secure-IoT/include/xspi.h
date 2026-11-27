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
 * @file xspi.h
 * @brief Header file for the xspi software driver implementation.
 * @details This file contains the core definitions, enums, and structures. 
 *          It defines the macro for the configurable Chip Select (nCS) pin and outlines the 
 *          xSPI operating modes (Single, Dual, Quad, Octal). It also provides the 
 *          function declarations for resetting configurations, transmitting data, 
 *          and receiving data over the multi-I/O lines.
 * @version 1.0
 * @authors Sai Girish S (saigirish@mindgrovetech.in)
 * @date 12-06-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-06-2026 | 1.0     | Sai Girish S          | Initial release.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_XSPI_H_
#define BSP_INCLUDE_XSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief GPIO pin assigned as the Active-Low Chip Select (nCS) line.
 */
#define nCS          3

/**
 * @enum xspi_mode_t
 * 
 * @brief Multi-I/O line modes for the xSPI phases.
 * This enumeration defines the number of data lines used during the
 * Instruction, Address, Alternate Byte, or Data phases of communication.
 */
typedef enum {
    xSPI_MODE_NONE   = 0U,  /* Phase is skipped entirely */
    xSPI_MODE_SINGLE = 1U,  /* Single line mode (Standard Serial) */
    xSPI_MODE_DUAL   = 2U,  /* Dual lines mode (2 wires) */
    xSPI_MODE_SPI    = 3U,  /* Standard SPI mode mapping */
    xSPI_MODE_QUAD   = 4U,  /* Quad lines mode (4 wires) */
    xSPI_MODE_OCTAL  = 8U   /* Octal lines mode (8 wires) */
} xspi_mode_t;

/**
 * @struct xSPI_Struct_t
 * 
 * @brief Configuration structure for an xSPI transaction sequence.
 * This structure holds all the parameters needed to execute a complete command sequence 
 * (Instruction -> Address -> Alternate Bytes -> Dummy Cycles -> Data).
 */
typedef struct {
    xspi_mode_t inst_mode;       // Operating mode for the Instruction phase
    uint8_t     inst;            // The instruction/command byte to transmit

    xspi_mode_t addr_mode;       // Operating mode for the Address phase
    uint8_t     addr_size;       // Size of the address phase in bytes
                                 // (e.g., 3 or 4)

    xspi_mode_t altr_byte_mode;  // Operating mode for the Alternate
                                 // Byte phase
    uint8_t     altr_byte_size;  // Size of the alternate byte phase in bytes

    uint8_t     dummy_cycles;    // Number of dummy wait cycles before
                                 // data phase

    xspi_mode_t data_mode;       // Operating mode for the Data phase

    uint32_t    addr;            // The target memory address value
    uint32_t    altr_byte;       // The alternate byte value
} xSPI_Struct_t;

/**
 * @brief Transmits a command sequence and data payload to the flash device.
 * 
 * @details This function handles pinning configuration, establishes communication by pulling 
 * nCS low, sends the configured command phases, and writes out the data buffer.
 * 
 * @param xspi Pointer to the xSPI configuration structure.
 * @param tx_data Pointer to the data buffer to be transmitted.
 * @param data_len Number of bytes to transfer from the transmit buffer.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful completion
 * - @ref EFAULT on Null pointer.
 * - Error code returned by GPIO config error or PRO_IO error.
 */
uint16_t xSPI_Transmit(const xSPI_Struct_t *xspi, const uint8_t *tx_data,
                       size_t data_len);

/**
 * @brief Transmits a command sequence and receives data back from the flash device.
 * 
 * @details This function sends the configured command phases, dynamically reconfigures 
 * the PRO_IO peripheral directions for reading, and captures incoming data.
 * 
 * @param xspi Pointer to the xSPI configuration structure.
 * @param rx_data Pointer to the data buffer where received bytes will be saved.
 * @param data_len Number of bytes to read from the device.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful completion
 * - @ref EFAULT on Null pointer.
 * - Error code returned by GPIO config error or PRO_IO error.
 */
uint16_t xSPI_Receive(const xSPI_Struct_t *xspi, uint8_t *rx_data,
                      size_t data_len);

/**
 * @brief Clears and resets the xSPI configuration structure.
 * 
 * @details Erases all settings inside the provided structure back to zero, ensuring 
 * no leftover configurations cause errors in the next transaction.
 * 
 * @param xspi Pointer to the xSPI structure instance to be reset.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS (0) on successful completion
 * - @ref EFAULT on Null pointer.
 */
uint16_t xSPI_Config_Reset(xSPI_Struct_t *xspi);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_XSPI_H_
