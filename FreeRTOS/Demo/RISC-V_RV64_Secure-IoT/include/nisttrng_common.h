/*
 * Copyright (c) 2012-2016 Synopsys, Inc.
 * This Synopsys software and associated documentation (hereinafter the
 * "Software") is an unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you. The
 * Software IS NOT an item of Licensed Software or a Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Products
 * with Synopsys or any supplement thereto. Synopsys is a registered trademark
 * of Synopsys, Inc. Other names included in the SOFTWARE may be the
 * trademarks of their respective owners.
 *
 * The contents of this file are dual-licensed; you may select either version
 * 2 of the GNU General Public License ("GPL") or the BSD-3-Clause license
 * ("BSD-3-Clause"). The GPL is included in the COPYING file accompanying the
 * SOFTWARE. The BSD License is copied below.
 *
 * BSD-3-Clause License:
 * Copyright (c) 2012-2016 Synopsys, Inc. and/or its affiliates.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer, without
 *    modification.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The names of the above-listed copyright holders may not be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BSP_INCLUDE_NISTTRNG_COMMON_H_
#define BSP_INCLUDE_NISTTRNG_COMMON_H_

#include <stdint.h>
#include "log.h"

/**< (1<<19) Default maximum bits per TRNG request */
#define NIST_DFLT_MAX_BITS_PER_REQ  524288U

#define CRYPTO_OK (0)                  /**< Operation successful */
#define CRYPTO_FATAL (-105)             /**< Fatal error occurred */
#define CRYPTO_NOT_INSTANTIATED (-114)  /**< TRNG not instantiated error */


/**
 * @enum nisttrng_sec_strength
 * @brief Security strength options for TRNG (AES128 or AES256)
 */
typedef enum nisttrng_sec_strength {
    SEC_STRNT_AES128 = 0U, /**< AES-128 security strength */
    SEC_STRNT_AES256 = 1U  /**< AES-256 security strength */
} nisttrng_sec_strength;


/**
 * @enum nisttrng_current_state
 * @brief Current state of the TRNG state machine
 */
typedef enum nisttrng_current_state {
    NIST_TRNG_STATE_INITIALIZE = 0U,   /**< Initial state */
    NIST_TRNG_STATE_UNINSTANTIATE,     /**< Uninstantiate state */
    NIST_TRNG_STATE_INSTANTIATE,       /**< Instantiate state */
    NIST_TRNG_STATE_RESEED,            /**< Reseed state */
    NIST_TRNG_STATE_GENERATE           /**< Generate state */
} nisttrng_current_state;


/**
 * @struct nist_trng_state
 * @brief Structure holding the state, status, and counters for the TRNG.
 */
typedef struct {
    /**
     * @struct status
     * @brief Status information for the TRNG
     */
    struct {
        nisttrng_current_state current_state; /**< Current TRNG state */
        unsigned int secure_mode : 1;         /**< Secure mode enabled flag */
        nisttrng_sec_strength sec_strength;   /**< Security strength in use */
        volatile uint32_t alarm_code;         /**< Alarm code (if any) */
        unsigned int rnc_enabled : 1;         /**< RNC enabled flag */
    } status;

    /**
     * @struct counters
     * @brief Counters and reminders for TRNG usage
     */
    struct {
        uint32_t max_bits_per_req;     /**< Maximum bits allowed per request */
        uint64_t max_req_per_seed;     /**< Maximum requests allowed per seed */
        uint32_t bits_per_req_left;    /**< Bits left for current request */
        uint64_t req_per_seed_left;    /**< Requests left for current seed */
    } counters;
} nist_trng_state;


/**
 * @fn int nisttrng_full_kat(nist_trng_state* state)
 * @brief Perform a full Known Answer Test (KAT) on the TRNG.
 * @param state Pointer to the TRNG state structure.
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_full_kat(nist_trng_state* state);


/**
 * @fn int nisttrng_init(nist_trng_state* state)
 * @brief Initialize the TRNG state structure.
 * @param state Pointer to the TRNG state structure.
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_init(nist_trng_state* state);


/**
 * @fn int nisttrng_instantiate(nist_trng_state* state, uint32_t req_sec_strength)
 * @brief Instantiate the TRNG with the required security strength.
 * @param state Pointer to the TRNG state structure.
 * @param req_sec_strength Required security strength (see nisttrng_sec_strength).
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_instantiate(nist_trng_state* state, uint32_t req_sec_strength);


/**
 * @fn int nisttrng_uninstantiate(nist_trng_state* state)
 * @brief Uninstantiate (zeroize) the TRNG state.
 * @param state Pointer to the TRNG state structure.
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_uninstantiate(nist_trng_state* state);


/**
 * @fn int nisttrng_reseed(nist_trng_state* state)
 * @brief Reseed the TRNG.
 * @param state Pointer to the TRNG state structure.
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_reseed(nist_trng_state* state);


/**
 * @fn int nisttrng_generate(nist_trng_state* state, void* random_bits, uint32_t req_num_bytes)
 * @brief Generate random bits using the TRNG.
 * @param state Pointer to the TRNG state structure.
 * @param random_bits Pointer to the output buffer for random data.
 * @param req_num_bytes Number of random bytes to generate.
 * @return 0 on success, negative error code on failure.
 */
int nisttrng_generate(nist_trng_state* state, void* random_bits,
                      uint32_t req_num_bytes);


#endif  // BSP_INCLUDE_NISTTRNG_COMMON_H_
