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
 * @file    qspi_flash.h
 * @brief   QSPI Flash driver interface
 * @details Provides API declarations for performing read, write, erase,
 *          configuration and status operations on external QSPI flash memory
 *          devices connected to Mindgrove Silicon's QSPI peripheral.
 * @version 1.1
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Narasimha R V (narasimha@mindgrovetech.in)
 *          Vignesh Kumar J (vigneshkumar@mindgrovetech.in)
 *          Harini P (harinip@mindgrovetech.in)
 * @date 18-02-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by     | Description
 * -----------|---------|-----------------|-------------------------------------
 * 08-07-2023 | 1.0     | Vishwajith N S  | Initial release.
 * 18-02-2026 | 1.1     | Narasimha R V   | MISRA fixes, code optimization and
 *            |         |                 | cleanup.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_QSPI_FLASH_H_
#define BSP_INCLUDE_QSPI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "qspi.h"

#define FLASH_DEFAULT_PRESCALER    10U

/**
 * @brief Flash operation wait mode selection.
 *
 * Defines whether the flash driver should wait for the
 * completion of program/erase operations internally
 * or return immediately after issuing the command.
 *
 * This is mainly used for flash write, program, and erase APIs.
 *
 * Modes:
 * - FLASH_WAIT_FOR_COMPLETION :
 *   Driver performs internal polling of the flash status
 *   register and returns only after the operation is fully
 *   completed (blocking behavior).
 *
 * - FLASH_NO_WAIT :
 *   Driver issues the flash command and returns immediately
 *   without waiting for completion (non-blocking behavior).
 *
 * This mode is useful when the user wants to manually handle:
 * - Suspend/Resume operations
 * - Custom polling
 * - Background erase/program monitoring
 *
 * @note
 * Use FLASH_NO_WAIT when suspend/resume support is required,
 * since internal polling would prevent issuing suspend commands
 * while the operation is in progress.
 */
typedef enum {
    FLASH_WAIT_FOR_COMPLETION = 0,   /* Wait internally until flash operation   completes */
    FLASH_NO_WAIT                    /* Return immediately without waiting */
} flash_wait_mode_t;

/* ============================================================
* READ OPERATIONS
* ============================================================*/

/**
 * @brief Perform Fast Quad Read operation (1-1-4 mode).
 *
 * @details
 * This function performs a fast read using Quad Output mode
 * with the following transfer format:
 * - Instruction on single line
 * - Address on single line
 * - Data read on four lines
 *
 * This improves read performance compared to standard
 * single-line read operations.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Buffer used to store the read data.
 *
 * @param[in] address
 * Flash start address to read from.
 *
 * @param[in] data_length
 * Number of bytes to read.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Read completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Fast_Read_Quad(const QSPI_Instance_t *qspi_inst,
                              uint8_t *data,
                              uint32_t address,
                              size_t data_length,
                              uint8_t prescaler);

/**
 * @brief Perform Fast Quad I/O Read operation (1-4-4 mode).
 *
 * @details
 * This function performs a fast read using Quad I/O mode
 * with the following transfer format:
 * - Instruction on single line
 * - Address on four lines
 * - Data read on four lines
 *
 * This provides faster access compared to standard
 * Quad Output Read.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Buffer used to store the read data.
 *
 * @param[in] address
 * Flash start address to read from.
 *
 * @param[in] data_length
 * Number of bytes to read.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Read completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Fast_Read_Quad_IO(const QSPI_Instance_t *qspi_inst,
                                 uint8_t *data,
                                 uint32_t address,
                                 size_t data_length,
                                 uint8_t prescaler);

/**
 * @brief Perform standard single-line read operation (1-1-1 mode).
 *
 * @details
 * This function performs a standard flash read where:
 * - Instruction is sent on single line
 * - Address is sent on single line
 * - Data is read on single line
 *
 * This is the basic read mode supported by all flash devices.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Buffer used to store the read data.
 *
 * @param[in] address
 * Flash start address to read from.
 *
 * @param[in] data_length
 * Number of bytes to read.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Read completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Fast_Read_Single(const QSPI_Instance_t *qspi_inst,
                                uint8_t *data,
                                uint32_t address,
                                size_t data_length,
                                uint8_t prescaler);

/* ============================================================
* PROGRAM OPERATIONS
* ============================================================*/

