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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*-----------------------------------------------------------*/
// FreeRTOS application includes.
/*-----------------------------------------------------------*/
#include "utils.h"
#include "io.h"

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

#define TEST_HELLO_TASK
// #define TEST_QUEUE
// #define TEST_SEMAPHORE_MUTEX
/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
// static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

int xGetCoreID( void )
{
int id;

	__asm ("csrr %0, mhartid" : "=r" ( id ) );

	return id;
}

#ifdef TEST_HELLO_TASK
volatile int task1_counter = 0;
volatile int task2_counter = 0;

void vTask1(void *pvParameters)
{
    for (;;)
    {
        task1_counter++;
        printf("[Task 1] Hello from Task1\n\r");

        vTaskDelay(pdMS_TO_TICKS(500)); // Delay 500ms
    }
}

// Task 2
void vTask2(void *pvParameters)
{
    for (;;)
    {
        task2_counter++;
        printf("[Task 2] Hello from Task2\n\r");

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1s
    }
}
#endif

#ifdef TEST_QUEUE
QueueHandle_t xQueue;

void producer(void* pv) {
    for (uint8_t i = 0;; i++) {
        if (xQueueSend(xQueue, &i, pdMS_TO_TICKS(100)) == pdPASS) {
            printf("Producer: Sent data %d\n\r", i);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void consumer(void* pv) {
    uint8_t received_val;
    for (;;) {
        if (xQueueReceive(xQueue, &received_val, pdMS_TO_TICKS(100)) == pdPASS) {
            printf("Consumer: Data Received %d\n\r", received_val);
        }
        // vTaskDelay(pdMS_TO_TICKS(500));
    }
}
#endif /*TEST_QUEUE*/

#ifdef TEST_SEMAPHORE_MUTEX
// Handles
SemaphoreHandle_t xBinarySemaphore;   
SemaphoreHandle_t xCountingSemaphore; 
SemaphoreHandle_t xMutex;             

void vISR_Simulator(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        xSemaphoreGive(xBinarySemaphore);
        printf("[ISR] Event signaled!\n\r");
    }
}

void vTaskWaitForEvent(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("[Task] Received event from ISR\n\r");
        }
    }
}

void vResourceUser(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("[Task %s] Got a resource token!\n\r", pcTaskGetName(NULL));
            vTaskDelay(pdMS_TO_TICKS(1000)); // Use resource
            xSemaphoreGive(xCountingSemaphore); // Release token
            printf("[Task %s] Released resource token!\n\r", pcTaskGetName(NULL));
        }
    }
}

void vUARTTask(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            printf("[Task %s] Printing safely\n\r", pcTaskGetName(NULL));
            vTaskDelay(pdMS_TO_TICKS(500));
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
#endif /*TEST_SEMAPHORE_MUTEX*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
// Main Code.
/*-----------------------------------------------------------*/

int main_hello( void )
{
    int hartid;
    printf( "\nHello World from FreeRTOS on Secure-IoT SoC!\n" );
    hartid = xGetCoreID();
    printf( "\nRunning on Core: %d\n\n", hartid);
    // vToggleLED();

    #ifdef TEST_HELLO_TASK
    xTaskCreate(vTask1, "Task1", 256, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", 256, NULL, 1, NULL);

    // Start scheduler
    vTaskStartScheduler();

    while(1);
    #endif

    #ifdef TEST_QUEUE
    xQueue = xQueueCreate(5, sizeof(uint8_t));

    if (xQueue != NULL) {
        xTaskCreate(producer, "Producer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(consumer, "Consumer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

        size_t freeHeap = xPortGetFreeHeapSize();
        printf("Free Heap: %u bytes\n", (unsigned int)freeHeap);
        vTaskStartScheduler();
    }


    for (;;);
    #endif /*TEST_QUEUE*/

    #ifdef TEST_SEMAPHORE_MUTEX
    size_t freeHeap = xPortGetFreeHeapSize();
        printf("Free Heap: %u bytes\n", (unsigned int)freeHeap);
        // vTaskStartScheduler();
    // Test for faults
    // uint32_t *badPtr = (uint32_t* )0xF0000000;
    // uint32_t val = *badPtr;
    // printf("%d", val);

    xBinarySemaphore   = xSemaphoreCreateBinary();
    // asm volatile("fence.i");
    xCountingSemaphore = xSemaphoreCreateCounting(3, 3); // Max=3 tokens
    // asm volatile("fence.i");
    xMutex             = xSemaphoreCreateMutex();

    if(xBinarySemaphore != NULL && xCountingSemaphore != NULL && xMutex != NULL)
    {
        // Binary semaphore use
        xTaskCreate(vISR_Simulator, "ISR_Sim", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vTaskWaitForEvent, "WaitEvt", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Counting semaphore use
        xTaskCreate(vResourceUser, "ResUser1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vResourceUser, "ResUser2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vResourceUser, "ResUser3", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Mutex use
        xTaskCreate(vUARTTask, "UART1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vUARTTask, "UART2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Start scheduler
        size_t freeHeap = xPortGetFreeHeapSize();
        printf("Free Heap: %u bytes\n", (unsigned int)freeHeap);
        vTaskStartScheduler();
    }

    for(;;);
    #endif /*TEST_SEMAPHORE_MUTEX*/

    return 0;
}
