#!/usr/bin/env python3
"""
build_firm_script.py

Builds the flash image for Mindgrove MGS2401 applications.

Secure_Boot=n (FLASH_LINK=y):
    Non-secure "dev" image = 0x200-byte header + raw app code.
    Final image, in write order:
        0x000  header: app length @0x00, QSPI flag @0x08
               (0xFFFFFFFF = disabled; 0x00000001 = enabled, with
               CR @0x0C, DCR @0x10, CCR @0x14 following)
        0x200  app code, unpadded, running to end of file

Secure_Boot=y (FLASH_LINK=y):
    Three PEM private keys are used: APP, ROOT (certificate), SSBL.

    MANUFACTURING (is_manufacturing=True):
        Builds one combined image. Final image, in write order:
            0x000000  Boot bundle header (0x200): total signed length
                       @0x00, QSPI flag @0x08 (same layout as the
                       non-secure header: 0xFFFFFFFF disabled, or
                       0x00000001 enabled with CR/DCR/CCR @0x0C/0x10/
                       0x14), signature @0x100. The signature covers
                       SSBL code, the OTA commit app, AND crypto.bin
                       together, as one contiguous range (see
                       sign_boot_bundle()).
            0x000200  SSBL code, padded with 0xFF up to
                       SSBL_CODE_MAX_SIZE, so this region plus the
                       header above ends at SSBL_MAX_SIZE (80KB).
            0x014000  OTA commit app code.bin, padded to
                       OTA_COMMIT_APP_MAX_SIZE (52KB). Part of the
                       signed boot bundle above. The OTA commit app is
                       included in this signed range because it has
                       flash write access (it erases the inactive slot
                       and rewrites the reserved metadata region on
                       every OTA commit), so it must be authenticated
                       by the same root of trust as SSBL. One
                       consequence: the OTA commit app can only be
                       updated by a full manufacturing re-flash under
                       the SSBL key, not swapped independently.
            0x021000  crypto.bin, padded to CRYPTO_BIN_MAX_SIZE
                       (160KB). Not independently signed --
                       authenticated only as part of the combined boot
                       bundle signature above.
            0x049000  Reserved metadata region (active slot, current
                       firmware version, revoked key ids).
            slot addr Signed app image (cert + fw header + code),
                       written at the selected slot's flash offset
                       (SLOT_A_OFFSET or SLOT_B_OFFSET). Everything
                       between the end of the reserved region and the
                       slot offset is left as 0xFF filler.

    OTA (is_manufacturing=False):
        Builds ONLY the signed app image (cert + fw header + code +
        trailing CRC32). SSBL, the OTA commit app, crypto.bin, and the
        reserved metadata region are untouched -- nothing else is
        written to the output file.
"""

import argparse
import getpass
import hashlib
import os
import struct
import sys
import zlib

from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

# ══════════════════════════════════════════════════════════════════
#  Shared constants
# ══════════════════════════════════════════════════════════════════

HEADER_SIZE = 0x200
PAD_BYTE = 0xFF

# Header field layout.
LEN_OFFSET = 0x00
LEN_SIZE = 8
QSPI_FLAG_OFFSET = 0x08
QSPI_FLAG_SIZE = 4
CR_OFFSET = 0x0C
DCR_OFFSET = 0x10
CCR_OFFSET = 0x14
FIELD_SIZE = 4
QSPI_CONFIG_DISABLED = 0xFFFFFFFF
QSPI_CONFIG_ENABLED = 0x00000001

# Secure boot header (SSBL region, flash offset 0x000000).
SSBL_LEN_OFFSET = 0x00
SSBL_LEN_SIZE = 8
SSBL_SIG_OFFSET = 0x100
SSBL_SIG_SIZE = 256

# SSBL / OTA commit app / crypto.bin: three regions back-to-back in
# flash, all covered by one signature.
SSBL_MAX_SIZE = 0x014000
SSBL_CODE_MAX_SIZE = SSBL_MAX_SIZE - HEADER_SIZE

OTA_COMMIT_APP_FLASH_OFFSET = SSBL_MAX_SIZE
OTA_COMMIT_APP_MAX_SIZE = 0x00D000

CRYPTO_BIN_OFFSET = OTA_COMMIT_APP_FLASH_OFFSET + OTA_COMMIT_APP_MAX_SIZE
CRYPTO_BIN_MAX_SIZE = 0x028000

# Flash base and app slot addresses.
FLASH_BASE = 0x90000000
FLASH_ORIGIN_SLOT_A = 0x9004A000
FLASH_ORIGIN_SLOT_B = 0x900A5000
SLOT_A_OFFSET = FLASH_ORIGIN_SLOT_A - FLASH_BASE
SLOT_B_OFFSET = FLASH_ORIGIN_SLOT_B - FLASH_BASE
SLOT_MAX_SIZE = SLOT_B_OFFSET - SLOT_A_OFFSET

# Reserved metadata region, immediately follows crypto.bin.
RESERVED_REGION_OFFSET = CRYPTO_BIN_OFFSET + CRYPTO_BIN_MAX_SIZE
ACTIVE_SLOT_REL = 0x000
CURRENT_VERSION_REL = 0x004
CURRENT_VERSION_SIZE = 3
REVOKED_COUNT_REL = 0x008
REVOKED_KEY_IDS_REL = 0x009
MAX_REVOKED_KEYS = 16
SLOT_A_ACTIVE = 0x0A
SLOT_B_ACTIVE = 0x0B
REVOKED_KEY_ID_SENTINEL = 0xFF

assert SSBL_MAX_SIZE + OTA_COMMIT_APP_MAX_SIZE + CRYPTO_BIN_MAX_SIZE == RESERVED_REGION_OFFSET, \
    "SSBL_MAX_SIZE + OTA_COMMIT_APP_MAX_SIZE + CRYPTO_BIN_MAX_SIZE must equal RESERVED_REGION_OFFSET"