/**
 * @brief Program a flash page using Quad Page Program (1-1-4 mode).
 *
 * @details
 * This function programs data into flash memory using the
 * Quad Page Program command (0x32), where:
 * - Instruction is sent on single line
 * - Address is sent on single line
 * - Data is transferred on four lines
 *
 * The function supports both blocking and non-blocking behavior
 * using @ref flash_wait_mode_t.
 *
 * If @ref FLASH_WAIT_FOR_COMPLETION is selected, the driver waits
 * internally until the WIP (Write In Progress) bit clears,
 * indicating that the page program operation has completed.
 *
 * If @ref FLASH_NO_WAIT is selected, the function returns
 * immediately after issuing the program command.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the data buffer to be programmed.
 *
 * @param[in] address
 * Flash start address where programming should begin.
 *
 * @param[in] data_length
 * Number of bytes to program.
 *
 * @param[in] wait_mode
 * Flash wait mode selection:
 * - FLASH_WAIT_FOR_COMPLETION
 * - FLASH_NO_WAIT
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Page program completed successfully
 * - ETIMEDOUT   : Timeout while waiting for WIP bit to clear
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Flash write enable must be performed using
 * @ref Flash_Write_Enable() before calling this function.
 *
 * @warning
 * When using @ref FLASH_NO_WAIT, the user is responsible for
 * checking operation completion before issuing further flash
 * commands or using suspend/resume operations.
 */
uint16_t Flash_Input_Page_Quad(const QSPI_Instance_t *qspi_inst,
                               uint8_t *data,
                               uint32_t address,
                               size_t data_length,
                               flash_wait_mode_t wait_mode,
                               uint8_t prescaler);

/**
 * @brief Program a flash page using Single Page Program (1-1-1 mode).
 *
 * @details
 * This function programs data into flash memory using the
 * Single Page Program command (0x02), where:
 * - Instruction is sent on single line
 * - Address is sent on single line
 * - Data is transferred on single line
 *
 * The function supports both blocking and non-blocking behavior
 * using @ref flash_wait_mode_t.
 *
 * If @ref FLASH_WAIT_FOR_COMPLETION is selected, the driver waits
 * internally until the WIP (Write In Progress) bit clears,
 * indicating that the page program operation has completed.
 *
 * If @ref FLASH_NO_WAIT is selected, the function returns
 * immediately after issuing the program command.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the data buffer to be programmed.
 *
 * @param[in] address
 * Flash start address where programming should begin.
 *
 * @param[in] data_length
 * Number of bytes to program.
 *
 * @param[in] wait_mode
 * Flash wait mode selection:
 * - FLASH_WAIT_FOR_COMPLETION
 * - FLASH_NO_WAIT
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Page program completed successfully
 * - ETIMEDOUT   : Timeout while waiting for WIP bit to clear
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Flash write enable must be performed using
 * @ref Flash_Write_Enable() before calling this function.
 *
 * @warning
 * When using @ref FLASH_NO_WAIT, the user is responsible for
 * checking operation completion before issuing further flash
 * commands or using suspend/resume operations.
 */
uint16_t Flash_Input_Page_Single(const QSPI_Instance_t *qspi_inst,
                                 uint8_t *data,
                                 uint32_t address,
                                 size_t data_length,
                                 flash_wait_mode_t wait_mode,
                                 uint8_t prescaler);

/* ============================================================
* ERASE OPERATIONS
* ============================================================*/

