#include <stdint.h>
#include <avr/wdt.h>

void watchdogInit(void) __attribute__((naked)) __attribute__((section(".init3")));
