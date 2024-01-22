#ifndef CRYPTO_HELPERS_INCLUDED
#define CRYPTO_HELPERS_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// Check if calloc memory has been allocated
void check_calloc(unsigned char * ptr);


// Getting a non zero 8 bit random number
/** @fn __uint8_t get_non_zero_rand_8()
 * @brief Creates an 8 bit random non zero number
 * @return Returns an 8 bit random non zero number
 */
__uint8_t get_non_zero_rand_8();


// Getting a non zero 64 bit random number
/** @fn __uint64_t get_non_zero_rand_64()
 * @brief Creates a 64 bit random non zero number
 * @return Returns a 64 bit random non zero number
 */
__uint64_t get_non_zero_rand_64();


// Reverse a 64 bit register bytewise
/** @fn __uint64_t reverse_64bit(__uint64_t input_number)
 * @brief Reverses a 64 bit number. Ie it converts a 64 bit register value from litte ending to big endian or vice versa
 * @param __uint64_t input_number : number which has to be byte reversed
 * @return Returns the reversed 64 bit number
 */
__uint64_t reverse_64bit(__uint64_t input_number);


// Prints string given length with space after every 8 bytes
void test_print(short int strlength, unsigned char * aes_output);


// Prints string given length in bits
void print_test_string(unsigned char *p_to_string, int length_in_bits);


// Prints string given length in dword (64bits)
/** @fn void print_uchar64(unsigned char *printname, unsigned char *printarg, int len_bytes, bool mode)
 * @brief Helper function to print characters (with 0 prepend), one byte at a time
 * @param unsigned char *printname : comments to print before printing actual arg
 * @param unsigned char *printarg : argument to print char by char
 * @param int len_dword : length of arg in dword - i.e if length in bits is 64, length of dword is 1 (bits length/64 = dword length)
 * @param bool mode : =0 -> little endian(print_little_endian), =1 -> big endian(print_big_endian)
 * @return Does not return anything.
 */
void print_uchar64(unsigned char *printname, unsigned char *printarg, int len_dword, bool mode);


// Prints string given length in bytes
/** @fn void print_uchar8(unsigned char *printname, unsigned char *printarg, int len_bytes, bool mode)
 * @brief Helper function to print characters (with 0 prepend), one byte at a time
 * @param unsigned char *printname : comments to print before printing actual arg
 * @param unsigned char *printarg : argument to print char by char
 * @param int len_bytes : length of arg in bytes
 * @param bool mode : =0 -> little endian(print_little_endian), =1 -> big endian(print_big_endian)
 * @return Does not return anything.
 */
void print_uchar8(unsigned char *printname, unsigned char *printarg, int len_bytes, bool mode);


// Converts ascii hex to binary hex
/** @fn void convert_asciihex_to_binaryhex(unsigned char *ascii_hex_pointer, unsigned char *binary_hex_pointer)
 * @brief Helper function to which converts hexadecimal stored in ascii format to binary format.
 * @param unsigned char *ascii_hex_pointer
 * @param unsigned char *binary_hex_pointer
 * @return Does not return anything. Changes binary_hex_pointer in place.
 */
void convert_asciihex_to_binaryhex(unsigned char *ascii_hex_pointer, unsigned char *binary_hex_pointer);


#endif