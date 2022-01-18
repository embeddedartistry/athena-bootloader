#include <avr/pgmspace.h>
#include <gpio.h>

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT, NOT_A_PORT, (uint16_t)&DDRB, (uint16_t)&DDRC, (uint16_t)&DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT, NOT_A_PORT, (uint16_t)&PORTB, (uint16_t)&PORTC, (uint16_t)&PORTD,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD, PD, PD, PD, PD, PD, PD, PB, /* 8 */
	PB, PB, PB, PB, PB, PC, /* 14 */
	PC, PC, PC, PC, PC,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), _BV(0), /* 8, port B */
	_BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(0), /* 14, port C */
	_BV(1), _BV(2), _BV(3), _BV(4), _BV(5),
};

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,		 (uint16_t)&DDRA, (uint16_t)&DDRB, (uint16_t)&DDRC, (uint16_t)&DDRD,
	(uint16_t)&DDRE, (uint16_t)&DDRF, (uint16_t)&DDRG, (uint16_t)&DDRH, NOT_A_PORT,
	(uint16_t)&DDRJ, (uint16_t)&DDRK, (uint16_t)&DDRL,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,		  (uint16_t)&PORTA, (uint16_t)&PORTB, (uint16_t)&PORTC, (uint16_t)&PORTD,
	(uint16_t)&PORTE, (uint16_t)&PORTF, (uint16_t)&PORTG, (uint16_t)&PORTH, NOT_A_PORT,
	(uint16_t)&PORTJ, (uint16_t)&PORTK, (uint16_t)&PORTL,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST
	// -------------------------------------------
	PE, // PE 0 ** 0 ** USART0_RX
	PE, // PE 1 ** 1 ** USART0_TX
	PE, // PE 4 ** 2 ** PWM2
	PE, // PE 5 ** 3 ** PWM3
	PG, // PG 5 ** 4 ** PWM4
	PE, // PE 3 ** 5 ** PWM5
	PH, // PH 3 ** 6 ** PWM6
	PH, // PH 4 ** 7 ** PWM7
	PH, // PH 5 ** 8 ** PWM8
	PH, // PH 6 ** 9 ** PWM9
	PB, // PB 4 ** 10 ** PWM10
	PB, // PB 5 ** 11 ** PWM11
	PB, // PB 6 ** 12 ** PWM12
	PB, // PB 7 ** 13 ** PWM13
	PJ, // PJ 1 ** 14 ** USART3_TX
	PJ, // PJ 0 ** 15 ** USART3_RX
	PH, // PH 1 ** 16 ** USART2_TX
	PH, // PH 0 ** 17 ** USART2_RX
	PD, // PD 3 ** 18 ** USART1_TX
	PD, // PD 2 ** 19 ** USART1_RX
	PD, // PD 1 ** 20 ** I2C_SDA
	PD, // PD 0 ** 21 ** I2C_SCL
	PA, // PA 0 ** 22 ** D22
	PA, // PA 1 ** 23 ** D23
	PA, // PA 2 ** 24 ** D24
	PA, // PA 3 ** 25 ** D25
	PA, // PA 4 ** 26 ** D26
	PA, // PA 5 ** 27 ** D27
	PA, // PA 6 ** 28 ** D28
	PA, // PA 7 ** 29 ** D29
	PC, // PC 7 ** 30 ** D30
	PC, // PC 6 ** 31 ** D31
	PC, // PC 5 ** 32 ** D32
	PC, // PC 4 ** 33 ** D33
	PC, // PC 3 ** 34 ** D34
	PC, // PC 2 ** 35 ** D35
	PC, // PC 1 ** 36 ** D36
	PC, // PC 0 ** 37 ** D37
	PD, // PD 7 ** 38 ** D38
	PG, // PG 2 ** 39 ** D39
	PG, // PG 1 ** 40 ** D40
	PG, // PG 0 ** 41 ** D41
	PL, // PL 7 ** 42 ** D42
	PL, // PL 6 ** 43 ** D43
	PL, // PL 5 ** 44 ** D44
	PL, // PL 4 ** 45 ** D45
	PL, // PL 3 ** 46 ** D46
	PL, // PL 2 ** 47 ** D47
	PL, // PL 1 ** 48 ** D48
	PL, // PL 0 ** 49 ** D49
	PB, // PB 3 ** 50 ** SPI_MISO
	PB, // PB 2 ** 51 ** SPI_MOSI
	PB, // PB 1 ** 52 ** SPI_SCK
	PB, // PB 0 ** 53 ** SPI_SS
	PF, // PF 0 ** 54 ** A0
	PF, // PF 1 ** 55 ** A1
	PF, // PF 2 ** 56 ** A2
	PF, // PF 3 ** 57 ** A3
	PF, // PF 4 ** 58 ** A4
	PF, // PF 5 ** 59 ** A5
	PF, // PF 6 ** 60 ** A6
	PF, // PF 7 ** 61 ** A7
	PK, // PK 0 ** 62 ** A8
	PK, // PK 1 ** 63 ** A9
	PK, // PK 2 ** 64 ** A10
	PK, // PK 3 ** 65 ** A11
	PK, // PK 4 ** 66 ** A12
	PK, // PK 5 ** 67 ** A13
	PK, // PK 6 ** 68 ** A14
	PK, // PK 7 ** 69 ** A15
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	// PIN IN PORT
	// -------------------------------------------
	_BV(0), // PE 0 ** 0 ** USART0_RX
	_BV(1), // PE 1 ** 1 ** USART0_TX
	_BV(4), // PE 4 ** 2 ** PWM2
	_BV(5), // PE 5 ** 3 ** PWM3
	_BV(5), // PG 5 ** 4 ** PWM4
	_BV(3), // PE 3 ** 5 ** PWM5
	_BV(3), // PH 3 ** 6 ** PWM6
	_BV(4), // PH 4 ** 7 ** PWM7
	_BV(5), // PH 5 ** 8 ** PWM8
	_BV(6), // PH 6 ** 9 ** PWM9
	_BV(4), // PB 4 ** 10 ** PWM10
	_BV(5), // PB 5 ** 11 ** PWM11
	_BV(6), // PB 6 ** 12 ** PWM12
	_BV(7), // PB 7 ** 13 ** PWM13
	_BV(1), // PJ 1 ** 14 ** USART3_TX
	_BV(0), // PJ 0 ** 15 ** USART3_RX
	_BV(1), // PH 1 ** 16 ** USART2_TX
	_BV(0), // PH 0 ** 17 ** USART2_RX
	_BV(3), // PD 3 ** 18 ** USART1_TX
	_BV(2), // PD 2 ** 19 ** USART1_RX
	_BV(1), // PD 1 ** 20 ** I2C_SDA
	_BV(0), // PD 0 ** 21 ** I2C_SCL
	_BV(0), // PA 0 ** 22 ** D22
	_BV(1), // PA 1 ** 23 ** D23
	_BV(2), // PA 2 ** 24 ** D24
	_BV(3), // PA 3 ** 25 ** D25
	_BV(4), // PA 4 ** 26 ** D26
	_BV(5), // PA 5 ** 27 ** D27
	_BV(6), // PA 6 ** 28 ** D28
	_BV(7), // PA 7 ** 29 ** D29
	_BV(7), // PC 7 ** 30 ** D30
	_BV(6), // PC 6 ** 31 ** D31
	_BV(5), // PC 5 ** 32 ** D32
	_BV(4), // PC 4 ** 33 ** D33
	_BV(3), // PC 3 ** 34 ** D34
	_BV(2), // PC 2 ** 35 ** D35
	_BV(1), // PC 1 ** 36 ** D36
	_BV(0), // PC 0 ** 37 ** D37
	_BV(7), // PD 7 ** 38 ** D38
	_BV(2), // PG 2 ** 39 ** D39
	_BV(1), // PG 1 ** 40 ** D40
	_BV(0), // PG 0 ** 41 ** D41
	_BV(7), // PL 7 ** 42 ** D42
	_BV(6), // PL 6 ** 43 ** D43
	_BV(5), // PL 5 ** 44 ** D44
	_BV(4), // PL 4 ** 45 ** D45
	_BV(3), // PL 3 ** 46 ** D46
	_BV(2), // PL 2 ** 47 ** D47
	_BV(1), // PL 1 ** 48 ** D48
	_BV(0), // PL 0 ** 49 ** D49
	_BV(3), // PB 3 ** 50 ** SPI_MISO
	_BV(2), // PB 2 ** 51 ** SPI_MOSI
	_BV(1), // PB 1 ** 52 ** SPI_SCK
	_BV(0), // PB 0 ** 53 ** SPI_SS
	_BV(0), // PF 0 ** 54 ** A0
	_BV(1), // PF 1 ** 55 ** A1
	_BV(2), // PF 2 ** 56 ** A2
	_BV(3), // PF 3 ** 57 ** A3
	_BV(4), // PF 4 ** 58 ** A4
	_BV(5), // PF 5 ** 59 ** A5
	_BV(6), // PF 6 ** 60 ** A6
	_BV(7), // PF 7 ** 61 ** A7
	_BV(0), // PK 0 ** 62 ** A8
	_BV(1), // PK 1 ** 63 ** A9
	_BV(2), // PK 2 ** 64 ** A10
	_BV(3), // PK 3 ** 65 ** A11
	_BV(4), // PK 4 ** 66 ** A12
	_BV(5), // PK 5 ** 67 ** A13
	_BV(6), // PK 6 ** 68 ** A14
	_BV(7), // PK 7 ** 69 ** A15
};
/** Leonardo */
#elif defined(__AVR_ATmega32U4__)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,		 NOT_A_PORT,	  (uint16_t)&DDRB, (uint16_t)&DDRC,
	(uint16_t)&DDRD, (uint16_t)&DDRE, (uint16_t)&DDRF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0 - PD2
	PD, // D1 - PD3
	PD, // D2 - PD1
	PD, // D3 - PD0
	PD, // D4 - PD4
	PC, // D5 - PC6
	PD, // D6 - PD7
	PE, // D7 - PE6

	PB, // D8 - PB4
	PB, // D9 - PB5
	PB, // D10 - PB6
	PB, // D11 - PB7
	PD, // D12 - PD6
	PC, // D13 - PC7

	PB, // D14 - MISO - PB3
	PB, // D15 - SCK - PB1
	PB, // D16 - MOSI - PB2
	PB, // D17 - SS - PB0

	PF, // D18 - A0 - PF7
	PF, // D19 - A1 - PF6
	PF, // D20 - A2 - PF5
	PF, // D21 - A3 - PF4
	PF, // D22 - A4 - PF1
	PF, // D23 - A5 - PF0

	PD, // D24 / D4 - A6 - PD4
	PD, // D25 / D6 - A7 - PD7
	PB, // D26 / D8 - A8 - PB4
	PB, // D27 / D9 - A9 - PB5
	PB, // D28 / D10 - A10 - PB6
	PD, // D29 / D12 - A11 - PD6
	PD, // D30 / TX Led - PD5
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,		  NOT_A_PORT,		(uint16_t)&PORTB, (uint16_t)&PORTC,
	(uint16_t)&PORTD, (uint16_t)&PORTE, (uint16_t)&PORTF,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(2), // D0 - PD2
	_BV(3), // D1 - PD3
	_BV(1), // D2 - PD1
	_BV(0), // D3 - PD0
	_BV(4), // D4 - PD4
	_BV(6), // D5 - PC6
	_BV(7), // D6 - PD7
	_BV(6), // D7 - PE6

	_BV(4), // D8 - PB4
	_BV(5), // D9 - PB5
	_BV(6), // D10 - PB6
	_BV(7), // D11 - PB7
	_BV(6), // D12 - PD6
	_BV(7), // D13 - PC7

	_BV(3), // D14 - MISO - PB3
	_BV(1), // D15 - SCK - PB1
	_BV(2), // D16 - MOSI - PB2
	_BV(0), // D17 - SS - PB0

	_BV(7), // D18 - A0 - PF7
	_BV(6), // D19 - A1 - PF6
	_BV(5), // D20 - A2 - PF5
	_BV(4), // D21 - A3 - PF4
	_BV(1), // D22 - A4 - PF1
	_BV(0), // D23 - A5 - PF0

	_BV(4), // D24 / D4 - A6 - PD4
	_BV(7), // D25 / D6 - A7 - PD7
	_BV(4), // D26 / D8 - A8 - PB4
	_BV(5), // D27 / D9 - A9 - PB5
	_BV(6), // D28 / D10 - A10 - PB6
	_BV(6), // D29 / D12 - A11 - PD6
	_BV(5), // D30 / TX Led - PD5
};
/** 
 * Mega1284P
 *  this pin mapping will work with the below layout (Bobuino)
 *  other boards with Mega1284P might have different mapping
 *  between the Arduino "virtual" pins to the hardware pins
**/
#elif defined(__AVR_ATmega1284P__)

