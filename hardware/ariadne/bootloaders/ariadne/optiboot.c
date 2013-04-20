/* Name: optiboot.c
 * Author: .
 * Copyright: Stelios Tsampas based on the optiboot bootloader
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: Ariadne bootloader
 * Function: optiboot flasher
 * Version: 0.2 support for USB flashing
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

#include "util.h"
#include "serial.h"
#include "watchdog.h"
#include "neteeprom.h"
#include "optiboot.h"
#include "optiboot_stk500.h"

#if _DEBUG > 0
	#include "debug.h"
#endif
	
static uint16_t address = 0;
static uint8_t  length;


static void verifySpace(void)
{
	if(getCh() != CRC_EOP) {
		watchdogConfig(WATCHDOG_16MS);	// shorten WD timeout
		while(1)						// and busy-loop so that WD causes
			;							// a reset and app start.
	}
	putCh(STK_INSYNC);
}


static void getNch(uint8_t count)
{
	do getCh();
	while(--count);
	verifySpace();
}


uint8_t processOptiboot(void)
{
	uint8_t ch;

	ch = getCh();

	if(ch == STK_GET_PARAMETER) {
		unsigned char which = getCh();
		verifySpace();
		if(which == 0x82) {
			/*
			 * Send tftpboot version as "minor SW version"
			 */
			putCh(ARIADNE_MINVER);
		} else if(which == 0x81) {
			putCh(ARIADNE_MAJVER);
		} else {
			/*
			 * GET PARAMETER returns a generic 0x03 reply for
			 * other parameters - enough to keep Avrdude happy
			 */
			putCh(0x03);
		}
	} else if(ch == STK_SET_DEVICE) {
		// SET DEVICE is ignored
		getNch(20);
	} else if(ch == STK_SET_DEVICE_EXT) {
		// SET DEVICE EXT is ignored
		getNch(4);
	} else if(ch == STK_LOAD_ADDRESS) {
		// LOAD ADDRESS
		uint16_t newAddress;
		newAddress = getCh();
		newAddress = (newAddress & 0xff) | (getCh() << 8);

#ifdef RAMPZ
		// Transfer top bit to RAMPZ
		RAMPZ = (newAddress & 0x8000) ? 1 : 0;
#endif

		newAddress += newAddress; // Convert from word address to byte address
		address = newAddress;
		verifySpace();
	} else if(ch == STK_UNIVERSAL) {
		// UNIVERSAL command is ignored
		getNch(4);
		putCh(0x00);
	}
	/* Write memory, length is big endian and is in bytes */
	else if(ch == STK_PROG_PAGE) {
		// This should probably go somewhere but I don't yet know it's place
		//eeprom_write_byte(EEPROM_IMG_STAT, EEPROM_IMG_BAD_VALUE);
		// PROGRAM PAGE - we support flash programming only, not EEPROM
		uint8_t  buff[256];
		uint8_t* bufPtr;
		uint16_t addrPtr;

		getCh();			/* getlen() */
		length = getCh();
		getCh();

		// If we are in RWW section, immediately start page erase
		if(address < NRWWSTART) boot_page_erase((uint16_t)(void*)address);

		// While that is going on, read in page contents
		bufPtr = buff;
		do* bufPtr++ = getCh();
		while(--length);

		// If we are in NRWW section, page erase has to be delayed until now.
		// Todo: Take RAMPZ into account
		if(address >= NRWWSTART) boot_page_erase((uint16_t)(void*)address);

		// Read command terminator, start reply
		verifySpace();

		// If only a partial page is to be programmed, the erase might not be complete.
		// So check that here
		boot_spm_busy_wait();

		// Copy buffer into programming buffer
		bufPtr = buff;
		addrPtr = (uint16_t)(void*)address;
		ch = SPM_PAGESIZE / 2;
		do {
			uint16_t a;
			a = *bufPtr++;
			a |= (*bufPtr++) << 8;
			boot_page_fill((uint16_t)(void*)addrPtr, a);
			addrPtr += 2;
		} while(--ch);

		// Write from programming buffer
		boot_page_write((uint16_t)(void*)address);
		boot_spm_busy_wait();

#if defined(RWWSRE)
		// Reenable read access to flash
		boot_rww_enable();
#endif
	}
	/* Read memory block mode, length is big endian.  */
	else if(ch == STK_READ_PAGE) {
		// READ PAGE - we only read flash
		getCh();			/* getlen() */
		length = getCh();
		getCh();

		verifySpace();

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560)
		//      do putCh(pgm_read_byte_near(address++));
		//      while (--length);
		do {
			uint8_t result;
			__asm__("elpm %0,Z\n":"=r"(result):"z"(address));
			putCh(result);
			address++;
		} while(--length);
#else
		do putCh(pgm_read_byte_near(address++));
		while(--length);
#endif
	}
	/* Get device signature bytes  */
	else if(ch == STK_READ_SIGN) {
		// READ SIGN - return what Avrdude wants to hear
		verifySpace();
		putCh(SIGNATURE_0);
		putCh(SIGNATURE_1);
		putCh(SIGNATURE_2);
	} else if(ch == STK_LEAVE_PROGMODE) {
		// Adaboot no-wait mod
		//watchdogConfig(WATCHDOG_16MS);
		verifySpace();
		eeprom_write_byte(EEPROM_IMG_STAT, EEPROM_IMG_OK_VALUE);
		putCh(STK_OK);
		return(0);
	} else {
		// This covers the response to commands like STK_ENTER_PROGMODE
		verifySpace();
	}
	putCh(STK_OK);
	return(1);
}