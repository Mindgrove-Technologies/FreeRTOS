/**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. 
 * All rights reserved.
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
 * @file dma.h
 * @brief Contains driver APIs for DMA Controller Interface
 * @details Provides the API for configuring and controlling the 
 *          Direct Memory Access (DMA) controller, including channel 
 *          configuration, interrupt management, transfer setup, and 
 *          status monitoring.
 * @version 1.0
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Narasimha R V (narasimha@mindgrovetech.in)
 * @date 27-11-2024
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by        | Description
 * -----------|---------|--------------------|-------------------------------
 * 27-11-2024 | 1.0     | Vishwajith N S     | Initial release
 * 27-02-2026 | 1.1     | Narasimha R V      | Code refactoring, MISRA 
 *            |         |                    | compliance improvements and 
 *            |         |                    | API enhancements
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_DMA_H_
#define BSP_INCLUDE_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "errors.h"
#include "stdbool.h"
#include "secure_iot.h"

/* Peripheral register offsets used for DMA address calculation */

#define AES_OUT_OFFSET                  0x40U
#define SHA_OUT_OFFSET                  0x80U
#define RSA_OUT_OFFSET                  0x80U
#define QSPI_DATA_OFFSET                0x20U
#define UART_TX_OFFSET                  0x4U
#define UART_RX_OFFSET                  0x8U
#define SPI_TX_OFFSET                   0x8U
#define SPI_RX_OFFSET                   0xCU
#define ITRACE_DATA_OFFSET              0x40U
#define ADC_DATA_OFFSET                 0x4U
#define PRO_IO_DUO_DATA_OFFSET          0x70U
#define PRO_IO_TETRA_DATA_OFFSET        0x78U
#define PRO_IO_OCTA_DATA_OFFSET         0x80U
#define PRO_IO_FUSION_DATA_OFFSET       0x88U

/**
 * @defgroup DMA_Peripheral_Addresses DMA Peripheral Register Addresses
 * @brief Macros defining peripheral data register addresses used by DMA.
 *
 * These macros provide base addresses for various peripheral data registers.
 * They can be used in DMA configuration for source and destination addresses.
 *
 * @note Users should use these macros instead of hardcoding addresses.
 * @{
 */

/* ================= CRYPTO PERIPHERALS ================= */

/** @brief AES input data register address */
#define AES_INP_REG_ADDR        (AES_BASE)

/** @brief SHA input data register address */
#define SHA_INP_REG_ADDR        (SHA256_BASE)

/** @brief RSA input data register address */
#define RSA_INP_REG_ADDR        (RSA_BASE)

/** @brief AES output data register address */
#define AES_OUT_REG_ADDR        (AES_BASE + AES_OUT_OFFSET)

/** @brief SHA output data register address */
#define SHA_OUT_REG_ADDR        (SHA256_BASE + SHA_OUT_OFFSET)

/** @brief RSA output data register address */
#define RSA_OUT_REG_ADDR        (RSA_BASE + RSA_OUT_OFFSET)


/* ================= QSPI ================= */

/** @brief QSPI0 data register address */
#define QSPI0_DATA_REG_ADDR     (QUADSPI0_BASE + QSPI_DATA_OFFSET)

/** @brief QSPI1 data register address */
#define QSPI1_DATA_REG_ADDR     (QUADSPI1_BASE + QSPI_DATA_OFFSET)


/* ================= UART TX/RX REGISTERS ================= */

/** @brief UART0 TX register address */
#define UART0_TX_REG_ADDR       (UART0_BASE + UART_TX_OFFSET)

/** @brief UART1 TX register address */
#define UART1_TX_REG_ADDR       (UART1_BASE + UART_TX_OFFSET)

/** @brief UART2 TX register address */
#define UART2_TX_REG_ADDR       (UART2_BASE + UART_TX_OFFSET)

/** @brief UART3 TX register address */
#define UART3_TX_REG_ADDR       (UART3_BASE + UART_TX_OFFSET)

/** @brief UART4 TX register address */
#define UART4_TX_REG_ADDR       (UART4_BASE + UART_TX_OFFSET)

/** @brief UART0 RX register address */
#define UART0_RX_REG_ADDR       (UART0_BASE + UART_RX_OFFSET)

