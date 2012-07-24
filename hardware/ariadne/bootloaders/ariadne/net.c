/* Name: net.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Network and W5100 chip support
 * Version: 0.1 tftp / flashing functional
 */

#include "net.h"
#include "neteeprom.h"
#include "pin_defs.h"
#include "serial.h"
#include "tftp.h"
#include "debug.h"

#define SS_LOW() PORTB &= ~_BV(SS)
#define SS_HIGH() PORTB |= _BV(SS)

uint8_t registerBuffer[REGISTER_BLOCK_SIZE] = {
	0x80,         // MR Mode - reset device

	// EEPROM block starts here
	GW_ADDR,      // GWR Gateway IP Address Register
	SUB_MASK,  // SUBR Subnet Mask Register
	MAC_ADDR,     // SHAR Source Hardware Address Register
	IP_ADDR,      // SIPR Source IP Address Register
	// EEPROM block ends here

	0, 0,         // Reserved locations
	0,            // IR Interrupt Register
	0,            // IMR Interrupt Mask Register
	0x07, 0xd0,   // RTR Retry Time-value Register
	0x80,         // RCR Retry Count Register
	0x55,         // RMSR Rx Memory Size Register, 2K per socket
	0x55          // TMSR Tx Memory Size Register, 2K per socket
};


void netWriteReg(uint16_t address, uint8_t value)
{
#ifdef _DEBUGMORE_NET
	traceln(" Net: netWriteReg: ");
	tracenum(address);
	trace(", ");
	tracenum(value);
#endif

	// Send uint8_t to Ethernet controller
	SPCR = _BV(SPE) | _BV(MSTR); // Set SPI as master
	SS_LOW();
	SPDR = SPI_WRITE;
	while(!(SPSR & _BV(SPIF)));
	SPDR = address >> 8;
	while(!(SPSR & _BV(SPIF)));
	SPDR = address & 0xff;
	while(!(SPSR & _BV(SPIF)));
	SPDR = value;
	while(!(SPSR & _BV(SPIF)));
	SS_HIGH();
	SPCR = 0; // Turn off SPI
}

uint8_t netReadReg(uint16_t address)
{
//#ifdef _DEBUGMORE_NET
//    traceln(" Net: netReadReg: ");
//    tracenum(address);
//#endif

	// Read uint8_t from Ethernet controller
	uint8_t returnValue;
	SPCR = _BV(SPE) | _BV(MSTR);
	SS_LOW();
	SPDR = SPI_READ;
	while(!(SPSR & _BV(SPIF)));
	SPDR = address >> 8;
	while(!(SPSR & _BV(SPIF)));
	SPDR = address & 0xff;
	while(!(SPSR & _BV(SPIF)));
	SPDR = 0;
	while(!(SPSR & _BV(SPIF)));
	SS_HIGH();
	returnValue = SPDR;
	SPCR = 0;
	return(returnValue);
}

uint16_t netReadWord(uint16_t address)
{
	// Read uint16_t from Ethernet controller
	return((netReadReg(address) << 8) | netReadReg(address + 1));
}

void netWriteWord(uint16_t address, uint16_t value)
{
	// Write uint16_t to Ethernet controller
	netWriteReg(address++, value >> 8);
	netWriteReg(address, value & 0xff);
}

void netInit()
{
	uint8_t i;

	// Set up outputs to communicate with W5100 chip
	// Set pins as output
	DDRB = _BV(SCK) | _BV(MOSI) | _BV(SS);
	// Set pins high
	PORTB = _BV(SCK) | _BV(MISO) | _BV(MOSI) | _BV(SS);
#ifdef _ARDUINO_ETHERNET
	DDRB |= _BV(LED);
	PORTB |= _BV(LED);
#endif

	// Set up SPI
	// Set the Double SPI Speed Bit
	SPSR = (1 << SPI2X);

#ifndef _TFTP_RANDOM_PORT
	tftpPort = TFTP_STATIC_PORT;
#endif
	/* Pull in altered presets
	 * if available from AVR EEPROM (if signature bytes are set)*/
	if((eeprom_read_byte(EEPROM_SIG_1) == EEPROM_SIG_1_VALUE)
		&& (eeprom_read_byte(EEPROM_SIG_2) == EEPROM_SIG_2_VALUE)) {

		for(i = 0; i < EEPROM_SETTINGS_SIZE; i++)
			registerBuffer[i+1] = eeprom_read_byte(EEPROM_DATA+i);
#ifndef _TFTP_RANDOM_PORT
		tftpPort = ((eeprom_read_byte(EEPROM_PORT+1)<<8) + eeprom_read_byte(EEPROM_PORT));
#endif

#ifdef _VERBOSE
		traceln(" Net: Using EEPROM settings");
#endif
	}
#ifdef _VERBOSE
	else {
		traceln(" Net: Using built-in settings");
	}
#endif

#ifdef _VERBOSE
	traceln("\tAddress: ");
	for(i = 15; i < 19; i++) {
		tracenet(registerBuffer[i]);
		if(i != 18) putch(0x2E);
	}
	traceln("\t Subnet: ");
	for(i = 5; i < 9; i++) {
		tracenet(registerBuffer[i]);
		if(i != 8) putch(0x2E);
	}
	traceln("\tGateway: ");
	for(i = 1; i < 5; i++) {
		tracenet(registerBuffer[i]);
		if(i != 4) putch(0x2E);
	}
	traceln("\t    MAC: ");
	for(i = 9; i < 15; i++) {
		tracenet(registerBuffer[i]);
		if(i != 14) putch(0x2E);
	}
#ifndef _TFTP_RANDOM_PORT
	traceln("\t   Port: ");
	tracenum(tftpPort);
#endif
#endif

	// Configure Wiznet chip
	for(i = 0; i < REGISTER_BLOCK_SIZE; i++)
		netWriteReg(i, registerBuffer[i]);
#ifdef _VERBOSE
		traceln(" Net: Network init done");
#endif
}

