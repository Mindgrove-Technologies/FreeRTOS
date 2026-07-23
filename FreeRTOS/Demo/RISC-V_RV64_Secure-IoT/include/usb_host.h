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
 * @file usb_host.h
 * @brief Public interface and API declarations for the software-driven USB 
 * Low-Speed peripheral driver.
 * @details Defines the core transaction data structures, physical pin 
 * mapping matrices, configuration masks, and functional prototypes required 
 * to operate the low-level GPIO bit-bang USB communication layer.
 * @version 1.1
 * @authors Narasimha R V (narasimha@mindgrovetech.in)
 * @date 02-06-2026
 *
 * @section History
 * -----------------------------------------------------------------------------
 * Date       | Version | Modified by           | Description                   
 * -----------|---------|-----------------------|-------------------------------
 * 02-06-2026 | 1.0     | Narasimha R V         | Initial release.                  
 * -----------------------------------------------------------------------------
 * 
 */

#ifndef USB_HOST_BITBANG_H
#define USB_HOST_BITBANG_H

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
 * @details This flag is set to `1` by the @ref USB_Host_RX_IRQHandler immediately 
 * after the hardware lines exit the sampling loop and an End-of-Packet (EOP) sequence 
 * is confirmed. 
 *
 * @note The application or higher-level protocol layer must poll this flag (or utilize a 
 * blocking getter function) to detect incoming traffic, and **must clear it back to `0`** * after processing the buffer payload.
 */
extern volatile uint8_t host_rx_packet_ready;

/**
 * @brief Bus condition monitoring flag indicating a hardware-driven USB Bus Reset event.
 *
 * @details Set to `1` inside the @ref USB_Host_RX_IRQHandler if a Single-Ended Zero (SE0) 
 * condition—where both D+ and D- lines remain driven low simultaneously—persists beyond 
 * the calibrated timeout window threshold of 14,000 sampling delay iterations.
 *
 * @warning When this flag is set, the upstream host has requested a reset sequence. 
 * The device stack state machines must clear internal memory buffers, reset token tracking 
 * counters, and prepare for re-enumeration.
 */
extern volatile uint8_t host_reset_flag;

/**
 * @brief Primary data staging array for decoded, byte-aligned USB packet data.
 *
 * @details This staging matrix is populated on-the-fly by the receiver engine.
 *  The array employs an implicit packet formatting architecture:
 * - **`host_rx_buffer[0]`**: Contains the absolute length of the captured packet 
 *   payload in bytes.
 * - **`host_rx_buffer[1]` up to `host_rx_buffer[n]`**: Contains the sequential,
 *   validated payload bytes 
 * (e.g., SYNC, PID, Data field blocks, and appended CRC segments).
 *
 * @note Bit-unstuffing modifications and NRZI inversions are fully executed before data is committed into this buffer block.
 */
extern uint8_t host_rx_buffer[64];

/**
 * @brief Diagnostic tracking buffer capturing the raw, undecoded logical stream of the D- line.
 *
 * @details Stores physical line states sequentially at each sampling interval
 *  *prior* to running NRZI decoding or bit-unstuffing algorithms. This array
 *  acts as an isolated diagnostic trace buffer, enabling logic validation,
 *  timing verification, and bit-stream analysis during testing.
 */
extern uint8_t host_raw_bits[200];

/**
 * @brief Status flag indicating a Keep-Alive (KA) pulse transmission event.
 *
 * @details This flag is used to track the synchronization of low-speed USB
 * Keep-Alive markers (End-of-Packet pulses) generated during idle periods. 
 * It is typically set to `1` inside periodic hardware timer events—such as the
 * @ref on_machine_timer_interrupt—immediately after the software successfully 
 * completes the SE0-to-J-state transition sequence on the bus.
 *
 * @note This flag can be used by the application layer to synchronize outbound
 * traffic. To prevent data corruption or collisions with the periodic hardware
 * Keep-Alive signal, initiate your transmit transactions exactly after the 
 * `ka_sent` flag is asserted. The application must clear this flag back to `0`
 * after detecting the event.
 */
extern volatile uint8_t ka_sent;

/** @} */ // End of USB_Driver_Globals defgroup

/* -------------------------------------------------------------------------- */
/*                                USB PID TYPES                                */
/* -------------------------------------------------------------------------- */

/**
 * @enum USB_Host_PID
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
} USB_Host_PID;

/**
 * @defgroup USB_Driver_Structs USB Driver Data Structures
 * @brief Data containers and configuration matrices for handling USB packets and physical interfaces.
 * @{
 */

