/* Name: net.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Network and W5100 chip support
 * Version: 0.1 tftp / flashing functional
 */

#include "net.h"
#include "main.h"
#include "neteeprom.h"

#include "debug.h"


uint8_t registerBuffer[REGISTER_BLOCK_SIZE] = {
  0x80,                           // MR Mode - reset device

  //FIXME: Edited to ITYE network
  // EEPROM block starts here
  150,140,5,1,                    // GWR Gateway IP Address Register
  255,255,255,128,                // SUBR Subnet Mask Register
  0x12,0x34,0x45,0x78,0x9A,0xBC,  // SHAR Source Hardware Address Register
  150,140,5,120,                  // SIPR Source IP Address Register
  // EEPROM block ends here

  0,0,                            // Reserved locations
  0,                              // IR Interrupt Register
  0,                              // IMR Interrupt Mask Register
  0x07,0xd0,                      // RTR Retry Time-value Register
  0x80,                           // RCR Retry Count Register
  0x55,                           // RMSR Rx Memory Size Register, 2K per socket
  0x55                            // TMSR Tx Memory Size Register, 2K per socket
};

void netWriteReg(uint16_t address, uint8_t value) {
#ifdef _DEBUGMORE_NET
  traceln(" Net: netWriteReg: ");
  tracenum(address);
  trace(", ");
  tracenum(value);
#endif

  // Send uint8_t to Ethernet controller
  SPCR = _BV(SPE) | _BV(MSTR); // Set SPI as master
  SS_LOW();
  SPDR = SPI_WRITE;       while(!(SPSR & _BV(SPIF)));
  SPDR = address >> 8;    while(!(SPSR & _BV(SPIF)));
  SPDR = address & 0xff;  while(!(SPSR & _BV(SPIF)));
  SPDR = value;           while(!(SPSR & _BV(SPIF)));
  SS_HIGH();
  SPCR = 0; // Turn of SPI
}


uint8_t netReadReg(uint16_t address) {
//#ifdef _DEBUGMORE
//    traceln(" Net: netReadReg: ");
//    tracenum(address);
//#endif

  // Read uint8_t from Ethernet controller
  uint8_t returnValue;
  SPCR = _BV(SPE) | _BV(MSTR);
  SS_LOW();
  SPDR = SPI_READ;        while(!(SPSR & _BV(SPIF)));
  SPDR = address >> 8;    while(!(SPSR & _BV(SPIF)));
  SPDR = address & 0xff;  while(!(SPSR & _BV(SPIF)));
  SPDR = 0;               while(!(SPSR & _BV(SPIF)));
  SS_HIGH();
  returnValue = SPDR;
  SPCR = 0;
  return(returnValue);
}


uint16_t netReadWord(uint16_t address) {
  // Read uint16_t from Ethernet controller
  return((netReadReg(address)<<8) | netReadReg(address+1));
}


void netWriteWord(uint16_t address, uint16_t value) {
  // Write uint16_t to Ethernet controller
  netWriteReg(address++, value >> 8);
  netWriteReg(address, value & 0xff);
}


void netInit() {
  // Initialize Wiznet chip
  // Set up SPI

  /* Pull in altered presets
   * if available from AVR EEPROM (if signature bytes are set)*/
  if ( (eeprom_read_byte(EEPROM_SIG_1)==EEPROM_SIG_1_VALUE)
        && (eeprom_read_byte(EEPROM_SIG_2)==EEPROM_SIG_2_VALUE)) {
      uint8_t i=0;
#ifdef _DEBUG_NET
      traceln(" Net: Using EEPROM settings");
#endif
      for (;i<18; i++) registerBuffer[i+1] = eeprom_read_byte(EEPROM_DATA+i);
  } else {
#ifdef _DEBUG_NET
      traceln(" Net: Using default ip 150.140.5.120");
#endif
    ;
  }

  // Configure Wiznet chip
  uint8_t i=0;
  for (; i<REGISTER_BLOCK_SIZE; i++) netWriteReg(i, registerBuffer[i]);
}
