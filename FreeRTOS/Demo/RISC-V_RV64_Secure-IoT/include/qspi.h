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
 * @file    qspi.h
 * @brief   QSPI peripheral driver interface
 * @details Provides configuration structures, macros and API declarations for
 *          controlling the Quad SPI peripheral including command sequencing,
 *          indirect read/write transfers, memory mapped access and status 
 *          polling.
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

#ifndef BSP_INCLUDE_QSPI_H_
#define BSP_INCLUDE_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @defgroup qspi_general_macros General QSPI Macros
 * @brief Common constants used in QSPI driver.
 * @{
 */

/**
 * @brief Maximum allowed QSPI clock frequency.
 */
#define MAX_QSPI_FREQ     75000000UL

/**
 * @brief QSPI write operation.
 */
#define QSPI_WRITE        (0U)

/**
 * @brief QSPI read operation.
 */
#define QSPI_READ         (1U)

/**
 * @brief QSPI FIFO full level.
 */
#define QSPI_FIFO_FULL    (16U)

/**
 * @brief QSPI FIFO empty level.
 */
#define QSPI_FIFO_EMPTY   (0U)

/** @} */

/**
 * @defgroup qspi_ccr_mode_macros CCR Mode Macros
 * @brief QSPI Communication Configuration Register (CCR) mode selection macros.
 * @{
 */

/**
 * @brief No instruction phase.
 */
#define CCR_IMODE_NIL          (0x0U)

/**
 * @brief Instruction on single line.
 */
#define CCR_IMODE_SINGLE_LINE  (0x1U)

/**
 * @brief Instruction on two lines.
 */
#define CCR_IMODE_TWO_LINE     (0x2U)

/**
 * @brief Instruction on four lines.
 */
#define CCR_IMODE_FOUR_LINE    (0x3U)


/**
 * @brief No address phase.
 */
#define CCR_ADMODE_NIL          (0x0U)

/**
 * @brief Address on single line.
 */
#define CCR_ADMODE_SINGLE_LINE  (0x1U)

/**
 * @brief Address on two lines.
 */
#define CCR_ADMODE_TWO_LINE     (0x2U)

/**
 * @brief Address on four lines.
 */
#define CCR_ADMODE_FOUR_LINE    (0x3U)


/**
 * @brief 8-bit address size.
 */
#define CCR_ADSIZE_8_BIT   (0x0U)

/**
 * @brief 16-bit address size.
 */
#define CCR_ADSIZE_16_BIT  (0x1U)

/**
 * @brief 24-bit address size.
 */
#define CCR_ADSIZE_24_BIT  (0x2U)

/**
 * @brief 32-bit address size.
 */
#define CCR_ADSIZE_32_BIT  (0x3U)

/** @} */

/**
 * @defgroup qspi_ccr_alt_data_macros Alternate Byte and Data Mode Macros
 * @brief QSPI alternate byte, data mode, and functional mode selection macros.
 * @{
 */

/**
 * @brief No alternate byte phase.
 */
#define CCR_ABMODE_NIL          (0x0U)

/**
 * @brief Alternate byte on single line.
 */
#define CCR_ABMODE_SINGLE_LINE  (0x1U)

/**
 * @brief Alternate byte on two lines.
 */
#define CCR_ABMODE_TWO_LINE     (0x2U)

/**
 * @brief Alternate byte on four lines.
 */
#define CCR_ABMODE_FOUR_LINE    (0x3U)


/**
 * @brief 8-bit alternate byte size.
 */
#define CCR_ABSIZE_8_BIT   (0x0U)

/**
 * @brief 16-bit alternate byte size.
 */
#define CCR_ABSIZE_16_BIT  (0x1U)

/**
 * @brief 24-bit alternate byte size.
 */
#define CCR_ABSIZE_24_BIT  (0x2U)

/**
 * @brief 32-bit alternate byte size.
 */
#define CCR_ABSIZE_32_BIT  (0x3U)


/**
 * @brief No data phase.
 */
#define CCR_DMODE_NO_DATA      (0x0U)

/**
 * @brief Data on single line.
 */
#define CCR_DMODE_SINGLE_LINE  (0x1U)

/**
 * @brief Data on two lines.
 */
#define CCR_DMODE_TWO_LINE     (0x2U)

/**
 * @brief Data on four lines.
 */
#define CCR_DMODE_FOUR_LINE    (0x3U)


/**
 * @brief Indirect write mode.
 */
#define CCR_FMODE_INDIRECT_WRITE  (0x0U)

/**
 * @brief Indirect read mode.
 */
#define CCR_FMODE_INDIRECT_READ   (0x1U)

/**
 * @brief Automatic polling mode.
 */
#define CCR_FMODE_APM             (0x2U)

/**
 * @brief Memory mapped mode.
 */
#define CCR_FMODE_MMM             (0x3U)

/** @} */
/**
 * @defgroup qspi_memory_poll_macros Memory Mapped and Poll Match Macros
 * @brief QSPI memory mapped mode and polling match mode selection macros.
 * @{
 */

/**
 * @brief XIP (Execute In Place) mode.
 */
#define CCR_MM_MODE_XIP  (0x0U)

/**
 * @brief RAM mode.
 */
#define CCR_MM_MODE_RAM  (0x1U)

/**
 * @brief AND match mode for automatic polling.
 */
#define PMM_AND  (0U)

/**
 * @brief OR match mode for automatic polling.
 */
#define PMM_OR   (1U)

/** @} */

/* ============================================================
 * Register Bitfield Macros
 * ============================================================*/

/**
 * @defgroup qspi_register_macros Register Bitfield Macros
 * @brief Bitfield helper macros for QSPI registers (CR, DCR, SR, FCR, CCR, RMC).
 * @{
 */

/* ---------------- CR ---------------- */

/** @brief FIFO threshold mask (CR[10:7]). */
#define CR_FTHRES_MASK   (0xFU << 7U)

/** @brief Set clock prescaler (CR[31:18]). */
#define CR_PRESCALER(x)  ((uint32_t)(x) << 18U)

/** @brief Poll match mode (CR[17]). */
#define CR_PMM(x)        ((uint32_t)(x) << 17U)

/** @brief Auto-polling mode stop (CR[16]). */
#define CR_APMS(x)       ((uint32_t)(x) << 16U)

/** @brief Timeout interrupt enable (CR[15]). */
#define CR_TOIE(x)       ((uint32_t)(x) << 15U)

/** @brief Status match interrupt enable (CR[14]). */
#define CR_SMIE(x)       ((uint32_t)(x) << 14U)

/** @brief FIFO threshold interrupt enable (CR[13]). */
#define CR_FTIE(x)       ((uint32_t)(x) << 13U)

/** @brief Transfer complete interrupt enable (CR[12]). */
#define CR_TCIE(x)       ((uint32_t)(x) << 12U)

/** @brief Transfer error interrupt enable (CR[11]). */
#define CR_TEIE(x)       ((uint32_t)(x) << 11U)

/** @brief Set FIFO threshold (CR[10:7]). */
#define CR_FTHRES(x)     ((uint32_t)(x) << 7U)

/** @brief Timeout counter enable (CR[3]). */
#define CR_TCEN(x)       ((uint32_t)(x) << 3U)

/** @brief Abort ongoing transaction (CR[1]). */
#define CR_ABORT(x)      ((uint32_t)(x) << 1U)

/** @brief Enable QSPI peripheral (CR[0]). */
#define CR_EN(x)         ((uint32_t)(x) << 0U)


/* ---------------- DCR ---------------- */

/** @brief Mode byte (DCR[28:21]). */
#define DCR_MODE_BYTE(x) ((uint32_t)(x) << 21U)

/** @brief Flash size (DCR[20:16]). */
#define DCR_FSIZE(x)     ((uint32_t)(x) << 16U)

/** @brief Chip select high time (DCR[10:8]). */
#define DCR_CSHT(x)      ((uint32_t)(x) << 8U)

/** @brief Clock mode (DCR[0]). */
#define DCR_CKMODE(x)    ((uint32_t)(x))


/* ---------------- SR ---------------- */

/** @brief FIFO level (SR[8]). */
#define SR_FLEVEL   (1U << 8)

/** @brief Busy flag (SR[5]). */
#define SR_BUSY     (1U << 5)

/** @brief Timeout flag (SR[4]). */
#define SR_TOF      (1U << 4)

/** @brief Status match flag (SR[3]). */
#define SR_SMF      (1U << 3)

/** @brief FIFO threshold flag (SR[2]). */
#define SR_FTF      (1U << 2)

/** @brief Transfer complete flag (SR[1]). */
#define SR_TCF      (1U << 1)

/** @brief Transfer error flag (SR[0]). */
#define SR_TEF      (1U << 0)


/* ---------------- FCR ---------------- */

/** @brief Clear timeout flag. */
#define FCR_CTOF  (1U << 4)

/** @brief Clear status match flag. */
#define FCR_CSMF  (1U << 3)

/** @brief Clear transfer complete flag. */
#define FCR_CTCF  (1U << 1)

/** @brief Clear transfer error flag. */
#define FCR_CTEF  (1U << 0)


/* ---------------- CCR ---------------- */

/** @brief DDR mode (CCR[31]). */
#define CCR_DDRM(x)                ((uint32_t)(x) << 31U)

/** @brief Memory mapped mode select (CCR[30]). */
#define CCR_MM_MODE(x)             ((uint32_t)(x) << 30U)

/** @brief Dummy bit enable (CCR[29]). */
#define CCR_DUMMY_BIT(x)           ((uint32_t)(x) << 29U)

/** @brief Send instruction only once (CCR[28]). */
#define CCR_SIOO(x)                ((uint32_t)(x) << 28U)

/** @brief Functional mode (CCR[27:26]). */
#define CCR_FMODE(x)               ((uint32_t)(x) << 26U)

/** @brief Data mode (CCR[25:24]). */
#define CCR_DMODE(x)               ((uint32_t)(x) << 24U)

/** @brief Dummy phase enable (CCR[23]). */
#define CCR_DUMMY_CONFIRMATION(x)  ((uint32_t)(x) << 23U)

/** @brief Dummy cycles (CCR[22:18]). */
#define CCR_DCYC(x)                ((uint32_t)(x) << 18U)

/** @brief Alternate byte size (CCR[17:16]). */
#define CCR_ABSIZE(x)              ((uint32_t)(x) << 16U)

/** @brief Alternate byte mode (CCR[15:14]). */
#define CCR_ABMODE(x)              ((uint32_t)(x) << 14U)

/** @brief Address size (CCR[13:12]). */
#define CCR_ADSIZE(x)              ((uint32_t)(x) << 12U)

/** @brief Address mode (CCR[11:10]). */
#define CCR_ADMODE(x)              ((uint32_t)(x) << 10U)

/** @brief Instruction mode (CCR[9:8]). */
#define CCR_IMODE(x)               ((uint32_t)(x) << 8U)

/** @brief Instruction opcode (CCR[7:0]). */
#define CCR_INSTRUCTION(x)         ((uint32_t)(x))


/* ---------------- RMC ---------------- */

/** @brief Write dummy cycles (RMC[25:21]). */
#define RMC_WDCYC(x)   ((uint32_t)(x) << 21U)

/** @brief Read dummy cycles (RMC[20:16]). */
#define RMC_RDCYC(x)   ((uint32_t)(x) << 16U)

/** @brief Write instruction (RMC[15:8]). */
#define RMC_WINSTR(x)  ((uint32_t)(x) << 8U)

/** @brief Read instruction (RMC[7:0]). */
#define RMC_RINSTR(x)  ((uint32_t)(x))

/** @} */

/* ============================================================
 * Instance Handles
 * ============================================================*/

/**
 * @defgroup QSPI_Instance_Type QSPI Instance Handle Type
 * @brief Opaque QSPI hardware instance handle type.
 *
 * This module defines the QSPI instance handle type used by the QSPI driver.
 * The type is opaque and represents a specific QSPI hardware block.
 *
 * Users must not create objects of this type directly. Valid instances
 * must only be obtained using the provided QSPI instance constructors
 * or QSPI instance macros.
 *
 * @{
 */

/**
 * @brief Opaque QSPI hardware instance type.
 *
 * This type represents a specific QSPI hardware block on the SoC.
 * The internal structure is hidden from the user and is only known
 * to the QSPI driver implementation.
 *
 * @note This type must only be used as a handle.
 *       Do not attempt to allocate or define objects of this type.
 */
typedef struct QSPI_Instance QSPI_Instance_t;

/**
 * @brief Get handle for QSPI instance 0.
 *
 * @return Pointer to QSPI0 hardware instance descriptor.
 */
const QSPI_Instance_t *QSPI_INSTANCE_0(void);

/**
 * @brief Get handle for QSPI instance 1.
 *
 * @return Pointer to QSPI1 hardware instance descriptor.
 */
const QSPI_Instance_t *QSPI_INSTANCE_1(void);

/** @} */ /* end of QSPI_Instance_Type */

/**
 * @defgroup QSPI_Instance_Macros QSPI Instance Macros
 * @brief QSPI hardware instance macros.
 *
 * These macros expand to constant QSPI hardware instance handles.
 * They provide a convenient and readable way to select QSPI instances.
 *
 * @{
 */

/**
 * @def QSPI0
 * @ingroup QSPI_Instance_Macros
 * @brief Macro for QSPI instance 0 handle.
 */
#define QSPI0   QSPI_INSTANCE_0()

/**
 * @def QSPI1
 * @ingroup QSPI_Instance_Macros
 * @brief Macro for QSPI instance 1 handle.
 */
#define QSPI1   QSPI_INSTANCE_1()

/** @} */ /* end of QSPI_Instance_Macros */

/**
 * @enum QSPI_IRQn_Type
 * 
 * @brief QSPI interrupt ID
 * 
 * This enumeration defines the available QSPI interrupt numbers supported by the platform.
*/
typedef enum {
/* =========================================  Secure_IoT Specific Interrupt Numbers  ========================================= */
  QUADSPI0_IRQn             =  58,              /*!< 58 QUADSPI0                                                               */
  QUADSPI0_READY_IRQn       =  59,              /*!< 59 QUADSPI0_READY                                                         */
  QUADSPI1_IRQn             =  60,              /*!< 60 QUADSPI1                                                               */
  QUADSPI1_READY_IRQn       =  61,              /*!< 61 QUADSPI1_READY                                                         */
} QSPI_IRQn_Type;

/**
 * @brief QSPI transaction configuration structure.
 *
 * This structure encapsulates all configuration parameters required
 * to perform a QSPI transaction including:
 *  - Instruction phase configuration
 *  - Address phase configuration
 *  - Alternate byte configuration
 *  - Dummy cycle configuration
 *  - Data phase configuration
 *  - Functional mode selection
 *  - Memory mapped mode configuration
 *  - Interrupt and control settings
 *  - Status polling configuration
 *
 * The structure is passed to QSPI_Transaction() to execute a transfer.
 */

typedef struct {
/**
 * @brief QSPI hardware instance.
 *
 * Pointer to the QSPI peripheral instance used for the transaction.
 *
 * This identifies which QSPI controller is used for the current
 * operation (for example, QSPI0).
 */
const QSPI_Instance_t *qspi_inst;

/**
 * @brief Data buffer pointer.
 *
 * Pointer to the transmit or receive buffer used for
 * QSPI data transfer.
 *
 * - For write operations: source buffer
 * - For read operations: destination buffer
 *
 * @note
 * This must not be NULL when data_mode is enabled.
 */
uint8_t *data_buffer;

/**
 * @brief Target memory address.
 *
 * Specifies the flash or PSRAM address used for the transaction.
 *
 * This field is used only when address mode is enabled.
 * The address size is controlled by the address_size field.
 */
uint32_t address;

/**
 * @brief Alternate byte value.
 *
 * Specifies the alternate byte transmitted during the
 * alternate-byte phase of the QSPI transaction.
 *
 * This is commonly used in:
 * - XIP mode
 * - Memory-mapped mode
 * - Vendor-specific special access modes
 */
uint32_t alternate_byte;

/**
 * @brief Transfer length.
 *
 * Specifies the total number of bytes to be transferred
 * during the QSPI transaction.
 *
 * This field is used only when a data phase is present.
 *
 * @note
 * - For indirect read/write mode, this defines the exact
 *   number of bytes transferred.
 * - For memory-mapped mode, this field is typically not used.
 */
uint32_t length;

/**
 * @brief Status register mask value.
 *
 * Specifies the bit mask used during automatic polling mode.
 *
 * Only the bits enabled in this mask are compared against
 * the status_match value.
 *
 * Commonly used for monitoring:
 * - WIP (Write In Progress) bit
 * - WEL (Write Enable Latch) bit
 * - QE (Quad Enable) bit
 * - Suspend/Resume status bits
 *
 * Example:
 * - status_mask = 0x01U → monitor WIP bit only
 */
uint32_t status_mask;

/**
 * @brief Status register match value.
 *
 * Specifies the expected value used during automatic
 * polling mode.
 *
 * The masked bits from the received status register are
 * compared against this value to determine whether the
 * polling condition is satisfied.
 *
 * Example:
 * - status_mask  = 0x01U
 * - status_match = 0x00U
 *
 * This means:
 * Wait until WIP bit becomes 0 (operation complete)
 */
uint32_t status_match;

/**
 * @brief Functional mode.
 *
 * Specifies the functional mode of QSPI operation.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_FMODE_INDIRECT_WRITE : Indirect write mode
 * - CCR_FMODE_INDIRECT_READ  : Indirect read mode
 * - CCR_FMODE_APM            : Automatic polling mode
 * - CCR_FMODE_MMM            : Memory-mapped mode
 */
uint8_t functional_mode;

/**
 * @brief Instruction opcode.
 *
 * Specifies the instruction/command to be transmitted to the
 * external flash or PSRAM device.
 *
 * Example values:
 * - 0x03 : Normal Read
 * - 0x02 : Page Program
 * - 0x20 : Sector Erase
 * - 0x6B : Fast Read Quad Read
 */
uint8_t instruction;

/**
 * @brief Instruction mode.
 *
 * Specifies the instruction phase mode of operation.
 *
 * Users should use the predefined macros provided by the driver
 * instead of hardcoding raw values.
 *
 * Valid values:
 * - CCR_IMODE_NIL           : No instruction
 * - CCR_IMODE_SINGLE_LINE   : Single line instruction
 * - CCR_IMODE_TWO_LINE      : Two lines instruction
 * - CCR_IMODE_FOUR_LINE     : Four lines instruction
 */
uint8_t instruction_mode;

/**
 * @brief Address mode.
 *
 * Specifies the address phase mode of operation.
 *
 * Users should use the predefined macros provided by the driver
 * instead of hardcoding raw values.
 *
 * Valid values:
 * - CCR_ADMODE_NIL           : No address
 * - CCR_ADMODE_SINGLE_LINE   : Address on single line
 * - CCR_ADMODE_TWO_LINE      : Address on two lines
 * - CCR_ADMODE_FOUR_LINE     : Address on four lines
 */
uint8_t address_mode;

/**
 * @brief Address size.
 *
 * Defines the address size used during the transaction.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_ADSIZE_8_BIT    : 8-bit address
 * - CCR_ADSIZE_16_BIT   : 16-bit address
 * - CCR_ADSIZE_24_BIT   : 24-bit address
 * - CCR_ADSIZE_32_BIT   : 32-bit address
 */
uint8_t address_size;

/**
 * @brief Alternate byte mode.
 *
 * Specifies the alternate-byte phase mode of operation.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_ABMODE_NIL           : No alternate byte
 * - CCR_ABMODE_SINGLE_LINE   : Alternate byte on single line
 * - CCR_ABMODE_TWO_LINE      : Alternate byte on two lines
 * - CCR_ABMODE_FOUR_LINE     : Alternate byte on four lines
 */
uint8_t alternate_byte_mode;

/**
 * @brief Alternate byte size.
 *
 * Specifies the size of alternate byte transfer.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_ABSIZE_8_BIT    : 8-bit
 * - CCR_ABSIZE_16_BIT   : 16-bit
 * - CCR_ABSIZE_24_BIT   : 24-bit
 * - CCR_ABSIZE_32_BIT   : 32-bit
 */
uint8_t alternate_byte_size;

/**
 * @brief Data mode.
 *
 * Specifies the data phase mode of operation.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_DMODE_NO_DATA       : No data phase
 * - CCR_DMODE_SINGLE_LINE   : Single line transaction
 * - CCR_DMODE_TWO_LINE      : Two lines transaction
 * - CCR_DMODE_FOUR_LINE     : Four lines transaction
 */
uint8_t data_mode;

/**
 * @brief Flash memory size.
 *
 * Specifies the external flash memory size configuration.
 *
 * This value is programmed into the FSIZE field of the
 * Device Configuration Register (DCR).
 *
 * Flash size is calculated as:
 *   2^(fmem_size + 1)
 *
 * Example:
 * - fmem_size = 23 ==> 16MB flash
 * - fmem_size = 27 ==> 256MB flash
 */
uint8_t fmem_size;

/**
 * @brief FIFO threshold level.
 *
 * Defines the FIFO threshold value used to trigger
 * the FIFO Threshold Flag (FTF).
 *
 * In indirect mode:
 * - Write mode → FTF is set when FIFO has enough empty space
 * - Read mode → FTF is set when FIFO has enough filled data
 *
 * Valid range:
 * - 0 to 14
 *
 * Hardware interpretation:
 * - If value = x
 * - Threshold = x + 1 bytes
 */
uint8_t fthresh;

/**
 * @brief Chip Select High Time.
 *
 * Specifies the minimum number of clock cycles for which
 * chip select (nCS) remains high between two commands.
 *
 * This helps satisfy external flash timing requirements.
 *
 * Configured using the CSHT field in DCR register.
 */
uint8_t csht;

/**
 * @brief Dummy phase enable.
 *
 * Enables or disables the dummy phase during QSPI communication.
 *
 * Valid values:
 * - 0U : Dummy phase disabled
 * - 1U : Dummy phase enabled
 */
uint8_t dummy_mode :1;

/**
 * @brief Dummy bit control.
 *
 * Controls whether dummy cycles are inserted before
 * the data phase starts.
 *
 * Valid values:
 * - 0U : No dummy cycles
 * - 1U : Dummy cycles enabled
 */
uint8_t dummy_bit :1;


/**
 * @brief Number of dummy cycles.
 *
 * Specifies the duration of the dummy phase.
 *
 * This defines the number of dummy clock cycles inserted
 * before data transfer begins in both SDR and DDR modes.
 *
 * Valid range:
 * - 0 to 31 clock cycles
 */
uint8_t dummy_cycles :5;

/**
 * @brief Send Instruction Only Once (SIOO).
 *
 * Enables the Send Instruction Only Once mode.
 *
 * Valid values:
 * - 0U : Send instruction for every transaction
 * - 1U : Send instruction only for the first transaction
 *
 * This is mainly useful in memory-mapped mode for performance
 * optimization.
 */
uint8_t sioo :1;

/**
 * @brief Memory mapped mode type.
 *
 * Selects the type of memory-mapped mode operation.
 *
 * Users should use the predefined macros provided by the driver.
 *
 * Valid values:
 * - CCR_MM_MODE_XIP   : XIP mode
 * - CCR_MM_MODE_RAM   : RAM mode
 */
uint8_t mm_mode :1;

/**
 * @brief Clock mode.
 *
 * Selects the idle state of the QSPI clock when chip
 * select (nCS) is inactive.
 *
 * Valid values:
 * - 0U : Clock remains low when nCS is high (Mode 0)
 * - 1U : Clock remains high when nCS is high (Mode 3)
 *
 * Can only be modified when BUSY = 0.
 */
uint8_t clk_mode :1;

/**
 * @brief Timeout counter enable.
 *
 * Enables or disables the timeout counter used in
 * memory-mapped mode transactions.
 *
 * Valid values:
 * - 0U : Timeout counter disabled
 * - 1U : Timeout counter enabled
 *
 * When enabled, if no flash activity occurs for the
 * configured timeout period, chip select (nCS) is released
 * automatically and an error flag may be generated.
 *
 * Can only be modified when BUSY = 0.
 */
uint32_t tcen :1;

/**
 * @brief Transfer error interrupt enable.
 *
 * Enables interrupt generation when a transfer error occurs.
 *
 * Valid values:
 * - 0U : Transfer error interrupt disabled
 * - 1U : Transfer error interrupt enabled
 *
 * Errors may include:
 * - Timeout expiration
 * - Communication failure
 * - Invalid transfer completion
 */
uint32_t teie :1;

/**
 * @brief Transfer complete interrupt enable.
 *
 * Enables interrupt generation when a QSPI transfer
 * completes successfully.
 *
 * Valid values:
 * - 0U : Transfer complete interrupt disabled
 * - 1U : Transfer complete interrupt enabled
 */
uint32_t tcie :1;

/**
 * @brief FIFO threshold interrupt enable.
 *
 * Enables interrupt generation when FIFO reaches the
 * configured threshold level.
 *
 * Valid values:
 * - 0U : FIFO threshold interrupt disabled
 * - 1U : FIFO threshold interrupt enabled
 *
 * This is used for efficient data handling during
 * transmit and receive operations.
 */
uint32_t ftie :1;

/**
 * @brief Status match interrupt enable.
 *
 * Enables interrupt generation when received status
 * matches the configured status_match value.
 *
 * Valid values:
 * - 0U : Status match interrupt disabled
 * - 1U : Status match interrupt enabled
 *
 * Commonly used in automatic polling mode.
 */
uint32_t smie :1;

/**
 * @brief Timeout interrupt enable.
 *
 * Enables interrupt generation when the timeout counter
 * expires.
 *
 * Valid values:
 * - 0U : Timeout interrupt disabled
 * - 1U : Timeout interrupt enabled
 *
 * Indicates that the ongoing command sequence exceeded
 * the allowed timeout period.
 */
uint32_t toie :1;

/**
 * @brief Automatic polling mode stop.
 *
 * Controls whether automatic polling stops after
 * a successful match.
 *
 * Valid values:
 * - 0U : Polling stops only by ABORT or disabling QSPI
 * - 1U : Polling stops automatically when match occurs
 *
 * Can only be modified when BUSY = 0.
 */
uint32_t apms :1;

/**
 * @brief Polling match mode.
 *
 * Selects the match logic used during automatic
 * polling mode.
 *
 * Users should use the predefined macros:
 * - PMM_AND
 * - PMM_OR
 *
 * Valid values:
 * - PMM_AND (0U) : AND match mode
 *   Match occurs only when all masked bits match
 *
 * - PMM_OR (1U) : OR match mode
 *   Match occurs when any masked bit matches
 *
 * Can only be modified when BUSY = 0.
 */
uint32_t pmm :1;

/**
 * @brief Clock prescaler.
 *
 * Defines the clock scaling factor used to generate
 * QSPI clock from the interconnect/system clock.
 *
 * QSPI Clock is calculated as:
 *   QSPI_CLK = BASE_CLOCK / (PRESCALER + 1)
 *
 * Minimum allowed value:
 * - 6
 *
 * Example:
 * - PRESCALER = 7 → divide by 8
 */
uint32_t prescaler :14;

/**
 * @brief RAM write mode dummy cycle count.
 *
 * Specifies the number of dummy clock cycles inserted
 * before write data transmission in RAM mode.
 *
 * This field maps to the WDCYC field of the
 * RAM Mode Configuration Register (RMC).
 *
 * Valid range:
 * - 0 to 31 clock cycles
 */
uint32_t wr_dcyc :5;

/**
 * @brief RAM read mode dummy cycle count.
 *
 * Specifies the number of dummy clock cycles inserted
 * before data reception during read transactions
 * in RAM mode.
 *
 * This field maps to the RDCYC field of the
 * RAM Mode Configuration Register (RMC).
 *
 * This is commonly required for fast-read operations
 * where the external memory needs additional latency.
 *
 * Valid range:
 * - 0 to 31 clock cycles
 */
uint32_t rd_dcyc :5;

/**
 * @brief RAM write mode instruction.
 *
 * Specifies the command opcode used for write/program
 * operations in RAM mode.
 *
 * This field maps to the WINST field of the
 * RAM Mode Configuration Register (RMC).
 *
 * Example values:
 * - 0x02 : Page Program
 * - 0x32 : Quad Page Program
 */
uint32_t wr_instr :8;

/**
 * @brief RAM read mode instruction.
 *
 * Specifies the read command opcode used for
 * memory-mapped RAM mode transactions.
 *
 * This field maps to the RINST field of the
 * RAM Mode Configuration Register (RMC).
 *
 * Example values:
 * - 0x03 : Normal Read
 * - 0x0B : Fast Read
 * - 0x6B : Fast Read Quad Output
 */
uint32_t rd_instr :8;
} qspi_msg;

/* ============================================================
 * API Prototypes
 * ============================================================*/

/**
 * @brief Perform a QSPI transaction based on the provided configuration.
 *
 * @details
 * This function configures the QSPI peripheral and executes the requested
 * transaction using the parameters provided in the @p msg structure.
 *
 * Supported functional modes:
 * - Indirect Write Mode
 * - Indirect Read Mode
 * - Automatic Polling Mode (APM)
 * - Memory-Mapped Mode (MMM)
 *   - XIP mode
 *   - RAM mode
 *
 * The function performs the following operations:
 * - Validates input parameters
 * - Verifies QSPI clock frequency against maximum supported frequency
 * - Waits for the peripheral BUSY flag to clear before reconfiguration
 * - Configures CR (Control Register)
 * - Configures DCR (Device Configuration Register)
 * - Clears QSPI status flags
 * - Configures DLR (Data Length Register)
 * - Configures CCR (Communication Configuration Register)
 * - Configures RMC for RAM memory-mapped mode if required
 * - Handles indirect read/write data transfer using optimized
 *   64-bit / 32-bit / 16-bit / 8-bit accesses based on alignment
 * - Waits for FIFO Threshold Flag (FTF)
 * - Waits for Transfer Complete Flag (TCF)
 * - Waits for BUSY bit to clear after transfer completion
 *
 * For memory-mapped mode:
 * - XIP mode is configured directly
 * - RAM mode additionally configures RMC register
 *
 * For Automatic Polling Mode:
 * - Status mask and match registers are configured
 *
 * Timeout protection is implemented for:
 * - Peripheral BUSY wait
 * - FIFO Threshold Flag (FTF)
 * - Transfer Complete Flag (TCF)
 *
 * @param[in] msg
 * Pointer to QSPI transaction configuration structure of type
 * @ref qspi_msg.
 *
 * @return
 * - SUCCESS     : Transaction completed successfully
 * - EFAULT      : Invalid message pointer or NULL data buffer
 * - EPERM       : Invalid prescaler or QSPI clock exceeds maximum limit
 * - EINVAL      : Unsupported functional mode
 * - ETIMEDOUT   : Timeout occurred while waiting for BUSY, FTF, or TCF
 *
 * @note
 * Before starting a new transaction in memory-mapped mode
 * (XIP mode or RAM mode), the user must call
 * @ref QSPI_Abort_Transaction() to abort the existing transaction
 * and allow reconfiguration of the peripheral.
 *
 * @warning
 * If QSPI remains in memory-mapped mode and a new transaction is started
 * without aborting the previous one, the peripheral may remain busy or
 * configuration register writes may not take effect.
 */
uint16_t QSPI_Transaction(qspi_msg *msg);

/**
 * @brief Wait for the QSPI Status Match Flag (SMF) to be set.
 *
 * @details
 * This function waits until the Status Match Flag (SMF) in the
 * QSPI Status Register (SR) is set.
 *
 * The SMF flag is primarily used in Automatic Polling Mode (APM)
 * to indicate that the received status value matches the expected
 * value configured in the match and mask registers.
 *
 * Timeout protection is implemented using @ref Wait_For_Timeout()
 * to prevent indefinite blocking if the expected match does not occur.
 *
 * @param[in] qspi_inst
 * Pointer to the QSPI hardware instance.
 *
 * @return
 * - SUCCESS    : Status match detected successfully
 * - ETIMEDOUT  : Timeout occurred before SMF flag was set
 *
 * @note
 * This function performs blocking polling with timeout protection.
 *
 * @warning
 * Ensure that Automatic Polling Mode (APM) is properly configured
 * before calling this function. If the expected status condition is
 * never met, the function will return ETIMEDOUT.
 */
uint16_t QSPI_SMF_Flag_Check(const QSPI_Instance_t *qspi_inst);

/**
 * @brief Abort the current QSPI transaction in memory-mapped mode.
 *
 * @details This API is used to abort an ongoing QSPI transaction when the
 *          peripheral is operating in Memory-Mapped Mode (MMM), such as
 *          XIP mode or PSRAM mode, before starting a new transaction or
 *          reconfiguring the QSPI peripheral.
 *
 *          In memory-mapped mode, the QSPI controller does not allow direct
 *          reconfiguration of registers like CR and CCR. Therefore, the
 *          current transaction must be aborted first to safely start a new
 *          transaction.
 *
 *          This function sets the ABORT bit in the Control Register (CR)
 *          and waits until the BUSY bit in the Status Register (SR) is
 *          cleared, indicating that the abort operation is complete.
 *
 * @param qspi_inst Pointer to the QSPI instance structure.
 *                  Example: QSPI0
 *
 * @return
 *         SUCCESS     : Transaction aborted successfully
 *         ETIMEDOUT   : Timeout occurred while waiting for BUSY bit to clear
 *
 */
uint16_t QSPI_Abort_Transaction(const QSPI_Instance_t *qspi_inst);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_QSPI_H_
