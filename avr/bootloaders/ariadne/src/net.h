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
#ifndef IP_ADDR
#define IP_ADDR 10, 0, 1, 199
#endif

#ifndef SUB_MASK
#define SUB_MASK 255, 255, 255, 0
#endif

#ifndef GW_ADDR
#define GW_ADDR 10, 0, 1, 1
#endif

#ifndef MAC_ADDR
#define MAC_ADDR 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#endif

void netInit(void);

#endif
