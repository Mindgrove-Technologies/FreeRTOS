
/**
 * @file  gpiov2.h
 * @project shakti devt board
 * @brief  header file for gpio driver
 */

#include <stdint.h>
#include "platform.h"

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

void check_gpiov2();
void gpiov2_init();
long int gpiov2_read_word(int *addr);
void gpiov2_write_word(int *addr, unsigned long val);
void gpiov2_set_direction(unsigned long gpio_pin,int direction);
void gpiov2_set(unsigned long gpio_pin, int value);
void gpiov2_clear(unsigned long gpio_pin);
void gpiov2_toggle(unsigned long gpio_pin);
void gpiov2_interrupt_config(unsigned long, int toggleInterupt);
long int gpiov2_read_data_register();
