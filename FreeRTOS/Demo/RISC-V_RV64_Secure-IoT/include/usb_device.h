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
 * @file usb_device.h
 * @brief Public interface and API declarations for the software-driven USB 
 * Low-Speed peripheral driver.
 * @details Defines the core transaction data structures, physical pin 
 * mapping matrices, configuration masks, and functional prototypes required 
 * to operate the low-level GPIO bit-bang USB communication layer.
 * @version 1.1
 * @authors Vishwajith N S (vishwajith@mindgrovetech.in)
 *          Narasimha R V (narasimha@mindgrovetech.in)
 * @date 02-06-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * | Date       | Version | Modified by           | Description                   
 * |-----------|---------|-----------------------|------------------------------
 * |28-10-2025 | 1.0     | Vishwajith N S        | Initial release.    
 * |02-06-2026 | 1.1     | Narasimha R V         | Complete reworking of TX/RX  
 * |           |         |                       | functions, code optimization,
 * |           |         |                       | and full documentation.
 * -----------------------------------------------------------------------------
 * 
 */

#ifndef USB_DEVICE_BITBANG_H
#define USB_DEVICE_BITBANG_H

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                           SHARED GLOBAL VARIABLES                           */
/* -------------------------------------------------------------------------- */

/**
 * @defgroup USB_Driver_Globals Shared Driver State Variables
 * @brief Global state buffers and status flags shared between the physical layer engines and the protocol layer.
 * @{
 */

/**
 * @brief Synchronization status flag indicating a completed packet reception.
 *
 * @details This flag is set to `1` by the @ref USB_Device_RX_IRQHandler immediately 
 * after the hardware lines exit the sampling loop and an End-of-Packet (EOP) sequence 
 * is confirmed. 
 *
 * @note The application or higher-level protocol layer must poll this flag (or utilize a 
 * blocking getter function) to detect incoming traffic, and **must clear it back to `0`** * after processing the buffer payload.
 */
extern volatile uint8_t dev_rx_packet_ready;

/**
 * @brief Bus condition monitoring flag indicating a hardware-driven USB Bus Reset event.
 *
 * @details Set to `1` inside the @ref USB_Device_RX_IRQHandler if a Single-Ended Zero (SE0) 
 * condition—where both D+ and D- lines remain driven low simultaneously—persists beyond 
 * the calibrated timeout window threshold of 14,000 sampling delay iterations.
 *
 * @warning When this flag is set, the upstream host has requested a reset sequence. 
 * The device stack state machines must clear internal memory buffers, reset token tracking 
 * counters, and prepare for re-enumeration.
 */
extern volatile uint8_t dev_reset_flag;

/**
 * @brief Primary data staging array for decoded, byte-aligned USB packet data.
 *
 * @details This staging matrix is populated on-the-fly by the receiver engine.
 *  The array employs an implicit packet formatting architecture:
 * - **`dev_rx_buffer[0]`**: Contains the absolute length of the captured packet 
 *   payload in bytes.
 * - **`dev_rx_buffer[1]` up to `dev_rx_buffer[n]`**: Contains the sequential,
 *   validated payload bytes 
 * (e.g., SYNC, PID, Data field blocks, and appended CRC segments).
 *
 * @note Bit-unstuffing modifications and NRZI inversions are fully executed before data is committed into this buffer block.
 */
extern uint8_t dev_rx_buffer[64];

/**
 * @brief Diagnostic tracking buffer capturing the raw, undecoded logical stream of the D- line.
 *
 * @details Stores physical line states sequentially at each sampling interval
 *  *prior* to running NRZI decoding or bit-unstuffing algorithms. This array
 *  acts as an isolated diagnostic trace buffer, enabling logic validation,
 *  timing verification, and bit-stream analysis during testing.
 */
extern uint8_t dev_raw_bits[200];

/** @} */ // End of USB_Driver_Globals defgroup

/* -------------------------------------------------------------------------- */
/*                                USB PID TYPES                                */
/* -------------------------------------------------------------------------- */

/**
 * @enum USB_PID
 * @brief USB Packet Identifier (PID) values.
 *
 * Only the lower 4 bits are significant; the upper 4 bits are
 * transmitted as the bitwise complement.
 */
