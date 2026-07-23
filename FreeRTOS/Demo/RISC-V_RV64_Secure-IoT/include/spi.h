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
 * @file  spi.h
 * @brief Contains the declarations for the SPI interface.
 * @details This header file provides the function prototypes, macro definitions,
 *          and data types required for configuring and implementing the SPI driver.
 * @version 1.1
 * @authors Harini P (harinip@mindgrovetech.in)
 * @date 15-08-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 20-07-2023 | 1.0     | Kishore. J            | Initial release.              
 * 15-08-2025 | 1.1     | Harini P              | Optimised the SPI driver and 
 *            |         |                       | aligned it with MISRA-C
 *            |         |                       | guidelines.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_SPI_H_
#define BSP_INCLUDE_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @enum SPI_FIFO_Interrupt_t
 * @brief SPI FIFO Interrupt Enable values.
 *
 * This enumeration defines the interrupt enable bit values for
 * TX FIFO and RX FIFO threshold levels. These values can be
 * combined using bitwise OR to enable multiple FIFO interrupts.
 */
typedef enum {
    /** The value of this enum is 1U (ie. 1 << 0).
     * It enables TX FIFO empty interrupt.
     */
    TX_FIFO_INTR_EMPTY = 1U,

    /** The value of this enum is 2U (ie. 1 << 1).
     * It enables TX FIFO dual interrupt.
     */
    TX_FIFO_INTR_DUAL  = 2U,

    /** The value of this enum is 4U (ie. 1 << 2).
     * It enables TX FIFO quad interrupt.
     */
    TX_FIFO_INTR_QUAD  = 4U,

    /** The value of this enum is 8U (ie. 1 << 3).
     * It enables TX FIFO octal interrupt.
     */
    TX_FIFO_INTR_OCTAL = 8U,

    /** The value of this enum is 16U (ie. 1 << 4).
     * It enables TX FIFO half interrupt.
     */
    TX_FIFO_INTR_HALF  = 16U,

    /** The value of this enum is 32U (ie. 1 << 5).
     * It enables TX FIFO 24-level interrupt.
     */
    TX_FIFO_INTR_24    = 32U,

    /** The value of this enum is 64U (ie. 1 << 6).
     * It enables TX FIFO 28-level interrupt.
     */
    TX_FIFO_INTR_28    = 64U,

    /** The value of this enum is 128U (ie. 1 << 7).
     * It enables TX FIFO 30-level interrupt.
     */
    TX_FIFO_INTR_30    = 128U,

    /** The value of this enum is 256U (ie. 1 << 8).
     * It enables TX FIFO full interrupt.
     */
    TX_FIFO_INTR_FULL  = 256U,

    /** The value of this enum is 512U (ie. 1 << 9).
     * It enables RX FIFO empty interrupt.
     */
    RX_FIFO_INTR_EMPTY = 512U,

    /** The value of this enum is 1024U (ie. 1 << 10).
     * It enables RX FIFO dual interrupt.
     */
    RX_FIFO_INTR_DUAL  = 1024U,

    /** The value of this enum is 2048U (ie. 1 << 11).
     * It enables RX FIFO quad interrupt.
     */
    RX_FIFO_INTR_QUAD  = 2048U,

    /** The value of this enum is 4096U (ie. 1 << 12).
     * It enables RX FIFO octal interrupt.
     */
    RX_FIFO_INTR_OCTAL = 4096U,

    /** The value of this enum is 8192U (ie. 1 << 13).
     * It enables RX FIFO half interrupt.
     */
    RX_FIFO_INTR_HALF  = 8192U,

    /** The value of this enum is 16384U (ie. 1 << 14).
     * It enables RX FIFO 24-level interrupt.
     */
    RX_FIFO_INTR_24    = 16384U,

    /** The value of this enum is 32768U (ie. 1 << 15).
     * It enables RX FIFO 28-level interrupt.
     */
    RX_FIFO_INTR_28    = 32768U,

    /** The value of this enum is 65536U (ie. 1 << 16).
     * It enables RX FIFO 30-level interrupt.
     */
    RX_FIFO_INTR_30    = 65536U,

    /** The value of this enum is 131072U (ie. 1 << 17).
     * It enables RX FIFO full interrupt.
     */
    RX_FIFO_INTR_FULL  = 131072U
} SPI_FIFO_Interrupt_t;