/** @brief UART1 RX register address */
#define UART1_RX_REG_ADDR       (UART1_BASE + UART_RX_OFFSET)

/** @brief UART2 RX register address */
#define UART2_RX_REG_ADDR       (UART2_BASE + UART_RX_OFFSET)

/** @brief UART3 RX register address */
#define UART3_RX_REG_ADDR       (UART3_BASE + UART_RX_OFFSET)

/** @brief UART4 RX register address */
#define UART4_RX_REG_ADDR       (UART4_BASE + UART_RX_OFFSET)


/* ================= SPI TX/RX REGISTERS ================= */

/** @brief SPI0 TX register address */
#define SPI0_TX_REG_ADDR        (SPI0_BASE + SPI_TX_OFFSET)

/** @brief SPI1 TX register address */
#define SPI1_TX_REG_ADDR        (SPI1_BASE + SPI_TX_OFFSET)

/** @brief SPI2 TX register address */
#define SPI2_TX_REG_ADDR        (SPI2_BASE + SPI_TX_OFFSET)

/** @brief SPI3 TX register address */
#define SPI3_TX_REG_ADDR        (SPI3_BASE + SPI_TX_OFFSET)

/** @brief SPI0 RX register address */
#define SPI0_RX_REG_ADDR        (SPI0_BASE + SPI_RX_OFFSET)

/** @brief SPI1 RX register address */
#define SPI1_RX_REG_ADDR        (SPI1_BASE + SPI_RX_OFFSET)

/** @brief SPI2 RX register address */
#define SPI2_RX_REG_ADDR        (SPI2_BASE + SPI_RX_OFFSET)

/** @brief SPI3 RX register address */
#define SPI3_RX_REG_ADDR        (SPI3_BASE + SPI_RX_OFFSET)


/* ================= OTHER PERIPHERALS ================= */

/** @brief Instruction trace data register address */
#define ITRACE_DATA_REG_ADDR    (ITRACE_RAM_BASE + ITRACE_DATA_OFFSET)

/** @brief ADC data register address */
#define ADC_DATA_REG_ADDR       (ADC_BASE + ADC_DATA_OFFSET)


/* ================= PRO IO ============================= */

/** @brief Pro IO Duo data register address */
#define PRO_IO_DUO_DATA_REG_ADDR     (GPIO_BASE + PRO_IO_DUO_DATA_OFFSET)

/** @brief Pro IO Tetra data register address */
#define PRO_IO_TETRA_DATA_REG_ADDR   (GPIO_BASE + PRO_IO_TETRA_DATA_OFFSET)

/** @brief Pro IO Octa data register address */
#define PRO_IO_OCTA_DATA_REG_ADDR    (GPIO_BASE + PRO_IO_OCTA_DATA_OFFSET)

/** @brief Pro IO Fusion data register address */
#define PRO_IO_FUSION_DATA_REG_ADDR  (GPIO_BASE + PRO_IO_FUSION_DATA_OFFSET)

/** @} */ /* end of DMA_Peripheral_Addresses */

/* ============================================================
 *                       Enumerations
 * ============================================================ */

/**
 * @brief DMA channel identifiers.
 *
 * Represents the available DMA channels in the controller.
 */
typedef enum {
    DMA_CHANNEL_0 = 0,  /**< DMA Channel 0 */
    DMA_CHANNEL_1,      /**< DMA Channel 1 */
    DMA_CHANNEL_2,      /**< DMA Channel 2 */
    DMA_CHANNEL_3,      /**< DMA Channel 3 */
    DMA_CHANNEL_4,      /**< DMA Channel 4 */
    DMA_CHANNEL_5,      /**< DMA Channel 5 */
    DMA_CHANNEL_6,      /**< DMA Channel 6 */
    DMA_CHANNEL_7       /**< DMA Channel 7 */
} DMA_Channel_t;

/**
 * @brief DMA channel priority levels.
 *
 * Defines the priority assigned to a DMA channel.
 */
typedef enum {
    DMA_PRIORITY_LOW = 0,      /**< Low priority */
    DMA_PRIORITY_MEDIUM,       /**< Medium priority */
    DMA_PRIORITY_HIGH,         /**< High priority */
    DMA_PRIORITY_VERY_HIGH     /**< Very high priority */
} DMA_Priority_Levels;