/**
 * @brief Erase a 4KB flash sector.
 *
 * @details
 * This function issues the 4KB Sector Erase command (0x20)
 * for the specified flash address.
 *
 * The function supports both blocking and non-blocking behavior
 * using @ref flash_wait_mode_t.
 *
 * If @ref FLASH_WAIT_FOR_COMPLETION is selected, the driver waits
 * internally until the WIP (Write In Progress) bit clears,
 * indicating that the erase operation has completed.
 *
 * If @ref FLASH_NO_WAIT is selected, the function returns
 * immediately after issuing the erase command.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] address
 * Start address of the 4KB sector to be erased.
 *
 * @param[in] wait_mode
 * Flash wait mode selection:
 * - FLASH_WAIT_FOR_COMPLETION
 * - FLASH_NO_WAIT
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Sector erase completed successfully
 * - ETIMEDOUT   : Timeout while waiting for WIP bit to clear
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Flash write enable must be performed using
 * @ref Flash_Write_Enable() before calling this function.
 *
 * @warning
 * When using @ref FLASH_NO_WAIT, the user is responsible for
 * checking erase completion before issuing further flash
 * operations or using suspend/resume commands.
 */
uint16_t Flash_Sector_4K_Erase(const QSPI_Instance_t *qspi_inst,
                               uint32_t address,
                               flash_wait_mode_t wait_mode,
                               uint8_t prescaler);

/**
 * @brief Erase a 32KB flash block.
 *
 * @details
 * This function issues the 32KB Block Erase command (0x52)
 * for the specified flash address.
 *
 * The function supports both blocking and non-blocking behavior
 * using @ref flash_wait_mode_t.
 *
 * If @ref FLASH_WAIT_FOR_COMPLETION is selected, the driver waits
 * internally until the WIP (Write In Progress) bit clears,
 * indicating that the erase operation has completed.
 *
 * If @ref FLASH_NO_WAIT is selected, the function returns
 * immediately after issuing the erase command.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] address
 * Start address of the 32KB block to be erased.
 *
 * @param[in] wait_mode
 * Flash wait mode selection:
 * - FLASH_WAIT_FOR_COMPLETION
 * - FLASH_NO_WAIT
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Block erase completed successfully
 * - ETIMEDOUT   : Timeout while waiting for WIP bit to clear
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Flash write enable must be performed using
 * @ref Flash_Write_Enable() before calling this function.
 *
 * @warning
 * When using @ref FLASH_NO_WAIT, the user is responsible for
 * checking erase completion before issuing further flash
 * operations or using suspend/resume commands.
 */
uint16_t Flash_Sector_32K_Erase(const QSPI_Instance_t *qspi_inst,
                                uint32_t address,
                                flash_wait_mode_t wait_mode,
                                uint8_t prescaler);

/**
 * @brief Perform complete flash chip erase.
 *
 * @details
 * This function issues the Chip Erase command to erase the
 * entire flash memory contents.
 *
 * Unlike sector/block erase APIs, this function always performs
 * blocking wait internally until the WIP (Write In Progress)
 * bit clears, indicating that the erase operation has completed.
 *
 * Since chip erase can take significantly longer than page
 * program or sector erase operations, an appropriate timeout
 * value is used internally.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Chip erase completed successfully
 * - ETIMEDOUT   : Timeout while waiting for chip erase completion
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Flash write enable must be performed using
 * @ref Flash_Write_Enable() before calling this function.
 *
 * @warning
 * This operation erases the complete flash memory and cannot
 * be undone. Use with caution.
 */
uint16_t Flash_Chip_Erase(const QSPI_Instance_t *qspi_inst,
                          uint8_t prescaler);

/* ============================================================
* CONTROL COMMANDS
* ============================================================*/

