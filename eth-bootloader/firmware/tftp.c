/* Name: tftp.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ethboot
 * Function: tftp implementation and flasher
 * Version: 0.1b tftp / flashing functional
 */

#include <avr/boot.h>
#include <avr/pgmspace.h>

#include "net.h"
#include "w5100_reg.h"
#include "tftp.h"
#include "main.h"
#include "validate.h"

#include "debug.h"

// TFTP message types
#define ERROR_UNKNOWN     0
#define ERROR_INVALID     1
#define ACK               2
#define ERROR_FULL        3
// Like an ACK, but for the final data packet.
#define FINAL_ACK         4
#define INVALID_IMAGE     5

#define TFTP_DATA_SIZE    512

// For 328 with 2Kword bootloader
#define MAX_ADDR          0x7000

#define UDP_HEADER_SIZE   8
#define TFTP_OPCODE_SIZE  2
#define TFTP_BLOCKNO_SIZE 2
#define TFTP_MAX_PAYLOAD  512
#define TFTP_PACKET_MAX_SIZE (UDP_HEADER_SIZE+TFTP_OPCODE_SIZE+  \
                              TFTP_BLOCKNO_SIZE+TFTP_MAX_PAYLOAD)

uint16_t lastPacket = 0;
//uint8_t downloadStarted=0;

/* Opcode?: tftp operation is unsupported. The bootloader only supports 'put' */
#define TFTP_OPCODE_ERROR_LEN 12
const unsigned char tftp_opcode_error_packet[] PROGMEM = "\12"   "\0\5"  "\0\0"  "Opcode?";

/* Full: Binary image file is larger than the available space. */
#define TFTP_FULL_ERROR_LEN 9
const unsigned char tftp_full_error_packet[] PROGMEM = "\x09"  "\0\5"  "\0\3"  "Full";

/* General catch-all error for unknown errors */
#define TFTP_UNKNOWN_ERROR_LEN 10
const unsigned char tftp_unknown_error_packet[] PROGMEM = "\10"   "\0\5"  "\0\0"  "Error";

/* Invalid image file: Doesn't look like a binary image file */
#define TFTP_INVALID_IMAGE 23
const unsigned char tftp_invalid_image_packet[] PROGMEM = "\23"   "\0\5"  "\0\0"  "Invalid image file";


