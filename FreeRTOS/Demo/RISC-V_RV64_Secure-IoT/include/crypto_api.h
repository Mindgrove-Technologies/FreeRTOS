#ifndef INCLUDES_CRYPTO_API_H_
#define INCLUDES_CRYPTO_API_H_

#include <stdint.h>

#if defined(TARGET_FLASH) && !defined(CRYPTO_DRIVER_IMPLEMENTATION)
#include "aes.h"
#include "sha256.h"
#include "rsa.h"
#include "rsa_padding.h"
#include "otp.h"
#include "trng.h"
#include "aes_gcm.h"
#include "ascon.h"
#include "cmac_aes.h"
#include "hmac.h"
#include "sha2.h"
#include "sha3.h"
#include "psa.h"
#include "crypto_helpers.h"
#include "bignum.h"
#include "ecc.h"
#include "aes_xts.h"
#endif

#define CRYPTO_GOT_OFFSET 0x21000U

/* GOT function list — add new functions here AND in crypto.ld */
#define CRYPTO_GOT_FUNCTIONS \
    X(AES_Run,                      uint16_t, (AES_Config_t *)) \
    X(AES_Zeroize,                  uint16_t, (void)) \
    X(SHA256_Single_Run,            uint16_t, (uint8_t *, const uint8_t *, size_t)) \
    X(SHA256_Multi_Run,             uint16_t, (const uint8_t *, size_t, size_t, size_t)) \
    X(SHA256_Read_Output,           uint16_t, (uint8_t *, size_t *)) \
    X(SHA256_Zeroize,               uint16_t, (void)) \
    X(RSA_Run,                      uint16_t, (RSA_Config_t *)) \
    X(RSA_Zeroize,                  uint16_t, (void)) \
    X(RSA_Generate_Key_Pair,        uint16_t, (RSA_key_pair *, const uint8_t *)) \
    X(RSAES_PKCS1_v1_5_Encrypt,     uint16_t, (uint8_t *, size_t, const uint8_t *, size_t)) \
    X(RSAES_PKCS1_v1_5_Decrypt,     uint16_t, (uint8_t *, size_t *, const uint8_t *, size_t)) \
    X(RSASSA_PKCS1_v1_5_Sign,       uint16_t, (uint8_t *, size_t, const uint8_t *, size_t)) \
    X(RSASSA_PKCS1_v1_5_Verify,     uint16_t, (const uint8_t *, size_t, const uint8_t *, size_t)) \
    X(RSAES_OAEP_Encrypt,           uint16_t, (uint8_t *, size_t, const uint8_t *, size_t, const uint8_t *, size_t)) \
    X(RSAES_OAEP_Decrypt,           uint16_t, (const uint8_t *, size_t, uint8_t *, size_t *, const uint8_t *, size_t)) \
    X(RSASSA_PSS_Sign,              uint16_t, (uint8_t *, size_t, const uint8_t *, size_t, const uint8_t *, size_t, uint8_t)) \
    X(RSASSA_PSS_Verify,            uint16_t, (const uint8_t *, size_t, const uint8_t *, size_t, size_t)) \
    X(OTP_Read,                     uint16_t, (const uintptr_t *, size_t, uint8_t *)) \
    X(OTP_Write,                    uint16_t, (const uintptr_t *, size_t, uint8_t *)) \
    X(OTP_TESTDEC_Test,             uint16_t, (const uintptr_t *, uint8_t *)) \
    X(OTP_WRTEST_Test,              uint16_t, (const uintptr_t *, uint8_t *)) \
    X(OTP_Read_CPUID,               uint16_t, (uint8_t *)) \
    X(OTP_Lock,                     uint16_t, (OTP_LOCK)) \
    X(TRNG_Generate,                uint16_t, (void *, size_t, uint8_t, uint64_t)) \
    X(TRNG_Full_KAT,                uint16_t, (void)) \
    X(AES_GCM,                      uint16_t, (AES_GCM_Config *)) \
    X(ASCON_AEAD128a,               uint16_t, (ASCON_AEAD128a_Config *)) \
    X(ASCON_Hash256,                uint16_t, (ASCON_Hash256_Config *)) \
    X(ASCON_XOF128,                 uint16_t, (ASCON_XOF128_Config *)) \
    X(ASCON_CXOF128,                uint16_t, (ASCON_CXOF128_Config *)) \
    X(CMAC_AES,                     uint16_t, (CMAC_AES_Config *)) \
    X(HMAC_Get_Context_Size,        size_t,   (void)) \
    X(HMAC_Reset,                   uint16_t, (Hmac *)) \
    X(HMAC_Set_Key,                 uint16_t, (Hmac *, HMAC_HashType_t, const uint8_t *, size_t)) \
    X(HMAC_Update,                  uint16_t, (Hmac *, const uint8_t *, size_t)) \
    X(HMAC_Final,                   uint16_t, (Hmac *, uint8_t *)) \
    X(SHA512_Init,                  uint16_t, (SHA2 *)) \
    X(SHA512_Update,                uint16_t, (SHA2 *, const uint8_t *, size_t)) \
    X(SHA512_Final,                 uint16_t, (SHA2 *, uint8_t *)) \
    X(SHA512_Run,                   uint16_t, (const uint8_t *, size_t, uint8_t *)) \
    X(SHA384_Init,                  uint16_t, (SHA2 *)) \
    X(SHA384_Update,                uint16_t, (SHA2 *, const uint8_t *, size_t)) \
    X(SHA384_Final,                 uint16_t, (SHA2 *, uint8_t *)) \
    X(SHA384_Run,                   uint16_t, (const uint8_t *, size_t, uint8_t *)) \
    X(Sha3_224_Init,                uint16_t, (Sha3 *)) \
    X(Sha3_224_Update,              uint16_t, (Sha3 *, const uint8_t *, uint32_t)) \
    X(Sha3_224_Final,               uint16_t, (Sha3 *, uint8_t *)) \
    X(Sha3_256_Init,                uint16_t, (Sha3 *)) \
    X(Sha3_256_Update,              uint16_t, (Sha3 *, const uint8_t *, uint32_t)) \
    X(Sha3_256_Final,               uint16_t, (Sha3 *, uint8_t *)) \
    X(Sha3_384_Init,                uint16_t, (Sha3 *)) \
    X(Sha3_384_Update,              uint16_t, (Sha3 *, const uint8_t *, uint32_t)) \
    X(Sha3_384_Final,               uint16_t, (Sha3 *, uint8_t *)) \
    X(Sha3_512_Init,                uint16_t, (Sha3 *)) \
    X(Sha3_512_Update,              uint16_t, (Sha3 *, const uint8_t *, uint32_t)) \
    X(Sha3_512_Final,               uint16_t, (Sha3 *, uint8_t *)) \
    X(psa_cipher_encrypt,           psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_cipher_decrypt,           psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_cipher_encrypt_setup,     psa_status_t, (psa_cipher_operation_t *, psa_key_id_t, psa_algorithm_t)) \
    X(psa_cipher_decrypt_setup,     psa_status_t, (psa_cipher_operation_t *, psa_key_id_t, psa_algorithm_t)) \
    X(psa_cipher_set_iv,            psa_status_t, (psa_cipher_operation_t *, const uint8_t *, size_t)) \
    X(psa_cipher_update,            psa_status_t, (psa_cipher_operation_t *, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_cipher_finish,            psa_status_t, (psa_cipher_operation_t *, uint8_t *, size_t, size_t *)) \
    X(psa_cipher_abort,             psa_status_t, (psa_cipher_operation_t *)) \
    X(mg_psa_cipher_multirun,       psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *, mg_operation_t)) \
    X(psa_hash_compute,             psa_status_t, (psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_hash_compare,             psa_status_t, (psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t)) \
    X(psa_hash_setup,               psa_status_t, (psa_hash_operation_t *, psa_algorithm_t)) \
    X(psa_hash_update,              psa_status_t, (psa_hash_operation_t *, const uint8_t *, size_t)) \
    X(psa_hash_finish,              psa_status_t, (psa_hash_operation_t *, uint8_t *, size_t, size_t *)) \
    X(psa_hash_abort,               psa_status_t, (psa_hash_operation_t *)) \
    X(mg_psa_hash_compute_multirun, psa_status_t, (psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_sign_message,             psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_sign_message_with_salt,   psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *, const uint8_t *, size_t)) \
    X(psa_verify_message,           psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t)) \
    X(psa_verify_message_with_salt, psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t, size_t)) \
    X(psa_asymmetric_encrypt,       psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_asymmetric_decrypt,       psa_status_t, (psa_key_id_t, psa_algorithm_t, const uint8_t *, size_t, const uint8_t *, size_t, uint8_t *, size_t, size_t *)) \
    X(psa_generate_random,          psa_status_t, (uint8_t *, size_t)) \
    X(psa_crypto_init,              psa_status_t, (void)) \
    X(psa_set_key_type,             void, (psa_key_attributes_t *, psa_key_type_t)) \
    X(psa_set_key_algorithm,        void, (psa_key_attributes_t *, psa_algorithm_t)) \
    X(psa_set_key_bits,             void, (psa_key_attributes_t *, size_t)) \
    X(psa_set_key_lifetime,         void, (psa_key_attributes_t *, psa_key_lifetime_t)) \
    X(psa_set_key_usage_flags,      void, (psa_key_attributes_t *, psa_key_usage_t)) \
    X(psa_set_key_id,               void, (psa_key_attributes_t *, psa_key_id_t)) \
    X(psa_get_key_type,             psa_key_type_t, (const psa_key_attributes_t *)) \
    X(psa_get_key_algorithm,        psa_algorithm_t, (const psa_key_attributes_t *)) \
    X(psa_get_key_bits,             size_t, (const psa_key_attributes_t *)) \
    X(psa_get_key_lifetime,         psa_key_lifetime_t, (const psa_key_attributes_t *)) \
    X(psa_get_key_usage_flags,      psa_key_usage_t, (const psa_key_attributes_t *)) \
    X(psa_get_key_id,               psa_key_id_t, (const psa_key_attributes_t *)) \
    X(psa_import_key,               psa_status_t, (const psa_key_attributes_t *, const uint8_t *, size_t, psa_key_id_t *)) \
    X(mg_psa_rsa_export_key,        psa_status_t, (psa_key_type_t, mg_rsa_context *, uint8_t *, size_t, size_t *)) \
    X(psa_destroy_key,              psa_status_t, (psa_key_id_t)) \
    X(memset_opt,                   void *, (void *, int, size_t)) \
    X(BigNum_Print_Int_to_Hex,      uint16_t, (const bn_int *)) \
    X(BigNum_Set_Digit,             uint16_t, (bn_int *, bn_digit)) \
    X(BigNum_Right_Shift,           uint16_t, (bn_int *, uint16_t)) \
    X(BigNum_Left_Shift,            uint16_t, (bn_int *, uint16_t)) \
    X(BigNum_XOR_CT,                uint16_t, (const bn_int *, const bn_int *, size_t, bn_int *)) \
    X(BigNum_Mod,                   uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Mod_Digit,             uint16_t, (const bn_int *, bn_digit, bn_digit *)) \
    X(BigNum_Calculate_R2_Mod_N,    uint16_t, (const bn_int *, bn_int *)) \
    X(BigNum_Compare,               int8_t,   (const bn_int *, const bn_int *)) \
    X(BigNum_Compare_Digit,         int8_t,   (const bn_int *, bn_digit)) \
    X(BigNum_Subtract,              uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Subtract_Digit,        uint16_t, (const bn_int *, bn_digit, bn_int *)) \
    X(BigNum_SubMod_CT,             uint16_t, (const bn_int *, const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Addition,              uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_AddMod_CT,             uint16_t, (const bn_int *, const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Divide,                uint16_t, (const bn_int *, const bn_int *, bn_int *, bn_int *)) \
    X(BigNum_Divide_2d,             uint16_t, (const bn_int *, uint16_t, bn_int *)) \
    X(BigNum_Div2Mod_CT,            uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Multiply,              uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_MulMod,                uint16_t, (const bn_int *, const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Square,                uint16_t, (const bn_int *, bn_int *)) \
    X(BigNum_GCD_Binary,            uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_LCM,                   uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Exptmod,               uint16_t, (const bn_int *, const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Invmod,                uint16_t, (const bn_int *, const bn_int *, bn_int *)) \
    X(BigNum_Is_Prime,              uint16_t, (bn_int *, uint8_t, bool *)) \
    X(BigNum_Unsigned_Bin_Size,     uint16_t, (const bn_int *, size_t *)) \
    X(BigNum_Read_Unsigned_Bin,     uint16_t, (bn_int *, const uint8_t *, uint16_t)) \
    X(BigNum_Write_Unsigned_Bin,    uint16_t, (const bn_int *, uint8_t *, uint16_t)) \
    X(BigNum_Write_Unsigned_Bin_Len,uint16_t, (const bn_int *, uint8_t *, uint16_t)) \
    X(Check_Calloc,                 uint16_t, (const uint8_t *)) \
    X(Get_Non_Zero_Rand_8,          uint8_t,  (void)) \
    X(Get_Non_Zero_Rand_64,         uint64_t, (void)) \
    X(Reverse_64bit,                uint64_t, (uint64_t)) \
    X(Test_Print,                   void,     (size_t, const uint8_t *)) \
    X(Print_Test_String,            void,     (const uint8_t *, size_t)) \
    X(Print_uchar8,                 void,     (const char *, const uint8_t *, size_t, print_endian_t)) \
    X(Print_uchar64,                void,     (const char *, const uint8_t *, size_t, print_endian_t)) \
    X(Hex_String_To_Byte_Array,     uint16_t, (const char *, uint8_t *, size_t *)) \
    X(Crypto_Wait_For_Timeout,      uint16_t, (const volatile uint8_t *, uint8_t, uint64_t, crypto_wait_polarity_t)) \
    X(Crypto_Runtime_Init,          void,     (void)) \
    X(ECC_Key_Alloc,                ecc_key_t *, (void)) \
    X(ECC_Free_Key,                 uint16_t, (ecc_key_t *)) \
    X(ECC_Generate_Key_Pair,        uint16_t, (ECC_Curve_ID_t, ecc_key_t *)) \
    X(ECDH_Shared_Secret,           uint16_t, (ecc_key_t *, ecc_key_t *, uint8_t *)) \
    X(ECC_Sign_Hash,                uint16_t, (const uint8_t *, uint32_t, uint8_t *, uint32_t *, ecc_key_t *)) \
    X(ECC_Verify_Hash,              uint16_t, (const uint8_t *, const uint8_t *, uint32_t, uint8_t *, ecc_key_t *)) \
    X(ECC_Import_Public_Key,        uint16_t, (ECC_Curve_ID_t, ecc_key_t *, const uint8_t *, const uint8_t *, uint32_t)) \
    X(ECC_Import_Private_Key,       uint16_t, (ECC_Curve_ID_t, ecc_key_t *, const uint8_t *, uint32_t)) \
    X(ECC_Export_Public_Key,        uint16_t, (ecc_key_t *, uint8_t *, uint32_t *, uint8_t *, uint32_t *)) \
    X(ECC_Sign_Hash_Fixed_K,        uint16_t, (const uint8_t *, uint32_t, uint8_t *, uint32_t *, ecc_key_t *, const uint8_t *)) \
    X(AES_XTS,                      uint16_t, (AES_XTS_Config *)) \
    X(BigNum_Count_Bits,            uint32_t, (const bn_int *)) \
    X(BigNum_Mont_Setup,            uint16_t, (const bn_int *, bn_digit *)) \
    X(BigNum_Mont_Norm,             uint16_t, (bn_int *, const bn_int *)) \
    X(BigNum_Mont_Reduce,           uint16_t, (bn_int *, const bn_int *, bn_digit)) \
    X(BigNum_Invmod_Mont_CT,        uint16_t, (const bn_int *, const bn_int *, bn_int *, bn_digit)) \
    X(BigNum_CondSwapCT,            uint16_t, (bn_int *, bn_int *, uint8_t))

#if defined(TARGET_FLASH) && !defined(CRYPTO_DRIVER_IMPLEMENTATION)

/* GOT struct — one function pointer per entry */
#define X(name, ret, args) ret (*(name)) args;
typedef struct {
    CRYPTO_GOT_FUNCTIONS
} CryptoGOT_t;
#undef X

/* Returns pointer to GOT in flash at runtime */
static inline CryptoGOT_t *Get_Crypto_GOT(void) {
    uintptr_t pc = 0;
    __asm__ volatile("auipc %0, 0" : "=r"(pc));
    return (CryptoGOT_t *)((pc & 0xFFFFFFFFF0000000ULL) + CRYPTO_GOT_OFFSET);
}

/* AES macros */
#define AES_Run(cfg)  (Get_Crypto_GOT()->AES_Run(cfg))
#define AES_Zeroize() (Get_Crypto_GOT()->AES_Zeroize())

/* SHA256 macros */
#define SHA256_Single_Run(out, in, len)             (Get_Crypto_GOT()->SHA256_Single_Run(out, in, len))
#define SHA256_Multi_Run(in, in_len, tot_len, iter) (Get_Crypto_GOT()->SHA256_Multi_Run(in, in_len, tot_len, iter))
#define SHA256_Read_Output(out, len)                (Get_Crypto_GOT()->SHA256_Read_Output(out, len))
#define SHA256_Zeroize()                            (Get_Crypto_GOT()->SHA256_Zeroize())

/* RSA macros */
#define RSA_Run(cfg) (Get_Crypto_GOT()->RSA_Run(cfg))
#define RSA_Zeroize() (Get_Crypto_GOT()->RSA_Zeroize())
#define RSA_Generate_Key_Pair(key_pair_output, input_public_expo) \
    (Get_Crypto_GOT()->RSA_Generate_Key_Pair(key_pair_output, input_public_expo))

/* RSA padding macros */
#define RSAES_PKCS1_v1_5_Encrypt(out, out_len, in, in_len) \
    (Get_Crypto_GOT()->RSAES_PKCS1_v1_5_Encrypt(out, out_len, in, in_len))
#define RSAES_PKCS1_v1_5_Decrypt(out, out_len, in, in_len) \
    (Get_Crypto_GOT()->RSAES_PKCS1_v1_5_Decrypt(out, out_len, in, in_len))
#define RSASSA_PKCS1_v1_5_Sign(out, out_len, in, in_len) \
    (Get_Crypto_GOT()->RSASSA_PKCS1_v1_5_Sign(out, out_len, in, in_len))
#define RSASSA_PKCS1_v1_5_Verify(sig, sig_len, in, in_len) \
    (Get_Crypto_GOT()->RSASSA_PKCS1_v1_5_Verify(sig, sig_len, in, in_len))
#define RSAES_OAEP_Encrypt(out, out_len, in, in_len, label, label_len) \
    (Get_Crypto_GOT()->RSAES_OAEP_Encrypt(out, out_len, in, in_len, label, label_len))
#define RSAES_OAEP_Decrypt(in, in_len, out, out_len, label, label_len) \
    (Get_Crypto_GOT()->RSAES_OAEP_Decrypt(in, in_len, out, out_len, label, label_len))
#define RSASSA_PSS_Sign(out, out_len, in, in_len, salt, salt_len, use_salt) \
    (Get_Crypto_GOT()->RSASSA_PSS_Sign(out, out_len, in, in_len, salt, salt_len, use_salt))
#define RSASSA_PSS_Verify(sig, sig_len, in, in_len, exp_salt_len) \
    (Get_Crypto_GOT()->RSASSA_PSS_Verify(sig, sig_len, in, in_len, exp_salt_len))

/* OTP macros */
#define OTP_Read(addr, bytes, data)     (Get_Crypto_GOT()->OTP_Read(addr, bytes, data))
#define OTP_Write(addr, num_bits, data) (Get_Crypto_GOT()->OTP_Write(addr, num_bits, data))
#define OTP_TESTDEC_Test(addr, data)    (Get_Crypto_GOT()->OTP_TESTDEC_Test(addr, data))
#define OTP_WRTEST_Test(addr, status)   (Get_Crypto_GOT()->OTP_WRTEST_Test(addr, status))
#define OTP_Read_CPUID(data)            (Get_Crypto_GOT()->OTP_Read_CPUID(data))
#define OTP_Lock(lock)                  (Get_Crypto_GOT()->OTP_Lock(lock))

/* TRNG macros */
#define TRNG_Generate(buf, req_bytes, vtrng, timeout) \
    (Get_Crypto_GOT()->TRNG_Generate(buf, req_bytes, vtrng, timeout))
#define TRNG_Full_KAT() (Get_Crypto_GOT()->TRNG_Full_KAT())

/* AES-GCM macro */
#define AES_GCM(cfg) (Get_Crypto_GOT()->AES_GCM(cfg))

/* ASCON macros */
#define ASCON_AEAD128a(cfg) (Get_Crypto_GOT()->ASCON_AEAD128a(cfg))
#define ASCON_Hash256(cfg)  (Get_Crypto_GOT()->ASCON_Hash256(cfg))
#define ASCON_XOF128(cfg)   (Get_Crypto_GOT()->ASCON_XOF128(cfg))
#define ASCON_CXOF128(cfg)  (Get_Crypto_GOT()->ASCON_CXOF128(cfg))

/* CMAC-AES macro */
#define CMAC_AES(cfg) (Get_Crypto_GOT()->CMAC_AES(cfg))

/* HMAC macros */
#define HMAC_Get_Context_Size()            (Get_Crypto_GOT()->HMAC_Get_Context_Size())
#define HMAC_Reset(hmac)                   (Get_Crypto_GOT()->HMAC_Reset(hmac))
#define HMAC_Set_Key(hmac, type, key, len) (Get_Crypto_GOT()->HMAC_Set_Key(hmac, type, key, len))
#define HMAC_Update(hmac, msg, len)        (Get_Crypto_GOT()->HMAC_Update(hmac, msg, len))
#define HMAC_Final(hmac, hash)             (Get_Crypto_GOT()->HMAC_Final(hmac, hash))

/* SHA2-512 / SHA2-384 macros */
#define SHA512_Init(ctx)              (Get_Crypto_GOT()->SHA512_Init(ctx))
#define SHA512_Update(ctx, data, len) (Get_Crypto_GOT()->SHA512_Update(ctx, data, len))
#define SHA512_Final(ctx, hash)       (Get_Crypto_GOT()->SHA512_Final(ctx, hash))
#define SHA512_Run(data, len, hash)   (Get_Crypto_GOT()->SHA512_Run(data, len, hash))
#define SHA384_Init(ctx)              (Get_Crypto_GOT()->SHA384_Init(ctx))
#define SHA384_Update(ctx, data, len) (Get_Crypto_GOT()->SHA384_Update(ctx, data, len))
#define SHA384_Final(ctx, hash)       (Get_Crypto_GOT()->SHA384_Final(ctx, hash))
#define SHA384_Run(data, len, hash)   (Get_Crypto_GOT()->SHA384_Run(data, len, hash))

/* SHA-3 macros */
#define Sha3_224_Init(ctx)              (Get_Crypto_GOT()->Sha3_224_Init(ctx))
#define Sha3_224_Update(ctx, data, len) (Get_Crypto_GOT()->Sha3_224_Update(ctx, data, len))
#define Sha3_224_Final(ctx, hash)       (Get_Crypto_GOT()->Sha3_224_Final(ctx, hash))
#define Sha3_256_Init(ctx)              (Get_Crypto_GOT()->Sha3_256_Init(ctx))
#define Sha3_256_Update(ctx, data, len) (Get_Crypto_GOT()->Sha3_256_Update(ctx, data, len))
#define Sha3_256_Final(ctx, hash)       (Get_Crypto_GOT()->Sha3_256_Final(ctx, hash))
#define Sha3_384_Init(ctx)              (Get_Crypto_GOT()->Sha3_384_Init(ctx))
#define Sha3_384_Update(ctx, data, len) (Get_Crypto_GOT()->Sha3_384_Update(ctx, data, len))
#define Sha3_384_Final(ctx, hash)       (Get_Crypto_GOT()->Sha3_384_Final(ctx, hash))
#define Sha3_512_Init(ctx)              (Get_Crypto_GOT()->Sha3_512_Init(ctx))
#define Sha3_512_Update(ctx, data, len) (Get_Crypto_GOT()->Sha3_512_Update(ctx, data, len))
#define Sha3_512_Final(ctx, hash)       (Get_Crypto_GOT()->Sha3_512_Final(ctx, hash))

/* PSA cipher macros */
#define psa_cipher_encrypt(key, alg, in, in_len, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_cipher_encrypt(key, alg, in, in_len, out, out_size, out_len))
#define psa_cipher_decrypt(key, alg, in, in_len, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_cipher_decrypt(key, alg, in, in_len, out, out_size, out_len))
#define psa_cipher_encrypt_setup(op, key, alg) \
    (Get_Crypto_GOT()->psa_cipher_encrypt_setup(op, key, alg))
#define psa_cipher_decrypt_setup(op, key, alg) \
    (Get_Crypto_GOT()->psa_cipher_decrypt_setup(op, key, alg))
#define psa_cipher_set_iv(op, iv, iv_len) \
    (Get_Crypto_GOT()->psa_cipher_set_iv(op, iv, iv_len))
#define psa_cipher_update(op, in, in_len, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_cipher_update(op, in, in_len, out, out_size, out_len))
#define psa_cipher_finish(op, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_cipher_finish(op, out, out_size, out_len))
#define psa_cipher_abort(op) \
    (Get_Crypto_GOT()->psa_cipher_abort(op))
#define mg_psa_cipher_multirun(key, alg, iv, iv_len, in, in_len, out, out_size, out_len, op) \
    (Get_Crypto_GOT()->mg_psa_cipher_multirun(key, alg, iv, iv_len, in, in_len, out, out_size, out_len, op))

/* PSA hash macros */
#define psa_hash_compute(alg, in, in_len, hash, hash_size, hash_len) \
    (Get_Crypto_GOT()->psa_hash_compute(alg, in, in_len, hash, hash_size, hash_len))
#define psa_hash_compare(alg, in, in_len, hash, hash_len) \
    (Get_Crypto_GOT()->psa_hash_compare(alg, in, in_len, hash, hash_len))
#define psa_hash_setup(op, alg) \
    (Get_Crypto_GOT()->psa_hash_setup(op, alg))
#define psa_hash_update(op, in, in_len) \
    (Get_Crypto_GOT()->psa_hash_update(op, in, in_len))
#define psa_hash_finish(op, hash, hash_size, hash_len) \
    (Get_Crypto_GOT()->psa_hash_finish(op, hash, hash_size, hash_len))
#define psa_hash_abort(op) \
    (Get_Crypto_GOT()->psa_hash_abort(op))
#define mg_psa_hash_compute_multirun(alg, in, in_len, hash, hash_size, hash_len) \
    (Get_Crypto_GOT()->mg_psa_hash_compute_multirun(alg, in, in_len, hash, hash_size, hash_len))

/* PSA sign/verify/asymmetric macros */
#define psa_sign_message(key, alg, in, in_len, sig, sig_size, sig_len) \
    (Get_Crypto_GOT()->psa_sign_message(key, alg, in, in_len, sig, sig_size, sig_len))
#define psa_sign_message_with_salt(key, alg, in, in_len, sig, sig_size, sig_len, salt, salt_len) \
    (Get_Crypto_GOT()->psa_sign_message_with_salt(key, alg, in, in_len, sig, sig_size, sig_len, salt, salt_len))
#define psa_verify_message(key, alg, in, in_len, sig, sig_len) \
    (Get_Crypto_GOT()->psa_verify_message(key, alg, in, in_len, sig, sig_len))
#define psa_verify_message_with_salt(key, alg, in, in_len, sig, sig_len, exp_salt_len) \
    (Get_Crypto_GOT()->psa_verify_message_with_salt(key, alg, in, in_len, sig, sig_len, exp_salt_len))
#define psa_asymmetric_encrypt(key, alg, in, in_len, salt, salt_len, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_asymmetric_encrypt(key, alg, in, in_len, salt, salt_len, out, out_size, out_len))
#define psa_asymmetric_decrypt(key, alg, in, in_len, salt, salt_len, out, out_size, out_len) \
    (Get_Crypto_GOT()->psa_asymmetric_decrypt(key, alg, in, in_len, salt, salt_len, out, out_size, out_len))

/* PSA random / init macros */
#define psa_generate_random(out, out_size)  (Get_Crypto_GOT()->psa_generate_random(out, out_size))
#define psa_crypto_init()                   (Get_Crypto_GOT()->psa_crypto_init())

/* PSA key attribute macros */
#define psa_set_key_type(attr, type)         (Get_Crypto_GOT()->psa_set_key_type(attr, type))
#define psa_set_key_algorithm(attr, alg)     (Get_Crypto_GOT()->psa_set_key_algorithm(attr, alg))
#define psa_set_key_bits(attr, bits)         (Get_Crypto_GOT()->psa_set_key_bits(attr, bits))
#define psa_set_key_lifetime(attr, life)     (Get_Crypto_GOT()->psa_set_key_lifetime(attr, life))
#define psa_set_key_usage_flags(attr, flags) (Get_Crypto_GOT()->psa_set_key_usage_flags(attr, flags))
#define psa_set_key_id(attr, key)            (Get_Crypto_GOT()->psa_set_key_id(attr, key))
#define psa_get_key_type(attr)               (Get_Crypto_GOT()->psa_get_key_type(attr))
#define psa_get_key_algorithm(attr)          (Get_Crypto_GOT()->psa_get_key_algorithm(attr))
#define psa_get_key_bits(attr)               (Get_Crypto_GOT()->psa_get_key_bits(attr))
#define psa_get_key_lifetime(attr)           (Get_Crypto_GOT()->psa_get_key_lifetime(attr))
#define psa_get_key_usage_flags(attr)        (Get_Crypto_GOT()->psa_get_key_usage_flags(attr))
#define psa_get_key_id(attr)                 (Get_Crypto_GOT()->psa_get_key_id(attr))

/* PSA key management macros */
#define psa_import_key(attr, data, data_len, key) \
                             (Get_Crypto_GOT()->psa_import_key(attr, data, data_len, key))
#define mg_psa_rsa_export_key(type, rsa, data, data_size, data_len) \
                             (Get_Crypto_GOT()->mg_psa_rsa_export_key(type, rsa, data, data_size, data_len))
#define psa_destroy_key(key) (Get_Crypto_GOT()->psa_destroy_key(key))

/* crypto_helpers macros */
#define Check_Calloc(ptr)                                    (Get_Crypto_GOT()->Check_Calloc(ptr))
#define Get_Non_Zero_Rand_8()                                (Get_Crypto_GOT()->Get_Non_Zero_Rand_8())
#define Get_Non_Zero_Rand_64()                               (Get_Crypto_GOT()->Get_Non_Zero_Rand_64())
#define Reverse_64bit(input_number)                          (Get_Crypto_GOT()->Reverse_64bit(input_number))
#define Test_Print(length, data)                             (Get_Crypto_GOT()->Test_Print(length, data))
#define Print_Test_String(data, length_in_bits)              (Get_Crypto_GOT()->Print_Test_String(data, length_in_bits))
#define Print_uchar8(name, data, len_bytes, mode)            (Get_Crypto_GOT()->Print_uchar8(name, data, len_bytes, mode))
#define Print_uchar64(name, data, len_dword, mode)           (Get_Crypto_GOT()->Print_uchar64(name, data, len_dword, mode))
#define Hex_String_To_Byte_Array(hex_str, byte_array, size)  (Get_Crypto_GOT()->Hex_String_To_Byte_Array(hex_str, byte_array, size))
#define Crypto_Wait_For_Timeout(reg, bit, timeout, polarity) (Get_Crypto_GOT()->Crypto_Wait_For_Timeout(reg, bit, timeout, polarity))
#define Crypto_Runtime_Init()                                (Get_Crypto_GOT()->Crypto_Runtime_Init())

/* ECC macros */
#define ECC_Key_Alloc()                                                (Get_Crypto_GOT()->ECC_Key_Alloc())
#define ECC_Free_Key(key)                                              (Get_Crypto_GOT()->ECC_Free_Key(key))
#define ECC_Generate_Key_Pair(curve_id, key)                           (Get_Crypto_GOT()->ECC_Generate_Key_Pair(curve_id, key))
#define ECDH_Shared_Secret(private_key, public_key, out)               (Get_Crypto_GOT()->ECDH_Shared_Secret(private_key, public_key, out))
#define ECC_Sign_Hash(hash, hashlen, out, outlen, key)                 (Get_Crypto_GOT()->ECC_Sign_Hash(hash, hashlen, out, outlen, key))
#define ECC_Verify_Hash(sig, hash, hashlen, res, key)                  (Get_Crypto_GOT()->ECC_Verify_Hash(sig, hash, hashlen, res, key))
#define ECC_Import_Public_Key(curve_id, key, qx, qy, key_bytes)        (Get_Crypto_GOT()->ECC_Import_Public_Key(curve_id, key, qx, qy, key_bytes))
#define ECC_Import_Private_Key(curve_id, key, priv_bytes, priv_len)    (Get_Crypto_GOT()->ECC_Import_Private_Key(curve_id, key, priv_bytes, priv_len))
#define ECC_Export_Public_Key(key, qx, qxLen, qy, qyLen)               (Get_Crypto_GOT()->ECC_Export_Public_Key(key, qx, qxLen, qy, qyLen))
#define ECC_Sign_Hash_Fixed_K(hash, hashlen, out, outlen, key, fixed_k) \
    (Get_Crypto_GOT()->ECC_Sign_Hash_Fixed_K(hash, hashlen, out, outlen, key, fixed_k))

/* BigNum macros */
#define memset_opt(ptr, value, n)                              (Get_Crypto_GOT()->memset_opt(ptr, value, n))
#define BigNum_Print_Int_to_Hex(a)                             (Get_Crypto_GOT()->BigNum_Print_Int_to_Hex(a))
#define BigNum_Set_Digit(a, d)                                 (Get_Crypto_GOT()->BigNum_Set_Digit(a, d))
#define BigNum_Right_Shift(a, n)                               (Get_Crypto_GOT()->BigNum_Right_Shift(a, n))
#define BigNum_Left_Shift(a, n)                                (Get_Crypto_GOT()->BigNum_Left_Shift(a, n))
#define BigNum_XOR_CT(a, b, len, result)                       (Get_Crypto_GOT()->BigNum_XOR_CT(a, b, len, result))
#define BigNum_Mod(a, b, rem)                                  (Get_Crypto_GOT()->BigNum_Mod(a, b, rem))
#define BigNum_Mod_Digit(a, digit, rem)                        (Get_Crypto_GOT()->BigNum_Mod_Digit(a, digit, rem))
#define BigNum_Calculate_R2_Mod_N(n, result)                   (Get_Crypto_GOT()->BigNum_Calculate_R2_Mod_N(n, result))
#define BigNum_Compare(a, b)                                   (Get_Crypto_GOT()->BigNum_Compare(a, b))
#define BigNum_Compare_Digit(a, digit)                         (Get_Crypto_GOT()->BigNum_Compare_Digit(a, digit))
#define BigNum_Subtract(a, b, result)                          (Get_Crypto_GOT()->BigNum_Subtract(a, b, result))
#define BigNum_Subtract_Digit(a, digit, result)                (Get_Crypto_GOT()->BigNum_Subtract_Digit(a, digit, result))
#define BigNum_SubMod_CT(a, b, mod, result)                    (Get_Crypto_GOT()->BigNum_SubMod_CT(a, b, mod, result))
#define BigNum_Addition(a, b, result)                          (Get_Crypto_GOT()->BigNum_Addition(a, b, result))
#define BigNum_AddMod_CT(a, b, mod, result)                    (Get_Crypto_GOT()->BigNum_AddMod_CT(a, b, mod, result))
#define BigNum_Divide(a, b, quot, rem)                         (Get_Crypto_GOT()->BigNum_Divide(a, b, quot, rem))
#define BigNum_Divide_2d(a, shift, result)                     (Get_Crypto_GOT()->BigNum_Divide_2d(a, shift, result))
#define BigNum_Div2Mod_CT(a, mod, result)                      (Get_Crypto_GOT()->BigNum_Div2Mod_CT(a, mod, result))
#define BigNum_Multiply(a, b, result)                          (Get_Crypto_GOT()->BigNum_Multiply(a, b, result))
#define BigNum_MulMod(a, b, mod, res)                          (Get_Crypto_GOT()->BigNum_MulMod(a, b, mod, res))
#define BigNum_Square(base, result)                            (Get_Crypto_GOT()->BigNum_Square(base, result))
#define BigNum_GCD_Binary(a, b, res)                           (Get_Crypto_GOT()->BigNum_GCD_Binary(a, b, res))
#define BigNum_LCM(a, b, res)                                  (Get_Crypto_GOT()->BigNum_LCM(a, b, res))
#define BigNum_Exptmod(base, exp, mod, result)                 (Get_Crypto_GOT()->BigNum_Exptmod(base, exp, mod, result))
#define BigNum_Invmod(a, b, res)                               (Get_Crypto_GOT()->BigNum_Invmod(a, b, res))
#define BigNum_Is_Prime(num, rounds, isprime)                  (Get_Crypto_GOT()->BigNum_Is_Prime(num, rounds, isprime))
#define BigNum_Unsigned_Bin_Size(a, size)                      (Get_Crypto_GOT()->BigNum_Unsigned_Bin_Size(a, size))
#define BigNum_Read_Unsigned_Bin(a, byte_array, len)           (Get_Crypto_GOT()->BigNum_Read_Unsigned_Bin(a, byte_array, len))
#define BigNum_Write_Unsigned_Bin(a, byte_array, len)          (Get_Crypto_GOT()->BigNum_Write_Unsigned_Bin(a, byte_array, len))
#define BigNum_Write_Unsigned_Bin_Len(a, byte_array, out_size) (Get_Crypto_GOT()->BigNum_Write_Unsigned_Bin_Len(a, byte_array, out_size))
#define BigNum_Count_Bits(a)                                   (Get_Crypto_GOT()->BigNum_Count_Bits(a))
#define BigNum_Mont_Setup(m, mp)                               (Get_Crypto_GOT()->BigNum_Mont_Setup(m, mp))
#define BigNum_Mont_Norm(norm, m)                              (Get_Crypto_GOT()->BigNum_Mont_Norm(norm, m))
#define BigNum_Mont_Reduce(a, m, mp)                           (Get_Crypto_GOT()->BigNum_Mont_Reduce(a, m, mp))
#define BigNum_Invmod_Mont_CT(a, m, r, mp)                     (Get_Crypto_GOT()->BigNum_Invmod_Mont_CT(a, m, r, mp))
#define BigNum_CondSwapCT(a, b, swap)                          (Get_Crypto_GOT()->BigNum_CondSwapCT(a, b, swap))

/* AES-XTS macro */
#define AES_XTS(cfg) (Get_Crypto_GOT()->AES_XTS(cfg))

#endif /* defined(TARGET_FLASH) && !defined(CRYPTO_DRIVER_IMPLEMENTATION) */

#endif /* INCLUDES_CRYPTO_API_H_ */
