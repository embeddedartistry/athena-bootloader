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

#if (DEBUG_ALL > 1)
	#define DEBUG_MAIN	2
	#define DEBUG_NET	2
	#define DEBUG_TFTP	2
	#define DEBUG_VALD	2
	#define DEBUG_UTIL	2
#elif (DEBUG_ALL > 0)
	#define DEBUG_MAIN	1
	#define DEBUG_NET	1
	#define DEBUG_TFTP	1
	#define DEBUG_VALD	1
	#define DEBUG_UTIL	1
#endif

#if (DEBUG_MAIN > 0) || (DEBUG_NET > 0) || (DEBUG_TFTP > 0) \
	|| (DEBUG_VALD > 0) || (DEBUG_UTIL > 0) || (DEBUG_BTN > 0)
	#define ENABLE_DEBUG
#endif

#if defined(ENABLE_DEBUG)

#if (FLASHEND > 0x10000)
	#if defined(__AVR_ATmega2560__)
		#define PROGMEM_OFFSET 0x30000
	#elif defined(__AVR_ATmega1280__)
		#define PROGMEM_OFFSET 0x10000
	#else
		#error "Unknown MCU. Cannot define PROGMEM_OFFSET"
	#endif
#endif

void tracePGM(const void* p_msg);
void tracePGMln(const void* p_prefix, const void* p_msg);
void tracehex(uint16_t num, uint8_t len);
void trace(char* msg);
void traceln(const void* p_prefix, char* msg);

#define tracenum(num) tracehex(num, 4)
#define tracenet(num) tracehex(num, 2)

#if defined(DEBUG_BTN)
void buttonInit(void);
void button(void);
#endif

#endif

#endif