# App/cert slot layout.
CERT_OFFSET_SIGNATURE     = 0x000
CERT_OFFSET_APP_PUBKEY    = 0x100
CERT_OFFSET_KEY_ID        = 0x200
CERT_OFFSET_REVOKED_KEYID = 0x201
CERT_SIZE                 = 0x300

OFFSET_SIGNATURE    = CERT_SIZE + 0x000
OFFSET_APP_SIZE     = CERT_SIZE + 0x100
OFFSET_PADDING_MODE = CERT_SIZE + 0x108
OFFSET_SALT_LEN     = CERT_SIZE + 0x109
OFFSET_FW_VERSION   = CERT_SIZE + 0x10A
OFFSET_APP_CODE     = CERT_SIZE + 0x200

RSA_KEY_BYTES    = 256
SIGNATURE_SIZE   = RSA_KEY_BYTES
APP_HEADER_SIZE  = OFFSET_APP_CODE - OFFSET_APP_SIZE
CERT_SIGNED_SIZE = RSA_KEY_BYTES + 1 + 1
CRC_SIZE         = 4

PADDING_MODE_PKCS1_V1_5 = 0x01
PADDING_MODE_PSS        = 0x02
SALT_LEN_DEFAULT_SENTINEL = 0xFF
SALT_LEN_DEFAULT_BYTES    = 32

SHA256_DIGEST_SIZE = 32
RSA_E = 65537

SHA256_DIGEST_INFO = bytes([
    0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65,
    0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20,
])


# ══════════════════════════════════════════════════════════════════
#  Generic prompt helpers
# ══════════════════════════════════════════════════════════════════

def prompt_yes_no(question):
    while True:
        ans = input(f"{question} (y/n): ").strip().lower()
        if ans in ("y", "yes"):
            return True
        if ans in ("n", "no"):
            return False
        print("Please answer 'y' or 'n'.")


def prompt_hex_value(field_name):
    while True:
        raw = input(f"  Enter {field_name} value (hex, e.g. 0x1234ABCD): ").strip()
        try:
            val = int(raw, 16)
        except ValueError:
            print("  Invalid hex value, try again.")
            continue
        if not (0 <= val <= 0xFFFFFFFF):
            print("  Value must fit in 32 bits.")
            continue
        return val


def prompt_path(question):
    while True:
        p = input(f"{question}: ").strip().strip('"').strip("'")
        try:
            with open(p, "rb"):
                pass
            return p
        except OSError as e:
            print(f"  Could not open '{p}': {e}. Try again.")


def bytes_to_int(b: bytes) -> int:
    return int.from_bytes(b, byteorder='big')


def int_to_bytes(n: int, length: int) -> bytes:
    return n.to_bytes(length, byteorder='big')


def reverse_every_8_bytes(data: bytes) -> bytes:
    if len(data) % 8 != 0:
        raise ValueError(f"data length {len(data)} is not a multiple of 8; "
                          f"cannot apply per-word swap")
    return b''.join(data[i:i + 8][::-1] for i in range(0, len(data), 8))


def pad_to(data: bytes, size: int, what: str) -> bytes:
    if len(data) > size:
        raise ValueError(f"{what} is {len(data)} bytes, exceeds max {size} bytes")
    return data + bytes([PAD_BYTE] * (size - len(data)))


# ══════════════════════════════════════════════════════════════════
#  PEM key loading
# ══════════════════════════════════════════════════════════════════

def load_rsa_private_key_from_pem(path: str):
    with open(path, "rb") as f:
        pem_data = f.read()

    password = None
    while True:
        try:
            private_key = serialization.load_pem_private_key(
                pem_data, password=password, backend=default_backend()
            )
            break
        except TypeError:
            password = getpass.getpass(f"  Passphrase for '{path}': ").encode()
        except ValueError as e:
            if password is not None:
                print(f"  Incorrect passphrase or corrupt key: {e}")
                password = getpass.getpass(f"  Passphrase for '{path}' (try again): ").encode()
            else:
                raise

    numbers = private_key.private_numbers()
    n = numbers.public_numbers.n
    d = numbers.d
    key_bytes = (n.bit_length() + 7) // 8
    return n, d, key_bytes


def read_key_pair_pem(label: str) -> tuple:
    print(f"--- {label} key ---")
    while True:
        path = input(f"Path to the {label} private key PEM file: ").strip().strip('"').strip("'")
        try:
            n, d, key_bytes = load_rsa_private_key_from_pem(path)
        except FileNotFoundError:
            print(f"  File not found: '{path}'. Please try again.")
            continue
        except Exception as e:
            print(f"  Error loading key: {e}. Please try again.")
            continue

        if key_bytes != 256:
            print(f"  Warning: key is {key_bytes} bytes ({key_bytes*8}-bit) -- expected 256 bytes (RSA-2048)")
        else:
            print(f"  Loaded {label} key: RSA-2048 ({key_bytes} bytes)")
        return n, d


# ══════════════════════════════════════════════════════════════════
#  RSA signature encoding (PKCS#1 v1.5 and PSS)
# ══════════════════════════════════════════════════════════════════

def pkcs1_v15_encode(digest: bytes, em_len: int) -> bytes:
    t = SHA256_DIGEST_INFO + digest
    t_len = len(t)
    if em_len < t_len + 11:
        raise ValueError("Intended encoded message length too short")
    ps_len = em_len - t_len - 3
    em = b'\x00\x01' + b'\xff' * ps_len + b'\x00' + t
    assert len(em) == em_len
    return em


def mgf1(seed: bytes, mask_len: int) -> bytes:
    t = b''
    counter = 0
    while len(t) < mask_len:
        c = counter.to_bytes(4, 'big')
        t += hashlib.sha256(seed + c).digest()
        counter += 1
    return t[:mask_len]


