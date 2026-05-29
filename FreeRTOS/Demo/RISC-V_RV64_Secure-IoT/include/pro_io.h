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
 * @file pro_io.h
 * @brief Contains driver header for GPIO Interface
 * @details Provides the API for sequential read/write operations for the 
 *          hardware Pro IO.
 * @version 1.1
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 * @date 06-03-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 23-09-2025 | 1.0     | Shri Mahaalakshmi S J | Added sequential read/write 
 *            |         |                       | support to Pro IO APIs
 * 06-03-2026 | 1.1     | Shri Mahaalakshmi S J | Optimized Pro IO APIs and 
 *            |         |                       | enhanced MISRA compilance
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_PRO_IO_H_
#define BSP_INCLUDE_PRO_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @enum PRO_IO_Number
 * @brief Pro IO width selection.
 *
 * This enumeration defines the available Pro IO hardware configurations.
 * Each value represents the specific grouping of GPIO pins.
 */
typedef enum {
    /** The value of this enum is 0U. It represents a DUO Pro IO */
    PRO_IO_DUO    = 0U,

    /** The value of this enum is 1U. It represents a TETRA Pro IO */
    PRO_IO_TETRA  = 1U,

    /** The value of this enum is 2U. It represents an OCTA Pro IO */
    PRO_IO_OCTA   = 2U,

    /** The value of this enum is 3U. It represents a combined FUSION Pro IO */
    PRO_IO_FUSION = 3U
} PRO_IO_Number;

/**
 * @enum PRO_IO_Direction
 * @brief Pro IO data direction.
 *
 * This enumeration defines whether the Pro IO is used
 * for reading data from GPIO pins or writing data to GPIO pins.
 */
typedef enum {
    /** The value of this enum is 0U. Data is enqueued (read from GPIO pins) */
    PRO_IO_READ  = 0U,

    /** The value of this enum is 1U. Data is dequeued (written to GPIO pins) */
    PRO_IO_WRITE = 1U
} PRO_IO_Direction;

/**
 * @enum PRO_IO_Clock_Edge_Select
 * @brief Pro IO clock edge selection.
 *
 * This enumeration specifies the clock edge on which
 * Pro IO data is sampled or driven.
 */
typedef enum {
    /** The value of this enum is 0U. Data is captured on the 
     * positive clock edge */
    CLK_POSITIVE_EDGE = 0U,

    /** The value of this enum is 1U. Data is captured on the 
     * negative clock edge */
    CLK_NEGATIVE_EDGE = 1U
} PRO_IO_Clock_Edge_Select;

/**
 * @enum PRO_IO_Clock_Source
 * @brief Pro IO clock source selection.
 *
 * This enumeration defines whether the Pro IO operates
 * using an internally generated clock or an external clock source.
 */
typedef enum {
    /** The value of this enum is 0U. Internal clock source is used */
    CLK_INTERNAL = 0U,

    /** The value of this enum is 1U. External clock source is used */
    CLK_EXTERNAL = 1U
} PRO_IO_Clock_Source;

/**
 * @enum PRO_IO_Mode
 * @brief Pro IO operating modes.
 *
 * This enumeration defines all valid combinations of
 * clock source and data transfer direction for GPIO pro_ioing.
 */
typedef enum {
    /** The value of this enum is 1U.
     *  External clock is used and data is read from the Pro IO */
    MODE_EXT_CLK_READ  = 1U,

    /** The value of this enum is 2U.
     *  External clock is used and data is written to the Pro IO */
    MODE_EXT_CLK_WRITE = 2U,

    /** The value of this enum is 3U.
     *  Internal clock is used and data is read from the Pro IO */
    MODE_INT_CLK_READ  = 3U,

    /** The value of this enum is 4U.
     *  Internal clock is used and data is written to the Pro IO */
    MODE_INT_CLK_WRITE = 4U
} PRO_IO_Mode;

