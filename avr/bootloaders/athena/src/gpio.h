#ifndef GPIO_H_
#define GPIO_H_

#include "util.h" // TODO: refactor util.h so this isn't needed; used for PROGMEM_OFFSET definition
#include <avr/pgmspace.h>
#include <stdint.h>

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];
extern const uint16_t PROGMEM port_to_mode_PGM[];

// Helper macros for Mega / Uno differentiation
#if(FLASHEND > 0x10000)
/* 0x30000 was added to fix the issues of progmem with >64Kb flash.
 * 0x30000 is specific to atmega2560 and won't work on smaller or larger flashes.
 * I should find a way to macro the calculation of this value
 */
#define ADJUSTED_PGM_READ_BYTE(P) pgm_read_byte_far(PROGMEM_OFFSET + P)
#define ADJUSTED_PGM_READ_WORD(P) pgm_read_word_far(PROGMEM_OFFSET + P)
#else
#define ADJUSTED_PGM_READ_BYTE(P) pgm_read_byte(P)
#define ADJUSTED_PGM_READ_WORD(P) pgm_read_word(P)
#endif

// Pulled from AVR Core - Arduino.h
// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
//
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) (ADJUSTED_PGM_READ_BYTE(digital_pin_to_port_PGM + (P)))
#define digitalPinToBitMask(P) (ADJUSTED_PGM_READ_BYTE(digital_pin_to_bit_mask_PGM + (P)))
#define portModeRegister(P) ((volatile uint8_t*)(ADJUSTED_PGM_READ_WORD(port_to_mode_PGM + (P))))
#define portOutputRegister(P) \
	((volatile uint8_t*)(ADJUSTED_PGM_READ_WORD(port_to_output_PGM + (P))))
#define portModeRegister(P) ((volatile uint8_t*)(ADJUSTED_PGM_READ_WORD(port_to_mode_PGM + (P))))

/** Pin definitions for SPI and leds */
/** Uno and Deumilanove */
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
/** SPI port and pins */
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK PINB5 // Pin 13
#define MISO PINB4 // Pin 12
#define MOSI PINB3 // Pin 11
#define SS PINB2 // Pin 10
/** Ethernet Shield SS pin */
#define ETH_DDR DDRB
#define ETH_PORT PORTB
#define ETH_SS PINB2 // Pin 10
/** SD card SS pin */
#define SD_DDR DDRD
#define SD_PORT PORTD
#define SD_SS PIND4 // Pin 04
/** Onboard LED is connected to pin PB5 in Duemilanove and Uno or to PB1 in Ethernet */
#define LED_DDR DDRB
#define LED_PORT PORTB
#if defined(ARDUINO_ETHERNET)
#define LED PINB1 // Pin 9
#else
#define LED PINB5 // Pin 13
#endif

/** Mega1280 and Mega2560 */
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
/** SPI port and pins */
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK PINB1 // Pin 52
#define MISO PINB3 // Pin 50
#define MOSI PINB2 // Pin 51
#define SS PINB0 // Pin 53
/** Ethernet Shield SS pin */
#define ETH_DDR DDRB
#define ETH_PORT PORTB
#define ETH_SS PINB4 // Pin 10
/** SD card SS pin */
#define SD_DDR DDRG
#define SD_PORT PORTG
#define SD_SS PING5 // Pin 04
/** Onboard LED is connected to pin PB7 on Arduino Mega */
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED PINB7

/** Mega1284P */
#elif defined(__AVR_ATmega1284P__)
/** SPI port and pins */
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK PINB7 // Pin 8
#define MISO PINB6 // Pin 7
#define MOSI PINB5 // Pin 6
#define SS PINB4 // Pin 5
/** Ethernet Shield SS pin */
#define ETH_DDR DDRB
#define ETH_PORT PORTB
#define ETH_SS PINB4 // Pin 5
/** SD card SS pin */
#define SD_DDR DDRC
#define SD_PORT PORTC
#define SD_SS PINC7 // Pin 04
/** Onboard RED LED is connected to pin PD7 on SEM 1284P board */
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED PIND7

/** Leonardo */
#elif defined(__AVR_ATmega32U4__)
/** SPI port and pins */
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK PINB1 // Pin 15
#define MISO PINB3 // Pin 14
#define MOSI PINB2 // Pin 16
#define SS PINB0 // Pin 17

/** Ethernet Shield SS pin */
#define ETH_DDR DDRB
#define ETH_PORT PORTB
#define ETH_SS PINB6 // Pin 10

/** SD card SS pin */
#define SD_DDR DDRD
#define SD_PORT PORTD

#define SD_SS PIND4 // Pin 04

/** Onboard LED is connected to pin PC7 on Arduino Leonardo Ethernet */
#define LED_DDR DDRC
#define LED_PORT PORTC
#define LED PINC7
#else
#error "No pin definitions for MCU available"
#endif

#endif // GPIO_H_