/**
 * @brief Enable flash write operations.
 *
 * @details
 * This function issues the Write Enable command to set the
 * Write Enable Latch (WEL) bit in the flash status register.
 *
 * After this operation, flash program, erase, and status register
 * write operations are permitted until @ref Flash_Write_Disable()
 * is issued or the device automatically clears the WEL bit
 * after a write/erase cycle.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Write enable completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Enable(const QSPI_Instance_t *qspi_inst,
                            uint8_t prescaler);

/**
 * @brief Disable flash write operations.
 *
 * @details
 * This function issues the Write Disable command to clear the
 * Write Enable Latch (WEL) bit in the flash status register.
 *
 * After this operation, flash program, erase, and status register
 * write operations are blocked until @ref Flash_Write_Enable()
 * is issued again.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Write disable completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Disable(const QSPI_Instance_t *qspi_inst,
                             uint8_t prescaler);


/**
 * @brief Suspend an ongoing erase or program operation.
 *
 * @details
 * This function issues the Suspend command (0x75) to pause an
 * active erase or page program operation.
 *
 * Suspend is valid only when:
 * - WIP (Write In Progress) bit = 1
 * - SUS (Suspend) bit = 0
 *
 * The function checks these conditions before issuing the command
 * and waits until the SUS bit becomes set.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Suspend completed successfully
 * - EINVAL      : Invalid flash state for suspend
 * - ETIMEDOUT   : Timeout while waiting for SUS bit to set
 * - Other error codes returned by register read/write operations
 *
 * @note
 * Suspend is typically used to perform read or write operations
 * to other flash regions while erase/program is paused.
 */
uint16_t Flash_Suspend(const QSPI_Instance_t *qspi_inst,
                       uint8_t prescaler);

/**
 * @brief Resume a suspended erase or program operation.
 *
 * @details
 * This function issues the Resume command (0x7A) to continue a
 * previously suspended erase or page program operation.
 *
 * Resume is valid only when:
 * - WIP (Write In Progress) bit = 0
 * - SUS (Suspend) bit = 1
 *
 * The function checks these conditions before issuing the command
 * and waits until the SUS bit clears.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS     : Resume completed successfully
 * - EINVAL      : Invalid flash state for resume
 * - ETIMEDOUT   : Timeout while waiting for SUS bit to clear
 * - Other error codes returned by register read/write operations
 */
uint16_t Flash_Resume(const QSPI_Instance_t *qspi_inst,
                      uint8_t prescaler);

/**
 * @brief Put flash into deep power-down mode.
 *
 * @details
 * This function issues the Deep Power-Down command to reduce
 * flash power consumption when the device is idle.
 *
 * In this mode, most flash operations are unavailable until
 * the device is released using
 * @ref Flash_Release_Power_Down().
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Flash entered deep power-down mode successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Power_Down(const QSPI_Instance_t *qspi_inst,
                          uint8_t prescaler);

/**
 * @brief Release flash from deep power-down mode.
 *
 * @details
 * This function issues the Release Power-Down command to wake
 * the flash device from deep power-down mode and restore normal
 * operation.
 *
 * After this command, the flash becomes accessible again for
 * read, write, and erase operations.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Flash released from deep power-down successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Release_Power_Down(const QSPI_Instance_t *qspi_inst,
                                  uint8_t prescaler);

/* ============================================================
* STATUS REGISTER OPERATIONS
* ============================================================*/