def pss_encode(digest: bytes, em_bits: int, salt_len: int, salt: bytes = None) -> bytes:
    h_len = SHA256_DIGEST_SIZE
    em_len = (em_bits + 7) // 8
    if em_len < h_len + salt_len + 2:
        raise ValueError("Intended encoded message length too short for PSS")
    if salt is None:
        salt = os.urandom(salt_len)
    elif len(salt) != salt_len:
        raise ValueError("Provided salt does not match salt_len")

    m_prime = (b'\x00' * 8) + digest + salt
    h = hashlib.sha256(m_prime).digest()

    ps_len = em_len - salt_len - h_len - 2
    db = (b'\x00' * ps_len) + b'\x01' + salt
    db_mask = mgf1(h, em_len - h_len - 1)
    masked_db = bytes(a ^ b for a, b in zip(db, db_mask))

    extra_bits = 8 * em_len - em_bits
    if extra_bits > 0:
        mask = 0xFF >> extra_bits
        masked_db = bytes([masked_db[0] & mask]) + masked_db[1:]

    em = masked_db + h + b'\xbc'
    assert len(em) == em_len
    return em


def pss_verify_from_em(em: bytes, digest: bytes, salt_len: int) -> bool:
    h_len = SHA256_DIGEST_SIZE
    em_bits = RSA_KEY_BYTES * 8 - 1
    em_len = (em_bits + 7) // 8
    if len(em) != em_len or em[-1] != 0xbc:
        return False

    masked_db = em[:em_len - h_len - 1]
    h = em[em_len - h_len - 1: em_len - 1]

    extra_bits = 8 * em_len - em_bits
    mask = 0xFF >> extra_bits if extra_bits > 0 else 0xFF
    if extra_bits > 0 and (masked_db[0] & ~mask & 0xFF) != 0:
        return False

    db_mask = mgf1(h, em_len - h_len - 1)
    db = bytes(a ^ b for a, b in zip(masked_db, db_mask))
    db = bytes([db[0] & mask]) + db[1:]

    ps_len = em_len - salt_len - h_len - 2
    if db[:ps_len] != b'\x00' * ps_len or db[ps_len] != 0x01:
        return False
    salt = db[ps_len + 1:]

    m_prime = (b'\x00' * 8) + digest + salt
    h_prime = hashlib.sha256(m_prime).digest()
    return h_prime == h


def encode_em(digest: bytes, padding_mode: int, salt_len_actual: int) -> bytes:
    if padding_mode == PADDING_MODE_PKCS1_V1_5:
        return pkcs1_v15_encode(digest, RSA_KEY_BYTES)
    return pss_encode(digest, em_bits=RSA_KEY_BYTES * 8 - 1, salt_len=salt_len_actual)


def verify_em(em_check: bytes, digest: bytes, padding_mode: int, salt_len: int) -> bool:
    if padding_mode == PADDING_MODE_PKCS1_V1_5:
        return em_check == pkcs1_v15_encode(digest, RSA_KEY_BYTES)
    if padding_mode == PADDING_MODE_PSS:
        return pss_verify_from_em(em_check, digest, salt_len)
    return False


def rsa_sign_em(em: bytes, d: int, n: int, key_bytes: int) -> bytes:
    em_int = bytes_to_int(em)
    if em_int >= n:
        raise ValueError("Encoded message >= modulus -- check the key")
    sig_int = pow(em_int, d, n)
    return int_to_bytes(sig_int, key_bytes)


def rsa_self_verify(signature: bytes, expected_em: bytes, n: int, e: int, key_bytes: int) -> bool:
    sig_int = bytes_to_int(signature)
    em_check = int_to_bytes(pow(sig_int, e, n), key_bytes)
    return em_check == expected_em


def compute_crc32(data: bytes) -> int:
    return zlib.crc32(data) & 0xFFFFFFFF


# ══════════════════════════════════════════════════════════════════
#  App/cert signing input helpers
# ══════════════════════════════════════════════════════════════════

def get_padding_mode() -> int:
    while True:
        choice = input("Signature padding mode (used for BOTH the certificate and the "
                        "firmware signature): (1) PKCS#1 v1.5, (2) PSS: ").strip()
        if choice == '1':
            return PADDING_MODE_PKCS1_V1_5
        if choice == '2':
            return PADDING_MODE_PSS
        print("  Invalid choice. Enter 1 or 2.")


def get_salt_len(padding_mode: int) -> tuple:
    if padding_mode == PADDING_MODE_PKCS1_V1_5:
        return SALT_LEN_DEFAULT_SENTINEL, 0
    resp = input("PSS salt length in bytes, applies to both signatures "
                 "[Enter for default: 0xFF sentinel -> 32 bytes]: ").strip()
    if resp == "":
        return SALT_LEN_DEFAULT_SENTINEL, SALT_LEN_DEFAULT_BYTES
    salt_len = int(resp)
    if not (0 <= salt_len <= 254):
        raise ValueError("salt_len must be 0-254 (0xFF is reserved as the default sentinel)")
    return salt_len, salt_len


def get_fw_version() -> bytes:
    print("Firmware version (major.minor.patch, each 0-255 as independent single bytes):")
    while True:
        resp = input("  Enter as major.minor.patch (e.g. 1.0.5 or 22.1.25): ").strip()
        parts = resp.split(".")
        if len(parts) != 3:
            print("  Please enter exactly three numbers separated by dots, e.g. 1.0.5")
            continue
        try:
            major, minor, patch = (int(p) for p in parts)
            if not all(0 <= v <= 255 for v in (major, minor, patch)):
                raise ValueError
        except ValueError:
            print("  Each of major/minor/patch must be an integer 0-255.")
            continue
        return bytes([major, minor, patch])


def get_byte_id(prompt: str) -> int:
    while True:
        resp = input(prompt).strip()
        try:
            val = int(resp, 0)
            if not (0 <= val <= 255):
                raise ValueError
            return val
        except ValueError:
            print("  Please enter an integer 0-255 (e.g. 1, or 0x01).")


