/* Name: debug.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Utility routines for bootloader debugging
 * Version: 0.1b tftp / flashing functional
 */

#include "debug.h"
#include "serial.h"


#ifdef _DEBUG
void trace(char *msg)
{
	uint8_t c;
	if(*msg != '\0') {
		while((c = *msg++)) putch(c);
	}
}

void traceln(char *msg)
{
	trace("\r\n");
	trace(msg);
}

void tracenum(uint16_t num)
{
	trace("0x");
	puthex(num >> 12);
	puthex(num >> 8);
	puthex(num >> 4);
	puthex(num);
}

#ifdef _DEBUG_STEP
void stepInit()
{
	PORTB |= _BV(PB1);
}

/* FIXME: the button is reverse, shortcircuiting when it
 * is released. Code was modified accordingly */
uint8_t checkbutton()
{
	/* the button is pressed when BUTTON_BIT is clear */
	if(bit_is_clear(PINB, PB1)) {
		_delay_ms(25);
		if(bit_is_clear(PINB, PB1)) return 0;
	}
	return 1;
}

void step()
{
	while(1) if(checkbutton()) break;
	_delay_ms(250); // Lock input
}
#endif
#endif