/**
 * @brief Read Status Register 1.
 *
 * @details
 * This function reads Status Register 1 from the flash device.
 *
 * Status Register 1 typically contains:
 * - BUSY (WIP) bit
 * - Write Enable Latch (WEL) bit
 * - Block protection bits
 *
 * It is commonly used to monitor erase/program completion
 * and write enable status.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer to the buffer where the status register value
 * will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 1 read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Status_Register1(const QSPI_Instance_t *qspi_inst,
                                     uint8_t* data,
                                     uint8_t prescaler);

/**
 * @brief Read Status Register 2.
 *
 * @details
 * This function reads Status Register 2 from the flash device.
 *
 * Status Register 2 typically contains:
 * - Suspend (SUS) bit
 * - Quad Enable (QE) bit
 * - Security and protection control bits
 *
 * It is commonly used for suspend/resume handling and
 * quad mode verification.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer to the buffer where the status register value
 * will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 2 read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Status_Register2(const QSPI_Instance_t *qspi_inst,
                                     uint8_t* data,
                                     uint8_t prescaler);

/**
 * @brief Read Status Register 3.
 *
 * @details
 * This function reads Status Register 3 from the flash device.
 *
 * Status Register 3 typically contains:
 * - Drive strength control
 * - Output driver configuration
 * - Additional device-specific configuration bits
 *
 * It is mainly used for advanced flash configuration.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer to the buffer where the status register value
 * will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 3 read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Status_Register3(const QSPI_Instance_t *qspi_inst,
                                     uint8_t* data,
                                     uint8_t prescaler);

/**
 * @brief Read Flag Status Register.
 *
 * @details
 * This function reads the Flag Status Register (FSR) from the
 * flash device.
 *
 * The FSR is used to monitor:
 * - Program/erase completion
 * - Program/erase failures
 * - Protection errors
 * - Controller status conditions
 *
 * It is commonly used for operation status checking and
 * error detection.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer to the buffer where the flag status value
 * will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Flag Status Register read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Flag_Status_Register(const QSPI_Instance_t *qspi_inst,
                                         uint8_t* data,
                                         uint8_t prescaler);

/**
 * @brief Enable volatile write access to status registers.
 *
 * @details
 * This function issues the Volatile Write Enable command (0x50)
 * to allow modification of flash status registers in volatile mode.
 *
 * Changes made using this command are temporary and will be lost
 * after power cycle or reset.
 *
 * This must be called before writing Status Register 1, 2, or 3
 * when volatile updates are required.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Volatile write enable completed successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Enable_Status_Register(const QSPI_Instance_t *qspi_inst,
                                            uint8_t prescaler);

/**
 * @brief Write Status Register 1.
 *
 * @details
 * This function writes the provided value to Status Register 1.
 *
 * It is typically used for:
 * - Block protection configuration
 * - Write protection control
 * - Status register management
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the value to be written.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 1 written successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Status_Register1(const QSPI_Instance_t *qspi_inst,
                                      uint8_t* statusData,
                                      uint8_t prescaler);

/**
 * @brief Write Status Register 2.
 *
 * @details
 * This function writes the provided value to Status Register 2.
 *
 * It is typically used for:
 * - Quad Enable (QE) configuration
 * - Suspend control bits
 * - Security and protection settings
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the value to be written.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 2 written successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Status_Register2(const QSPI_Instance_t *qspi_inst,
                                      uint8_t* statusData,
                                      uint8_t prescaler);

/**
 * @brief Write Status Register 3.
 *
 * @details
 * This function writes the provided value to Status Register 3.
 *
 * It is typically used for:
 * - Drive strength configuration
 * - Output driver control
 * - Additional device-specific settings
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the value to be written.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Status Register 3 written successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Status_Register3(const QSPI_Instance_t *qspi_inst,
                                      uint8_t* statusData,
                                      uint8_t prescaler);

/* ============================================================
* SPECIAL FEATURE OPERATIONS
* ============================================================*/

/**
 * @brief Read Global Freeze Bit.
 *
 * @details
 * This function reads the Global Freeze Bit from the flash device.
 *
 * The Global Freeze Bit is used to lock protection-related
 * configuration and prevent accidental modification of
 * protection registers until the next reset or power cycle.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer where the Global Freeze Bit value will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Global Freeze Bit read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Global_Freeze_Bit(const QSPI_Instance_t *qspi_inst,
                                      uint8_t* data,
                                      uint8_t prescaler);

/**
 * @brief Set or clear Global Freeze Bit.
 *
 * @details
 * This function writes the Global Freeze Bit to enable or
 * disable protection register freezing.
 *
 * Value options:
 * - 0 : Clear Global Freeze Bit
 * - 1 : Set Global Freeze Bit
 *
 * Once enabled, modification of protection settings may be
 * restricted until reset depending on flash device behavior.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Value to be programmed:
 * - 0U : Clear
 * - 1U : Set
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : Global Freeze Bit updated successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Write_Global_Freeze_Bit(const QSPI_Instance_t *qspi_inst,
                                       uint8_t data,
                                       uint8_t prescaler);

/* ============================================================
* FLASH IDENTIFICATION & CONFIGURATION
* ============================================================*/

