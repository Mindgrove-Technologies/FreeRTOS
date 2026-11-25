#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "io.h"

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
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int app_main( void ) {
    xQueue = xQueueCreate(5, sizeof(uint8_t));

    if (xQueue != NULL) {
        xTaskCreate(producer, "Producer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(consumer, "Consumer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

        vTaskStartScheduler();
    }

    for (;;);
}