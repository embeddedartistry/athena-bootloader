/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: athena
 * Function: debugging progmem strings
 */

#ifndef debug_main_h
#define debug_main_h

#include <avr/pgmspace.h>

#define PP_STRINGIFY_IMPL(X) #X
#define PP_STRINGIFY(X) PP_STRINGIFY_IMPL(X)

#if(DEBUG_MAIN > 0)
#undef DBG_MAIN
#define DBG_MAIN(block) block
#define tracePGMlnMain(msg) tracePGMln(mDebugMain_PREFIX, msg)
const unsigned char mDebugMain_PREFIX[] PROGMEM = "Main: ";
const unsigned char mDebug_UpdateMode[] PROGMEM = "Update Mode Requested";
const unsigned char mDebug_GoodImage[] PROGMEM = "Good image signature detected";
const unsigned char mDebug_BadImage[] PROGMEM = "BAD image signature detected";

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
#if defined(ARDUINO_ETHERNET)
#define TITLE_BOARD_TYPE "Athena for Arduino Ethernet"
#else
#define TITLE_BOARD_TYPE "Athena for Arduino Uno"
#endif
#elif defined(__AVR_ATmega2560__)
#define TITLE_BOARD_TYPE "Athena for Arduino Mega2560"
#elif defined(__AVR_ATmega1284P__)
#define TITLE_BOARD_TYPE "Athena for Arduino Mega1284P"
#else
#define TITLE_BOARD_TYPE "Unknown MCU with athena"
#endif

#if defined(__WIZ_W5100__)
#define TITLE_WIZ_TYPE "W5100"
#elif defined(__WIZ_W5200__)
#define TITLE_WIZ_TYPE "W5200"
#elif defined(__WIZ_W5500__)
#define TITLE_WIZ_TYPE "W5500"
#endif

const unsigned char mDebugMain_TITLE[] PROGMEM =
	TITLE_BOARD_TYPE " (" TITLE_WIZ_TYPE "), Version " PP_STRINGIFY(BUILD_TAG);
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
