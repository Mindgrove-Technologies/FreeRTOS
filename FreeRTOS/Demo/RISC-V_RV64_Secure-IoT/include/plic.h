/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : plic.h
 * Brief Description of file         : This file contains PLIC configuration function prototypes.
 * Name of Author                    : Vishwajith.N.S
 * Email ID                          : vishwajith@mindgrovetech.in
 *
 *
 *
 * @file plic.h
 * @author Vishwajith .N.S (vishwajith@mindgrovetech.in)
 * @brief This file contains PLIC configuration function prototypes.
 * @version 1.0
 * @date 2025-07-07
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef PLIC_H
#define PLIC_H
#ifdef __cplusplus
extern "C" {
#endif
#include"secure_iot.h"
#include "stddef.h"
#include"errors.h"
#define PLIC_MAX_INTERRUPT_SRC  58
/* 7 priority levels are supported.
   PLIC_PRIORITY_1 means 'no interrupt threshold' */
enum{
   PLIC_PRIORITY_1,
   PLIC_PRIORITY_2,
   PLIC_PRIORITY_3,
   PLIC_PRIORITY_4,
   PLIC_PRIORITY_5,
   PLIC_PRIORITY_6,
   PLIC_PRIORITY_7 
};
typedef void (*PLIC_IRQHandler_t)(void * args);
typedef struct{
    PLIC_IRQHandler_t function;
    void *args;
}PLIC_Interrupt_type;

/* Function prototypes */

/**
 * @fn uint8_t PLIC_Interrupt_Complete(uint32_t priority_value)
 * 
 * @brief Used to complete a particular PLIC interrupt.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents the interrupt id for which interrupt has to be completed.
 * 
 * @return ENODEV if interrupt id is invalid else return SUCCESS.
 */
uint8_t PLIC_Interrupt_Complete(uint32_t interrupt_id);
/**
 * @fn void PLIC_Handler()
 * 
 * @brief Used to handle PLIC interrupt.
 * 1.Reads the claim register to find source of PLIC interrupt.
 * 2.Calls the appropriate Interrupt handler and services it.
 * 3.Writes the claim register with serviced interrupt id to signal PLIC core that current PLIC handler work is done.
 */
void   PLIC_Handler(void);
/**
 * @fn uint8_t PLIC_Interrupt_Enable(uint32_t priority_value)
 * 
 * @brief Used to disable a particular PLIC interrupt.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents the interrupt id for which interrupt has to be enabled.
 * 
 * @return ENODEV if interrupt id is invalid else return SUCCESS.
 */
uint8_t PLIC_Interrupt_Enable(uint32_t interrupt_id);
/**
 * @fn uint8_t PLIC_Interrupt_Disable(uint32_t priority_value)
 * 
 * @brief Used to disable a particular PLIC interrupt.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents the interrupt id for which interrupt has to be disabled.
 * 
 * @return ENODEV if interrupt id is invalid else return SUCCESS.
 */
uint8_t PLIC_Interrupt_Disable(uint32_t interrupt_id);
/**
 * @fn uint8_t PLIC_Interrupt_Threshold(uint32_t priority_value)
 * 
 * @brief Used to set threshold for interrupt to be triggered.
 * 
 * @param priority_value The parameter \a priority_value is an unsigned integer that represents the priority value above which interrupt will be triggered.
 * 
 * @return ENODEV if interrupt id is invalid,ERRINVAL if priority value is invalid else return SUCCESS.
 */
uint8_t PLIC_Interrupt_Threshold(uint32_t priority_value);
/**
 * @fn uint8_t PLIC_Interrupt_Pending(uint8_t interrupt_id)
 * 
 * @brief Used to check the pending status of a PLIC interrupt.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents for which interrupt pending status has to be checked.
 * 
 * @return 1 if it is pending,0 if it is either claimed or not triggered and ENODEV when interrupt id is not valid.
 */
uint8_t PLIC_Interrupt_Pending(uint8_t int_id);\
/**
 * @fn uint8_t PLIC_Set_Interrupt_Priority(uint32_t interrupt_id, uint32_t priority_value)
 * 
 * @brief Used to set interrupt priority for a paricular PLIC interrupt.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents for which interrupt priority has to be set.
 * @param priority_value The parameter \a priority_value is an unsigned integer that represents the interrupt priority value.
 * 
 * @return ENODEV if interrupt id is invalid,ERRINVAL if priority value is invalid else return SUCCESS.
 */
uint8_t PLIC_Set_Interrupt_Priority(uint32_t int_id, uint32_t priority_value);
/**
 * @fn void PLIC_Nested_Interrupt(uint8_t enable)
 * 
 * @brief Used to configure mode of PLIC interrupt whether nested or non nested interrupt.
 * 
 * @param enable The parameter \a enable if its non zero then PLIC interrupts will be in nested else it will be non nested mode.
 *               Nested interrupts is way of handling interrupts where a high priority interrupt will prempt a low priority interrupt.
 *               Non nested interrupts is way of handling where low priority interrupts is completely serviced even a high priority 
 *               interrupt has arrived.
 * 
 * 
 */
void   PLIC_Nested_Interrupt(uint8_t enable);
/**
 * @fn void Global_interrupt_enable()
 * 
 * @brief Used to enable machine external interrupt globally.
 *  
 */
void   Global_interrupt_enable(void);
/**
 * @fn void PLIC_Init()
 * 
 * @brief Used to disable all interrupts PLIC interrupts,set interrupt threshold and do global interrupt enable.
 * 
 */
void   PLIC_Init(void);
/**
 * @fn uint8_t PLIC_Set_Handler(uint8_t interrupt_id,PLIC_IRQHandler_t handler,void *args)
 * 
 * @brief Used to map handler to a particular PLIC interrupt and initial arguement to interrupt handler.
 * 
 * @param interrupt_id The parameter \a interrupt_id is an unsigned integer that represents to which interrupt handler should be mapped.
 * @param handler The parameter \a handler is a function pointer that has adress to which control should go.
 * @param args The parameter \a args is of void * type used to pass arguement to interrupt handler.
 * 
 * 
 * @return SUCCESS when successfully initialised and ENODEV when device not found.
 */
uint8_t PLIC_Set_Handler(uint8_t interrupt_id,PLIC_IRQHandler_t handler,void *args);
#ifdef __cplusplus
}
#endif

#endif