def resolve_qspi_config(args, question="Custom QSPI Configuration needed?"):
    """Resolves QSPI config from CLI flags (non-interactive) or by prompting
    the user. Returns (qspi_enabled, cr, dcr, ccr). Shared by both the
    non-secure header and the secure SSBL/manufacturing header, since both
    use the same CR/DCR/CCR field layout."""

    if args.qspi_config == "n":
        print("QSPI_CONFIG=n: custom QSPI configuration disabled, "
              "fields will be set to 0xFF (sentinel).")
        return False, 0, 0, 0

    if args.qspi_config == "y":
        if args.cr is not None and args.dcr is not None and args.ccr is not None:
            return True, args.cr, args.dcr, args.ccr
        print("QSPI_CONFIG=y: enter the custom QSPI configuration values:")
        cr = prompt_hex_value("CR")
        dcr = prompt_hex_value("DCR")
        ccr = prompt_hex_value("CCR")
        return True, cr, dcr, ccr

    if args.non_interactive:
        if args.cr is not None and args.dcr is not None and args.ccr is not None:
            return True, args.cr, args.dcr, args.ccr
        return False, 0, 0, 0

    qspi_enabled = prompt_yes_no(question)
    if not qspi_enabled:
        print("Custom QSPI Configuration disabled, fields will be set to 0xFF (sentinel).")
        return False, 0, 0, 0

    print("Enter the custom QSPI configuration values:")
    cr = prompt_hex_value("CR")
    dcr = prompt_hex_value("DCR")
    ccr = prompt_hex_value("CCR")
    return True, cr, dcr, ccr


# ══════════════════════════════════════════════════════════════════
#  Non-secure path
# ══════════════════════════════════════════════════════════════════

def build_nonsecure_header(app_length, qspi_enabled, cr, dcr, ccr):
    header = bytearray([PAD_BYTE] * HEADER_SIZE)
    header[LEN_OFFSET:LEN_OFFSET + LEN_SIZE] = struct.pack("<Q", app_length)
    flag_val = QSPI_CONFIG_ENABLED if qspi_enabled else QSPI_CONFIG_DISABLED
    header[QSPI_FLAG_OFFSET:QSPI_FLAG_OFFSET + QSPI_FLAG_SIZE] = struct.pack("<I", flag_val)
    if qspi_enabled:
        header[CR_OFFSET:CR_OFFSET + FIELD_SIZE] = struct.pack("<I", cr)
        header[DCR_OFFSET:DCR_OFFSET + FIELD_SIZE] = struct.pack("<I", dcr)
        header[CCR_OFFSET:CCR_OFFSET + FIELD_SIZE] = struct.pack("<I", ccr)
    return bytes(header)


def do_nonsecure(args):
    try:
        with open(args.app_bin, "rb") as f:
            app_code = f.read()
    except OSError as e:
        print(f"Error reading app binary: {e}", file=sys.stderr)
        sys.exit(1)

    app_length = len(app_code)

    if not args.non_interactive:
        print(f"App binary: {args.app_bin} ({app_length} bytes)")

    qspi_enabled, cr, dcr, ccr = resolve_qspi_config(args, question="\nCustom QSPI Configuration needed?")

    header = build_nonsecure_header(app_length, qspi_enabled, cr, dcr, ccr)

    print("\nBuilding combined header + app image...")
    with open(args.output, "wb") as f:
        f.write(header)
        f.write(app_code)

    print(f"\nWrote non-secure image: {args.output}")
    print(f"  Header size : {HEADER_SIZE} (0x{HEADER_SIZE:X}) bytes")
    print(f"  App length  : {app_length} (0x{app_length:X}) bytes")
    print(f"  Total size  : {HEADER_SIZE + app_length} bytes")
    print(f"  QSPI config : {'ENABLED (0x%08X)' % QSPI_CONFIG_ENABLED if qspi_enabled else 'DISABLED (0xFFFFFFFF)'}")
    if qspi_enabled:
        print(f"    CR  = 0x{cr:08X}")
        print(f"    DCR = 0x{dcr:08X}")
        print(f"    CCR = 0x{ccr:08X}")
    print(f"Combined image ready: {args.output}")


# ══════════════════════════════════════════════════════════════════
#  Secure path: boot bundle (SSBL + OTA commit app + crypto.bin)
#  combined signing
# ══════════════════════════════════════════════════════════════════

def sign_boot_bundle(ssbl_code: bytes, ota_code: bytes, crypto_bin: bytes,
                      pem_path: str) -> tuple:
    if len(ssbl_code) > SSBL_CODE_MAX_SIZE:
        raise ValueError(f"SSBL code is {len(ssbl_code)} bytes, exceeds max "
                          f"{SSBL_CODE_MAX_SIZE} bytes")
    if len(ota_code) > OTA_COMMIT_APP_MAX_SIZE:
        raise ValueError(f"OTA commit app is {len(ota_code)} bytes, exceeds max "
                          f"{OTA_COMMIT_APP_MAX_SIZE} bytes")
    if len(crypto_bin) > CRYPTO_BIN_MAX_SIZE:
        raise ValueError(f"crypto.bin is {len(crypto_bin)} bytes, exceeds max "
                          f"{CRYPTO_BIN_MAX_SIZE} bytes")

    ssbl_padded = pad_to(ssbl_code, SSBL_CODE_MAX_SIZE, "SSBL code")
    ota_padded = pad_to(ota_code, OTA_COMMIT_APP_MAX_SIZE, "OTA commit app")

    crypto_verify_len = (len(crypto_bin) + 7) & ~7
    crypto_for_hash = pad_to(crypto_bin, crypto_verify_len, "crypto.bin (8-byte align)")

    n, d, key_bytes = load_rsa_private_key_from_pem(pem_path)
    if key_bytes != SSBL_SIG_SIZE:
        print(f"  Warning: key is {key_bytes} bytes, expected {SSBL_SIG_SIZE} (RSA-2048)")

    contiguous = ssbl_padded + ota_padded + crypto_for_hash
    swapped = reverse_every_8_bytes(contiguous)
    digest = hashlib.sha256(swapped).digest()
    em = pkcs1_v15_encode(digest, key_bytes)
    em_int = bytes_to_int(em)
    if em_int >= n:
        raise ValueError("Encoded message (EM) >= modulus (n); check the key")

    sig_int = pow(em_int, d, n)
    signature = int_to_bytes(sig_int, key_bytes)

    if not rsa_self_verify(signature, em, n, RSA_E, key_bytes):
        raise ValueError("Self-verification of boot bundle signature FAILED -- aborting, "
                          "do not flash this image")

    total_verified_len = len(contiguous)
    return reverse_every_8_bytes(signature), n, total_verified_len


