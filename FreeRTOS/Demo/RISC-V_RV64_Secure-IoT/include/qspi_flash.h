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

/* ============================================================
 * READ OPERATIONS
 * ============================================================*/

/**
 * @brief Perform Fast Quad Read operation (1-1-4 mode).
 *
 * Sends instruction over single line, address over single line,
 * and reads data over four data lines.
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param data       Buffer to store read data.
 * @param address    Flash start address.
 * @param data_length Number of bytes to read.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Fast_Read_Quad(const QSPI_Instance_t *qspi_inst,
                              uint8_t *data,
                              uint32_t address,
                              uint8_t data_length);

/**
 * @brief Perform Fast Quad I/O Read operation (1-4-4 mode).
 *
 * Sends instruction over single line, address over four lines,
 * and reads data over four data lines.
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param data       Buffer to store read data.
 * @param address    Flash start address.
 * @param data_length Number of bytes to read.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Fast_Read_Quad_IO(const QSPI_Instance_t *qspi_inst,
                                 uint8_t *data,
                                 uint32_t address,
                                 uint8_t data_length);

/**
 * @brief Perform Standard Single Line Read (1-1-1 mode).
 *
 * Instruction, address and data are transferred over a single line.
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param data       Buffer to store read data.
 * @param address    Flash start address.
 * @param data_length Number of bytes to read.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Fast_Read_Single(const QSPI_Instance_t *qspi_inst,
                                uint8_t *data,
                                uint32_t address,
                                uint8_t data_length);

/* ============================================================
 * PROGRAM OPERATIONS
 * ============================================================*/

/**
 * @brief Program flash page using Quad mode (1-1-4).
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param data       Data buffer to program.
 * @param address    Flash start address.
 * @param data_length Number of bytes to program.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Input_Page_Quad(const QSPI_Instance_t *qspi_inst,
                               uint8_t *data,
                               uint32_t address,
                               uint8_t data_length);

/**
 * @brief Program flash page using Single line mode (1-1-1).
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param data       Data buffer to program.
 * @param address    Flash start address.
 * @param data_length Number of bytes to program.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Input_Page_Single(const QSPI_Instance_t *qspi_inst,
                                 uint8_t *data,
                                 uint32_t address,
                                 uint8_t data_length);

/* ============================================================
 * ERASE OPERATIONS
 * ============================================================*/

/**
 * @brief Erase 4KB sector.
 *
 * This function initiates erase and returns immediately.
 * Use status register polling to check completion.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param address   Sector start address.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Sector_4K_Erase(const QSPI_Instance_t *qspi_inst,
                               uint32_t address);

/**
 * @brief Erase 32KB block.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param address   Block start address.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Sector_32K_Erase(const QSPI_Instance_t *qspi_inst,
                                uint32_t address);

/**
 * @brief Perform complete chip erase.
 *
 * Blocks internally until erase operation completes.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Chip_Erase(const QSPI_Instance_t *qspi_inst);

/* ============================================================
 * CONTROL COMMANDS
 * ============================================================*/

/**
 * @brief Enable flash write operations.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Enable(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Disable flash write operations.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Disable(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Suspend ongoing erase/program operation.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Suspend(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Resume suspended erase/program operation.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Resume(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Put flash into deep power-down mode.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Power_Down(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Release flash from deep power-down mode.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Release_Power_Down(const QSPI_Instance_t *qspi_inst);

/* ============================================================
 * STATUS REGISTER OPERATIONS
 * ============================================================*/