/**
 * @enum SPI_DMA_Size
 * @brief SPI DMA transfer size identifiers.
 *
 * This enumeration defines the available bit-widths for Direct Memory Access (DMA)
 * transfers within the SPI module.
 */
typedef enum {
    /** The value of this enum is 1U. It represents 8-bit data */
    SIZE_8  = 1U,
    /** The value of this enum is 2U. It represents 16-bit data */
    SIZE_16 = 2U,
    /** The value of this enum is 3U. It represents 32-bit data */
    SIZE_32 = 3U,
    /** The value of this enum is 4U. It represents 64-bit data */
    SIZE_64 = 4U
} SPI_DMA_Size;


/**
 * @enum SPI_Clk_Mode
 * @brief SPI Clock Polarity and Phase modes.
 *
 * This enumeration defines the standard SPI clock modes based on
 * CPOL and CPHA settings.
 */
typedef enum {
    /** The value of this enum is 0U. It represents CPOL = 0, CPHA = 0 */
    MODE_0,
    /** The value of this enum is 1U. It represents CPOL = 0, CPHA = 1 
     * (Not supported) */
    MODE_1,
    /** The value of this enum is 2U. It represents CPOL = 1, CPHA = 0 
     * (Not supported) */
    MODE_2,
    /** The value of this enum is 3U. It represents CPOL = 1, CPHA = 1 */
    MODE_3
} SPI_Clk_Mode;


/**
 * @enum SPI_Comm_Mode
 * @brief SPI Communication Directionality.
 *
 * This enumeration defines the data flow direction for the SPI transaction.
 */
typedef enum {
    /** The value of this enum is 0U. It represents Transmit only */
    TX = 0U,
    /** The value of this enum is 1U. It represents Receive only */
    RX = 1U,
    /** The value of this enum is 2U. It represents Transmit then Receive */
    HALF_DUPLEX = 2U,
    /** The value of this enum is 3U. It represents Simultaneous Transmit and 
     *  Receive */
    FULL_DUPLEX = 3U
} SPI_Comm_Mode;

/**
 * @enum SPI_IRQn_Type
 * 
 * @brief SPI interrupt ID
 * 
 * This enumeration defines the available SPI interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  SPI0_IRQn                 =  62,              /*!< 62 SPI0                                                                   */
  SPI1_IRQn                 =  63,              /*!< 63 SPI1                                                                   */
  SPI2_IRQn                 =  64,              /*!< 64 SPI2                                                                   */
  SPI3_IRQn                 =  65,              /*!< 65 SPI3                                                                   */
} SPI_IRQn_Type;

/**
 * @defgroup SPI_Instance_Type SPI Instance Handle Type
 * @brief Opaque SPI hardware instance handle type.
 *
 * This module defines the SPI instance handle type used by the SPI driver.
 * The type is opaque and represents a specific SPI hardware block.
 *
 * Users must not create objects of this type directly. Valid instances
 * must only be obtained using the provided SPI instance constructors
 * or SPI instance macros.
 *
 * @{
 */

/**
 * @brief Opaque SPI hardware instance type.
 *
 * This type represents a specific SPI hardware block on the SoC.
 * The internal structure is hidden from the user and is only known
 * to the SPI driver implementation.
 *
 * @note This type must only be used as a handle.
 *       Do not attempt to allocate or define objects of this type.
 */
typedef struct SPI_Instance SPI_Instance_t;

/** @} */  /* end of SPI_Instance_Type */


/**
 * @defgroup SPI_Instance_Constructors SPI Instance Constructors
 * @brief SPI hardware instance constructors.
 *
 * These functions return opaque handles to SPI hardware instances.
 * The returned handles must be passed to the SPI driver configuration.
 *
 * @{
 */

/**
 * @brief Get handle for SPI instance 0.
 * @return Pointer to SPI0 hardware instance descriptor.
 */
const SPI_Instance_t *SPI_INSTANCE_0(void);

/**
 * @brief Get handle for SPI instance 1.
 * @return Pointer to SPI1 hardware instance descriptor.
 */
const SPI_Instance_t *SPI_INSTANCE_1(void);

/**
 * @brief Get handle for SPI instance 2.
 * @return Pointer to SPI2 hardware instance descriptor.
 */
const SPI_Instance_t *SPI_INSTANCE_2(void);

/**
 * @brief Get handle for SPI instance 3.
 * @return Pointer to SPI3 hardware instance descriptor.
 */
const SPI_Instance_t *SPI_INSTANCE_3(void);

/** @} */  /* end of SPI_Instance_Constructors */


