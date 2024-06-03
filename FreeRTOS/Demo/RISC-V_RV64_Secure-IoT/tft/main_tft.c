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

#include "spi_v2.h"
#include "gpiov2.h"
#include "platform.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "ili9341_touch_driver.h"
#include "ili9341_driver.h"
#include "ili9341_fonts_driver.h"
#include "testimg_driver.h"
#include "testimg.h"
#include "output_image_hex_values.h"

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

void loop() {
    // Check border
    ILI9341_FillScreen(ILI9341_BLACK);

    for(int x = 0; x < ILI9341_WIDTH; x++) {
        ILI9341_DrawPixel(x, 0, ILI9341_RED);
        ILI9341_DrawPixel(x, ILI9341_HEIGHT - 1, ILI9341_RED);
    }

    for(int y = 0; y < ILI9341_HEIGHT; y++) {
        ILI9341_DrawPixel(0, y, ILI9341_RED);
        ILI9341_DrawPixel(ILI9341_WIDTH - 1, y, ILI9341_RED);
    }

    delay_us(1000);

    // Check font
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0,"Font_7x10, HELLO", Font_7x10, ILI9341_RED, ILI9341_BLACK);
    ILI9341_WriteString(0, 3*10, "Font_11x18,HELLO", Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
    ILI9341_WriteString(0, 3*20, "Font_16x26,HELLO", Font_16x26, ILI9341_BLUE, ILI9341_BLACK);

    delay_us(1000);
    ILI9341_InvertColors(true);
    delay_us(1000);
    ILI9341_InvertColors(false);

    delay_us(5000);

    // Check colors
    ILI9341_FillScreen(ILI9341_WHITE);
    ILI9341_WriteString(0, 0, "WHITE", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_BLUE);
    ILI9341_WriteString(0, 0, "BLUE", Font_11x18, ILI9341_BLACK, ILI9341_BLUE);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_RED);
    ILI9341_WriteString(0, 0, "RED", Font_11x18, ILI9341_BLACK, ILI9341_RED);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_GREEN);
    ILI9341_WriteString(0, 0, "GREEN", Font_11x18, ILI9341_BLACK, ILI9341_GREEN);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_CYAN);
    ILI9341_WriteString(0, 0, "CYAN", Font_11x18, ILI9341_BLACK, ILI9341_CYAN);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_MAGENTA);
    ILI9341_WriteString(0, 0, "MAGENTA", Font_11x18, ILI9341_BLACK, ILI9341_MAGENTA);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_YELLOW);
    ILI9341_WriteString(0, 0, "YELLOW", Font_11x18, ILI9341_BLACK, ILI9341_YELLOW);
    delay_us(500);

    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0, "BLACK", Font_11x18, ILI9341_WHITE, ILI9341_BLACK);
    delay_us(500);

    ILI9341_DrawImage((ILI9341_WIDTH - 240)/2, (ILI9341_HEIGHT - 240)/2, 240, 240, (const uint16_t*)test_img_240x240);
    delay_us(3000);

}

int main_rtc (void)
{    
    ILI9341_Init();

    printf("\n\n font\n\n");
    // ILI9341_FillScreen(ILI9341_RED); // Sends color to fill the screen
    ILI9341_FillScreen(ILI9341_BLACK); // Sends color to fill the screen
    ILI9341_WriteString(0,3*20,"MINDGROVE",Font_16x26,ILI9341_GREEN,ILI9341_BLACK); // sends text to the display
    delay_us(10000);
    ILI9341_WriteString(0,4*20,"Technologies",Font_16x26,ILI9341_GREEN,ILI9341_BLACK); // sends text to the display
    // delay_us(10000);
    // ILI9341_FillScreen(ILI9341_BLACK); // Sends color to fill the screen
    delay_us(5000);
    // printf("\n completed \n");
    // ILI9341_DrawImage(0, 20, 240, 240, (const uint16_t*)test_img_240x240); // sends Image data to the display
    // // // delay_us(3000);
    // // // ILI9341_FillScreen(ILI9341_BLACK);
    // // // delay_us(3000);
    // // // ILI9341_DrawImage((ILI9341_WIDTH - 240)/2,(ILI9341_HEIGHT-240)/2,240,240,
    // // // (const uint16_t*)testimg);
    // delay_us(3000);
    // printf("\n completed \n");
    // // ILI9341_FillScreen(ILI9341_BLACK);
    // // delay_us(2000);
    // // printf("\n completed \n");
    // // ILI9341_DrawImage((ILI9341_WIDTH - 240)/2,(ILI9341_HEIGHT-240)/2,240,240,
    // // (const uint16_t*)output_image_hex_values);
    // delay_us(3000);
    // printf("\n completed \n");
                //loop();
    return 0;
 }