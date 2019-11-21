/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ariadne
 * Function: debugging progmem strings
 */

#ifndef debug_main_h
#define debug_main_h

#include <avr/pgmspace.h>

#if(DEBUG_MAIN > 0)
#undef DBG_MAIN
#define DBG_MAIN(block) block
#define tracePGMlnMain(msg) tracePGMln(mDebugMain_PREFIX, msg)
const unsigned char mDebugMain_PREFIX[] PROGMEM = "Main: ";

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#if defined(ARDUINO_ETHERNET)
const unsigned char mDebugMain_TITLE[] PROGMEM = "Ariadne for Arduino Ethernet, Version 0.9";
#else
const unsigned char mDebugMain_TITLE[] PROGMEM = "Ariadne for Arduino Uno, Version 0.9";
#endif
#elif defined(__AVR_ATmega2560__)
const unsigned char mDebugMain_TITLE[] PROGMEM = "Ariadne for Arduino Mega2560, Version 0.9";
#elif defined(__AVR_ATmega1284P__)
const unsigned char mDebugMain_TITLE[] PROGMEM = "Ariadne for Arduino Mega1284P, Version 0.9";
#else
const unsigned char mDebugMain_TITLE[] PROGMEM = "Unknown MCU with ariadne, Version 0.9";
#endif
const unsigned char mDebugMain_EXIT[] PROGMEM = "Start user app";
#if(DEBUG_MAIN > 1)
#undef DBG_MAIN_EX
#define DBG_MAIN_EX(block) block
const unsigned char mDebugMain_SPI[] PROGMEM = "Init spi comm";
const unsigned char mDebugMain_NET[] PROGMEM = "Init network layer";
const unsigned char mDebugMain_TFTP[] PROGMEM = "Init tftp server";
#if defined(DEBUG_BTN)
const unsigned char mDebugMain_BTN[] PROGMEM = "Init button debugging";
#endif
#endif
#endif

#endif