/**
 * @defgroup SPI_Instance_Macros SPI Instance Macros
 * @brief SPI hardware instance macros.
 *
 * These macros expand to constant SPI hardware instance handles.
 * They provide a convenient and readable way to select SPI instances.
 *
 * @{
 */
 
/**
 * @def SPI0
 * @brief Macro for SPI instance 0 handle.
 */
#define SPI0 SPI_INSTANCE_0()

/**
 * @def SPI1
 * @brief Macro for SPI instance 1 handle.
 */
#define SPI1 SPI_INSTANCE_1()

/**
 * @def SPI2
 * @brief Macro for SPI instance 2 handle.
 */
#define SPI2 SPI_INSTANCE_2()

/**
 * @def SPI3
 * @brief Macro for SPI instance 3 handle.
 */
#define SPI3 SPI_INSTANCE_3()

/** @} */  /* end of SPI_Instance_Macros */


/**
 * @brief SPI configuration structure.
 *
 * @details This structure contains all parameters required to initialize
 *          and configure an SPI peripheral instance.
 */
typedef struct {
    /**
     *  @brief SPI clock frequency in mHz.
     */
    uint32_t spi_freq;

    /**
     * @brief SPI hardware instance handle.
     *
     * This parameter selects the SPI hardware block to be used.
     * Valid instances are `SPI1`, `SPI2`, `SPI3` and `SPI4`.
     */
    const SPI_Instance_t *spi_num;

    /**
     * @brief Clock polarity and phase mode.
     * @note Uses @ref SPI_ClkMode_t (`MODE_0` or `MODE_3`).
     *       `MODE_1` and `MODE_2` are not supported. 
     */
    SPI_Clk_Mode spi_clk_mode : 2;

    /** 
     * @brief Setup time before data transmission.
     */
    uint8_t setup_time;

    /**
     *  @brief Hold time after data transmission.
     */
    uint8_t hold_time;

    /** 
     * @brief SPI operating mode: `true` if slave mode or 
     *                            `false` if master mode.
     */
    bool is_slave_mode;

    /** 
     * @brief Bit order: `true` if lsb first or
     *                   `false` if msb first.
     */
    bool is_lsb;

    /**
     *  @brief Communication mode: `TX`, `RX`, `HALF_DUPLEX`, `FULL_DUPLEX`.
     */
    SPI_Comm_Mode comm_mode    : 2;

    /**
     *  @brief Number of bits to be transferred per frame (8, 16, 32).
     */
    uint8_t spi_size;

    /** 
     * @brief Chip select control: `true` if software ncs or
     *                             `false` if `hardware ncs`.
     * @note Ignored if SPI is configured as Slave mode.
     */
    bool is_software_ncs;
} SPI_Config_t;


/**
 * @brief SPI transaction buffer structure.
 *
 * @details This structure manages the data buffers used for 
 * SPI read and write operations.
 */
typedef struct {
    /**
     * @brief Transmit buffer pointer.
     * @note Required if comm_mode is TX, HALF_DUPLEX, or FULL_DUPLEX.
     */
    void *tx_buf;

    /**
     * @brief Receive buffer pointer.
     * @note Required if comm_mode is RX, HALF_DUPLEX, or FULL_DUPLEX.
     */
    void *rx_buf;

    /**
     * @brief Data size in bits (8, 16, or 32).
     * @details Data size to be read/write from/to RX_reg/TX_reg.
     */
    uint8_t data_size;

    /** @brief Number of elements to be transferred. */
    size_t len;
} spi_buffer;


/* Function prototypes */

/**
 * @brief Configures and enables the specified SPI instance for communication.
 * 
 * @details This function sets the SPI parameters such as clock mode, chip select type,
 *          frequency, setup/hold times, communication mode, data size, bit order, and 
 *          master/slave mode based on the provided configuration structure.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure containing all SPI configuration parameters.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Configuration applied successfully.
 * - @ref EFAULT   Returned if a SPI_Config_t structure pointer is passed as NULL.
 * - @ref EINFREQ  Returned if the provided SPI frequency is out of the supported range.
 */
uint16_t SPI_Config(const SPI_Config_t *spi_config);

/**
 * @brief Controls the SPI chip select (NCS) line in software mode. Ignored if 
 *        SPI is configured as Slave mode, used only for SPI Master mode.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * @param ncs_val Boolean value to set the NCS line (0 = deassert, 1 = assert).
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS NCS line state set successfully.
 * - @ref EFAULT Returned if a SPI_Config_t structure pointer is passed as NULL.
 */
