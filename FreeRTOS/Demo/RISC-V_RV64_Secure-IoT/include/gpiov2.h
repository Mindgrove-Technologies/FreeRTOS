/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : gpiov2.h
 * Brief Description of file         : Header to Standard gpio driver
 * Name of Author                    : Kapil Shyam. M
 * Email ID                          : kapilshyamm@gmail.com
 * 
 * @file gpiov2.h
 * @author Kapil Shyam. M (kapilshyamm@gmail.com)
 * @brief This is a Baremetal GPIO Driver's Header file for Mindgrove Silicon's GPIO Peripheral
 * @version 0.2
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2023. All rights reserved.
 * 
 */

#include <stdint.h>
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_DIRECTION_CNTRL_REG  (GPIO_START + (0 * GPIO_OFFSET ))
#define GPIO_DATA_REG             (GPIO_START + (1 * GPIO_OFFSET ))
#define GPIO_SET_REG              (GPIO_START + (2 * GPIO_OFFSET ))
#define GPIO_CLEAR_REG            (GPIO_START + (3 * GPIO_OFFSET ))
#define GPIO_TOGGLE_REG           (GPIO_START + (4 * GPIO_OFFSET ))
#define GPIO_QUAL_REG             (GPIO_START + (5 * GPIO_OFFSET ))
#define GPIO_INTERRUPT_CONFIG_REG (GPIO_START + (6 * GPIO_OFFSET ))

#define GPIO_IN  0
#define GPIO_OUT 1
#define GPIO_QUAL_MAX_CYCLES 15
#define ALL_GPIO_PINS -1

void gpiov2_init();
long int gpiov2_read_word(int *addr);
void gpiov2_write_word(int *addr, unsigned long val);
void gpiov2_set_direction(unsigned long gpio_pin,int direction);
void gpiov2_set(unsigned long gpio_pin);
void gpiov2_clear(unsigned long gpio_pin);
void gpiov2_toggle(unsigned long gpio_pin);
void gpiov2_interrupt_config(unsigned long pinNo, int low_ena);
long int gpiov2_read_data_register();
void gpiov2_set_qualification_cycles(unsigned int cycles);
int gpiov2_read_pin_status(uint32_t pin_number);
void gpiov2_write_data_register(unsigned long data_word);

#ifdef __cplusplus
}
#endif