def build_boot_region(ssbl_code: bytes, ota_code: bytes, crypto_bin: bytes,
                       signature_flash: bytes, total_verified_len: int,
                       qspi_enabled: bool = False, cr: int = 0, dcr: int = 0,
                       ccr: int = 0) -> bytes:
    if len(ssbl_code) > SSBL_CODE_MAX_SIZE:
        raise ValueError(f"SSBL code is {len(ssbl_code)} bytes, exceeds max {SSBL_CODE_MAX_SIZE} bytes")
    if len(ota_code) > OTA_COMMIT_APP_MAX_SIZE:
        raise ValueError(f"OTA commit app is {len(ota_code)} bytes, exceeds max {OTA_COMMIT_APP_MAX_SIZE} bytes")
    if len(crypto_bin) > CRYPTO_BIN_MAX_SIZE:
        raise ValueError(f"crypto.bin is {len(crypto_bin)} bytes, exceeds max {CRYPTO_BIN_MAX_SIZE} bytes")

    header = bytearray([PAD_BYTE] * HEADER_SIZE)
    header[SSBL_LEN_OFFSET:SSBL_LEN_OFFSET + SSBL_LEN_SIZE] = struct.pack("<Q", total_verified_len)
    flag_val = QSPI_CONFIG_ENABLED if qspi_enabled else QSPI_CONFIG_DISABLED
    header[QSPI_FLAG_OFFSET:QSPI_FLAG_OFFSET + QSPI_FLAG_SIZE] = struct.pack("<I", flag_val)
    if qspi_enabled:
        header[CR_OFFSET:CR_OFFSET + FIELD_SIZE] = struct.pack("<I", cr)
        header[DCR_OFFSET:DCR_OFFSET + FIELD_SIZE] = struct.pack("<I", dcr)
        header[CCR_OFFSET:CCR_OFFSET + FIELD_SIZE] = struct.pack("<I", ccr)
    header[SSBL_SIG_OFFSET:SSBL_SIG_OFFSET + SSBL_SIG_SIZE] = signature_flash

    ssbl_padded = pad_to(ssbl_code, SSBL_CODE_MAX_SIZE, "SSBL code")
    ota_padded = pad_to(ota_code, OTA_COMMIT_APP_MAX_SIZE, "OTA commit app")
    crypto_padded = pad_to(crypto_bin, CRYPTO_BIN_MAX_SIZE, "crypto.bin")

    region = bytes(header) + ssbl_padded + ota_padded + crypto_padded
    assert len(region) == SSBL_MAX_SIZE + OTA_COMMIT_APP_MAX_SIZE + CRYPTO_BIN_MAX_SIZE == RESERVED_REGION_OFFSET
    return region


def readback_verify_boot_region(boot_region: bytes, n: int, e: int = RSA_E) -> bool:
    total_verified_len = struct.unpack("<Q", boot_region[SSBL_LEN_OFFSET:SSBL_LEN_OFFSET + SSBL_LEN_SIZE])[0]
    sig_flash = boot_region[SSBL_SIG_OFFSET:SSBL_SIG_OFFSET + SSBL_SIG_SIZE]

    if total_verified_len > SSBL_MAX_SIZE + OTA_COMMIT_APP_MAX_SIZE + CRYPTO_BIN_MAX_SIZE - HEADER_SIZE:
        return False
    if total_verified_len % 8 != 0:
        return False

    contiguous = boot_region[HEADER_SIZE:HEADER_SIZE + total_verified_len]
    if len(contiguous) != total_verified_len:
        return False

    sig_be = reverse_every_8_bytes(sig_flash)
    swapped = reverse_every_8_bytes(contiguous)
    digest = hashlib.sha256(swapped).digest()
    sig_int = bytes_to_int(sig_be)
    em_check = int_to_bytes(pow(sig_int, e, n), SSBL_SIG_SIZE)
    return em_check == pkcs1_v15_encode(digest, SSBL_SIG_SIZE)


# ══════════════════════════════════════════════════════════════════
#  Secure path: app/certificate signing
# ══════════════════════════════════════════════════════════════════

