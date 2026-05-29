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
#include "log.h"
#include "secure_iot.h"
#include "errors.h"

/* ============================================================
 * General Constants
 * ============================================================*/
#define QSPI_WRITE     (0U)
#define QSPI_READ      (1U)

#define QSPI_FIFO_FULL   (16U)
#define QSPI_FIFO_EMPTY  (0U)

/* ============================================================
 * CCR Mode Macros
 * ============================================================*/

/* Instruction mode */
#define CCR_IMODE_NIL          (0x0U)
#define CCR_IMODE_SINGLE_LINE  (0x1U)
#define CCR_IMODE_TWO_LINE     (0x2U)
#define CCR_IMODE_FOUR_LINE    (0x3U)

/* Address mode */
#define CCR_ADMODE_NIL          (0x0U)
#define CCR_ADMODE_SINGLE_LINE  (0x1U)
#define CCR_ADMODE_TWO_LINE     (0x2U)
#define CCR_ADMODE_FOUR_LINE    (0x3U)

/* Address size */
#define CCR_ADSIZE_8_BIT   (0x0U)
#define CCR_ADSIZE_16_BIT  (0x1U)
#define CCR_ADSIZE_24_BIT  (0x2U)
#define CCR_ADSIZE_32_BIT  (0x3U)

/* Alternate byte mode */
#define CCR_ABMODE_NIL          (0x0U)
#define CCR_ABMODE_SINGLE_LINE  (0x1U)
#define CCR_ABMODE_TWO_LINE     (0x2U)
#define CCR_ABMODE_FOUR_LINE    (0x3U)

/* Alternate byte size */
#define CCR_ABSIZE_8_BIT   (0x0U)
#define CCR_ABSIZE_16_BIT  (0x1U)
#define CCR_ABSIZE_24_BIT  (0x2U)
#define CCR_ABSIZE_32_BIT  (0x3U)

/* Data mode */
#define CCR_DMODE_NO_DATA      (0x0U)
#define CCR_DMODE_SINGLE_LINE  (0x1U)
#define CCR_DMODE_TWO_LINE     (0x2U)
#define CCR_DMODE_FOUR_LINE    (0x3U)

/* Functional mode */
#define CCR_FMODE_INDIRECT_WRITE  (0x0U)
#define CCR_FMODE_INDIRECT_READ   (0x1U)
#define CCR_FMODE_APM             (0x2U)
#define CCR_FMODE_MMM             (0x3U)

/* Memory mapped mode */
#define CCR_MM_MODE_XIP  (0x0U)
#define CCR_MM_MODE_RAM  (0x1U)

/* Poll match mode */
#define PMM_AND  (0U)
#define PMM_OR   (1U)

/* ============================================================
 * Register Bitfield Macros
 * ============================================================*/

#define CR_FTHRES_MASK   (0xFU << 7U)

#define CR_PRESCALER(x)  ((uint32_t)(x) << 18U)
#define CR_PMM(x)        ((uint32_t)(x) << 17U)
#define CR_APMS(x)       ((uint32_t)(x) << 16U)
#define CR_TOIE(x)       ((uint32_t)(x) << 15U)
#define CR_SMIE(x)       ((uint32_t)(x) << 14U)
#define CR_FTIE(x)       ((uint32_t)(x) << 13U)
#define CR_TCIE(x)       ((uint32_t)(x) << 12U)
#define CR_TEIE(x)       ((uint32_t)(x) << 11U)
#define CR_FTHRES(x)     ((uint32_t)(x) << 7U)
#define CR_TCEN(x)       ((uint32_t)(x) << 3U)
#define CR_ABORT(x)      ((uint32_t)(x) << 1U)
#define CR_EN(x)         ((uint32_t)(x) << 0U)

/* DCR */
#define DCR_MODE_BYTE(x) ((uint32_t)(x) << 21U)
#define DCR_FSIZE(x)     ((uint32_t)(x) << 16U)
#define DCR_CSHT(x)      ((uint32_t)(x) << 8U)
#define DCR_CKMODE(x)    ((uint32_t)(x))

/* Status register */
#define SR_FLEVEL   (1U << 8)
#define SR_BUSY     (1U << 5)
#define SR_TOF      (1U << 4)
#define SR_SMF      (1U << 3)
#define SR_FTF      (1U << 2)
#define SR_TCF      (1U << 1)
#define SR_TEF      (1U << 0)

