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

// Included for default networking address definitions
#include <AthenaEEPROM/AthenaEEPROM_defs.h>

/* Network settings */
#ifndef IP_ADDR
#define IP_ADDR DEFAULT_IP_ADDR
#endif

#ifndef SUB_MASK
#define SUB_MASK DEFAULT_SUB_MASK
#endif

#ifndef GW_ADDR
#define GW_ADDR DEFAULT_GW_ADDR
#endif

#ifndef MAC_ADDR
#define MAC_ADDR DEFAULT_MAC_ADDR
#endif

void netInit(void);

#endif
