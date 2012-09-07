#ifndef optiboot_h
#define optiboot_h

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

/* This define cause the bootloader to hang during flashing
 * so it has been replaced with an uint8_t array of size 256
 * inside the programming phase */
//#define buff    ((uint8_t*)(RAMSTART))

/* C zero initialises all global variables. However, that requires
 * These definitions are NOT zero initialised, but that doesn't matter
 * This allows us to drop the zero init code, saving us memory */
#ifdef VIRTUAL_BOOT_PARTITION
	#define rstVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+4))
	#define wdtVect (*(uint16_t*)(RAMSTART+SPM_PAGESIZE*2+6))
#endif

/* Serial status flag, it is set to TRUE if flashing from
 * serial is currently active */
uint8_t serialFlashing;

uint8_t serialPoll();

#endif