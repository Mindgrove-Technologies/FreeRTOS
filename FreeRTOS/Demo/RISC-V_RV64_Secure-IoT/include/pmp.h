#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#define rows    3
#define cols    3

#define PMP_A   (3 << 3)  // Address matching type
#define PMP_A_NA4   (1 << 3)  // Naturally aligned four-byte region

#define SIZE    3 // Fixed size for 3x3 matrix
#define PMP_R   0x01  
#define PMP_W   0x02
#define PMP_X   0x04    
#define PMP_A_TOR   0x08    // Top of Range
#define PMP_A_NAPOT 0x18    // Naturally aligned power of two
#define PMP_L   0x80


#define CSR_PMPCFG0  0x3A0   // PMP Configuration register
#define CSR_PMPADDR0 0x3B0   // PMP Address register

#define __riscv_xlen 64
#define PMP_SHIFT    3
#define PMP_COUNT    8// Typically, 8 PMP registers

static inline unsigned long csr_pmpaddr_read_n(int n);
static inline unsigned long csr_pmpcfg_read();
static inline void csr_pmpaddr_write_n(int n, uint64_t value);
static inline void csr_pmpcfg_write(uint64_t value);
int pmp_disable(int n);
static unsigned long ctz(unsigned long x);
int is_pmp_entry_mapped(unsigned long entry);
int pmp_set(int n, uint8_t prot, uint64_t addr, int log2len);
int pmp_get(int n);
uint64_t generate_random_bits();