typedef enum {
    PID_OUT   = 1,   /**< OUT token */
    PID_ACK   = 2,   /**< ACK handshake */
    PID_DATA0 = 3,   /**< DATA0 packet */
    PID_PING  = 4,   /**< PING token */
    PID_SOF   = 5,   /**< Start Of Frame */
    PID_NYET  = 6,   /**< NYET handshake */
    PID_DATA2 = 7,   /**< DATA2 packet */
    PID_SPLIT = 8,   /**< SPLIT token */
    PID_IN    = 9,   /**< IN token */
    PID_NAK   = 10,  /**< NAK handshake */
    PID_DATA1 = 11,  /**< DATA1 packet */
    PID_PRE   = 12,  /**< PRE token */
    PID_ERR   = 12,  /**< ERR (same encoding as PRE) */
    PID_SETUP = 13,  /**< SETUP token */
    PID_STALL = 14,  /**< STALL handshake */
    PID_MDATA = 15   /**< MDATA packet */
} USB_Device_PID;

/**
 * @defgroup USB_Driver_Structs USB Driver Data Structures
 * @brief Data containers and configuration matrices for handling USB packets and physical interfaces.
 * @{
 */

/**
 * @struct USB_Device_Packet
 * @brief Unified transaction data structure representing a logical USB packet.
 * @details This structure encapsulates all metadata and raw payload 
 * requirements necessary to parse received incoming sequences or to stage out 
 * transaction framing (Token, Data, Handshake, or Special frames) prior to
 * serial bit-shifting.
 */
typedef struct {
    uint8_t pid; /**< Packet Identifier (PID). Specifies the packet type (e.g., SETUP, IN, OUT, DATA0, ACK). Only the lower 4 bits represent the raw PID value. */
    
    uint16_t frame_number; /**< 11-bit Frame Number. Utilized exclusively by Start-of-Frame (SOF) token packets; ignored for standard transaction structures. */
    
    uint8_t *data; /**< Array reference mapping directly to the data transaction payload. Ignored for Token and Handshake packet types. */
    
    uint8_t endpoint; /**< 4-bit Target Endpoint identifier. Parsed out of or built into standard USB Token packets. Valid range is 0 to 15. */
    
    uint8_t addr; /**< 7-bit Device Address field. Unique peripheral identification value assigned dynamically by the host during enumeration. Valid range is 0 to 127. */
    
    uint8_t data_length; /**< Absolute size of the targeted active payload segment pointed to by @ref data, specified in bytes. */
} USB_Device_Packet;

/**
 * @struct USB_Device_PinConfig
 * @brief Runtime physical hardware pin assignment configuration matrix.
 * @details Passed into the initialization engine to configure the dynamic 
 * software-defined transceiver. This allows the driver to map its bit-bang
 * execution logic over flexible, user-selected GPIO pins without altering or
 * recompiling underlying driver libraries.
 */
typedef struct {
    uint8_t pin_dplus; /**< Target GPIO pin number mapped to handle the differential USB D+ line. */
    
    uint8_t pin_dminus; /**< Target GPIO pin number mapped to handle the differential USB D- line. */
    
    uint8_t pin_button; /**< Hardware GPIO identifier allocated for manual user-trigger events, boot triggers, or physical state overrides. */
} USB_Device_PinConfig;

/** @} */ // End of USB_Driver_Structs defgroup

/* -------------------------------------------------------------------------- */
/*                             FUNCTION PROTOTYPES                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initializes the USB Host peripheral interface and registers the receive ISR.
 *
 * This function configures the physical-layer GPIO pins and hardware interrupt architecture 
 * required to receive USB packets. To ensure thread safety and prevent race conditions 
 * while modifying the Platform-Level Interrupt Controller (PLIC) configurations, it atomically 
 * shields the operation by disabling and re-enabling Machine External Interrupts.
 *
 * ### Initialization Sequence Workflow
 * 1. **Interrupt Shielding (Disable):** Clears the `MEIE` bit (Bit 11) in the RISC-V `mie` 
 * (Machine Interrupt Enable) Control and Status Register (CSR) using an atomic `csrrc` instruction.
 * 2. **ISR Registration:** Assigns the internal receive handler as the servicing routine for the 
 * `DPLUS` interrupt line.
 * 3. **Hardware Line Setup:** Configures both `DMINUS` and `DPLUS` pins as digital inputs.
 * 4. **Trigger Configuration:** Enables edge/level interrupt triggering explicitly on the 
 * `DPLUS` line to capture incoming USB packet resume/start signaling.
 * 5. **Priority Assignment:** Elevates the `DPLUS` interrupt vector to the highest priority 
 * to minimize interrupt latency during high-speed edge transitions.
 * 6. **Interrupt Shielding (Enable):** Restores and sets the `MEIE` bit back in the `mie` 
 * register using an atomic `csrrs` instruction to resume normal interrupt handling.
 *
 * @note Disabling the global `MEIE` bit during configuration guarantees that the PLIC register 
 * modifications and handler assignments are fully atomic relative to other external hardware events.
 * @return void
 */

