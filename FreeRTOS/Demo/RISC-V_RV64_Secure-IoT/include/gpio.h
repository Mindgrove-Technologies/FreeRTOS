
/**
 * @file  gpio.h
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

#define GPIO_IN  0x00000000
#define GPIO_OUT 0xFFFFFFFF
#define GPIO_QUAL_MAX_CYCLES 15
#define ALL_GPIO_PINS -1

#ifdef __cplusplus
extern "C" {
#endif
/* Struct to access GPIO registers as 16 bit registers */
typedef struct
{
	uint32_t  direction;	           /*! direction register */
	uint32_t  reserved0;                /*! reserved for future use */
	uint32_t  data;	        		   /*! data register */
	uint32_t  reserved1;                /*! reserved for future use */
	uint32_t  set;	         	   /*! set register */
	uint32_t  reserved2;                /*! reserved for future use */
	uint32_t  clear;	         	   /*! clear register */
	uint32_t  reserved3;                /*! reserved for future use */
	uint32_t  toggle;	            /*! toggle register */
	uint32_t  reserved4;                /*! reserved for future use */
	uint8_t  qualification;	   /*! qualification register */
	uint8_t  reserved5;                /*! reserved for future use */
	uint16_t  reserved6;              /*! reserved for future use */
	uint32_t  reserved12;              /*! reserved for future use */
	uint32_t  intr_config;	   /*! interrupt configuration register */
//	uint16_t  reserved13;              /*! reserved for future use */
	uint32_t  reserved7;              /*! reserved for future use */
} gpio_struct;

extern gpio_struct *gpio_instance;

extern void check_gpio();
extern void gpio_init();
extern long int gpio_read_word(int *addr);
extern void gpio_write_word(int *addr, unsigned long val);
extern void gpio_set_direction(unsigned long gpio_pin,int direction);
extern void gpio_set(unsigned long);
extern void gpio_clear(unsigned long);
extern void gpio_toggle(unsigned long);
extern void gpio_interrupt_config(unsigned long, int toggleInterupt);
extern void gpio_set_qualification_cycles(unsigned int);
extern long int gpio_read_data_register();

#ifdef __cplusplus
}
#endif