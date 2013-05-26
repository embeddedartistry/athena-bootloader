#ifndef net_h
#define net_h

/* Network settings */
#define IP_ADDR     192,168,1,128
#define SUB_MASK    255,255,255,0
#define GW_ADDR     192,168,1,254
#define MAC_ADDR    0xDE,0xAD,0xBE,0xEF,0xFE,0xED

void netInit(void);

#endif
