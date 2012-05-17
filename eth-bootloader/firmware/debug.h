#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/** general debug **/
//#define _DEBUG
//#define _DEBUGMORE
/** tftp.c **/
//#define _DEBUG_TFTP
//#define _DEBUGMORE_TFTP
/** net.c **/
//#define _DEBUG_NET
//#define _DEBUGMORE_NET
/** validate.c **/
//#define _DEBUG_VALD

#ifdef _DEBUG
#define TRACE(msg) trace(msg)
void step();
void trace(char* msg);
void traceln(char* msg);
void tracenum(uint16_t num);
void debugInit();
#endif
