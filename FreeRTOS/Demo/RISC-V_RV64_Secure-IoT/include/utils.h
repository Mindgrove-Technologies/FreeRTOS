/****************************************************************************
 * Project           		: shakti devt board
 * Name of the file	     	: utils.h
 * Brief Description of file    : Header file for utils
 * Name of Author    	        : Sathya Narayanan N & Kapil Shyam. M
 * Email ID                     : sathya281@gmail.com, kapilshyamm@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
/**
 * @file utils.h
 * @brief header file for utils
 * @details This is the header file for util.c
 */

#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* function prototype */
void mcycle_init();
extern inline uint64_t get_mcycle_start();
extern inline uint64_t get_mcycle_stop();
extern int millis(int total_cycles);
void delayms(long delay);
void waitfor(unsigned int secs);
void delay(unsigned long seconds);
float pow_10(unsigned int y);
void reverse(char *str, int len);
void ftoa( float, char *, unsigned int);
void ftoa1(double f, int precision);
int atox(const char p[], int pos);
void delay_loop(unsigned long cntr1, unsigned long cntr2);
unsigned long read_word(size_t *addr);
void write_word(size_t *addr, unsigned long val);
int int_to_string(int number, char str[], unsigned int afterpoint);
void _printf_(const char *fmt, va_list ap);
int printf(const char* fmt, ...);
int vsscanf(const char *inp, char const *fmt0, va_list ap);
int sscanf(const char *inp, char const *fmt0, ...);
int _scan_(const char * format, va_list vl);
int scan(const char* format, ...);
long strtol1(const char *nptr, char **endptr, int base);
float strtof1(const char* str, char** endptr);
void cache_perf_init();
void print_cache_perf();
void print_cache_miss_percentage();
void stalls_perf_init();
void print_stalls_perf();
void branches_perf_init();
void print_branches_perf();
void arithops_perf_init();
void print_arithops_perf();
void disable_all_perf();
void clear_all_perf();

#ifdef __cplusplus
}
#endif

#endif