/**
 * @brief Read Status Register 1.
 *
 * Retrieves the contents of Status Register 1 from the flash device.
 * Typically contains BUSY bit, Write Enable Latch (WEL) and protection bits.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to buffer where status byte will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Status_Register1(const QSPI_Instance_t *qspi_inst,
                                     uint8_t *data);

/**
 * @brief Read Status Register 2.
 *
 * Retrieves the contents of Status Register 2 from the flash device.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to buffer where status byte will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Status_Register2(const QSPI_Instance_t *qspi_inst,
                                     uint8_t *data);

/**
 * @brief Read Status Register 3.
 *
 * Retrieves the contents of Status Register 3 from the flash device.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to buffer where status byte will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Status_Register3(const QSPI_Instance_t *qspi_inst,
                                     uint8_t *data);

/**
 * @brief Read Flag Status Register.
 *
 * Retrieves the Flag Status Register which indicates program/erase
 * completion and error conditions.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to buffer where flag status byte will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Flag_Status_Register(const QSPI_Instance_t *qspi_inst,
        uint8_t *data);

/**
 * @brief Enable write access to status registers.
 *
 * Must be called before modifying any status register.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Enable_Status_Register(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Write Status Register 1.
 *
 * Programs Status Register 1 with provided value.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to value to be written.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Status_Register1(const QSPI_Instance_t *qspi_inst,
                                      uint8_t *data);

/**
 * @brief Write Status Register 2.
 *
 * Programs Status Register 2 with provided value.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to value to be written.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Status_Register2(const QSPI_Instance_t *qspi_inst,
                                      uint8_t *data);

/**
 * @brief Write Status Register 3.
 *
 * Programs Status Register 3 with provided value.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to value to be written.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Status_Register3(const QSPI_Instance_t *qspi_inst,
                                      uint8_t *data);

/* ============================================================
 * SPECIAL FEATURE OPERATIONS
 * ============================================================*/

/**
 * @brief Read Global Freeze Bit.
 *
 * Retrieves the state of the Global Freeze bit.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer where freeze bit value will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Global_Freeze_Bit(const QSPI_Instance_t *qspi_inst,
                                      uint8_t *data);

/**
 * @brief Set or clear Global Freeze Bit.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Value to program (0 = clear, 1 = set).
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_Global_Freeze_Bit(const QSPI_Instance_t *qspi_inst,
                                       uint8_t data);

/* ============================================================
 * FLASH IDENTIFICATION & CONFIGURATION
 * ============================================================*/

/**
 * @brief Read Serial Flash Discoverable Parameters (SFDP).
 *
 * Reads SFDP table entry from specified address.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param address   SFDP table address.
 * @param data      Pointer where read byte will be stored.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_SFDP(const QSPI_Instance_t *qspi_inst,
                         uint32_t address,
                         uint8_t *data);

/**
 * @brief Read JEDEC ID.
 *
 * Retrieves manufacturer ID, memory type and capacity.
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param id        Pointer to 3-byte buffer to store JEDEC ID.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_Jedec_ID(const QSPI_Instance_t *qspi_inst,
                             uint8_t *id);

/**
 * @brief Read Non-Volatile Configuration Register (NVCR).
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to buffer to store NVCR contents.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Read_NVCR(const QSPI_Instance_t *qspi_inst,
                         uint8_t *data);

/**
 * @brief Write Non-Volatile Configuration Register (NVCR).
 *
 * @param qspi_inst Pointer to QSPI hardware instance.
 * @param data      Pointer to data to be written to NVCR.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_Write_NVCR(const QSPI_Instance_t *qspi_inst,
                          uint8_t *data);

/* ============================================================
 * MEMORY MODES
 * ============================================================*/

/**
 * @brief Initialize Flash in Execute-In-Place (XIP) mode.
 *
 * Configures the QSPI peripheral in Memory-Mapped Mode (MMM)
 * to allow direct execution of code from external flash memory.
 * After initialization, the flash contents can be accessed
 * directly through the mapped memory region.
 *
 * @param qspi_inst  Pointer to QSPI hardware instance.
 * @param flash_size Flash size configuration value used to
 *                   program the FSIZE field of QSPI.
 *
 * @return SUCCESS on success, error code otherwise.
 */
uint16_t Flash_XIP_Init(const QSPI_Instance_t *qspi_inst,
                        uint32_t flash_size);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INCLUDE_QSPI_FLASH_H_ */
