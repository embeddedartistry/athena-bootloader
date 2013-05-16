#ifndef pin_defs_h
#define pin_defs_h

/* Uno and Deumilanove */
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
	/* Onboard LED is connected to pin PB5 in Duemilanove and Uno or to PB1 in Ethernet */
	#define LED_DDR     DDRB
	#define LED_PORT    PORTB
	#define LED_PIN     PINB
	#if defined(ARDUINO_ETHERNET)
		#define LED     PINB1  //Pin 9
	#else
		#define LED     PINB5  //Pin 13
	#endif

	/* SPI port and pins */
	#define SPI_DDR		DDRB
	#define SPI_PORT	PORTB
	#define SPI_PIN		PINB

	#define SCK     PINB5  //Pin 13
	#define MISO    PINB4  //Pin 12
	#define MOSI    PINB3  //Pin 11
	#define SS      PINB2  //Pin 10
	#define ESS		PINB2  //Pin 10

	/* SD card SS pin */
	#define SD_DDR	DDRD
	#define SD_PORT	PORTD
	#define SS_PIN	PIND
	#define SD_SS	PIND4  //Pin 04

/* Mega1280 and Mega2560 */
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	/* Onboard LED is connected to pin PB7 on Arduino Mega */
	#define LED_DDR     DDRB
	#define LED_PORT    PORTB
	#define LED_PIN     PINB
	#define LED         PINB7

	/* SPI port and pins */
	#define SPI_DDR		DDRB
	#define SPI_PORT	PORTB
	#define SPI_PIN		PINB

	#define SCK		PINB1  //Pin 52
	#define MISO	PINB3  //Pin 50
	#define MOSI	PINB2  //Pin 51
	#define SS      PINB0  //Pin 53
	#define ESS		PINB4  //Pin 10

	/* SD card SS pin */
	#define SD_DDR	DDRG
	#define SD_PORT	PORTG
	#define SD_PIN	PING
	#define SD_SS	PING5  //Pin 04

/* Leonardo */
#elif defined(__AVR_ATmega32U4__)

#else
	#error "no pin definitions for MCU available"
#endif

#endif
