#include <stdint.h>

#ifdef __cplusplus
extern "c"{
#endif

#define ADC_BASE_ADDRESS  0x00032000

/* control register */
#define ADC_CONTROLREG    0x00
#define ADC_DISLVL        0x0001
#define ADC_INIT          0x0002
#define ADC_SOC           0x0020
#define ADC_FREE_RUN      0x0100
#define ADC_INTR_ENABLE   0x0200
#define ADC_12BIT         0x00C0
#define ADC_10BIT         0x0080
#define ADC_8BIT          0x0040
#define ADC_6BIT          0x0000
#define ADC_CHANNEL0      0x0000  
#define ADC_CHANNEL1      0x0004
#define ADC_CHANNEL2      0x0008
#define ADC_CHANNEL3      0x000C
#define ADC_CHANNEL4      0x0010
#define ADC_CHANNEL5      0x0014
#define ADC_CHANNEL6      0x0018
#define ADC_CHANNEL7      0x001C

/* output register */
#define ADC_OUTPUTREG     0x04
#define ADC_EOC           0x1000
#define ADC_OUTPUT_MASK   0x0FFF


void adc_init();
void config_control_reg(int channel, int resolution, int en_freerun, int en_intr);
int start_conversion();
int read_output();
int end_conversion();


#ifdef __cplusplus
}
#endif