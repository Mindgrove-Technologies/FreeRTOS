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

/*-----------------------------------------------------------*/
// Main Code.
/*-----------------------------------------------------------*/

int motor1_dir;
int motor2_dir;
int motor3_dir;
int motor1_speed;
int motor2_speed;
int motor3_speed;

int main_stepper() {
  int count = 0;
  int x = 1;
  int y = 1;
  int z = 1;

  gpiov2_init();
  gpiov2_set_direction(16, 1); // For direction
  gpiov2_set_direction(17, 1); // For step
  gpiov2_set_direction(18, 1); // For direction
  gpiov2_set_direction(19, 1); // For step
  gpiov2_set_direction(20, 1); // For direction
  gpiov2_set_direction(21, 1); // For step
  gpiov2_set_direction(22, 0); // GPIO input

  while (1) {

    printf("\n Enter the motor's direction '0 - anticlockwise', '1 - clockwise' ");

    printf("\n Motor1 : ");
    scan("%d", &motor1_dir);

    printf("\n Motor2 : ");
    scan("%d", &motor2_dir);

    printf("\n Motor3 : ");
    scan("%d", &motor3_dir);

    printf("\n Enter the speed of the motors : ");

    printf("\n Motor 1 : ");
    scan("%d", &motor1_speed);

    printf("\n Motor 2 : ");
    scan("%d", &motor2_speed);

    printf("\n Motor 3 : ");
    scan("%d", &motor3_speed);

    if (motor1_dir == 1) {
        printf("\n Motor1 direction set to 1 ");
        gpiov2_set(16);
    } else {
        printf("\n Motor1 direction set to 0 ");
        gpiov2_clear(16);
    }


    if (motor2_dir == 1) {
        printf("\n Motor2 direction set to 1 ");
        gpiov2_set(18);
    } else {
        printf("\n Motor2 direction set to 0 ");
        gpiov2_clear(18);
    }


    if (motor3_dir == 1) {
        printf("\n Motor3 direction set to 1 ");
        gpiov2_set(20);
    } else {
        printf("\n Motor3 direction set to 0 ");
        gpiov2_clear(20);
    }

    gpiov2_set(17);
    gpiov2_set(19);
    gpiov2_set(21);

    while (1) {

      waitfor(200);

      if (count % motor1_speed == 0) {
        if (x % 2 == 1) {
          gpiov2_clear(17);
        } else {
          gpiov2_set(17);
        }
        x = x + 1;
      }

      if (count % motor2_speed == 0) {
        if (y % 2 == 1) {
          gpiov2_clear(19);
        } else {
          gpiov2_set(19);
        }
        y = y + 1;
      }

      if (count % motor3_speed == 0) {
        if (z % 2 == 1) {
          gpiov2_clear(21);
        } else {
          gpiov2_set(21);
        }
        z = z + 1;
      }
      count = count + 1;

      if (gpiov2_read_pin_status(22) == 1) {
        break;
      }
    }
  }
}