/**
 * @brief DMA data transfer size.
 *
 * Represents the width of each DMA transfer unit.
 */
typedef enum {
    DMA_BYTE = 0,        /**< 8-bit transfer  */
    DMA_TWOBYTE,         /**< 16-bit transfer */
    DMA_FOURBYTE,        /**< 32-bit transfer */
    DMA_EIGHTBYTE        /**< 64-bit transfer */
} DMA_Data_Size_t;

typedef enum {
    DMA_QSPI_UNKNOWN = 0,
    DMA_QSPI_FLASH,
    DMA_QSPI_PSRAM
} dma_qspi_type_t;

/* ============================================================
 *                       Structures
 * ============================================================ */

/**
 * @brief DMA configuration structure.
 *
 * This structure holds all parameters required for DMA transfer
 * setup including source/destination addresses, transfer size,
 * priority and data width.
 */
typedef struct {
    /**
     * @brief Source address.
     *
     * Pointer to the source location for DMA transfer.
     *
     * - Can be a memory address or a peripheral register address.
     * - For peripheral transfers, this should point to the
     *   peripheral data register (e.g., UART RX, SPI RX).
     *
     * @note
     * - Users are recommended to use the predefined macros
     *   provided in this driver for peripheral addresses
     *   (e.g., UARTx_RX_REG_ADDR, SPIx_RX_REG_ADDR) instead of
     *   hardcoding raw addresses.
     * - Ensure the address is cast to the appropriate type
     *   (uint32_t *) before assignment.
     */
    uint32_t *src_addr;

    /**
     * @brief Destination address.
     *
     * Pointer to the destination location for DMA transfer.
     *
     * - Can be a memory address or a peripheral register address.
     * - For peripheral transfers, this should point to the
     *   peripheral data register (e.g., UART TX, SPI TX).
     *
     * @note
     * Users are recommended to use the predefined macros
     * provided in this driver for peripheral addresses
     * (e.g., UARTx_TX_ADDR, SPIx_TX_ADDR) instead of
     * hardcoding raw addresses.
     * - Ensure the address is cast to the appropriate type
     *   (uint32_t *) before assignment.
     */
    uint32_t *dest_addr;

    /**
     * @brief Transfer length.
     *
     * Specifies the total number of bytes to be transferred.
     *
     * @note
     * - This value is specified in bytes.
     * - Must be aligned with the selected data size:
     *   - DMA_BYTE      : any value
     *   - DMA_TWOBYTE   : multiple of 2
     *   - DMA_FOURBYTE  : multiple of 4
     *   - DMA_EIGHTBYTE : multiple of 8
     * - Incorrect alignment may lead to data corruption or transfer errors.
     * - Maximum transfer length accepted is \(2^{24} - 1\) (UINT24_MAX).
     * - Values exceeding 24 bits are masked off (upper 8 bits ignored).
     */
    uint32_t transfer_length;

    /**
     * @brief DMA channel priority level.
     *
     * Determines the priority of the DMA channel during
     * arbitration when multiple channels request access.
     *
     * Higher priority channels are serviced first.
     */
    DMA_Priority_Levels priority;

    /**
     * @brief Source data width.
     *
     * Specifies the data width for each transfer from source.
     *
     * Valid values:
     * - DMA_BYTE      : 8-bit transfer
     * - DMA_TWOBYTE   : 16-bit transfer
     * - DMA_FOURBYTE  : 32-bit transfer
     * - DMA_EIGHTBYTE : 64-bit transfer
     */
    uint8_t src_data_size;

    /**
     * @brief Destination data width.
     *
     * Specifies the data width for each transfer to destination.
     *
     * Valid values:
     * - DMA_BYTE      : 8-bit transfer
     * - DMA_TWOBYTE   : 16-bit transfer
     * - DMA_FOURBYTE  : 32-bit transfer
     * - DMA_EIGHTBYTE : 64-bit transfer
     */
    uint8_t dest_data_size;

    /**
     * @brief DMA channel number.
     *
     * Selects the DMA channel to be used for the transfer.
     *
     * Valid values:
     * - DMA_CHANNEL_0 to DMA_CHANNEL_7
     */
    DMA_Channel_t chn_no;

    /**
     * @brief External flash size.
     *
     * Specifies the total size of the external QSPI flash memory.
     *
     * - Used to dynamically determine the valid flash address range
     *   for DMA memory detection.
     *
     * @note
     * - This value must be provided in bytes.
     * - The valid flash address range is computed as:
     *   [DMA_FLASH_START_ADDR, DMA_FLASH_START_ADDR + flash_size - 1]
     * - Ensure this value correctly matches the actual flash size
     *   configured in the system to avoid invalid memory access.
     */
    uint32_t flash_size;

    /**
     * @brief External PSRAM size.
     *
     * Specifies the total size of the external PSRAM memory.
     *
     * - Used to dynamically determine the valid PSRAM address range
     *   for DMA memory detection.
     *
     * @note
     * - This value must be provided in bytes.
     * - The valid PSRAM address range is computed as:
     *   [DMA_PSRAM_START_ADDR, DMA_PSRAM_START_ADDR + psram_size - 1]
     * - Ensure this value correctly matches the actual PSRAM size
     *   configured in the system to avoid invalid memory access.
     */
    uint32_t psram_size;

    /**
     * @brief QSPI destination device type for DMA transfer.
     *
     * Indicates the type of memory device connected to the QSPI
     * destination address. This field is used by the DMA driver
     * to enforce access restrictions, particularly to prevent
     * invalid write operations to Flash memory.
     *
     * @details
     * - When the destination address falls within a QSPI memory region,
     *   the DMA driver relies on this field to determine whether the
     *   target device is Flash or PSRAM.
     *
     * - If the destination corresponds to QSPI Flash:
     *      - DMA write operations are NOT permitted.
     *      - The driver will reject the transfer and return an error.
     *
     * - If the destination corresponds to QSPI PSRAM:
     *      - DMA write operations are allowed.
     *
     * @usage
     * - This field MUST be explicitly set by the user when the destination
     *   address lies within a QSPI region.
     *
     * - For non-QSPI destination addresses (e.g., RAM or peripherals),
     *   this field MUST be set to DMA_QSPI_UNKNOWN.
     *
     * @validation
     * - If the destination address is within QSPI range AND:
     *      - dest_qspi_type == DMA_QSPI_UNKNOWN
     *        → The driver will return an error (invalid configuration).
     *
     *      - dest_qspi_type == DMA_QSPI_FLASH
     *        → The driver will return an error (write to Flash not allowed).
     *
     *      - dest_qspi_type == DMA_QSPI_PSRAM
     *        → The transfer is allowed.
     *
     * @note
     * - The DMA driver does NOT perform runtime detection of the connected
     *   QSPI device (Flash/PSRAM).
     * - The correctness of this field is entirely the responsibility of
     *   the user/application layer.
     * - Providing incorrect information may lead to invalid memory access
     *   or undefined system behavior.
     */
    dma_qspi_type_t dest_qspi_type;
} DMA_Config_t;