uint16_t SPI_Software_NCS(const SPI_Config_t *spi_config, bool ncs_val);

/**
 * @brief Flushes (clears) the RX FIFO buffer of the specified SPI instance.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  RX FIFO flushed successfully.
 * - @ref EFAULT Returned if a SPI_Config_t structure pointer is passed as NULL.
 */
uint16_t SPI_Flush_RX_FIFO(const SPI_Config_t *spi_config);

/**
 * @brief Enables a interrupt bits based on the provided interrupt.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * @param interrupt  FIFO Interrupts can be enabled using the FIFO INTERRUPT ENABLE macros.
 *                   Multiple interrupts can be enabled together by OR-ing the macros
 *                   (e.g., TX_FIFO_INTR_EMPTY | RX_FIFO_INTR_FULL).
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Interrupt(s) enabled successfully.
 * - @ref EFAULT  Returned if a SPI_Config_t structure pointer is passed as NULL.
 */
uint16_t SPI_Interrupt_Enable(const SPI_Config_t *spi_config, \
                                uint32_t  interrupt_type);

/**
 * @brief Waits until SPI transmission is complete.
 * 
 * @details This function continuously monitors the TX FIFO empty status and 
 *          SPI busy flag.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  TX FIFO is empty and SPI is no longer busy.
 * - @ref EFAULT  Returned if a SPI_Config_t structure pointer is passed as NULL.
 * - @ref ETIMEDOUT Returned if the TX FIFO is not empty within the expected 
 *                  time in in slave mode, or if the TX FIFO is not empty or
 *                  the SPI is busy within the timeout period in master mode.
 */
uint16_t SPI_Wait_Till_TX_Complete(const SPI_Config_t *spi_config);

/**
 * @brief Checks the SPI TX FIFO empty and busy status.
 * 
 * @details This function reads the TX FIFO empty flag and SPI busy flag. 
 *          The returned value indicates whether the SPI peripheral is busy and
 *          whether the TX FIFO is empty.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * 
 * @return Returns a byte value where the first 2-bits indicated the status value:
 *         - 00: Not busy, TX FIFO not empty  
 *         - 01: Not busy, TX FIFO empty  
 *         - 10: Busy, TX FIFO not empty  
 *         - 11: Busy, TX FIFO empty  
 */
uint8_t SPI_Check_TX_And_Busy_Status(const SPI_Config_t *spi_config);

/**
 * @brief Disables the specified SPI instance.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  SPI instance disabled successfully.
 * - @ref EFAULT  Returned if a SPI_Config_t structure pointer is passed as NULL.
 */
uint16_t SPI_Disable(const SPI_Config_t *spi_config);

/**
 * @brief Transfers data over SPI in TX, RX, HALF_DUPLEX, or FULL_DUPLEX mode.
 * 
 * @details This function performs SPI data transmission and/or reception depending on the
 *          configured communication mode in \a spi_config. It supports master and slave 
 *          modes with data sizes of 8, 16, or 32 bits.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance
 *                   and communication mode.
 * @param buf        Pointer to a spi_buffer structure.
 *                   
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  Data transfer completed successfully.
 * - @ref EFAULT  Returned if a SPI_Config_t structure pointer is passed as NULL.
 * - @ref EINVAL  Returned if an invalid data size is provided.
 */
uint16_t SPI_Transceive(const SPI_Config_t *spi_config, const spi_buffer *buf);

/**
 * @brief Configures the DMA transfer size for SPI communication.
 * 
 * @details This function sets the TX and RX DMA transfer sizes based on the provided 
 *          communication mode and data size. The DMA size can be configured between 
 *          8 bits and 64 bits.
 * 
 * @param spi_config Pointer to an SPI_Config_t structure specifying the SPI instance 
 *                   and communication mode.
 * @param size       DMA transfer size @ref SPI_DMA_Size (`SIZE_8`, `SIZE_16`,
 *                                                        `SIZE_32`, `SIZE_64`).
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS  DMA transfer size configured successfully.
 * - @ref EFAULT  Returned if a SPI_Config_t structure pointer is passed as NULL.
 * - @ref EINVAL  Returned if an invalid DMA size is provided.
 */
uint16_t SPI_DMA(const SPI_Config_t *spi_config, SPI_DMA_Size size);

#ifdef __cplusplus
}
#endif
#endif  // BSP_INCLUDE_SPI_H_