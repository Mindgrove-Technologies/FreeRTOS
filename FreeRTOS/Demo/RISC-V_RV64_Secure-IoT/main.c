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

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>
#include "gpio.h"
#include "gpiov2.h"
#include "utils.h"
#include <semphr.h>

/* Run a simple demo just prints 'Blink' */
#define DEMO_BLINKY    1

extern void freertos_risc_v_trap_handler( void );

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName );
void vApplicationTickHook( void );

/*
 * Setup the Spike simulator to run this demo.
 */
static void prvSetupSpike( void );

int main_blinky( void );

static SemaphoreHandle_t idle_wakeup;

void vTask1(void *pvParameters) {
  while (1) {
    printf("Task 1 running...\n");
    vTaskDelay(pdMS_TO_TICKS(100)); // Simulate task execution time
    printf("Task 1 yielding...\n");
    taskYIELD();
  }
}

void vTask2(void *pvParameters) {
  while (1) {
    printf("Task 2 running...\n");
    vTaskDelay(pdMS_TO_TICKS(200)); // Simulate task execution time
  }
}

void vIdleTask(void *pvParameters) {
  while (1) {
    // Simulate idle task doing nothing (low priority operations)
    vTaskDelay(pdMS_TO_TICKS(1));
    xSemaphoreGive(idle_wakeup); // Signal scheduler when idle task wakes up
  }
}

int main(void) {
  prvSetupSpike();
  idle_wakeup = xSemaphoreCreateBinary();

  // Create tasks with different priorities
  xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // Start the idle task (lower priority than other tasks)
  xTaskCreate(vIdleTask, "Idle Task", configMINIMAL_STACK_SIZE, NULL, configIDLE_PRIORITY, NULL);

  // Start the scheduler
  vTaskStartScheduler();

  // Should not reach here as control is handed over to scheduler
  while (1);
}



/*-----------------------------------------------------------------------------------------------------------------*/
// Task function to print Hello World
// static void prvHelloWorldTask(void* pvParameters) {
//   gpiov2_init();
//   gpiov2_set_direction(9,1);
//   for (int i =0; i<10; i++){
//     gpiov2_set(9);
//     delay(2);
//     gpiov2_clear(9);
//     delay(2);
//     printf("Hello World from task 1!\n");
//     // Simulate some work (optional delay)
//     // vTaskDelay(pdMS_TO_TICKS(1));
//   }
// }
// // Task function to print goodbye
// static void prvGoodbyeTask(void* pvParameters) {
//   gpiov2_init();
//   gpiov2_set_direction(10,1);
//   for (int i= 0 ; i<10 ; i++) {
//     gpiov2_set(10);
//     delay(2);
//     gpiov2_clear(10);
//     delay(2);

//     printf("Goodbye from Task 2!\n");
//     // vTaskDelay(pdMS_TO_TICKS(10));
//     // vTaskDelay(pdMS_TO_TICKS(1));
//   }
// }

// static void prvblink ( void * pvParamters) {
//   gpiov2_init();
//   gpiov2_set_direction(10, 0);
//   while (1) {
//     // gpiov2_set(10);
//     // delay(2);
//     gpiov2_clear(10);
//   }
// }

// static void prvToggleLED( void* pvParameters )
// {
//     gpiov2_init();
//     //Assumption 1 ---> output, 0 ---> input
//     gpiov2_set_direction(9, 1);
//     gpiov2_set_direction(10, 1);
//     gpiov2_set_direction(8, 1);
//     gpiov2_set_direction(11, 1);

//       // delay_loop(500,500);
//      for(int i=0; i< 3; i++){
//       switch(i){
//           case 0: 
//             printf("Blinking 0");
//             gpiov2_set(9);
//             delay(2);
//             // vTaskDelay(pdMS_TO_TICKS(10));
//             gpiov2_clear(9);
//             break;
//           case 1:
//             printf("Blinking 1");
//             gpiov2_set(10);
//             delay(2);
//             // vTaskDelay(pdMS_TO_TICKS(10));
//             gpiov2_clear(10);
//             break;
//           case 2:
//             printf("Blinking 2");
//             gpiov2_set(11);
//             delay(2);
//             // vTaskDelay(pdMS_TO_TICKS(10));
//             gpiov2_clear(11);
//             break;
//           case 3:
//             printf("Blinking 3");
//             gpiov2_set(8);
//             delay(2);
//             // vTaskDelay(pdMS_TO_TICKS(10));
//             gpiov2_clear(8);
//             break;
//       }
//     }
// }

// int main(void) {

//   prvSetupSpike();
//   // Create the Hello World task
//   xTaskCreate(prvHelloWorldTask,
//               "Hello World Task",
//               configMINIMAL_STACK_SIZE,
//               NULL,
//               1, // Set a low priority (optional)
//               NULL);

//    // Create the Goodbye task with a higher priority
//   xTaskCreate(prvGoodbyeTask,
//               "Goodbye Task",
//               configMINIMAL_STACK_SIZE,
//               NULL,
//               2,
//               NULL);

//     // xTaskCreate(prvblink,
//     //           "Toggle LED Task",
//     //           configMINIMAL_STACK_SIZE,
//     //           NULL,
//     //           1,
//     //           NULL);

//   // Start the FreeRTOS scheduler
//   vTaskStartScheduler();

//   // We should never get here! If vTaskStartScheduler returns, there was an error.
//   return 1;
// }
/*-----------------------------------------------------------------------------------------------------------------*/

// int main( void )
// {
//     int ret;

//     prvSetupSpike();

//     #if defined( DEMO_BLINKY )
//         ret = main_blinky();
//     #else
//     #error "Please add or select demo."
//     #endif

//     return ret;
// }
/*-----------------------------------------------------------*/

static void prvSetupSpike( void )
{
    __asm__ volatile ( "csrw mtvec, %0" : : "r" ( freertos_risc_v_trap_handler ) );
}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
     * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
     * function that will get called if a call to pvPortMalloc() fails.
     * pvPortMalloc() is called internally by the kernel whenever a task, queue,
     * timer or semaphore is created.  It is also called by various parts of the
     * demo application.  If heap_1.c or heap_2.c are used, then the size of the
     * heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
     * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
     * to query the size of free heap space that remains (although it does not
     * provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
     * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
     * task.  It is essential that code added to this hook function never attempts
     * to block in any way (for example, call xQueueReceive() with a block time
     * specified, or call vTaskDelay()).  If the application makes use of the
     * vTaskDelete() API function (as this demo application does) then it is also
     * important that vApplicationIdleHook() is permitted to return to its calling
     * function, because it is the responsibility of the idle task to clean up
     * memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
    volatile uint32_t ulSetTo1ToExitFunction = 0;

    taskDISABLE_INTERRUPTS();

    while( ulSetTo1ToExitFunction != 1 )
    {
        __asm volatile ( "NOP" );
    }
}
