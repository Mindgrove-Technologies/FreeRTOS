#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "platform.h"
#include "log.h"

#define STARTMM 0x90000000                                                      
#define ENDMM   0x9FFFFFFF

#define WRITE 0
#define READ 1

#define FIFO_FULL 16
#define FIFO_EMPTY 0

#define FIFO_DEPTH_8  16
#define FIFO_DEPTH_16 FIFO_DEPTH_8/2
#define FIFO_DEPTH_32 FIFO_DEPTH_8/4

#define QSPI0 0
#define QSPI1 1

#define CCR_IMODE_NIL             0x0
#define CCR_IMODE_SINGLE_LINE     0x1
#define CCR_IMODE_TWO_LINE        0x2
#define CCR_IMODE_FOUR_LINE       0x3
#define CCR_ADMODE_NIL            0x0
#define CCR_ADMODE_SINGLE_LINE    0x1
#define CCR_ADMODE_TWO_LINE       0x2
#define CCR_ADMODE_FOUR_LINE      0x3
#define CCR_ADSIZE_8_BIT          0x0
#define CCR_ADSIZE_16_BIT         0x1
#define CCR_ADSIZE_24_BIT         0x2
#define CCR_ADSIZE_32_BIT         0x3
#define CCR_ABMODE_NIL            0x0
#define CCR_ABMODE_SINGLE_LINE    0x1
#define CCR_ABMODE_TW0_LINE       0x2
#define CCR_ABMODE_FOUR_LINE      0x3
#define CCR_ABSIZE_8_BIT          0x0
#define CCR_ABSIZE_16_BIT         0x1
#define CCR_ABSIZE_24_BIT         0x2
#define CCR_ABSIZE_32_BIT         0x3
#define CCR_DMODE_NO_DATA         0x0
#define CCR_DMODE_SINGLE_LINE     0x1
#define CCR_DMODE_TWO_LINE        0x2
#define CCR_DMODE_FOUR_LINE       0x3
#define CCR_FMODE_INDIRECT_WRITE  0x0
#define CCR_FMODE_INDIRECT_READ   0x1
#define CCR_FMODE_APM             0x2
#define CCR_FMODE_MMM             0x3
#define CCR_MM_MODE_XIP           0x0
#define CCR_MM_MODE_RAM           0x1

//! Enable QSPI_DEBUG for debugging purposes.
// #define QSPI_DEBUG 1
int qspi_init(int qspi_num);
void qspi_config_cr(int qspi_num, int prescale, int pmm, int apms, int toie, int smie, int ftie, int tcie, int teie, int fifo_th, int timeout_en);
void qspi_enable(int qspi_num);
void qspi_disable(int qspi_num);
void qspi_abort(int qspi_num);
void qspi_config_dcr(int qspi_num, int mode_byte, int fmem_size, int clk_mode);
uint32_t qspi_read_status(int qspi_num);
int qspi_check_fifo_full(int qspi_num);
int qspi_check_fifo_empty(int qspi_num);
void qspi_wait_till_tx_complete(int qspi_num);
void qspi_wait_till_not_busy(int qspi_num);
void qspi_clear_flags(int qspi_num, int clr_tof, int clr_smf, int clr_tcf, int clr_tef);
int qspi_data_len(int qspi_num, uint32_t data_len, int wr_rd);
void qspi_config_ccr(int qspi_num, uint8_t instr, int imode, int admode, int adsize, int abmode, int absize,\
    int dummy_cycles, int dummy_conf, int dmode, int fmode, int sioo, int dummy_bit, int ddr_mode,int mm_mode);
int qspi_config_ar(int qspi_num, uint32_t addr, int fmode);
void qspi_config_abr(int qspi_num, uint32_t alt_byte);
void qspi8_wr_dr(int qspi_num, uint8_t* tx_data, int data_size);
void qspi16_wr_dr(int qspi_num, uint16_t tx_data[FIFO_DEPTH_16], int data_size);
void qspi32_wr_dr(int qspi_num, uint32_t tx_data[FIFO_DEPTH_32], int data_size);
void qspi_wait_till_rx_fifo_fills(int qspi_num, uint32_t data_len);
uint8_t qspi8_rd_dr(int qspi_num);
uint16_t qspi16_rd_dr(int qspi_num);
uint32_t qspi32_rd_dr(int qspi_num);
void qspi_config_psmkr(int qspi_num, uint32_t status_mask);
void qspi_config_psmar(int qspi_num, uint32_t status_match);
void qspi_config_pir(int qspi_num, uint32_t poll_interval);
void qspi_config_lptr(int qspi_num, uint32_t time_out);
uint8_t qspi_check_transaction_complete (int qspi_num);
void qspi_wait_till_transaction_complete (int qspi_num);


#ifdef __cplusplus
}
#endif
