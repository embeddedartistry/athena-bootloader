/* Name: spi.c
 * Author: Stelios Tsampas
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: athena bootloader
 * Function: SPI functions
 */

#include "spi.h"
#include "debug.h"
#include "debug_spi.h"
#include "gpio.h"
#include "neteeprom.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdint.h>

volatile uint8_t* eth_cs_ddr = &ETH_DDR;
volatile uint8_t* eth_cs_port = &ETH_PORT;
uint8_t eth_cs_pin = _BV(ETH_SS);

void initialize_eth_cs(void)
{
	uint8_t eth_pin = eeprom_read_byte((uint8_t*)NETEEPROM_ETHERNET_CS_PIN);

	if(eth_pin != 0xff)
	{
		DBG_SPI(tracePGMlnSpi(mDebugSpi_CUSTOM_ETH_CS); tracenum(eth_pin);)
		uint8_t eth_port = digitalPinToPort(eth_pin);
		eth_cs_port = portOutputRegister(eth_port);
		eth_cs_ddr = portModeRegister(eth_port);
		eth_cs_pin = digitalPinToBitMask(eth_pin);
	}

	/** Set ethernet SS high */
	*eth_cs_port |= eth_cs_pin;
	/** Set ethernet SS as output */
	*eth_cs_ddr |= eth_cs_pin;
}

void spi_cs_low(void)
{
	*eth_cs_port &= ~eth_cs_pin;
}

void spi_cs_high(void)
{
	*eth_cs_port |= eth_cs_pin;
}

/** Send uint8_t to Ethernet controller */
void spiWriteReg(uint16_t address, uint8_t cb, uint8_t value)
{
#ifndef __WIZ_W5500__
	(void)cb;
#endif

	DBG_SPI_EX(tracePGMlnSpi(mDebugSpi_NWREG); tracenum(address); tracePGM(mDebugSpi_COMMA);
			   tracenum(value);)

	SS_LOW();

#if defined(__WIZ_W5100__)
	SPDR = SPI_WRITE;
	while(!(SPSR & _BV(SPIF)))
		;
#endif

	SPDR = address >> 8;
	while(!(SPSR & _BV(SPIF)))
		;

	SPDR = address & 0xff;
	while(!(SPSR & _BV(SPIF)))
		;

#if defined(__WIZ_W5200__)
	SPDR = 0x80;
	while(!(SPSR & _BV(SPIF)))
		;

	SPDR = 0x01;
	while(!(SPSR & _BV(SPIF)))
		;
#endif
#if defined(__WIZ_W5500__)
	SPDR = cb; // Socket 3 BSB Write 0x6D Selects Socket 3 Register, write mode, 1 byte data length
	while(!(SPSR & _BV(SPIF)))
		;
#endif

	SPDR = value;
	while(!(SPSR & _BV(SPIF)))
		;

	SS_HIGH();
}

void spiWriteWord(uint16_t address, uint8_t cb, uint16_t value)
{
	// Write uint16_t to Ethernet controller
	spiWriteReg(address++, cb, value >> 8);
	spiWriteReg(address, cb, value & 0xff);
}

/** Read uint8_t from Ethernet controller */
uint8_t spiReadReg(uint16_t address, uint8_t cb)
{
#ifndef __WIZ_W5500__
	(void)cb;
#endif

#if defined(SPAM_ME)
	DBG_SPI_EX(tracePGMlnSpi(mDebugSpi_NRREG); tracenum(address);)
#endif

	uint8_t returnValue;

	SS_LOW();

#if defined(__WIZ_W5100__)
	SPDR = SPI_READ;
	while(!(SPSR & _BV(SPIF)))
		;
#endif

	SPDR = address >> 8;
	while(!(SPSR & _BV(SPIF)))
		;

	SPDR = address & 0xff;
	while(!(SPSR & _BV(SPIF)))
		;

#if defined(__WIZ_W5200__)
	SPDR = 0x00;
	while(!(SPSR & _BV(SPIF)))
		;

	SPDR = 0x01;
	while(!(SPSR & _BV(SPIF)))
		;
#endif
#if defined(__WIZ_W5500__)
	SPDR = cb; // Socket 3 BSB Read 0x69 Selects Socket 3 Register, read mode, 1 byte data length
	while(!(SPSR & _BV(SPIF)))
		;
#endif

	SPDR = 0;
	while(!(SPSR & _BV(SPIF)))
		;

	SS_HIGH();
	returnValue = SPDR;

	return (returnValue);
}

uint16_t spiReadWord(uint16_t address, uint8_t cb)
{
	// Read uint16_t from Ethernet controller
	return ((spiReadReg(address, cb) << 8) | spiReadReg(address + 1, cb));
}

void spiInit(void)
{
	/**
	 * Set up pins for SPI communication.
	 * At this stage all pins are set to HIGH. This in fact DISABLES SPI for both Ethernet and SD.
	 * SS pin for ethernet is pulled low just in time for reading or writing data inside those
	 * functions. */
	SPCR = 0;

	/**
	 *
	 * Note that the primary SS pin must be set as output high before setting `MSTR`
	 * in SPCR. If the CS line is low input, the device automatically sets you into slave mode.
	 *
	 * Why not |= here?
	 * We need to set Pins 11..13 to inputs if ATMega2560, because the shield pinout conflicts
	 */
	/** Set SPI pins high */
	SPI_PORT = _BV(SS);
	/** Set SPI pins as output */
	SPI_DDR = _BV(SCK) | _BV(MOSI) | _BV(SS);

	initialize_eth_cs();

	/** Disable SD card */
	/** Set SD SS pin high */
	SD_PORT |= _BV(SD_SS);
	/** Set SD SS pin as output */
	SD_DDR |= _BV(SD_SS);

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
#if(LED != SCK)
	/** Set up pins to flash the onboard led */
	/** Set led pin to high */
	LED_PORT |= _BV(LED);
	/** Set led pin as output */
	LED_DDR |= _BV(LED);
#endif
#endif

	/** Set up SPI as master, mode 0
	 ** Set the Double SPI Speed Bit */
	SPCR = _BV(SPE) | _BV(MSTR);
	SPSR = (1 << SPI2X);

	DBG_SPI(tracePGMlnSpi(mDebugSpi_DONE);)
}
