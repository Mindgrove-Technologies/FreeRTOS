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
 * @file benchmark.h
 * @brief Contains Helper utilities to manage performance counters.
 * @details Helper utilities to manage performance counters easily and display
 * various benchmark results.
 * @version 1.0
 * @authors Sai Kiran Narayanaswami (saikiran@mindgrovetech.in)
 * @date 23-04-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by             | Description                   
 * -----------|---------|-------------------------|-----------------------------
 * 23-04-2025 | 1.0     | Sai Kiran Narayanaswami | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef UTILS_BENCHMARK_H
#define UTILS_BENCHMARK_H

#include "perf_monitors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 
 * @brief Initializes the performance benchmarking module
 * @details Prepares and configures internal performance counters, registers, or hardware metrics trackers to begin recording system execution data./
*/
void PERF_Init_Benchmark();

/**
 * @brief Outputs the recorded performance metrics to the system log
 * @details Captures the current metrics data and prints a formatted summary to the console or log file, prepended with a descriptive label for identification.
 * @param char *label - text string to identify and tag the specific benchmark log output
*/
void PERF_Print_Benchmark(char *label);

/**
 * @brief Pauses the active performance benchmark tracking
 * @details Temporarily halts the accumulation of metrics such as cycles and retired instructions, preserving the current state without resetting the counters.
*/
void PERF_Pause_Benchmark();

/**
 * @brief Resets all performance benchmark metrics to zero
 * @details Clears all accumulated values in the performance trackers, resetting cycle counts, instruction counts, and related metrics back to their initial baseline.
*/
void PERF_Reset_Benchmark();

/**
 * @brief Resumes a previously paused performance benchmark
 * @details Unpauses the tracking mechanism, allowing performance counters to continue accumulating metrics from where they were last paused.
*/
void PERF_Continue_Benchmark();

/**
 * @brief Retrieves the total elapsed processor cycles
 * @details Reads and returns the total number of hardware clock cycles that have accumulated during the active benchmark period.
*/
uint64_t PERF_Get_Elapsed_Cycles();

/**
 * @brief Retrieves the total number of retired instructions
 * @details Reads and returns the total count of instructions that have successfully executed and retired (instret) during the active benchmark period.
*/
uint64_t PERF_Get_Instret_Count();

#ifdef __cplusplus
}
#endif

#endif // UTILS_BENCHMARK_H