uint8_t processPacket(uint16_t packetSize) {
  uint8_t buffer[TFTP_PACKET_MAX_SIZE];
  uint16_t readPointer;
  uint16_t writeAddr;
  // Transfer entire packet to RAM
  uint8_t* bufPtr = buffer;
  uint16_t count;

#ifdef _DEBUG_TFTP
  traceln("Tftp: ----");
  traceln("Tftp: Starting processing packet of size ");
  tracenum(packetSize);
  if(packetSize>=0x800) traceln("Tftp: Overflow");
  //  step();
#endif

  readPointer = netReadWord(REG_S3_RX_RD0);
#ifdef _DEBUG_TFTP
  traceln("Tftp: readPointer at position ");
  tracenum(readPointer);
#endif
  if (readPointer==0) readPointer += S3_RX_START;
  for (count=TFTP_PACKET_MAX_SIZE; count--;) {
#ifdef _DEBUG_TFTP
    if ((count==TFTP_PACKET_MAX_SIZE-1) || (count==0)) {
      traceln("Tftp: Reading from position ");
      tracenum(readPointer);
    }
#endif
    *bufPtr++ = netReadReg(readPointer++);
    if (readPointer==S3_RX_END) readPointer = S3_RX_START;
  }
  netWriteWord(REG_S3_RX_RD0, readPointer);     // Write back new pointer
  netWriteReg(REG_S3_CR, CR_RECV);
  while(netReadReg(REG_S3_CR));
#ifdef _DEBUG_TFTP
  traceln("Tftp: Bytes left to read ");
  tracenum(netReadWord(REG_S3_RX_RSR0));
#endif

#ifdef _DEBUGMORE_TFTP
  // Dump packet
  bufPtr = buffer;
  traceln("");
  for (count=TFTP_PACKET_MAX_SIZE/2; count--;) {
    uint16_t val = *bufPtr++;
    val |= (*bufPtr++) << 8;
    tracenum(val);
    if((count%8)==0 && count!=0) traceln("");
    else trace(" ");
  }
#endif

#ifdef _DEBUG
  traceln("Tftp: Setting return address");
#endif

  // Set up return IP address and port
  uint8_t i;
  for (i=0; i<6; i++) netWriteReg(REG_S3_DIPR0+i, buffer[i]);

  // Parse packet
  uint16_t tftpDataLen = (buffer[6]<<8) + buffer[7];
  uint16_t tftpOpcode = (buffer[8]<<8) + buffer[9];
  uint16_t tftpBlock = (buffer[10]<<8) + buffer[11];
#ifdef _DEBUG
  traceln("Tftp: This is block ");
  tracenum(tftpBlock);
  trace(" with opcode ");
  tracenum(tftpOpcode);
  trace(" and data length ");
  tracenum(tftpDataLen - (TFTP_OPCODE_SIZE+TFTP_BLOCKNO_SIZE));
#endif

  uint8_t returnCode = ERROR_UNKNOWN;
  uint16_t packetLength;


  switch (tftpOpcode) {
    case TFTP_OPCODE_RRQ: // Read request
    default:
#ifdef _DEBUG
//      traceln("Tftp: Invalid opcode ");
//      tracenum(tftpOpcode);
#endif
      // Invalid - return error
      returnCode = ERROR_INVALID;
      break;

    case TFTP_OPCODE_WRQ: // Write request
#ifdef _DEBUG
      traceln("Tftp: Write request");
#endif
      netWriteReg(REG_S3_CR, CR_RECV);
      netWriteReg(REG_S3_CR, CR_CLOSE);
      do {
        netWriteReg(REG_S3_MR, MR_UDP);
        netWriteReg(REG_S3_CR, CR_OPEN);
        //netWriteWord(REG_S3_PORT0, (buffer[4]<<8) | ~buffer[5]); // Generate a 'random' TID (RFC1350)
        netWriteWord(REG_S3_PORT0, TFTP_RANDOM_PORT);   // Not any more
        if (netReadReg(REG_S3_SR) != SOCK_UDP)
          netWriteReg(REG_S3_CR, CR_CLOSE);
      } while (netReadReg(REG_S3_SR) != SOCK_UDP);
#ifdef _DEBUG
      traceln("Tftp: Changed to port ");
      //tracenum((buffer[4]<<8) | (buffer[5]^0x55));
      tracenum(TFTP_RANDOM_PORT);
#endif
      lastPacket = 0;
      returnCode = ACK; // Send back acknowledge for packet 0
      break;

    case TFTP_OPCODE_DATA:
      packetLength = tftpDataLen - (TFTP_OPCODE_SIZE+TFTP_BLOCKNO_SIZE);
      lastPacket = tftpBlock;
      writeAddr = (tftpBlock-1) << 9; // Flash write address for this block
#ifdef _DEBUG
      traceln("Tftp: Data for block ");
      tracenum(lastPacket);
#endif

      if ((writeAddr+packetLength) > MAX_ADDR) {
        // Flash is full - abort with an error before a bootloader overwrite occurs
        // Application is now corrupt, so do not hand over.
#ifdef _DEBUG
    	traceln("Tftp: Flash is full");
#endif
        returnCode = ERROR_FULL;
      } else {
#ifdef _DEBUG
        traceln("Tftp: Writing data from address ");
        tracenum(writeAddr);
#endif

        uint8_t* pageBase = buffer + (UDP_HEADER_SIZE+TFTP_OPCODE_SIZE+TFTP_BLOCKNO_SIZE); // Start of block data
        uint16_t offset = 0; // Block offset

        // Round up packet length to a full flash sector size
        while (packetLength % SPM_PAGESIZE) packetLength++;
#ifdef _DEBUG
        traceln("Tftp: Packet length adjusted to ");
        tracenum(packetLength);
#endif
        if (writeAddr == 0) {
          // First sector - validate
          if (!validImage(pageBase)) {
            returnCode = INVALID_IMAGE;
            /* FIXME: Validity checks. Small programms (under 512 bytes?) don't
             * have the the JMP sections and that is why app.bin was failing.
             * When flashing big binaries is fixed, uncomment the break below.*/
            #ifndef _DEBUG
            break;
            #endif
          }
        }

        // Flash packets
        for (offset=0; offset < packetLength;) {
          uint16_t writeValue = (pageBase[offset]) | (pageBase[offset+1]<<8);
          boot_page_fill(writeAddr+offset, writeValue);
#ifdef _DEBUGMORE
          if ((offset==0)||((offset==(packetLength-2)))) {
            traceln("Tftp: Writing ");
            tracenum(writeValue);
            trace(" at offset ");
            tracenum(writeAddr+offset);
          }
#endif
          offset += 2;
          if (offset % SPM_PAGESIZE == 0) {
            boot_page_erase(writeAddr + offset - SPM_PAGESIZE);
            boot_spm_busy_wait();
            boot_page_write(writeAddr + offset - SPM_PAGESIZE);
            boot_spm_busy_wait();
            boot_rww_enable();
          }
        }

        if (packetLength<TFTP_DATA_SIZE) {
          // Flash is complete
          // Hand over to application
#ifdef _DEBUG
       	  traceln("Tftp: Flash is complete");
#endif
          returnCode = FINAL_ACK;
        } else {
          returnCode = ACK;
        }
      }
      break;

    // Acknowledgment
    case TFTP_OPCODE_ACK:
#ifdef _DEBUG
      traceln("Tftp: Acknowledge");
#endif
      break;

    // Error signal
    case TFTP_OPCODE_ERROR:
#ifdef _DEBUG
     traceln("Tftp: Error");
#endif
      break;

  }
  return(returnCode);
}

