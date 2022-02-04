/* Name: util.c
 * Author: Stelios Tsampas
 * Copyright: Arduino and optiboot
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: tftpboot
 * Function: Utility routines for various stuff
 * Version: 0.2 support for USB flashing
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "debug.h"
#include "debug_util.h"
#include "gpio.h"
#include "tftp.h"
#include "util.h"

static uint16_t last_timer_1;
static uint16_t tick = 0;

void updateLed(void)
{
	uint16_t next_timer_1 = TCNT1;

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
	if(next_timer_1 & 0x1000)
	{
		LED_PORT ^= _BV(LED); // Led pin high
	}
	else
	{
		LED_PORT &= ~_BV(LED); // Led pin low
	}
#endif

	if(next_timer_1 < last_timer_1)
	{
		tick++;
		DBG_UTIL(tracePGMlnUtil(mDebugUtil_TICK); tracenum(tick); tracePGMlnUtil(mDebugUtil_NEXT);
				 tracenum(next_timer_1); tracePGMlnUtil(mDebugUtil_LAST); tracenum(last_timer_1);)
	}

	last_timer_1 = next_timer_1;
}

void resetTick(void)
{
	TCNT1 = 0;
	tick = 0;
	wdt_reset(); // Added so can use wdt
}

uint8_t timedOut(void)
{
	// Never timeout if there is no code in flash,
	//  but if flashing started- we do want to check for timeout
	if(!tftpFlashing && pgm_read_word(0x0000) == 0xFFFF)
	{
		return (0);
	}

	return (tick > TIMEOUT) ? 1 : 0;
}