/**
 * @brief Pro IO configuration structure.
 *
 * @details This structure contains all parameters required to configure
 *          a Pro IO for parallel data transfer. It defines the pro_io
 *          width, clock source, clock edge, data direction, and data size
 *          used during pro_ioed GPIO operations.
 */
typedef struct {
    /**
     * @brief Pro IO width selection.
     *
     * This parameter selects the number of GPIO pins grouped together
     * to form a pro_io interface.
     *
     * @note Uses @ref PRO_IO_Number.
     *       Valid values are:
     *          - PRO_IO_DUO(0)
     *          - PRO_IO_TETRA(1)
     *          - PRO_IO_OCTA(2)
     *          - PRO_IO_FUSION(3)
     */
    uint8_t pro_io_num;

    /**
     * @brief Pro IO clock prescaler value.
     *
     * This parameter defines the prescaler applied to the internal
     * clock source used by the Pro IO.
     *
     * @note This field is used only when @ref clk_sel is
     *       set to CLK_INTERNAL.
     */
    uint32_t prescale;

    /**
     * @brief Pro IO data width selection.
     *
     * This parameter specifies the number of bits transferred
     * per pro_io operation.
     *
     * @note Uses @ref PRO_IO_Data_Size.
     *       Valid values are:
     *       - DATA_SIZE_8(1)
     *       - DATA_SIZE_16(2)
     *       - DATA_SIZE_32(3)
     */
    uint32_t data_size;

    /**
     * @brief Pro IO data transfer direction.
     *
     * This parameter selects whether data is read from GPIO pins
     * into the pro_io or written from the pro_io to GPIO pins.
     *
     * @note Uses @ref PRO_IO_Direction.
     *       Valid values are:
     *       - PRO_IO_READ(0)
     *       - PRO_IO_WRITE(1)
     */
    uint8_t direction;

    /**
     * @brief Pro IO clock edge selection.
     *
     * This parameter defines the clock edge on which data is
     * sampled during pro_io operation.
     *
     * @note Uses @ref GPIO_Clock_Edge_Select.
     *       Valid values are:
     *       - CLK_POSITIVE_EDGE(0)
     *       - CLK_NEGATIVE_EDGE(1)
     */
    uint8_t clk_edge_sel;

    /**
     * @brief Pro IO clock source selection.
     *
     * This parameter selects whether the Pro IO operates
     * using an internal clock or an external clock source.
     *
     * @note Uses @ref GPIO_Clock_Source.
     *       Valid values are:
     *       - CLK_INTERNAL(0)
     *       - CLK_EXTERNAL(1)
     */
    uint8_t clk_sel;

    /**
     * @brief Pro IO operating mode.
     *
     * This parameter defines the complete pro_io operating mode
     * by combining the clock source and data transfer direction.
     *
     * @note Uses @ref PRO_IO_Mode.
     *       Valid values are:
     *       - MODE_EXT_CLK_READ(1)
     *       - MODE_EXT_CLK_WRITE(2)
     *       - MODE_INT_CLK_READ(3)
     *       - MODE_INT_CLK_WRITE(4)
     */
    uint8_t mode;

    /**
     * @brief Pro IO operation timeout value.
     *
     * This parameter defines the maximum number of CPU cycles to wait 
     * for hardware status flags (e.g., Buffer Ready or Not Full) 
     * before aborting an operation.
     *
     * @note This value is compared against the RISC-V `mcycle` CSR 
     * to ensure non-blocking execution during hardware polling.
     */
    uint64_t timeout;
} PRO_IO_Struct_t;

/**
 * @brief The function `PRO_IO_Config` configures the Pro IO hardware based on 
 * the provided configuration parameters.
 * 
 * @param config Pointer to a constant @ref PRO_IO_Struct_t structure.
 * This parameter defines the operational parameters for the Pro IO hardware, 
 * including: such as pro_io number, mode, data size, direction, clock edge selection, 
 * clock selection, prescale value.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS: Configuration applied successfully.
 * - @ref EFAULT: The provided config pointer is NULL.
 * - @ref EBUFFERNUM: pro_io_num is out of range (0-3). Check if the 
 * correct instance ID is passed.
 * - @ref EINVALSIZE: data_size is invalid. Ensure it is DATA_SIZE_8, 
 * DATA_SIZE_16, or DATA_SIZE_32.
 * - @ref EBITSEL: Invalid selection for direction, clock edge, or clock 
 * source (must be binary 0 or 1).
 * - @ref EINVALMODE: The operation mode is outside the allowed range (1-4).
 */
