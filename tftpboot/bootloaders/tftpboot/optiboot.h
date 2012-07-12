#include <inttypes.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#if defined(__AVR_ATmega168__)
	#define RAMSTART (0x100)
	#define NRWWSTART (0x3800)
#elif defined(__AVR_ATmega328P__)
	#define RAMSTART (0x100)
	#define NRWWSTART (0x7000)
#elif defined (__AVR_ATmega644P__)
	#define RAMSTART (0x100)
	#define NRWWSTART (0xE000)
#elif defined(__AVR_ATtiny84__)
	#define RAMSTART (0x100)
	#define NRWWSTART (0x0000)
#elif defined(__AVR_ATmega1280__)
	#define RAMSTART (0x200)
	#define NRWWSTART (0xE000)
#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__)
	#define RAMSTART (0x100)
	#define NRWWSTART (0x1800)
#endif

/* C zero initialises all global variables. However, that requires */
/* These definitions are NOT zero initialised, but that doesn't matter */
/* This allows us to drop the zero init code, saving us memory */
#define buff    ((uint8_t*)(RAMSTART))
#ifdef VIRTUAL_BOOT_PARTITION
	#define rstVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+4))
	#define wdtVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+6))
#endif

#define TFTPBOOT_MAJVER 0
#define TFTPBOOT_MINVER 2
#define MAKESTR(a) #a
#define MAKEVER(a, b) MAKESTR(a*256+b)

/* FIXME: For some reason avr-gcc 4.7.1 decides that a ".word" has size of 4
 * and a ".byte" has size of 2 registers. For now this will stay like this
 * and change the -Wl,--section-start=.version=0x7ffe in the Makefile to 7ffc */
asm("  .section .version\n"
	"tftpboot_version:  .word " MAKEVER(TFTPBOOT_MAJVER, TFTPBOOT_MINVER) "\n"
	"  .section .text\n");

/* Serial status flag, it is set to TRUE if flashing from
 * serial is currently active */
uint8_t serialFlashing;

uint8_t serialPoll();
