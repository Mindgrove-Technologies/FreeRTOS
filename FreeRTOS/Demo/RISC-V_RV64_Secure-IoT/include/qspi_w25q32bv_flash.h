#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H

#include <stdint.h>
#include <qspi.h>

#define PRESCALE_QSPI 9

// Function prototypes
void fastReadQuad(uint8_t qspinum, uint8_t* data, uint32_t address, uint8_t data_length);
void fastReadQuad32(uint8_t qspinum,uint32_t* data,uint32_t address,uint8_t data_length);
void fastReadQuadIO(uint8_t qspinum, uint8_t* data, uint32_t address, uint8_t data_length);
void inputpageQuad(uint8_t qspinum, uint8_t* data, uint32_t address, uint8_t data_length);
void sector4KErase(uint8_t qspinum, uint32_t address);
void sector32KErase(uint8_t qspinum, uint32_t address);
void chipErase(uint8_t qspinum);
void writeEnable(uint8_t qspinum);
void writeDisable(uint8_t qspinum);
void suspend(uint8_t qspinum);
void continuosmode_reset(uint8_t qspinum);
void resume(uint8_t qspinum);
void power_down(uint8_t qspinum);
void release_power_down(uint8_t qspinum);
uint8_t readStatusRegister(uint8_t qspinum);
uint8_t readFlagStatusRegister(uint8_t qspinum);
void writeEnableStatusRegister(uint8_t qspinum);
void writeStatusRegister(uint8_t qspinum, uint8_t* statusData);
uint8_t readGlobalFreezeBit(uint8_t qspinum);
void writeGlobalFreezeBit(uint8_t qspinum);
uint8_t readFlashSFDP(uint8_t qspinum,uint32_t address);
uint8_t readNVCR(uint8_t qspinum, uint8_t* data);
void writeNVCR(uint8_t qspinum, uint8_t* data);
void qspi_xip_init(uint8_t qspinum, int flash_size);

#endif // FLASH_DRIVER_H
