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

#ifndef BSP_INCLUDE_NISTTRNG_H_
#define BSP_INCLUDE_NISTTRNG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Do not change the following parameters */
/**< TRNG random block size in bits */
#define NIST_TRNG_RAND_BLK_SIZE_BITS 128U
/**< Maximum number of retries for TRNG operations */
#define NIST_TRNG_RETRY_MAX 5000000U

/*
 * Reduced the maximum number of requests per seed for NIST TRNG from 1<<49 
 * to 1<<5 (32).
 * Reason: We do not want to allow such a large number of requests using a
 * single seed.
 */
#define NIST_DFLT_MAX_REQ_PER_SEED 32U

#define NIST_TRNG_DFLT_MAX_REJECTS 10          /**< Default max rejects */

#define CRYPTO_FAILED   (-1)    /**< Operation did not succeed */
#define CRYPTO_TIMEOUT  (-21)   /**< Timeout: operation took too long */

#define DRBG_INSTANTIATED(cs)                                                  \
  ((((cs) == NIST_TRNG_STATE_INSTANTIATE) ||                                   \
    ((cs) == NIST_TRNG_STATE_RESEED) || ((cs) == NIST_TRNG_STATE_GENERATE))    \
       ? 1U                                                                    \
       : 0U) /**< Macro to check if DRBG is instantiated */

/**< Get channel rate after reset */
#define NISTTRNG_EDU_RBC_CTRL_GET_CH_RATE_AFTER_RESET 0x0
/**< Set channel underrun blank after reset */
#define NISTTRNG_EDU_RBC_CTRL_SET_CH_URUN_BLANK_AFTER_RESET 0x3
/**< Command to get random data */
#define NIST_TRNG_EDU_VTRNG_VCTRL_CMD_GET_RANDOM 0x1U

/* CTRL */
#define NIST_TRNG_REG_CTRL_CMD_GEN_NOISE 1U     // Command to generate noise
#define NIST_TRNG_REG_CTRL_CMD_CREATE_STATE 3U  // Command to create state
#define NIST_TRNG_REG_CTRL_CMD_RENEW_STATE 4U   // Command to renew state
#define NIST_TRNG_REG_CTRL_CMD_KAT 8U           // Command for Known Answer Test
#define NIST_TRNG_REG_CTRL_CMD_ZEROIZE 15U      // Command to zeroize state

/* EDU CTRL */
#define NIST_TRNG_EDU_RNC_CTRL_CMD_RNC_DISABLE_TO_HOLD 0  // Disable RNC to hold
#define NIST_TRNG_EDU_RNC_CTRL_CMD_RNC_ENABLE 1           // Enable RNC
#define NIST_TRNG_EDU_RNC_CTRL_CMD_RNC_FINISH_TO_IDLE 3   // Finish RNC to idle

/*EDU_RBC_CTRL*/
#define NIST_TRNG_EDU_RBC_CTRL_CH0_URUN_BLANK 24    // Channel 0 underrun blank
#define NIST_TRNG_EDU_RBC_CTRL_CH0_RATE 0           // Channel 0 rate

// Set channel rate
#define NISTTRNG_EDU_RBC_CTRL_SET_CH_RATE(z, y, x) \
  (((y) & ~(UINT32_C(0x1) << (x))) | (UINT32_C(z) << (x)))
// Set channel underrun blank
#define NISTTRNG_EDU_RBC_CTRL_SET_CH_URUN_BLANK(z, y, x) \
  (((y) & ~(UINT32_C(0x3) << (x))) | (UINT32_C(z) << (x)))

/* ALARMS */
#define NIST_TRNG_REG_ALARM_FAILED_TEST_ID_OK 0U    // Alarm:failed test ID OK

/* ISTAT */
/* ISTAT */
#define NIST_TRNG_REG_ISTAT_DONE      (1UL << 4)  // Done interrupt
#define NIST_TRNG_REG_ISTAT_ALARMS    (1UL << 3)  // Alarms interrupt
#define NIST_TRNG_REG_ISTAT_KAT_COMPLETE (1UL << 1)  // KAT complete interrupt
#define NIST_TRNG_REG_ISTAT_ZEROIZE   (1UL << 0)  // Zeroize interrupt

/* STAT */
#define NIST_TRNG_REG_STAT_BUSY (1UL << 31)        /**< Status: busy */

/*EDU_STAT*/
// RNC enabled
#define NIST_TRNG_EDU_STAT_RNC_ENABLED(x) (((x) >> UINT32_C(2)) & UINT32_C(1))
// Set max rejects
#define NIST_TRNG_REG_SMODE_SET_MAX_REJECTS(y, x) \
  (((y) & ~(UINT32_C(255) << UINT32_C(2))) | ((x) << UINT32_C(2)))
// Set secure enable
#define NIST_TRNG_REG_SMODE_SET_SECURE_EN(y, x) \
  (((y) & ~(UINT32_C(1) << UINT32_C(1))) | ((x) << UINT32_C(1)))
// Set nonce
#define NIST_TRNG_REG_SMODE_SET_NONCE(y, x) \
  (((y) & ~(UINT32_C(1) << UINT32_C(0))) | ((x) << UINT32_C(0)))
// Set secure algorithm
#define NIST_TRNG_REG_MODE_SET_SEC_ALG(y, x) \
  (((y) & ~(UINT32_C(1) << UINT32_C(0))) | ((x) << UINT32_C(0)))
