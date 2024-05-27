#include "i2c_v2.h"//Includes the definitions of i2c communication protocol
#include "log.h"
#include "utils.h"
#include "platform.h"
#include <stdint.h>
#include<stdbool.h>
#include <stdarg.h>
#include "uart.h"
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

#define PRESCALE_DIV 39
#define SCL_DIV 19
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
#define lcd_data data




  void lcd_clear(uint8_t,uint8_t);
  void lcd_home(uint8_t,uint8_t);

  void lcd_noDisplay(uint8_t,uint8_t);
  void lcd_display(uint8_t,uint8_t);
  void lcd_noBlink(uint8_t,uint8_t);
  void lcd_blink(uint8_t,uint8_t);
  void lcd_noCursor(uint8_t,uint8_t);
  void lcd_cursor(uint8_t,uint8_t);
  void lcd_scrollDisplayLeft(uint8_t,uint8_t);
  void lcd_scrollDisplayRight(uint8_t,uint8_t);
  void lcd_leftToRight(uint8_t,uint8_t);
  void lcd_rightToLeft(uint8_t,uint8_t);
  void lcd_autoscroll(uint8_t,uint8_t);
  void lcd_noAutoscroll(uint8_t,uint8_t);
  //void lcd_createChar(uint8_t, uint8_t[]);
  void lcd_setCursor(uint8_t,uint8_t,uint8_t,uint8_t); 
  void lcd_backlight(uint8_t,uint8_t);
  void lcd_noBacklight(uint8_t,uint8_t);


void lcd_init(uint8_t,uint8_t,uint8_t,uint8_t);
void transmit_nibble(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
void command(uint8_t,uint8_t,uint8_t);
void data(uint8_t,uint8_t,uint8_t);
void lcd_string(uint8_t,uint8_t,uint8_t *);
void lcd_printnumber(uint8_t,uint8_t,uint32_t);
int lcd_scan(uint8_t i2c_number,uint8_t slave_address,const char* format, ...);
int lcd_printf(uint8_t i2c_number,uint8_t slave_address,const char* fmt, ...);
void lcd_backspace(uint8_t i2c_number,uint8_t slave_address);