/* ============================================================
 *                       API Prototypes
 * ============================================================ */

/**

* @brief Get interrupt status for a DMA channel.
*
* Reads the interrupt status register corresponding to the
* selected DMA channel and returns the status flags.
*
* The interrupt status is returned in the lower 4 bits of @p dma_status,
* where each bit represents a specific interrupt condition. Multiple
* interrupt conditions may be set simultaneously.
*
* Possible status values:
* * 0x0 : No interrupt pending
* * 0x1 : Global interrupt flag
* * 0x2 : Transfer complete
* * 0x4 : Half transfer reached
* * 0x8 : Transfer error occurred
* * Combination of above values indicates multiple events
*
* @param[in]  dma_config   Pointer to DMA configuration structure.
* @param[out] dma_status   Pointer to store interrupt status bits.
*
* @retval SUCCESS   Operation successful
* @retval ECHRNG    Invalid DMA channel number
* @retval EFAULT    Null pointer passed
  */
uint16_t DMA_Interrupt_Status(const DMA_Config_t *dma_config,
                              uint8_t *dma_status);

/**
 * @brief Clear DMA interrupt flags.
 *
 * Clears the selected interrupt flags for the given DMA channel.
 *
 * @param[in] dma_config                  Pointer to DMA configuration structure.
 * @param[in] transfer_error_int_flag     Set to true to clear transfer error flag.
 * @param[in] half_transfer_int_flag      Set to true to clear half-transfer flag.
 * @param[in] transfer_complete_int_flag  Set to true to clear transfer complete flag.
 * @param[in] global_int_flag             Set to true to clear global interrupt flag.
 *
 * @retval SUCCESS   Operation successful
 * @retval ECHRNG    Invalid DMA channel number
 * @retval EFAULT    Null pointer passed
 */
