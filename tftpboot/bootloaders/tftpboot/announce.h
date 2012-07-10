#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <string.h>

#define ANNOUNCE_PORT (5555)
#define ANNOUNCE_DATA_SIZE    128
#define bootloaderIdentStringLength (12)

#ifdef _ANNOUNCE
void announceInit();
void announcePoll();
#endif