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
 * Project                   : MGS2401 SoC
 * @file errors.h
 * @brief  Header file defining standard error codes for drivers.
 * @details  This file defines macros for standard and driver specific error 
 *           codes used in drivers for consistent error handling.
 * @version 1.0
 * @authors Harini P (harinip@mindgrovetech.in)
 * @date 10-09-2024
 * 
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 10-09-2024 | 1.0     | Harini P              | Initial release.              
 * -----------------------------------------------------------------------------
 */

#ifndef BSP_INCLUDE_ERRORS_H_
#define BSP_INCLUDE_ERRORS_H_

/**
 * @defgroup ERROR_CODES Error Code Macros
 *
 * @brief Standard error codes used across all drivers and modules.
 *
 * @{
 */

/** @brief Operation completed successfully. */
#define SUCCESS          0U

/** @brief Operation not permitted. */
#define EPERM            1U

/** @brief No such file or directory. */
#define ENOENT           2U

/** @brief No such process. */
#define ESRCH            3U

/** @brief Interrupted system call. */
#define EINTR            4U

/** @brief I/O error. */
#define EIO              5U

/** @brief No such device or address. */
#define ENXIO            6U

/** @brief Argument list too long. */
#define E2BIG            7U

/** @brief Exec format error. */
#define ENOEXEC          8U

/** @brief Bad file number. */
#define EBADF            9U

/** @brief No child processes. */
#define ECHILD          10U

/** @brief Try again. */
#define EAGAIN          11U

/** @brief Out of memory. */
#define ENOMEM          12U

/** @brief Permission denied. */
#define EACCES          13U

/** @brief Bad address. */
#define EFAULT          14U

/** @brief Block device required. */
#define ENOTBLK         15U

/** @brief Device or resource busy. */
#define EBUSY           16U

/** @brief File exists. */
#define EEXIST          17U

/** @brief Cross-device link. */
#define EXDEV           18U

/** @brief No such device. */
#define ENODEV          19U

/** @brief Not a directory. */
#define ENOTDIR         20U

/** @brief Is a directory. */
#define EISDIR          21U

/** @brief Invalid argument. */
#define EINVAL          22U

/** @brief File table overflow. */
#define ENFILE          23U

/** @brief Too many open files. */
#define EMFILE          24U

/** @brief Not a typewriter. */
#define ENOTTY          25U

/** @brief Text file busy. */
#define ETXTBSY         26U

/** @brief File too large. */
#define EFBIG           27U

/** @brief No space left on device. */
#define ENOSPC          28U

/** @brief Illegal seek. */
#define ESPIPE          29U

/** @brief Read-only file system. */
#define EROFS           30U

/** @brief Too many links. */
#define EMLINK          31U

/** @brief Broken pipe. */
#define EPIPE           32U

/** @brief Math argument out of domain of function. */
#define EDOM            33U

/** @brief Math result not representable. */
#define ERANGE          34U

/** @brief Resource deadlock would occur. */
#define EDEADLK         35U

/** @brief File name too long. */
#define ENAMETOOLONG    36U

/** @brief No record locks available. */
#define ENOLCK          37U

/** @brief Invalid system call number. */
#define ENOSYS          38U

/** @brief Directory not empty. */
#define ENOTEMPTY       39U

/** @brief Too many symbolic links encountered. */
#define ELOOP           40U

/** @brief Operation would block (same as EAGAIN). */
#define EWOULDBLOCK     EAGAIN

/** @brief No message of desired type. */
#define ENOMSG          42U

/** @brief Identifier removed. */
#define EIDRM           43U

/** @brief Channel number out of range. */
#define ECHRNG          44U

/** @brief Level 2 not synchronized. */
#define EL2NSYNC        45U

/** @brief Level 3 halted. */
#define EL3HLT          46U

/** @brief Level 3 reset. */
#define EL3RST          47U

/** @brief Link number out of range. */
#define ELNRNG          48U

/** @brief Protocol driver not attached. */
#define EUNATCH         49U

/** @brief No CSI structure available. */
#define ENOCSI          50U

/** @brief Level 2 halted. */
#define EL2HLT          51U

/** @brief Invalid exchange. */
#define EBADE           52U

/** @brief Invalid request descriptor. */
#define EBADR           53U

/** @brief Exchange full. */
#define EXFULL          54U

/** @brief No anode. */
#define ENOANO          55U

/** @brief Invalid request code. */
#define EBADRQC         56U

/** @brief Invalid slot. */
#define EBADSLT         57U

/** @brief Bad font file format. */
#define EBFONT          58U

/** @brief Device not a stream. */
#define ENOSTR          59U

/** @brief No data available. */
#define ENODATA         60U

/** @brief Timer expired. */
#define ETIME           61U

/** @brief Out of streams resources. */
#define ENOSR           62U

/** @brief Machine is not on the network. */
#define ENONET          63U

