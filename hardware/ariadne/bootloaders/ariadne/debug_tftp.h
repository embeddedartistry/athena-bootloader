/* Name: debug_msg.h
 * Author: Stelios Tsampas
 * Copyright: codebender.cc
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: ariadne
 * Function: debugging progmem strings
 * Version: 0.5 "Mega2560"
 */

#ifndef debug_tftp_h
#define debug_tftp_h

#include <avr/pgmspace.h>

#if (DEBUG > 0)
#ifdef DEBUG_TFTP
	#undef DBG_TFTP
	#define DBG_TFTP(block) block
	#define tracePGMlnTftp(msg) tracePGMln(mTftpDebug_PREFIX, msg)
const unsigned char mTftpDebug_PREFIX[] PROGMEM =	"Tftp: ";
const unsigned char mTftpDebug_START[]	PROGMEM =	"Processing packet of size ";
const unsigned char mTftpDebug_OVFL[]	PROGMEM =	"Overflow";
const unsigned char mTftpDebug_RADDR[]	PROGMEM =	"Set up return address";
const unsigned char mTftpDebug_BLOCK[]	PROGMEM =	"This is block ";
const unsigned char mTftpDebug_OPCODE[]	PROGMEM =	" with opcode ";
const unsigned char mTftpDebug_DLEN[]	PROGMEM =	" and data length ";
const unsigned char mTftpDebug_OPRRQ[]	PROGMEM =	"Read request";
const unsigned char mTftpDebug_OPWRQ[]	PROGMEM =	"Write request";
const unsigned char mTftpDebug_OPDATA[]	PROGMEM =	"Data block";
const unsigned char mTftpDebug_NPORT[]	PROGMEM =	"Changed to port ";
const unsigned char mTftpDebug_FULL[]	PROGMEM =	"Flash is full";
const unsigned char mTftpDebug_WRADDR[]	PROGMEM =	"Writing data from address ";
const unsigned char mTftpDebug_PLEN[]	PROGMEM =	"Packet length adjusted to ";
const unsigned char mTftpDebug_DONE[]	PROGMEM =	"Flashing complete";
const unsigned char mTftpDebug_OPACK[]	PROGMEM =	"Acknowledge";
const unsigned char mTftpDebug_OPERR[]	PROGMEM =	"Error";
const unsigned char mTftpDebug_INVOP[]	PROGMEM =	"Invalid opcode ";
const unsigned char mTftpDebug_SACK[]	PROGMEM =	"Sent ACK";
const unsigned char mTftpDebug_SFACK[]	PROGMEM =	"Sent Final ACK";
const unsigned char mTftpDebug_RESP[]	PROGMEM =	"Response sent";
const unsigned char mTftpDebug_INIT[]	PROGMEM =	"TFTP server init done";
const unsigned char mTftpDebug_PORT[]	PROGMEM =	"\t   Port: ";
	#if (DEBUG > 1)
		#undef DBG_TFTP_EX
		#define DBG_TFTP_EX(block) block
const unsigned char mTftpDebug_NEWLINE[]	PROGMEM =	"\r\n";
const unsigned char mTftpDebug_SPACE[]	PROGMEM =	" ";
const unsigned char mTftpDebug_RPTR[]	PROGMEM =	"readPointer at position ";
const unsigned char mTftpDebug_RPOS[]	PROGMEM =	"Reading from position ";
const unsigned char mTftpDebug_BLEFT[]	PROGMEM =	"Bytes left ";
const unsigned char mTftpDebug_WRITE[]	PROGMEM =	"Writing ";
const unsigned char mTftpDebug_OFFSET[]	PROGMEM =	" at offset ";
	#endif
#endif
#endif

#endif