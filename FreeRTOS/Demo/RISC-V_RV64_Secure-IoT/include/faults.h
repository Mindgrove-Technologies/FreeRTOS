/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : faults.h
 * Brief Description of file         : This file contains function prototypes of various fault weak handlers.
 * Name of Author                    : Vishwajith.N.S
 * Email ID                          : vishwajith@mindgrovetech.in
 *
 *
 *
 * @file faults.h
 * @author Vishwajith .N.S (vishwajith@mindgrovetech.in)
 * @brief This file contains function prototypes of various fault weak handlers.
 * @version 1.0
 * @date 2025-07-07
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef FAULTS_H
#define FAULTS_H
#include<stdint.h>
typedef void (*fault_handler_t)(void);
extern const fault_handler_t fault_handlers[];
void on_instruction_address_misaligned(void);
void on_instruction_access_fault(void);
void on_illegal_instruction(void);
void on_breakpoint(void);
void on_load_address_misaligned(void);
void on_load_access_fault(void);
void on_store_amo_address_misaligned(void);
void on_store_amo_access_fault(void);
void on_ecall_from_umode(void);
void on_ecall_from_smode(void);
void on_reserved_10(void);
void on_ecall_from_mmode(void);
void on_instruction_page_fault(void);
void on_load_page_fault(void);
void on_reserved_14(void);
void on_store_amo_page_fault(void);
#endif