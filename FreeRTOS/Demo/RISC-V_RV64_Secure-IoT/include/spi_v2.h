/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : spi.h
 * Brief Description of file         : Header to Standard spi driver
 * Name of Author                    : Kapil Shyam. M
 * Email ID                          : kapilshyamm@gmail.com
 * 
 * @file spi.h
 * @author Kapil Shyam. M (kapilshyamm@gmail.com)
 * @brief This is a Baremetal SSPI Driver's Header file for Mindgrove Silicon's SPI Peripheral
 * @version 0.2
 * @date 2023-04-16
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2023. All rights reserved.
 * 
 */
#include <stdint.h>
#include "platform.h"
#include "log.h"

/*! `sspi_struct *sspi_instance[SSPI_MAX_COUNT];` is declaring an array of pointers to structures of
type `sspi_struct`. The size of the array is defined by the constant `SSPI_MAX_COUNT`. This array is
used to store the base addresses of the different SPI instances that are initialized in the system.
Each element of the array corresponds to a specific SPI instance, and the base address of that
instance is stored in the corresponding element of the array. This allows the driver to access the
registers of a specific SPI instance by using its base address. */
//extern sspi_struct *sspi_instance[SSPI_MAX_COUNT];

#define FIFO_DEPTH_8  32
#define FIFO_DEPTH_16 FIFO_DEPTH_8/2
#define FIFO_DEPTH_32 FIFO_DEPTH_8/4


#define MASTER 1
#define SLAVE 0

#define DISABLE 0
#define ENABLE 1

#define LSB_FIRST 1
#define MSB_FIRST 0

#define SIMPLEX_TX 0
#define SIMPLEX_RX 1
#define HALF_DUPLEX 2
#define FULL_DUPLEX 3

#define SUCCESS 0
#define FAILURE -1
#define TIMEOUT -2

#define SPI0 0
#define SPI1 1
#define SPI2 2
#define SPI3 3

/**
 * @fn void spi_enable(int spi_number)
 * 
 * @brief The function enables a specified SPI instance.
 * 
 * @param spi_number The spi_number parameter is an integer that represents the SPI instance number. It
 * is used to select the specific SPI peripheral that needs to be enabled.
 */
void sspi_enable(int spi_number);

/**
 * @fn void spi_disable(int spi_number)
 * 
 * @brief This function disables a specified SPI instance.
 * 
 * @param spi_number The parameter `spi_number` is an integer that represents the SPI 
 * instance number. It is used to identify the specific SPI peripheral that needs to be disabled.
 */
void sspi_disable(int spi_number);

/**
 * @fn int sspi_init(int spi_number)
 * @details The function initializes a SPI instance and returns its base address.
 * 
 * @param spi_number The SPI instance number that needs to be initialized. It should be an integer
 * value between 0 and SSPI_MAX_COUNT-1.
 * 
 * @return the base address of the SPI instance that was initialized. If the initialization fails due
 * to an invalid SPI instance number, it returns -1.
 */
int sspi_init(int spi_number);

/**
 * @fn void sspi_busy(int spi_number)
 * @brief The function waits until the SPI communication is busy.
 * @details It is used to select the specific SPI instance that needs to be checked for busy status.
 * @param spi_number The parameter \a spi_number is an integer that represents the SPI instance number.
 * 
 */
void sspi_busy(int spi_number);

