/* Name: watchdog.c
 * Author: Stelios Tsampas
 * Copyright: Stelios Tsampas
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ethboot
 * Function: watchdog/reset handling
 * Version: 0.1b tftp / flashing functional
 */

#include "watchdog.h"

#include "debug.h"

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

/**
 * Disable the watchdog timer to prevent
 * iternal reset loop of doom and despair
 */
void watchdogInit(void) {
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    wdt_disable();
}
