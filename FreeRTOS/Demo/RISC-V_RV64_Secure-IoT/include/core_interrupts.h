/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : core_interrupts.h
 * Brief Description of file         : This file contains function prototypes of various core level interrupt weak handlers.
 * Name of Author                    : Vishwajith.N.S
 * Email ID                          : vishwajith@mindgrovetech.in
 *
 *
 *
 * @file core_interrupts.h
 * @author Vishwajith .N.S (vishwajith@mindgrovetech.in)
 * @brief This file contains various core level weak interrupt handlers.
 * @version 1.0
 * @date 2025-07-07
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef CORE_INTERRUPTS_H
#define CORE_INTERRUPTS_H
#include<stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef void (*core_interrupt_handler_t)(void);
// Weak user-overridable interrupt callback hooks
void on_fault(void) __attribute__((weak));
void on_supervisor_software_interrupt(void) __attribute__((weak));
void on_machine_software_interrupt(void) __attribute__((weak));
void on_supervisor_timer_interrupt(void) __attribute__((weak));
void on_machine_timer_interrupt(void) __attribute__((weak));
void on_supervisor_external_interrupt(void) __attribute__((weak));
void on_machine_external_interrupt(void) __attribute__((weak));
void on_default_interrupt(void) __attribute__((weak));
// Naked, weak interrupt entry handlers
void fault_handler(void) __attribute__((naked, weak));
void supervisor_software_interrupt_handler(void) __attribute__((naked, weak));
void machine_software_interrupt_handler(void) __attribute__((naked, weak));
void supervisor_timer_interrupt_handler(void) __attribute__((naked, weak));
void machine_timer_interrupt_handler(void) __attribute__((naked, weak));
void supervisor_external_interrupt_handler(void) __attribute__((naked, weak));
void machine_external_interrupt_handler(void) __attribute__((naked, weak));
void default_interrupt_handler(void) __attribute__((naked, weak));

#ifdef __cplusplus
}
#endif

#endif // VECTORED_H