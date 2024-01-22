/**
 * Project                           : Secure IoT SoC
 * Name of the file                  : gpt.h
 * Brief Description of file         : Header to Standard gp_timer driver
 * Name of Author                    : Suneeth. D
 * Email ID                          : suneethdamodharan@gmail.com
 * 
 * @file gpt.h
 * @author Suneeth. D (suneethdamodharan@gmail.com)
 * @brief This is a Baremetal GP_TIMER Driver's header file for Mindgrove Silicon's General Purpose Timer module.
 * @version 0.2
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2023. All rights reserved.
 * 
 */

#include <stdint.h>
#include "platform.h"

/*! General Purpose Timer*/
#define GPT_MAX_COUNT 4
#define GPT_BASE_OFFSET 0x00000020
#define GPT_BASE_ADDRESS 0x00044200
#define GPT0_BASE_ADDRESS 0x00044200
#define GPT0_END_ADDRESS 0x0004421F
#define GPT1_BASE_ADDRESS 0x00044220
#define GPT1_END_ADDRESS 0x0004423F
#define GPT2_BASE_ADDRESS 0x00044240
#define GPT2_END_ADDRESS 0x0004425F
#define GPT3_BASE_ADDRESS 0x00044260
#define GPT3_END_ADDRESS 0x0004427F

/*! Registers of each GP_TIMER*/
#define CTRL_REG         0x00044200
#define CLK_CNTRL_REG    0x00044204
#define CNTR_VAL_REG     0x00044208
#define RPTD_CNT_REG     0x0004420C
#define DUTY_CYC_REG     0x00044210
#define PERIOD_REG       0x00044214
#define CAPT_IP_REG      0x00044218

/* Bit Specifications of each GP_TIMER's register*/
/* Control Register */

#define GPT_EN              (1<<0)
#define GPT_MODE(x)         (x<<2)
#define GPT_OUTPUT_EN       (1<<4)
#define COUNT_RESET         (1<<5)
#define CONTIN_CNT_EN       (1<<6)
#define PWM_FALL_INTR_EN    (1<<7) 
#define PWM_RISE_INTR_EN    (1<<8)
#define CNTR_OFLOW_INTR_EN  (1<<9)
#define CNTR_UFLOW_INTR_EN  (1<<10)
#define CAPTURE_IP(x)       (x<<15) 

/* Clock Control Reg*/

#define CLK_SRC (0<<0)
#define CLK_PRESCALAR(x)    (x<<1)

/* Duty Cycle Reg*/

#define DUTY_CYC_VAL(x)     (x<<0)

/* Period Reg*/

#define PRD_VAL(x)          (x<<0)

/* Function Declarations*/
/**
 * @fn void gptInit(int gpt_num, int mode, int period, int prescalar, int dutycycle, int cnt_en, int capture_val)
 * 
 * @brief The function initialises the gp_timer with different modes and configures necessary registers
 * 
 * @details It is used to select the specific GPT instance, set the mode, set the prescalar, set the dutycycle, enable
 * continous count and set the input bit to be captured
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * @note \a gpt_num value must be always less than GPT_MAX_COUNT.
 * 
 * @param mode The parameter \a mode is an integer that sets tCONTIN_CNT_ENhe mode of gp_timer
 * @note \a mode value must be always in the range 0<=mode<4.
 * 
 * @param period The parameter \a period is an integer that sets the period of gp_timer till which it needs to count
 * 
 * @param prescalar The parameter \a prescalar is an integer that sets the prescalar value with which the clock gets divided
 * 
 * @param dutycycle The parameter \a dutycycle is an integer that sets the duty cycle value for the pulse generated in PWM
 * 
 * @param cnt_en The paramter \a cnt_en is integer that enables continous count mode of the gptimer
 * 
 * @param capture_val The parameter \a capture_val is an integer that sets the input value to be captured
 *
 * @param output_en The parameter \a output_en is an integer that sets the output_enable high in the control and status register
 * 
 * 
 */