uint16_t DMA_Clear_Interrupt_Flags(const DMA_Config_t *dma_config,
                                   bool transfer_error_int_flag,
                                   bool half_transfer_int_flag,
                                   bool transfer_complete_int_flag,
                                   bool global_int_flag);

/**
 * @brief Enable selected DMA interrupts.
 *
 * Enables the specified interrupt sources for the given DMA channel.
 *
 * @param[in] dma_config                 Pointer to DMA configuration structure.
 * @param[in] transfer_error_int_en      Enable transfer error interrupt.
 * @param[in] half_transfer_int_en       Enable half-transfer interrupt.
 * @param[in] transfer_complete_int_en   Enable transfer complete interrupt.
 *
 * @retval SUCCESS   Operation successful
 * @retval ECHRNG    Invalid DMA channel number
 * @retval EFAULT    Null pointer passed
 */
uint16_t DMA_Enable_Interrupts(const DMA_Config_t *dma_config,
                               bool transfer_error_int_en,
                               bool half_transfer_int_en,
                               bool transfer_complete_int_en);

/**
 * @brief Disable selected DMA interrupts.
 *
 * Disables the specified interrupt sources for the given DMA channel.
 *
 * @param[in] dma_config                 Pointer to DMA configuration structure.
 * @param[in] transfer_error_int_en      Disable transfer error interrupt.
 * @param[in] half_transfer_int_en       Disable half-transfer interrupt.
 * @param[in] transfer_complete_int_en   Disable transfer complete interrupt.
 *
 * @retval SUCCESS   Operation successful
 * @retval ECHRNG    Invalid DMA channel number
 * @retval EFAULT    Null pointer passed
 */
uint16_t DMA_Disable_Interrupts(const DMA_Config_t *dma_config,
                                bool transfer_error_int_en,
                                bool half_transfer_int_en,
                                bool transfer_complete_int_en);

/**
 * @brief Get DMA channel status.
 *
 * Retrieves the current enable/disable status of the DMA channel.
 *
 * @param[in]  dma_config Pointer to DMA configuration structure.
 * @param[out] state      Pointer to store channel state.
 *                        - 0 : Channel disabled
 *                        - 1 : Channel enabled
 *
 * @retval SUCCESS     Operation successful
 * @retval ECHRNG      Invalid DMA channel number
 * @retval EFAULT      Null pointer passed
 * @retval ETIMEDOUT   Timeout while checking channel state
 */
uint16_t DMA_Channel_Status(const DMA_Config_t *dma_config,
                            uint8_t *state);

/**
 * @brief Set DMA channel state (Enable/Disable).
 *
 * Enables or disables the specified DMA channel.
 *
 * @param[in] dma_config Pointer to DMA configuration structure.
 * @param[in] enable     Channel state control:
 *                       - true  : Enable channel
 *                       - false : Disable channel
 *
 * @retval SUCCESS     Operation successful
 * @retval ECHRNG      Invalid DMA channel number
 * @retval EFAULT      Null pointer passed
 * @retval ETIMEDOUT   Timeout while enabling/disabling channel
 */
uint16_t DMA_Channel_Set_State(const DMA_Config_t *dma_config,
                               bool enable);

/**
 * @brief Configure DMA transfer parameters.
 *
 * Configures the DMA channel with source and destination addresses,
 * transfer length, priority level, and data width.
 *
 * @param[in] dma_config Pointer to DMA configuration structure.
 *
 * @note
 * - Channel must be disabled before calling this function.
 * - Transfer length must be aligned with data size.
 *
 * @retval SUCCESS     Operation successful
 * @retval ECHRNG      Invalid DMA channel number
 * @retval EFAULT      Null pointer passed
 * @retval ETIMEDOUT   Timeout while waiting for channel disable
 * @retval EINVAL      Invalid configuration parameters
 */
uint16_t DMA_Transfer_Configure(const DMA_Config_t *dma_config);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_DMA_H_