/**
 * @brief Read Serial Flash Discoverable Parameters (SFDP).
 *
 * @details
 * This function reads data from the SFDP table of the flash device.
 *
 * SFDP provides standard flash parameter information such as:
 * - Supported commands
 * - Erase sizes
 * - Addressing modes
 * - Timing requirements
 * - Device capabilities
 *
 * The read starts from the specified SFDP address.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] address
 * SFDP table start address to read from.
 *
 * @param[out] data
 * Pointer where the read SFDP byte will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : SFDP data read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_SFDP(const QSPI_Instance_t *qspi_inst,
                         uint32_t address,
                         uint8_t* data,
                         uint8_t prescaler);

/**
 * @brief Read JEDEC ID.
 *
 * @details
 * This function reads the JEDEC identification of the flash device.
 *
 * The returned JEDEC ID typically contains:
 * - Manufacturer ID
 * - Memory Type
 * - Memory Capacity
 *
 * This is commonly used for flash device detection and validation.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] id
 * Pointer to a 3-byte buffer used to store the JEDEC ID.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : JEDEC ID read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_Jedec_ID(const QSPI_Instance_t *qspi_inst,
                             uint8_t *id,
                             uint8_t prescaler);

/**
 * @brief Read Non-Volatile Configuration Register (NVCR).
 *
 * @details
 * This function reads the Non-Volatile Configuration Register (NVCR)
 * from the flash device.
 *
 * NVCR stores persistent configuration settings such as:
 * - Dummy cycle configuration
 * - Output driver strength
 * - Reset and protocol behavior
 * - Device-specific operating modes
 *
 * These settings are retained across power cycles and reset.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[out] data
 * Pointer to the buffer where NVCR contents will be stored.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : NVCR read successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 */
uint16_t Flash_Read_NVCR(const QSPI_Instance_t *qspi_inst,
                         uint8_t* data,
                         uint8_t prescaler);

/**
 * @brief Write Non-Volatile Configuration Register (NVCR).
 *
 * @details
 * This function writes data to the Non-Volatile Configuration
 * Register (NVCR) of the flash device.
 *
 * NVCR is used to configure persistent flash settings such as:
 * - Dummy cycles
 * - Driver strength
 * - Reset behavior
 * - Operating protocol settings
 *
 * Since this is a non-volatile write, changes are retained
 * after reset and power cycle.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] data
 * Pointer to the data to be written to NVCR.
 *
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return * - SUCCESS : NVCR written successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * @ref Flash_Write_Enable() must be called before writing NVCR.
 */
uint16_t Flash_Write_NVCR(const QSPI_Instance_t *qspi_inst,
                          uint8_t* data,
                          uint8_t prescaler);

/* ============================================================
* MEMORY MODES
* ============================================================*/

/**
 * @brief Initialize flash in Execute-In-Place (XIP) mode.
 *
 * @details
 * This function configures the QSPI peripheral in
 * Memory-Mapped Mode (MMM) with XIP mode enabled,
 * allowing direct access to external flash memory
 * through the processor address space.
 *
 * After successful initialization:
 * - Flash contents can be accessed like normal memory
 * - Instructions can be executed directly from flash
 * - No explicit read command is required for access
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @param[in] flash_mem_size
 * Flash size configuration value used to program
 * the flash_mem_size field of the QSPI DCR register.
 *
 * Example:
 * If flash size is 4 MB:
 * flash_mem_size = 21
 * because:
 * Memory Size = 2^(flash_mem_size + 1)
 * 
 * @param[in] prescaler
 * Clock prescaler value used to divide the QSPI input clock.
 *
 * QSPI clock frequency is computed as:
 *   QSPI_CLK = BASE_CLK / (prescaler + 1)
 *
 * where BASE_CLK is the QSPI peripheral input clock (700 Mhz).
 *
 * @note
 * - The configured QSPI clock must not exceed the maximum
 *   frequency (75 MHz) supported by the external flash device.
 *
 * @return
 * - SUCCESS : XIP mode initialized successfully
 * - Other error codes returned by @ref QSPI_Transaction()
 *
 * @note
 * Before starting a new indirect transaction after XIP mode,
 * the user must call @ref QSPI_Abort_Transaction()
 * to exit memory-mapped mode and allow reconfiguration.
 */
uint16_t Flash_XIP_Init(const QSPI_Instance_t *qspi_inst,
                        size_t flash_mem_size,
                        uint8_t prescaler);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INCLUDE_QSPI_FLASH_H_ */