/**
 * @fn void sclk_config( int spi_number, int pol, int pha, int prescale, int setup_time, int hold_time)
 * 
 * @details The function configures the clock control settings for a specified SPI instance.
 * 
 * @param spi_number The SPI (Serial Peripheral Interface) Instance number for which the configuration is being
 * applied to.
 * 
 * @param pol The polarity of the clock signal. It determines the idle state of the clock signal. If it
 * is set to 0, the clock signal is low when idle. If it is set to 1, the clock signal is high when
 * idle.
 * 
 * @param pha The pha parameter in the sclk_config function is used to set the clock phase of the SPI
 * (Serial Peripheral Interface) bus. It determines the timing relationship between the data and the
 * clock signal. A value of 0 indicates that data is captured on the leading edge of the clock signal,
 * while
 * 
 * @param prescale The prescale parameter is used to set the clock frequency of the SPI bus. It
 * determines the division factor of the system clock frequency to generate the SPI clock frequency.
 * 
 * @param setup_time The setup time parameter in the sclk_config function is the delay between the
 * assertion of the chip select signal and the start of the clock signal. It is the time required for
 * the slave device to prepare for data transfer.
 * 
 * @param hold_time Hold time refers to the amount of time that the SPI clock signal should be held low
 * after the data has been transmitted. This is important to ensure that the receiving device has
 * enough time to sample the data correctly. The hold time is typically specified in terms of clock
 * cycles.
 */
void sclk_config( int spi_number, int pol, int pha, int prescale, int setup_time, int hold_time);

/**
 * @fn void sspi_comm_control_config(int spi_number, int master_mode, int lsb_first, int comm_mode, int spi_size)
 * 
 * @details The function configures the communication control for a specified SPI Instance based on the given
 * parameters.
 * 
 * @param spi_number The SPI Instance number to be configured.
 * 
 * @param master_mode Specifies whether the SPI interface is operating in master or slave mode. It can
 * take the values MASTER or SLAVE which is defined in the \a sspi.h file.
 * 
 * @param lsb_first The lsb_first parameter is a flag that determines whether the least significant bit
 * (LSB) or the most significant bit (MSB) is transmitted first in the SPI communication. If lsb_first
 * is set to 1, the LSB is transmitted first, and if it is set to 0, MSB will be transmitted first.
 * 
 * @param comm_mode The communication mode of the SPI interface, which can be set to either 
 * \arg SPI_MODE_0 = SIMPLEX_TX,
 * \arg SPI_MODE_1 = SIMPLEX_RX,
 * \arg SPI_MODE_2 = HALF_DUPLEX, or
 * \arg SPI_MODE_3 = FULL_DUPLEX. 
 * These defines are included in the \a sspi.h .
 * 
 * @param spi_size spi_size refers to the total number of bits to be transmitted and received in each
 * SPI transaction. It is used to configure the SPI communication control register. Mostly, the values for 
 * the \a spi_size will be 8, 16. and 32.
 * 
 */
void sspi_comm_control_config(int spi_number, int master_mode, int lsb_first, int comm_mode, int spi_size);
int sspi8_transmit_data(int spi_number, uint8_t tx_data[FIFO_DEPTH_8], uint8_t data_size);
int sspi16_transmit_data(int spi_number, uint16_t tx_data[FIFO_DEPTH_16], uint8_t data_size);
int sspi32_transmit_data(int spi_number, uint32_t tx_data[FIFO_DEPTH_32], uint8_t data_size);
void sspi_wait_till_tx_not_en(int spi_number);
void sspi_wait_till_tx_complete(int spi_number);
int sspi_check_tx_fifo_32(int spi_number);
int sspi_check_tx_fifo_30(int spi_number);
int sspi_check_tx_fifo_28(int spi_number);
//int sspi_receive_data(int spi_number, int spi_size);
void sspi_wait_till_rxfifo_not_empty(int spi_number);
void sspi_wait_till_rxfifo_2(int spi_number);
void sspi_wait_till_rxfifo_4(int spi_number);

void sspi8_receive_data(int spi_number, uint8_t rx_data[FIFO_DEPTH_8], uint8_t length);
void sspi16_receive_data(int spi_number, uint16_t rx_data[FIFO_DEPTH_16], uint8_t data_size);
void sspi32_receive_data(int spi_number, uint32_t tx_data[FIFO_DEPTH_32], uint8_t data_size);
                                              
void inter_enable_config(int spi_number, uint32_t value);