/**
 * @struct USB_Host_Packet
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
} USB_Host_Packet;

/**
 * @struct USB_Host_PinConfig
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
} USB_Host_PinConfig;

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

void USB_Host_Init_Hardware(void);

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
 * @param[in] pins Pointer to a constant @ref USB_Host_PinConfig structure containing the target 
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
uint16_t USB_Host_Init_Pins(USB_Host_PinConfig *pins);

/**
 * @brief Serializes a packet structure into a raw byte-staged stream for hardware transmission.
 *
 * This function handles USB physical layer (PHY) packet framing. It translates high-level 
 * structured inputs (such as tokens, payloads, and handshakes) into a sequential byte 
 * buffer (`data_temp`). It automatically handles the generation of hardware-level synchronization 
 * markers, complement-verified Packet Identifiers (PIDs), and error-checking blocks (CRC5/CRC16).
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
 * For token-type packets, the 11-bit payload required by the `crc5_usb_bytes` algorithm is 
 * compressed and written into `data_temp[2]` and `data_temp[3]` according to the following 
 * layout configuration:
 * - **Byte 2 (`data_temp[2]`):**
 * - Bits `[6:0]`: Device Address (`USB_Host_Packet->addr`).
 * - Bit  `[7]`:   Endpoint Bit 0 (`USB_Host_Packet->endpoint & 0x01`).
 * - **Byte 3 (`data_temp[3]`):**
 * - Bits `[2:0]`: Endpoint Bits 3:1 (`(USB_Host_Packet->endpoint >> 1) & 0x07`).
 * - Bits `[7:3]`: Calculated 5-bit CRC (`(crc5 & 0x1F) << 3`).
 *
 * @param[in]  USB_Host_Packet       Pointer to the source configuration structure defining the PID type, 
 * payload constraints, data length, and endpoint routing.
 * @param[out] data_temp    Pointer to the continuous, pre-allocated destination array where 
 * the framed wire-bytes are serialized.
 * @param[out] total_length Pointer to an assigned variable populated upon exit with the exact 
 * cumulative length of the stream measured strictly in **bits**.
 *
 * @return SUCCESS      Success; packet framing successfully parsed and committed to memory.
 * @return EFAULT       Execution Fault; an undefined or unsupported PID configuration was supplied.
 *
 * @note The final output assigned to `total_length` tracks bits instead of bytes to directly 
 * accommodate lower-level NRZI and bit-stuffing shift registers.
 * @see crc5_usb_bytes
 * @see crc16_usb_byte
 */
uint16_t USB_Host_TX_Stream_Generator(USB_Host_Packet *packet, uint8_t *data_temp, uint16_t *total_length);

/**
 * @brief Transmits a bitstream using NRZI encoding and branchless bit-stuffing via GPIO.
 *
 * This function processes a raw input bit buffer, applies USB-style NRZI 
 * encoding (toggling on a '0' bit, maintaining state on a '1' bit), and 
 * performs branchless bit-stuffing. If six consecutive '1's are detected,
 * an automatic '0' bit isstuffed into the stream to guarantee a transition
 * for clock recovery.
 *
 * ### Execution Workflow
 * 1. **Initialization:** Disables the associated PLIC GPIO interrupts and
 * configures the `DPLUS` and `DMINUS` pins as digital outputs.
 * 2. **Bit Extraction & Bit-Stuffing Loop:** Iterates bit-by-bit over the 
 * input data. Utilizes branchless math (multiplications and set-if-equal 
 * sequences) to calculate consecutive '1' counts, conditionally flip the 
 * state for stuffing, and compute the final pin output values.
 * 3. **Hardware Toggling:** Leverages the atomic GPIO Toggle register to
 * simultaneously flip `DPLUS` and `DMINUS` paths, neutralizing potential
 * jitter from sequential writes.
 * 4. **EOP Generation:** After the payload stream finishes, sends an 
 * End-of-Packet (EOP) 
 * sequence comprising 2 bit-times of Single-Ended Zero (SE0), followed by an
 *+ idle 'J' state.
 * 5. **Teardown:** Reconfigures the pins back to inputs before returning.
 *
 * ### Stack Frame Architecture (Frame Pointer `s0`)
 * Because this is a naked function, the stack allocation (-64 bytes) and variable locations 
 * are manually managed. The mapping is structured as follows:
 * | Stack Offset | Variable Name        | Data Type   | Description |
 * | :---         | :---                 | :---        | :---        |
 * | `-56(s0)`    | `input`              | `uint8_t*`  | Pointer to the transmission buffer base |
 * | `-58(s0)`    | `input_len_bits`     | `uint16_t`  | Total bit budget to push out |
 * | `-18(s0)`    | `prev_nrzi`          | `uint8_t`   | Tracks the last calculated NRZI bit state |
 * | `-19(s0)`    | `ones_count`         | `uint8_t`   | Tracks consecutive '1' bits (0 to 6) |
 * | `-24(s0)`    | `prev_state`         | `uint32_t`  | Tracks the previous GPIO bitmask state |
 * | `-28(s0)`    | `next_state`         | `uint32_t`  | Holds the target state calculation |
 * | `-32(s0)`    | `change_in_state`    | `uint32_t`  | Resulting XOR bitmask written to the Toggle Register |
 * | `-36(s0)`    | `bit`                | `uint32_t`  | The current scalar bit sliced from memory |
 * | `-38(s0)`    | `in_bit`             | `uint16_t`  | Global bit index counter |
 * | `-17(s0)`    | `stuffing_flag`      | `uint8_t`   | High (1) if `ones_count == 6`, zero otherwise |
 *
 * @param[in] input          Pointer to the byte array containing the serialized bit data.
 * @param[in] input_len_bits The precise number of bits to transmit (from Sync field through CRC).
 * @return void
 */

