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
#include <util/delay.h>

#include "util.h"
#include "neteeprom.h"
#include "pin_defs.h"

uint16_t lastTimer1;
uint16_t tick = 0;


void updateLed(void)
{
	uint16_t nextTimer1 = TCNT1;
	if(nextTimer1 & 0x400) LED_PORT ^= _BV(LED); // Led pin high
	else LED_PORT &= ~_BV(LED); // Led pin low
	if(nextTimer1 < lastTimer1) tick++;
	lastTimer1 = nextTimer1;
}

void resetTick(void)
{
	tick = 0;
}

uint8_t timedOut(void)
{
	// Never timeout if there is no code in Flash
	if(eeprom_read_byte(EEPROM_IMG_STAT) == EEPROM_IMG_BAD_VALUE) return(0);
	if(tick > TIMEOUT) return(1);
	return(0);
}
