#include<stdint.h>

#define I2C_PIN 0x80
#define I2C_ESO 0x40
#define I2C_ES1 0x20
#define I2C_ES2 0x10
#define I2C_ENI 0x08
#define I2C_STA 0x04
#define I2C_STO 0x02
#define I2C_ACK 0x01

#define I2C_INI 0x40   
#define I2C_STS 0x20
#define I2C_BER 0x10
#define I2C_AD0 0x08
#define I2C_LRB 0x08
#define I2C_AAS 0x04
#define I2C_LAB 0x02
#define I2C_BB  0x01

#define I2C_START         (I2C_PIN | I2C_ESO | I2C_STA | I2C_ACK)
#define I2C_STOP          (I2C_PIN | I2C_ESO | I2C_STO | I2C_ACK)
#define I2C_REPSTART      (                 I2C_ESO | I2C_STA | I2C_ACK)
#define I2C_IDLE          (I2C_ESO                  | I2C_ACK)
#define I2C_NACK          (I2C_ESO)
#define I2C_DISABLE       (I2C_PIN|I2C_ACK)


#define I2C_READ 1
#define I2C_WRITE 0
#define MAX_I2C_COUNT 2



/*
`define     S2             8'h00
`define     Control        8'h08
`define     S0             8'h10
`define     Status         8'h18
`define     S01            8'h20
`define     S3             8'h28
`define     Time           8'h30
`define     SCL            8'h38
*/

#ifdef __cplusplus
extern "C" {
#endif





#define I2C0 0
#define I2C1 1
#define WRITE_MODE 0
#define READ_MODE 1

void i2c_start_bit_(uint8_t);
void i2c_begin(uint8_t,uint8_t,uint32_t);
void i2c_target_address(uint8_t,uint8_t,uint8_t);
void wait_till_txrx_operation_Completes_(uint8_t);
void wait_till_I2c_bus_free_(uint8_t);
void i2c_write_byte(uint8_t,uint8_t);
void i2c_end(uint8_t);
void i2c_disable(uint8_t);
uint8_t i2c_read_byte(uint8_t i2c_number);

#ifdef __cplusplus
}
#endif