def build_signed_app_slot_image(app_code: bytes, is_ota: bool):
    app_size = len(app_code)

    padding_mode = get_padding_mode()
    salt_len_byte, salt_len_actual = get_salt_len(padding_mode)
    fw_version_bytes = get_fw_version()
    key_id = get_byte_id("App key_id (0-255): ")
    revoked_key_id = get_byte_id("revoked_key_id (0-255, e.g. 0xFF if none revoked): ")

    print()
    app_n, app_d = read_key_pair_pem("APP")
    print()
    root_n, root_d = read_key_pair_pem("ROOT (certificate)")
    print()

    app_pubkey_bytes = int_to_bytes(app_n, RSA_KEY_BYTES)
    cert_signed_data = app_pubkey_bytes + bytes([key_id, revoked_key_id])
    cert_digest = hashlib.sha256(cert_signed_data).digest()
    cert_em = encode_em(cert_digest, padding_mode, salt_len_actual)
    cert_signature = rsa_sign_em(cert_em, root_d, root_n, RSA_KEY_BYTES)
    if not rsa_self_verify(cert_signature, cert_em, root_n, RSA_E, RSA_KEY_BYTES):
        raise ValueError("Cert self-verification FAILED -- aborting")
    print("  Cert self-verification PASSED.")

    header = bytearray([PAD_BYTE]) * APP_HEADER_SIZE
    header[0:8] = app_size.to_bytes(8, 'big')
    header[OFFSET_PADDING_MODE - OFFSET_APP_SIZE] = padding_mode
    header[OFFSET_SALT_LEN - OFFSET_APP_SIZE] = salt_len_byte
    fw_ver_rel = OFFSET_FW_VERSION - OFFSET_APP_SIZE
    header[fw_ver_rel:fw_ver_rel + 3] = fw_version_bytes
    header = bytes(header)

    signed_data = header + app_code
    fw_digest = hashlib.sha256(signed_data).digest()
    fw_em = encode_em(fw_digest, padding_mode, salt_len_actual)
    fw_signature = rsa_sign_em(fw_em, app_d, app_n, RSA_KEY_BYTES)
    if not rsa_self_verify(fw_signature, fw_em, app_n, RSA_E, RSA_KEY_BYTES):
        raise ValueError("Firmware self-verification FAILED -- aborting")
    print("  Firmware self-verification PASSED.")

    crc_end = OFFSET_APP_CODE + app_size
    total_size = crc_end + (CRC_SIZE if is_ota else 0)
    image = bytearray([PAD_BYTE] * total_size)
    image[CERT_OFFSET_SIGNATURE:CERT_OFFSET_SIGNATURE + SIGNATURE_SIZE] = cert_signature
    image[CERT_OFFSET_APP_PUBKEY:CERT_OFFSET_APP_PUBKEY + RSA_KEY_BYTES] = app_pubkey_bytes
    image[CERT_OFFSET_KEY_ID] = key_id
    image[CERT_OFFSET_REVOKED_KEYID] = revoked_key_id
    image[OFFSET_SIGNATURE:OFFSET_SIGNATURE + SIGNATURE_SIZE] = fw_signature
    image[OFFSET_APP_SIZE:OFFSET_APP_CODE] = header
    image[OFFSET_APP_CODE:OFFSET_APP_CODE + app_size] = app_code

    if is_ota:
        crc_value = compute_crc32(bytes(image[0:crc_end]))
        image[crc_end:crc_end + CRC_SIZE] = crc_value.to_bytes(CRC_SIZE, 'big')

    return bytes(image), fw_version_bytes, app_n, root_n


def build_reserved_region(slot: str, fw_version_bytes: bytes) -> bytes:
    region = bytearray([PAD_BYTE] * (REVOKED_KEY_IDS_REL + MAX_REVOKED_KEYS))
    region[ACTIVE_SLOT_REL] = SLOT_A_ACTIVE if slot == "A" else SLOT_B_ACTIVE
    region[CURRENT_VERSION_REL:CURRENT_VERSION_REL + CURRENT_VERSION_SIZE] = fw_version_bytes
    region[REVOKED_COUNT_REL] = REVOKED_KEY_ID_SENTINEL
    return bytes(region)


def readback_verify_app_image(image: bytes, root_n: int, app_n: int, e: int, is_ota: bool) -> tuple:
    padding_mode = image[OFFSET_PADDING_MODE]
    raw_salt_len = image[OFFSET_SALT_LEN]
    salt_len = SALT_LEN_DEFAULT_BYTES if raw_salt_len == SALT_LEN_DEFAULT_SENTINEL else raw_salt_len

    cert_signature = image[CERT_OFFSET_SIGNATURE:CERT_OFFSET_SIGNATURE + SIGNATURE_SIZE]
    app_pubkey_bytes = image[CERT_OFFSET_APP_PUBKEY:CERT_OFFSET_APP_PUBKEY + RSA_KEY_BYTES]
    key_id = image[CERT_OFFSET_KEY_ID]
    revoked_key_id = image[CERT_OFFSET_REVOKED_KEYID]

    cert_signed_data = app_pubkey_bytes + bytes([key_id, revoked_key_id])
    cert_digest = hashlib.sha256(cert_signed_data).digest()

    cert_sig_int = bytes_to_int(cert_signature)
    cert_em_check = int_to_bytes(pow(cert_sig_int, e, root_n), RSA_KEY_BYTES)
    cert_ok = verify_em(cert_em_check, cert_digest, padding_mode, salt_len)

    recovered_app_n = bytes_to_int(app_pubkey_bytes)
    if recovered_app_n != app_n:
        fw_ok = False
    else:
        fw_signature = image[OFFSET_SIGNATURE:OFFSET_SIGNATURE + SIGNATURE_SIZE]
        app_size = int.from_bytes(image[OFFSET_APP_SIZE:OFFSET_APP_SIZE + 8], 'big')
        fw_signed_data = image[OFFSET_APP_SIZE:OFFSET_APP_CODE + app_size]
        fw_digest = hashlib.sha256(fw_signed_data).digest()
        fw_sig_int = bytes_to_int(fw_signature)
        fw_em_check = int_to_bytes(pow(fw_sig_int, e, recovered_app_n), RSA_KEY_BYTES)
        fw_ok = verify_em(fw_em_check, fw_digest, padding_mode, salt_len)

    if not is_ota:
        return cert_ok, fw_ok, None

    app_size = int.from_bytes(image[OFFSET_APP_SIZE:OFFSET_APP_SIZE + 8], 'big')
    crc_end = OFFSET_APP_CODE + app_size
    stored_crc = int.from_bytes(image[crc_end:crc_end + CRC_SIZE], 'big')
    recomputed_crc = compute_crc32(image[0:crc_end])
    crc_ok = (stored_crc == recomputed_crc)

    return cert_ok, fw_ok, crc_ok


