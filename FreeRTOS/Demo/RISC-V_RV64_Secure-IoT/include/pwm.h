/**
 * Project                               : Secure IoT SoC
 * Name of the file                      : pwm.h
 * Brief Description of file             : This is a Baremetal PWM Driver header file for Mindgrove Silicon's PWM Peripheral.
 * Name of Author                        : Harini Sree.S
 * Email ID                              : <harini@mindgrovetech.in>
 */
/**
 * @file pwm.h
 * @brief Header file for pwm
 * @details this is the header file for PWM 
 * @date 2025-04-24
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2024. All rights reserved.  
 */


#ifdef __cplusplus
extern "C" {
#endif

#include<stdbool.h>
#include <stdint.h>

#define PWM_PIN(x)  (1 << x)

typedef enum
{
  rise_interrupt,       //Enable interrupt only on rise
  fall_interrupt,       //Enable interrupt only on fall
  halfperiod_interrupt, //Enable interrupt only on halfperiod
  no_interrupt          //Disable interrupts
}pwm_interrupt_modes;

typedef struct 
{
  uint32_t duty;        
  uint32_t period;
  pwm_interrupt_modes interrupt_mode;
  bool change_output_polarity;
  uint16_t prescalar_value;
  uint32_t deadband_delay;
}PWM_Config_t;

/** @fn pwm_start
 * @brief Function to start a specific pwm module
 * @details This function will start the specific pwm module
* @param[in] uint32_t config - Structure to set the parameters of the PWM signal.
 * @param[in] uint32_t pwm_pins - The bitwise OR'ed value of PWM pin identifiers representing the active PWM channels. 
 * @param[Out] uint16_t Return value
 * @return Flag
 */
uint16_t PWM_Start(PWM_Config_t *config, uint32_t pwm_pins);

/** @fn  pwm_stop
 * @brief Function to stop a specific pwm module
 * @details This function will stop a specific pwm module
 * @param[in] uint32_t pwm_pins - The bitwise OR'ed value of PWM pin identifiers representing the active PWM channels. 
 * @param[out] uint16_t Return value 
 */
uint16_t PWM_Stop(uint32_t pwm_pins);


#ifdef __cplusplus
}
#endif