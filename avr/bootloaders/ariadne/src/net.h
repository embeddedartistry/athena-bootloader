#ifndef net_h
#define net_h

#if defined(__WIZ_W5100__)
	#include "w5100.h"
#elif defined(__WIZ_W5200__)
	#include "w5200.h"
#elif defined(__WIZ_W5500__)
	#include "w5500.h"
#else
	#error "Unknown PHY. Cannot find the proper network driver."
#endif

/* Network settings */
#define IP_ADDR     10,0,1,199
#define SUB_MASK    255,255,255,0
#define GW_ADDR     10,0,1,1
#define MAC_ADDR    0xDE,0xAD,0xBE,0xEF,0xFE,0xED

void netInit(void);

#endif
