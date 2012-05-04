#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define TIMEOUT 3
#define LED_PIN 5

void updateLed();
uint8_t timedOut();