void sendResponse(uint16_t response) {
  uint8_t txBuffer[100];
  uint8_t *txPtr = txBuffer;
  uint8_t packetLength;
  uint16_t writePointer;

  writePointer = netReadWord(REG_S3_TX_WR0) + S3_TX_START;
  switch (response) {
    default:
    case ERROR_UNKNOWN:
      // Send unknown error packet
      packetLength = TFTP_UNKNOWN_ERROR_LEN;
      memcpy_P(txBuffer,tftp_unknown_error_packet,packetLength);
      break;
    case ERROR_INVALID:
      // Send invalid opcode packet
      packetLength = TFTP_OPCODE_ERROR_LEN;
      memcpy_P(txBuffer,tftp_opcode_error_packet,packetLength);
      break;
    case ERROR_FULL:
      // Send unknown error packet
      packetLength = TFTP_FULL_ERROR_LEN;
      memcpy_P(txBuffer,tftp_full_error_packet,packetLength);
      break;
    case ACK:
#ifdef _DEBUG
      traceln("Tftp: Sent ACK");
      /* no break */
#endif
    case FINAL_ACK:
#ifdef _DEBUG
      if(response == FINAL_ACK) {
      traceln("Tftp: Sent Final ACK ");
      tracenum(lastPacket);
      }
#endif
      packetLength = 4;
      *txPtr++ = TFTP_OPCODE_ACK >> 8;
      *txPtr++ = TFTP_OPCODE_ACK & 0xff;
      // lastPacket is block code
      *txPtr++ = lastPacket >> 8;
      *txPtr = lastPacket & 0xff;
      break;
  }

  txPtr = txBuffer;
  while (packetLength--) {
    netWriteReg(writePointer++, *txPtr++);
    if (writePointer==S3_TX_END) writePointer = S3_TX_START;
  }
  netWriteWord(REG_S3_TX_WR0, writePointer-S3_TX_START);
  netWriteReg(REG_S3_CR, CR_SEND);
  while (netReadReg(REG_S3_CR));
#ifdef _DEBUG
  traceln("Tftp: Response sent");
#endif
}


/**
 * Initializes the network controller
 */
void tftpInit() {
  // Open socket
  do {
    // Write TFTP Port
    netWriteWord(REG_S3_PORT0, TFTP_PORT);
    // Write mode
    netWriteReg(REG_S3_MR, MR_UDP);
    // Open Socket
    netWriteReg(REG_S3_CR, CR_OPEN);
    // Read Status
    if (netReadReg(REG_S3_SR) != SOCK_UDP)
      // Close Socket if it wasn't initialized correctly
      netWriteReg(REG_S3_CR, CR_CLOSE);
  // If socket correctly opened continue
  } while (netReadReg(REG_S3_SR) != SOCK_UDP);
}


/**
 * Looks for a connection
 */
uint8_t tftpPoll() {
  uint8_t response = ACK;
  // Get the size of the recieved data
  uint16_t packetSize = netReadWord(REG_S3_RX_RSR0);

  // If we have data
  if (packetSize) {
    // Process Packet and get TFTP response code
    response = processPacket(packetSize);
    // Send the response
    sendResponse(response);
    //Delay to let the ACK reach it's target
    _delay_ms(500);
  }
  if ((response == FINAL_ACK) || timedOut()) {
    netWriteReg(REG_S3_CR, CR_CLOSE);
    // Complete
    return(0);
  }
  // Tftp continues
  return(1);
}
