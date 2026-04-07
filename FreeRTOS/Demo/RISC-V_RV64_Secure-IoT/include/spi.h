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
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2023. All rights reserved.
 * 
 */

#ifndef SPI_H
#define SPI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "log.h"
#include "errors.h"
#include "secure_iot.h"
#include <stdbool.h>
#include "gptimer.h"

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

#define FAILURE -1
#define TIMEOUT1 -2

#define SPI0 0
#define SPI1 1
#define SPI2 2
#define SPI3 3

#define DATA_SIZE_8 8
#define DATA_SIZE_16 16
#define DATA_SIZE_32 32


typedef struct 
{
    uint8_t spi_number;
    uint8_t pol             :1;
    uint8_t pha             :1;
    uint8_t prescale;
    uint8_t setup_time;
    uint8_t hold_time;
    uint8_t spi_mode        :1;
    uint8_t lsb_first       :1;
    uint8_t comm_mode;
    uint8_t spi_size;
    uint8_t bits;
    uint8_t configure       :1;
}SPI_Config_t;


struct spi_buf {
	void *buf;
	int len;
};
struct spi_buf_set {
	const struct spi_buf *buffers;
	int count;
};

/**
 * @fn The function SPI_Transceive(SPI_Config_t *config, const struct spi_buf_set *tx_bufs, const struct spi_buf_set *rx_bufs) 
 * handles SPI communication based on the specified configuration and buffer sets for full duplex, simplex transmit, simplex receive, 
 * and half duplex modes with different data sizes.
 * 
 * @param config The `config` parameter in the `SPI_Transceive` function is a pointer to a structure of
 * type `SPI_Config_t`. This structure likely contains configuration settings for the SPI
 * communication, such as communication mode (full duplex, simplex TX, simplex RX, half duplex) and
 * data size (8-bit
 * @param tx_bufs `tx_bufs` is a pointer to a struct `spi_buf_set` which contains information about the
 * transmit buffers for SPI communication. It likely includes details such as the buffer length, buffer
 * data, and any other necessary configuration for transmitting data over SPI.
 * @param rx_bufs The `rx_bufs` parameter in the `SPI_Transceive` function is a pointer to a struct
 * `spi_buf_set` which contains information about the receive buffers for the SPI communication. It
 * allows the function to read data from the SPI bus into the specified receive buffers.
 * 
 * @return The function `SPI_Transceive` is returning a value `SUCCESS`.
 */
uint8_t SPI_Transceive(SPI_Config_t *config, const struct spi_buf_set *tx_bufs,
			                    const struct spi_buf_set *rx_bufs);


#ifdef __cplusplus
}
#endif
#endif