void gptInit(int gpt_num, int mode, int period, int prescalar, int dutycycle, int cnt_en, int capture_val, int output_en);
/**
 * @fn void setPeriod(int period, int gpt_num)
 * 
 * @brief The function sets the period for the specific gpt instance
 * 
 * @details It is used to set the thrrshold value for the specific gpt instance to count 
 * 
 * @param period The parameter \a period is an integer that sets the threshold value for the counter
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 */
void setPeriod(int period, int gpt_num);
/**
 * @fn void setDutycycle(int dutycycle, int gpt_num, int period)
 * 
 * @brief The function sets the duty cycle for the specific gpt instance
 * 
 * @details It is used to set the duty cycle for the pulse generated in PWM mode by 
 * the specific GPT instance. 
 * 
 * @param dutycycle The parameter \a dutycycle is an integer that sets the dutycycle value for the counter
 * @note \a dutycycle value must be always 0<dutycycle<=100.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 */
void setDutycycle(int dutycycle, int gpt_num, int period);
/**
 * @fn void setPrescalar(int prescalar, int gpt_num)
 * 
 * @brief The function sets the prescalar value for the specific gpt instance
 * 
 * @details It is used to set the prescalar value for the specific gpt instance by a factor which the clk is divided
 * and the counter counts the value of clock cycles accordingly. 
 * 
 * @param prescalar The parameter \a prescalar is an integer that sets the prescalar value for the counter
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 */
void setPrescalar(int prescalar, int gpt_num);
/**
 * @fn int readCounterVal(int gpt_num)
 * 
 * @brief The function reads the value of the counter.
 * 
 * @details It is used to read the value of counter of a specific gpt instance at any time during its 
 * counting process. 
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a 32-bit counter value that is currently stored in the counter register
 */
int readCounterVal(int gpt_num);
/**
 * @fn int readReptdCount(int gpt_num)
 * 
 * @brief The function reads the value of repeated counts.
 * 
 * @details It is used to read the value of repeated counts counted by the counter of specific 
 * GPT instance when continous count is enabled
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a 32-bit repeated count value that is currently stored in the Repeated_Count register
 */
int readReptdCount(int gpt_num);
/**
 * @fn int readCaptrdVal(int gpt_num)
 * 
 * @brief The function reads captured counter value
 * 
 * @details It is used to read the value of counter value captured when a desired input is captured
 *  at a gpio pin
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a 32-bit counter value that is captured and currently stored in Capture_register
 */
int readCaptrdVal(int gpt_num);
/**
 * @fn int readOflowintrpt(int gpt_num)
 * 
 * @brief The function reads the overflow interrupt signal generated
 * 
 * @details It is used to read the counter overflow interrupt generated once the counter reaches the threshold value.
 * The function waits till the overflow interrupt signal is generated in the control register.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a bool value of True when the counter overflow interrupt is generated in Control_register
 */
int readOflowintrpt(int gpt_num);
/**
 * @fn int readUflowintrpt(int gpt_num)
 * 
 * @brief The function reads the underflow interrupt signal generated
 * 
 * @details It is used to read the counter underflow interrupt generated once the counter reaches the threshold value.
 * The function waits till the underflow interrupt signal is generated in the control register.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a bool value of True when the counter underflow interrupt is generated in Control_register
 */
int readUflowintrpt(int gpt_num);
/**
 * @fn readFallintrpt(int gpt_num)
 * 
 * @brief The function reads the PWM Falling Edge interrupt signal generated
 * 
 * @details It is used to read the Falling Edge interrupt signal generated for every Falling Edge of 
 * the generated PWM pulse.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a bool value of True when the counter underflow interrupt is generated in Control_register
 */
int readFallintrpt(int gpt_num);
/**
 * @fn readRiseintrpt(int gpt_num)
 * 
 * @brief The function reads the PWM Rising Edge interrupt signal generated
 * 
 * @details It is used to read the Rising Edge interrupt signal generated for every Rising Edge of 
 * the generated PWM pulse.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a bool value of True when the counter underflow interrupt is generated in Control_register
 */
int readRiseintrpt(int gpt_num);
/**
 * @fn void resetGptimer(gpt_num)
 * 
 * @brief The function resets the specific GPT instance
 * 
 * @details It is used to clear all the interrupts and counter values and resets the mode of the specific
 * GPT instance.
 * 
 * @param gpt_num The parameter \a gpt_num is an integer that represents the GPT instance number.
 * 
 * @return a bool value of True when the counter underflow interrupt is generated in Control_register
 */
void resetGptimer(gpt_num);

//EOF (End Of File)
