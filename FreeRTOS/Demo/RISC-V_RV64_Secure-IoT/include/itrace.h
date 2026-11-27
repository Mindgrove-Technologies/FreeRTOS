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
 * @file itrace.h
 * @brief Public APIs for Instruction Trace (ITRACE) driver
 * @details Provides macros, enums and public APIs required to
 *          configure and control the ITRACE hardware module. The interface
 *          supports configuration of instruction trace comparators and
 *          filters, enabling/disabling trace capture, and managing trace
 *          data transfer through DMA.
 * @version 1.1
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 *          Sai Girish S (saigirish@mindgrovetech.in)
 * @date 10-03-2026
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 06-01-2024 | 1.0     | Shri Mahaalakshmi S J | Initial release.
 *            |         |                       |
 * 10-03-2026 | 1.1     | Sai Girish S          | Refactored APIs and extended
 *            |         |                       | driver functionality. Added
 *            |         |                       | support for configuring all
 *            |         |                       | three comparators and filters,
 *            |         |                       | and introduced additional APIs
 *            |         |                       | for improved trace
 *            |         |                       | configuration and control.
 *            |         |                       | Added DMA ISR-based software
 *            |         |                       | workaround for ITRACE RAM
 *            |         |                       | wrap handling.
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_ITRACE_H_
#define BSP_INCLUDE_ITRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup UART_TX_REG_ADDR UART Transmit Register Addresses
 *
 * @brief Memory-mapped UART transmit register addresses used as DMA destinations.
 *
 * These macros compute the addresses of UART transmit (TX) data registers
 * using the UART base address defined in @ref secureiot.h and the register
 * offset. This makes the macros reusable even if the base address changes.
 *
 * They are intended to be used as destination addresses in DMA transfers,
 * for example with @ref ITRACE_RAM_Enable(), where trace data stored in
 * ITRACE RAM is streamed out through a UART interface.
 *
 * @{
 */

/** 
 * @brief Offset of the UART transmit data register from the UART base address.
 */
#define UART_TX_REG_OFFSET     (0x04UL)

/**
 * @brief Transmit register address for UART1.
 *
 * Can be used as the destination address in @ref ITRACE_RAM_Enable()
 * to transmit trace data over UART1.
 */
#define UART1_TX_REG_ADDR  ((uint32_t *)((UART1_BASE) + UART_TX_REG_OFFSET))

/**
 * @brief Transmit register address for UART2.
 *
 * Can be used as the destination address in @ref ITRACE_RAM_Enable()
 * to transmit trace data over UART2.
 */
#define UART2_TX_REG_ADDR  ((uint32_t *)((UART2_BASE) + UART_TX_REG_OFFSET))

/**
 * @brief Transmit register address for UART3.
 *
 * Can be used as the destination address in @ref ITRACE_RAM_Enable()
 * to transmit trace data over UART3.
 */
#define UART3_TX_REG_ADDR  ((uint32_t *)((UART3_BASE) + UART_TX_REG_OFFSET))

/**
 * @brief Transmit register address for UART4.
 *
 * Can be used as the destination address in @ref ITRACE_RAM_Enable()
 * to transmit trace data over UART4.
 */
#define UART4_TX_REG_ADDR  ((uint32_t *)((UART4_BASE) + UART_TX_REG_OFFSET))

/** @} */

/**
 * @enum ITRACE_filter_t
 * @brief Instruction trace filter identifiers.
 *
 * This enumeration defines the available filter blocks in the
 * ITRACE hardware module. Filters are used to selectively allow
 * trace generation based on comparator match conditions.
 */
typedef enum {
    /** The value of this enum is 0U. It represents Filter instance 0. */
    ITRACE_FILTER0 = 0U,
    /** The value of this enum is 1U. It represents Filter instance 1. */
    ITRACE_FILTER1 = 1U,
    /** The value of this enum is 2U. It represents Filter instance 2. */
    ITRACE_FILTER2 = 2U
} ITRACE_filter_t;

/**
 * @enum ITRACE_comp_t
 * @brief Comparator identifiers used in instruction trace matching.
 *
 * The ITRACE hardware supports multiple comparators which
 * evaluate instruction addresses or execution context values
 * against programmed match values.
 */
typedef enum {
    /** The value of this enum is 0U. It represents Comparator 1. */
    ITRACE_COMP1 = 0U,
    /** The value of this enum is 1U. It represents Comparator 2. */
    ITRACE_COMP2 = 1U,
    /** The value of this enum is 2U. It represents Comparator 3. */
    ITRACE_COMP3 = 2U
} ITRACE_comp_t;

/**
 * @enum ITRACE_resync_mode_t
 * @brief Resynchronization mode configuration.
 *
 * This enumeration specifies how often synchronization packets
 * are inserted in the trace stream to maintain trace decoder
 * alignment.
 */
typedef enum {
    /** Resynchronization packets are disabled. */
    ITRACE_RESYNC_DISABLE = 0U,
    /** Resynchronization based on packet count. */
    ITRACE_RESYNC_PACKET_COUNT = 1U,
    /** Resynchronization based on clock cycle count. */
    ITRACE_RESYNC_CLOCK_COUNT = 2U
} ITRACE_resync_mode_t;

/**
 * @enum ITRACE_input_mode_t
 * @brief Comparator input source selection.
 *
 * Defines the input signal used by the comparator for
 * performing match operations.
 */
typedef enum {
    /** Comparator input is the instruction address. */
    ITRACE_INPUT_IADDR = 0U,
    /** Comparator input is the execution context value. */
    ITRACE_INPUT_CONTEXT = 1U
} ITRACE_input_mode_t;

/**
 * @enum ITRACE_privilege_mode_t
 * @brief Privilege mode selection for trace filtering.
 *
 * Defines the processor privilege level used by the
 * ITRACE filter module for privilege-based trace filtering.
 */
typedef enum {
    /** User mode (U-mode) */
    ITRACE_PRIVILEGE_USER = 0U,
    /** Supervisor mode (S-mode) */
    ITRACE_PRIVILEGE_SUPERVISOR = 1U,
    /** Machine mode (M-mode) */
    ITRACE_PRIVILEGE_MACHINE = 3U
} ITRACE_privilege_mode_t;

/**
 * @enum ITRACE_compare_mode_t
 * @brief Comparator comparison operations.
 *
 * Defines the comparison function applied between
 * the comparator input and the programmed match value.
 */
typedef enum {
    /** Equal comparison operation. */
    ITRACE_CMP_EQ = 0U,
    /** Not equal comparison operation. */
    ITRACE_CMP_NE = 1U,
    /** Less than comparison operation. */
    ITRACE_CMP_LT = 2U,
    /** Less than or equal comparison operation. */
    ITRACE_CMP_LE = 3U,
    /** Greater than comparison operation. */
    ITRACE_CMP_GT = 4U,
    /** Greater than or equal comparison operation. */
    ITRACE_CMP_GE = 5U,
    /** Comparator always evaluates to FALSE. */
    ITRACE_CMP_FALSE = 6U,
    /** Comparator always evaluates to TRUE. */
    ITRACE_CMP_TRUE = 7U
} ITRACE_compare_mode_t;

/**
 * @enum ITRACE_ps_mode_t
 * @brief Primary and Secondary comparator combination modes.
 *
 * Determines how the results of the primary and secondary
 * comparator conditions are combined to generate a match event.
 */
typedef enum {
    /** 
     * Match occurs when only the Primary comparator condition (P) is evaluated.
     * The Secondary comparator result is ignored.
     */
    ITRACE_PS_P_ONLY = 0U,

    /** 
     * Match occurs only when both Primary (P) and Secondary (S) comparator
     * conditions evaluate to true.
     *
     * Logical condition: (P && S)
     */
    ITRACE_PS_P_AND_S = 1U,

    /** 
     * Match occurs when the logical AND result of the Primary (P) and
     * Secondary (S) comparator conditions evaluates to false.
     *
     * Logical condition: !(P && S)
     */
    ITRACE_PS_NOT_P_AND_S = 2U,

    /** 
     * The Primary comparator condition (P) is latched once it evaluates
     * to true. The match event occurs only after the Secondary comparator
     * condition (S) becomes true while the latched primary condition
     * remains active.
     *
     * Logical behavior: P is captured first, then S must occur to trigger
     * the match event.
     */
    ITRACE_PS_LATCH_P_UNTIL_S = 3U
} ITRACE_ps_mode_t;

/**
 * @brief Instruction Trace comparator configuration structure.
 *
 * @details This structure contains all parameters required to configure
 * an instruction trace comparator. Each comparator can evaluate
 * primary and secondary inputs using programmable comparison
 * operations and match values. The results can be combined using
 * configurable logical modes to generate trace events.
 *
 * @note The structure must be properly initialized before calling
 *       @ref ITRACE_Config_Comparator().
 */
typedef struct {
    /**
     * @brief Comparator instance to configure.
     *
     * Possible values:
     * - @ref ITRACE_COMP1
     * - @ref ITRACE_COMP2
     * - @ref ITRACE_COMP3
     */
    ITRACE_comp_t comp;

    /**
     * @brief Primary comparator input source.
     *
     * Possible values:
     * - @ref ITRACE_INPUT_IADDR   (Instruction address)
     * - @ref ITRACE_INPUT_CONTEXT (Execution context value)
     */
    ITRACE_input_mode_t p_input;

    /**
     * @brief Primary comparator comparison function.
     *
     * Possible values:
     * - @ref ITRACE_CMP_EQ
     * - @ref ITRACE_CMP_NE
     * - @ref ITRACE_CMP_LT
     * - @ref ITRACE_CMP_LE
     * - @ref ITRACE_CMP_GT
     * - @ref ITRACE_CMP_GE
     * - @ref ITRACE_CMP_FALSE
     * - @ref ITRACE_CMP_TRUE
     */
    ITRACE_compare_mode_t p_cmp;

    /**
     * @brief Primary comparator match value.
     *
     * The value against which the primary input is compared.
     * The interpretation of this value depends on the selected
     * primary input source:
     *
     * - If @ref ITRACE_INPUT_IADDR is selected, this represents
     *   an instruction address.
     * - If @ref ITRACE_INPUT_CONTEXT is selected, this represents
     *   the execution context value.
     */
    uint32_t p_value;

    /**
     * @brief Secondary comparator input source.
     *
     * Possible values:
     * - @ref ITRACE_INPUT_IADDR   (Instruction address) 
     * - @ref ITRACE_INPUT_CONTEXT (Execution context value)
     */
    ITRACE_input_mode_t s_input;

    /**
     * @brief Secondary comparator comparison function.
     *
     * Possible values:
     * - @ref ITRACE_CMP_EQ
     * - @ref ITRACE_CMP_NE
     * - @ref ITRACE_CMP_LT
     * - @ref ITRACE_CMP_LE
     * - @ref ITRACE_CMP_GT
     * - @ref ITRACE_CMP_GE
     * - @ref ITRACE_CMP_FALSE
     * - @ref ITRACE_CMP_TRUE
     */
    ITRACE_compare_mode_t s_cmp;

    /**
     * @brief Secondary comparator match value.
     *
     * Value compared against the selected secondary input source.
     * The interpretation of this value depends on the selected
     * secondary input source:
     *
     * - If @ref ITRACE_INPUT_IADDR is selected, this represents
     *   an instruction address.
     * - If @ref ITRACE_INPUT_CONTEXT is selected, this represents
     *   the execution context value.
     */
    uint32_t s_value;

    /**
     * @brief Primary-secondary comparator combination mode.
     *
     * Determines how the results of the primary and secondary
     * comparisons are logically combined.
     *
     * Possible values:
     * - @ref ITRACE_PS_P_ONLY
     * - @ref ITRACE_PS_P_AND_S
     * - @ref ITRACE_PS_NOT_P_AND_S
     * - @ref ITRACE_PS_LATCH_P_UNTIL_S
     */
    ITRACE_ps_mode_t ps_mode;

    /**
     * @brief Primary comparator notification enable flag.
     *
     * Enables notification when the primary comparator condition
     * evaluates to true.
     *
     * Possible values:
     * - true  : Enable notification
     * - false : Disable notification
     */
    bool p_notify;

    /**
     * @brief Secondary comparator notification enable flag.
     *
     * Enables notification when the secondary comparator condition
     * evaluates to true.
     *
     * Possible values:
     * - true  : Enable notification
     * - false : Disable notification
     */
    bool s_notify;
} ITRACE_comp_config_t;

/**
 * @brief Instruction Trace filter configuration structure.
 *
 * @details This structure contains all parameters required to configure
 * an instruction trace filter. Filters determine which comparator match
 * events are allowed to generate trace packets. A filter can enable one
 * or more comparators and optionally restrict trace generation based on
 * execution privilege level.
 *
 * @note The structure must be properly initialized before calling
 *       the corresponding filter configuration API.
 */
typedef struct {
    /**
     * @brief Filter instance to configure.
     *
     * Possible values:
     * - @ref ITRACE_FILTER0
     * - @ref ITRACE_FILTER1
     * - @ref ITRACE_FILTER2
     */
    ITRACE_filter_t filter;

    /**
     * @brief Enable Comparator 1 for this filter.
     *
     * When enabled, comparator 1 match result contributes to
     * trace generation.
     *
     * Possible values:
     * - true  : Enable comparator 1
     * - false : Disable comparator 1
     */
    bool comp1;

    /**
     * @brief Enable Comparator 2 for this filter.
     *
     * Possible values:
     * - true  : Enable comparator 2
     * - false : Disable comparator 2
     */
    bool comp2;

    /**
     * @brief Enable Comparator 3 for this filter.
     *
     * Possible values:
     * - true  : Enable comparator 3
     * - false : Disable comparator 3
     */
    bool comp3;

    /**
     * @brief Enable privilege level based filtering.
     *
     * When enabled, trace packets are generated only when execution
     * occurs at the specified privilege level.
     *
     * Possible values:
     * - true  : Enable privilege filtering
     * - false : Disable privilege filtering (ignore privilege level)
     */
    bool privilege_enable;

    /**
     * @brief Execution privilege level used for filtering.
     *
     * This field is considered only when @ref privilege_enable is true.
     *
     * Possible values:
     * - @ref ITRACE_PRIVILEGE_USER
     * - @ref ITRACE_PRIVILEGE_SUPERVISOR
     * - @ref ITRACE_PRIVILEGE_MACHINE
     */
    ITRACE_privilege_mode_t privilege_level;
} ITRACE_filter_config_t;

 /**
 * @brief Configures an instruction trace comparator.
 *
 * @details This function configures the specified comparator with
 * primary and secondary comparison inputs, comparison functions,
 * match values, and logical combination modes.
 *
 * @param comp_config Pointer to the comparator configuration structure.
 *
 * @return 
 * - @ref SUCCESS
 *        Comparator successfully configured.
 *
 * - @ref EINVAL
 *        Returned if:
 *         - @p comp_config is NULL
 *         - Invalid primary input mode
 *         - Invalid secondary input mode
 *         - Invalid comparator comparison mode
 *         - Invalid primary-secondary combination mode
 *         - Invalid comparator instance
 */
uint16_t ITRACE_Config_Comparator(const ITRACE_comp_config_t *comp_config);

/**
 * @brief Configures a trace filter.
 *
 * @details This API configures a filter that determines which comparator
 * match events contribute to trace packet generation. A filter can combine
 * one or more comparator outputs and optionally restrict tracing based on
 * processor privilege level.
 *
 * The filter is enabled as part of this configuration.
 *
 * @param filter_config Pointer to filter configuration structure.
 *
 * @note At least one comparator (comp1, comp2, or comp3) must be enabled.
 *       Each enabled comparator must be configured using
 *       @ref ITRACE_Config_Comparator() before calling this API.
 *
 * @note If privilege filtering is enabled, only trace events generated
 *       at the specified privilege level are considered.
 *
 * @return 
 * - @ref SUCCESS
 *        Filter configured successfully.
 *
 * - @ref EPERM
 *        Returned if none of the comparators are enabled
 *        (comp1, comp2 and comp3 all set to false).
 *
 * - @ref EINVAL
 *        Returned in the following cases:
 *        - Invalid filter identifier is provided.
 *        - privilege_enable is true and privilege_level is greater than 3.
 */
uint16_t ITRACE_Config_Filter(const ITRACE_filter_config_t *filter_config);

/**
 * @brief Configures instruction trace for a specific address range.
 *
 * @details This helper function configures a comparator and filter
 * to generate instruction trace packets only when execution occurs
 * within the specified address range.
 *
 * @param comparator Comparator instance used for the address range match.
 * @param filter Filter instance used to enable the comparator match event.
 * @param start_addr Start address of the trace region.
 * @param end_addr End address of the trace region.
 * 
 * @return 
 * - @ref SUCCESS
 *        Address range configuration successful.
 * 
 * - @ref EFAULT
 *        Returned if either @p start_addr or @p end_addr is NULL.
 * 
 * - @ref EINVAL
 *        Returned in the follwing cases:
 *        - Returned if an invalid filter or comparator identifier is provided.
 *        - Returned if invalid address range is provided.
 *
 * - Error code returned by @ref ITRACE_Config_Comparator()
 *   Propagated if comparator configuration fails.
 *
 * - Error code returned by @ref ITRACE_Config_Filter()
 *   Propagated if filter configuration fails.
 * 
 * @note privilege level filtering, primary and secondary notify are disabled
 */
uint16_t ITRACE_Config_Address_Range(ITRACE_comp_t comparator,
                                     ITRACE_filter_t filter,
                                     uint32_t *start_addr, uint32_t *end_addr);

/**
 * @brief Enables trace RAM and configures DMA transfer.
 *
 * @details Configures the trace RAM buffer and initializes
 * DMA transfer to move trace data from the ITRACE RAM
 * to a user-provided memory destination.
 *
 * @param dma_channel DMA channel used for trace data transfer.
 * @param dma_dst_addr Destination memory address for storing trace data.
 *
 * @return 
 * - @ref SUCCESS
 *        Trace RAM and DMA configuration completed successfully.
 *
 * - @ref EFAULT
 *        Returned if @p dma_dst_addr is NULL.
 *
 * - Error code returned by @ref DMA_Transfer_Configure()
 *   Propagated if DMA configuration fails.
 *
 * - Error code returned by @ref DMA_Channel_Set_State()
 *   Propagated if enabling the DMA channel fails.
 */
uint16_t ITRACE_RAM_Enable(uint8_t dma_channel, uint32_t *dma_dst_addr);

/**
 * @brief Enables the Instruction Trace module.
 *
 * @details Configures the trace engine and enables instruction
 * trace generation with the specified resynchronization mode
 * and maximum resynchronization interval.
 *
 * @param resync_mode Resynchronization mode configuration.
 * @param resync_max Maximum resynchronization interval value.
 * @note The @p resync_max field is limited to 4 bits in hardware,
 *       therefore valid values range from 0 to 15.
 *
 * @return 
 * - @ref SUCCESS
 *        Trace module successfully enabled.
 *
 * - @ref EINVAL
 *        Returned in the following cases:
 *         - if an invalid @p resync_mode value is provided.
 *         - if an invalid @p resync_max value is provided.
 */
uint16_t ITRACE_Enable(ITRACE_resync_mode_t resync_mode, uint8_t resync_max);

/**
 * @brief Disables the Instruction Trace module.
 *
 * @details This function disables instruction tracing and stops the
 * ITRACE RAM operation. Before disabling the module, it waits until
 * the trace RAM buffer becomes empty (write pointer equals read pointer).
 * If the buffer does not drain before the timeout expires, the function
 * returns a timeout error.
 *
 * Additionally, if DMA transfer was previously configured using
 * @ref ITRACE_RAM_Enable(), the corresponding DMA channel will be
 * disabled as part of this operation.
 *
 * @return 
 * - @ref SUCCESS
 *        The trace module was successfully disabled.
 *
 * - @ref ETIMEDOUT
 *        Returned if the trace RAM buffer does not drain within the
 *        internal timeout period while waiting for write and read
 *        pointers to match.
 *
 * - Error code returned by @ref DMA_Channel_Set_State()
 *   Propagated if disabling the DMA channel fails.
 */
uint16_t ITRACE_Disable(void);

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_ITRACE_H_
