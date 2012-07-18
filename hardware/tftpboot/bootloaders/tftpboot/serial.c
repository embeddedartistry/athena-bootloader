/* Name: serial.c
 * Author: .
 * Copyright: Stelios Tsampas with stuff from Arduino and optiboot
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: tftpboot
 * Function: UART support of optiboot and debugging
 * Version: 0.2 support for USB flashing
 */

#include "serial.h"
#include "watchdog.h"
#include "stk500.h"
#include "pin_defs.h"


void serialInit()
{
	UCSR0A = _BV(U2X0); //Double speed mode USART0
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	UBRR0L = (uint8_t)((F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1);
#ifdef _VERBOSE
	DDRD = 0x92;
#endif
}


void putch(uint8_t c)
{
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
}


void puthex(uint8_t c)
{
	c &= 0xf;
	if(c > 9) c += 7;
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = c + '0';
}


uint8_t getch(void)
{
	uint8_t ch;

	while(!(UCSR0A & _BV(RXC0)));
	if(!(UCSR0A & _BV(FE0))) {
		/*
		 * A Framing Error indicates (probably) that something is talking
		 * to us at the wrong bit rate.  Assume that this is because it
		 * expects to be talking to the application, and DON'T reset the
		 * watchdog.  This should cause the bootloader to abort and run
		 * the application "soon", if it keeps happening.  (Note that we
		 * don't care that an invalid char is returned...)
		 */
		watchdogReset();
	}
	ch = UDR0;

	return ch;
}
