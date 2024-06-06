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
// #include "utils.h"
// #include "lcd_driver.h"
// #include "i2c_v2.h"

#include <stdint.h>
#include "spi_v2.h"
#include "utils.h"
#include "rfid_driver.h"
#include "gpiov2.h"

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

#define RST_PIN         16          // Configurable, see typical pin layout above
#define SPI_NO          1         // Configurable, see typical pin layout above
#define SPI_STANDARD 10,0,0
#define TAG {0xB3,0x11,0x84,0x0F}
#define CARD {0xC3,0x96,0xf5,0x0d}
uint8_t arr[10] = CARD;

void validate_card(Uid *uid,uint8_t *arr)
{	printf("valid card!!!!\n");
	uint8_t wrong = 0;
	printf("valid card!!!!\n");
	for (uint8_t i = 0; i < uid->size; i++) {
		printf("valid card1!!!!\n");
		if(uid->uidByte[i] == arr[i])
		{
			wrong = 1;
			break;
		}
	} 
	if(wrong == 0) printf("Door open !\n");
	else printf("Access Denied!!\n");
}

void main_rfid() {
	gpiov2_init();
	mfrc522_t var;
	mfrc522_t *handle = (mfrc522_t*)(&var);
	handle->rst_pin=1;
	handle->spi_no=SPI_NO;
	handle->ncs=2;
 	sspi_init(handle->spi_no);
  	sclk_config(handle->spi_no, 0, 0, SPI_STANDARD);
  	sspi_comm_control_config(handle->spi_no, MASTER, MSB_FIRST, FULL_DUPLEX, 8);
	PCD_Init(handle);		// Init MFRC522
	delayms(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	PCD_DumpVersionToSerial(handle);	// Show details of PCD - MFRC522 Card Reader details
	printf("\nScan PICC to see UID, SAK, type, and data blocks...");
	while(1) {
		// printf("********************\n");
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
		if ( ! PICC_IsNewCardPresent(handle)) {
			// printf("********************\n");
			continue;
		}
	// Select one of the cards
		if ( ! PICC_ReadCardSerial(handle)) {
				printf("Card detected \n");
				// validate_card(handle->uid,arr);
			continue;
		}

	delayms(100);
	}
}