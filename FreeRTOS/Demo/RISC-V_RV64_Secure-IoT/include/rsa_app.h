/**************************************************************************
 * Project                      : Mindgrove Silicon
 * Name of the file             : rsa_app.h
 * Brief Description of file    : Contains the header files for rsa application.
 * Name of Author               : Siddhanth Ramani
 * Email ID                     : siddhanth@mindgrovetech.in

 Copyright (C) 2021 Mindgrovetech Pvt Ltd. All rights reserved.
 ***************************************************************************/
/**
  @file rsa_app.h
  @brief Contains the header files for rsa application.
*/

// Includes
#include <stdio.h>
#include <stdbool.h>


// Padding mode structure
// struct_rsa_padding rsa_padding : The struct contains three variables :
// int padding_mode : the padding which should be done : Choose one of 0 (RSA_NULL_PAD), 1(RSAES_PKCS1_v1_5_PAD), 2 (RSAES_OAEP_PAD)
// unsigned char * label : a random label string  
// __uint64_t label_length : length of label string
typedef struct{
    int padding_mode;
    unsigned char * label;    
    __uint64_t label_length;
} struct_rsa_padding;


/** @fn void run_rsa_2048(__uint64_t *rsa_output
                              , unsigned char *input_text, unsigned char *exp_text, unsigned char *mod_text
                              , long int input_len_bits, long int exp_len_bits, long int mod_len_bits
                              , bool encrypt
                              , struct_rsa_padding rsa_padding)
 * @brief The main function which runs the rsa algorithm on H/W
 * @details This function does a couple of things. 
 * 1. It first creates pointers to rsa registers.
 * 2. It then does some checks to ensure the data is as per required.
 * 3. It then passed on the user inputs to the RSA registers and waits for an output to be created.
 * 4. Once the output is created, the output is decoded as per required and the final output 
 * is the one that will be accessed by the users.
 * @param , __uint64_t *rsa_output : pointer to where the RSA output has to be stored
 * @param unsigned char *input_text : pointer to the message that has to be encoded
 * @param unsigned char *exp_text : pointer to the key text
 * @param unsigned char *mod_text : pointer to the mod text
 * @param long int input_len_bits : length of input message in bits
 * @param long int exp_len_bits : length of exp in bits
 * @param long int mod_len_bits : length of mod in bits
 * @param bool encrypt : =1 -> encrypt mode (rsa_encrypt_mode), =0 -> decrypt mode (rsa_decrypt_mode)
 * @param struct_rsa_padding rsa_padding : The struct contains three variables :
 * int padding_mode : the padding which should be done : Choose one of 0 (RSA_NULL_PAD), 1(RSAES_PKCS1_v1_5_PAD), 2 (RSAES_OAEP_PAD)
 * unsigned char * label : a random label string  
 * __uint64_t label_length : length of label string
 * @return Returns nothing. Changes rsa_output in place.
 */
void run_rsa_2048(__uint64_t *rsa_output
                              , unsigned char *input_text, unsigned char *exp_text, unsigned char *mod_text
                              , long int input_len_bits, long int exp_len_bits, long int mod_len_bits
                              , bool encrypt
                              , struct_rsa_padding rsa_padding);
