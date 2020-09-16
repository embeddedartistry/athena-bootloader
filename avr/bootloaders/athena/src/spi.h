#ifndef spi_h
#define spi_h

#define SPI_WRITE (0xF0)
#define SPI_READ (0x0F)

#define SS_LOW() ETH_PORT &= ~_BV(ETH_SS)
#define SS_HIGH() ETH_PORT |= _BV(ETH_SS)

void spiInit(void);

void spiWriteReg(uint16_t address, uint8_t cb, uint8_t value);
void spiWriteWord(uint16_t address, uint8_t cb, uint16_t value);

uint8_t spiReadReg(uint16_t address, uint8_t cb);
uint16_t spiReadWord(uint16_t address, uint8_t cb);

#endif
