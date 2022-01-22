/* Name: tftp.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ethboot
 * Function: tftp implementation and flasher
 * Version: 0.2 tftp / flashing functional
 */

#include "tftp.h"
#include "debug.h"
#include "debug_tftp.h"
#include "net.h"
#include "neteeprom.h"
#include "serial.h"
#include "spi.h"
#include "util.h"
#include "validate.h"
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/** Opcode?: tftp operation is unsupported. The bootloader only supports 'put' */
#define TFTP_OPCODE_ERROR_LEN 12
const unsigned char tftp_opcode_error_packet[] PROGMEM = "\0\5"
														 "\0\4"
														 "Opcode?";
/** Full: Binary image file is larger than the available space. */
#define TFTP_FULL_ERROR_LEN 9
const unsigned char tftp_full_error_packet[] PROGMEM = "\0\5"
													   "\0\3"
													   "Full";
/** General catch-all error for unknown errors */
#define TFTP_UNKNOWN_ERROR_LEN 10
const unsigned char tftp_unknown_error_packet[] PROGMEM = "\0\5"
														  "\0\0"
														  "Error";
/** Invalid image file: Doesn't look like a binary image file */
#define TFTP_INVALID_IMAGE_LEN 23
const unsigned char tftp_invalid_image_packet[] PROGMEM = "\0\5"
														  "\0\0"
														  "Invalid image file";

uint8_t tftpFlashing;
uint8_t tftpInitError;

#ifndef TFTP_RANDOM_PORT
uint16_t tftpTransferPort;
#endif

uint16_t lastPacket = 0, highPacket = 0;

static void sockInit(uint16_t port)
{
	DBG_TFTP(tracePGMlnTftp(mDebugTftp_SOCK); tracenum(port);)

	uint8_t read_val = 0;
	uint8_t err_count = 0;

	spiWriteReg(REG_S3_CR, S3_W_CB, CR_CLOSE);
	do
	{
		// wait for command to complete
		read_val = spiReadReg(REG_S3_CR, S3_R_CB);
		err_count++;

		if(err_count > 128)
		{
			DBG_TFTP(tracePGMlnTftp(mDebugTftp_SOCKCLOSEERR); puthex(read_val);)

			tftpInitError = TRUE;
			return;
		}
	} while(read_val != SOCK_CLOSED);

	err_count = 0;

	do
	{
		// Write interrupt
		spiWriteReg(REG_S3_IR, S3_W_CB, 0xFF);
		// Write mode
		spiWriteReg(REG_S3_MR, S3_W_CB, MR_UDP);
		// Write TFTP Port
		spiWriteWord(REG_S3_PORT0, S3_W_CB, port);
		// Open Socket
		spiWriteReg(REG_S3_CR, S3_W_CB, CR_OPEN);

		while(spiReadReg(REG_S3_CR, S3_R_CB))
			; // Wait for command to complete

		err_count++;

		if(err_count > 128)
		{
			DBG_TFTP(tracePGMlnTftp(mDebugTftp_SOCKOPENERR); puthex(read_val);)

			tftpInitError = TRUE;
			return;
		}
		read_val = spiReadReg(REG_S3_SR, S3_R_CB);
	} while(read_val != SOCK_UDP); // If socket correctly opened continue

	DBG_TFTP(tracePGMlnTftp(mDebugTftp_SOCKDONE);)
}

