/* Name: main.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: tftpboot
 * Function: Bootloader core
 * Version: 0.2 support for USB flashing
 */

#include "util.h"
#include "net.h"
#include "tftp.h"
#include "serial.h"
#include "optiboot.h"
#include "watchdog.h"
#include "debug.h"

#ifdef _ANNOUNCE
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
	 * r1 contains zero
	 */
	//cli();
	asm volatile ("clr __zero_reg__");
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

	//Initialize UART communication
	serialInit();
#ifdef _DEBUG
	traceln("\r\nMain: TFTP-Bootloader for Arduino Uno, Version 0.2");
#endif

#ifdef _DEBUG_STEP
	stepInit();
	traceln("Main: Button stepping enabled");
#endif

	// Initialize W5100 chip
	netInit();
#ifdef _DEBUG
	traceln("Main: Net init done");
#endif

	// Attempt tftp download
	tftpInit();
#ifdef _DEBUG
	traceln("Main: TFTP init done");
#endif

/* This code is to be used with the java-client inherited from the
 * Arduino project. We don't support that yet and it adds about
 * 600 bytes to the binary. So off it goes */
#ifdef _ANNOUNCE
	announceInit();
#ifdef _DEBUG
	traceln("Main: Network broadcast init done");
#endif
#endif

	_delay_ms(100);

	serialFlashing = FALSE;
	tftpFlashing = FALSE;

	for(;;) {
		/* If there is no serial flashing under way, poll tftp */
		if(!serialFlashing)
			/* If tftp recieved a FINAL_ACK, break */
			if(tftpPoll() == 0) break;
		/* If there is no tftp flashing, poll serial */
		if(!tftpFlashing)
			/* If flashing is done exit */
			if(serialPoll() == 0) break;

		/* As explained above this goes out */
#ifdef _ANNOUNCE
		announcePoll();
#endif
		/* Blink the notification led */
		updateLed();
	}

	/* Exit to foreground application */
#ifdef _DEBUG
	traceln("Main: Start user app");
#endif
	appStart();
	return(0); /* never reached */
}