uint16_t PRO_IO_Config(PRO_IO_Struct_t *config);

/**
 * @brief The function `PRO_IO_Write` writes data to a specified register based 
 * on the configuration provided.
 * 
 * @param config Pointer to a constant @ref PRO_IO_Struct_t structure.
 * This parameter defines the operational parameters for the Pro IO hardware, 
 * including: such as pro_io number, data size.
 * 
 * @param data The `data` parameter is used to write data to a specific register 
 * based on the configuration provided in the `config` parameter.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS: Configuration applied successfully.
 * - @ref EJUKEBOX: Timeout occurred while waiting for the Pro IO FIFO/Status 
 * ready flag.
 * - @ref EBUFFERNUM: pro_io_num is out of range (0-3). Check if the 
 * correct instance ID is passed.
 * - @ref EINVALSIZE: data_size is invalid. Ensure it is DATA_SIZE_8, 
 * DATA_SIZE_16, or DATA_SIZE_32.
 */
uint16_t PRO_IO_Write(PRO_IO_Struct_t *config, uint32_t data);

/**
 * @brief The function `PRO_IO_Read` reads data from a specified PRO_IO port 
 * based on the configuration provided.
 * 
 * @param config Pointer to a constant @ref PRO_IO_Struct_t structure.
 * This parameter defines the operational parameters for the Pro IO hardware, 
 * including: such as pro_io number,data size.
 * 
 * @param rx_data The `rx_data` parameter read from the Pro IO register
 * will be stored. The function reads data from the Pro IO register based on the
 * configuration provided and stores it in the `rx_data
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS: Configuration applied successfully.
 * - @ref EJUKEBOX: Timeout occurred while waiting for the Pro IO FIFO/Status 
 * ready flag.
 * - @ref EBUFFERNUM: pro_io_num is out of range (0-3). Check if the 
 * correct instance ID is passed.
 * - @ref EINVALSIZE: data_size is invalid. Ensure it is DATA_SIZE_8, 
 * DATA_SIZE_16, or DATA_SIZE_32.
 */
uint16_t PRO_IO_Read(PRO_IO_Struct_t *config, uint32_t *rx_data);

/**
 * @brief The function `PRO_IO_Wait_Till_tx` waits until the specified PRO_IO 
 * channel is ready for transmission.
 * 
 * @param config Pointer to a constant @ref PRO_IO_Struct_t structure.
 * This parameter defines the operational parameters for the Pro IO hardware, 
 * such as pro_io number.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS: Configuration applied successfully.
 * - @ref EJUKEBOX: Timeout occurred while waiting for the Pro IO FIFO/Status 
 * ready flag.
 * - @ref EBUFFERNUM: pro_io_num is out of range (0-3). Check if the 
 * correct instance ID is passed.
 */
uint16_t PRO_IO_Wait_Till_tx(PRO_IO_Struct_t *config);

/**
 * @brief The function PRO_IO_Disable disables a specific PRO_IO pin based on 
 * the provided configuration.
 * 
 * @param config Pointer to a constant @ref PRO_IO_Struct_t structure.
 * This parameter defines the operational parameters for the Pro IO hardware, 
 * such as pro_io number.
 * 
 * @return Returns a 16-bit status code:
 * - @ref SUCCESS: Configuration applied successfully.
 * - @ref EBUFFERNUM: pro_io_num is out of range (0-3). Check if the 
 * correct instance ID is passed.
 */
uint16_t PRO_IO_Disable(PRO_IO_Struct_t *config);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_PRO_IO_H_
