/**
 * @file benchmark.h
 * @author Sai Kiran Narayanaswami (saikiran@mindgrovetech.in)
 * @brief Helper utilities to manage performance counters easily and display
 *        various benchmark results.
 * @example ```
 * PERF_Init_Benchmark(); // Call this the first time
 * // do work
 * PERF_Pause_Benchmark();
 * PERF_Print_Benchmark();
 * // Do non-benchmark work
 * PERF_Reset_Benchmark ();
 * continue_benchmakr();
 * // do other work
 * PERF_Pause_Benchmark();
 * print_benchmakr();
 * ```
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 * 
 */

#ifndef UTILS_BENCHMARK_H
#define UTILS_BENCHMARK_H

#include "perf_monitors.h"

#ifdef __cplusplus
extern "C" {
#endif

void PERF_Init_Benchmark();
void PERF_Print_Benchmark(char * label);
void PERF_Pause_Benchmark();
void PERF_Reset_Benchmark();
void PERF_Continue_Benchmark();
uint64_t PERF_Get_Elapsed_Cycles();
uint64_t PERF_Get_Instret_Count();

#ifdef __cplusplus
}
#endif

#endif // UTILS_BENCHMARK_H
