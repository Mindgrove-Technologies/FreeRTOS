/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : interrupt_control.h
 * Brief Description of file         : This file contains function prototypes of non_vectored_trap entry(overrideable) and mode switching function.
 * Name of Author                    : Vishwajith.N.S
 * Email ID                          : vishwajith@mindgrovetech.in
 *
 *
 *
 * @file interrupt_control.h
 * @author Vishwajith .N.S (vishwajith@mindgrovetech.in)
 * @brief This file contains function prototypes of non_vectored_trap entry(overrideable) and mode switching function.
 * @version 1.0
 * @date 2025-07-07
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef INTERRRUPT_CONTROL_H
#define INTERRRUPT_CONTROL_H
#include<stdint.h>
enum{
    NON_VECTORED_MODE,
    VECTORED
};
/**
 * @fn void non_vectored_trap_entry(void) 
 * 
 * @brief This function is called when an interrupt is triggered in non vectored mode not by user but by core.
 * 1.It pushes all the integral core registers value to stack.
 * 2.Reads the mcause value to see whether arrived exception is either fault or interrupt.
 * 3.If it is interrupt,then function corresponding to interrupt is called from machine interrupt table.
 * 4.If it is fault,then fault handler is called.
 * 5.After servicing either one of them,again register values are popped back from stack to core registers.
 * 
 * If User wants he can override this function.
 */
void non_vectored_trap_entry(void);
/**
 * @fn void core_switch_interrupt_mode(uint8_t mode)
 * 
 * @brief Used to switch between vectored and non vectored mode of interrupt.
 * 
 * @param mode The parameter \a mode is used to set whether vectored or non vectored mode.
 * 
 */
void core_switch_interrupt_mode(uint8_t mode);
#endif