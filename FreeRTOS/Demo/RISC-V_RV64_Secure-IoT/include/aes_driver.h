/**************************************************************************
 * Project                      : Mindgrove Silicon
 * Name of the file             : aes_driver.h
 * Brief Description of file    : Contains the header files for aes256 driver.
 * Name of Author               : Siddhanth Ramani
 * Email ID                     : siddhanth@mindgrovetech.in

 Copyright (C) 2021 Mindgrovetech Pvt Ltd. All rights reserved.
 ***************************************************************************/
/**
  @file aes_driver.h
  @brief Contains the header files for aes256 driver.
*/

#ifndef AES_DRIVER_INCLUDED
#define AES_DRIVER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include <stdio.h>


void input_text_to_aes(__uint64_t *input_reg_64, unsigned char *input_text);


void input_key_to_aes(__uint64_t *aes_key_reg_64, unsigned char *key, int hex_key_len);


void input_iv_to_aes(__uint64_t *aes_iv_reg_64, unsigned char *iv);


void do_checks_singleshotram(int input_len_bits, int key_len_bits, int mode);


void do_checks_multishotram(int key_len_bits, int mode);


unsigned char * get_output_singleshotram(unsigned char * aes_output, volatile __uint64_t * aes_output_reg_64);


unsigned char * get_output_multishotram(unsigned char * aes_output, volatile __uint64_t * aes_output_reg_64);


void run_ascii_aes_singleshotram(unsigned char *aes_output
                                            , unsigned char *input_text, unsigned char *key, unsigned char *iv
                                            , long int input_len_bits , long int key_len_bits
                                            , int mode, int encrypt_or_decrypt);


int run_ascii_aes_multishotram(unsigned char *aes_output
                                , unsigned char *input_text
                                , unsigned char *key
                                , unsigned char *iv 
                                , long int key_len_bits 
                                , int mode
                                , int encrypt_or_decrypt
                                , int iterated_length_bits);

#ifdef __cplusplus
}
#endif

#endif
