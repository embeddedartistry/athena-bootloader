/* Name: main.c
 * Author: .
 * Copyright: Arduino
 * License: GPL http://www.gnu.org/licenses/gpl-2.0.html
 * Project: tftpboot
 * Function: Bootloader core
 * Version: 0.2 support for USB flashing
 */

#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "debug.h"
#include "debug_main.h"
#include "net.h"
#include "neteeprom.h"
#include "serial.h"
#include "spi.h"
#include "tftp.h"
#include "util.h"
#include "watchdog.h"

int main(void) __attribute__((OS_main)) __attribute__((section(".init9")));
void appStart(void) __attribute__((naked));
void check_and_update_eeprom_version(void);
void check_and_update_bootloader_version(void);

bool update_requested = false;

int main(void)
{
	uint8_t ch;

	/* This code makes the following assumptions:
	 * No interrupts will execute
	 * SP points to RAMEND
	 * r1 contains zero
	 * If not, uncomment the following instructions. */
	// cli();
	asm volatile("clr __zero_reg__");
#if defined(__AVR_ATmega8__)
	SP = RAMEND; // This is done by hardware reset
#endif

	/* Disable the watchdog timer to prevent
	 * eternal reset loop of doom and despair */
	ch = MCUSR;
	MCUSR = 0;
	if(ch & (_BV(WDRF) | _BV(BORF) | _BV(PORF)))
	{
		if(eeprom_read_byte((uint8_t*)NETEEPROM_IMG_STAT) == NETEEPROM_IMG_OK_VALUE)
		{
			wdt_disable();
			appStart();
		}
	}
	wdt_enable(WDTO_8S);

	// Check if an update has been requested. If so, restore the image status to OK_VALUE
	// in case a new image is not received over the network.
	if(eeprom_read_byte((uint8_t*)NETEEPROM_IMG_STAT) == NETEEPROM_ENTER_UPDATE_MODE_VALUE)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_IMG_STAT, NETEEPROM_IMG_OK_VALUE);
		update_requested = true;
	}

	// Prescaler=0, ClkIO Period = 62,5ns
	// TCCR1B values:
	// 0x01 -> ClkIO/1 -> 62,5ns period, 4ms max
	// 0x02 -> ClkIO/8 -> 500ns period, 32ms max
	// 0X03 -> ClkIO/64 -> 4us period, 256ms max
	// 0x04 -> ClkIO/256 -> 16us period, 1024ms max
	// 0x05 -> ClkIO/1024 -> 64us period, 4096ms max
	// Set up Timer 1 as timekeeper for LED flashing
	TCCR1B = _BV(CS12); // Same thing as TCCR1B = 0x04;

	check_and_update_eeprom_version();
	check_and_update_bootloader_version();

	/* Initialize UART communication */
	serialInit();
	// Print the program name, hardware type, and software version
	DBG_MAIN(tracePGMlnMain(mDebugMain_TITLE);)

	DBG_BTN(DBG_MAIN_EX(tracePGMlnMain(mDebugMain_BTN);) buttonInit();)

	/* Initalize SPI communication */
	DBG_MAIN_EX(tracePGMlnMain(mDebugMain_SPI);)
	spiInit();

	/* Initialize networking */
	DBG_MAIN_EX(tracePGMlnMain(mDebugMain_NET);)
	netInit();

	/* Initialize the UDP socket for tftp */
	DBG_MAIN_EX(tracePGMlnMain(mDebugMain_TFTP);)
	tftpInit();

	serialFlashing = FALSE;
	tftpFlashing = FALSE;

	for(;;)
	{
		// If there is no serial flashing under way, poll tftp
		if(!serialFlashing && !tftpInitError)
		{
			// If tftp recieved a FINAL_ACK, break
			if(tftpPoll() == 0)
			{
				break;
			}
		}

		// If there is no tftp flashing, poll serial
		if(!tftpFlashing)
		{
			// If flashing is done, exit the loop & boot the app
			if(serialPoll() == 0)
			{
				// We need to delay to give avrdude time to cleanly finish
				// Failure to delay can cause a checksum error on the
				// final command, which borks the upload process from the IDE
				// side of the world
				_delay_ms(1);
				break;
			}
		}

		if(timedOut())
		{
			if(eeprom_read_byte((uint8_t*)NETEEPROM_IMG_STAT) == NETEEPROM_IMG_OK_VALUE &&
				update_requested == false)
			{
				// Note that we only want to escape this loop if IMG_OK_VALUE is a match
				// and update_requested == false; otherwise, we need to keep waiting for
				// an image.
				break;
			}

			// TODO: determine the conditions for reseting server OR reseting socket
			if(tftpFlashing == TRUE)
			{
				// Delete first page of flash memory
				boot_page_erase(0);
				// Reinitialize TFTP
				tftpInit();
				// Reset the timeout counter
				resetTick();
				// Unset tftp flag
				tftpFlashing = FALSE;
			}
		}
		wdt_reset();
		/* Blink the notification led */
		wdt_reset(); // Required so it doesn`t hang.
		updateLed();
	}

	/* Exit to user application */
	wdt_disable();
	appStart();
	// return(0); /* never reached */
}

void appStart(void)
{
	asm volatile("clr    r30     \n\t"
				 "clr    r31     \n\t"
				 "ijmp   \n\t");
}

void check_and_update_eeprom_version()
{
	/* Write EEPROM format version information if different from the defined values */
	if(eeprom_read_byte((uint8_t*)NETEEPROM_MAJVER) != ATHENA_EEPROM_FORMAT_MAJVER)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_MAJVER, ATHENA_EEPROM_FORMAT_MAJVER);
	}

	if(eeprom_read_byte((uint8_t*)NETEEPROM_MINVER) != ATHENA_EEPROM_FORMAT_MINVER)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_MINVER, ATHENA_EEPROM_FORMAT_MINVER);
	}
}

void check_and_update_bootloader_version()
{
	/* Write Athena version information if different from the defined values */
	if(eeprom_read_byte((uint8_t*)NETEEPROM_ATHENA_VER_MAJOR) != BUILD_MAJOR_VER)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_ATHENA_VER_MAJOR, BUILD_MAJOR_VER);
	}

	if(eeprom_read_byte((uint8_t*)NETEEPROM_ATHENA_VER_MINOR) != BUILD_MINOR_VER)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_ATHENA_VER_MINOR, BUILD_MINOR_VER);
	}

	if(eeprom_read_byte((uint8_t*)NETEEPROM_ATHENA_VER_PATCH) != BUILD_PATCH_VER)
	{
		eeprom_write_byte((uint8_t*)NETEEPROM_ATHENA_VER_PATCH, BUILD_PATCH_VER);
	}
}
