/*
 * Project                           : Secure IoT SoC
 * Name of the file                  : rsa.h
 * Brief Description of file         : Header to Standard RSA Driver
 * Name of Author                    : Jennifer Vinita  
 * Email ID                          : jennifer@mindgrovetech.in 
 * 
 * @file rsa.h
 * @author Jennifer Vinita J (jennifer@mindgrovetech.in)
 * @brief This is a Baremetal RSA Driver's Header file for Mindgrove Silicon's RSA Peripheral
 * @version 0.2
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 * 
 */

#ifndef RSA_DRIVER_INCLUDED
#define RSA_DRIVER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include "io.h"
// #include <tommath.h>

// Padding mode structure
typedef struct{
    int padding_mode;
    unsigned char * label;    
    __uint64_t label_length;
} struct_rsa_padding;


int RSA_Run(uint8_t *output,uint8_t *input,uint8_t *exp,uint8_t *mod,uint8_t* r2modn);

#ifdef __cplusplus
}
#endif

#endif