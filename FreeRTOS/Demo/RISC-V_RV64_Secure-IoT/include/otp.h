#ifndef OTP_H
#define OTP_H
#endif
#include <stdint.h>
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OTP_BASE_ADDRESS  0x33000

#define OTP_CONTROLREG    0x00
#define OTP_START         1 << 0
#define OTP_R_W           1 << 1
/* READ  - 1
 * WRITE - 0
 */
#define OTP_STARTUP       1 << 2

#define OTP_STATUS        0x08
#define OTP_GET_OUTPUT    1 << 0
#define OTP_PROG_STATUS   1 << 1
#define OTP_FATAL         1 << 2
#define OTP_STARTUP_DONE  1 << 3

#define OTP_ADDR          0x10
#define OTP_READ_DATA     0x18
#define OTP_WRITE_DATA    0x20

typedef struct {
  uint32_t control;
  uint32_t reserved0;
  volatile uint32_t status;
  uint32_t reserved1;
  uint32_t addr;
  uint32_t reserved2;
  volatile uint32_t read_data;
  uint32_t reserved3;
  uint32_t write_data;
  uint32_t reserved4;
} otp_struct;

int otp_init();
uint8_t otp_read(uint32_t addr_in);
int otp_write(uint32_t addr_in, uint32_t data_in);
uint8_t otp_read_32bit_data(uint32_t addr_in, uint8_t* data_out, uint8_t data_len);

#ifdef __cplusplus
}
#endif