#if(DEBUG_TFTP > 0)
static uint8_t processPacket(uint16_t packetSize)
#else
static uint8_t processPacket(void)
#endif
{
	uint8_t buffer[TFTP_PACKET_MAX_SIZE];
	uint16_t readPointer;
	address_t writeAddr;
	// Transfer entire packet to RAM
	uint8_t* bufPtr = buffer;
	uint16_t count;

	DBG_TFTP(tracePGMlnTftp(mDebugTftp_START); tracenum(packetSize);

			 if(packetSize >= 0x800) { tracePGMlnTftp(mDebugTftp_OVFL); }

			 DBG_BTN(button();))

	// Read data from chip to buffer

	// Use uint16_t overflow from 0xFFFF to 0x10000 to follow WIZnet internal pointer
	readPointer = spiReadWord(REG_S3_RX_RD0, S3_R_CB);

	DBG_TFTP_EX(tracePGMlnTftp(mDebugTftp_RPTR); tracenum(readPointer);)

	for(count = TFTP_PACKET_MAX_SIZE; count--;)
	{
		DBG_TFTP_EX(if((count == TFTP_PACKET_MAX_SIZE - 1) || (count == 0)) {
			tracePGMlnTftp(mDebugTftp_RPOS);
			tracenum(readPointer);
		})

#if defined(__WIZ_W5500__)
		*bufPtr++ = spiReadReg(readPointer++, S3_RXBUF_CB);
		// W5500 have [automaticly] offset address mapping between the read pointer
		//  to the physical address
#else
		*bufPtr++ = spiReadReg(S3_map_readPointer_to_phy_address(readPointer++), 0);
		// W5100 & W5200 should read from the physical address
		// address (relative to the base address) calculate by masking with the buffer size
#endif
	}

	spiWriteWord(REG_S3_RX_RD0, S3_W_CB, readPointer); // Write back new pointer
	spiWriteReg(REG_S3_CR, S3_W_CB, CR_RECV);

	while(spiReadReg(REG_S3_CR, S3_R_CB))
		;

	DBG_TFTP_EX(tracePGMlnTftp(mDebugTftp_BLEFT); tracenum(spiReadWord(REG_S3_RX_RSR0, S3_R_CB));)

	// Dump packet
	DBG_TFTP_EX(bufPtr = buffer; tracePGM(mDebugTftp_NEWLINE);

				for(count = TFTP_PACKET_MAX_SIZE / 2; count--;) {
					uint16_t val = *bufPtr++;
					val |= (*bufPtr++) << 8;
					tracenum(val);

					if((count % 8) == 0 && count != 0)
						tracePGM(mDebugTftp_NEWLINE);
					else
						putch(0x20); // Print space
				})

	// Set up return IP address and port
	uint8_t i;

	for(i = 0; i < 6; i++)
		spiWriteReg(REG_S3_DIPR0 + i, S3_W_CB, buffer[i]);

	DBG_TFTP(tracePGMlnTftp(mDebugTftp_RADDR);)

	// Parse packet
	uint16_t tftpDataLen = (buffer[6] << 8) + buffer[7];
	uint16_t tftpOpcode = (buffer[8] << 8) + buffer[9];
	uint16_t tftpBlock = (buffer[10] << 8) + buffer[11];

	DBG_TFTP(tracePGMlnTftp(mDebugTftp_BLOCK); tracenum(tftpBlock); tracePGM(mDebugTftp_OPCODE);
			 tracenum(tftpOpcode); tracePGM(mDebugTftp_DLEN);
			 tracenum(tftpDataLen - (TFTP_OPCODE_SIZE + TFTP_BLOCKNO_SIZE));)

	if((tftpOpcode == TFTP_OPCODE_DATA) &&
	   ((tftpBlock > MAX_ADDR / 0x200) || (tftpBlock < highPacket) || (tftpBlock > highPacket + 1)))
		tftpOpcode = TFTP_OPCODE_UKN;

	if(tftpDataLen > (0x200 + TFTP_OPCODE_SIZE + TFTP_BLOCKNO_SIZE))
		tftpOpcode = TFTP_OPCODE_UKN;

	uint8_t returnCode = ERROR_UNKNOWN;
	uint16_t packetLength;

	switch(tftpOpcode)
	{
		case TFTP_OPCODE_RRQ: // Read request
			DBG_TFTP(tracePGMlnTftp(mDebugTftp_OPRRQ);)
			break;

		case TFTP_OPCODE_WRQ: // Write request
			// Valid WRQ -> reset timer
			resetTick();

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_OPWRQ);)

			// Flagging image as invalid since the flashing process has started
			eeprom_write_byte((uint8_t*)NETEEPROM_IMG_STAT, NETEEPROM_IMG_BAD_VALUE);

#if defined(RANDOM_TFTP_DATA_PORT)
			sockInit((buffer[4] << 8) | ~buffer[5]); // Generate a 'random' TID (RFC1350)
#else
			sockInit(tftpTransferPort);
#endif

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_NPORT);
#if defined(RANDOM_TFTP_DATA_PORT)
					 tracenum((buffer[4] << 8) | (buffer[5] ^ 0x55));
#else
					 tracenum(tftpTransferPort);
#endif
			)

			lastPacket = highPacket = 0;
			returnCode = ACK; // Send back acknowledge for packet 0
			break;

		case TFTP_OPCODE_DATA:
			// Valid Data Packet -> reset timer
			resetTick();

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_OPDATA);)

			packetLength = tftpDataLen - (TFTP_OPCODE_SIZE + TFTP_BLOCKNO_SIZE);
			lastPacket = tftpBlock;
#if defined(RAMPZ)
			writeAddr = (((address_t)((tftpBlock - 1) / 0x80) << 16) |
						 ((address_t)((tftpBlock - 1) % 0x80) << 9));
