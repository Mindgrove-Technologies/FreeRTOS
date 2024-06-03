#include "ili9341_fonts_driver.h"
#include "ili9341_touch_driver.h"
#include <stdbool.h>

#ifndef ILI9341_H_
#define ILI9341_H_

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define ILI9341_SPI_PORT SPI1

#define ILI9341_CS_Pin        0
#define ILI9341_RES_Pin       4
#define ILI9341_DC_Pin        5

// default orientation
#define ILI9341_WIDTH  240 // ILI9341 max TFT width
#define ILI9341_HEIGHT 320 // ILI9341 max TFT height
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)

#define ILI9341_NOP 0x00     // No-op register
#define ILI9341_SWRESET 0x01 // Software reset register
#define ILI9341_RDDID 0x04   // Read display identification information
#define ILI9341_RDDST 0x09   // Read Display Status

#define ILI9341_SLPIN 0x10  // Enter Sleep Mode
#define ILI9341_SLPOUT 0x11 // Sleep Out
#define ILI9341_PTLON 0x12  // Partial Mode ON
#define ILI9341_NORON 0x13  // Normal Display Mode ON

#define ILI9341_RDMODE 0x0A     // Read Display Power Mode
#define ILI9341_RDMADCTL 0x0B   // Read Display MADCTL
#define ILI9341_RDPIXFMT 0x0C   // Read Display Pixel Format
#define ILI9341_RDIMGFMT 0x0D   // Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F // Read Display Self-Diagnostic Result

#define ILI9341_INVOFF 0x20   // Display Inversion OFF
#define ILI9341_INVON 0x21    // Display Inversion ON
#define ILI9341_GAMMASET 0x26 // Gamma Set
#define ILI9341_DISPOFF 0x28  // Display OFF
#define ILI9341_DISPON 0x29   // Display ON

#define ILI9341_CASET 0x2A // Column Address Set
#define ILI9341_PASET 0x2B // Page Address Set
#define ILI9341_RAMWR 0x2C // Memory Write
#define ILI9341_RAMRD 0x2E // Memory Read

#define ILI9341_PTLAR 0x30    // Partial Area
#define ILI9341_VSCRDEF 0x33  // Vertical Scrolling Definition
#define ILI9341_MADCTL 0x36   // Memory Access Control
#define ILI9341_VSCRSADD 0x37 // Vertical Scrolling Start Address
#define ILI9341_PIXFMT 0x3A   // COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1 0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2 0xB2 // Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3 0xB3 // Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR  0xB4 // Display Inversion Control
#define ILI9341_DFUNCTR 0xB6 // Display Function Control

#define ILI9341_PWCTR1 0xC0 // Power Control 1
#define ILI9341_PWCTR2 0xC1 // Power Control 2
#define ILI9341_PWCTR3 0xC2 // Power Control 3
#define ILI9341_PWCTR4 0xC3 // Power Control 4
#define ILI9341_PWCTR5 0xC4 // Power Control 5
#define ILI9341_VMCTR1 0xC5 // VCOM Control 1
#define ILI9341_VMCTR2 0xC7 // VCOM Control 2

#define ILI9341_RDID1 0xDA // Read ID 1
#define ILI9341_RDID2 0xDB // Read ID 2
#define ILI9341_RDID3 0xDC // Read ID 3
#define ILI9341_RDID4 0xDD // Read ID 4

#define ILI9341_GMCTRP1 0xE0 // Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 // Negative Gamma Correction

// rotate right
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// rotate left
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// upside down
/*
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)
*/

/********************************************************/
// Color definitions
#define ILI9341_BLACK       0x0000      //   0,   0,   0
#define ILI9341_NAVY        0x000F      //   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0      //   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF      //   0, 125, 123
#define ILI9341_MAROON      0x7800      // 123,   0,   0
#define ILI9341_PURPLE      0x780F      // 123,   0, 123
#define ILI9341_OLIVE       0x7BE0      // 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618      // 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF      // 123, 125, 123
#define ILI9341_BLUE        0x001F      //   0,   0, 255
#define ILI9341_GREEN       0x07E0      //   0, 255,   0
#define ILI9341_CYAN        0x07FF      //   0, 255, 255
#define ILI9341_RED         0xF800      // 255,   0,   0
#define ILI9341_MAGENTA     0xF81F      // 255,   0, 255
#define ILI9341_YELLOW      0xFFE0      // 255, 255,   0
#define ILI9341_WHITE       0xFFFF      // 255, 255, 255
#define ILI9341_ORANGE      0xFD20      // 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5      // 173, 255,  41
#define ILI9341_PINK        0xFC18      // 255, 130, 198
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
/********************************************************/

/**
 * @fn delay_us(uint32_t microseconds)
 * @brief This function gives delay in microseconds
 * @details The function `delay_us` is designed to introduce a delay in microseconds by executing nested loops.
 * @param microseconds the amount of time, in microseconds, the function used to delay or pause the program execution
 */
void delay_us(uint32_t microseconds);

/**
 * @fn ILI9341_Unselect()
 * @brief This function sets the CS pin
 * @details The ILI9341_Unselect function sets the ILI9341_CS_Pin to high using the gpiov2_set function.
 */
void ILI9341_Unselect();

/**
 * @fn ILI9341_Select()
 * @brief This function clears the CS pin
 * @details The ILI9341_Select function selects the ILI9341 display by clearing the CS pin.
 */
static void ILI9341_Select();

/**
 * @fn ILI9341_Reset()
 * @brief This function toggles the Reset pin
 * @details The ILI9341_Reset function resets the ILI9341 display by toggling the reset pin.
 */
static void ILI9341_Reset();

