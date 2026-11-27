#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "io.h"
#include "trng.h"

void vTask1(void *pvParameters) {
    for (;;)
    {
        printf("[Task 1] Hello from Task1\n\r");

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vTask2(void *pvParameters) {
    for (;;)
    {
        printf("[Task 2] Hello from Task2\n\r");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int xGetCoreID( void ) {
    int id;

	__asm ("csrr %0, mhartid" : "=r" ( id ) );

	return id;
}

int app_main( void ) {
    int hartid;
    printf( "\nHello World from FreeRTOS on Secure-IoT SoC!\n" );
    hartid = xGetCoreID();
    printf( "\nRunning on Core: %d\n\n", hartid);

    xTaskCreate(vTask1, "Task1", 256, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