// ATMEL ATMEGA1284P on OSWIN (Bobuino pin layout)
//
//                       +---\/---+
//           (D 4) PB0 1 |        | 40 PA0 (D 21) AI 7
//           (D 5) PB1 2 |        | 39 PA1 (D 20) AI 6
//      INT2 (D 6) PB2 3 |        | 38 PA2 (D 19) AI 5
//       PWM (D 7) PB3 4 |        | 37 PA3 (D 18) AI 4
//   PWM/SS (D 10) PB4 5 |        | 36 PA4 (D 17) AI 3
//     MOSI (D 11) PB5 6 |        | 35 PA5 (D 16) AI 2
// PWM/MISO (D 12) PB6 7 |        | 34 PA6 (D 15) AI 1
//  PWM/SCK (D 13) PB7 8 |        | 33 PA7 (D 14) AI 0
//                 RST 9 |        | 32 AREF
//                VCC 10 |        | 31 GND
//                GND 11 |        | 30 AVCC
//              XTAL2 12 |        | 29 PC7 (D 29)
//              XTAL1 13 |        | 28 PC6 (D 28)
//      RX0 (D 0) PD0 14 |        | 27 PC5 (D 27) TDI
//      TX0 (D 1) PD1 15 |        | 26 PC4 (D 26) TDO
// INT0 RX1 (D 2) PD2 16 |        | 25 PC3 (D 25) TMS
// INT1 TX1 (D 3) PD3 17 |        | 24 PC2 (D 24) TCK
//     PWM (D 30) PD4 18 |        | 23 PC1 (D 23) SDA
//      PWM (D 8) PD5 19 |        | 22 PC0 (D 22) SCL
//      PWM (D 9) PD6 20 |        | 21 PD7 (D 31) PWM
//                       +--------+
//

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
  PD, // D0
  PD, // D1
  PD, // D2
  PD, // D3
  PB, // D4
  PB, // D5
  PB, // D6
  PB, // D7
  PD, // D8
  PD, // D9
  PB, // D10
  PB, // D11
  PB, // D12
  PB, // D13
  PA, // D14
  PA, // D15
  PA, // D16
  PA, // D17
  PA, // D18
  PA, // D19
  PA, // D20
  PA, // D21
  PC, // D22
  PC, // D23
  PC, // D24
  PC, // D25
  PC, // D26
  PC, // D27
  PC, // D28
  PC, // D29
  PD, // D30
  PD, // D31
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
  _BV(0), // D0 PD0
  _BV(1), // D1 PD1
  _BV(2), // D2 PD2
  _BV(3), // D3 PD3
  _BV(0), // D4 PB0
  _BV(1), // D5 PB1
  _BV(2), // D6 PB2
  _BV(3), // D7 PB3
  _BV(5), // D8 PD5
  _BV(6), // D9 PD6
  _BV(4), // D10 PB4
  _BV(5), // D11 PB5
  _BV(6), // D12 PB6
  _BV(7), // D13 PB7
  _BV(7), // D14 PA7
  _BV(6), // D15 PA6
  _BV(5), // D16 PA5
  _BV(4), // D17 PA4
  _BV(3), // D18 PA3
  _BV(2), // D19 PA2
  _BV(1), // D20 PA1
  _BV(0), // D21 PA0
  _BV(0), // D22 PC0
  _BV(1), // D23 PC1
  _BV(2), // D24 PC2
  _BV(3), // D25 PC3
  _BV(4), // D26 PC4
  _BV(5), // D27 PC5
  _BV(6), // D28 PC6
  _BV(7), // D29 PC7
  _BV(4), // D30 PD4
  _BV(7), // D31 PD7
};

#endif
