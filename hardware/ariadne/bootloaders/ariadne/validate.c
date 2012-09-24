/* Validate first sector is a genuine image */

#include <stdint.h>

#include "validate.h"
#include "debug.h"
#include "serial.h"


uint8_t validImage(uint8_t* base)
{
	/* Check that a jump table is present in the first flash sector */
	uint8_t i;
	for(i = 0; i < 0x34; i += 4) {
		// For each vector, check it is of the form:
		// 0x0C 0x94 0xWX 0xYZ  ; JMP 0xWXYZ
		if(base[i] != 0x0c) {
#ifdef _DEBUG_VALD
			traceln("Vald: Validation failed at address ");
			tracenum(i);
			trace(" with ");
			tracenum(base[i]);
			trace(" instead of 0x0C");
#endif
			return(0);
		}
		if(base[i + 1] != 0x94) {
#ifdef _DEBUG_VALD
			traceln("Vald: Validation failed at address ");
			tracenum(i + 1);
			trace(" with ");
			tracenum(base[i + 1]);
			trace(" instead of 0x94");
#endif
			return(0);
		}
	}
#ifdef _DEBUG_VALD
	traceln("Vald: Valid image");
#endif
	return(1);
}
