#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define TIMEOUT 3

#ifndef LED_START_FLASHES
	#define LED_START_FLASHES 0
#endif

#define TRUE  1
#define FALSE 0

void updateLed();
uint8_t timedOut();
