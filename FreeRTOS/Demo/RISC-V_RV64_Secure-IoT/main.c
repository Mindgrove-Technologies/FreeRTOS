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

/*-----------------------------------------------------------*/

int main( void )
{
    int ret;

    prvSetupSpike();

    #if defined( DEMO_BLINKY )
        ret = main_blinky();
    #else
    #error "Please add or select demo."
    #endif

    return ret;
}
/*-----------------------------------------------------------*/
static void prvSetupSpike( void )
{
    __asm__ volatile ( "csrw mtvec, %0" : : "r" ( freertos_risc_v_trap_handler ) );
}

/*-----------------------------------------------------------*/

void vToggleLED( void )
{
    gpiov2_init();
    //Assumption 1 ---> output, 0 ---> input
    gpiov2_write_word(GPIO_DIRECTION_CNTRL_REG, GPIO0);

   while(1){
      delay_loop(500,500);
     for(int i=0; i< 8; i++){
      switch(i){
          case 0: 
            gpiov2_write_word(GPIO_DATA_REG, GPIO0);
            delay(2);
            break;
          case 1: 
            gpiov2_write_word(GPIO_DATA_REG, GPIO1);
            delay(2);
            break;
          case 2:
            gpiov2_write_word(GPIO_DATA_REG, GPIO2);
            delay(2);
            break;
          case 3:
            gpiov2_write_word(GPIO_DATA_REG, GPIO3);
            delay(2);
            break;
          case 4:
            gpiov2_write_word(GPIO_DATA_REG, GPIO4);
            delay(2);
            break;
          case 5:
            gpiov2_write_word(GPIO_DATA_REG, GPIO5);
            delay(2);
            break;
          case 6:
            gpiov2_write_word(GPIO_DATA_REG, GPIO6);
            delay(2);
            break;
          case 7:
            gpiov2_write_word(GPIO_DATA_REG, GPIO7);
            delay(2);
            break;
      }
    }
   }
    
}

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