/**
 * @fn ILI9341_WriteCommand(uint8_t cmd)
 * @brief This function writes a command to the display
 * @details The ILI9341_WriteCommand function writes a command to the ILI9341 display controller using 
 * SPI communication
 * @param cmd specify the command to be sent to the ILI9341 display controller.
 */
static void ILI9341_WriteCommand(uint8_t cmd);

/**
 * @fn ILI9341_WriteData(uint8_t* buff, size_t buff_size)
 * @brief This function writes a data to the display
 * @details The ILI9341_WriteData function writes data to the ILI9341 display controller using 
 * SPI communication
 * @param buff pointer to an array that contains the data to be written to the ILI9341 display
 * @param buff_size size of the buffer contains the data to be written to the ILI9341 display 
 */
static void ILI9341_WriteData(uint8_t* buff, size_t buff_size);

/**
 * @fn ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
 * @brief This function sets the address window
 * @details The function `ILI9341_SetAddressWindow` sets the address window for writing data 
 * to the ILI9341 display controller
 * @param x0 x0 is the starting x-coordinate of the address window for the ILI9341 display
 * @param y0 y0 is the starting y-coordinate of the address window for the ILI9341 display
 * @param x1 x1 is the ending x-coordinate of the address window for the ILI9341 display
 * @param y1 y1 is the ending y-coordinate of the address window for the ILI9341 display
 */
static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @fn ILI9341_Init() 
 * @brief This function initializes the display
 * @details The function `ILI9341_Init` initializes an ILI9341 display by sending a series of 
 * commands and data to configure various settings and parameters.
 */
void ILI9341_Init();

/**
 * @fn ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
 * @brief This function draws a single pixel 
 * @details The ILI9341_DrawPixel function draws a single pixel of a specified color at the given coordinates on
 * an ILI9341 display
 * @param x x-coordinate of the pixel on the ILI9341 display where you want to draw the pixel
 * @param y y-coordinate of the pixel on the ILI9341 display where you want to draw the pixel
 * @param color color of the pixel to be drawn on the ILI9341 display
 * @return If the conditions `(x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)` are met, the function
 * `ILI9341_DrawPixel` will return without executing the rest of the code inside the function.
 */
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @fn void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
 * @brief This function writes character
 * @details The function ILI9341_WriteChar writes a character to a specific location on an ILI9341 display 
 * using a specified font, color, and background color
 * @param x x-coordinate where the character will be written on the display
 * @param y y-coordinate where the character will be written on the display
 * @param ch character to write to the ILI9341 display using the specified font
 * @param font The function is of type `FontDef`, a structure containing information about the font to 
 * be used for displaying characters. It includes the width, height, and data of the characters in the font set
 * @param color color of the pixel to be drawn on the ILI9341 display
 * @param bgcolor background color that will be used to fill the area behind the character 
 */
static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);

/**
 * @fn ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
 * @brief This function writes a string of characters
 * @details The ILI9341_WriteString function writes a string of characters to a specified location on an ILI9341
 * display using a specified font, color, and background color.
 * @param x x-coordinate where the character will be written on the display
 * @param y y-coordinate where the character will be written on the display
 * @param str pointer to a constant character array (string) that contains the text to be written
 * @param font The function is of type `FontDef`, a structure containing information about the font to 
 * be used for displaying characters. It includes the width, height, and data of the characters in the font set.
 * @param color color of the pixel to be drawn on the ILI9341 display
 * @param bgcolor background color that will be used to fill the area behind the character
 */
void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

/**
 * @fn ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
 * @brief This function fills rectangular area
 * @details The function ILI9341_FillRectangle fills a rectangular area on an ILI9341 display 
 * with a specified color
 * @param x x-coordinate of the rectangle to be filled on the display
 * @param y y-coordinate of the rectangle to be filled on the display
 * @param w width of the rectangle to be filled with the specified color
 * @param h height of the rectangle to be filled with the specified color
 * @param color color that used to fill the specified rectangle on the ILI9341 display
 * @return If the x or y coordinates are outside the display boundaries, the function will return
 * without performing any operations.
 */
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @fn ILI9341_FillScreen(uint16_t color)
 * @brief This function fills the entire screen
 * @details The ILI9341_FillScreen function fills the entire screen of an ILI9341 display 
 * with a specified color
 * @param color the color to fill the screen 
 */
void ILI9341_FillScreen(uint16_t color);

/**
 * @fn ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
 * @brief This function used to draw an image
 * @details The ILI9341_DrawImage function is used to draw an image on an ILI9341 display at specified
 * coordinates with the given width, height, and pixel data.
 * @param x x-coordinate where the image will be drawn on the display
 * @param y y-coordinate where the image will be drawn on the display
 * @param w width of the image drawn on the ILI9341 display. It specifies the number of pixels 
 * in the horizontal direction of the image.
 * @param h height of the image to drawn on the ILI9341 display. It specifies the number of pixels 
 * in the vertical direction of the image.
 * @param data pointer to an array of contains the pixel data for the image to be drawn
 * on the ILI9341 display, Each element in the array represents the color
 * @return If the conditions in the if statements are met, the function will return without executing
 * the rest of the code.
 */
void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);

/**
 * @fn ILI9341_InvertColors(bool invert)
 * @brief This function toggles the color inversion
 * @details The function ILI9341_InvertColors toggles the color inversion on an ILI9341 display 
 * based on the invert parameter
 * @param invert determines whether to invert the colors on the ILI9341 display. 
 * If `invert` is true, the colors will be inverted (white becomes black, black
 * becomes white). If `invert` is false, the colors will not be inverted.
 */
void ILI9341_InvertColors(bool invert);

#endif /* ILI9341_H_ */
