#ifndef util_h
#define util_h

#define TIMEOUT 3

#ifndef LED_START_FLASHES
	#define LED_START_FLASHES 0
#endif

#define TRUE  1
#define FALSE 0

#define TFTPBOOT_MAJVER 0
#define TFTPBOOT_MINVER 3
#define MAKESTR(a) #a
#define MAKEVER(a, b) MAKESTR(a*256+b)

/* FIXME: For some reason avr-gcc 4.7.1 decides that a ".word" has size of 4
 * and a ".byte" has size of 2 registers. For now this will stay like this
 * and change the -Wl,--section-start=.version=0x7ffe in the Makefile to 7ffc */
// asm("  .section .version\n"
//     "tftpboot_version: .word " MAKEVER(TFTPBOOT_MAJVER, TFTPBOOT_MINVER) "\n"
//     "  .section .text\n");

void updateLed(void);
void resetTick(void);
uint8_t timedOut(void);

#endif