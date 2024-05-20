/**************************************************************************
 * Project                      : Mindgrove Silicon
 * Name of the file             : rsa_driver.h
 * Brief Description of file    : Contains the header files for rsa driver.
 * Name of Author               : Siddhanth Ramani
 * Email ID                     : siddhanth@mindgrovetech.in

 Copyright (C) 2021 Mindgrovetech Pvt Ltd. All rights reserved.
 ***************************************************************************/
/**
  @file rsa_driver.h
  @brief Contains the header files for rsa driver.
*/

#ifndef RSA_DRIVER_INCLUDED
#define RSA_DRIVER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include <stdio.h>
#include <tommath.h>

// Padding mode structure
typedef struct{
    int padding_mode;
    unsigned char * label;    
    __uint64_t label_length;
} struct_rsa_padding;


mp_int get_R2modN(unsigned char * char_N, long int mod_len);


__uint8_t * I20SP(__uint32_t x, __uint8_t x_len);


void MGF1(unsigned char * mask, unsigned char * mgf_seed, long int mgf_seed_len, long int mask_len);


void create_db(unsigned char *db, unsigned char * l_hash, unsigned char * input_text, long int m_len);


void pad_rsa_input_pkcs15(__uint64_t *load_input, unsigned char *input_text, int input_len_bits, struct_rsa_padding rsa_padding);


void pad_rsa_input_oaep(__uint64_t *load_input, unsigned char *input_text
                        , int input_len_bits, struct_rsa_padding rsa_padding);


void pad_rsa_input(__uint64_t *load_input, unsigned char *input_text, int input_len_bits, struct_rsa_padding rsa_padding);


void input_to_rsa_prepend_zero(__uint64_t *load_input, unsigned char *input, int input_len);


void convert_60_to_64(__uint64_t *R2modN_64, unsigned short int mod_len_bits, int allocated);


void load_to_rsa(__uint64_t *rsa_reg_to_load, __uint64_t *load_input);


unsigned char * get_rsa_output(__uint64_t * rsa_output, volatile __uint64_t * rsa_output_reg_64);


void print_dp(mp_digit * dp);


long int get_char_length(unsigned char *char_array_pointer);


void decode_rsa_output_pkcs15(__uint64_t *rsa_output);


void decode_rsa_output_oaep(__uint64_t *rsa_output, struct_rsa_padding rsa_padding);


void decode_rsa_output(__uint64_t *rsa_output, struct_rsa_padding rsa_padding);


void testing_encoding_decoding_rev_message(__uint64_t * rsa_output, __uint64_t * load_input);


void do_checks_rsa(unsigned char *mod_text
                    , long int input_len_bits, long int exp_len_bits, long int mod_len_bits
                    , struct_rsa_padding rsa_padding);


void run_rsa_2048(__uint64_t *rsa_output
                              , unsigned char *input_text, unsigned char *exp_text, unsigned char *mod_text
                              , long int input_len_bits, long int exp_len_bits, long int mod_len_bits
                              , bool encrypt
                              , struct_rsa_padding rsa_padding);


#ifdef __cplusplus
}
#endif

#endif
