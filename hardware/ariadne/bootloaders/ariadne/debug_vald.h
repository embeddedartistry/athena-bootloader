/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ariadne
 * Function: debugging progmem strings
 * Version: 0.5 "Mega2560"
 */

#ifndef debug_vald_h
#define debug_vald_h

#include <avr/pgmspace.h>

#if (DEBUG > 0)
#if defined(DEBUG_VALD)
	#undef DBG_VALD
	#define DBG_VALD(block) block
	#define tracePGMlnVald(msg) tracePGMln(mValdDebug_PREFIX, msg)
const unsigned char mValdDebug_PREFIX[] PROGMEM =	"Vald: ";
const unsigned char mValdDebug_VALID[]	PROGMEM =	"Valid image";
const unsigned char mValdDebug_FAILED[]	PROGMEM =	"Failed at ";
const unsigned char mValdDebug_WITH[]	PROGMEM =	" with ";
const unsigned char mValdDebug_0x0C[]	PROGMEM =	" instead of 0x0C";
const unsigned char mValdDebug_0x94[]	PROGMEM =	" instead of 0x94";
	#endif
#endif

#endif