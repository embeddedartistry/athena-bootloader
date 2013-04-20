#ifndef net_h
#define net_h

/* Network settings */
#define IP_ADDR     192,168,1,128
#define SUB_MASK    255,255,255,0
#define GW_ADDR     192,168,1,1
#define MAC_ADDR    0xDE,0xAD,0xBE,0xEF,0xFE,0xED

#define SPI_WRITE (0xF0)
#define SPI_READ  (0x0F)


void netWriteReg(uint16_t address, uint8_t value);
uint8_t netReadReg(uint16_t address);
uint16_t netReadWord(uint16_t address);
void netWriteWord(uint16_t address, uint16_t value);
void netInit(void);

#endif