void USB_Host_TX_Bits_Branchless (const uint8_t *input,
                                  uint16_t input_len_bits);

/**
 * @brief Initializes and starts the periodic 1 ms USB Keep-Alive (KA) signaling timer.
 *
 * This function programs the hardware timer to expire precisely after a 1 ms window has elapsed. 
 * It acts as the initial bootstrap trigger that schedules the first execution of the 
 * Machine Timer Interrupt Service Routine.
 *
 * ### Behavioral Context & Hardware Sequence
 * Every 1 ms, the hardware timer expires and vectors to the ISR. This periodic heartbeat 
 * executes a deterministic USB physical-layer bus sequence to maintain host connection:
 * 1. **Bus Reset/Clear:** Forces both `DPLUS` and `DMINUS` signals to a known baseline state 
 * using the GPIO peripheral registers.
 * 2. **Single-Ended Zero (SE0) State:** Drives the lines to generate an SE0 condition, holding 
 * the state for a calibrated hardware delay to satisfy USB bus specifications.
 * 3. **Bus Idle & State Update:** Transitions the line state back to operational mode and flags 
 * the global state variable `enumerated = 1`, confirming a completed link configuration cycle.
 * 4. **Periodic Rescheduling:** Automatically updates the hardware timer compare register 
 * forward by another 1 ms window, establishing a continuous, recurring interrupt loop.
 *
 * @note This function must be called exactly once during the USB peripheral initialization phase 
 * to kickstart the hardware's 1 ms time-base engine.
 * @return void
 */
void USB_Host_Start_KA(void);

/**
 * @brief Polls for a newly received USB packet with a hardware timeout guard.
 *
 * Blocks execution until the global `flag` is asserted by the RX ISR or 
 * the timeout ticks expire. Extracts the payload length and applies
 * a bitmask (`& 0xF`) to isolate the 4-bit Packet Identifier (PID)
 * from the raw receive buffer.
 *
 * @param[out] out_len Pointer to a variable populated with the received packet length.
 * @param[out] out_pid Pointer to a variable populated with the filtered 4-bit PID.
 *
 * @return SUCCESS    Packet successfully received and processed.
 * @return ETIMEDOUT  The polling loop expired before a packet was flagged.
 *
 * @note Automatically clears the global tracking variable (`flag = 0`) before returning.
 *
 * ### Usage Example
 * @code
 * // Call after transmission to capture the device handshake response
 * if (usb_receive_packet(&length, &pid) == SUCCESS) 
 * {
 * if (pid == PID_ACK) return SUCCESS; // Transaction verified
 * } 
 * else 
 * {
 * printf("RX Timeout\r\n"); // Device did not respond, trigger retry
 * }
 * @endcode
 */

uint16_t USB_Host_Receive_Packet(uint16_t *out_len, uint8_t *out_pid);

/**
 * @brief Forces a physical USB Bus Reset sequence by driving single-ended signaling.
 *
 * @details This function executes a hardware-level USB bus reset sequence by
 * directly manipulating the GPIO memory-mapped I/O (MMIO) registers. The reset
 * timing sequence operates as follows:
 * 1. Configures both D+ and D- pins as electrical push-pull outputs by writing
 * to the GPIO Output Enable register (`0x40200`).
 * 2. Actively drives both data lines low to establish a strict Single-Ended
 * Zero (SE0) state via the GPIO Set register (`0x40210`) and GPIO Clear 
 * register (`0x40218`).
 * 3. Holds the SE0 state for a stable duration of 30 milliseconds, surpassing
 * the standard USB specification requirement (minimum 10ms for a reset).
 * 4. Releases the bus back to the idle J-state configuration by pulling the
 * D- line high while maintaining D+ low, ensuring proper bus recovery.
 *
 * @param[in] pins Pointer to the @ref USB_Host_PinConfig structure containing the 
 * physical target hardware pin assignments for the D+ and D- lines.
 *
 * @return void
 *
 * @note This function uses explicit blocking millisecond delays (`Delay_MS`) to
 * guarantee compliance with structural USB low-speed signaling timing budgets.
 * @warning Calling this function disrupts all active traffic and forces the 
 * upstream host or attached device to re-initiate the enumeration process.
 */
void USB_Host_Bus_Reset(USB_Host_PinConfig *pins);

#endif /* USB_HOST_BITBANG_H */