void USB_Device_Init_Hardware(void);

/**
 * @brief Assigns and initializes the logical GPIO pins for the software USB engine.
 *
 * @details This function maps the user-defined hardware pin allocations provided at runtime
 * to internal, driver-scoped global configuration variables. Additionally, it 
 * calculates and caches a combined network bitmask (`net_mask`) containing both 
 * D+ and D- lines. This bitmask allows the transmission and reception engines to 
 * perform atomic bitwise read, write, and toggle operations across the differential 
 * bus in a single instruction cycle.
 *
 * @param[in] pins Pointer to a constant @ref USB_Device_PinConfig structure containing the target 
 * hardware pin mappings for the D+, D-, and physical button interfaces.
 *
 * @return 
 * @ref SUCCESS if pin configuration is valid and applied, 
 * @ref EINVAL if any pin index is out of the valid range (0-41).
 *
 * @note This function executes a native RISC-V memory fence instruction (`fence`) prior to 
 * exiting. This guarantees complete memory ordering, forcing all modified pin properties 
 * and bitmasks to be globally visible across all CPU processing pipeline stages before 
 * the hardware initialization or interrupt handlers are activated.
 */
uint16_t USB_Device_Init_Pins(USB_Device_PinConfig *pins);

/**
 * @brief Serializes a packet structure into a raw byte-staged stream for
 * hardware transmission.
 *
 * @details This function handles the USB physical layer (PHY) packet framing.
 * It translates a high-level structured input packet (such as tokens, data
 * payloads, and handshakes) into a sequential, byte-aligned temporary buffer
 * (`data_temp`). It automatically handles the generation of hardware-level
 * synchronization markers, complement-verified Packet Identifiers (PIDs), and
 * error-checking blocks (CRC5/CRC16).
 *
 * ### Generated Memory Byte Layouts
 * **DATA Packet Structure:**
 * @code
 * Byte 0        Byte 1        Byte 2 ... (N+1)    Byte (N+2)    Byte (N+3)
 * +------------+------------+--------------------+------------+------------+
 * | SYNC(0x80) | PID Check  |    Payload Data    | CRC16 Low  | CRC16 High |
 * +------------+------------+--------------------+------------+------------+
 * @endcode
 *
 * **TOKEN Packet Structure (SETUP, IN, OUT):**
 * @code
 * Byte 0        Byte 1        Byte 2               Byte 3
 * +------------+------------+--------------------+-------------------------+
 * | SYNC(0x80) | PID Check  | ADDR[6:0] | ENDP[0]| ENDP[3:1] | CRC5[4:0]   |
 * +------------+------------+--------------------+-------------------------+
 * @endcode
 *
 * **HANDSHAKE Packet Structure (ACK, NAK, STALL):**
 * @code
 * Byte 0        Byte 1
 * +------------+------------+
 * | SYNC(0x80) | PID Check  |
 * +------------+------------+
 * @endcode
 *
 * ### Token Field Bit-Mapping Architecture
 * For token-type packets, the 11-bit payload required by the `CRC5_USB_Bytes`
 * algorithm is compressed and written into `data_temp[2]` and `data_temp[3]`
 * according to the following layout configuration:
 * - **Byte 2 (`data_temp[2]`):**
 * - Bits `[6:0]`: Device Address (`USB_Device_Packet->addr`).
 * - Bit  `[7]`:   Endpoint Bit 0 (`USB_Device_Packet->endpoint & 0x01`).
 * - **Byte 3 (`data_temp[3]`):**
 * - Bits `[2:0]`: Endpoint Bits 3:1 (`(USB_Device_Packet->endpoint >> 1) & 0x07`).
 * - Bits `[7:3]`: Calculated 5-bit CRC (`(crc5 & 0x1F) << 3`).
 *
 * @param[in] USB_Device_Packet Pointer to the source configuration structure defining
 * the PID type, payload constraints, data length, and endpoint routing
 * parameters.
 *
 * @return SUCCESS  Success; packet framing was successfully parsed and
 * shifted out to the bus.
 * @return EFAULT   Execution Fault; an undefined or unsupported PID
 * configuration was supplied.
 *
 * @note The internal length tracker calculates transaction bounds using 
 * **bits** instead of bytes to directly accommodate the lower-level branchless
 *  NRZI bit-stuffing shift registers.
 *
 */
uint16_t USB_Device_Transmit_Packet(USB_Device_Packet *packet);

#endif /* USB_DEVICE_BITBANG_H */