# ══════════════════════════════════════════════════════════════════
#  Secure path
# ══════════════════════════════════════════════════════════════════

def read_binary_or_exit(path: str, label: str) -> bytes:
    """Reads a binary file, printing a labeled error and exiting on failure."""
    try:
        with open(path, "rb") as f:
            return f.read()
    except OSError as e:
        print(f"Error reading {label}: {e}", file=sys.stderr)
        sys.exit(1)


def prompt_manufacturing_flag(args) -> bool:
    if args.manufacturing is not None:
        return args.manufacturing == "y"

    print("Is this the INITIAL MANUFACTURING firmware (factory/golden image)?")
    print("  y -> builds the FULL flash image: SSBL + OTA commit app + crypto.bin")
    print("       (all jointly signed as one boot bundle) + reserved metadata")
    print("       + signed app, all combined for a fresh device.")
    print("  n -> this is an OTA update image: builds ONLY the signed app image.")
    return prompt_yes_no("Manufacturing build?")


def prompt_slot(args) -> str:
    if args.slot in ("A", "B"):
        print(f"Slot: {args.slot} (provided by build system)")
        return args.slot

    print("Which slot is this app image for?")
    print(f"  A -> 0x{FLASH_ORIGIN_SLOT_A:08X} (the golden/manufacturing slot)")
    print(f"  B -> 0x{FLASH_ORIGIN_SLOT_B:08X} (the alternate OTA slot)")
    slot = ""
    while slot not in ("A", "B"):
        slot = input("Enter slot [A/B]: ").strip().upper()
    print(f"  -> Slot {slot} selected.")
    return slot


def write_ota_app_image(args, slot: str, signed_app_image: bytes):
    slot_offset = SLOT_A_OFFSET if slot == "A" else SLOT_B_OFFSET
    slot_addr = FLASH_ORIGIN_SLOT_A if slot == "A" else FLASH_ORIGIN_SLOT_B

    if len(signed_app_image) > SLOT_MAX_SIZE:
        print(f"Error: signed app image is {len(signed_app_image)} bytes, exceeds "
              f"the {SLOT_MAX_SIZE}-byte slot size -- refusing to write anything.",
              file=sys.stderr)
        sys.exit(1)

    with open(args.output, "wb") as f:
        f.write(signed_app_image)

    print(f"Wrote OTA app image: {args.output}")
    print(f"  Target slot (inactive) : {slot}")
    print(f"  Flash start address    : 0x{slot_addr:08X} (offset 0x{slot_offset:06X})")
    print(f"  Size                   : {len(signed_app_image)} / {SLOT_MAX_SIZE} bytes")


def build_manufacturing_image(args, slot: str, signed_app_image: bytes, fw_version_bytes: bytes):
    if not args.ssbl_bin:
        print("Error: --ssbl-bin is required for a manufacturing build", file=sys.stderr)
        sys.exit(1)
    if not args.ota_flash_prog_bin:
        print("Error: --ota-flash-prog-bin is required for a manufacturing build", file=sys.stderr)
        sys.exit(1)
    if not args.crypto_bin:
        print("Error: --crypto-bin is required for a manufacturing build", file=sys.stderr)
        sys.exit(1)

    ssbl_code = read_binary_or_exit(args.ssbl_bin, "SSBL binary")
    ota_flash_prog_code = read_binary_or_exit(args.ota_flash_prog_bin, "ota_flash_prog binary")
    crypto_bin = read_binary_or_exit(args.crypto_bin, "crypto.bin")

    print("SSBL / manufacturing image QSPI configuration:")
    qspi_enabled, cr, dcr, ccr = resolve_qspi_config(args)
    print()

    print("--- SSBL key ---")
    pem_path = args.pem or prompt_path("Path to the SSBL private key PEM file")
    print(f"\nSigning SSBL ({len(ssbl_code)} bytes) + OTA commit app "
          f"({len(ota_flash_prog_code)} bytes) + crypto.bin ({len(crypto_bin)} bytes) "
          f"together with '{pem_path}' ...")
    try:
        signature_flash, ssbl_n, total_verified_len = sign_boot_bundle(
            ssbl_code, ota_flash_prog_code, crypto_bin, pem_path)
    except Exception as e:
        print(f"Error signing boot bundle: {e}", file=sys.stderr)
        sys.exit(1)
    print("  Boot bundle signature OK (self-verified)")

    try:
        boot_region = build_boot_region(ssbl_code, ota_flash_prog_code, crypto_bin,
                                         signature_flash, total_verified_len,
                                         qspi_enabled, cr, dcr, ccr)
    except Exception as e:
        print(f"Error building boot region: {e}", file=sys.stderr)
        sys.exit(1)

    print("Running end-to-end readback self-check on the assembled boot bundle "
          "(SSBL + OTA commit app + crypto.bin)...")
    boot_ok = readback_verify_boot_region(boot_region, ssbl_n, RSA_E)
    print("  ✓ Boot bundle readback PASSED." if boot_ok else "  ✗ Boot bundle readback FAILED.")
    if not boot_ok:
        print("Error: readback verification of the boot bundle FAILED -- refusing to "
              "write anything.", file=sys.stderr)
        sys.exit(1)
    print()

    slot_offset = SLOT_A_OFFSET if slot == "A" else SLOT_B_OFFSET
    if slot_offset < len(boot_region):
        print(f"Error: Slot {slot} offset overlaps the boot bundle", file=sys.stderr)
        sys.exit(1)
    if len(signed_app_image) > SLOT_MAX_SIZE:
        print(f"Error: signed app image is {len(signed_app_image)} bytes, exceeds "
              f"the {SLOT_MAX_SIZE}-byte slot size -- refusing to write anything.",
              file=sys.stderr)
        sys.exit(1)

    reserved_region = build_reserved_region(slot, fw_version_bytes)
    total_size = slot_offset + len(signed_app_image)

    image = bytearray([PAD_BYTE] * total_size)
    image[0:len(boot_region)] = boot_region
    image[RESERVED_REGION_OFFSET:RESERVED_REGION_OFFSET + len(reserved_region)] = reserved_region
    image[slot_offset:slot_offset + len(signed_app_image)] = signed_app_image

    with open(args.output, "wb") as f:
        f.write(bytes(image))

    print(f"\nWrote manufacturing image: {args.output}")
    print(f"  SSBL region       : 0x000000-0x{SSBL_MAX_SIZE:06X} "
          f"(header, size@0x00, sig@0x100 -- covers SSBL+OTA commit app+crypto.bin)")
    print(f"  OTA commit app    : 0x{OTA_COMMIT_APP_FLASH_OFFSET:06X}-0x{CRYPTO_BIN_OFFSET:06X} "
          f"({len(ota_flash_prog_code)} bytes used of {OTA_COMMIT_APP_MAX_SIZE})")
    print(f"  crypto.bin        : 0x{CRYPTO_BIN_OFFSET:06X}-0x{RESERVED_REGION_OFFSET:06X} "
          f"({len(crypto_bin)} bytes used of {CRYPTO_BIN_MAX_SIZE})")
    print(f"  QSPI config       : {'ENABLED (0x%08X)' % QSPI_CONFIG_ENABLED if qspi_enabled else 'DISABLED (0xFFFFFFFF)'}")
    if qspi_enabled:
        print(f"    CR  = 0x{cr:08X}")
        print(f"    DCR = 0x{dcr:08X}")
        print(f"    CCR = 0x{ccr:08X}")
    print(f"  Reserved metadata : 0x{RESERVED_REGION_OFFSET:06X} "
          f"(active_slot={'SLOT_A_ACTIVE' if slot == 'A' else 'SLOT_B_ACTIVE'}, "
          f"current_version={fw_version_bytes[0]}.{fw_version_bytes[1]}.{fw_version_bytes[2]}, revoked_count=0)")
    print(f"  App (Slot {slot})      : 0x{slot_offset:06X}, {len(signed_app_image)} / {SLOT_MAX_SIZE} bytes")
    print(f"  Total image size  : {len(image)} bytes")


