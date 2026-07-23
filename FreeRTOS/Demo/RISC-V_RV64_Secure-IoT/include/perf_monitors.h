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
 * @file perf_monitors.h
 * @brief This is a Baremetal perf_monitor Driver's header file for Mindgrove Silicon's performance monitor.
 * @version 1.0
 * @authors Shri Mahaalakshmi S J (mahaalakshmi@mindgrovetech.in)
 * @date 04-12-2024
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 04-12-2024 | 1.0     | Shri Mahaalakshmi S J | Initial release
 * -----------------------------------------------------------------------------
 */

#ifndef PERF_MONITOR_H
#define PERF_MONITOR_H

#include <stddef.h>
#include <stdint.h>
#include "encoding.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Cache_perf_t
{
    uint64_t Ifbhit;           // Instruction Cache Fill Buffer Hits
    uint64_t Ifbrelease;       // Instruction Cache Fill Buffer Releases
    uint64_t Imiss;            // Instruction Cache Misses
    uint64_t Inc_access;       // Non-cached Instruction Access
    uint64_t Iaccess;          // Instruction Cache Accesses

    uint64_t Dread_access;     // Data Cache Read Accesses
    uint64_t Dwrite_access;    // Data Cache Write Accesses
    uint64_t Datomic_access;   // Data Cache Atomic Accesses
    uint64_t Dnc_read_access;  // Non-cached Data Read Accesses
    uint64_t Dnc_write_access; // Non-cached Data Write Accesses

    uint64_t Dread_miss;       // Data Cache Read Misses
    uint64_t Dwrite_miss;      // Data Cache Write Misses
    uint64_t Datomic_miss;     // Data Cache Atomic Misses

    uint64_t Dread_fbhit;      // Data Cache Read Fill Buffer Hits
    uint64_t Dwrite_fbhit;     // Data Cache Write Fill Buffer Hits
    uint64_t Datomic_fbhit;    // Data Cache Atomic Fill Buffer Hits

    uint64_t Dfbrelease;       // Data Cache Fill Buffer Releases
    uint64_t Dline_evictions;  // Data Cache Line Evictions

    uint64_t Itlb_miss;        // Instruction TLB Misses
    uint64_t Dtlb_miss;        // Data TLB Misses
};

struct Stalls_perf_t
{
  uint64_t rawstalls; // RAW stalls
  uint64_t exestalls; // Execution stalls
};

struct Branch_perf_t 
{
  uint64_t misprediction; // Branch misprediction
  uint64_t jumps; // Number of jumps
  uint64_t branches; // Number of branches
};

struct Arith_perf_t 
{
  uint64_t floats; // Floating-point Operations
  uint64_t muldiv; // Multiply and Divide Operations
};

/** 
 * @brief The function 'PERF_Mcycle_Init' Clears the mcycle register
 * 
 * @return Void
 */
void PERF_Mcycle_Init();

/**
 * @fn uint64_t PERF_Get_Mcycle()
 * 
 * @brief Reads and returns the current machine cycle counter value.
 * 
 * @return Current value of the mcycle register.
 */
uint64_t PERF_Get_Mcycle();

/** 
 * @fn uint64_t millis(int total_cycles)
 * 
 * @brief The function 'millis' converts the total number of cycles taken to milliseconds
 * 
 * @return returns the real time in milliseconds
 */
// uint64_t millis(uint64_t total_cycles);

/** 
 * @brief The function `PERF_Cache_Init` initializes performance monitoring counters and events related to
 * cache accesses.
 * 
 * @return Void
 */
void PERF_Cache_Init();

/** 
 * @brief The function `PERF_Print_Cache` prints various performance metrics related to instruction cache
 * (ICache) and data cache (DCache) based on the given number of iterations.
 * 
 * @param iterations The `iterations` parameter is used to calculate the average value of each metric over 
 * a certain number of iterations. By dividing the value read from the performance counters by the `iterations`
 * 
 * @return Void
 */
void PERF_Print_Cache(int iterations);

/** 
 * @brief The function calculates and prints the cache miss percentages for instruction cache (ICache) and
 * data cache (DCache) reads and writes.
 * 
 * @return Void
 */
void PERF_Print_Cache_MissPercentage();

/** 
 * @brief The function `PERF_Stalls_Init` initializes performance monitoring counters for raw and execution
 * stalls.
 * 
 * @return Void
 */
void PERF_Stalls_Init();

/** 
 * @brief The function `PERF_Print_Stalls` prints the number of raw stalls and execution stalls per iteration.
 * 
 * @param iterations The `iterations` parameter is used to calculate the average value of each metric over 
 * a certain number of iterations. By dividing the value read from the performance counters by the `iterations`
 * 
 * @return Void
 */
void PERF_Print_Stalls(int iterations);

/** 
 * @brief The function `PERF_Branches_Init` initializes performance monitoring counters for branch
 * instructions in a C program.
 * 
 * @return Void
 */
void PERF_Branches_Init();

/**
 * @brief The function `PERF_Print_Branches` calculates and prints the number of branch mispredictions, jumps,
 * branches, and the percentage of branch mispredictions based on the given number of iterations.
 * 
 * @param iterations The `iterations` parameter is used to calculate the average value of each metric over 
 * a certain number of iterations. By dividing the value read from the performance counters by the `iterations`
 * 
 * @return Void
 */
void PERF_Print_Branches(int iterations);

/** 
 * @brief The function `PERF_Arithops_Init` initializes performance monitoring counters for floating-point
 * operations and multiplication/division events.
 * 
 * @return Void
 */
void PERF_Arithops_Init();

/** 
 * @brief The function `PERF_Print_Arithops` prints the average number of floating-point operations and
 * multiplication/division operations per iteration.
 * 
 * @param iterations The `iterations` parameter is used to calculate the average value of each metric over 
 * a certain number of iterations. By dividing the value read from the performance counters by the `iterations`
 * 
 * @return Void
 */
void PERF_Print_Arithops(int iterations);

/** 
 * @brief The function `PERF_Set_Event` sets a specific event for a performance counter in a system.
 * 
 * @param counter The `counter` parameter sets the event for the specified counter based on the value 
 * of the `counter` parameter.
 * 
 * @param event The `event` parameter sets the specific event to set for a performance counter.
 * 
 * @return Void
 */
void PERF_Set_Event(int counter, int event);

/** 
 * @brief The function `PERF_Disable` disables a specific performance counter based on the 
 * input parameter `counter`.
 * 
 * @param counter The `counter` parameter is used in a switch-case statement to determine which performance 
 * counter to disable by writing specific values to control and event registers (`mhpmcounterX` and `mhpmeventX`).
 * 
 * @return Void
 */
void PERF_Disable(int counter);

/** 
 * @brief The function `PERF_Disable_All` disables all performance monitoring event registers.
 * 
 * @return Void
 */
void PERF_Disable_All();

/** 
 * @brief The function `PERF_Clear_All` clears all performance monitor counter values.
 * 
 * @return Void
 */
void PERF_Clear_All();

#ifdef __cplusplus
}
#endif

#endif