#else
			writeAddr =
				(address_t)((address_t)(tftpBlock - 1) << 9); // Flash write address for this block
#endif

			if((writeAddr + packetLength) > MAX_ADDR)
			{
				// Flash is full - abort with an error before a bootloader overwrite occurs
				// Application is now corrupt, so do not hand over.

				DBG_TFTP(tracePGMlnTftp(mDebugTftp_FULL);)

				returnCode = ERROR_FULL;
			}
			else
			{
				DBG_TFTP(tracePGMlnTftp(mDebugTftp_WRADDR); traceadd(writeAddr);)

				uint8_t* pageBase = buffer + (UDP_HEADER_SIZE + TFTP_OPCODE_SIZE +
											  TFTP_BLOCKNO_SIZE); // Start of block data
				uint16_t offset = 0; // Block offset

				// Set the return code before packetLength gets rounded up
				if(packetLength < TFTP_DATA_SIZE)
				{
					returnCode = FINAL_ACK;
				}
				else
				{
					returnCode = ACK;
				}

				// Round up packet length to a full flash sector size
				while(packetLength % SPM_PAGESIZE)
				{
					packetLength++;
				}

				DBG_TFTP(tracePGMlnTftp(mDebugTftp_PLEN); tracenum(packetLength);)

				// Flash packets
				uint16_t writeValue;
				for(offset = 0; offset < packetLength;)
				{
					writeValue = (pageBase[offset]) | (pageBase[offset + 1] << 8);
					boot_page_fill(writeAddr + offset, writeValue);

					DBG_TFTP_EX(if((offset == 0) || ((offset == (packetLength - 2)))) {
						tracePGMlnTftp(mDebugTftp_WRITE);
						tracenum(writeValue);
						tracePGM(mDebugTftp_OFFSET);
						tracenum(writeAddr + offset);
					})

					offset += 2;

					if(offset % SPM_PAGESIZE == 0)
					{
						boot_page_erase(writeAddr + offset - SPM_PAGESIZE);
						boot_spm_busy_wait();
						boot_page_write(writeAddr + offset - SPM_PAGESIZE);
						boot_spm_busy_wait();
#if defined(RWWSRE)
						// Reenable read access to flash
						boot_rww_enable();
#endif
					}
				}

				if(returnCode == FINAL_ACK)
				{
					// Flash is complete
					// Hand over to application

					DBG_TFTP(tracePGMlnTftp(mDebugTftp_DONE);)

					// Flag the image as valid since we received the last packet
					eeprom_write_byte((uint8_t*)NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VALUE);
				}
			}

			break;

		// Acknowledgment
		case TFTP_OPCODE_ACK:

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_OPACK);)

			break;

		// Error signal
		case TFTP_OPCODE_ERROR:

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_OPERR);)

			/* FIXME: Resetting might be needed here too */
			break;

		default:
			DBG_TFTP(tracePGMlnTftp(mDebugTftp_INVOP); tracenum(tftpOpcode);)

#if defined(RANDOM_TFTP_DATA_PORT)
			sockInit((buffer[4] << 8) | ~buffer[5]); // Generate a 'random' TID (RFC1350)
#else
			sockInit(tftpTransferPort);
#endif
			/* FIXME: This is where the tftp server should be resetted.
			 * It can be done by reinitializig the tftpd or
			 * by resetting the device. I should find out which is best...
			 * Right now it is being done by resetting the timer if we have a
			 * data packet. */
			// Invalid - return error
			returnCode = ERROR_INVALID;
			break;
	}

	return (returnCode);
}

