#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "io.h"

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
            vTaskDelay(pdMS_TO_TICKS(1000));
            xSemaphoreGive(xCountingSemaphore);
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

int app_main() {
    xBinarySemaphore   = xSemaphoreCreateBinary();
    // asm volatile("fence.i");
    xCountingSemaphore = xSemaphoreCreateCounting(3, 3); // Max=3 tokens
    // asm volatile("fence.i");
    xMutex             = xSemaphoreCreateMutex();

    if(xBinarySemaphore != NULL && xCountingSemaphore != NULL && xMutex != NULL)
    {
        // Binary semaphore usage
        xTaskCreate(vISR_Simulator, "ISR_Sim", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vTaskWaitForEvent, "WaitEvt", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Counting semaphore usage
        xTaskCreate(vResourceUser, "ResUser1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vResourceUser, "ResUser2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vResourceUser, "ResUser3", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Mutex usage
        xTaskCreate(vUARTTask, "UART1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vUARTTask, "UART2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        // Start scheduler
        vTaskStartScheduler();
    }

    for(;;);
}