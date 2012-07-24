#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/** general debug **/
//#define _DEBUG
//#define _DEBUGMORE
#ifdef _DEBUGMORE
	#define _DEBUG
#endif
#ifdef _DEBUG
	#define _VERBOSE
#endif
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

#ifdef _VERBOSE
void trace(char *msg);
void traceln(char *msg);
void tracehex(uint16_t num, uint8_t len);
void tracedec(uint16_t num, uint8_t len);
#define tracenum(num) tracehex(num, 4)
#define tracenet(num) tracehex(num, 2)
#ifdef _DEBUG_STEP
void stepInit();
void step();
#endif
#endif