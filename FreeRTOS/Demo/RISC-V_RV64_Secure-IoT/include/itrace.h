/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : itrace.h
 * Brief Description of file         : This is a Baremetal itrace Driver file for Mindgrove Silicon's itrace Peripheral.
 * Name of Author                    : Shri Mahaalakshmi S J, Santhosh M
 * Email ID                          : mahaalakshmi@mindgrovetech.in, msanthosh@mindgrovetech.in
 *
 * @file itrace.h
 * @author Santhosh M (msanthosh@mindgrovetech.in)
 * @author Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 * @brief This is a Baremetal Itrace Driver file for Mindgrove Silicon's itrace Peripheral.
 * @version 0.1
 * @date 2024-01-06
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */

#ifndef ITRACE_H
#define ITRACE_H

#include <stdint.h>
#include "secure_iot.h"
#include "log.h"
#include "errors.h"
#include "dma.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TRENCODER_BASE 0x00060000
#define CNTRL          0x00
#define OPTIONALMODES  0x04
#define FILTERCONTROL   0x08
#define COMP1CONTROL    0x0c
#define COMP1PMATCHLOW  0x18
#define COMP1PMATCHHIGH 0x1c
#define COMP1SMATCHLOW  0x20
#define COMP1SMATCHHIGH 0x24

#define TRRAM_BASE     0x00060100
#define RAM_CNTRL      0x00
#define RAM_DATA       0x40
#define RAM_WP_LOW     0x20
#define RAM_RP_LOW     0x28

/**
 * The function `ITRACE_Ctrl` sets specific bits in a control register to enable instruction tracing.
 */
void ITRACE_Ctrl();

/**
 * The function `ITRACE_Ram_Ctrl` configures the ITRACE RAM controller and initiates a DMA transfer.
 * 
 * @param buffer The `buffer` parameter used to send the address through DMA.
 */
void ITRACE_Ram_Ctrl(uint32_t buffer);

/**
 * The function `ITRACE_Read_Ram_Data` reads data from a RAM buffer and returns the data along with the
 * count of elements read.
 * 
 * @param count The `count` parameter in the `ITRACE_Read_Ram_Data` function is a pointer to an
 * integer. This pointer is used to store the count of elements read from the ITRACE_RAM data. 
 * 
 * @return The function `ITRACE_Read_Ram_Data` is returning a pointer to an array, data read from a memory location. 
 */
int* ITRACE_Read_Ram_Data(int *count);


/**
 * The function `ITRACE_Filter_val` sets the control register `COMP1_CTRL` with specified match mode, S
 * function, and P function values for ITRACE.
 * 
 * @param MatchMode MatchMode specifies the comparison mode for the ITRACE filter. It determines how
 * the incoming data is compared with the filter settings.
 * @param S_Function The `S_Function` parameter is used to specify the type of source function for the
 * trace comparison. 
 * @param P_Function The `P_Function` parameter in the `ITRACE_Filter_val` function represents a
 * specific function or operation related to the ITRACE component. The exact functionality of
 * `P_Function` would depend on the specific implementation and design of the ITRACE module in your
 * system.
 */
void ITRACE_Filter_val(uint8_t MatchMode, uint8_t S_Function, uint8_t P_Function);

/**
 * The function `ITRACE_Comp_Ctrl` sets filter control settings based on the input `Comp_num`.
 * 
 * @param Comp_num The  input parameter `Comp_num`, which is used determine the filter control settings.
 */
void ITRACE_Comp_Ctrl(uint8_t Comp_num);

/**
 * The function `ITRACE_Disable_Ctrl_Reg` disables the control register of the ITRACE module.
 */
void ITRACE_Disable_Ctrl_Reg();

/**
 * The function `ITRACE_Disable_RAM_ctrl_Reg` disables the RAM control register of the ITRACE module.
 */
void ITRACE_Disable_RAM_ctrl_Reg();

/**
 * The function `ITRACE_RAM_Wrap` sets a flag in the control register to stop tracing when the trace
 * RAM wraps around.
 */
void ITRACE_RAM_Wrap();

#ifdef __cplusplus
}
#endif

#endif

