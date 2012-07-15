#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/** general debug **/
//#define _DEBUG
//#define _DEBUGMORE
/** debugging steps **/
//#define DEBUG_STEP
/** tftp.c **/
//#define _DEBUG_TFTP
//#define _DEBUGMORE_TFTP
/** net.c **/
//#define _DEBUG_NET
//#define _DEBUGMORE_NET
/** validate.c **/
//#define _DEBUG_VALD
/** optiboot.c **/
//#define _DEBUG_OPTI

//#ifdef _DEBUG
void trace(char *msg);
void traceln(char *msg);
void tracenum(uint16_t num);
#ifdef _DEBUG_STEP
void stepInit();
void step();
#endif
//#endif