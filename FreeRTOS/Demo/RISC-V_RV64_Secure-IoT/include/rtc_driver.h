#include "i2c_v2.h"//Includes the definitions of i2c communication protocol//
#include "log.h"
#include "utils.h"
#include "platform.h"
#include <stdint.h>
#include<stdbool.h>

#define CLOCK_H24 0
#define CLOCK_H12 1

#define HOUR_AM 0
#define HOUR_PM 1
#define HOUR_24 2

#define DS3231_ADDR 0x68


/*ftypes*/
uint8_t bitSet(uint8_t data,uint8_t shift);
uint8_t bitClear(uint8_t data,uint8_t shift);
uint8_t bitRead(uint8_t data,uint8_t shift);
void DS3231_begin(uint8_t i2c_number,uint8_t prescale_div,uint32_t scl_div);
uint8_t bitWrite(uint8_t data,uint8_t shift,uint8_t bit);
void DS3231_setHourMode(uint8_t i2c_number,uint8_t h_mode);
uint8_t DS3231_getHourMode(uint8_t i2c_number);
void DS3231_setMeridiem(uint8_t i2c_number,uint8_t meridiem);
uint8_t DS3231_getMeridiem(uint8_t i2c_number);
uint8_t DS3231_getSeconds(uint8_t i2c_number);
void DS3231_setSeconds(uint8_t i2c_number,uint8_t seconds);
uint8_t DS3231_getMinutes(uint8_t i2c_number);
void DS3231_setMinutes(uint8_t i2c_number,uint8_t minutes);
uint8_t DS3231_getHours(uint8_t i2c_number);
void  DS3231_setHours(uint8_t i2c_number,uint8_t hours);
uint8_t DS3231_getWeek(uint8_t i2c_number);
void DS3231_setWeek(uint8_t i2c_number,uint8_t week);
void DS3231_updateWeek(uint8_t i2c_number);
uint8_t DS3231_getDay(uint8_t i2c_number);
void DS3231_setDay(uint8_t i2c_number,uint8_t day);
uint8_t DS3231_getMonth(uint8_t i2c_number);
void DS3231_setMonth(uint8_t i2c_number,uint8_t month);
uint16_t DS3231_getYear(uint8_t i2c_number);
void DS3231_setYear(uint8_t i2c_number,uint16_t year);
void DS3231_setTime(uint8_t i2c_number,uint8_t hours, uint8_t minutes, uint8_t seconds);
void DS3231_setDate(uint8_t i2c_number,uint8_t day, uint8_t month, uint16_t year);
void DS3231_setDateTime(uint8_t i2c_number,char* date, char* time);
uint8_t bcd2bin(uint8_t val);
uint8_t bin2bcd(uint8_t val);