/* Flag clear */
#define FCR_CTOF  (1U << 4)
#define FCR_CSMF  (1U << 3)
#define FCR_CTCF  (1U << 1)
#define FCR_CTEF  (1U << 0)

/* CCR */
#define CCR_DDRM(x)                ((uint32_t)(x) << 31U)
#define CCR_MM_MODE(x)             ((uint32_t)(x) << 30U)
#define CCR_DUMMY_BIT(x)           ((uint32_t)(x) << 29U)
#define CCR_SIOO(x)                ((uint32_t)(x) << 28U)
#define CCR_FMODE(x)               ((uint32_t)(x) << 26U)
#define CCR_DMODE(x)               ((uint32_t)(x) << 24U)
#define CCR_DUMMY_CONFIRMATION(x)  ((uint32_t)(x) << 23U)
#define CCR_DCYC(x)                ((uint32_t)(x) << 18U)
#define CCR_ABSIZE(x)              ((uint32_t)(x) << 16U)
#define CCR_ABMODE(x)              ((uint32_t)(x) << 14U)
#define CCR_ADSIZE(x)              ((uint32_t)(x) << 12U)
#define CCR_ADMODE(x)              ((uint32_t)(x) << 10U)
#define CCR_IMODE(x)               ((uint32_t)(x) << 8U)
#define CCR_INSTRUCTION(x)         ((uint32_t)(x))

/* RMC */
#define RMC_WDCYC(x)   ((uint32_t)(x) << 21U)
#define RMC_RDCYC(x)   ((uint32_t)(x) << 16U)
#define RMC_WINSTR(x)  ((uint32_t)(x) << 8U)
#define RMC_RINSTR(x)  ((uint32_t)(x))

/* ============================================================
 * Instance Handles
 * ============================================================*/

typedef struct QSPI_Instance QSPI_Instance_t;

const QSPI_Instance_t *QSPI_INSTANCE_0(void);
const QSPI_Instance_t *QSPI_INSTANCE_1(void);

#define QSPI0   QSPI_INSTANCE_0()
#define QSPI1   QSPI_INSTANCE_1()

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
    const QSPI_Instance_t *qspi_inst;

    uint8_t functional_mode;
    uint8_t instruction;
    uint8_t instruction_mode;

    uint8_t address_mode;
    uint8_t address_size;
    uint32_t address;

    uint8_t alternate_byte_mode;
    uint32_t alternate_byte;
    uint8_t alternate_byte_size;

    uint8_t dummy_mode  :1;
    uint8_t dummy_bit   :1;
    uint8_t dummy_cycles:5;
    uint8_t sioo        :1;
    uint8_t mm_mode     :1;

    uint8_t data_mode;
    uint32_t length;
    uint8_t *data_buffer;

    uint8_t FMEM_SIZE;
    uint8_t CLK_MODE:1;
    uint8_t fthresh;
    uint8_t csht;

    uint32_t TCEN:1;
    uint32_t TEIE:1;
    uint32_t TCIE:1;
    uint32_t FTIE:1;
    uint32_t SMIE:1;
    uint32_t TOIE:1;
    uint32_t APMS:1;
    uint32_t PMM:1;
    uint32_t PRESCALER:14;

    uint32_t status_mask;
    uint32_t status_match;

    uint32_t rd_instr:8;
    uint32_t wr_instr:8;
    uint32_t rd_dcyc:5;
    uint32_t wr_dcyc:5;
} qspi_msg;

/* ============================================================
 * API Prototypes
 * ============================================================*/

/**
 * @brief Perform a QSPI transaction.
 *
 * @param msg Pointer to transaction configuration structure.
 *
 * @return
 * SUCCESS on success  
 * EFAULT if pointer invalid  
 * EPERM if QSPI clock exceeds limit
 * EINVAL if functional mode is not supported.
 */
uint16_t QSPI_Transaction(qspi_msg *msg);

/**
 * @brief Check Status Match Flag.
 *
 * @param instance QSPI instance
 * @param wait_till_match
 *        0 = single check
 *        non-zero = wait until match
 *
 * @return 1 if match detected, else ETIME
 */
uint16_t QSPI_SMF_Flag_Check(const QSPI_Instance_t *instance,
                             uint8_t wait_till_match);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_QSPI_H_