/** @brief Package not installed. */
#define ENOPKG          64U

/** @brief Object is remote. */
#define EREMOTE         65U

/** @brief Link has been severed. */
#define ENOLINK         66U

/** @brief Advertise error. */
#define EADV            67U

/** @brief Srmount error. */
#define ESRMNT          68U

/** @brief Communication error on send. */
#define ECOMM           69U

/** @brief Protocol error. */
#define EPROTO          70U

/** @brief Multihop attempted. */
#define EMULTIHOP       71U

/** @brief RFS specific error. */
#define EDOTDOT         72U

/** @brief Not a data message. */
#define EBADMSG         73U

/** @brief Value too large for defined data type. */
#define EOVERFLOW       74U

/** @brief Name not unique on network. */
#define ENOTUNIQ        75U

/** @brief File descriptor in bad state. */
#define EBADFD          76U

/** @brief Remote address changed. */
#define EREMCHG         77U

/** @brief Cannot access a needed shared library. */
#define ELIBACC         78U

/** @brief Accessing a corrupted shared library. */
#define ELIBBAD         79U

/** @brief .lib section in a.out corrupted. */
#define ELIBSCN         80U

/** @brief Attempting to link in too many shared libraries. */
#define ELIBMAX         81U

/** @brief Cannot exec a shared library directly. */
#define ELIBEXEC        82U

/** @brief Illegal byte sequence. */
#define EILSEQ          83U

/** @brief Interrupted system call should be restarted. */
#define ERESTART        84U

/** @brief Streams pipe error. */
#define ESTRPIPE        85U

/** @brief Too many users. */
#define EUSERS          86U

/** @brief Socket operation on non-socket. */
#define ENOTSOCK        87U

/** @brief Destination address required. */
#define EDESTADDRREQ    88U

/** @brief Message too long. */
#define EMSGSIZE        89U

/** @brief Protocol wrong type for socket. */
#define EPROTOTYPE      90U

/** @brief Protocol not available. */
#define ENOPROTOOPT     91U

/** @brief Protocol not supported. */
#define EPROTONOSUPPORT 92U

/** @brief Socket type not supported. */
#define ESOCKTNOSUPPORT 93U

/** @brief Operation not supported on transport endpoint. */
#define EOPNOTSUPP      94U

/** @brief Protocol family not supported. */
#define EPFNOSUPPORT    95U

/** @brief Address family not supported by protocol. */
#define EAFNOSUPPORT    96U

/** @brief Address already in use. */
#define EADDRINUSE      97U

/** @brief Cannot assign requested address. */
#define EADDRNOTAVAIL   98U

/** @brief Network is down. */
#define ENETDOWN        99U

/** @brief Network is unreachable. */
#define ENETUNREACH     100U

/** @brief Network dropped connection because of reset. */
#define ENETRESET       101U

/** @brief Software caused connection abort. */
#define ECONNABORTED    102U

/** @brief Connection reset by peer. */
#define ECONNRESET      103U

/** @brief No buffer space available. */
#define ENOBUFS         104U

/** @brief Transport endpoint is already connected. */
#define EISCONN         105U

/** @brief Transport endpoint is not connected. */
#define ENOTCONN        106U

/** @brief Cannot send after transport endpoint shutdown. */
#define ESHUTDOWN       107U

/** @brief Too many references: cannot splice. */
#define ETOOMANYREFS    108U

/** @brief Connection timed out. */
#define ETIMEDOUT       109U

/** @brief Connection refused. */
#define ECONNREFUSED    110U

/** @brief Host is down. */
#define EHOSTDOWN       111U

/** @brief No route to host. */
#define EHOSTUNREACH    112U

/** @brief Operation already in progress. */
#define EALREADY        113U

/** @brief Operation now in progress. */
#define EINPROGRESS     114U

/** @brief Stale file handle. */
#define ESTALE          115U

/** @brief Structure needs cleaning. */
#define EUCLEAN         116U

/** @brief Not a XENIX named type file. */
#define ENOTNAM         117U

/** @brief No XENIX semaphores available. */
#define ENAVAIL         118U

/** @brief Is a named type file. */
#define EISNAM          119U

/** @brief Remote I/O error. */
#define EREMOTEIO       120U

/** @brief Quota exceeded. */
#define EDQUOT          121U

/** @brief No medium found. */
#define ENOMEDIUM       122U

/** @brief Wrong medium type. */
#define EMEDIUMTYPE     123U

/** @brief Operation Canceled. */
#define ECANCELED       124U

/** @brief Required key not available. */
#define ENOKEY          125U

/** @brief Key has expired. */
#define EKEYEXPIRED     126U

/** @brief Key has been revoked. */
#define EKEYREVOKED     127U

/** @brief Key was rejected by service. */
#define EKEYREJECTED    128U

/** @brief Owner died. */
#define EOWNERDEAD      129U