// Set prediction resistance
#define NIST_TRNG_REG_MODE_SET_PRED_RESIST(y, x) \
  (((y) & ~(UINT32_C(1) << UINT32_C(3))) | ((x) << UINT32_C(3)))
// Set KAT selection
#define NIST_TRNG_REG_MODE_SET_KAT_SEL(y, x) \
  (((y) & ~(UINT32_C(3) << UINT32_C(7))) | ((x) << UINT32_C(7)))
// Set KAT vector
#define NIST_TRNG_REG_MODE_SET_KAT_VEC(y, x) \
  (((y) & ~(UINT32_C(3) << UINT32_C(5))) | ((x) << UINT32_C(5)))
// Get DRBG state
#define NIST_TRNG_REG_STAT_GET_DRBG_STATE(x) (((x) >> 7) & 3UL)

/* EDU_VSTAT */
/**< Check if EDU is busy */
#define NIST_TRNG_REG_EDU_VSTAT_BUSY(x) (((x) >> UINT32_C(31)) & UINT32_C(1))

/**< Check any RW1 in EDU VSTAT */
#define NIST_TRNG_REG_EDU_VSTAT_ANY_RW1(x) \
  (((x) >> UINT32_C(24)) & UINT32_C(1))

/**< Check slice valid 3 in EDU VSTAT */
#define NIST_TRNG_REG_EDU_VSTAT_SLICE_VLD3(x) \
  (((x) >> UINT32_C(11)) & UINT32_C(1))

/**< Check slice valid 2 in EDU VSTAT */
#define NIST_TRNG_REG_EDU_VSTAT_SLICE_VLD2(x) \
  (((x) >> UINT32_C(10)) & UINT32_C(1))

/**< Check slice valid 1 in EDU VSTAT */
#define NIST_TRNG_REG_EDU_VSTAT_SLICE_VLD1(x) \
  (((x) >> UINT32_C(9)) & UINT32_C(1))

/**< Check slice valid 0 in EDU VSTAT */
#define NIST_TRNG_REG_EDU_VSTAT_SLICE_VLD0(x) \
  (((x) >> UINT32_C(8)) & UINT32_C(1))

/*These error codes are not used.. Its For future reference*/
#define CRYPTO_INPROGRESS (-2)               /**< Operation in progress */
#define CRYPTO_INVALID_HANDLE (-3)           /**< Invalid handle */
#define CRYPTO_INVALID_CONTEXT (-4)          /**< Invalid context */
#define CRYPTO_INVALID_SIZE (-5)             /**< Invalid size */
#define CRYPTO_NOT_INITIALIZED (-6)          /**< Not initialized */
#define CRYPTO_NO_MEM (-7)                   /**< No memory (future use) */
#define CRYPTO_INVALID_ALG (-8)              /**< Invalid algorithm */
#define CRYPTO_INVALID_KEY_SIZE (-9)         /**< Invalid key size */
#define CRYPTO_INVALID_ARGUMENT (-10)        /**< Invalid argument */
#define CRYPTO_MODULE_DISABLED (-11)         /**< Module disabled */
#define CRYPTO_NOT_IMPLEMENTED (-12)         /**< Not implemented */
#define CRYPTO_INVALID_BLOCK_ALIGNMENT (-13) /**< Invalid block alignment */
#define CRYPTO_INVALID_MODE (-14)            /**< Invalid mode */
#define CRYPTO_INVALID_KEY (-15)             /**< Invalid key */
#define CRYPTO_AUTHENTICATION_FAILED (-16)   /**< Authentication failed */
#define CRYPTO_INVALID_IV_SIZE (-17)         /**< Invalid IV size */
#define CRYPTO_MEMORY_ERROR (-18)            /**< Memory error */
#define CRYPTO_LAST_ERROR (-19)              /**< Last error */
#define CRYPTO_HALTED (-20)                  /**< Halted */
#define CRYPTO_SRM_FAILED (-22)              /**< SRM failed */
#define CRYPTO_COMMON_ERROR_MAX (-100)       /**< Common error max */
#define CRYPTO_INVALID_ICV_KEY_SIZE (-100)   /**< Invalid ICV key size */
#define CRYPTO_INVALID_PARAMETER_SIZE (-101) /**< Invalid parameter size */
#define CRYPTO_SEQUENCE_OVERFLOW (-102)      /**< Sequence overflow */
#define CRYPTO_DISABLED (-103)               /**< Disabled */
#define CRYPTO_INVALID_VERSION (-104)        /**< Invalid version */
#define CRYPTO_INVALID_PAD (-106)            /**< Invalid pad */
#define CRYPTO_FIFO_FULL (-107)              /**< FIFO full */
#define CRYPTO_INVALID_SEQUENCE (-108)       /**< Invalid sequence */
#define CRYPTO_INVALID_FIRMWARE (-109)       /**< Invalid firmware */
#define CRYPTO_NOT_FOUND (-110)              /**< Not found */
#define CRYPTO_CMD_FIFO_INACTIVE (-111)      /**< Command FIFO inactive */
#define CRYPTO_INVALID_PROTOCOL (-112)       /**< Invalid protocol */
#define CRYPTO_REPLAY (-113)                 /**< Replay */
#define CRYPTO_RESEED_REQUIRED (-115)        /**< Reseed required */

#ifdef __cplusplus
}
#endif

#endif  // BSP_INCLUDE_NISTTRNG_H_
