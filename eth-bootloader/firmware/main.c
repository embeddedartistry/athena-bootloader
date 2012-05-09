/* Name: main.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Bootloader core
 * Version: 0.1b tftp / flashing functional
 */

#include "main.h"
#include "net.h"
#include "tftp.h"
#include "announce.h"
#include "watchdog.h"

#include "debug.h"

uint16_t lastTimer1;
uint16_t tick = 0;


int main(void) __attribute__ ((naked)) __attribute__ ((section (".init9")));

// Function
void (*app_start)(void) = 0x0000;

void updateLed() {
  uint16_t nextTimer1 = TCNT1;
  if (nextTimer1 & 0x400) PORTB |= _BV(LED_PIN); // Led pin high
  else PORTB &= ~_BV(LED_PIN); // Led pin low
  if (nextTimer1 < lastTimer1) tick++;
  lastTimer1 = nextTimer1;
}

uint8_t timedOut() {
  // Never timeout if there is no code in Flash
  if (pgm_read_word(0x0000) == 0xFFFF) return(0);
  if (tick > TIMEOUT) return(1);
  return(0);
}

int main(void) {
  // Watchdog timer disable
  watchdogInit();
  // Wait to ensure startup
  _delay_ms(300);

  //cli();
  //SP=RAMEND;  // This is done by hardware reset
  //asm volatile ("clr __zero_reg__");

  // Set up outputs to communicate with W5100 chip
  // Set pins as output
  DDRB = _BV(SCK_PIN) | _BV(MOSI_PIN) | _BV(SS_PIN);
  // Set pins high
  PORTB = _BV(SCK_PIN) | _BV(MISO_PIN) | _BV(MOSI_PIN) | _BV(SS_PIN);

  // Prescaler=0, ClkIO Period = 62,5ns
  // TCCR1B values:
  // 0x01 -> ClkIO/1 -> 62,5ns period, 4ms max
  // 0x02 -> ClkIO/8 -> 500ns period, 32ms max
  // 0X03 -> ClkIO/64 -> 4us period, 256ms max
  // 0x04 -> ClkIO/256 -> 16us period, 1024ms max
  // 0x05 -> ClkIO/1024 -> 64us period, 4096ms max
  // Set up Timer 1 as timekeeper for LED flashing
  TCCR1B = 0x05;
  //Set the Double SPI Speed Bit
  SPSR = (1 << SPI2X);

#ifdef _DEBUG
  //Start the debug system
  debugInit();
#endif

#ifdef _DEBUG
  traceln("\r\nMain: TFTP-Bootloader for Arduino Uno, Version 0.1b");
  //step();
#endif

  // Initialize W5100 chip
#ifdef _DEBUG
  traceln("Main: Net init...");
#endif
  netInit();
#ifdef _DEBUG
  traceln("Main: Net init done");
#endif

  // Attempt tftp download
#ifdef _DEBUG
  traceln("Main: TFTP init...");
#endif
  tftpInit();
#ifdef _DEBUG
  traceln("Main: TFTP init done");
#endif

#ifdef _DEBUG
  traceln("Main: Network broadcast init...");
#endif
  announceInit();
#ifdef _DEBUG
  traceln("Main: Network broadcast init done");
#endif

  for (;;) {
    // If tftp recieves a connection break
    if (tftpPoll() == 0) break;
    //
    announcePoll();
    // Blink the notification led
    updateLed();
  }

  // Exit to foreground application
#ifdef _DEBUG
  traceln("Main: Start user app");
#endif
  app_start();
  return(0); /* never reached */
}