/** @brief State not recoverable. */
#define ENOTRECOVERABLE 130U

/** @brief Operation not possible due to RF-kill. */
#define ERFKILL         131U

/** @brief Memory page has hardware error. */
#define EHWPOISON       132U

/** @} */


/**
 * @defgroup I2C_ERROR_CODES I2C error codes macros
 *
 * @brief Error codes specific to I2C driver operations.
 *
 * @{
 */

/** @brief No acknowledgement received for data. */
#define ENOACK          133U

/** @brief No acknowledgement received for slave address. */
#define ENOACKDEV       141U

/** @} */


/**
 * @defgroup UART_ERROR_CODES UART error codes macros 
 *
 * @brief Error codes specific to UART driver operations.
 *
 * @{
 */

/** @brief Invalid Transmit/Receive mode. */
#define ETRMODE         142U

/** @brief Software buffer is full. */
#define ECBUFFULL       143U

/** @brief Software buffer is empty. */
#define ECBUFEMPTY      144U

/** @brief Built-in hardware buffer is empty. */
#define EBUFEMPTY       145U

/** @} */


/**
 * @defgroup QSPI_ERROR_CODES QSPI error codes macros
 *
 * @brief Error codes specific to QSPI driver operations.
 *
 * @{
 */

/** @brief Length of data field exceeded. */
#define ELENEXCEED      146U

/** @} */


/**
 * @defgroup SPI_ERROR_CODES SPI error codes macros
 *
 * @brief Error codes specific to SPI driver operations.
 *
 * @{
 */

/** @brief Invalid clock mode. */
#define ECLKMODE        147U

/** @brief Invalid frequency (too high or too low). */
#define EINFREQ         148U

/** @} */

/**
 * @defgroup PRO_IO_ERROR_CODES PRO IO error codes macros
 *
 * @brief Error codes specific to PRO IO driver operations.
 *
 * @{
 */

/** @brief Invalid PRO IO Number */
#define EBUFFERNUM       150U

/** @brief Invalid Bit selection */
#define EBITSEL          151U

/** @brief Invalid Data size*/
#define EINVALSIZE       152U

/** @brief Invalid Mode selection*/
#define EINVALMODE       153U

/** @} */

/**
 * @defgroup CRYPTO_ERROR_CODES Cryptographic error codes macros
 *
 * @brief Error codes related to cryptographic operations.
 *
 * @{
 */

/** @brief Authentication tag verification failed. */
#define EAUTHTAGMISMATCH    149U

/** @} */

/**
 * @defgroup OTP_ERROR_CODES OTP write error codes macros
 *
 * @brief Error codes specific to OTP operations.
 *
 * @{
 */

/** @brief OTP Write operation failed. */
#define EOTPWRITE      154U

/** @brief Invalid OTP LOCK value. */
#define EOTPLOCK      155U
/** @} */

/**
 * @defgroup INTERNAL_ERROR_CODES Internal Kernel Error Codes
 *
 * @brief Error codes used internally by the kernel; should not be seen by user programs.
 *
 * @{
 */

/** @brief Restart the system call. */
#define ERESTARTSYS           512U

/** @brief Restart the system call if not interrupted. */
#define ERESTARTNOINTR        513U

/** @brief Restart if no handler is present. */
#define ERESTARTNOHAND        514U

/** @brief No ioctl command. */
#define ENOIOCTLCMD           515U

/** @brief Restart by calling sys_restart_syscall. */
#define ERESTARTRESTARTBLOCK 516U

/** @brief Driver requests probe retry. */
#define EPROBEDEFER          517U

/** @brief Open found a stale dentry. */
#define EOPENSTALE            518U

/** @} */


/**
 * @defgroup NFSV3_ERROR_CODES NFSv3 error codes macros.
 *
 * @brief Error codes specific to the NFSv3 protocol.
 *
 * @{
 */

/** @brief Illegal NFS file handle. */
#define EBADHANDLE            521U

/** @brief Update synchronization mismatch. */
#define ENOTSYNC              522U

/** @brief Cookie is stale. */
#define EBADCOOKIE            523U

/** @brief Operation is not supported. */
#define ENOTSUPP              524U

/** @brief Buffer or request is too small. */
#define ETOOSMALL             525U

/** @brief An untranslatable server error occurred. */
#define ESERVERFAULT          526U

/** @brief Type not supported by server. */
#define EBADTYPE              527U

/** @brief Request initiated, but will not complete before timeout. */
#define EJUKEBOX              528U

/** @brief IOCB queued; completion event will follow. */
#define EIOCBQUEUED           529U

/** @} */

/**
 * @defgroup legacy_defs Legacy Definitions
 * @brief Legacy macro definitions (soon to be deprecated).
 * @{
 */

/** @brief Instruction not supported (soon to be deprecated). */
#define ENOINST 999U

/** @} */


#endif  // BSP_INCLUDE_ERRORS_H_
