/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ariadne
 * Function: debugging progmem strings
 * Version: 0.5 "Mega2560"
 */

#ifndef debug_main_h
#define debug_main_h

#include <avr/pgmspace.h>

#if (DEBUG > 0)

/*
 * Basic debugging for "main.c". This is alway enabled when debugging is active
 * to print general information and the steps of the bootloader.
 */
#undef DBG_MAIN
#define DBG_MAIN(block) block
#define tracePGMlnMain(msg) tracePGMln(mMainDebug_PREFIX, msg)
const unsigned char mMainDebug_PREFIX[]	PROGMEM = 	"Main: ";

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#if defined(_ARDUINO_ETHERNET)
const unsigned char mMainDebug_TITLE[]	PROGMEM = 	"Ariadne for Arduino Ethernet, Version 0.5";
#else
const unsigned char mMainDebug_TITLE[]	PROGMEM = 	"Ariadne for Arduino Uno, Version 0.5";
#endif
#elif defined(__AVR_ATmega2560__)
const unsigned char mMainDebug_TITLE[]	PROGMEM = 	"Ariadne for Arduino Mega2560, Version 0.5";
#else
const unsigned char mMainDebug_TITLE[]	PROGMEM = 	"Unknown MCU with ariadne, Version 0.5";
#endif
#ifdef DEBUG_BTN
const unsigned char mMainDebug_BTN[]	PROGMEM =	"Init button debugging";
#endif
const unsigned char mMainDebug_NET[]	PROGMEM =	"Init network layer";
const unsigned char mMainDebug_TFTP[]	PROGMEM =	"Init tftp server";
#ifdef ANNOUNCE
const unsigned char mMainDebug_ANN[]	PROGMEM =	"Init network announce";
#endif
const unsigned char mMainDebug_EXIT[]	PROGMEM = 	"Start user app";

#endif
#endif