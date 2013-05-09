/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ariadne
 * Function: debugging progmem strings
 * Version: 0.5 "Mega2560"
 */

#ifndef debug_net_h
#define debug_net_h

#include <avr/pgmspace.h>

#if (DEBUG > 0)
#ifdef DEBUG_NET
	#undef DBG_NET
	#define DBG_NET(block) block
	#define tracePGMlnNet(msg) tracePGMln(mNetDebug_PREFIX, msg)
const unsigned char mNetDebug_PREFIX[]	PROGMEM =	" Net: ";
const unsigned char mNetDebug_EEPROM[]	PROGMEM =	"EEPROM settings";
const unsigned char mNetDebug_BUILTIN[]	PROGMEM =	"Built-in settings";
const unsigned char mNetDebug_ADDR[]	PROGMEM =	"\tAddress: ";
const unsigned char mNetDebug_SUBN[]	PROGMEM =	"\t Subnet: ";
const unsigned char mNetDebug_GW[]		PROGMEM =	"\tGateway: ";
const unsigned char mNetDebug_MAC[]		PROGMEM =	"\t    MAC: ";
const unsigned char mNetDebug_DONE[]	PROGMEM =	"Network init done";
	#if (DEBUG > 1)
		#undef DBG_NET_EX
		#define DBG_NET_EX(block) block
const unsigned char mNetDebug_COMMA[]	PROGMEM =	", ";
const unsigned char mNetDebug_NWREG[]	PROGMEM =	"netWriteReg: ";
const unsigned char mNetDebug_NRREG[]	PROGMEM =	"netReadReg: ";
	#endif
#endif
#endif

#endif