 /**
 * SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (c) 2021-2026 Mindgrove Technologies. All rights reserved.
 * 
 * @license Licensed under the Apache License, Version 2.0 (see LICENSE).
 * @licenseblock
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * @endlicenseblock
 * 
 * Project                   : Secure IoT SoC
 * @file crypto_config.h
 * @brief Header file for all crypto configs in Mindgrove Silicon Secure-IoT SoC.
 * @details This is the header file for the crypto configs used in  PSA Crypto 
 *          APIs in Secure-IOT Device. 
 * @version 1.0
 * @authors Jennifer Vinita J (jennifer@mindgrovetech.in) 
 * @date 12-08-2025
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 12-08-2025 | 1.0     | Jennifer Vinita J     | Initial release.  
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_CRYPTO_CONFIG_H_
#define BSP_INCLUDE_CRYPTO_CONFIG_H_

/**
 * @defgroup PSA_CRYPTO_CONFIG_MACROS PSA Crypto Configuration Macros
 *
 * @brief Configuration macros for enabling PSA Crypto features, storage,
 *        algorithms, and cipher modes.
 *
 * @{
 */

/**
 * @brief Dynamically resize the PSA key store to accommodate any number
 *        of volatile keys (until heap memory is exhausted).
 *
 * @note If disabled, the key store has fixed size #MG_PSA_KEY_SLOT_COUNT.
 */
#define MG_PSA_KEY_STORE_DYNAMIC

/** @brief Enable CTR mode in PSA Crypto. */
#define MG_PSA_ALG_CTR
/** @brief Enable CFB mode in PSA Crypto. */
#define MG_PSA_ALG_CFB
/** @brief Enable OFB mode in PSA Crypto. */
#define MG_PSA_ALG_OFB
/** @brief Enable ECB mode with no padding in PSA Crypto. */
#define MG_PSA_ALG_ECB_NO_PADDING
/** @brief Enable CBC mode with no padding in PSA Crypto. */
#define MG_PSA_ALG_CBC_NO_PADDING

/**
 * @brief Auto-enable CIPHER_C when any of the unauthenticated ciphers is 
 *        enabled in PSA.
 */
#if defined(MG_PSA_CRYPTO_C) && \
    (defined(MG_PSA_ALG_CTR) || \
     defined(MG_PSA_ALG_CFB) || \
     defined(MG_PSA_ALG_OFB) || \
     defined(MG_PSA_ALG_ECB_NO_PADDING) || \
     defined(MG_PSA_ALG_CBC_NO_PADDING))
#define MG_CIPHER_C
#endif

/** @brief Enable AES cipher. */
#define MG_AES_C
/** @brief Enable CBC cipher mode. */
#define MG_CIPHER_MODE_CBC
/** @brief Enable CFB cipher mode. */
#define MG_CIPHER_MODE_CFB
/** @brief Enable OFB cipher mode. */
#define MG_CIPHER_MODE_OFB
/** @brief Enable CTR cipher mode. */
#define MG_CIPHER_MODE_CTR

/** @} */

#endif  // BSP_INCLUDE_CRYPTO_CONFIG_H_
