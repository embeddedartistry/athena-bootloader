#ifndef serial_h
#define serial_h

#ifdef LUDICROUS_SPEED
#define BAUD_RATE 230400L
#endif

/* set the UART baud rate defaults */
#ifndef BAUD_RATE
#if F_CPU >= 8000000L
#define BAUD_RATE	115200L // Highest rate Avrdude win32 will support
#elif F_CPU >= 1000000L
#define BAUD_RATE	9600L   // 19200 also supported, but with significant error
#elif F_CPU >= 128000L
#define BAUD_RATE	4800L   // Good for 128kHz internal RC
#else
#define BAUD_RATE	1200L     // Good even at 32768Hz
#endif
#endif

/* Switch in soft UART for hard baud rates */
#if (F_CPU/BAUD_RATE) > 280 // > 57600 for 16MHz
#ifndef SOFT_UART
#define SOFT_UART
#endif
#endif

void serialInit();

void putch(uint8_t c);
void puthex(uint8_t c);

uint8_t getch(void);

#endif
