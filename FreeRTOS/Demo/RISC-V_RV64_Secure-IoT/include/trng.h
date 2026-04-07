/*
 * Project                           : Secure IoT SoC
 * Name of the file                  : trng.h
 * Brief Description of file         : Header to Standard TRNG Driver
 * Name of Author                    : Jennifer Vinita, Narashima  
 * Email ID                          : <jennifer@mindgrovetech.in> <narashima@mindgrovetech.in>
 * 
 * @file trng.h
 * @author Jennifer Vinita J (jennifer@mindgrovetech.in)
 * @author Narashima (narashima@mindgrovetech.in)
 * 
 * @brief This is a Baremetal TRNG Driver's Header file for Mindgrove Silicon's TRNG Peripheral
 * @version 0.2
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) Mindgrove Technologies Pvt. Ltd 2025. All rights reserved.
 * 
 */

#ifndef BSP_INCLUDE_TRNG_H_
#define BSP_INCLUDE_TRNG_H_

#include "nisttrng_common.h"


/**
 * @fn TRNG_init(nist_trng_state *state, uint16_t req_sec_strength)
 * @brief Initializes the TRNG with the specified security strength.
 * @param state Pointer to the TRNG state structure.
 * @param req_sec_strength Required security strength for initialization.
 * @return 0 on success, or a negative error code on failure.
 */
int TRNG_init(nist_trng_state *state, uint16_t req_sec_strength);

/**
 * @fn TRNG_Generate(void *output, int req_sec_strength, int no_of_bytes)
 * @details The function TRNG_Generate generates random numbers using a True
 * Random Number Generator (TRNG) based on NIST standards.
 * @param output is a pointer to the memory location where the generated random
 * bytes will be stored.
 * @param req_sec_strength represents the required security strength for the
 * random number generation process. It is used to specify the level of security
 * that the generated random numbers should possess.
 * @param no_of_bytes represents the number of bytes of random data that you
 * want to generate using the True Random Number Generator (TRNG).
 * @return an integer value. If the function executes successfully, it will
 * return 0. If there are any errors during the TRNG generation process, it will
 * return the corresponding error code.
 */
int TRNG_Generate(nist_trng_state *state, void* output, uint32_t no_of_bytes);


/**
 * @fn TRNG_Uninstantiate(nist_trng_state *state)
 * @brief Uninstantiates (zeroizes) the TRNG state.
 * @param state Pointer to the TRNG state structure.
 * @return 0 on success, or a negative error code on failure.
 */
int TRNG_Uninstantiate(nist_trng_state *state);


/**
 * @fn Rand()
 * @brief Returns a single random byte using the TRNG.
 * @return A random 8-bit unsigned integer.
 */
uint8_t Rand(void);

/**
 * @fn full_kat_test()
 * @details Performs a full KAT with all four combinations of the kat_sel and
 * kat_vec If any of the KAT fails, the API returns error.
 * @return If the function executes successfully, it will return 0. If there are
 * any errors during the TRNG generation process, it will return the
 * corresponding error code.
 */
int full_kat_test(void);

#endif  // BSP_INCLUDE_TRNG_H_