def do_secure(args):
    is_manufacturing = prompt_manufacturing_flag(args)
    print()

    slot = prompt_slot(args)
    print()

    app_code = read_binary_or_exit(args.app_bin, "app binary")

    print("Building and signing application image...")
    try:
        signed_app_image, fw_version_bytes, app_n, root_n = build_signed_app_slot_image(
            app_code, is_ota=not is_manufacturing
        )
    except Exception as e:
        print(f"Error building signed app image: {e}", file=sys.stderr)
        sys.exit(1)
    print(f"  Signed app image: {len(signed_app_image)} bytes")
    print()

    print("Running end-to-end readback self-check on the assembled app/cert image...")
    cert_ok, fw_ok, crc_ok = readback_verify_app_image(
        signed_app_image, root_n, app_n, RSA_E, is_ota=not is_manufacturing
    )
    print("  ✓ Cert readback PASSED." if cert_ok else "  ✗ Cert readback FAILED.")
    print("  ✓ Firmware readback PASSED." if fw_ok else "  ✗ Firmware readback FAILED.")
    if not is_manufacturing:
        print("  ✓ CRC readback PASSED." if crc_ok else "  ✗ CRC readback FAILED.")
    if not cert_ok or not fw_ok or (not is_manufacturing and not crc_ok):
        print("Error: readback verification of the app/cert image FAILED -- refusing to "
              "write anything.", file=sys.stderr)
        sys.exit(1)
    print()

    if not is_manufacturing:
        write_ota_app_image(args, slot, signed_app_image)
        return

    build_manufacturing_image(args, slot, signed_app_image, fw_version_bytes)


def main():
    parser = argparse.ArgumentParser(
        description="Build non-secure or secure flash image")
    parser.add_argument("--secure-boot", choices=["y", "n"], default="n")
    parser.add_argument("--app-bin", required=True, help="Path to app code.bin")
    parser.add_argument("--output", required=True, help="Path to write combined output .bin")

    parser.add_argument("--non-interactive", action="store_true")
    parser.add_argument("--qspi-config", choices=["y", "n"], default=None,
                         help="Preset the 'Custom QSPI Configuration needed?' answer "
                              "so the prompt is skipped. 'n' disables QSPI config "
                              "with no prompt at all. 'y' enables it and skips only "
                              "the yes/no question -- CR/DCR/CCR are still prompted "
                              "for unless --cr/--dcr/--ccr are also given.")
    parser.add_argument("--cr", type=lambda x: int(x, 16), default=None)
    parser.add_argument("--dcr", type=lambda x: int(x, 16), default=None)
    parser.add_argument("--ccr", type=lambda x: int(x, 16), default=None)

    parser.add_argument("--ssbl-bin", help="Path to SSBL code.bin (required for manufacturing)")
    parser.add_argument("--ota-flash-prog-bin",
                         help="Path to ota_flash_prog code.bin (required for manufacturing; "
                              "part of the signed boot bundle)")
    parser.add_argument("--crypto-bin", help="Path to crypto.bin (required for manufacturing)")
    parser.add_argument("--slot", choices=["A", "B"], default=None)
    parser.add_argument("--manufacturing", choices=["y", "n"], default=None)
    parser.add_argument("--pem", default=None)

    args = parser.parse_args()

    if args.secure_boot == "y":
        do_secure(args)
    else:
        do_nonsecure(args)


if __name__ == "__main__":
    main()