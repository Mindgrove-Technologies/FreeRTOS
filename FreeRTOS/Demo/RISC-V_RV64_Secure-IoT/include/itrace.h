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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The function `ITRACE_Ctrl` sets specific bits in a control register to enable instruction tracing.
 */
void ITRACE_Ctrl();

/**
 * The function `ITRACE_Ram_Ctrl` sets specific bits in a memory control register to enable and activate RAM
 */
void ITRACE_Ram_Ctrl();

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

