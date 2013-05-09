#ifndef debug_h
#define debug_h

/*
 * Define DBG_* dummy macros. These disable the debugging code if they aren't
 * overriden later on in the subsytem sections.
 */
#define DBG_MAIN(block)
#define DBG_NET(block)
#define DBG_NET_EX(block)
#define DBG_TFTP(block)
#define DBG_TFTP_EX(block)
#define DBG_VALD(block)
#define DBG_UTIL(block)
#define DBG_BTN(block)

#if (DEBUG > 0)

#ifdef DEBUG_ALL
	#define DEBUG_NET
	#define DEBUG_TFTP
	#define DEBUG_VALD
	#define DEBUG_UTIL
#endif

void tracePGM(const void* p_msg);
void tracePGMln(const void* p_prefix, const void* p_msg);
void tracehex(uint16_t num, uint8_t len);
void trace(char* msg);
void traceln(const void* p_prefix, char* msg);

#define tracenum(num) tracehex(num, 4)
#define tracenet(num) tracehex(num, 2)

#ifdef DEBUG_BTN
void buttonInit(void);
void button(void);
#endif

#endif

#endif