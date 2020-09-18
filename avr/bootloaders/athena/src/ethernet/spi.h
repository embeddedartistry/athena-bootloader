#ifndef spi_h
#define spi_h

#include <stdint.h>

#define SPI_WRITE (0xF0)
#define SPI_READ (0x0F)

#define SS_LOW() spi_cs_low()
#define SS_HIGH() spi_cs_high()

void spiInit(void);

void spiWriteReg(uint16_t address, uint8_t cb, uint8_t value);
void spiWriteWord(uint16_t address, uint8_t cb, uint16_t value);

uint8_t spiReadReg(uint16_t address, uint8_t cb);
uint16_t spiReadWord(uint16_t address, uint8_t cb);

void spi_cs_low(void);
void spi_cs_high(void);

#endif