static void sendResponse(uint16_t response)
{
	uint8_t txBuffer[100];
	uint8_t* txPtr = txBuffer;
	uint8_t packetLength;
	uint16_t writePointer;

	switch(response)
	{
		default:

		case ERROR_UNKNOWN:
			// Send unknown error packet
			packetLength = TFTP_UNKNOWN_ERROR_LEN;
#if(FLASHEND > 0x10000)
			memcpy_PF(txBuffer, PROGMEM_OFFSET + (uint32_t)(uint16_t)tftp_unknown_error_packet,
					  packetLength);
#else
			memcpy_P(txBuffer, tftp_unknown_error_packet, packetLength);
#endif
			break;

		case ERROR_INVALID:
			// Send invalid opcode packet
			packetLength = TFTP_OPCODE_ERROR_LEN;
#if(FLASHEND > 0x10000)
			memcpy_PF(txBuffer, PROGMEM_OFFSET + (uint32_t)(uint16_t)tftp_opcode_error_packet,
					  packetLength);
#else
			memcpy_P(txBuffer, tftp_opcode_error_packet, packetLength);
#endif
			break;

		case ERROR_FULL:
			// Send unknown error packet
			packetLength = TFTP_FULL_ERROR_LEN;
#if(FLASHEND > 0x10000)
			memcpy_PF(txBuffer, PROGMEM_OFFSET + (uint32_t)(uint16_t)tftp_full_error_packet,
					  packetLength);
#else
			memcpy_P(txBuffer, tftp_full_error_packet, packetLength);
#endif
			break;

		case ACK:
			if(lastPacket > highPacket)
			{
				highPacket = lastPacket;
			}

			DBG_TFTP(tracePGMlnTftp(mDebugTftp_SACK);)
			/* fallthrough */

		case FINAL_ACK:

			DBG_TFTP(if(response == FINAL_ACK) tracePGMlnTftp(mDebugTftp_SFACK);)

			packetLength = 4;
			*txPtr++ = TFTP_OPCODE_ACK >> 8;
			*txPtr++ = TFTP_OPCODE_ACK & 0xff;
			// lastPacket is block code
			*txPtr++ = lastPacket >> 8;
			*txPtr = lastPacket & 0xff;
			break;
	}

	txPtr = txBuffer;

	// Use uint16_t overflow from 0xFFFF to 0x10000 to follow WIZnet internal pointer
	writePointer = spiReadWord(REG_S3_TX_WR0, S3_R_CB);

	while(packetLength--)
	{
#if defined(__WIZ_W5500__)
		spiWriteReg(writePointer++, S3_TXBUF_CB, *txPtr++);
		// W5500 have [automaticly] offset address mapping between the write pointer
		//  to the physical address
#else
		spiWriteReg(S3_map_writePointer_to_phy_address(writePointer++), S3_TXBUF_CB, *txPtr++);
		// W5100 & W5200 should write to the physical address
		// address (relative to the base address) calculate by masking with the buffer size
#endif
	}

	spiWriteWord(REG_S3_TX_WR0, S3_W_CB, writePointer); // Write back new pointer

	spiWriteReg(REG_S3_CR, S3_W_CB, CR_SEND);

	while(spiReadReg(REG_S3_CR, S3_R_CB))
		;

	DBG_TFTP(tracePGMlnTftp(mDebugTftp_RESP);)
}

/**
 * Initializes the network controller
 */
void tftpInit(void)
{
	tftpInitError = FALSE;

	// Open socket
	sockInit(TFTP_PORT);

#if defined(RANDOM_TFTP_DATA_PORT)
#else
	if(eeprom_read_byte((uint8_t*)NETEEPROM_SIG_3) == NETEEPROM_SIG_3_VALUE)
	{
		tftpTransferPort = ((eeprom_read_byte((uint8_t*)NETEEPROM_PORT + 1) << 8) +
							eeprom_read_byte((uint8_t*)NETEEPROM_PORT));
	}
	else
	{
		tftpTransferPort = TFTP_DATA_PORT;
	}
#endif

	if(tftpInitError)
	{
		DBG_TFTP(tracePGMlnTftp(mDebugTftp_INIT_FAILED);)
	}
	else
	{
		DBG_TFTP(tracePGMlnTftp(mDebugTftp_INIT);
#if defined(RANDOM_TFTP_DATA_PORT)
#else
				 tracePGMlnTftp(mDebugTftp_PORT); tracenum(tftpTransferPort);
#endif
				 ) // End DBG_TFTP
	}
}

/**
 * Looks for a connection
 */
uint8_t tftpPoll(void)
{
	uint16_t packetSize;
	uint16_t prev = 0;
	uint8_t response = ACK;

	// Get the size of the recieved data
	packetSize = spiReadWord(REG_S3_RX_RSR0, S3_R_CB);

	// Reading twice (recommended by Wiznet, https://github.com/Wiznet/ioLibrary_Driver)
	while(packetSize != prev)
	{
		_delay_ms(TFTP_PACKET_DELAY);

		prev = packetSize;
		packetSize = spiReadWord(REG_S3_RX_RSR0, S3_R_CB);
	}

	if(packetSize)
	{
		tftpFlashing = TRUE;

		// Process Packet and get TFTP response code
#if(DEBUG_TFTP > 0)
		response = processPacket(packetSize);
#else
		response = processPacket();
#endif
		// Send the response
		sendResponse(response);
	}

	if(response == FINAL_ACK)
	{
		spiWriteReg(REG_S3_CR, S3_W_CB, CR_CLOSE);
		// Tftp Complete
		return (0);
	}

	// Tftp continues
	return (1);
}
