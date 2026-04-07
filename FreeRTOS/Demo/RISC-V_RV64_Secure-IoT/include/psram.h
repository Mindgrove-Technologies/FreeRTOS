/**
 * @file psram.h
 * @author Sai Kiran Narayanaswami (saikiran@mindgrovetech.in)
 * @author Daniel Mark Issac (daniel@mindgrovetech.in)
 * @brief This is the header for the APS6404L-SQN QSPI PSRAM Peripheral driver for Mindgrove SecureIOT.
 * @version 1.0
 * @date 2025-04-10
 *
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 *
 */
#ifndef PSRAM_DRIVER_H
#define PSRAM_DRIVER_H

#define PSRAM_INSTR_ENTER_QUAD_MODE 0x35
#define PSRAM_INSTR_QUAD_WRITE 0x38
#define PSRAM_INSTR_FAST_READ_QUAD 0xEB

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * PSRAM specific configuration functions.
 */

/**
 * @fn uint32_t psram_init_mmap(uint8_t qspinum, int flash_size,uint8_t fthresh, uint8_t access_mode)
 *
 * @brief Used to configure SPI for PSRAM initialisation in Memory Mapped mode. `psram_reset` needs to be
 * called before this function. Initialization needs to be done each time the access mode (read/write) is changed,
 * or the data width.
 * WARNING: Writing multiple bytes at a time causes endianness to be flipped.
 *
 * @param qspinum The parameter \a qspinum is an unsigned integer that represents the QSPI instance number.
 * @param ram_size The parameter \a ram_size specifies psram size in bytes, expressed as its base-2 log
 * (e.g 27 signifies a size of 2^27 bytes).
 * @param fthresh Specifies the data width: if 0 - 8 bit mode,1-16 mode,3 - 32 bit mode,7- 64 bit mode.             .
 * @param access_mode The parameter \a access_mode specifies whether to initialize (only) for reads (0) or writes (1).
 *
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_init_mmap(uint8_t qspinum, int ram_size, uint8_t fthresh, uint8_t access_mode);

/**
 * @fn uint32_t psram_init_mmap_quad(uint8_t qspinum, int flash_size,uint8_t fthresh, uint8_t access_mode)
 *
 * @brief Same as psram_init_mmap() , but initializes in QSPI Quad/Four-Line mode.
 * Requires the PSRAM_INSTR_ENTER_QUAD_MODE instruction to be issued prior to calling
 * this function (see psram_issue_instruction()).
 *
 * @param qspinum The parameter \a qspinum is an unsigned integer that represents the QSPI instance number.
 * @param ram_size The parameter \a ram_size specifies psram size in bytes, expressed as its base-2 log
 * (e.g `27` signifies a size of \f(2^{27}\f) bytes).
 * @param fthresh Specifies the data width: if 0 - 8 bit mode, 1-16 mode,3 - 32 bit mode, 7- 64 bit mode.             .
 * @param access_mode The parameter \a access_mode specifies whether to initialize (only) for reads (0) or writes (1).
 *
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_init_mmap_quad(uint8_t qspinum, int ram_size, uint8_t fthresh, uint8_t access_mode);

/**
 * @brief Enable the software reset functionality of the PSRAM.
 * Needs to be called before param_reset().
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param ram_size Specifies PSRAM size in bytes, expressed as its base-2 log.
 * (e.g `27` signifies a size of \f$2^{27}\f$ bytes).
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_reset_enable(uint8_t qspinum, int ram_size);

/**
 * @brief Reset the PSRAM. Needs to be called before any data transactions.
 * Requires param_reset_enable() to be called prior to this function.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param ram_size Specifies PSRAM size in bytes, expressed as its base-2 log.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_reset(uint8_t qspinum, int ram_size);

/**
 * @brief Issue a supported instruction to the PSRAM without any
 * data transfer, e.g PSRAM_INSTR_ENTER_QUAD_MODE to enter Quad/Four-Line mode.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param ram_size Specifies PSRAM size in bytes, expressed as its base-2 log.
 * @param instruction The code for the instruction to be issued as specified in the
 * PSRAM's datasheet.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_issue_instruction(uint8_t qspinum, int ram_size, uint8_t instruction);

/**
 * @brief Write a block of data in Quad mode.
 * WARNING: Writing multiple bytes at a time causes endianness to be flipped.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param data `uint8_t` type pointer to the data to be written.
 * @param address Starting address of the write operation (zero based).
 * @param data_length Number of bytes to be written.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_inputpageQuad(uint8_t qspinum, uint8_t *data, uint32_t address, uint32_t data_length);

/**
 * @brief Read a block of data in Quad mode.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param data `uint8_t` type pointer to memory where the read data is to be stored.
 * @param address Starting address of the read operation (zero based).
 * @param data_length Number of bytes to be read.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_fastReadQuad(uint8_t qspinum, uint8_t *data, uint32_t address, uint32_t data_length);

/**
 * @brief Read a block of data in Single-line mode.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param data `uint8_t` type pointer to memory where the read data is to be stored.
 * @param address Starting address of the read operation (zero based).
 * @param data_length Number of bytes to be read.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_fastReadSingle(uint8_t qspinum, uint8_t *data, uint32_t address, uint32_t data_length);

/**
 * @brief Read a block of data in Single-line mode.
 * @param qspinum Unsigned integer specifying the QSPI instance number.
 * @param data `uint8_t` type pointer to memory where the read data is to be stored.
 * @param address Starting address of the read operation (zero based).
 * @param data_length Number of bytes to be read.
 * @return SUCCESS if operation is successful and ENODEV if invalid instance number.
 */
uint32_t psram_inputpageSingle(uint8_t qspinum, uint8_t *data, uint32_t address, uint32_t data_length);

#ifdef __cplusplus
}
#endif

#endif // PSRAM_DRIVER_H
