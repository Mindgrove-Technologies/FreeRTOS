/**************************************************************************
 * Project                      : Mindgrove Silicon
 * Name of the file             : sha256_crypto.h
 * Brief Description of file    : Contains the header files for sha256 application.
 * Name of Author               : Siddhanth Ramani
 * Email ID                     : siddhanth@mindgrovetech.in

 Copyright (C) 2021 Mindgrovetech Pvt. Ltd. All rights reserved.
 ***************************************************************************/
/**
  @file sha256_driver_crypto.h
  @brief Contains the header files for sha256 application.
*/


// Functions

/** @fn void run_ascii_sha256_singleshotram(unsigned char *sha_output, unsigned char *input_text, long int input_len_bits)
 * @brief The main function which runs the SHA algorithm on H/W
 * @details This function does a couple of things. 
 * 1. It first creates pointers to SHA registers.
 * 2. It then sets them up as required and ensures SHA is ready.
 * 3. It then calculates some lengths that are required and the append bits for the last block.
 * 4. It then iterates across blocks and calls the required functions which will run SHA. While 
 * doing this, it ensures that the SHA output of previous block is passed on as the pre-hash
 * for the next block.
 * 5. It then gets the output and resets previously set configurations.
 * 6. The final step is returning a pointer to an array of 256 bits (the hash).
 * @param unsigned char *sha_output : pointer to where the SHA256 output has to be stored
 * @param unsigned char *input_text : pointer to the message that has to be encoded
 * @param long int input_len_bits : length of input message in bits
 * @return Returns nothing. Changes sha_output in place.
 */
void run_ascii_sha256_singleshotram(unsigned char *sha_output, unsigned char *input_text, long int input_len_bits);


/** @fn long int run_ascii_sha256_multishotram(unsigned char *sha_output 
                                        , unsigned char *input_text, long int input_len_bits
                                        , long int total_length
                                        , long int iterated_length_bits)
 * @brief The main function which runs the SHA algorithm on H/W by accessing RAM multiple times.
 * @details This function provides the same functionality as singleshotram but with the 
 * following differences : 
 * 1. It needs to be called multiple times by the user - one each after inputing text to 
 * a specific location.
 * 2. The sha_output is a pointer where the final output will be stored - shared by the user.
 * 3. The function returns an iterated_length_bits, which adds the current input_len_bits and
 * the previous iterated_length_bits shared by the user. This will help us keep track of whether
 * we are at the first block of run, or in the middle of the run, or in the last block of run 
 * (by comparing iterated_length_bits with the total_length_of_bits) at which point
 * we will need to return the output.
 * @param unsigned char *sha_output : pointer to where the SHA256 output has to be stored
 * @param unsigned char *input_text : pointer to the message that has to be encoded
 * @param long int input_len_bits : length of input message in bits
 * @param long int total_length : length of complete input message in bits
 * @param long int iterated_length_bits : number of bits in message which have already been processed by the SHA h/w
 * This parameter value should be the output from the previous function call. If calling this function for the first time, the value should be 0.
 * @return Returns the number of bits which have been processed by SHA hardware.
 */
int run_ascii_sha256_multishotram(unsigned char *sha_output 
                                        , unsigned char *input_text, long int input_len_bits
                                        , long int total_length
                                        , long int iterated_length_bits);
