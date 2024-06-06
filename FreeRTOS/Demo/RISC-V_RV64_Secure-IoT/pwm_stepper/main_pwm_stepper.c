/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


/*-----------------------------------------------------------*/
// FreeRTOS kernel includes.
/*-----------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/*-----------------------------------------------------------*/
// FreeRTOS application includes.
/*-----------------------------------------------------------*/

#include <stdint.h>
#include "pwmv2.h"
#include "gpiov2.h"
#include "platform.h"
#include "utils.h"
#include <stdio.h>

/*-----------------------------------------------------------*/
// Functions
/*-----------------------------------------------------------*/

/* Priorities used by the tasks. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
 * to ticks using the pdMS_TO_TICKS() macro. */
#define mainQUEUE_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 1000 )

/* The maximum number items the queue can hold.  The priority of the receiving
 * task is above the priority of the sending task, so the receiving task will
 * preempt the sending task and remove the queue items each time the sending task
 * writes to the queue.  Therefore the queue will never have more than one item in
 * it at any time, and even with a queue length of 1, the sending task will never
 * find the queue full. */
#define mainQUEUE_LENGTH                   ( 1 )

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

int xGetCoreID( void )
{
int id;

	__asm ("csrr %0, mhartid" : "=r" ( id ) );

	return id;
}

/*-----------------------------------------------------------*/
// Main Code.
/*-----------------------------------------------------------*/

#define PWM_0   0
#define PWM_1   1
#define PWM_2   2

int db_delay = 0;
uint16_t prescale = 50000;
bool output_polarity = true;
uint32_t period= 100;
int duty=50;

int main_pwm_stepper( void )
{   
    pinmux_enable_pwm(PWM_0);
    gpiov2_init();
    pwm_init(PWM_0);
    gpiov2_set_direction(1,1);
    pwm_set_prescalar_value(PWM_0, prescale);
    pwm_configure(PWM_0, period, duty, no_interrupt, db_delay,output_polarity);
    pwm_set_control(PWM_0, (PWM_OUTPUT_ENABLE));
    gpiov2_set(1);
    pwm_start(PWM_0);
    while(1)
    {   gpiov2_clear(1);
        delayms(3000);
        gpiov2_set(1);       
        delayms(3000);
    }
}