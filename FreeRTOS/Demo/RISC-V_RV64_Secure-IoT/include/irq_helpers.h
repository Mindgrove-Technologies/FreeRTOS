/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : irq_helpers.h
 * Brief Description of file         : This file contains trap entry and exit routine macros.
 * Name of Author                    : Vishwajith.N.S
 * Email ID                          : vishwajith@mindgrovetech.in
 *
 *
 *
 * @file irq_helpers.h
 * @author Vishwajith .N.S (vishwajith@mindgrovetech.in)
 * @brief This file contains trap entry and exit routine macros.
 * @version 1.0
 * @date 2025-07-07
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef IRQ_HELPERS
#define IRQ_HELPERS
/*Used to push intergral caller save core registers manually  to stack*/
#define PUSH_CONTEXT do { \
    __asm__ volatile ("sd ra, -248(sp)\n\t"); \ 
    __asm__ volatile ("jal Trap_entry\n\t"); \
} while(0)
/*Used to pop intergral caller save core registers manually from stack*/
#define POP_CONTEXT asm volatile("jal  Trap_exit\n\t")
/*Used to push intergral and floating caller save core registers manually  to stack*/
#define PUSH_CONTEXT_INT_FLOAT do { \
    __asm__ volatile ("jal Trap_entry_int_float\n\t"); \
} while(0)
/*Used to pop intergral and floating caller save core registers manually from stack*/
#define POP_CONTEXT_INT_FLOAT asm volatile("jal Trap_exit_int_float\n\t")
#endif