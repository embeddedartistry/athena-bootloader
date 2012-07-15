#include <avr/io.h>
#include <avr/eeprom.h>

// Network settings
#define IP_ADDR     192,168,1,128
#define SUBNET_MASK 255,255,255,0
#define GW_ADDR     192,168,1,1
#define MAC_ADDR    0xDE,0xAD,0xBE,0xEF,0xFE,0xED

#define EEPROM_SIG_1    ((uint8_t*)0)
#define EEPROM_SIG_2    ((uint8_t*)1)
#define EEPROM_IMG_STAT ((uint8_t*)2)
#define EEPROM_DATA     ((uint8_t*)3)
#define EEPROM_GATEWAY  ((uint8_t*)3)
#define EEPROM_MAC      ((uint8_t*)7)
#define EEPROM_IP       ((uint8_t*)13)

#define EEPROM_SIG_1_VALUE (0x55)
#define EEPROM_SIG_2_VALUE (0xAA)
#define EEPROM_IMG_OK_VALUE (0xBB)
#define EEPROM_IMG_BAD_VALUE (0xFF)


#define SPI_WRITE (0xF0)
#define SPI_READ (0x0F)

#define REGISTER_BLOCK_SIZE 28


void netWriteReg(uint16_t address, uint8_t value);
uint8_t netReadReg(uint16_t address);
uint16_t netReadWord(uint16_t address);
void netWriteWord(uint16_t address, uint16_t value);

void netInit();
