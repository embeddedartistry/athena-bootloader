#ifndef announce_h
#define announce_h

#define ANNOUNCE_PORT (5555)
#define ANNOUNCE_DATA_SIZE    128
#define bootloaderIdentStringLength (12)

#ifdef _ANNOUNCE
void announceInit();
void announcePoll();
#endif

#endif
