/* Name: main.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: tftpboot
 * Function: Bootloader core
 * Version: 0.2 support for USB flashing
 */

#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/boot.h>

#include "util.h"
#include "net.h"
#include "tftp.h"
#include "serial.h"
#include "neteeprom.h"
#include "watchdog.h"
#include "debug.h"
#include "debug_main.h"
#ifdef ANNOUNCE
	#include "announce.h"
#endif


int main(void) __attribute__((naked)) __attribute__((section(".init9")));
void (*appStart)(void) __attribute__((naked)) = 0x0000;


int main(void)
{
	// Watchdog timer disable
	watchdogDisable();
	// Wait to ensure startup
	_delay_ms(300);

	/* This code makes the following assumptions:
	 * No interrupts will execute
	 * SP points to RAMEND
	 * r1 contains zero */
	//cli();
	asm volatile("clr __zero_reg__");
#ifdef __AVR_ATmega8__
	SP = RAMEND;  // This is done by hardware reset
#endif

	// Prescaler=0, ClkIO Period = 62,5ns
	// TCCR1B values:
	// 0x01 -> ClkIO/1 -> 62,5ns period, 4ms max
	// 0x02 -> ClkIO/8 -> 500ns period, 32ms max
	// 0X03 -> ClkIO/64 -> 4us period, 256ms max
	// 0x04 -> ClkIO/256 -> 16us period, 1024ms max
	// 0x05 -> ClkIO/1024 -> 64us period, 4096ms max
	// Set up Timer 1 as timekeeper for LED flashing
	TCCR1B = _BV(CS12) | _BV(CS10); // Same thing as TCCR1B = 0x05;

	/* Write version information in the EEPROM */
	if(eeprom_read_byte(EEPROM_MAJVER) != ARIADNE_MAJVER)
		eeprom_write_byte(EEPROM_MAJVER, ARIADNE_MAJVER);
	if(eeprom_read_byte(EEPROM_MINVER) != ARIADNE_MINVER)
		eeprom_write_byte(EEPROM_MINVER, ARIADNE_MINVER);

	//Initialize UART communication
	serialInit();
	DBG_MAIN(tracePGMlnMain(mMainDebug_TITLE);)

	DBG_BTN(
		DBG_MAIN(tracePGMlnMain(mMainDebug_BTN);)
		buttonInit();
	)

	// Initialize W5100 chip
	DBG_MAIN(tracePGMlnMain(mMainDebug_NET);)
	netInit();

	// Initialize the UDP socket for tftp
	DBG_MAIN(tracePGMlnMain(mMainDebug_TFTP);)
	tftpInit();

	/* This code is to be used with the java-client inherited from the
	 * Arduino project. We don't support it and it adds about
	 * 600 bytes to the binary. So off it goes */
#ifdef ANNOUNCE
	DBG_MAIN(tracePGMlnMain(mMainDebug_ANN);)
	announceInit();
#endif

	_delay_ms(300);

	serialFlashing = FALSE;
	tftpFlashing = FALSE;

	for(;;) {
		// If there is no serial flashing under way, poll tftp
		if(!serialFlashing)
			// If tftp recieved a FINAL_ACK, break
			if(tftpPoll() == 0)
				break;

		// If there is no tftp flashing, poll serial
		if(!tftpFlashing)
			// If flashing is done exit
			if(serialPoll() == 0)
				break;

		/* As explained above this goes out */
#ifdef ANNOUNCE
		announcePoll();
#endif

		if(timedOut()) {
			if(eeprom_read_byte(EEPROM_IMG_STAT) == EEPROM_IMG_OK_VALUE) break;

			//TODO: determine the conditions for reseting server OR reseting socket
			if(tftpFlashing == TRUE) {
				// Delete first page of flash memory
				boot_page_erase(0);
				// Reinitialize TFTP
				tftpInit();
				// Reset the timeout counter
				resetTick();
				// Unset tftp flag
				tftpFlashing = FALSE;
			}
		}

		/* Blink the notification led */
		updateLed();
	}

	/* Exit to user application */
	DBG_MAIN(tracePGMlnMain(mMainDebug_EXIT);)
	appStart();
	return(0); /* never reached */
}

