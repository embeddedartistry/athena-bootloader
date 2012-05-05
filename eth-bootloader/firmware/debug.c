/* Name: debug.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: eboot
 * Function: Utility routines for bootloader debugging
 * Version: 0.1 tftp / flashing functional
 */

#include "debug.h"


#ifdef _DEBUG
void debugInit() {
  UCSR0A = 0x22;
  UCSR0B = 0x08;
  //UCSR0C = 0x06; // = reset state
  UBRR0 = 16; // 115k2 baud 8N1
  DDRD = 0x92;
  PORTB |= _BV(PB1);
}

void printchar(uint8_t c) {
  UDR0 = c;
  while(!(UCSR0A & _BV(UDRE0)));
}

void printhex(uint8_t c) {
  c &= 0xf;
  if(c > 9) c += 7;
  UDR0 = c + '0';
  while(!(UCSR0A & _BV(UDRE0)));
}

/* FIXME: the button is reverse, shortcircuiting when it
 * is released. Code was modified accordingly */
uint8_t checkbutton() {
  /* the button is pressed when BUTTON_BIT is clear */
  if(bit_is_clear(PINB, PB1)) {
    _delay_ms(25);
    if(bit_is_clear(PINB, PB1)) return 0;
  }
  return 1;
}


void step() {
  while(1) if(checkbutton()) break;
  _delay_ms(250); // Lock input
}

void trace(char* msg) {
  uint8_t c;
  if (*msg != '\0') {
    while((c = *msg++)) printchar(c);
  }
}

void traceln(char* msg) {
  trace("\r\n");
  trace(msg);
}

void tracenum(uint16_t num) {
  printchar('0');
  printchar('x');
  printhex(num >> 12);
  printhex(num >> 8);
  printhex(num >> 4);
  printhex(num);
}
#endif
